/* Copyright (c) 1995 Florian Groáe-Coosmann, RCS section at the eof         */
/* This module includes functions starting a job and copying its output      */
/* to our output file.                                                       */
#define INCL_NOPM
#define INCL_NOCOMMON
#define INCL_DOSSEMAPHORES
#define INCL_DOSFILEMGR
#define INCL_DOSPROCESS
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <process.h>
#include <fcntl.h>
#include <share.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include "server.h"

typedef struct _PROC {                  /* one process slot                  */
   struct _PROC  *next;
   int            pid;                  /* process ID, the following values  */
                                        /* are reserved:                     */
                                        /* 0 = never started                 */
                                        /* -1 = can't start, PROC may been   */
                                        /*      deleted.                     */
   int            rc;                   /* return code                       */
   int            reason;               /* reason of the termination (signal */
                                        /* number) the following value is    */
                                        /* reserverd: -1 = never started     */
   char          *tmpfilename;          /* file name of stdout and stderr of */
} PROC;                                 /* the newly started process         */

static PROC first = {NULL,0,0,0,NULL};

/*****************************************************************************/
/*  function name : EndProcess                                               */
/*                                                                           */
/*  arguments     : process id of a terminated job, reason of the            */
/*                  termination (0 or trap number), return code              */
/*                                                                           */
/*  description   : this is a function for the main thread, which wants to   */
/*                  indicate the termination of a job. The cleanup is done   */
/*                  by the cron thread later.                                */
/*                                                                           */
/*  note          : this has to be called while running in a signal          */
/*                  procedure, because we are NOT blocking the access to     */
/*                  the job list!                                            */
/*****************************************************************************/
void EndProcess(int pid,int reason,int rc)
{
   PROC *run = first.next;
   while (run != NULL) {
      if (pid == run->pid) {            /* process found                     */
         run->rc = rc;
         run->reason = reason;
         return;
      }
      run = run->next;
   }
}

/*****************************************************************************/
/*  function name : NewSlot                                                  */
/*                                                                           */
/*  return value  : a newly allocated and initialized process slot, NULL     */
/*                  in case of insufficent memory                            */
/*                                                                           */
/*  description   : creates a new process slot and initialized it to the     */
/*                  default value. It is appended to the job list.           */
/*                                                                           */
/*  note          : the blocking of the job list must be done by the         */
/*                  calling routine.                                         */
/*****************************************************************************/
static PROC *NewSlot(void)
{
   PROC *run,*last;
   last = &first;
   run = first.next;
   while (run != NULL) {                /* search for the insertion place    */
      last = run;
      run = run->next;
   }
   if ((run = malloc(sizeof(PROC))) == NULL)
      return(NULL);
   memset(run,0,sizeof(PROC));
   run->reason = -1;                    /* never started                     */
   last->next = run;                    /* insert into the list              */
   return(run);
}

/*****************************************************************************/
/*  function name : NewTmpFile                                               */
/*                                                                           */
/*  arguments     : buffer for a handle                                      */
/*                                                                           */
/*  return value  : allocated file name of a temporary file, NULL in case    */
/*                  of insufficent memory or other errors.                   */
/*                                                                           */
/*  description   : creates a new temporary file and returns the filename.   */
/*                  The handle of the file is returned in the argument       */
/*                  buffer.                                                  */
/*****************************************************************************/
static char *NewTmpFile(int *handle)
{
   char *buf,*ptr;
   unsigned i;
   if ((buf = malloc(strlen(DefIOPath) + 2 + 11)) == NULL)  /* delim+term. 0+*/
      return(NULL);                     /* length of filename component      */
   strcpy(buf,DefIOPath);               /* create the filename base          */
   if (DefIOPath[0] != '\0')
      if (strchr(Delims,DefIOPath[strlen(DefIOPath) - 1]) == NULL)
         strcat(buf,"\\");
   ptr = buf + strlen(buf);             /* where to append Cron????.tmp      */
   for (i = 0;i <= 0xFFFF;i++) {
      sprintf(ptr,"Cron%04X.tmp",i);
      if ((*handle = sopen(buf,
                           O_BINARY | O_WRONLY | O_CREAT | O_EXCL | O_SYNC,
                           SH_DENYWR,
                           S_IREAD | S_IWRITE)) != -1)
         return(buf);
   }
                                        /* can't allocate a tempfile, cleanup*/
   free(buf);
   return(NULL);
}

