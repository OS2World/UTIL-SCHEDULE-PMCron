/* Copyright (c) 1995 Florian Groáe-Coosmann, RCS section at the eof         */
/* This module holds the main stuff of the server process.                   */
/* All functions depending on the frontend type are stored either in         */
/* cron_det.c or cron_pm.c, little exception: main at the end of this module.*/
#define INCL_NOCOMMON
#define INCL_WINSHELLDATA
#define INCL_WINWINDOWMGR
#define INCL_WINMESSAGEMGR
#define INCL_WINFRAMEMGR
#define INCL_WINSYS

#define INCL_DOSSEMAPHORES
#define INCL_DOSRESOURCES
#define INCL_DOSERRORS
#define INCL_DOSMISC
#define INCL_DOSFILEMGR
#define INCL_DOSPROCESS
#define INCL_DOSMODULEMGR
#define INCL_DOSNLS

#include <os2.h>

#include <string.h>
#include <stddef.h>
#include <io.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <fnmatch.h>
#include <ctype.h>
#include <locale.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "server.h"

#ifndef DATEFMT_MM_DD_YY
#  define DATEFMT_MM_DD_YY  0x0000
#  define DATEFMT_DD_MM_YY  0x0001
#  define DATEFMT_YY_MM_DD  0x0002
#endif

#define ALL_FILES (FILE_NORMAL | FILE_READONLY | FILE_HIDDEN | FILE_SYSTEM | \
                   FILE_DIRECTORY | FILE_ARCHIVED)

                                        /* these values are missed in the    */
                                        /* os2emx.h file, token from bsedos.h*/
#ifndef PT_FULLSCREEN
#  define PT_FULLSCREEN          0      /* Full screen application           */
#  define PT_REALMODE            1      /* Real mode process                 */
#  define PT_WINDOWABLEVIO       2      /* VIO windowable application        */
#  define PT_PM                  3      /* Presentation Manager application  */
#  define PT_DETACHED            4      /* Detached application              */
#endif


HAB hab;                                /* the following four values are for */
HMQ hmq;                                /* PM only                           */
HWND hwndFrame = (HWND) 0;
HWND hwndClient = (HWND) 0;

char   CurrFilename[CCHMAXPATH] = "???";/* user modifiable values            */
RECTL  FrameRect,SaveFrameRect;         /* PM only                           */
ULONG  ProgramFlags;
ULONG  Threshold;
UCHAR  MaxOutputs;
UCHAR  CurrOutput;
CHAR   Crontabs[CCHMAXPATH];
CHAR   TCPIP_Allow[CCHMAXPATH];
CHAR   OutputPath[CCHMAXPATH];
USHORT Language;
USHORT CurrentLanguage;

const LANGUAGE Languages[] = {
   {ENGLISH,   LC_C_UK,       "English"},
   {ENGLISH,   LC_C_USA,      "English"},
   {GERMAN,    LC_C_GERMANY,  "Deutsch"},
   {FRENCH,    LC_C_FRANCE,   "Fran‡ais"}
};
const unsigned LanguagesCount = sizeof(Languages) / sizeof(Languages[0]);

LONG  Colors[4];                        /* used colors, see codes in server.h*/
                                        /* only used when running under PM   */
char Fontname[2 * FACESIZE + sizeof("10.")]; /* INI-file saveable font name  */
ULONG LastDayStarted = 0;               /* last day crond has run            */

int   GlobalStop = 0;                   /* flag: shall we stop the program?  */
int   TCPIPAvail = 0;                   /* is TCP/IP available?              */
int   RunningUnderPM = 0;               /* the name of the flag says it      */
ULONG ReWritePrfFlags = 0;              /* flags what has changed            */
CHAR  DefIOPath[CCHMAXPATH];            /* path of the program               */
char  Delims[] = "\\/:";                /* path delimiter                    */
FILE *out = NULL;                       /* current output file               */
int   cronhandle = -1;                  /* the handle of the Crontabs file   */
HEV   CronSem = (HEV) 0;                /* event semaphore to show the       */
                                        /* cron daemon that things have been */
                                        /* changed                           */

#ifdef DEBUG
FILE *DebugFile = NULL;
#endif
                                        /* front end related values:         */
void (*NewProgramStatus)(ULONG NewStatus);   /* show new program state       */
void (*NewStartTime)(time_t time);      /* show next job start time          */
void (*NewOutputFile)(void);            /* show the display output file      */
void (*JobsModified)(void);             /* advise a changed job list         */
void (*StopPM)(void);                   /* stop PM window                    */
void (*Error)(const char *s,HWND dummy);  /* show error string               */
void (*FatalError)(const char *s,HWND dummy);   /* show error string and die */
void (*Stop)(int code);                 /* signal handler: SIGTERM, ^C,^Break*/
int SetJumpEnabled = 0;                 /* flag: can we use longjmp          */
jmp_buf PrgEnd;                         /* jump if signaled                  */


static char *szUNKNOWN = "There was an access to an unknown string.\r\n"
                         "Please, inform the author, thank you.\r\n";
static const char PrfDaemon[] = "Cron Daemon";
static const char PrfScreen[] = "Screen Position";
static const char PrfFlags[] = "Flags";
static const char PrfNumMax[] = "Maximum Output";
static const char PrfNumAct[] = "Current Output";
static const char PrfThreshold[] = "Threshold";
static const char PrfOutput[] = "Outputpath";
static const char PrfTCPIPAllow[] = "TCP/IP Allow File";
static const char PrfCrontabs[] = "Crontabs";
static const char PrfLanguage[] = "Language";
static const char PrfColors[] = "Colors";
static const char PrfFont[] = "Font";
static const char PrfDate[] = "Date";
static const char CronDaemonClass[] = "Cron-Daemon-Class";
static TID CronTid = (TID) -1;
static HMTX WriteSem = (HMTX) 0;
static RECTL ScreenRect;

static char FilenameMaskSearch[] = "Cron000?.out";
#define SEARCH_DIGIT_POS (sizeof("Cron000") - 1)
static char FilenameMaskSet[] = "Cron000%u.out";

/*****************************************************************************/
/*  function name : Get                                                      */
/*                                                                           */
/*  arguments     : ID of a resource string, see IDS_... in cron_id.h        */
/*                                                                           */
/*  return value  : the resource string or a default string, never NULL      */
/*                                                                           */
/*  description   : Gets a resource string from the resource file appendix   */
/*                  of the program file. All resource strings will be        */
/*                  buffered. Thus, there are only some read operations      */
/*                  while running. You should not change the returned        */
/*                  string.                                                  */
/*                  The Language specifier is used to get the locale         */
/*                  specific string. You must not specify it.                */
/*                                                                           */
/*  note          : Only the strings between the                             */
/*                  IDS_firstnumber_mustbe_first_IDS and the                 */
/*                  IDS_lastnumber_mustbe_last_IDS will be loaded.           */
/*                  Otherwise, the function returns the default string.      */
/*****************************************************************************/
const char *Get(ULONG IDS)
                                        /* string resources are clustered in */
                                        /* 16 element portions, we'll buffer */
                                        /* all elements of a buffer. Thus we */
                                        /* need more space and have to align */
                                        /* the ID numbers.                   */
                                        /* compare these values with the IDs */
                                        /* found in cron_id.h                */
