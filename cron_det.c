/* Copyright (c) 1995 Florian Groáe-Coosmann, RCS section at the eof         */
/* This is the non PM frontend. This program type is still in testing.       */
/* See the PM frontend for more information. It should be preferred for OS/2.*/
/* Most important mistake: It can't "start" a program (no DOS, VIO sessions  */
/* etc)                                                                      */
/* Many interesting functions belonging to this frontend are placed in the   */
/* module cronmain.c, since they are shared with the cron_pm.c module.       */
#define INCL_NOCOMMON
#define INCL_DOSSEMAPHORES

#include <os2.h>

#include <io.h>
#include <stdarg.h>
#include <signal.h>
#include "server.h"

/*****************************************************************************/
/*  function name : DetachedNewProgramStatus                                 */
/*                                                                           */
/*  arguments     : ID of the resource string which should be display as     */
/*                  the new program status                                   */
/*                                                                           */
/*  description   : displays the new program status (normal, at startup,     */
/*                  etc.) on the program status line.                        */
/*                                                                           */
/*  note          : not used in the non PM version                           */
/*****************************************************************************/
void DetachedNewProgramStatus(ULONG NewStatus)
{
}

/*****************************************************************************/
/*  function name : DetachedNewStartTime                                     */
/*                                                                           */
/*  arguments     : time of the job that will be started next.               */
/*                                                                           */
/*  description   : displays the time of the next starting job on the        */
/*                  related display line. The time may be (time_t) -1. In    */
/*                  this case there are either no periodically running       */
/*                  jobs or no job time is in the computing distance (24-    */
/*                  48 hours)                                                */
/*                                                                           */
/*  note          : not used in the non PM version                           */
/*****************************************************************************/
void DetachedNewStartTime(time_t time)
{
}

/*****************************************************************************/
/*  function name : DetachedNewOutputFile                                    */
/*                                                                           */
/*  description   : displays the current output file on the related          */
/*                  display line.                                            */
/*                                                                           */
/*  note          : not used in the non PM version                           */
/*****************************************************************************/
void DetachedNewOutputFile(void)
{
}

/*****************************************************************************/
/*  function name : DetachedJobsModified                                     */
/*                                                                           */
/*  description   : Send the notebook dialog "Crontabs" a WM_USER. This      */
/*                  will advise it to refresh since the list of jobs has     */
/*                  been changed.                                            */
/*                                                                           */
/*  note          : not used in the non PM version                           */
/*****************************************************************************/
void DetachedJobsModified(void)
{
}

/*****************************************************************************/
/*  function name : DetachedStopPM                                           */
/*                                                                           */
/*  description   : Send the PM window a WM_CLOSE. The program should        */
/*                  terminate. The window has already received a WM_CLOSE    */
/*                  and will die now since StopPM is called from the         */
/*                  daemon thread.                                           */
/*                                                                           */
/*  note          : not used in the non PM version                           */
/*****************************************************************************/
void DetachedStopPM(void)
{
}

/*****************************************************************************/
/*  function name : XMessage                                                 */
/*                                                                           */
/*  arguments     : see printf                                               */
/*                                                                           */
/*  description   : like Message buts append \n to the message.              */
/*****************************************************************************/
static void XMessage(const char *fmt,...)
{
   va_list marker;
   if ((ProgramFlags & PRG_OUTPUT_IS_NUL) || (out == NULL))
      return;
   BlockOutput();
   fputs(GetTimeString(),out);
   va_start(marker,fmt);
   vfprintf(out,fmt,marker);
   va_end(marker);
   putc('\n',out);
   fflush(out);
   UnBlockOutput();
}

/*****************************************************************************/
/*  function name : DetachedError                                            */
/*                                                                           */
/*  arguments     : string describing an error, owner of the error window    */
/*                                                                           */
/*  description   : prints the error string into the output file. The HWND   */
/*                  parameter is for the PM version only.                    */
/*****************************************************************************/
void DetachedError(const char *s,HWND dummy)
{
   XMessage(s);
   Dprintf((char *) s);
}

/*****************************************************************************/
/*  function name : DetachedFatalError                                       */
/*                                                                           */
/*  arguments     : see Error.                                               */
/*                                                                           */
/*  description   : see Error but terminates the program.                    */
/*****************************************************************************/
void DetachedFatalError(const char *s,HWND dummy)
{
   Error(s,dummy);
   exit(-1);
}

/*****************************************************************************/
/*  function name : DetachedStop                                             */
/*                                                                           */
/*  arguments     : signal number generated by the runtime system            */
/*                                                                           */
/*  description   : This is a signal routine. This routine will shut down    */
/*                  the program gracefully if the user or the system sends   */
/*                  a signal (SIG_TERM, ^C or ^Break)                        */
/*                  On the first call the programm will be stoped normally   */
/*                  (with atexit execution in the cron thread). On the       */
/*                  second call the program dies with exit since we can      */
/*                  assume that the user has detected an error in the        */
/*                  atexit exeution and wants an emergency stop.             */
/*                  We must be very careful. If the signal is SIG_TERM       */
/*                  the runtime system will stop the program on              */
/*                  return. This isn't acceptable. Therefore we need the     */
/*                  use of longjmp.                                          */
/*                                                                           */
/*  note          : don't call directly                                      */
/*****************************************************************************/
void DetachedStop(int code)
{
   static int first = 1;
   if (first) {                         /* first call?                       */
      first = 0;
      GlobalStop++;                     /* show other threads this circum-   */
                                        /* stance                            */
      if (CronSem != (HEV) 0)           /* cron thread alive? awake it!      */
         DosPostEventSem(CronSem);      /* let atexit execution run          */
      if (pipehandle != -1)             /* Pipe open? close it, maybe it has */
         close(pipehandle);             /* blocked the joblist               */
      pipehandle = -1;
      signal(code,SIG_ACK);
      if (SetJumpEnabled)               /* can we use longjmp?               */
         longjmp(PrgEnd,1);
      else
         exit(0);
   }
                                        /* else called twice                 */
   signal(code,SIG_IGN);
   if ((ProgramFlags & PRG_OUTPUT_IS_NUL) || (out == NULL))
      exit(0);
   fputs(GetTimeString(),out);
   fputs(Get(IDS_DieOnSigSig),out);
   putc('\n',out);
   fflush(out);
   exit(0);
}

/* RCS depending informations
 *
 * $Name: Version121 $
 *
 * $Log: cron_det.c $
 * Revision 1.2  1995/02/20 12:53:23  Florian
 * All dialogs are placed into a notebook.
 * Some bugs fixed.
 *
 * Revision 1.1  1995/02/03 10:42:23  Florian
 * Initial revision
 *
 *
 */
static char rcsid[] = "@(#)$Id: cron_det.c 1.2 1995/02/20 12:53:23 Florian Rel $";
