/* Copyright (c) 1995 Florian Groáe-Coosmann, RCS section at the eof */
#ifndef SERVER_INCLUDED
#define SERVER_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include "common.h"
#include "cron_id.h"

#define WAKEUP 3600                     /* awake every hour to compute the   */
                                        /* next job time (value in seconds)  */
#define MAX_CRONFILESIZE 0x20000        /* don't allow Crontab files larger  */
                                        /* than this value, I think this is  */
                                        /* enough :-)                        */
#define MAX_IO 0x400                    /* pipe's communication size         */
#define WINDOW_REFRESH  ((ULONG) 0xC302C302) /* magic number for WM_USER     */

/* cronmain.c */
#define REWRITE_SCREEN        0x00000001ul
#define REWRITE_FLAGS         0x00000002ul
#define REWRITE_MAXIMUM       0x00000004ul
#define REWRITE_CURRENT       0x00000008ul
#define REWRITE_THRESHOLD     0x00000010ul
#define REWRITE_OUTPUTPATH    0x00000020ul
#define REWRITE_TCPIP_ALLOW   0x00000040ul
#define REWRITE_CRONFILENAME  0x00000080ul
#define REWRITE_LANGUAGE      0x00000100ul
#define REWRITE_COLORS        0x00000200ul
#define REWRITE_FONT          0x00000400ul
#define REWRITE_DATE          0x00000800ul

#define PRG_OUTPUT_IS_NUL     0x00000001ul
#define PRG_RUNATSTARTUP      0x00000002ul
#define PRG_RUNATEXIT         0x00000004ul
#define PRG_ALLOW_LAN         0x00000008ul
#define PRG_ALLOW_TCPIP       0x00000010ul
#define PRG_START_MINIMIZED   0x00000020ul
#define PRG_START_MAXIMIZED   0x00000040ul
#define PRG_ALLFLAGS          ((PRG_START_MAXIMIZED << 1) - 1)

#define ENGLISH                    0    /* 0 is the default value, don't chg.*/
#define GERMAN                 10000
#define FRENCH                 20000
typedef struct {
   USHORT   code;                       /* ENGLISH, GERMAN, etc              */
   char    *LANG_descr;                 /* values codes from env-var LANG    */
   char    *description;                /* full description                  */
} LANGUAGE;

extern HAB   hab;                       /* these four values are for the     */
extern HMQ   hmq;                       /* PM version only                   */
extern HWND  hwndFrame;
extern HWND  hwndClient;

extern char   CurrFilename[];           /* user modifiable values            */
extern ULONG  ProgramFlags;
extern RECTL  FrameRect,SaveFrameRect;
extern ULONG  Threshold;
extern UCHAR  MaxOutputs;
extern UCHAR  CurrOutput;
extern CHAR   Crontabs[];
extern CHAR   TCPIP_Allow[];
extern CHAR   OutputPath[CCHMAXPATH];
extern USHORT Language;                 /* LANGUAGE.code                     */
extern USHORT CurrentLanguage;
                                        /* Language Specific Identifier:     */
#define NLS(x) ((USHORT) ((USHORT) (x) + CurrentLanguage))
#define NONLS(x) ((USHORT) ((USHORT) (x) % 10000))

extern LONG   Colors[4];                /* used colors, see following codes  */
#define MYCLR_SYSFOREGROUND   0         /* foreground used by system         */
#define MYCLR_FOREGROUND      1         /* currently used foreground         */
#define MYCLR_SYSBACKGROUND   2         /* background used by system         */
#define MYCLR_BACKGROUND      3         /* currently used background         */
extern char Fontname[2 * FACESIZE + sizeof("10.")];   /* saveable font name  */
extern ULONG LastDayStarted;            /* last day crond has run            */

extern const LANGUAGE Languages[];
extern const unsigned LanguagesCount;   /* Number of Languages               */

extern int   GlobalStop;                /* flag: shall we stop the program?  */
extern int   TCPIPAvail;                /* is TCP/IP available?              */
extern int   RunningUnderPM;            /* the name of the flag says it      */
extern ULONG ReWritePrfFlags;           /* flags what has changed            */
extern CHAR  DefIOPath[CCHMAXPATH];     /* path of the program               */
extern char  Delims[];                  /* path delimiter                    */
extern FILE *out;                       /* current output file               */
extern int   cronhandle;                /* the handle of the Crontabs file   */
extern HEV   CronSem;                   /* event semaphore to show the       */
                                        /* cron daemon that things have been */
                                        /* changed                           */