#define FIRSTVALID (IDS_Title & 0xFFFFFFF0)
#define LASTVALID  (IDS_lastnumber_mustbe_last_IDS | 0xF)
{
   ULONG err,AssignNum,LoadID;
   char *buf;
   unsigned char len;
   unsigned i;

   static char *StringTable[LASTVALID - FIRSTVALID + 1]; /*buffer for strings*/
   static int first = 1;

   if (first) {                         /* initialize the items to NULL      */
      memset(StringTable,0,sizeof(StringTable));
      first = 0;
   }

   if ((IDS < FIRSTVALID) ||            /* no chance to find the string in   */
       (IDS > LASTVALID))               /* the resource                      */
      return(szUNKNOWN);
                                        /* it must be in the resource file   */
   IDS -= FIRSTVALID;                   /* align ID to the first element no. */
   if (StringTable[IDS] != NULL)        /* element in buffer?                */
      return(StringTable[IDS]);
                                        /* unknown string, try to load       */
                                        /* resource ID for the system        */
   LoadID = ((IDS + FIRSTVALID + CurrentLanguage) / 16) + 1;
   AssignNum = IDS & 0xFFFFFFF0;        /* first element in the cluster      */
   if ((err = DosGetResource((HMODULE) 0, /* get the resource, the system    */
                             RT_STRING, /* will allocate a new buffer for us */
                             LoadID,
                             (PPVOID) &buf)) == 0) {
      buf += 2;                         /* there is an USHORT at the begin of*/
                                        /* the resource (I think so :-( )    */
      for (i = 0;i < 16;i++) {
         len = *((unsigned char *) buf++);   /* take the length (UCHAR)      */
         StringTable[AssignNum + i] = buf;   /* assign buffer element        */
         buf += len;                    /* add the length                    */
      }
   } else                               /* resource not loaded?              */
      for (i = 0;i < 16;i++)            /* apply default string              */
         StringTable[AssignNum + i] = szUNKNOWN;
   if (StringTable[IDS] == NULL)        /* should never happen...            */
      StringTable[IDS] = szUNKNOWN;
   return(StringTable[IDS]);
#undef LASTVALID
#undef FIRSTVALID
}

/*****************************************************************************/
/*  function name : GetTime                                                  */
/*                                                                           */
/*  arguments     : pointer to a buffer, the length of the buffer und a      */
/*                  time.                                                    */
/*                                                                           */
/*  description   : puts the date and time of the third argument into the    */
/*                  buffer in the default form of the current country.       */
/*                  The country info is gotten from the system on first      */
/*                  time.                                                    */
/*****************************************************************************/
void GetTime(char *buf,size_t bufsize,time_t t)
{
   static char formatstring[30] = "";
   if (formatstring[0] == 0) {
      COUNTRYCODE cc = {0,0};
      COUNTRYINFO ci;
      ULONG done;
      char *date;
      char *time;
      strcpy(formatstring,"%d.%m.%Y %H:%M:%S"); /* default for Germany and   */
                                        /* other european states             */
      if ((DosQueryCtryInfo(sizeof(ci),&cc,&ci,&done) != 0) &&
          (done >= offsetof(COUNTRYINFO,fsTimeFmt) + sizeof(ci.fsTimeFmt))) {
         if (ci.fsDateFmt == DATEFMT_MM_DD_YY)
            date = "%%m%s%%d%s%%Y ";    /* e.g. %m.%d.%Y                     */
         else if (ci.fsDateFmt == DATEFMT_YY_MM_DD)
            date = "%%Y%s%%m%s%%d ";
         else
            date = "%%d%s%%m%s%%Y ";
         if (ci.fsTimeFmt)              /* 24 hour clock                     */
            time = "%%H%s%%M%s%%S";
         else
            time = "%%I%s%%M%s%%S%%p";
         sprintf(formatstring,
                 date,
                 ci.szDateSeparator,
                 ci.szDateSeparator,
                 ci.szDateSeparator);
         sprintf(formatstring + strlen(formatstring),
                 time,
                 ci.szTimeSeparator,
                 ci.szTimeSeparator,
                 ci.szTimeSeparator);
      }
   }
   strftime(buf,bufsize,formatstring,localtime(&t));
}

/*****************************************************************************/
/*  function name : GetTimeString                                            */
/*                                                                           */
/*  return value  : pointer to a static buffer holding the current date      */
/*                  and time string with " : " appended.                     */
/*                                                                           */
/*  description   : see return value. You should not change the return       */
/*                  value.                                                   */
/*****************************************************************************/
const char *GetTimeString(void)
{
   time_t t;
   static char buf[40];
   time(&t);
   GetTime(buf,sizeof(buf) - 4,t);
   strcat(buf," : ");
   return(buf);
}

/*****************************************************************************/
/*  function name : BlockOutput                                              */
/*                                                                           */
/*  description   : the calling thread gets the exclusive access to the      */
/*                  current output file.                                     */
/*                                                                           */
/*  note          : call UnBlockOutput as soon as possible                   */
/*****************************************************************************/
void BlockOutput(void)
{
   ULONG err;
   if (WriteSem == (HMTX) 0)            /* not yet allocated? OK             */
      return;
   do {
      err = DosRequestMutexSem(WriteSem,SEM_INDEFINITE_WAIT);
   } while ((err != 0) && (err != ERROR_INTERRUPT));
}

/*****************************************************************************/
/*  function name : UnBlockOutput                                            */
/*                                                                           */
/*  description   : releases the exclusive access to the current output      */
/*                  file.                                                    */
/*                                                                           */
/*  note          : should only called by the thread that has called         */
/*                  BlockOutput                                              */
/*****************************************************************************/
void UnBlockOutput(void)
{
   DosReleaseMutexSem(WriteSem);
}

/*****************************************************************************/
/*  function name : Message                                                  */
/*                                                                           */
/*  arguments     : see printf                                               */
/*                                                                           */
/*  description   : prints the date and time and the message to the          */
/*                  current output file. The written data is flushed to      */
/*                  disk physically. Is the output file grows beyond the     */
/*                  threshold a new output file is generated.                */
/*****************************************************************************/
void Message(const char *fmt,...)
{
   va_list marker;
   if ((ProgramFlags & PRG_OUTPUT_IS_NUL) || (out == NULL)) /* nothing to do */
      return;
   BlockOutput();
   if (filelength(fileno(out)) >= Threshold)
      NextOutputFile(0);
   fputs(GetTimeString(),out);
   va_start(marker,fmt);
   vfprintf(out,fmt,marker);
   va_end(marker);
   fflush(out);
   UnBlockOutput();
}

