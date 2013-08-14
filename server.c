/* Copyright (c) 1995 Florian Groáe-Coosmann, RCS section at the eof         */
/* This module includes all functions needed to communicate with the pipe.   */
#define INCL_NOPM
#define INCL_NOCOMMON
#define INCL_DOSSEMAPHORES
#define INCL_DOSNMPIPES
#define INCL_DOSFILEMGR
#define INCL_DOSPROCESS
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include "server.h"


char  PipeName[] = DEF_PIPENAME;        /* the name of the beast             */
int   pipehandle = -1;                  /* not open at startup               */

/*****************************************************************************/
/*  function name : IsPipeThere                                              */
/*                                                                           */
/*  return value  : 1 if the pipe already exists (another crond is           */
/*                  running), 0 if not                                       */
/*                                                                           */
/*  description   : tries to open the named pipe and decides wether the      */
/*                  daemon is alredy running.                                */
/*****************************************************************************/
int IsPipeThere(void)
{
   int handle;
   if ((handle = open(PipeName,O_BINARY | O_RDWR)) == -1) {
      if (errno == ENOENT)              /* only ENOENT is OK, since the pipe */
         return(0);                     /* may be in use or in a disconneted */
                                        /* state.                            */
      return(1);
   }
   close(handle);
   return(1);
}

/*****************************************************************************/
/*  function name : WaitClientDisconnect                                     */
/*                                                                           */
/*  description   : waits until the client closes the pipe. This is needed   */
/*                  to provide data send back, since the client has to       */
/*                  receive the data before we close the pipe.               */
/*                  We wait until the pipe is closed, either by the client   */
/*                  or by the cron thread which will close it after a        */
/*                  timeout (currently about 1 minute).                      */
/*                                                                           */
/*  note          : Maybe, this function is unnessesary if we use the        */
/*                  NP_NOWRITEBEHIND-flag while opening the pipe?            */
/*****************************************************************************/
void WaitClientDisConnect(void)
{
   ULONG err,state,done;
   AVAILDATA avail;
   time_t kill;
   unsigned wait = 32;
   char DummyBuf[1];
   kill = time(NULL) + 20;              /* don't believe in the cron thread! */
                                        /* Close the pipe after 20 minutes.  */
                                        /* That's more than enough.          */
   do {
      err = DosPeekNPipe(pipehandle,
                         DummyBuf,
                         1,
                         &done,
                         &avail,
                         &state);
      if ((err != 0) || (state != NP_STATE_CONNECTED))
         return;                        /* pipe is closed                    */
      DosSleep(wait);                   /* first, wait 1 time slice, double  */
      wait <<= 1;                       /* the waiting time until the maximum*/
      if (wait > 1000)                  /* of 1 second is reached            */
         wait = 1000;
   } while (time(NULL) < kill);
}

/*****************************************************************************/
/*  function name : ReadBlockedMsgPipe                                       */
/*                                                                           */
/*  arguments     : pointer to the handle of the pipe, buffer, size of the   */
/*                  buffer, minimum to read before returning                 */
/*                                                                           */
/*  return value  : bytes that have been read or -errno in case of an        */
/*                  error                                                    */
/*                                                                           */
/*  description   : reads bytes out of the pipe. Some possible errors were   */
/*                  checked to correct reading problems.                     */
/*                  Normally, the function returns after reading a minimum   */
/*                  of bytes.                                                */
/*                                                                           */
/*  note          : the file may have been closed externally while reading   */
/*                  in this function. The pipes work in the correct manner   */
/*                  in this case but sockets return an errno of EINTR.       */
/*                  This is an acceptable error in most cases of retry       */
/*                  but we have to abort! (There is no way for both          */
/*                  handles to figure out the close operation, really.)      */
/*                  Therefore, we use the pointer to the handle to see       */
/*                  wether the pipe or socket has been closed.               */
/*****************************************************************************/
int ReadBlockedMsgPipe(volatile int *hf,char *buffer,size_t maxbuffer,
                                                              size_t minbuffer)
{
   int i;
   int done = 0;
   errno = 0;
   i = read(*hf,buffer,maxbuffer);
   if (i > 0) {                         /* looks fine..                      */
      done = i;
      if ((size_t) i >= minbuffer)      /* is it enough?                     */
         return(i);                     /* yes, all done                     */
   }
                                        /* i <= 0:                           */
   else if ((errno != EAGAIN) && (errno != EINTR)) /* accept interrupts and  */
                                        /* empty read buffers only           */
      return((errno) ? -errno : -EINVAL); /* unexpected error                */
   while (((errno == EAGAIN) ||         /* these errnos are "good" errnos    */
           (errno == EINTR) ||
           (errno == 0)) &&
          (done < minbuffer)) {
      if (*hf == -1)                    /* has the file been closed from     */
         break;                         /* the cron thread?                  */
      errno = 0;                        /* reset errno and try to read       */
      i = read(*hf,buffer + done,maxbuffer - (size_t) done); /* another chunk*/
      if (i > 0)                        /* read OK?                          */
         done += i;
                                        /* maybe, we run into an error if we */
                                        /* read 0 bytes again and again but  */
                                        /* we'll be killed by the cron thread*/
                                        /* after a timeout, he's a good      */
                                        /* fellow :-)                        */
   }
   if (done > 0)                        /* anything done?                    */
      return(done);                     /* ignore any errors                 */
   return((errno) ? -errno : -EINVAL);  /* errno not set? choose EINVAL      */
}