#define printf()                        /* no printf's or puts's! they won't */
#define puts()                          /* output anything, use Dprintf or   */
#ifdef DEBUG                            /* Dputs instead                     */
extern FILE *DebugFile;
void Dputs(char *s);
void Dprintf(char *fmt,...);
#else
#define Dputs(s)
#define Dprintf(s)
#endif
extern void (*NewProgramStatus)(ULONG NewStatus);  /* show new program state */
extern void (*NewStartTime)(time_t time); /* show next job start time        */
extern void (*NewOutputFile)(void);     /* show the display output file      */
extern void (*JobsModified)(void);      /* advise a changed job list         */
extern void (*StopPM)(void);            /* stop PM window                    */
extern void (*Error)(const char *s,HWND dummy); /* show error string         */
extern void (*FatalError)(const char *s,HWND dummy);  /* show error and die  */
extern void (*Stop)(int code);          /* signal handler                    */
extern int SetJumpEnabled;              /* flag: can we use longjmp          */
extern jmp_buf PrgEnd;                  /* jump if signaled                  */

void FullName(char *Filename);          /* make a fully qualified pathname   */
void AppendPathComponent(char *path,const char *component);
int CheckOutputPath(char *s);           /* is output path OK?                */
int NextOutputFile(int Append);         /* use next output file              */
int ReReadCrontabs(char *s);            /* (re)read the Crontabs file        */
int ReWritePrf(void);                   /* rewrite changed profile data      */
const char *Get(ULONG IDS);             /* get the requested string from     */
                                        /* the resource appendix and buffer  */
void GetTime(char *buf,size_t bufsize,time_t t);   /* return time t in buf   */
const char *GetTimeString(void);        /* return locally buffered GetTime   */
                                        /* with " : "                        */
void BlockOutput(void);                 /* use the output file exclusively   */
void UnBlockOutput(void);               /* release the output file           */
void Message(const char *fmt,...);      /* print(f) to the output file       */
const char *GetError(int errnocode);    /* return a description of the errno */
                                        /* WITHOUT trailing newlines         */
void Perror(const char *s);             /* perror to output                  */
const char *ResolvOS2Err(ULONG msgno,const char *name,...); /* resolve the   */
                                        /* OS/2-error msgno und return the   */
                                        /* appropriate message string        */
void ChildDies(int code);

/* cron_det.c */
void DetachedNewProgramStatus(ULONG NewStatus); /* see NewProgramStatus      */
void DetachedNewStartTime(time_t time); /* see NewStartTime                  */
void DetachedNewOutputFile(void);       /* see NewOutputFile                 */
void DetachedJobsModified(void);        /* see JobsModified                  */
void DetachedStopPM(void);              /* see StopPM                        */
void DetachedError(const char *s,HWND dummy);   /* see Error                 */
void DetachedFatalError(const char *s,HWND dummy); /* see FatalError         */
void DetachedStop(int code);            /* see Stop                          */

/* cron_pm.c */
void PMError(const char *s,HWND owner); /* see Error                         */
void PMFatalError(const char *s,HWND owner); /* see FatalError               */
void WinError(HWND owner);              /* show GPI error                    */
void FatalWinError(HWND owner);         /* show GPI error and die            */
void SetPosOfWindow(HWND framewin);     /* save current window pos. and size */
MRESULT EXPENTRY ClientWndProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2);
void PMNewProgramStatus(ULONG NewStatus); /* see NewProgramStatus            */
void PMNewStartTime(time_t time);       /* see NewStartTime                  */
void PMNewOutputFile(void);             /* see NewOutputFile                 */
void PMJobsModified(void);              /* see JobsModified                  */
void PMStopPM(void);                    /* see StopPM                        */
void PMStop(int code);                  /* see Stop                          */

/* server.c */
extern char  PipeName[];                /* the name of the communication pipe*/
extern int   pipehandle;                /* the handle of the pipe            */
int IsPipeThere(void);                  /* check for existing pipe           */
void WaitClientDisConnect(void);        /* wait for the client to release    */
                                        /* the pipe                          */
int ReadBlockedMsgPipe(volatile int *hf,char *buffer,size_t maxbuffer,
                                                             size_t minbuffer);
int ScanNumber(char *s,ULONG *num);     /* scan s for a number >= 0 into num */
void ProcessRequest(volatile int *hf,char *name);  /* deal with pipe requests*/