/*****************************************************************************/
/*  function name : GetError                                                 */
/*                                                                           */
/*  arguments     : errno code                                               */
/*                                                                           */
/*  return value  : the appropriate errno string                             */
/*                                                                           */
/*  description   : resolves the current errno to a string. The errno        */
/*                  string is not terminated by a newline. You should not    */
/*                  change the return value.                                 */
/*****************************************************************************/
const char *GetError(int errnocode)
{
   static char strerrorbuf[128];
   char *ptr;
   ptr = strerror(errnocode);
   if (strlen(ptr) >= sizeof(strerrorbuf))
      memcpy(strerrorbuf,ptr,sizeof(strerrorbuf));
   else
      strcpy(strerrorbuf,ptr);
   strerrorbuf[sizeof(strerrorbuf) - 1] = 0;
   if ((ptr = strpbrk(strerrorbuf,"\r\n")) != NULL)
      *ptr = 0;
   return(strerrorbuf);
}

/*****************************************************************************/
/*  function name : Perror                                                   */
/*                                                                           */
/*  arguments     : see perror                                               */
/*                                                                           */
/*  description   : see perror but the output is preceeded by the current    */
/*                  date and time and is written to the current output       */
/*                  file.                                                    */
/*****************************************************************************/
void Perror(const char *s)
{
   if ((s != NULL) && (*s != '\0'))
      Message("%s: %s\n",
              s,
              GetError(errno));
   else
      Message("%s\n",GetError(errno));
}

/*****************************************************************************/
/*  function name : ResolvOS2Err                                             */
/*                                                                           */
/*  arguments     : OS/2 error code, NULL terminated list of strings         */
/*                  describing the error conditions or files.                */
/*                                                                           */
/*  return value  : the appropriate error string                             */
/*                                                                           */
/*  description   : resolves the error code to a string. The string is       */
/*                  generated by the system. You should not change the       */
/*                  return value.                                            */
/*****************************************************************************/
const char *ResolvOS2Err(ULONG msgno,const char *name,...)
{
   static CHAR msgbuf[513];             /* should be enough                  */
   CHAR *names[10];                     /* maximum by system                 */
   ULONG count = 0;
   ULONG done;
   va_list marker;
   va_start(marker,name);
   while ((count < 10) && (name != NULL)) {
      names[count++] = (CHAR *) name;
      name = va_arg(marker,CHAR *);
   }
   va_end(marker);
   msgbuf[0] = 0;                       /* good default in case of an error  */
   DosGetMessage(names,count,msgbuf,sizeof(msgbuf),msgno,"oso001.msg",&done);
   return((char *) msgbuf);
}

/*****************************************************************************/
/*  function name : ChildDies                                                */
/*                                                                           */
/*  arguments     : signal number generated by the runtime system            */
/*                                                                           */
/*  description   : This is a signal routine. This routine is called by a    */
/*                  SIGCHLD. It looks for the child which has died and       */
/*                  calls EndProcess with the pid of the child. Then,        */
/*                  the cron thread is awaken to reap the child.             */
/*                                                                           */
/*  note          : don't call directly, thread save?                        */
/*****************************************************************************/
void ChildDies(int code)
{
   int status;
   int pid;
   while ((pid = waitpid(-1,&status,WNOHANG)) > 0) {  /* never block!        */
      EndProcess(pid,
                 WIFEXITED(status) ? 0 : WTERMSIG(status),
                 WEXITSTATUS(status));
   }
   signal(code,SIG_ACK);
   DosPostEventSem(CronSem);
}

/*****************************************************************************/
/*  function name : PipeErr                                                  */
/*                                                                           */
/*  arguments     : signal number generated by the runtime system            */
/*                                                                           */
/*  description   : This is a signal routine. This routine is called by a    */
/*                  SIG_PIPE. The signal is ignored.                         */
/*                                                                           */
/*  note          : don't call directly. We don't set signal(SIGPIPE,        */
/*                  SIG_IGN) because the pipe thread should awake and        */
/*                  get an ERROR_INTERRUPT by the system to reconnect the    */
/*                  pipe.                                                    */
/*****************************************************************************/
void PipeErr(int code)
{
   signal(code,SIG_ACK);
}

/*****************************************************************************/
/*  function name : FullName                                                 */
/*                                                                           */
/*  arguments     : path or filename                                         */
/*                                                                           */
/*  description   : generates the fully qualified pathname of the            */
/*                  argument. If an error occures the buffer is unchanged.   */
/*                                                                           */
/*  note          : the buffer Filename should be big enough, use a size     */
/*                  of CCHMAXPATH.                                           */
/*****************************************************************************/
void FullName(char *Filename)
{
   size_t len;
   char buf[CCHMAXPATH];
   if (strlen(Filename) == 0)           /* argument == "" ? assume current   */
      strcpy(Filename,".");             /* directory                         */

   if (DosQueryPathInfo(Filename,
                        FIL_QUERYFULLNAME,
                        buf,
                        sizeof(buf)) == 0) {
      if (strnicmp(buf,"\\DEV\\",5) == 0) {  /* don't expand character device*/
         strcpy(Filename,buf + 5);      /* names, instead, eat up the leading*/
         return;                        /* '\DEV\'                           */
      }
      strcpy(Filename,buf);
   } else
      return;
   if ((len = strlen(Filename)) == 0)   /* should never happen...            */
      return;

   if ((len == 3) && (Filename[1] == ':'))   /* don't cut the trailing back- */
      return;                           /* slash of "?:\"                    */
   if (Filename[len - 1] == '\\')       /* cut trailing backslash            */
      Filename[len - 1] = '\0';
}

/*****************************************************************************/
/*  function name : AppendPathComponent                                      */
/*                                                                           */
/*  arguments     : pathname and a new component of the path or filename     */
/*                                                                           */
/*  description   : append to the first argument the second one. Delimits    */
/*                  them by a backslash if needed.                           */
/*                                                                           */
/*  note          : the first buffer should be big enough, use a size of     */
/*                  CCHMAXPATH.                                              */
/*****************************************************************************/
void AppendPathComponent(char *path,const char *component)
{
   size_t len = strlen(path);
   if (len == 0)
      strcpy(path,component);
   else if (strchr(Delims,path[len - 1]) != NULL)  /* Be careful! suppose    */
      strcat(path,component);           /* path == "A:", component == "file" */
   else {
      strcat(path,"\\");
      strcat(path,component);
   }
}

/*****************************************************************************/
/*  function name : CheckOutputPath                                          */
/*                                                                           */
/*  arguments     : pathname                                                 */
/*                                                                           */
/*  return value  : 1, if the pathname looks fine to be an output path,      */
/*                  0 if not.                                                */
/*                                                                           */
/*  description   : checks wether the argument is a directory and doesn't    */
/*                  has nondestroyable entries of the form "Cron000?.out".   */
/*****************************************************************************/
int CheckOutputPath(char *s)
{
   DIR *dir;
   struct dirent *ent;
   int retval = 1;
   char buf[CCHMAXPATH];
   if (strlen(s) >= CCHMAXPATH - 14)
      return(0);
   FullName(s);
   strcpy(buf,s);
   if ((dir = opendir(buf)) == NULL)    /* no directory?                     */
      return(0);
   while ((ent = readdir(dir)) != NULL) {
      if (fnmatch(FilenameMaskSearch,ent->d_name,_FNM_OS2 | _FNM_IGNORECASE)
                                                                          != 0)
         continue;
      if (!isdigit(ent->d_name[SEARCH_DIGIT_POS]))
         continue;
      if (ent->d_attr & (A_RONLY | A_HIDDEN | A_SYSTEM | A_DIR)) {
         retval = 0;
         break;
      }
   }
   closedir(dir);
   return(retval);
}