/*****************************************************************************/
/*  function name : WaitDelete                                               */
/*                                                                           */
/*  arguments     : filename to delete                                       */
/*                                                                           */
/*  description   : delete the given file. If an error occurs we assume a    */
/*                  race condition and wait approximately 2 time slices,     */
/*                  then we try again once more. If the error persists we    */
/*                  write an error message to the output.                    */
/*****************************************************************************/
static void WaitDelete(char *s)
{
   ULONG os2rc;
   if (DosDelete(s) == 0)
      return;
   DosSleep(64);                        /* 2 * TimeSlice                     */
   if ((os2rc = DosDelete(s)) != 0)
      Message(Get(IDS_TempfileNotDeleted),
              s,os2rc);
}

/*****************************************************************************/
/*  function name : CopyTemp                                                 */
/*                                                                           */
/*  arguments     : filename of the output file of a job, process id,        */
/*                  return code und reason of the termination of the job     */
/*                                                                           */
/*  description   : writes a message that the job has been stoped and        */
/*                  copies the contents of the job output file to our        */
/*                  output file.                                             */
/*****************************************************************************/
static void CopyTemp(char *fn,int pid,int rc,int reason)
{
   int chunk,done,done2;
   int handle;
   long len;
   int neednl = 1;                      /* newline needed at eof?            */
   static char notbuf[128];             /* minimal emergency buffer          */
   static const char *Msg;
   char *buf;

   if (out == NULL) {                    /* ignore output?                    */
      WaitDelete(fn);
      return;
   }
   if (reason == 0)
      Msg = Get(IDS_JobEndsNormal);
   else {
      Msg = Get(IDS_JobEndsDueSignal);
      rc = reason;
   }
   if ((handle = sopen(fn,O_RDONLY | O_BINARY | O_NOINHERIT,SH_DENYNO)) == -1){
      Message(Msg,pid,rc,Get(IDS_TempfileInaccessable));
      return;
   }
   len = filelength(handle);
   if (len == 0) {                      /* file empty?                       */
      Message(Msg,pid,rc,Get(IDS_TempfileEmpty));
      close(handle);
      WaitDelete(fn);
      return;
   }
   if ((buf = malloc(0x4000)) == NULL) {  /* get a temorary buffer           */
      buf = notbuf;                     /* no memory, use the emergency buf  */
      chunk = 128;
   } else
      chunk = 0x4000;

   BlockOutput();                       /* don't allow any access to the     */
                                        /* output file, we work on it        */
   if (filelength(fileno(out)) >= Threshold) /* need a fresh output file?    */
      NextOutputFile(0);
   fputs(GetTimeString(),out);          /* normal Message processing         */
   fprintf(out,Msg,pid,rc,Get(IDS_TempfileFollows));
   fflush(out);                         /* flush any buffers                 */
   lseek(fileno(out),0l,SEEK_END);      /* seek to end of file and change    */
   setmode(fileno(out),O_BINARY);       /* the writing mode to binary since  */
                                        /* the job output file is in binary  */
                                        /* mode too. Don't use the slow      */
                                        /* text mode.                        */
   while (len > 0) {                    /* copy until eof                    */
      if ((done = read(handle,buf,chunk)) <= 0)
         break;
      if ((done2 = write(fileno(out),buf,done)) <= 0)
         break;
      if ((done2 != done) || (done == 0))
         break;
      len -= done;
      if (len <= 0)                     /* that's all? Check if last char    */
         if (buf[done - 1] == '\n')     /* was a newline. In this case we    */
            neednl = 0;                 /* don't have to do it ourself.      */
   }
   if (neednl)                          /* append newline?                   */
      write(fileno(out),"\r\n",2);
   setmode(fileno(out),O_TEXT);         /* back to standard text mode        */
   fseek(out,0l,SEEK_END);              /* seek to eof, update internal FILE */
                                        /* buffers                           */
   UnBlockOutput();                     /* end of copy                       */
   if (buf != notbuf)                   /* is there an allocated buffer?     */
      free(buf);                        /* free it!                          */
   close(handle);                       /* cleanup                           */
   WaitDelete(fn);
}