/* tables.c */
typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY  *next;
   char                *s;              /* command line with time            */
   const char          *StartCmd;       /* points to s after the time string */
   time_t               NextStartTime;  /* -1 = not in the next 24 hours     */
   unsigned long long   Minute;         /* bitsets                           */
   unsigned             Hour;           /* bitsets                           */
   unsigned             Day;            /* bitsets                           */
   unsigned             Month;          /* bitsets                           */
   unsigned             WeekDay;        /* bitsets                           */
   unsigned             AtStartup:1;    /* job must run at start of cron     */
   unsigned             AtExit:1;       /* job must run at exit of cron      */
   unsigned             Once:1;         /* run only once, then delete job    */
   unsigned             Deletable:1;    /* job is about to be deleted        */
   unsigned             IsComment:1;    /* no job, the line is a comment     */
   unsigned             Daily:1;        /* run job daily                     */
} LIST_ENTRY;

extern HMTX ThreadSem;                  /* exclusive sem for accessing the   */
                                        /* job table                         */
extern time_t AutoDisConnect;           /* timeout for the pipe client       */
extern time_t AutoCloseSocket;          /* timeout for the socket client     */
extern LIST_ENTRY ListHead;             /* start the the current table       */
extern ULONG EditEntryNumber;           /* No. of job which is edited or -1  */

void BlockProcess(void);                /* use the table exclusively         */
void UnBlockProcess(void);              /* release the table                 */
LIST_ENTRY *LookupEntryNum(ULONG num,int AllowComment);  /* look for the num */
                                        /* entry in the table, comment lines */
                                        /* may be counted/looked for, too.   */
                                        /* 1 based!!!!                       */
int DeleteListEntry(LIST_ENTRY *entry,LIST_ENTRY *head); /* delete the entry */
                                        /* off the list head                 */
#define CL_FILE   1                     /* create list to write Crontabs file*/
#define CL_USER   2                     /* create list for the user          */
#define CL_PM     3                     /* create list for PM                */
char *CreateList(int flag);             /* create a very long string with    */
                                        /* all table entries delimited by    */
                                        /* CR/LF pairs either with numbers   */
                                        /* and without comment lines of      */
                                        /* without numbers but with comments */
                                        /* see CL_??? for details            */
int DeleteEntryNum(ULONG num,char *replybuf);   /* delete job num, copy a    */
                                        /* message to replybuf if successful */
int NewEntry(char *line,size_t size,char *replybuf);  /* insert a new job    */
                                        /* from line (length size, need not  */
                                        /* to be 0-term.), fill reply on succ*/
const char *ParseLine(const char *buf,size_t bufsize,size_t *start,size_t *len);
                                        /* return next line from a buffer    */
int ReadInFile(int handle,LIST_ENTRY *head); /* read in the complete file    */
                                        /* into head                         */
void CronDaemon(void *dummy);           /* thread procedure                  */

/* process.c */
void EndProcess(int pid,int reason,int rc);  /* show rc, copy temp. file     */
void StartProcess(LIST_ENTRY *job);     /* start a new job                   */
int ReapChildren(void);                 /* EndProcess for every stoped job   */
void ShowStillRunnings(void);           /* show not deleted temp. files      */

/* tcpip.c */
extern volatile int CommSock;           /* currenty opened socket to a client*/
void CheckTCPIPAvail(void);             /* checks the availability of TCP/IP */
                                        /* and opens the main socket         */
int ReadAllow(char *s);                 /* (re)read the TCP/IP allow file    */
void SocketThread(void *dummy);         /* deals with TCP/IP connections     */

#endif  /* SERVER_INCLUDED */

/* RCS depending informations
 *
 * $Id: server.h 1.5 1995/10/18 09:46:10 Florian Rel $
 *
 * $Name: Version121 $
 *
 * $Log: server.h $
 * Revision 1.5  1995/10/18 09:46:10  Florian
 * Font, colors and window state and current date variables added.
 * Added support of the French language.
 *
 * Revision 1.4  1995/08/03 07:44:26  Florian
 * NLS supported (English and German)
 *
 * Revision 1.3  1995/03/15 09:07:34  Florian
 * Some minor bugs fixed.
 * TCP/IP support added.
 *
 * Revision 1.2  1995/02/20 12:53:23  Florian
 * All dialogs are placed into a notebook.
 * Some bugs fixed.
 *
 * Revision 1.1  1995/02/03 10:42:49  Florian
 * Initial revision
 *
 *
 */