/*****************************************************************************/
/*  function name : NextOutputFile                                           */
/*                                                                           */
/*  arguments     : flag, if the output file must be opened in append mode   */
/*                                                                           */
/*  return value  : handle of the new output file,-1 on error                */
/*                                                                           */
/*  description   : opens a new (or old) output file. The function uses      */
/*                  the variables OutputPath, CurrOutput und MaxOutputs to   */
/*                  determine the correct filename. New data is appended     */
/*                  to the current output file, if Append is set. This is    */
/*                  the normal behavior while starting the program. While    */
/*                  running and changing the output file all data in the     */
/*                  new output file is destroyed.                            */
/*                  If the program flag PRG_OUTPUT_IS_NUL is set the         */
/*                  output file is closed und 0 is returned.                 */
/*                  This function takes care of the Threshold.               */
/*                                                                           */
/*  note          : Please, don't use the returned handle, use the global    */
/*                  variable out instead.                                    */
/*****************************************************************************/
int NextOutputFile(int Append)
{
   int handle;
   int AddFlags;
   FILE *file,*hlpfile;
   unsigned short i,done;
   char Filename[CCHMAXPATH];
   char *ptr;

   if (ProgramFlags & PRG_OUTPUT_IS_NUL) {   /* ignore all output?           */
      if (out != NULL)
         fclose(out);
      out = NULL;
      strcpy(CurrFilename,"nul");
      NewOutputFile();
      return(0);
   }

   if (Append)                          /* Open in append mode?              */
      AddFlags = O_APPEND;
   else
      AddFlags = O_TRUNC | O_APPEND;    /* Yes, we append too, but we trunc  */
                                        /* the file before                   */
   i = CurrOutput;                      /* current output file number        */
   if (!Append)                         /* new file? use valid next number   */
      if (++i >= MaxOutputs)
         i = 0;
   strcpy(Filename,OutputPath);
   AppendPathComponent(Filename,"?");   /* place a dummy as filename into buf*/
   ptr = Filename + strlen(Filename) - 1; /* thus we avoid problems with     */
                                        /* paths with/without trailing back- */
                                        /* slashs                            */
   for (done = 0;done < MaxOutputs;done++) { /* step through all valid names */
      sprintf(ptr,FilenameMaskSet,i);
      if ((handle = sopen(Filename,
                          AddFlags | O_CREAT | O_TEXT | O_WRONLY |
                                                       O_SYNC | O_NOINHERIT,
                          SH_DENYWR,
                          S_IREAD | S_IWRITE)) != -1) {
         if ((filelength(handle) >= Threshold) ||  /* append mode?           */
             ((file = fdopen(handle,"at")) == NULL)) {   /* we need a FILE   */
            close(handle);
            if (++i >= MaxOutputs)
               i = 0;
            AddFlags = O_TRUNC | O_APPEND;   /* trunc next file              */
            continue;
         }
         fseek(file,0,SEEK_END);        /* there are problems with fdopen    */
         if (i != CurrOutput) {         /* New output number?                */
            CurrOutput = (UCHAR) i;
            ReWritePrfFlags |= REWRITE_CURRENT;
            ReWritePrf();
         }
         hlpfile = out;                 /* save old file, don't ever use     */
         out = file;                    /* BlockOutput, thus make the change */
                                        /* atomically                        */
         if (hlpfile != NULL)
            fclose(hlpfile);
         strcpy(CurrFilename,Filename);
         NewOutputFile();
         return(handle);
      }
      if (++i >= MaxOutputs)            /* error opening file, check next    */
         i = 0;
      AddFlags = O_TRUNC | O_APPEND;    /* trunc next file                   */
   }
   return(-1);
}

/*****************************************************************************/
/*  function name : ReadCrontabs                                             */
/*                                                                           */
/*  arguments     : new name of the Crontabs file                            */
/*                                                                           */
/*  return value  : 1 on success, 0 otherwise                                */
/*                                                                           */
/*  description   : reads a Crontabs file and builds a complete new          */
/*                  joblist. If this is done sucessfully, the default list   */
/*                  is set to the new one and the old one is deleted.        */
/*                  The cron thread is informed of the new list.             */
/*                                                                           */
/*  note          : only "nul" and regular files are allowed.                */
/*                  In opposite of the unix version we have permanent        */
/*                  access to the file. This is the right manner! Now we     */
/*                  can sleep for a long time and we don't need to reread    */
/*                  the Crontabs file every minute thus saving processor     */
/*                  time.                                                    */
/*****************************************************************************/
int ReReadCrontabs(char *s)
{
   LIST_ENTRY NewHead, oldhead;
   int handle,oldhandle,htype = HT_DEV_OTHER;

   if ((handle = sopen(s,
                       O_BINARY | O_RDWR | O_CREAT | O_NOINHERIT,SH_DENYWR,
                       S_IREAD | S_IWRITE)) == -1)
      return(0);
   ioctl(handle,FGETHTYPE,&htype);
   if ((htype != HT_FILE) && (htype != HT_DEV_NUL)) { /* not "nul" or regular*/
      close(handle);                    /* file? We won't read "KBD$" !!!    */
      return(0);
   }
   memset(&NewHead,0,sizeof(NewHead));  /* build the new list                */
   if (ReadInFile(handle,&NewHead) != 0) {   /* error interpreting file?     */
      close(handle);
      return(0);
   }

   BlockProcess();                      /* we want to change the joblist     */
   oldhandle = cronhandle;              /* save old settings                 */
   oldhead = ListHead;
   cronhandle = handle;                 /* setup the newer ones              */
   ListHead = NewHead;
   UnBlockProcess();                    /* OK, delete old resources but first*/
                                        /* tell the cron thread to compute   */
   if (CronSem != (HEV) 0)              /* the next starting time...         */
      DosPostEventSem(CronSem);
   JobsModified();                      /* the list is modified              */

   if (oldhandle != -1)                 /* close the old list                */
      close(oldhandle);
   while (DeleteListEntry(oldhead.next,&oldhead) == 0)   /* delete the       */
      ;                                 /* complete old list                 */
   return(1);
}


/*****************************************************************************/
/*  function name : ReadPrf                                                  */
/*                                                                           */
/*  arguments     : name of the profile subentry (see Prf...[] at top)       */
/*                  buffer for data                                          */
/*                  minimum length of data to accept                         */
/*                  maximum length of data to accept                         */
/*                                                                           */
/*  return value  : size of read data or 0 on error                          */
/*                                                                           */
/*  description   : reads from getenv(USER_INI) the supplied subentry of     */
/*                  the entry PrfDaemon. It only accepts data lengths in a   */
/*                  distict intervall.                                       */
/*                                                                           */
/*  note          : don't call direct, use ReadProfileData instead           */
/*****************************************************************************/
static ULONG ReadPrf(PCSZ Itemname,VOID *buf,ULONG minsize,ULONG maxsize)
{
   ULONG size = maxsize;
   if (!PrfQueryProfileData(HINI_USERPROFILE,
                            PrfDaemon,
                            Itemname,
                            buf,
                            &size))
      return(0);
   if (size < minsize)
      return(0);
   return(size);
}