/*****************************************************************************/
/*  function name : StartProcess                                             */
/*                                                                           */
/*  arguments     : list entry of a cron job                                 */
/*                                                                           */
/*  description   : starts the job and appends job infos to the running      */
/*                  process list (process slot). All errors were process     */
/*                  locally.                                                 */
/*                  We use CMD (getenv(COMSPEC)) to start any job. Every     */
/*                  job gets a freshly created temporary output file.        */
/*                                                                           */
/*  note          : the blocking of the job list must be done by the         */
/*                  calling routine.                                         */
/*****************************************************************************/
void StartProcess(LIST_ENTRY *job)
{
   PROC *proc;
   int tmpfilehandle;
   static char *cmd = NULL;
   if (cmd == NULL)                     /* never determined?                 */
      if ((cmd = getenv("COMSPEC")) == NULL)
         FatalError(Get(IDS_NoComSpec),(HWND) 0);
   if ((proc = NewSlot()) == NULL) {    /* can't create a job slot?          */
      Message(Get(IDS_NotEnoughMemory));
      return;
   }
   if ((proc->tmpfilename = NewTmpFile(&tmpfilehandle)) == NULL) {
      Message(Get(IDS_NoMemOrNoTempfile));
      proc->pid = -1;                   /* don't delete the job slot         */
      proc->reason = 0;                 /* immediately, we may run into race */
      return;                           /* conditions due some signals to    */
                                        /* the main thread. The deletion is  */
   }                                    /* done by ReapChildren              */

   if (tmpfilehandle != 1)              /* reassign stdout and stderr of the */
      dup2(tmpfilehandle,1);            /* newly created job to the          */
   if (tmpfilehandle != 2)              /* temprary file                     */
      dup2(tmpfilehandle,2);
   if ((tmpfilehandle != 1) && (tmpfilehandle != 2))  /* close the tempfile  */
      close(tmpfilehandle);             /* thus the only owner is the new    */
                                        /* job.                              */
                                        /* Now, we start the job, we let it  */
                                        /* run in the background of the      */
                                        /* current process. This is either   */
                                        /* detach or unvisible. The user     */
                                        /* (Crontabs!) must support a "start"*/
                                        /* command to show it in a visible   */
                                        /* session except in case of a PM    */
                                        /* program with crond started as a   */
                                        /* PM program, too.                  */

   if ((proc->pid = spawnl(P_NOWAIT,    /* P_SESSION has some problems, yet  */
                           cmd,
                           cmd,
                           "/C",
                           job->StartCmd,
                           NULL)) == -1) {
      proc->reason = 0;                 /* proc->pid is -1, the mess is      */
      Message(Get(IDS_CantStartJob),    /* cleaned up by the next call of    */
             job->StartCmd,strerror(errno)); /* ReapChildren                 */
   } else                               /* job started normally              */
      Message(Get(IDS_JobStarted),
              job->StartCmd,proc->pid);
   dup2(0,1);                           /* stdout and stderr back to the     */
   dup2(0,2);                           /* "nul" device                      */
}

/*****************************************************************************/
/*  function name : ReapChildren                                             */
/*                                                                           */
/*  return value  : number of still outstanding jobs                         */
/*                                                                           */
/*  description   : looks for terminated jobs in the job list, copies        */
/*                  their output to our output files und frees all           */
/*                  allocated resources belonging to the children.           */
/*****************************************************************************/
int ReapChildren(void)
{
   PROC *run,*last,*del;
   int pid,retval = 0;
   BlockProcess();                      /* block the job list                */
   last = &first;
   run = last->next;
   while (run != NULL) {                /* check every job slot              */
      if ((run->pid != 0) && (run->reason != -1)) {   /* job has started?    */
         if ((pid = run->pid) != -1) {  /* job stated normally?              */
            run->pid = -1;              /* job has ended, copy its output    */
            CopyTemp(run->tmpfilename,pid,run->rc,run->reason);
         }
         /* else: error while spawning the process */
         del = run;
         last->next = run->next;        /* cut off the list                  */
         run = run->next;               /* set run to the next candidat      */
         if (del->tmpfilename != NULL)  /* tempfile available?               */
            free(del->tmpfilename);
         free(del);                     /* free the job slot                 */
         continue;
      }
                                        /* job is running:                   */
      last = run;
      run = run->next;
      retval++;
   }
   UnBlockProcess();                    /* allow the access of the job list  */
                                        /* by other threads                  */
   return(retval);
}

/*****************************************************************************/
/*  function name : ShowStillRunnings                                        */
/*                                                                           */
/*  description   : displays all running jobs with their output file. This   */
/*                  function is called once at the end of the cron thread.   */
/*****************************************************************************/
void ShowStillRunnings(void)
{
   PROC *run = first.next;
   while (run != NULL) {                /* no need to block the list         */
      if ((run->pid != 0) && (run->pid != -1))
         Message(Get(IDS_JobYetActive),
                 run->pid,
                 run->tmpfilename);
      run = run->next;
   }
}

/* RCS depending informations
 *
 * $Name: Version121 $
 *
 * $Log: process.c $
 * Revision 1.1  1995/02/03 10:42:46  Florian
 * Initial revision
 *
 *
 */
static char rcsid[] = "@(#)$Id: process.c 1.1 1995/02/03 10:42:46 Florian Rel $";
