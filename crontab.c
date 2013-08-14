/* Copyright (c) 1995 Florian Gro·e-Coosmann, RCS section at the eof         */
/* crontab, the communication tool for the crond daemon.                     */
/* We want to allow the compilation not only under OS/2. Thus we don't use   */
/* the .RC-files to hold strings!                                            */
/* There is another unsolved and probably unsolvable problem: The language   */
/* depending output strings depend on one or more preselected codepages.     */
/* I hope the user has selected the right while setting LANG to his/her      */
/* preferred value.                                                          */
/* I have a font display tool using VioTBL.DCP. Ask for it in case of        */
/* problems. Florian.                                                        */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include <fcntl.h>
#include <io.h>
#ifdef TCPIP_SUPPORT
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <sys/ioctl.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#endif
#include "client.h"

char *Buf;
size_t BufSize = 0x1000;
int hf = -1;
char *Destination = NULL;
char DefPipename[] = DEF_PIPENAME;

#define ENGLISH            0
#define GERMAN             1
#define FRENCH             2

typedef struct {
   unsigned    code;                    /* ENGLISH, GERMAN, etc              */
   char       *LANG_descr;              /* values codes from env-var LANG    */
   char       *description;             /* full description                  */
} LANGUAGE;

const LANGUAGE Languages[] = {
   {ENGLISH,   LC_C_UK,       "English"},
   {ENGLISH,   LC_C_USA,      "English"},
   {GERMAN,    LC_C_GERMANY,  "Deutsch"},
   {FRENCH,    LC_C_FRANCE,   "Franáais"}
};
const unsigned LanguagesCount = sizeof(Languages) / sizeof(Languages[0]);
unsigned Language = 0;

#define S_NOMEM            0
#define S_COMMERR          1
#define S_CONNCLOSED       2
#define S_LARGEARG         3
#define S_CRONDNOTSTARTED  4
#define S_CRONDBUSY        5
#define S_TRANSERR         6
#define S_WRONGANSWER      7
#define S_READERR          8