/*****************************************************************************/
/*  function name : WritePrf                                                 */
/*                                                                           */
/*  arguments     : name of the profile subentry (see Prf...[] at top)       */
/*                  buffer of data                                           */
/*                  length of buffer                                         */
/*                                                                           */
/*  return value  : 1 on success, 0 on error                                 */
/*                                                                           */
/*  description   : writes to getenv(USER_INI) the supplied subentry of      */
/*                  the entry PrfDaemon.                                     */
/*****************************************************************************/
static int WritePrf(PCSZ Itemname,VOID *buf,ULONG size)
{
   if (PrfWriteProfileData(HINI_USERPROFILE,
                           PrfDaemon,
                           Itemname,
                           buf,
                           size) == TRUE)
      return(1);
   return(0);
}

/*****************************************************************************/
/*  function name : ReWriteOnePrf                                            */
/*                                                                           */
/*  arguments     : mask of the profile subentry which has to be written,    */
/*                  see REWRITE_... flags in the header file                 */
/*                                                                           */
/*  return value  : 1 if successful, 0 otherwise                             */
/*                                                                           */
/*  description   : writes one entry of the profile data to the              */
/*                  getenv(USER_INI) file.                                   */
/*                                                                           */
/*  note          : don't call direct, use ReWritePrf instead.               */
/*****************************************************************************/
static int ReWriteOnePrf(ULONG num)
{
   switch (num) {
      case REWRITE_SCREEN:              /* PM only                           */
         if (RunningUnderPM)
            return(WritePrf(PrfScreen,&FrameRect,sizeof(FrameRect)));
         else
            return(1);
      case REWRITE_FLAGS:
         return(WritePrf(PrfFlags,&ProgramFlags,sizeof(ProgramFlags)));
      case REWRITE_MAXIMUM:
         return(WritePrf(PrfNumMax,&MaxOutputs,sizeof(MaxOutputs)));
      case REWRITE_CURRENT:
         return(WritePrf(PrfNumAct,&CurrOutput,sizeof(CurrOutput)));
      case REWRITE_THRESHOLD:
         return(WritePrf(PrfThreshold,&Threshold,sizeof(Threshold)));
      case REWRITE_OUTPUTPATH:
         return(WritePrf(PrfOutput,OutputPath,strlen(OutputPath) + 1));
      case REWRITE_TCPIP_ALLOW:
         return(WritePrf(PrfTCPIPAllow,TCPIP_Allow,strlen(TCPIP_Allow) + 1));
      case REWRITE_CRONFILENAME:
         return(WritePrf(PrfCrontabs,Crontabs,strlen(Crontabs) + 1));
      case REWRITE_LANGUAGE:
         return(WritePrf(PrfLanguage,&Language,sizeof(Language)));
      case REWRITE_COLORS:
         return(WritePrf(PrfColors,&Colors,sizeof(Colors)));
      case REWRITE_FONT:
         return(WritePrf(PrfFont,Fontname,strlen(Fontname) + 1));
      case REWRITE_DATE:
         return(WritePrf(PrfDate,&LastDayStarted,sizeof(LastDayStarted)));
   }
   return(1);                           /* default is success since some     */
}                                       /* possible flags are not used       */

/*****************************************************************************/
/*  function name : ReWritePrf                                               */
/*                                                                           */
/*  return value  : 1 if successful, 0 otherwise                             */
/*                                                                           */
/*  description   : writes all changed profile datas to the                  */
/*                  getenv(USER_INI) file. This function should be called    */
/*                  if any of the profile data changes. Maybe this           */
/*                  function should be called periodically if one of the     */
/*                  ReWritePrfFlags is set. After writing the flags are      */
/*                  updated avoiding write overhead.                         */
/*****************************************************************************/
int ReWritePrf(void)
{
   int retval = 1;                      /* assume no errors                  */
   ULONG i;
   for (i = 1ul;i != 0;i <<= 1)
      if (ReWritePrfFlags & i) {
         if (!ReWriteOnePrf(i))
            retval = 0;
         else
            ReWritePrfFlags &= ~i;
      }
   return(retval);
}

