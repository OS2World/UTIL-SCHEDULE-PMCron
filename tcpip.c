/* Copyright (c) 1995 Florian Groáe-Coosmann, RCS section at the eof         */
/* This module includes all functions exclusively related to the TCP/IP      */
/* handling.                                                                 */
#define INCL_NOCOMMON
#define INCL_DOSSEMAPHORES
#include <os2.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <share.h>
#include <io.h>
#include "server.h"

#ifndef MAXHOSTNAMELEN
#  define MAXHOSTNAMELEN (256+64)       /* be save!                          */
#endif

typedef struct {
   unsigned    entries;
   u_long     *hosts;
} TCPIP_ALLOW;

volatile int CommSock = -1;             /* currenty opened socket to a client*/
static int MainSock = -1;               /* main communication socket         */
static int NameServerProblem = 0;       /* flag used to determine problems   */
static u_long IPofThisHost = INADDR_NONE; /* IP number of your adapter       */
static TCPIP_ALLOW AccessList = {0,NULL}; /* current access list             */

/*****************************************************************************/
/*  function name : CheckTCPIPAvail                                          */
/*                                                                           */
/*  description   : checks the availability of TCP/IP and sets the global    */
/*                  flag. Opens the MainSock.                                */
/*****************************************************************************/
void CheckTCPIPAvail(void)
{
   struct sockaddr_in si;
   int on = 1;

   TCPIPAvail = 0;                      /* TCP/IP not supported on default   */
   if ((MainSock = socket(AF_INET,SOCK_STREAM,PF_UNSPEC)) < 0)
      return;
   if (setsockopt(MainSock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0) {
      close(MainSock);
      return;
   }
   memset(&si,0,sizeof(si));
   si.sin_family      = AF_INET;
   si.sin_port        = htons(CRON_TCPIP_PORT);
   si.sin_addr.s_addr = INADDR_ANY;
   if (bind(MainSock,(struct sockaddr *) &si,sizeof(si)) < 0) {
      close(MainSock);
      return;
   }
   if (listen(MainSock,1) < 0) {        /* allow only one host to await a    */
      close(MainSock);                  /* connection                        */
      return;
   }
   IPofThisHost = gethostid();
   TCPIPAvail = 1;                      /* TCP/IP supported                  */
}

/*****************************************************************************/
/*  function name : ResolveIPNumberFromHost                                  */
/*                                                                           */
/*  arguments     : hostname, buffer to receive the corresponding IP         */
/*                  number of the host                                       */
/*                                                                           */
/*  return value  : 0 on success, -1 on error, 1 in case of problems with    */
/*                  the name server                                          */
/*                                                                           */
/*  description   : tries to get the IP number of the host given in          */
/*                  hostname. Sets the buffer of the IP number on success.   */
/*****************************************************************************/
static int ResolveIPNumberFromHost(const char *hostname,u_long *ipno)
{
   u_long retval;
   struct hostent *host;
   extern int h_errno;                  /* currently not found in any include*/
                                        /* file                              */

   if ((retval = inet_addr(hostname)) != INADDR_NONE) {
      *ipno = retval;
      return(0);
   }
                                        /* hostname is not a number sequence?*/
   host = gethostbyname(hostname);
   if ((host != NULL) && (host->h_length >= sizeof(u_long))) {
      *ipno = *((u_long*) host->h_addr_list[0]);
      return(0);
   }
                                        /* maybe, the nameserver or the      */
                                        /* nameservice is down. In this case */
                                        /*we return a 1 to show an acceptable*/
                                        /* error                             */
   if (h_errno != HOST_NOT_FOUND)
      return(1);
   return(-1);
}

/*****************************************************************************/
/*  function name : ResolveHostFromIP                                        */
/*                                                                           */
/*  arguments     : IP number of the host, buffer to hold the human          */
/*                  readable host name.                                      */
/*                                                                           */
/*  description   : fills the buffer with the host name of the given IP      */
/*                  address.                                                 */
/*                                                                           */
/*  note          : the buffer should be large enough (MAXHOSTNAMELEN)       */
/*****************************************************************************/
static void ResolveHostFromIP(u_long ipno,char *fullname)
{
   char *p;
   struct in_addr ia;
   struct hostent *host;

   host = gethostbyaddr((char *) &ipno,sizeof(u_long),AF_INET);
   if (host != NULL)
      strcpy(fullname,host->h_name);
   else {
      ia.s_addr = ipno;
      if ((p = inet_ntoa(ia)) != NULL)
         strcpy(fullname,p);
      else
         strcpy(fullname,Get(IDS_UnknownHost));
   }
}

/*****************************************************************************/
/*  function name : InsertIPEntry                                            */
/*                                                                           */
/*  arguments     : string with a new TCP/IP host, its length, head of the   */
/*                  list of allowed host names.                              */
/*                                                                           */
/*  return value  : 0 on success, errno otherwise                            */
/*                                                                           */
/*  description   : duplicates and parses the string and appends it to the   */
/*                  list. This function checks the validity and sets all     */
/*                  necessary entry values, too.                             */
/*****************************************************************************/
static int InsertIPEntry(const char *s,size_t len,TCPIP_ALLOW *head)
{
   int err;
   char c;
   u_long ipno;
   char ReadInBuf[MAXHOSTNAMELEN];

   while (isspace(*s) && (len > 0)) {   /* cut off leading and trailing      */
      s++;                              /* whitespaces                       */
      len--;
   }
   while (len > 0) {
      if (isspace(s[len - 1]))
         len--;
      else
         break;
   }
   if (len == 0)                        /* empty line?                       */
      return(0);
   if ((*s == '#') || (*s == ';'))      /* comment line?                     */
      return(0);
   if (len >= MAXHOSTNAMELEN)
      return(EINVAL);

   memcpy(ReadInBuf,s,len);             /* may be valid, copy and check for  */
   ReadInBuf[len] = 0;                  /* spaces, only one entry per line   */
   s = ReadInBuf;                       /* is allowed                        */
   while ((c = *s++) != 0)
      if (isspace(c))
         return(EINVAL);

   if ((err = ResolveIPNumberFromHost(ReadInBuf,&ipno)) != 0) {
      if (err == 1) {
         NameServerProblem = 1;
         return(0);
      }
      return(EINVAL);
   }
                                        /* the ipno is valid, insert it into */
                                        /* the list                          */
   if (head->hosts == NULL) {           /* never allocated?                  */
      if ((head->hosts = malloc(64 * sizeof(u_long))) == NULL)
         return(ENOMEM);
   } else if (head->entries >= 64) {    /* default list is full?             */
      if ((head->hosts = realloc(head->hosts,(head->entries + 1) *
                                                      sizeof(u_long))) == NULL)
         return(ENOMEM);
   }
   head->hosts[head->entries++] = ipno;
   return(0);
}

/*****************************************************************************/
/*  function name : ReadIPNames                                              */
/*                                                                           */
/*  arguments     : handle of an open file, head of the list of hosts        */
/*                  which are allowed to use this cron daemon via TCP/IP.    */
/*                                                                           */
/*  return value  : 0 on success, errno otherwise (will be set!)             */
/*                                                                           */
/*  description   : reads a TCP/IP allow file and builds a new list. The     */
/*                  file must be opened in O_BINARY mode.                    */
/*****************************************************************************/
static int ReadIPNames(int handle,TCPIP_ALLOW *head)
{
   char *buf;
   const char *ptr;
   long flen;
   size_t pos,len;
   int err,htype;

   ioctl(handle,FGETHTYPE,&htype);
   if (htype == HT_DEV_NUL)             /* we don't have to do anything      */
      return(0);                        /* while reading the "nul" device    */
   if ((flen = filelength(handle)) == -1l)      /* device?                   */
      return(errno);
   if ((lseek(handle,0l,SEEK_SET)) == -1l)      /* device?                   */
      return(errno);
   if (flen == 0)                       /* empty file?                       */
      return(0);
   if (flen >= MAX_CRONFILESIZE)        /* potential mistake of the user?    */
      return(errno = EACCES);
   if ((buf = malloc((size_t) flen + 1)) == NULL)  /* read the complete file */
      return(errno = ENOMEM);           /* in one operation                  */
   errno = 0;
   if ((long) read(handle,buf,(size_t) flen) != flen) {  /* do the operation */
      if (errno == 0)
         errno = EIO;
      free(buf);
      return(errno);
   }
   buf[(size_t) flen] = 0;              /* terminate the buffer              */
                                        /* now we can use our fast function  */
   pos = 0;
   while ((ptr = ParseLine(buf,(size_t) flen,&pos,&len)) != NULL) {
      if ((err = InsertIPEntry(ptr,len,head)) != 0) {
         free(buf);                     /* error? free up the buffer and     */
         if (head->hosts != NULL) {     /* the host list                     */
            free(head->hosts);
            head->hosts = NULL;
         }
         return(errno = err);
      }
   }
   free(buf);
   return(0);
}

/*****************************************************************************/
/*  function name : ReadAllow                                                */
/*                                                                           */
/*  arguments     : new name of the TCP/IP-Access-Allow-File                 */
/*                                                                           */
/*  return value  : 1 on success, 0 otherwise                                */
/*                                                                           */
/*  description   : reads a file of TCP/IP host names. If this is done       */
/*                  sucessfully, the default list is set to the new one      */
/*                  and the old one is deleted.                              */
/*                                                                           */
/*  note          : only "nul" and regular files are allowed.                */
/*****************************************************************************/
int ReadAllow(char *s)
{
   TCPIP_ALLOW NewHead, oldhead;
   int handle,htype = HT_DEV_OTHER;

   if ((handle = sopen(s,O_BINARY | O_RDONLY | O_NOINHERIT,SH_DENYWR)) == -1)
      return(0);
   ioctl(handle,FGETHTYPE,&htype);
   if ((htype != HT_FILE) && (htype != HT_DEV_NUL)) { /* not "nul" or regular*/
      close(handle);                    /* file? We won't read "KBD$" !!!    */
      return(0);
   }
   NameServerProblem = 0;               /* reset this flag                   */
   memset(&NewHead,0,sizeof(NewHead));  /* build the new list                */
   if (ReadIPNames(handle,&NewHead) != 0) {  /* error interpreting file?     */
      close(handle);
      return(0);
   }
   close(handle);

   oldhead = AccessList;
   AccessList = NewHead;
   if (oldhead.hosts != NULL)
      free(oldhead.hosts);
   if (NameServerProblem)
      PMError(Get(IDS_NameServerProblem),(HWND) 0);
   return(1);
}

/*****************************************************************************/
/*  function name : IsHostInAccessList                                       */
/*                                                                           */
/*  arguments     : IP number of a host                                      */
/*                                                                           */
/*  return value  : 1 if the host is in the access list, 0 otherwise         */
/*                                                                           */
/*  description   : checks wether the IP-Number of the host is within        */
/*                  the access list of TCP/IP hosts.                         */
/*                                                                           */
/*  note          : the localhost (127.0.0.1) and the IP number of the       */
/*                  current host return 1 always                             */
/*****************************************************************************/
static int IsHostInAccessList(u_long ipno)
{
   unsigned i;
   if (ipno == INADDR_LOOPBACK)
      return(1);
   if ((ipno == IPofThisHost) && (IPofThisHost != INADDR_NONE))
      return(1);
   if ((ProgramFlags & PRG_ALLOW_TCPIP) == 0)
      return(0);
   for (i = 0;i < AccessList.entries;i++)
      if (ipno == AccessList.hosts[i])
         return(1);
   return(0);
}

/*****************************************************************************/
/*  function name : SocketThread                                             */
/*                                                                           */
/*  arguments     : not needed, must be void *                               */
/*                                                                           */
/*  description   : this is a thread function. It processes all requests     */
/*                  to the already opened main socket.                       */
/*                  Stops running if GlobalStop is set. The function         */
/*                  requests an automatic disconnect by the cron thread      */
/*                  avoiding any user to block the socket for a long time.   */
/*                                                                           */
/*  note          : should be called via _beginthread                        */
/*****************************************************************************/
void SocketThread(void *dummy)
{
   int size;
   struct sockaddr_in partner;
   char partner_name[MAXHOSTNAMELEN];
   static struct linger linger = {1 /* l_onoff */, 5 /* l_linger=5 sec */ };


   do {                                 /* while (!GlobalStop)               */
      if (GlobalStop)                   /* check at mostly every place...    */
         break;
      size = sizeof(partner);
                                        /* wait for a client                 */
      CommSock = accept(MainSock,(struct sockaddr *) &partner,&size);
      if (CommSock < 0) {               /* interrupted?                      */
         if (GlobalStop)
            break;
         continue;
      }
      ResolveHostFromIP(partner.sin_addr.s_addr,partner_name);
      if (IsHostInAccessList(partner.sin_addr.s_addr))
         Message(Get(IDS_TCPIPConnection),partner_name);
      else {
         Message(Get(IDS_TCPIPRejected),partner_name);
         close(CommSock);
         CommSock = -1;
         continue;
      }
      NewProgramStatus(IDS_StatusCommunication);
      AutoCloseSocket = time(NULL) + 60;  /* let the socket close after a    */
                                        /* minute                            */
      DosPostEventSem(CronSem);         /* cron thread shall kill the pipe   */
                                        /* after AutoDisConnect              */
      ProcessRequest(&CommSock,partner_name);   /* do the work               */
      if ((AutoCloseSocket != (time_t) -1l) && (!GlobalStop))
         setsockopt(CommSock,SOL_SOCKET,SO_LINGER,&linger,sizeof(linger));
                                        /* wait for the client to close the  */
                                        /* socket. Otherwise, the client may */
                                        /* loose some data send.             */
      AutoCloseSocket = (time_t) -1l;   /* don't let close the socket        */
      NewProgramStatus(IDS_StatusNormal);
      if (GlobalStop)
         break;
      close(CommSock);
      CommSock = -1;
      DosPostEventSem(CronSem);         /* inform the cron thread that there */
                                        /* is no need to close the socket    */
                                        /* any longer                        */
   } while (!GlobalStop);
}

/* RCS depending informations
 *
 * $Name: Version121 $
 *
 * $Log: tcpip.c $
 * Revision 1.1  1995/03/15 09:07:34  Florian
 * Initial revision
 *
 *
 */
static char rcsid[] = "@(#)$Id: tcpip.c 1.1 1995/03/15 09:07:34 Florian Rel $";