/*****************************************************************************/
/*  function name : ScanNumber                                               */
/*                                                                           */
/*  arguments     : 0-terminated string, buffer for the result               */
/*                                                                           */
/*  return value  : errno value in case of an error, 0 otherwise             */
/*                                                                           */
/*  description   : tries to convert the string to a positive decimal        */
/*                  number and puts it to the supplied buffer. Leading       */
/*                  and trailing whitespaces are accepted.                   */
/*****************************************************************************/
int ScanNumber(char *s,ULONG *num)
{
   ULONG val;
   char *ptr;
   while (isspace(*s))                  /* jump over leading whitespaces     */
      s++;
   if ((*s == '\0') || (*s == '-'))     /* empty string or the negation sign?*/
      return(EINVAL);
   errno = 0;
   val = strtoul(s,&ptr,10);            /* try to convert                    */
   if ((s == ptr) || (errno))           /* nothing done or an error?         */
      return(EINVAL);
   while (isspace(*ptr))                /* jump over trailing whitespaces    */
      ptr++;
   if (*ptr != '\0')                    /* any more characters in the string?*/
      return(EINVAL);
   *num = val;                          /* assign value and return OK        */
   return(0);
}

/*****************************************************************************/
/*  function name : ProcessRequest                                           */
/*                                                                           */
/*  arguments     : pointer to a pipe or socket handle, name of the pipe     */
/*                  or socket.                                               */
/*                                                                           */
/*  description   : processes one request from the pipe or socket. The       */
/*                  pipe or socket must be in the CONNECTED state.           */
/*                                                                           */
/*  note          : the file may have been closed externally while doing     */
/*                  some work in this function. The pipes work in the        */
/*                  correct manner in this case but sockets return an        */
/*                  errno of EINTR.                                          */
/*                  This is an acceptable error in most cases of retry       */
/*                  but we have to abort! (There is no way for both          */
/*                  handles to figure out the close operation, really.)      */
/*                  Therefore, we use the pointer to the handle to see       */
/*                  wether the pipe or socket has been closed.               */
/*****************************************************************************/
void ProcessRequest(volatile int *hf,char *name)
{
   char tmpbuf[0x1000];
   char *buf = NULL,Request;

   int err,done = 0;
   ULONG i;
                                        /* read the requested function       */
   if ((done = ReadBlockedMsgPipe(hf,tmpbuf,sizeof(tmpbuf)-1,1)) < 0) {
      errno = -done;                    /* set back the errno value          */
      Perror(name);                     /* and print it to the output file   */
      return;
   }
                                        /* analyse the function code         */
   if (done == 0) {                     /* pipe opened and closed immediately*/
      Message(Get(IDS_0ByteCommunication));
      return;
   }
   tmpbuf[done] = 0;                    /* terminate the request             */
   err = EINVAL;                        /* assume an error, if err != 0 the  */
                                        /* code below fills the answer buffer*/
                                        /* with the appropriate error message*/
   Request = tmpbuf[0];                 /* extract the request code          */
   switch (Request) {
      case 'D':                         /* Delete                            */
         if ((done < 2) || (ScanNumber(tmpbuf + 1,&i) != 0))
            break;
         BlockProcess();                /* block the job list                */
         if ((i == 0) || (LookupEntryNum(i,0) == NULL)) {   /* job number    */
            err = 0;                    /* doesn't exist? Set the reply      */
            strcpy(tmpbuf,Get(IDS_JobNotFound));   /* explicitely            */
         } else if ((err = DeleteEntryNum(i,tmpbuf)) == 0)
            JobsModified();             /* the list is modified              */
         UnBlockProcess();              /* allow other threads to continue   */
         break;

      case 'R':                         /* Read list request                 */
         if (done != 1)
            break;
         BlockProcess();                /* block the job list                */
         buf = CreateList(CL_USER);     /* create a buffer with all jobs and */
                                        /* their job numbers                 */
         UnBlockProcess();              /* allow other threads to continue   */
         if (buf == NULL) {             /* not enough memory? Don't stop the */
            err = ENOMEM;               /* process, since the cron thread is */
            Message(Get(IDS_NotEnoughMemory));  /* the only important thread */
         } else {                       /* there are data to send back. They */
            err = 0;                    /* may be larger than every standard */
                                        /* buffer, thus send them in a       */
                                        /* different write operation         */
            sprintf(tmpbuf,"crond: %lu Data OK\r\n",  /* don't change this!!!*/
                           (unsigned long) strlen(buf));
         }
         break;

      case 'N':                         /* New Entry                         */
         if (done < 2)
            break;
         BlockProcess();                /* block the job list                */
         if ((err = NewEntry(tmpbuf + 1,(size_t) (done - 1),tmpbuf)) == 0)
            JobsModified();
         UnBlockProcess();              /* allow other threads to continue   */
         break;

      default:                          /* err has been set to EINVAL, that's*/
         ;                              /* pretty good                       */
   }
   if (err)                             /* any errors? Convert them to a     */
      sprintf(tmpbuf,"crond: %s\r\n",GetError(err));  /* message             */

   if ((done = write(*hf,tmpbuf,strlen(tmpbuf))) < 0) {  /* error while      */
      if (buf != NULL)                  /* writing the answer?               */
         free(buf);
      Perror(Get(IDS_ErrRequest));      /* notice it in out output file      */
      return;
   } else if ((size_t) done != strlen(tmpbuf)) {   /* can't write the whole  */
      if (buf != NULL)                  /* message back?                     */
         free(buf);
      Message(Get(IDS_ErrRequestIncomplete));
      return;
   }
   if ((Request != 'R') || (err)) {     /* no more transmits or an error?    */
      if (buf != NULL)                  /* should never happen...            */
         free(buf);
      return;
   }
                                        /* Request == 'R', send the data     */
                                        /* buffer but first of all let us    */
                                        /* transmit the data. crontab expects*/
                                        /* a small buffer of one line. If    */
                                        /* we use pipes over a network or    */
                                        /* to DOS boxes the data may be      */
                                        /* collected. This will cause an     */
                                        /* error with much data.             */
   DosResetBuffer(*hf);
   if ((done = write(*hf,buf,strlen(buf))) < 0)
      Perror(Get(IDS_ErrList));
   else if ((size_t) done != strlen(buf))
      Message(Get(IDS_ErrListIncomplete),
              done,(unsigned long) strlen(buf));
   free(buf);
}

/* RCS depending informations
 *
 * $Name: Version121 $
 *
 * $Log: server.c $
 * Revision 1.4  1995/10/18 09:46:10  Florian
 * Problems while writing to DOS-boxes or networks: need flushing between
 * writes.
 *
 * Revision 1.3  1995/03/15 09:07:34  Florian
 * Some minor bugs fixed.
 * TCP/IP support added.
 *
 * Revision 1.2  1995/02/20 12:53:23  Florian
 * All dialogs are placed into a notebook.
 * Some bugs fixed.
 *
 * Revision 1.1  1995/02/03 10:42:48  Florian
 * Initial revision
 *
 *
 */
static char rcsid[] = "@(#)$Id: server.c 1.4 1995/10/18 09:46:10 Florian Rel $";