/*****************************************************************************/
/*  function name : ReadProfileData                                          */
/*                                                                           */
/*  description   : reads in all needed entries from the getenv(USER_INI)    */
/*                  file. If there are problems (never initialized,          */
/*                  invalid filenames, etc.) we set default values           */
/*                  allowing starting up the program.                        */
/*                  This function is quiet a bit awful. Be careful not to    */
/*                  run into race conditions!                                */
/*                                                                           */
/*  note          : this function is called once at start of program         */
/*****************************************************************************/
void ReadProfileData(void)
{
   int ok;
   if (RunningUnderPM) {
                                        /* query desktop size                */
      if (!WinQueryWindowRect(HWND_DESKTOP,&ScreenRect))
         FatalWinError((HWND) 0);
                                        /* query frame size                  */
      if (!ReadPrf(PrfScreen,&FrameRect,sizeof(FrameRect),sizeof(FrameRect))) {
                                        /* center the window sized (300,100) */
         FrameRect.xRight = 300;
         FrameRect.yTop = 100;
         FrameRect.xLeft = (ScreenRect.xRight - 300) / 2;
         FrameRect.yBottom = (ScreenRect.yTop - 100) / 2;
         ReWritePrfFlags |= REWRITE_SCREEN;
      } else {
                                        /* put window into the visible region*/
         if (FrameRect.xLeft < -5)      /* little tolerance                  */
            FrameRect.xLeft = 0;
         if (FrameRect.yBottom < -5)
            FrameRect.yBottom = 0;
         if (FrameRect.xLeft + FrameRect.xRight > ScreenRect.xRight + 5)
            FrameRect.xLeft = ScreenRect.xRight - FrameRect.xRight;
         if (FrameRect.yBottom + FrameRect.yTop > ScreenRect.yTop + 5)
            FrameRect.yBottom = ScreenRect.yTop - FrameRect.yTop;
      }
      SaveFrameRect = FrameRect;

      if (!ReadPrf(PrfColors,Colors,sizeof(Colors),sizeof(Colors))) {
         Colors[MYCLR_SYSFOREGROUND] = Colors[MYCLR_FOREGROUND] =
                            WinQuerySysColor(HWND_DESKTOP,SYSCLR_WINDOWTEXT,0);
         Colors[MYCLR_SYSBACKGROUND] = Colors[MYCLR_BACKGROUND] =
                                WinQuerySysColor(HWND_DESKTOP,SYSCLR_WINDOW,0);
         ReWritePrfFlags |= REWRITE_COLORS;
      }
      if (!ReadPrf(PrfFont,Fontname,2,sizeof(Fontname)))
         strcpy(Fontname,"");
   }
                                        /* first try to read the flags       */
   if (!ReadPrf(PrfFlags,&ProgramFlags,sizeof(ProgramFlags),
                                                       sizeof(ProgramFlags))) {
      ReWritePrfFlags |= REWRITE_FLAGS;
      ProgramFlags = PRG_OUTPUT_IS_NUL; /* default: avoid output files       */
   } else
      ProgramFlags &= PRG_ALLFLAGS;     /* kill illegal flags                */

   if (!ReadPrf(PrfCrontabs,Crontabs,2,sizeof(Crontabs))) { /* read Crontabs */
      ReWritePrfFlags |= REWRITE_CRONFILENAME;  /* name                      */
      strcpy(Crontabs,DefIOPath);       /* this is the default path          */
      AppendPathComponent(Crontabs,"Crontabs");
   }
   if (!ReReadCrontabs(Crontabs)) {    /* error reading Crontabs file?       */
      ok = 0;
      if ((ReWritePrfFlags & REWRITE_CRONFILENAME) == 0) {
         strcpy(Crontabs,DefIOPath);    /* perhaps a destroyed Crontabs      */
         AppendPathComponent(Crontabs,"Crontabs");
         if (ReReadCrontabs(Crontabs)) {
            Error(Get(IDS_DefaultCrontabsFile),(HWND) 0);
            ok = 1;
         }
      }
      if (!ok) {                        /* Crontabs still bad?               */
         strcpy(Crontabs,"nul");
         if (!ReReadCrontabs(Crontabs))
            FatalError(Get(IDS_NoCrontabsFile),(HWND) 0);
         else
            Error(Get(IDS_ErrCrontabsFile),(HWND) 0);
      }
   }
   if ((!ReadPrf(PrfNumMax,&MaxOutputs,sizeof(MaxOutputs),sizeof(MaxOutputs)))||
       ((MaxOutputs > 10) || (MaxOutputs < 2))) {
      ReWritePrfFlags |= REWRITE_MAXIMUM;
      MaxOutputs = 10;
   }
   if ((!ReadPrf(PrfNumAct,&CurrOutput,sizeof(CurrOutput),sizeof(CurrOutput)))||
       (CurrOutput > MaxOutputs)) {
      ReWritePrfFlags |= REWRITE_CURRENT;
      CurrOutput = 0;
   }
   if (!ReadPrf(PrfThreshold,&Threshold,sizeof(Threshold),sizeof(Threshold))) {
      ReWritePrfFlags |= REWRITE_THRESHOLD;
      Threshold = 0x4000;
   }
                                        /* we need the above values! don't   */
                                        /* set the following more to the top */
                                        /* of the file                       */
   if (!ReadPrf(PrfOutput,OutputPath,2,sizeof(OutputPath))) {
      ReWritePrfFlags |= REWRITE_OUTPUTPATH;
      strcpy(OutputPath,DefIOPath);
   }
   if (!CheckOutputPath(OutputPath)) {  /* output path not acceptable?       */
      if (ReWritePrfFlags & REWRITE_OUTPUTPATH) /* already default path?     */
         Error(Get(IDS_NoOutputFile),(HWND) 0); /* no output                 */
      else {
         ReWritePrfFlags |= REWRITE_OUTPUTPATH;
         strcpy(OutputPath,DefIOPath);
         if (!CheckOutputPath(OutputPath))
            Error(Get(IDS_NoOutputFile),(HWND) 0);
         else
            Error(Get(IDS_DefaultOutput),(HWND) 0);
      }
   }
   BlockOutput();                       /* setup the output file             */
   if (NextOutputFile(1) == -1)         /* can't set any output file?        */
      Error(Get(IDS_NoOutputFile),(HWND) 0);
   UnBlockOutput();
   if (!ReadPrf(PrfTCPIPAllow,&TCPIP_Allow,2,sizeof(TCPIP_Allow))) {
      ReWritePrfFlags |= REWRITE_TCPIP_ALLOW;
      strcpy(TCPIP_Allow,"nul");
   }
   if (!ReadAllow(TCPIP_Allow)) {
      if (ReWritePrfFlags & REWRITE_TCPIP_ALLOW)   /* already default file   */
         Error(Get(IDS_NoTCPIPFile),(HWND) 0);
      else {
         ReWritePrfFlags |= REWRITE_TCPIP_ALLOW;
         strcpy(TCPIP_Allow,"nul");
         if (!ReadAllow(TCPIP_Allow))
            Error(Get(IDS_NoTCPIPFile),(HWND) 0);
         else
            Error(Get(IDS_DefaultTCPIPFile),(HWND) 0);
      }
   }

   if (!ReadPrf(PrfDate,&LastDayStarted,sizeof(LastDayStarted),
                                                     sizeof(LastDayStarted)))
      LastDayStarted = 0ul;             /* no need to write changes          */

   if (ReWritePrfFlags)                 /* rewrite changed settings          */
      ReWritePrf();
}

/*****************************************************************************/
/*  function name : PipeThread                                               */
/*                                                                           */
/*  arguments     : not needed, must be void *                               */
/*                                                                           */
/*  description   : this is a thread function. It processes all requests     */
/*                  to the already opened pipe.                              */
/*                  Stops running if GlobalStop is set. The function         */
/*                  requests an automatic disconnect by the cron thread      */
/*                  avoiding any user to block the pipe for a long time.     */
/*                                                                           */
/*  note          : should be called via _beginthread                        */
/*****************************************************************************/
void PipeThread(void *dummy)
{
   ULONG err;

   do {                                 /* while (!GlobalStop)               */
      if (GlobalStop)                   /* check at mostly every place...    */
         break;
      err = DosConnectNPipe(pipehandle);  /* wait for a client               */
      if (err) {                        /* maybe ERROR_INTERRUPT?            */
         if (GlobalStop)
            break;
         DosDisConnectNPipe(pipehandle);  /* maybe open/close, this must     */
                                        /* be done                           */
         continue;
      }
      NewProgramStatus(IDS_StatusCommunication);
      AutoDisConnect = time(NULL) + 60; /* let the pipe close after a minute */
      DosPostEventSem(CronSem);         /* cron thread shall kill the pipe   */
                                        /* after AutoDisConnect              */
      ProcessRequest(&pipehandle,PipeName);  /* do the work                  */
      if ((AutoDisConnect != (time_t) -1l) && (!GlobalStop))
         WaitClientDisConnect();        /* wait for the client to close the  */
                                        /* pipe. Otherwise, the client may   */
                                        /* loose some data send.             */
                                        /* ANSWER ME: Don't we need to wait  */
                                        /* for the client is we set the      */
                                        /* NP_NOWRITEBEHIND-Flag while       */
                                        /* creating the pipe?                */

      AutoDisConnect = (time_t) -1l;    /* don't let close the pipe          */
      NewProgramStatus(IDS_StatusNormal);
      if (GlobalStop)
         break;
      DosDisConnectNPipe(pipehandle);
      DosPostEventSem(CronSem);         /* inform the cron thread that there */
                                        /* is no need to close the pipe any  */
                                        /* longer                            */
   } while (!GlobalStop);
}