/*****************************************************************************/
/*  function name : GetString                                                */
/*                                                                           */
/*  arguments     : number of the requested string. Use the S_... values.    */
/*                                                                           */
/*  return value  : the requested string or "???" in case on an error        */
/*                                                                           */
/*  description   : returns the string. The language identifier is used to   */
/*                  return the right string for the locale.                  */
/*****************************************************************************/
const char *GetString(unsigned Id)
{
   switch (Id) {
      case S_NOMEM: switch (Language) {
            case GERMAN:   return("Zuwenig freier Speicher\r\n");
            case FRENCH:   return("MÇmoire insuffisance\r\n");
            case ENGLISH:
            default:       return("Not enough free memory\r\n");
         }
      case S_COMMERR: switch (Language) {
            case GERMAN:   return("Der Kommunikationszugriff zu crond liefert "
                                                          "den Fehler %s\r\n");
            case FRENCH:   return("La communication avec cron retourne"
                                  " l'erreur %s\r\n");
            case ENGLISH:
            default:       return("The communication access to cron returns "
                                                           "the error %s\r\n");
         }
      case S_CONNCLOSED: switch (Language) {
            case GERMAN:   return("Crond hat die Verbindung abgebrochen\r\n");
            case FRENCH:   return("Crond a fermÇ la connexion\r\n");
            case ENGLISH:
            default:       return("Crond has closed the connection\r\n");
         }
      case S_LARGEARG: switch (Language) {
            case GERMAN:   return("Die Summe der Zeichen in den Argumenten ist"
                                                          " grî·er als %lu\r\n"
                                  "Bitte reduzieren!\r\n");
            case FRENCH:   return("La somme des caractäres dans les arguments"
                                  " dÇpasse %lu\r\n"
                                  "Veuillez diminuer !\r\n");
            case ENGLISH:
            default:       return("The sum of the characters in the arguments "
                                                              "exceeds %lu\r\n"
                                  "please reduce!\r\n");
         }
      case S_CRONDNOTSTARTED: switch (Language) {
            case GERMAN:   return("Crond wurde (auf dem angegebenen Rechner) "
                                                         "nicht gestartet,\r\n"
                                  "oder der Rechner kann im Netz nicht "
                                                       "erreicht werden.\r\n");
            case FRENCH:   return("Crond n'a pas ÇtÇ lancÇ (sur l'ordinateur"
                                  " spÇcifiÇ),\r\n"
                                  "ou bien l'ordinateur ne peut àtre joint"
                                  " sur le rÇseau.\r\n");
            case ENGLISH:
            default:       return("Crond has not been started (on the given "
                                                               "computer),\r\n"
                                  "or the computer can't be reached in the "
                                                               "network.\r\n");
         }
      case S_CRONDBUSY: switch (Language) {
            case GERMAN:   return("Probieren Sie es erneut, crond ist gerade "
                                                           "beschÑftigt.\r\n");
            case FRENCH:   return("Veuillez rÇ-essayer. Crond est occupÇ"
                                  " en ce moment.\r\n");
            case ENGLISH:
            default:       return("Please retry. Currently, Crond is "
                                                                  "busy.\r\n");
         }
      case S_TRANSERR: switch (Language) {
            case GERMAN:   return("Es konnten nur %d von %lu Byte zu crond "
                                                     "Åbertragen werden.\r\n");
            case FRENCH:   return("Seulement %d octets sur %lu on pu àtre"
                                  " transmis Ö crond.\r\n");
            case ENGLISH:
            default:       return("Only %d of %lu bytes could be transferred "
                                                              "to crond.\r\n");
         }
      case S_WRONGANSWER: switch (Language) {
            case GERMAN:   return("Crond liefert falsche Informationen, "
                                              "verweigert den Zugriff oder\r\n"
                                  "die Kommunikation wurde gestîrt.\r\n");
            case FRENCH:   return("Crond renvoie des informations incorrectes,"
                                  " refuse l'accäs ou bien la communication"
                                  " est brouillÇe.\r\n");
            case ENGLISH:
            default:       return("Crond returns the wrong informations, "
                                                 "denies the access or the\r\n"
                                  "communication has been jammed.\r\n");
         }
      case S_READERR: switch (Language) {
            case GERMAN:   return("(Nur %d von %lu Zeichen wurden "
                                                            "empfangen.)\r\n");
            case FRENCH:   return("Seulement %d octets sur %lu ont ÇtÇ"
                                  " reáus.\r\n");
            case ENGLISH:
            default:       return("Only %d of %lu bytes have been "
                                                             "received.)\r\n");
         }
   }
   return("???");                       /* default value                     */
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
/*                  wether the pipe has been closed.                         */
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
/*  function name : Handler                                                  */
/*                                                                           */
/*  arguments     : signal number generated by the runtime system            */
/*                                                                           */
/*  description   : This is a signal routine. This routine is called by a    */
/*                  ^C or Break. If the pipe or socket is opened, it is      */
/*                  will be closed. Otherwise or on the next call we die     */
/*                  normally.                                                */
/*                                                                           */
/*  note          : don't call directly                                      */
/*****************************************************************************/
void Handler(int sig)
{
   if (hf != -1) {
      close(hf);
      hf = -1;
   } else
      exit(1);
   signal(sig,SIG_ACK);
}

/*****************************************************************************/
/*  function name : NoMem                                                    */
/*                                                                           */
/*  description   : Prints an error message (not enough memory) and dies.    */
/*****************************************************************************/
void NoMem(void)
{
   printf(GetString(S_NOMEM));
   exit(-1);
}

/*****************************************************************************/
/*  function name : CommErr                                                  */
/*                                                                           */
/*  arguments     : errno code                                               */
/*                                                                           */
/*  description   : displays the translated errno code as a result of a      */
/*                  communication error with the daemon and dies. If the     */
/*                  pipe or socket is already closed we assume an            */
/*                  interrupt by the user. Thus, we don't display the        */
/*                  string (but die).                                        */
/*****************************************************************************/
void CommErr(int errnocode)
{
   char strerrorbuf[256],*ptr;
   if (hf == -1)
      exit(3);
   strcpy(strerrorbuf,strerror(errnocode));
   if ((ptr = strpbrk(strerrorbuf,"\r\n")) != NULL)
      *ptr = 0;
   printf(GetString(S_COMMERR),strerrorbuf);
   exit(3);
}

/*****************************************************************************/
/*  function name : ConnectionClosed                                         */
/*                                                                           */
/*  description   : displays the the string "crond has closed the            */
/*                  connection" and dies. If the pipe or socket is           */
/*                  already closed we assume an interrupt by the user.       */
/*                  Thus, we don't display the string (but die).             */
/*****************************************************************************/
void ConnectionClosed(void)
{
   if (hf == -1)
      exit(3);
   printf(GetString(S_CONNCLOSED));
   exit(3);
}

/*****************************************************************************/
/*  function name : PipeHandler                                              */
/*                                                                           */
/*  arguments     : signal number generated by the runtime system            */
/*                                                                           */
/*  description   : This is a signal routine. This routine is called by a    */
/*                  SIGPIPE. We call ConnectionClosed, see above.            */
/*                                                                           */
/*  note          : don't call directly                                      */
/*****************************************************************************/
void PipeHandler(int sig)
{
   signal(sig,SIG_ACK);
   ConnectionClosed();
}

/*****************************************************************************/
/*  function name : GermanHelp                                               */
/*                                                                           */
/*  description   : displays help in German and dies.                        */
/*                                                                           */
/*  note          : see Help                                                 */
/*****************************************************************************/
void GermanHelp(void)
{
   printf("Gebrauch:\r\n"
          "\tcrontab [\"@\"Rechner] [\"Once\"] Zeiten Kommando\r\n"
          "oder\tcrontab [\"@\"Rechner] \"Delete\" Nummer\r\n"
          "oder\tcrontab [\"@\"Rechner] \"Show\"\r\n"
          "Mit @Rechner kann man einen anderen cron-DÑmon-Rechner angeben.\r\n"
#ifdef TCPIP_SUPPORT
          "Dabei wird zuerst der LAN-Weg und dann der TCP/IP-Weg probiert.\r\n"
#endif
          "Bei \"Show\" werden einen die aktuellen cron-jobs mit Nummern "
                                                               "angezeigt.\r\n"
          "Mit \"Delete\" Nummer lÑ·t sich ein bestimmter Job lîschen.\r\n"
          "Die Nummer kann mit \"Show\" ermittelt werden.\r\n"
          "\r\n"
          "Mit der ersten Darstellung wird ein Job an den cron-DÑmon "
                                                          "weitergeleitet,\r\n"
          "der zu dem gewissen Zeitpunkt oder Zeitpunkten abgearbeitet wird."
                                                                         "\r\n"
          "Als Kommando darf dabei jedes beliebige Kommando dienen.\r\n"
          "Die Zeiten setzen sich aus 5 Feldern zusammen:\r\n"
          "  a) Minute (Werte zwischen 0 und 59)    b) Stunde  (Werte zwischen"
                                                               " 0 und 23)\r\n"
          "  c) Tag    (Werte zwischen 1 und 31)    d) Monat   (Werte zwischen"
                                                               " 1 und 12)\r\n"
          "  e) Wochentag  (Werte zwischen 0 und 6, 0 = Sonntag)\r\n"
          "Jede der einzelnen Angaben darf eine durch Kommata getrennte "
                                                            "Intervalliste\r\n"
          "sein, jede Angabe darf auch durch einen Stern (*) fÅr \"jedesmal\" "
                                                                  "ersetzt\r\n"
          "werden.\r\n"
          "\r\n"
          "Ist \"Once\" angegeben, wird der Auftrag einmal ausgefÅhrt und "
                                                         "danach gelîscht.\r\n"
          "Die Spezialzeiten \"CronStart\", \"CronStop\" und \"Daily\" "
                                                                 "bedeuten\r\n"
          "\"beim Start des cron-DÑmon\", \"beim Ende des cron-DÑmon\" und\r\n"
          "\"einmal tÑglich zum frÅhestmîglichen Zeitpunkt\".\r\n"
          "\r\n"
          "Beispiel: Mit \"crontab 0,20,40 * * * 6 Kommando\" wird jeden "
                                                             "Samstag alle\r\n"
         "\t  20 Minuten Kommando ausgefÅhrt.\r\n"
          "\r\n"
         "Bei Programmen mit Benutzerinteraktion sollte \"start\" benutzt "
                                                                  "werden!\r\n"
         "\r\n"
         "Die Sprache dieses Programms kann Åber die Umgebungsvariable "
                                                      "\"LANG\" ausgewÑhlt\r\n"
         "werden:\r\n"
         "      Wert | Sprache\r\n"
         );
}

/*****************************************************************************/
/*  function name : EnglishHelp                                              */
/*                                                                           */
/*  description   : displays help in English and dies.                       */
/*                                                                           */
/*  note          : see Help                                                 */
/*****************************************************************************/
void EnglishHelp(void)
{
   printf("usage:\r\n"
          "\tcrontab [\"@\"Computer] [\"Once\"] Times Command\r\n"
          "or\tcrontab [\"@\"Computer] \"Delete\" Number\r\n"
          "or\tcrontab [\"@\"Computer] \"Show\"\r\n"
          "With @Computer another machine could be selected.\r\n"
#ifdef TCPIP_SUPPORT
          "First, the LAN then the TCP/IP connection will be tried.\r\n"
#endif
          "With \"Show\" the current cron-jobs will be displayed.\r\n"
          "With \"Delete\" Number a given job will be deleted.\r\n"
          "The number can be determined with a \"Show\" command.\r\n"
          "\r\n"
          "With the first usage a job will be send to the cron daemon. It will"
                                                             " be executed\r\n"
          "at a given time or given times.\r\n"
          "Every possible command may be used as Command.\r\n"
          "The Times field consists of 5 entries:\r\n"
          "  a) minute (values between 0 and 59)    b) hour  (values between 0"
                                                                 " and 23)\r\n"
          "  c) day    (values between 1 and 31)    d) month (values between 1"
                                                                 " and 12)\r\n"
          "  e) day of the week (values between 0 and 6, 0 = sunday)\r\n"
          "Every entry may be a comma delimited interval list. Every entry "
                                                                 "may be a\r\n"
          "star (*), its meaning is \"every possible value\".\r\n"
          "\r\n"
          "If \"Once\" is supplied, the job will be executed only once, then "
                                                               "it will be\r\n"
          "deleted.\r\n"
          "The meaning of the special times \"CronStart\", \"CronStop\" and "
                                                            "\"Daily\" are\r\n"
          "\"at start of the cron daemon\", \"at stop of the cron daemon\" "
                                                                      "and\r\n"
          "\"daily once at the earliest possible time\".\r\n"
          "\r\n"
          "example: With \"crontab 0,20,40 * * * 6 command\" on every "
                                                    "saturday command will\r\n"
          "\t be executed every 20 minutes.\r\n"
          "\r\n"
          "\"start\" should be used to executed programs with user "
                                                             "interaction.\r\n"
          "\r\n"
          "The language of this program may be selected by the environment "
                                                                 "variable\r\n"
          "\"LANG\":\r\n"
          "     value | language\r\n"
          );
}

/*****************************************************************************/
/*  function name : FrenchHelp                                               */
/*                                                                           */
/*  description   : displays help in French and dies.                        */
/*                                                                           */
/*  note          : see Help                                                 */
/*****************************************************************************/
void FrenchHelp(void)
{
   printf("Utilisationˇ:\r\n"
          "\tcrontab [Æ@ØOrdinateur] [ÆOnceØ] Heures Commande\r\n"
          "ou\tcrontab [Æ@ØOrdinateur] ÆDeleteØ NumÇro\r\n"
          "ou\tcrontab [Æ@ØOrdinateur] ÆShowØ\r\n"
          "@Ordinateur permet de s'adresser Ö une machine distante.\r\n"
#ifdef TCPIP_SUPPORT
          "Les connexions rÇseau (LAN) puis TCP/IP seront recherchÇes.\r\n"
#endif
          "ÆShowØ affiche les ÇvÇnements cron dÇfinis.\r\n"
          "ÆDeleteØ supprime un ÇvÇnement.\r\n"
          "Le numÇro peut àtre dÇterminÇ par une commande ÆShowØ.\r\n"
          "N'importe quelle commande peut àtre utilisÇe comme Commande.\r\n"
          "Le paramätre Heures se compose de 5 entrÇesˇ:\r\n"
          "  a) minute (de 0 Ö 59)    b) heure  (de 0 Ö 23)\r\n"
          "  c) jour   (de 1 Ö 31)    d) mois   (de 1 Ö 12)\r\n"
          "  e) jour de la semaine (de 0 Ö 6, 0 = dimanche)\r\n"
          "Chaque entrÇe peut àtre une liste d'ÇlÇments sÇparÇs par des "
                                                                "virgules.\r\n"
          "Une valeur * signifie Ætoutes les valeursØ.\r\n"
          "\r\n"
          "Si ÆOnceØ est prÇcisÇ, l'ÇvÇnement ne sera exÇcutÇ qu'une fois, "
                                                           "puis supprimÇ.\r\n"
          "Les heures spÇciales ÆCronStartØ, ÆCronStopØ et ÆDailyØ "
                                                               "signifient\r\n"
          "Æau lancement de cronØ, ÆÖ l'arràt de cronØ et Æune fois par "
                                                                     "jour\r\n"
          "aussi tìt que possibleØ.\r\n"
          "\r\n"
          "exempleˇ: avec Æcrontab 0,20,40 * * * 6 commandeØ, commande "
          "sera\r\n"
          "exÇcutÇe toutes les 20 minutes chaque samedi.\r\n"
          "\r\n"
          "ÆstartØ doit àtre utilisÇ pour les programmes nÇcessitant une\r\n"
          "intervention de l'utilisateur.\r\n"
          "\r\n"
          "La langue que pratique ce programme peut àtre choisie par la "
          "variable ÆLANGØˇ:\r\n"
          "    valeur | langue\r\n"
          );
}

/*****************************************************************************/
/*  function name : Help                                                     */
/*                                                                           */
/*  description   : displays help and dies.                                  */
/*                  The language specific help is displayed.                 */
/*****************************************************************************/
void Help(void)
{
   unsigned i;
   switch (Language) {
      case GERMAN:
         GermanHelp();
         break;

      case FRENCH:
         FrenchHelp();
         break;

      case ENGLISH:
      default:
         EnglishHelp();
         break;
   }
   printf("      -----+--------\r\n");
   for (i = 0;i < LanguagesCount;i++)
      printf("%10s | %s\r\n",Languages[i].LANG_descr,Languages[i].description);
   exit(3);
}

/*****************************************************************************/
/*  function name : BuildRequest                                             */
/*                                                                           */
/*  arguments     : arg-arguments from main                                  */
/*                                                                           */
/*  return value  : 1 on success, 0 in case of invalid arguments             */
/*                                                                           */
/*  description   : checks the arguments and builds an request that would    */
/*                  probably accepted by the daemon. The final checking      */
/*                  for the validity of the arguments is done by the         */
/*                  daemon.                                                  */
/*****************************************************************************/
int BuildRequest(int argc,char **argv)
{
   int i;
   char *s;
   if ((argc > 1) && (argv[1][0] == '@')) {  /* destination computer given?  */
      s = argv[1] + 1;                  /* jump over @                       */
      if (*s == 0)                      /* nothing more?                     */
         return(0);
      if ((Destination = malloc(strlen(DefPipename) + strlen(s) + 3)) == NULL)
         NoMem();
      strcpy(Destination,"\\\\");       /* build a new pipe name             */
      strcat(Destination,s);
      strcat(Destination,DefPipename);
      argc--;
      argv++;
   }
   if (argc < 2)                        /* empty command?                    */
      return(0);
   if (stricmp(argv[1],"Delete") == 0) {  /* delete a job?                   */
      if (argc != 3)                    /* only expect one more argument     */
         return(0);
      strcpy(Buf,"D ");                 /* code of "Delete"                  */
      strcat(Buf,argv[2]);
      return(1);
   } else if (stricmp(argv[1],"Show") == 0) {   /* display jobs?             */
      if (argc != 2)                    /* don't expect any argument         */
         return(0);
      strcpy(Buf,"R");                  /* code of "Read list"               */
      return(1);
   }
   if (argc < 2)                        /* new job, expect at least on more  */
      return(0);                        /* argument (they may be quoted      */
                                        /* together)                         */
   strcpy(Buf,"N");                     /* code of "New job"                 */
   for (i = 1;i < argc;i++) {
      strcat(Buf," ");                  /* add them                          */
      if (strlen(argv[i]) + strlen(Buf) + 1 > BufSize) { /* string too long? */
         printf(GetString(S_LARGEARG),(unsigned long) BufSize);
         exit(3);
      }
      strcat(Buf,argv[i]);
   }
   return(1);
}

/*****************************************************************************/
/*  function name : IsRequestPreAnswer                                       */
/*                                                                           */
/*  arguments     : string to check, buffer to hold the new answer size on   */
/*                  success.                                                 */
/*                                                                           */
/*  return value  : 1 on success, 0 otherwise                                */
/*                                                                           */
/*  description   : tries to interpret the string as a length description    */
/*                  from the daemon. Its form has to be                      */
/*                  "crond: xxx Data OK<WhiteSpace>" with xxx as a           */
/*                  positive number. In this case the supplied buffer is     */
/*                  filled ond 1 returned.                                   */
/*****************************************************************************/
int IsRequestPreAnswer(char *s,size_t *bytes)
{
   char buf[101];
   static char DataOK[] = "Data OK";
   char *ptr,*ptr2;
   unsigned long val;
   if (strlen(s) > sizeof(buf) - 1)     /* too many chars?                   */
      return(0);
   strcpy(buf,s);                       /* work on a local buffer.           */
   if (strnicmp(buf,"crond:",6) != 0)   /* check the beginning               */
      return(0);
   ptr = buf + 6;
   errno = 0;
   val = strtoul(ptr,&ptr2,10);         /* try to interpret a number         */
   if ((errno) || (val == 0) || (ptr == ptr2))  /* not a number?             */
      return(0);
   while (isspace(*ptr2))               /* jump over whitespaces             */
      ptr2++;

   if (strnicmp(ptr2,DataOK,strlen(DataOK)) != 0)  /* check the ending       */
      return(0);
   ptr2 += strlen(DataOK);
   while (isspace(*ptr2))               /* ,jump over whitespaces            */
      ptr2++;
   if (*ptr2 != '\0')                   /* more characters in the string?    */
      return(0);
   *bytes = (size_t) val;               /* OK, the string is accepted as a   */
   return(1);                           /* size string                       */
}

#ifdef TCPIP_SUPPORT
/*****************************************************************************/
/*  function name : open_socket                                              */
/*                                                                           */
/*  arguments     : string describing the destination in pipe notation       */
/*                                                                           */
/*  return value  : handle to a newly opened socket or -1 in case of an      */
/*                  error                                                    */
/*                                                                           */
/*  description   : tries to connect to the crond port number of the given   */
/*                  computer. The argument is a string of the form           */
/*                  `\\computername\pipe\pipename'. We extract the           */
/*                  computername, connect to it and return a handle of the   */
/*                  socket.                                                  */
/*****************************************************************************/
int open_socket(const char *s)
{
   int sock,off = 0,on = 1;
   char *name;
   struct hostent *host;
   unsigned long ip_no;
   struct sockaddr_in server;

#ifdef __EMX__
   if ((_emx_env & 0x200) == 0)         /* running under DOS?                */
      return(-1);                       /* socket are not supported          */
   else {                               /* running under OS/2?               */
      if (_emx_vcmp < 0x302e3961)       /* < 0.9a                            */
         return(-1);
   }
#endif
   if ((s[0] != '\\') || (s[1] != '\\'))  /* no computername given           */
      return(-1);
   if ((name = strdup(s + 2)) == NULL)  /* jump over the leading backslashes */
      NoMem();
   if (strchr(name,'\\') != NULL)       /* extract the computername          */
      *strchr(name,'\\') = '\0';

   memset(&server,0,sizeof(server));
   server.sin_family = AF_INET;
   server.sin_port = htons(CRON_TCPIP_PORT);
   ip_no = inet_addr(name);
   if (ip_no != INADDR_NONE)
      server.sin_addr.s_addr = ip_no;
   else {
      if ((host = gethostbyname(name)) == NULL) {
          free(name);
          return(-1);
      }
      memcpy(&server.sin_addr,host->h_addr,host->h_length);
   }
   free(name);

   if ((sock = socket(AF_INET,SOCK_STREAM,PF_UNSPEC)) < 0)
      return(-1);

   setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

   if (connect(sock,(struct sockaddr *) &server,sizeof(server)) < 0)
      return(-1);

   ioctl(sock,FIONBIO,&off);            /* don't use "non blocking IO"       */
   return(sock);
}
#endif

int main(int argc,char **argv)
{
   size_t w,wanted;
   int done = 0;
   unsigned i;
   char *ptr;
   setmode(1,O_BINARY);
   setmode(2,O_BINARY);
   signal(SIGINT,Handler);
   signal(SIGBREAK,Handler);
   signal(SIGPIPE,PipeHandler);

   Language = ENGLISH;                  /* Default in case of an error       */
   ptr = getenv("LANG");
   if (ptr != NULL) {
      for (i = 0;i < LanguagesCount;i++)
         if (strcmp(Languages[i].LANG_descr,ptr) == 0)
            Language = Languages[i].code;
   }

   if ((Buf = malloc(BufSize)) == NULL)
      NoMem();
   if (!BuildRequest(argc,argv))        /* invalid arguments?                */
      Help();

   if (Destination == NULL)
      Destination = DefPipename;
   if ((hf = open(Destination,O_BINARY | O_RDWR)) == -1)
      {
         switch (errno) {
            case ENOENT:                /* daemon not started                */
#ifdef TCPIP_SUPPORT
               if ((hf = open_socket(Destination)) != -1)   /* can open via  */
                  break;                /* TCP/IP?                           */
#endif
               printf(GetString(S_CRONDNOTSTARTED));
               return(2);
            case EPIPE:
            case EAGAIN:
            case EBUSY:                 /* daemon doesn't accept a communic. */
               printf(GetString(S_CRONDBUSY));
               return(2);
            default:                    /* unknown error                     */
               hf = 0;                  /* don't die without a message!      */
               CommErr(errno);
         }
      }

   w = strlen(Buf);
   if ((done = write(hf,Buf,w)) == -1) {  /* can't transmit the request?     */
      if ((errno == EPIPE) || (errno == EINVAL))
         ConnectionClosed();
        else
         CommErr(errno);
   } else if (done != w) {
      printf(GetString(S_TRANSERR),done,(unsigned long) w);
      return(3);
   }

   done = 0;                            /* await the answer                  */
   if ((done = ReadBlockedMsgPipe(&hf,Buf,BufSize - 1,1)) < 0) {
      if ((done == -EPIPE) || (done == -EINVAL))
         ConnectionClosed();
        else
         CommErr(-done);
      return(3);
   } else if (done < 6) {               /* crond sends "crond:" everytime    */
      printf(GetString(S_WRONGANSWER));
      return(3);
   }
   Buf[done] = 0;
   if (!IsRequestPreAnswer(Buf,&wanted)) {   /* simple answer?               */
      printf("%.*s\r\n",done,Buf);
      return(0);
   }
                                        /* wanted contens the number of bytes*/
                                        /* the daemon wants to send. ensure  */
                                        /* the buffer is large enough...     */

   if (wanted + 3 >= BufSize) {         /* some byte more than needed...     */
      BufSize = wanted + 4;
      if ((Buf = realloc(Buf,BufSize)) == NULL)
         NoMem();
   }
                                        /* read the extended answer          */
   if ((done = ReadBlockedMsgPipe(&hf,Buf,BufSize - 1,wanted)) < 0) {
      if ((done == -EPIPE) || (done == -EINVAL))
         ConnectionClosed();
        else
         CommErr(-done);
      return(3);
   } else if (done < wanted)
      printf(GetString(S_READERR),done,(unsigned long) wanted);
   close(hf);                           /* close the pipe before any try to  */
                                        /* display the data. Maybe, the user */
                                        /* uses "more" or other slow closing */
                                        /* programs. Allow crond to go to    */
                                        /* sleep.                            */
   hf = -1;
   printf("%.*s\r\n",done,Buf);
   return(0);
}

/* RCS depending informations
 *
 * $Name: Version121 $
 *
 * $Log: crontab.c $
 * Revision 1.4  1995/10/18 09:46:08  Florian
 * Some cosmetic changes.
 * Added support of the French language. Thanks to Raphaâl Vanney.
 *
 * Revision 1.3  1995/08/04 09:02:19  Florian
 * NLS supported (English and German)
 *
 * Revision 1.2  1995/03/15 09:07:34  Florian
 * Some minor bugs fixed.
 * TCP/IP support added.
 *
 * Revision 1.1  1995/02/03 10:42:35  Florian
 * Initial revision
 *
 *
 */
static char rcsid[] = "@(#)$Id: crontab.c 1.4 1995/10/18 09:46:08 Florian Rel $";