#ifdef DEBUG
/*****************************************************************************/
/*  function name : Dputs                                                    */
/*                                                                           */
/*  arguments     : see puts                                                 */
/*                                                                           */
/*  description   : writes the string to the debugfile in case of            */
/*                  debugging                                                */
/*                                                                           */
/*  note          : you can't use puts since stdout isn't a valid output     */
/*                  file. Use this function instead.                         */
/*****************************************************************************/
void Dputs(char *s)
{
   if (DebugFile != NULL)
      fprintf(DebugFile,"%s\n",s);
}

/*****************************************************************************/
/*  function name : Dprintf                                                  */
/*                                                                           */
/*  arguments     : see printf                                               */
/*                                                                           */
/*  description   : writes the data to the debugfile in case of              */
/*                  debugging                                                */
/*                                                                           */
/*  note          : you can't use printf since stdout isn't a valid          */
/*                  output file. Use this function instead.                  */
/*****************************************************************************/
void Dprintf(char *fmt,...)
{
   va_list marker;
   if (DebugFile == NULL)
      return;
   va_start(marker,fmt);
   vfprintf(DebugFile,fmt,marker);
   va_end(marker);
}
#endif

int main(int argc,char **argv)
{
   ULONG flFrameFlags;
   QMSG qmsg;
   ULONG id,err;
   static CHAR FullName[CCHMAXPATH];
   PTIB ptib;
   PPIB ppib;
   HPIPE hp;
   int i;
   char FailBuf[257],*ptr;
   static RESULTCODES res;

   if ((_emx_env & 0x0200) == 0) {      /* this must be done first           */
      fputs("This program can't run under DOS",stdout);
      return(-1);
   }

                                        /* are we running as a PM appl?      */
                                        /* we must set the signal handlers   */
                                        /* to the correct value              */
   if (DosGetInfoBlocks(&ptib,&ppib) != 0) { /* query module handle and pgm  */
      fputs("error calling DosGetInfoBlocks",stdout); /* type                */
      return(-1);
   }

   RunningUnderPM = (ppib->pib_ultype == PT_PM) ? 1 : 0;

   if (RunningUnderPM) {
      NewProgramStatus = PMNewProgramStatus;
      NewStartTime     = PMNewStartTime;
      NewOutputFile    = PMNewOutputFile;
      StopPM           = PMStopPM;
      Error            = PMError;
      FatalError       = PMFatalError;
      JobsModified     = PMJobsModified;
      Stop             = PMStop;
   } else {
      NewProgramStatus = DetachedNewProgramStatus;
      NewStartTime     = DetachedNewStartTime;
      NewOutputFile    = DetachedNewOutputFile;
      StopPM           = DetachedStopPM;
      Error            = DetachedError;
      FatalError       = DetachedFatalError;
      JobsModified     = DetachedJobsModified;
      Stop             = DetachedStop;
   }

   signal(SIGINT,Stop);
   signal(SIGBREAK,Stop);
   signal(SIGTERM,Stop);
   signal(SIGCHLD,ChildDies);
   signal(SIGPIPE,PipeErr);

   DosError(FERR_DISABLEHARDERR | FERR_ENABLEEXCEPTION);

   err = ReadPrf(PrfLanguage,&Language,sizeof(Language),sizeof(Language));
   if (err == 0) {                      /* The entry has not been found      */
      Language = ENGLISH;               /* Default in case of an error       */
      ptr = getenv("LANG");
      if (ptr != NULL) {
         for (i = 0;i < (int) LanguagesCount;i++)
            if (strcmp(Languages[i].LANG_descr,ptr) == 0)
               Language = Languages[i].code;
      }
      ReWritePrfFlags |= REWRITE_LANGUAGE;
   } else if (err == sizeof(Language)) {
                                        /* Valid language code?              */
      for (i = 0;i < (int) LanguagesCount;i++)
         if (Languages[i].code == Language)
            break;
      if (i >= (int) LanguagesCount) {  /* Unknown value                     */
         Language = ENGLISH;
         ReWritePrfFlags |= REWRITE_LANGUAGE;
      }
   } else {                             /* Entry corrupted, use English      */
      Language = ENGLISH;
      ReWritePrfFlags |= REWRITE_LANGUAGE;
   }
   if (ReWritePrfFlags & REWRITE_LANGUAGE)
      ReWritePrf();
   CurrentLanguage = Language;

   for (id = IDS_Title;id <= IDS_lastnumber_mustbe_last_IDS;id++) {
                                        /* preload all strings...            */
#ifdef CHECK_STRINGS                    /* can't use Dprintf or printf       */
      fprintf(stdout,"string of ID %lu is >%s<\n",id,Get(id));
#else
      Get(id);
#endif
   }
                                        /* check if some needed IDS have been*/
                                        /* loaded                            */
   if ((Get(IDS_Title) == szUNKNOWN) ||
       (Get(IDS_ErrInitialize) == szUNKNOWN)) {
      fputs("Can't load the resource strings, inform author or\n"
            "correct the IDs in the resource file appendage of the program.\n",
            stdout);
      return(-1);
   }

   if (RunningUnderPM) {                /* initialize PM                     */
      if ((hab = WinInitialize(0)) == (HAB) 0) {
         fputs(Get(IDS_ErrInitialize),stdout);
         return(-1);
      }
      if ((hmq = WinCreateMsgQueue(hab,0)) == (HMQ) 0) {
         fputs(Get(IDS_ErrMsgQueue),stdout);
         return(-1);
      }
   }
                                        /* Well, the whole stuff consists of */
                                        /* the following terms:              */
                                        /* 1) handle 0 is open on "nul" and  */
                                        /*    stdin points to this handle    */
                                        /* 2) handle 1 and 2 are reserved for*/
                                        /*    job redirection and should     */
                                        /*    point to "nul", too.           */
                                        /* 3) don't believe, these FILE's are*/
                                        /*    open! They are not in case of  */
                                        /*    PM!                            */
   fclose(stdin);                       /* fclose the standard file explicit */
   fclose(stdout);                      /*                                   */
   fclose(stderr);                      /*                                   */
   fcloseall();                         /* fclose all other                  */
   for (i = 0;i < 100;i++)              /* safety first!                     */
      close(i);
   if ((freopen("nul","r+b",stdin)  != stdin) ||   /* reassign standard files*/
       (freopen("nul","r+b",stdout) != stdout) ||
       (freopen("nul","r+b",stderr) != stderr))
      FatalError(Get(IDS_ErrReassignStdIO),(HWND) 0);
#ifdef DEBUG
                                        /* in case of debugging we open a    */
   if ((DebugFile = fopen("crond.out","at")) != NULL) {  /* helper file      */
                                        /* don't let childs inherit this file*/
      fcntl(fileno(DebugFile),F_SETFD,
                              fcntl(fileno(DebugFile),F_GETFD,0) | FD_CLOEXEC);
      setvbuf(DebugFile,NULL,_IONBF,0);
   }
#endif

   if (IsPipeThere())
      FatalError(Get(IDS_AlreadyStarted),(HWND) 0);

   CheckTCPIPAvail();

                                        /* setup needed semaphores           */
   if ((DosCreateMutexSem(NULL, &ThreadSem, 0, FALSE) != 0) ||
       (DosCreateMutexSem(NULL, &WriteSem,  0, FALSE) != 0) ||
       (DosCreateEventSem(NULL, &CronSem,   0, FALSE) != 0))
      FatalError(Get(IDS_ErrCreateSem),(HWND) 0);

                                        /* query own filename                */
   if (DosQueryModuleName(ppib->pib_hmte,sizeof(FullName),FullName) != 0)
      FatalError(Get(IDS_ErrQMod),(HWND) 0);

   strcpy(DefIOPath,FullName);          /* prepare default IO path           */

   ptr = DefIOPath;
   while (strpbrk(ptr,Delims) != NULL)
      ptr = strpbrk(ptr,Delims) + 1;
   *ptr = '\0';

   ReadProfileData();                   /* setup all saved profile data      */

   if (RunningUnderPM) {
      if (!WinRegisterClass(hab,
                            CronDaemonClass,
                            ClientWndProc,
                            CS_SIZEREDRAW | CS_MOVENOTIFY,
                            0L))
         FatalWinError((HWND) 0);

      flFrameFlags = FCF_STANDARD & ~FCF_ACCELTABLE;
      hwndFrame = WinCreateStdWindow(HWND_DESKTOP,
                                     0,
                                     &flFrameFlags,
                                     CronDaemonClass,
                                     Get(IDS_Title),
                                     0l,
                                     0,
                                     NLS(ID_CRON),
                                     &hwndClient);
      if (hwndFrame == (HWND) 0)
         FatalWinError((HWND) 0);
   } else {
#ifdef DEBUG
      if ((getenv("RUN_FOREGROUND") == NULL) &&
          (ppib->pib_ultype != PT_DETACHED)) {
#else
      if (ppib->pib_ultype != PT_DETACHED) { /* not detached?                */
#endif
         fcloseall();                   /* the now starting process needs    */
         close(cronhandle);             /* exclusive access to all files     */
         FailBuf[0] = 0;
         if ((err = DosExecPgm(FailBuf,
                               sizeof(FailBuf),
                               EXEC_BACKGROUND,
                               ppib->pib_pchcmd,
                               ppib->pib_pchenv,
                               &res,
                               FullName)) != 0) {
            out = fopen("SCREEN$","w+t"); /* out has been closed             */
            Message(Get(IDS_CantCallMyself),
                    err,
                    ResolvOS2Err(err,(FailBuf[0] != 0) ? FailBuf : FullName,
                                                                        NULL));
            exit(-1);
         }
         return(0);
      }
   }
                                        /* setup pipe                        */
   if ((DosCreateNPipe(PipeName,
                       &hp,
                       NP_ACCESS_DUPLEX | NP_NOINHERIT | NP_WRITEBEHIND,
                       NP_WAIT | NP_READMODE_BYTE | NP_TYPE_BYTE | 1,
                       MAX_IO,
                       MAX_IO,
                       500) != 0) ||
       (pipehandle = _imphandle(hp)) == -1)
      FatalError(Get(IDS_ErrNamedPipe),(HWND) 0);
   setmode(pipehandle,O_BINARY);

                                        /* start cron thread                 */
   if ((CronTid = (TID) _beginthread(CronDaemon,NULL,0x4000,NULL)) == (TID) -1)
      FatalError(Get(IDS_ErrStartDaemon),(HWND) 0);


   if (_beginthread(PipeThread,NULL,0x4000,NULL) == (TID) -1)
      FatalError(Get(IDS_ErrStartPipe),(HWND) 0);

   if (TCPIPAvail)
      if (_beginthread(SocketThread,NULL,0x4000,NULL) == (TID) -1)
         FatalError(Get(IDS_ErrStartSocket),(HWND) 0);   /* there is not     */
                                        /* enough memory to start the thread.*/
                                        /* if we continue we'll run into     */
                                        /* this problem later. I think it's  */
                                        /* better to terminate.              */

   if (RunningUnderPM) {
      ULONG fl = 0;
      if (ProgramFlags & PRG_START_MAXIMIZED)
         fl = SWP_MAXIMIZE;
      else if (ProgramFlags & PRG_START_MINIMIZED)
         fl = SWP_MINIMIZE;
      WinSetWindowPos(hwndFrame,0,
                      FrameRect.xLeft, FrameRect.yBottom,
                      FrameRect.xRight,FrameRect.yTop,
                      SWP_DEACTIVATE | SWP_MOVE | SWP_SIZE | SWP_SHOW | fl);
   }

   Message(Get(IDS_CronStarts));


   if (RunningUnderPM) {
      if (!setjmp(PrgEnd)) {
         SetJumpEnabled = 1;            /* allow the use of longjmp          */
         while (WinGetMsg(hab,&qmsg,0L,0,0))
            WinDispatchMsg(hab,&qmsg);
      }
      SetPosOfWindow(hwndFrame);
      WinDestroyWindow(hwndFrame);
      WinDestroyMsgQueue(hmq);
      WinTerminate(hab);
   } else {
      if (!setjmp(PrgEnd)) {
         SetJumpEnabled = 1;            /* allow the use of longjmp          */
         while (!GlobalStop)            /* wait until program should stop or */
            if (DosWaitThread(&CronTid,DCWW_WAIT) == 0) {
               CronTid = (TID) -1;
               break;
            }
      }
   }

   if (CronTid != (TID) -1)             /* cron thread still alive?          */
      if (DosWaitThread(&CronTid,DCWW_WAIT) != 0)  /* may occur              */
         DosSleep(1000);                /* be patient, GlobalStop is set...  */

   Message(Get(IDS_CronEnds));

   return(0);
}

/* RCS depending informations
 *
 * $Name: Version121 $
 *
 * $Log: cronmain.c $
 * Revision 1.7  1996/05/09 10:05:47  Florian
 * Problems with blanks in the help filename fixed.
 *
 * Revision 1.6  1995/10/18 09:46:07  Florian
 * Used font, colors and window state and current date are stored in the
 * INI-file now.
 * Better handling of the CHILD signal.
 * Added support of the French language.
 *
 * Revision 1.5  1995/08/03 07:43:23  Florian
 * NLS supported (English and German)
 *
 * Revision 1.4  1995/03/15 09:07:34  Florian
 * Some minor bugs fixed.
 * TCP/IP support added.
 *
 * Revision 1.3  1995/03/06 11:52:06  Florian
 * Many bugs fixed.
 * Fully supported online help.
 * Notebook layout revised.
 *
 * Revision 1.2  1995/02/20 12:53:23  Florian
 * All dialogs are placed into a notebook.
 * Some bugs fixed.
 *
 * Revision 1.1  1995/02/03 10:42:33  Florian
 * Initial revision
 *
 *
 */
static char rcsid[] = "@(#)$Id: cronmain.c 1.7 1996/05/09 10:05:47 Florian Exp $";
