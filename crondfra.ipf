.* Copyright (c) 1995 Florian Groáe-Coosmann, RCS section at the eof
.* Use &rbl. for a required blank ( ), &colon. for a colon (:)
:userdoc.
:docprof.
:ctrldef.
:ctrl ctrlid=CtrlID1 controls='ESC SEARCH PRINT INDEX CONTENTS BACK FORWARD'
coverpage.
:ectrldef.
.******************************************************************************
:h1 res=20100 name=ID_CRON.Le D‚mon Cron (r‚sum‚)
:i1 global.D‚mon Cron
:p.Le d‚mon cron est n‚ dans le monde unix. Ce programme en est un
portage vers OS/2. Certaines caract‚ristiques sp‚cifiques …
l'environnement unix ont ‚t‚ omises et d'autres fonctions sp‚cifiques …
OS/2 ont ‚t‚ ajout‚es, mais il n'y a pas de changement majeur dans les
fonctionnalit‚s ou les m‚thodes d'appel.

:p.Cron est un programme PM, et pas un d‚mon au sens strict d'unix, bien
qu'il soit possible de le lancer comme un d‚mon par la commande
:hp1.detach crond:ehp1..
Une fonctionnalit‚ manque sous OS/2, celle permettant de lancer un
programme en avant-plan … partir d'un programme s'ex‚cutant en
arriŠre-plan.
Utilisation … vos risques et p‚rils.

:p.Le D‚mon Cron ne reconnait aucun paramŠtre sur la ligne de commande.
La :link refid=IDD_NOTEBOOK reftype=hd.Configuration:elink. s'effectue
sur le bureau. Les paramŠtres erron‚s (aprŠs un changement de r‚pertoire
par exemple) seront r‚initialis‚s … leurs valeurs par d‚faut, ce qui
permet au programme de d‚marrer dans tous les cas.

:p.Comme sous unix, il est possible de modifier les entr‚es cron en
utilisant le programme :hp2.crontab:ehp2.. OS/2 ‚tant un systŠme
d'exploitation mono-utilisateur, les options :hp2.-r:ehp2. et
:hp2.-l:ehp2. ont ‚t‚ omises. Pour obtenir le d‚tail des conventions
d'appel, lancez :hp2.crontab:ehp2. sans paramŠtre.

Tous les paramŠtres importants se trouvent dans la description de la
:link reftype=hd refid=XMP_SYNTAX.syntaxe:elink. ou dans les
:link reftype=hd refid=XMP_XMP.exemples:elink..

:h4.:hp2.Quel est l'int‚rˆt du D‚mon Cron&rbl.?:ehp2.
:p.Le D‚mon Cron permet de lancer automatiquement des programmes … des
heures d‚termin‚es. Il est possible de d‚finir l'heure … la minute prŠs,
le(s) jour(s) du mois, le(s) mois et le(s) jour(s) de la semaine. Le
programme sera lanc‚ au(x) moment(s) d‚fini(s). Toutes les combinaisons
de ces ‚l‚ments peuvent ˆtre d‚finies, ce qui permet de g‚rer n'importe
quelle tƒche p‚riodique. Vous n'avez plus besoin d'aucune
:link refid=alarmclock reftype=fn.horloge:elink..
:fn id=alarmclock.Bien s–r, vous pouvez utiliser le d‚mon cron comme une
horloge&rbl.!:efn.

:p.Cron redirige toutes les sorties d'un programme lanc‚ vers un fichier
de sortie. Ce fichier peut ˆtre utilis‚ pour analyser ult‚rieurement les
‚ventuelles erreurs. Les programmes unix envoient la sortie de chaque
programme … leur propri‚taire, ce qui ne peut ˆtre fait sous OS/2.
:p.Un m‚canisme interne limite automatiquement la taille des fichiers de
sortie. Un nouveau fichier de sortie est utilis‚ … chaque fois que la
taille maximale est atteinte ou que l'utilisateur s‚lecte
:link refid=IDME_NEXTFILE reftype=hd.Fichier de sortie suivant:elink..

:p.Le D‚mon Cron remplace aussi le programme unix :hp2.at:ehp2.. Cette
commande lance un programme … une heure donn‚e, une seule fois. Le
mot-cl‚ :hp2.Once:ehp2. a ‚t‚ ajout‚ pour ‚viter la cr‚ation d'un
nouveau programme.

:p.De plus, le d‚mon peut lancer des programmes quand il s'initialise ou
quand il s'arrˆte.

:p.Le D‚mon Cron peut ˆtre utilis‚ au travers d'un r‚seau, grƒce au
programme :hp2.crontab:ehp2.. Les r‚seaux reconnus sont
:link refid=LAN_NETWORK reftype=hd.LAN/Netware:elink. et
:link refid=IDD_TCP reftype=hd.TCP/IP:elink..
.******************************************************************************
:h1.Couleurs et police
:i1.Couleurs
:i1.Police
:p.
Les couleurs et la police de la fenˆtre de PMCron peuvent ˆtre modifi‚es
en faisant glisser la couleur ou la police souhait‚es depuis les
palettes du dossier :hp2.configuration du systŠme:ehp2..
:p.Pour modifier la couleur de fond, faites glisser la nouvelle couleur
en pressant le bouton 2 de la souris, et lachez la sur la fenˆtre. Pour
modifier la couleur du texte, effectuez la mˆme op‚ration en maintenant
la touche :hp2.Ctrl:ehp2. enfonc‚e.
:p.Ces paramŠtres seront conserv‚s.
.******************************************************************************
:h1 res=20101 name=IDME_NEXTFILE.Fichier de sortie suivant
:i1.Fichier de sortie suivant
:p.La ligne de menu :hp2.Fichier de sortie suivant:ehp2. permet de
fermer le fichier de sortie actuel pour passer au suivant. Si le fichier
suivant existe d‚j…, il sera effac‚.

:p.Quand le
:link refid=IDDE_OUTPUT_COUNT reftype=hd.nombre de fichiers de sortie:elink.
maximum est atteint, le num‚ro repasse … 0.

:p.Le nom du fichier de sortie actuel est affich‚ dans la fenˆtre
principale du programme.

:p.Cette ligne de menu est indisponible si la case
:link refid=IDDE_OUTPUT_IGNORE_OUTPUT reftype=hd.Ignorer sortie:elink.
est coch‚e.

:p.Cette option peut ˆtre utile pour acc‚der au fichier de sortie actuel
car le fichier est conserv‚ ouvert par le programme. La lecture du
fichier est autoris‚e, mais la plupart des ‚diteurs exigent des droits
d'accŠs complets.
.******************************************************************************
:h1 hide res=20105 name=IDME_HELPKEYS.Touches
:p.Il n'y a pas de touches de raccourci sp‚cifiques … ce programme. L'
:link reftype=hd database='wphelp.hlp' refid=193.affectation des touches:elink.
est document‚e dans l':hp2.Index Principal:ehp2. du
:hp2.dossier Information:ehp2. sur le bureau.
.******************************************************************************
:h1 res=20200 name=IDD_NOTEBOOK.Configuration
:i1.Configuration
:p.Le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de
configuration:elink. peut ˆtre ouvert depuis le menu en s‚lectant la
ligne :hp2.Configuration:ehp2..
.*=============================================================================
:h2 res=20201 name=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration
:i1.Bloc-notes de configuration
:p.Le bloc-notes de configuration comporte cinq pages, accessibles de la
fa‡on habituelle. Chaque page dispose bien s–r d'une aide contextuelle.
Les cinq pages d‚crivent
:ul.
:li.le traitement des :link
 reftype=hd refid=IDD_CRON_FILE.Crontabs:elink.
:li.les :link
 reftype=hd refid=IDD_CRONTABS.entr‚es Crontabs:elink.
:li.l'utilisation des fichiers de :link
 reftype=hd refid=IDD_OUTPUT.Sortie:elink.
:li.les restrictions d'accŠs par une connexion
:link reftype=hd refid=IDD_TCP.TCP/IP:elink.
:li.les :link reftype=hd refid=IDD_GENERAL.options diverses:elink.
comme la langue.
:eul.
.*=============================================================================
:h2 res=20300 name=IDD_CRON_FILE.Traitement des Crontabs
:i1.Traitement des Crontabs
:p.Cette page du :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
permet d'alt‚rer le traitement des entr‚e Crontabs et de changer le
nom du fichier Crontabs.

:p.Si la case :link reftype=hd refid=IDDE_CRON_ATSTARTUP.Lancer les
Cronstart:elink. est coch‚e, le d‚mon lancera tous les programmes dont
l'heure de lancement est :hp2.CronStart:ehp2.. Ces ‚v‚nements sont
ignor‚s si cette case n'est pas coch‚e.

:p.Si la case :link reftype=hd refid=IDDE_CRON_ATEXIT.Lancer les
Cronstop:elink. est coch‚e, le d‚mon lancera tous les programmes dont
l'heure de lancement est :hp2.CronStop:ehp2.. Ces ‚v‚nements sont
ignor‚s si cette case n'est pas coch‚e.

:p.Vous pouvez aussi modifier le nom du fichier Crontabs en s‚lectant la
plaquette :link reftype=hd
refid=IDDE_CRON_FILENAMECHANGE.Recherche:elink..

.*-----------------------------------------------------------------------------
:h3 res=20301 name=IDDE_CRON_ATSTARTUP.Lancer les Cronstart
:i1.Lancer les Cronstart
:p.Contrairement aux versions unix, le d‚mon peut lancer des programmes
quand il d‚marre. Cette fonctionnalit‚ n'est pas trŠs utile sur les gros
systŠmes unix, mais sous OS/2 elle peut compl‚ter l'utilisation du
dossier de lancement et du fichier Startup.Cmd.

:p.D'autres renseignements sur l'utilisation des entr‚es Crontabs
se trouvent dans les sections
:link reftype=hd refid=XMP_SYNTAX.syntaxe:elink. et
:link reftype=hd refid=XMP_XMP.exemples:elink..

:p.Cette entr‚e se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant le traitement des
:link reftype=hd refid=IDD_CRON_FILE.Crontabs:elink..
.*-----------------------------------------------------------------------------
:h3 res=20302 name=IDDE_CRON_ATEXIT.Lancer les Cronstop
:i1.Lancer les Cronstop
:p.Contrairement aux versions unix, le d‚mon peut lancer des programmes
quand il est arrˆt‚. Le d‚mon ‚tant arrˆt‚ peu aprŠs le lancement de ces
commandes, il est pr‚f‚rable d'utiliser la commande :hp2.start:ehp2..
Cette fonction peut ˆtre utilis‚e pour des traitements tels que
facturation utilisateur, arrˆt du r‚seau...

:p.D'autres renseignements sur l'utilisation des entr‚es Crontabs
se trouvent dans les sections
:link reftype=hd refid=XMP_SYNTAX.syntaxe:elink. et
:link reftype=hd refid=XMP_XMP.exemples:elink..

:p.Cette entr‚e se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant le traitement des
:link reftype=hd refid=IDD_CRON_FILE.Crontabs:elink..
.*-----------------------------------------------------------------------------
:h3 res=20303 name=IDDE_CRON_FILENAME.Fichier
:i1.Nom du fichier (Crontabs)
:p.Le nom du fichier o— sont stock‚es les donn‚es Crontabs peut ˆtre
modifi‚ en s‚lectant la plaquette
:link reftype=hd refid=IDDE_CRON_FILENAMECHANGE.Recherche:elink.. Le nom
actuel du fichier est affich‚. La valeur par d‚faut de ce champ est
d‚termin‚e la premiŠre fois que le programme est lanc‚. Si une erreur se
produit en tentant d'acc‚der au fichier, le nom du fichier est remplac‚
par :hp2.nul:ehp2..

:p.Cette entr‚e se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant le traitement des
:link reftype=hd refid=IDD_CRON_FILE.Crontabs:elink..
.*-----------------------------------------------------------------------------
:h3 res=20304 name=IDDE_CRON_FILENAMECHANGE.Recherche
:i1.Recherche (Crontabs)
:p.Cette plaquette vous permet de modifier le chemin et le nom du
fichier Crontabs. La boŒte de dialogue standard est affich‚e pour vous
permettre de choisir un nouveau nom de fichier.

:p.Le fichier sera cr‚‚ s'il n'existe pas d‚j…. Un nom de fichier
correct doit ˆtre donn‚. La seule exception reconnue est le nom de
fichierÿ: :hp2.nul:ehp2., qui est utilis‚ comme une solution de secours.
Chaque ligne du fichier doit ˆtre conforme aux rŠgles de
:link reftype=hd refid=XMP_SYNTAX.syntaxe:elink.. Si le contenu du
fichier est accept‚, le nouveau nom de fichier est utilis‚, sinon
l'ancien nom de fichier est conserv‚.

:p.Cette entr‚e se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant le traitement des
:link reftype=hd refid=IDD_CRON_FILE.Crontabs:elink..
.*=============================================================================
:h2 res=20310 name=IDD_CRONTABS.Entr‚es Crontabs
:i1.Entr‚es Crontabs
:i1.Entr‚es du fichier Crontabs
:p.Cette page du :link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes
de configuration:elink. permet de consulter et de modifier les entr‚es
du fichier Crontabs.


:p.En s‚lectant la plaquette :link reftype=hd
refid=IDDE_CRONTABS_ADD.Ajouter:elink., le contenu du champ de saisie
:link reftype=hd refid=IDDE_CRONTABS_ENTRY.Nouvel ‚v‚nement:elink. sera
ajout‚ au fichier Crontabs. Le nouvel ‚v‚nement doit ˆtre conforme aux
rŠgles de :link reftype=hd refid=XMP_SYNTAX.syntaxe:elink..

:p.La plaquette :link reftype=hd
refid=IDDE_CRONTABS_CHANGE.Remplacer:elink. supprime l'‚v‚nement
s‚lectionn‚ et le remplace par le contenu du champ de saisie.

:p.La plaquette :link reftype=hd
refid=IDDE_CRONTABS_ABORT.Annuler:elink. permet d'annuler les
modifications apport‚es … un ‚v‚nement.

:p.Le fait de s‚lecter la plaquette :link reftype=hd
refid=IDDE_CRONTABS_DELETE.Effacer:elink. entraine la suppression de
toutes les lignes s‚lectionn‚es dans la liste des :link reftype=hd
refid=IDDE_CRONTABS_LIST.‚v‚nements d‚finis:elink..

:p.La plaquette :link reftype=hd
refid=IDDE_CRONTABS_EDIT.Modifier:elink. permet de modifier l'‚v‚nement
s‚lectionn‚ dans la liste des :link reftype=hd
refid=IDDE_CRONTABS_LIST.v‚nements d‚finis:elink.. L'‚v‚nement est
copi‚ dans le champ de saisieÿ; pour valider la modification, s‚lectez
la plaquette :link reftype=hd
refid=IDDE_CRONTABS_CHANGE.Remplacer:elink..

:p.Une plaquette n'est active que si l'op‚ration correspondante est
possible.
.*-----------------------------------------------------------------------------
:h3 res=20311 name=IDDE_CRONTABS_LIST.v‚nements d‚finis
:i1.Liste des ‚v‚nements d‚finis
:p.Cette liste contient tous les ‚v‚nements d‚finis dans le fichier
Crontabs. Chaque ligne peut ˆtre (d‚)s‚lectionn‚e … l'aide du bouton
gauche de la souris ou de la barre d'espacement. La plaquette
:link reftype=hd refid=IDDE_CRONTABS_DELETE.Effacer:elink.
supprime toutes les lignes s‚lectionn‚es.
:hp2.Attention, il n'y a pas de fonction ®d‚faire¯.:ehp2.

:p.Une ligne s‚lectionn‚e peut ˆtre modifi‚e en s‚lectant :link
reftype=hd refid=IDDE_CRONTABS_EDIT.Modifier:elink..

:p.Cette liste se trouve dans le :link reftype=hd
refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink. sur la page
concernant les :link reftype=hd refid=IDD_CRONTABS.entr‚es
Crontabs:elink..
.*-----------------------------------------------------------------------------
:h3 res=20312 name=IDDE_CRONTABS_ENTRY.Nouvel ‚v‚nement
:i1.Nouvel ‚v‚nement
:p.Ce champ de saisie permet d'entrer un nouvel ‚v‚nement. Le contenu
doit ˆtre conforme aux rŠgles de :link reftype=hd
refid=XMP_SYNTAX.syntaxe:elink.. AprŠs avoir saisi l'‚v‚nement, s‚lectez
la plaquette :link reftype=hd refid=IDDE_CRONTABS_ADD.Ajouter:elink.
pour l'ins‚rer dans le fichier Crontabs. Si le contenu du champ est
correct, il sera transf‚r‚ … la fin de la :link reftype=hd
refid=IDDE_CRONTABS_LIST.liste d'‚v‚nements:elink.. En cas d'erreur, le
contenu du champ est conserv‚ et un message affich‚.

:p.Si le contenu du champ de saisie provient de l'utilisation de la
plaquette :link reftype=hd refid=IDDE_CRONTABS_EDIT.Modifier:elink., la
modification est valid‚e par la plaquette :link reftype=hd
refid=IDDE_CRONTABS_CHANGE.Remplacer:elink.. La modification peut ˆtre
annul‚e en s‚lectant :link reftype=hd
refid=IDDE_CRONTABS_ABORT.Annuler:elink..

:p.Ce champ se trouve dans le :link reftype=hd
refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink. sur la page
concernant les :link reftype=hd refid=IDD_CRONTABS.entr‚es
Crontabs:elink..
.*-----------------------------------------------------------------------------
:h3 res=20313 name=IDDE_CRONTABS_ADD.Ajouter
:i1.Ajout d'un ‚v‚nement
:p.Cette plaquette transfŠre le contenu du champ :link reftype=hd
refid=IDDE_CRONTABS_ENTRY.Nouvel ‚v‚nement:elink. dans la liste des
‚v‚nements Crontabs.

:p.Le contenu de ce champ doit ˆtre conforme aux rŠgles de :link
reftype=hd refid=XMP_SYNTAX.syntaxe:elink.. En cas d'erreur, le contenu
du champ est conserv‚ et un message affich‚.

:p.Cette plaquette se trouve dans le :link reftype=hd
refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink. sur la page
concernant les :link reftype=hd refid=IDD_CRONTABS.entr‚es
Crontabs:elink..
.*-----------------------------------------------------------------------------
:h3 res=20316 name=IDDE_CRONTABS_CHANGE.Remplacer
:i1.Remplacer un ‚v‚nement
:p.La s‚lection de cette plaquette entraŒne le transfert du contenu du
champ de saisie :link reftype=hd refid=IDDE_CRONTABS_ENTRY.Nouvel
‚v‚nement:elink. dans la liste des ‚v‚nements, … la place de l'ancienne
ligne.

:p.Le contenu de ce champ doit ˆtre conforme aux rŠgles de :link
reftype=hd refid=XMP_SYNTAX.syntaxe:elink.. En cas d'erreur, le contenu
du champ est conserv‚ et un message affich‚.

:p.Cette plaquette se trouve dans le :link reftype=hd
refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink. sur la page
concernant les :link reftype=hd refid=IDD_CRONTABS.entr‚es
Crontabs:elink..
.*-----------------------------------------------------------------------------
:h3 res=20317 name=IDDE_CRONTABS_ABORT.Annuler
:i1.Annuler la modification d'un ‚v‚nement
:p.Cette plaquette annule les modifications saisies. Le champ de saisie
est vid‚ de son contenu.

:p.Cette plaquette se trouve dans le :link reftype=hd
refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink. sur la page
concernant les :link reftype=hd refid=IDD_CRONTABS.entr‚es
Crontabs:elink..
.*-----------------------------------------------------------------------------
:h3 res=20314 name=IDDE_CRONTABS_DELETE.Effacer
:i1.Effacer un ‚v‚nement
:p.Cette plaquette permet d'effacer toutes les lignes s‚lectionn‚es dans
la liste des :link reftype=hd refid=IDDE_CRONTABS_ENTRY.‚v‚nements
d‚finis:elink..

:p.Toutes les lignes s‚lectionn‚es seront irr‚m‚diablement effac‚es.

:p.Cette plaquette se trouve dans le :link reftype=hd
refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink. sur la page
concernant les :link reftype=hd refid=IDD_CRONTABS.entr‚es
Crontabs:elink..
.*-----------------------------------------------------------------------------
:h3 res=20315 name=IDDE_CRONTABS_EDIT.Modifier
:i1.Modification d'un ‚v‚nement
:p.Quand cette plaquette est s‚lect‚e, l'‚v‚nement s‚lectionn‚ dans la
liste des :link reftype=hd refid=IDDE_CRONTABS_LIST.‚v‚nements
d‚finis:elink. est copi‚ dans le champ de saisie d'un :link reftype=hd
refid=IDDE_CRONTABS_ENTRY.nouvel ‚v‚nement:elink., pour ˆtre modifi‚.
Par mesure de s‚curit‚, l'‚v‚nement est conserv‚ dans la liste pendant
que vous le modifiez. Pour valider la modification, s‚lectez la
plaquette :link reftype=hd refid=IDDE_CRONTABS_CHANGE.Remplacer:elink..
Pour l'annuler, s‚lectez la plaquette :link reftype=hd
refid=IDDE_CRONTABS_ABORT.Annuler:elink..

:p.Cette plaquette se trouve dans le :link reftype=hd
refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink. sur la page
concernant les :link reftype=hd refid=IDD_CRONTABS.entr‚es
Crontabs:elink..
.*=============================================================================
:h2 res=20400 name=IDD_OUTPUT.Sorties
:i1.Sorties
:p.Cette page du :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
permet de modifier le chemin des fichiers de sortie et les paramŠtres
relatifs aux sorties du d‚mon et de ses processus fils.

:p.L'espace maximum occup‚ sur disque par les fichiers de sortie est
d‚fini par les valeurs donn‚es au :link reftype=hd
refid=IDDE_OUTPUT_COUNT.Nombre de fichiers de sortie:elink. et … la
:link reftype=hd refid=IDDE_OUTPUT_THRES.Taille limite des
fichiers:elink..

:p.La plaquette :link reftype=hd refid=IDDE_OUTPUT_PATHCHANGE.Autre
r‚pertoire:elink. permet de d‚finir le r‚pertoire o— seront stock‚s les
fichiers de sortie.

:p.Il est possible d':link reftype=hd
refid=IDDE_OUTPUT_IGNORE_OUTPUT.Ignorer les sorties:elink. en cochant la
case correspondante.

:p.La version unix de Cron envoie la sortie de chaque programme lanc‚ …
l'utilisateur. OS/2 ne poss‚dant pas de systŠme standard d'envoi de
courrier, les messages et les sorties des programmes sont dirig‚s sur un
fichier. Si l'utilisateur n'y prends pas garde, ces fichiers pourraient
finir par occuper un espace important sur disque. Pour ‚viter ce
problŠme, un nouveau fichier est utilis‚ … chaque fois qu'un fichier de
sortie atteint la taille limite d‚finie. Les fichiers de sortie sont
nomm‚s :hp2.Cron000?.out:ehp2., le point d'interrogation ‚tant remplac‚
par un chiffre variant entre 0 et le nombre de fichiers de sortie
d‚fini.

:p.Si les sorties ne sont pas ignor‚es, le fichier de sortie actuel est
gard‚ ouvert en permanence. Certains programmes ne peuvent donc pas y
acc‚der -toutefois, la commande TYPE fonctionne.
.br
L'entr‚e :link reftype=hd refid=IDME_NEXTFILE.Fichier de sortie
suivant:elink. du menu permet de fermer le fichier de sortie actuel et
d'ouvrir le suivant. Le fichier de sortie ‚tant ferm‚, d'autres
programmes peuvent y acc‚der.
.*-----------------------------------------------------------------------------
:h3 res=10401 name=IDDE_OUTPUT_THRES.Taille limite
:i1.Taille limite des fichiers de sortie
:p.Le programme change de fichier de sortie si la taille du fichier
actuel d‚passe un maximum d‚fini.

:p.L'interaction avec les autres options est d‚crite dans l'aide de la
page :link reftype=hd refid=IDD_OUTPUT.Sorties:elink..

:p.Cet encart se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant les
:link reftype=hd refid=IDD_OUTPUT.Sorties:elink..
.*-----------------------------------------------------------------------------
:h3 res=10402 name=IDDE_OUTPUT_COUNT.Nombre de fichiers de sortie
:i1.Nombre de fichiers de sortie
:p.Les fichiers de sortie sont nomm‚s :hp2.Cron000?.out:ehp2., le point
d'interrogation ‚tant remplac‚ par un chiffre variant entre 0 et le
nombre de fichiers de sortie.

:p.L'interaction avec les autres options est d‚crite dans l'aide de la
page :link reftype=hd refid=IDD_OUTPUT.Sorties:elink..

:p.Cet encart se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant les
:link reftype=hd refid=IDD_OUTPUT.Sorties:elink..
.*-----------------------------------------------------------------------------
:h3 res=10403 name=IDDE_OUTPUT_PATH.R‚pertoire fichiers de sortie
:i1.R‚pertoire fichiers de sortie
:p.Ce champ affiche le r‚pertoire o— sont actuellement stock‚s les
fichiers de sortie. Il s'agit, par d‚faut, du r‚pertoire d'o— le
programme est lanc‚.

:p.S‚lectez la plaquette :link reftype=hd
refid=IDDE_OUTPUT_PATHCHANGE.Autre r‚pertoire:elink. pour d‚finir un
autre r‚pertoire. Une :link reftype=hd refid=IDD_OUTPUT_PATH.boŒte de
s‚lection:elink. sera ouverte..

:p.Ce champ se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant les
:link reftype=hd refid=IDD_OUTPUT.Sorties:elink..
.*-----------------------------------------------------------------------------
:h3 res=10404 name=IDDE_OUTPUT_PATHCHANGE.Autre r‚pertoire
:i1.Autre r‚pertoire
:p.Cette plaquette vous permet de modifier le r‚pertoire o— sont stock‚s
les fichiers de sortie. Si vous la s‚lectez, une :link reftype=hd
refid=IDD_OUTPUT_PATH.boŒte de s‚lection:elink. sera ouverte.

:p.Cette plaquette se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant les
:link reftype=hd refid=IDD_OUTPUT.Sorties:elink..
.*-----------------------------------------------------------------------------
:h3 res=10405 name=IDDE_OUTPUT_IGNORE_OUTPUT.Ignorer les sorties
:i1.Ignorer les sorties
:p.Il est possible d'ignorer totalement toutes les sorties du d‚mon et
de ses processus fils.

:p.Par d‚faut, cette case est coch‚e, c'est … dire que les sorties
des programmes sont ignor‚es. Il est conseill‚ de modifier les valeurs
de cette page pour les adapter … vos besoins.

:p.Si cette case est coch‚e l'option de menu :link reftype=hd
refid=IDME_NEXTFILE.Fichier de sortie suivant:elink. du menu principal
ne peut ˆtre s‚lect‚e.

:p.Cette plaquette se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant les
:link reftype=hd refid=IDD_OUTPUT.Sorties:elink..
.*-----------------------------------------------------------------------------
:h3 toc=4 res=10410 name=IDD_OUTPUT_PATH.Choix du r‚pertoire de sortie
:i1.Choix du r‚pertoire de sortie
:p.Cette boŒte de dialogue est ouverte quand vous s‚lectez la plaquette
:link reftype=hd refid=IDDE_OUTPUT_PATHCHANGE.Autre r‚pertoire:elink..

:p.Le r‚pertoire actuel des fichiers de sortie est affich‚ dans le :link
reftype=hd refid=IDDE_OUTPUT_PATH_ENTRY.Champ de saisie du r‚pertoire de
sortie:elink.. A chaque fois que vous cliquez deux fois sur une ligne de
la :link reftype=hd refid=IDDE_OUTPUT_PATH_LIST.Liste de
r‚pertoires:elink., son contenu est copi‚ dans le champ de saisie.

:p.Cette boŒte de dialogue est appel‚e depuis la page
:link reftype=hd refid=IDD_OUTPUT.Sorties:elink. du
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink..
.*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
:h4 res=10411 name=IDDE_OUTPUT_PATH_ENTRY.Champ de saisie du r‚pertoire de sortie
:i1.Champ de saisie du r‚pertoire de sortie
:p.Ce champ de saisie permet … l'utilisateur de saisir le nom du
r‚pertoire o— stocker les fichiers de sortie. Par d‚faut, ce champ
contient le r‚pertoire de sortie actuel.

:p.AprŠs avoir s‚lect‚ la plaquette de validation, le programme v‚rifie
que le contenu de ce champ v‚rifie certaines conditions&rbl.&colon.: le
r‚pertoire doit exister et permettre la manipulation de fichiers nomm‚s
:hp2.Cron0000.out:ehp2. … :hp2.Cron0009.out:ehp2..

.br
Si une erreur se produit, un message ad-hoc est affich‚. Un chemin
diff‚rent doit ˆtre pr‚cis‚, ou le r‚pertoire doit ˆtre cr‚‚.

:p.Ce champ de saisie se trouve dans la boŒte de dialogue
:link reftype=hd refid=IDD_OUTPUT_PATH.Choix du r‚pertoire de sortie:elink.
de la page
:link reftype=hd refid=IDD_OUTPUT.Sorties:elink. du
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink..
.*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
:h4 res=10412 name=IDDE_OUTPUT_PATH_LIST.Liste de r‚pertoires
:i1.Liste de r‚pertoires
:p.La liste de r‚pertoires fournit un moyen ais‚ de changer le
r‚pertoire des fichiers de sortie.

:p.La liste contient tous les sous-r‚pertoires du chemin courant et la
liste des unit‚s de disque, … l'exclusion des unit‚s de disquettes.

:p.Cette liste se trouve dans la boŒte de dialogue
:link reftype=hd refid=IDD_OUTPUT_PATH.Choix du r‚pertoire de sortie:elink.
de la page
:link reftype=hd refid=IDD_OUTPUT.Sorties:elink. du
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink..
.*=============================================================================
:h2 toc=3 res=10500 name=IDD_TCP.TCP/IP
:i1.TCP/IP
:p.Cette page du :link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de
configuration:elink. permet de configurer l'accŠs au d‚mon … partir
d'une machine distante via TCP/IP.

Il n'y a pas de restriction aux accŠs via TCP/IP. Le systŠme ne peut pas
interdire des communications sp‚cifiques. Les versions unix des d‚mons
TCP/IP utilisent des fonctions de s‚curit‚ locales qui ne sont pas
disponibles sous OS/2, celui-ci ‚tant un systŠme mono-utilisateur. Le
D‚mon Cron ne permet la communication via TCP/IP qu'… certains
ordinateurs, limitant ainsi le problŠme de s‚curit‚. Les noms des
ordinateurs sont lus dans un fichier contenant un nom par ligne. Un nom
d'ordinateur peut ˆtre une adresse physique (par exemple,
128.176.248.76) ou bien le nom internet (par exemple, sauron ou
sauron@uni-muenster.de).

:p.Le nom du fichier contenant la liste des ordinateurs autoris‚s
est affich‚ dans un champ de saisie. Pour en changer, s‚lectez la
plaquette
:link reftype=hd refid=IDDE_TCP_FILENAMECHANGE.Autre fichier:elink..

:p.L'accŠs … partir d'un ordinateur distant n'est autoris‚ que si la
case :link reftype=hd refid=IDDE_TCP_ALLOWACCESS.Autoriser
l'accŠs:elink. est coch‚e.
.br
Cette case ne peut ˆtre coch‚e que si TCP/IP est install‚ sur
l'ordinateur.

:p.L'accŠs au fichier Crontabs … partir d'un ordinateur distant se
fait … l'aide du programme :hp2.crontab:ehp2..

:p.Le num‚ro de port du D‚mon Cron est 64020.
.*-----------------------------------------------------------------------------
:h3 res=10501 name=IDDE_TCP_FILENAME.Nom de fichier
:i1.Nom de fichier (Autorisations d'accŠs TCP/IP)
:p.Le nom du fichier contenant les noms des ordinateurs autoris‚s
… acc‚der au d‚mon via TCP/IP peut ˆtre modifi‚ en s‚lectant la
plaquette
:link reftype=hd refid=IDDE_TCP_FILENAMECHANGE.Autre fichier:elink..
Le nom du fichier actuel sera affich‚. La premiŠre fois que le programme
est lanc‚, le fichier :hp2.nul:ehp2. est utilis‚ par d‚faut.

:p.Ce champ se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant
:link reftype=hd refid=IDD_TCP.TCP/IP:elink..
.*-----------------------------------------------------------------------------
:h3 res=10502 name=IDDE_TCP_FILENAMECHANGE.Autre fichier
:i1.Choix d'un autre fichier (d'autorisations TCP/IP)
:p.La s‚lection de cette plaquette vous permet de changer le r‚pertoire
et le nom du fichier contenant les autorisations d'accŠs TCP/IP. La
boŒte de dialogue standard du systŠme est utilis‚e.

:p.Le fichier sera cr‚‚ s'il n'existe pas d‚j…. Le fichier
:hp2.nul:ehp2. peut ˆtre utilis‚ comme ®fichier de secours¯. Chaque
ligne du fichier doit contenir une adresse TCP/IP. Les lignes vides et
les lignes commen‡ant par un diŠse (:hp2.#:ehp2.) ou un point-virgule
(:hp2.;:ehp2.) sont ignor‚es.

:p.Cette plaquette se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant
:link reftype=hd refid=IDD_TCP.TCP/IP:elink..
.*-----------------------------------------------------------------------------
:h3 res=10503 name=IDDE_TCP_ALLOWACCESS.Autoriser l'accŠs
:i1.Autoriser l'accŠs via TCP/IP
:i1.Autorisation d'accŠs via TCP/IP
:p.Cette case … cocher contr“le la possibilit‚ d'accŠs … distance via
TCP/IP.
:p.Cette case ne peut ˆtre coch‚e que si TCP/IP est install‚ sur
l'ordinateur.

:p.Cette case … cocher se trouve dans le :link reftype=hd
refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink. sur la page
concernant :link reftype=hd refid=IDD_TCP.TCP/IP:elink..
.*=============================================================================
:h2 res=10600 name=IDD_GENERAL.ParamŠtres divers
:i1.ParamŠtres divers
:p.Cette page du :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
vous permet de modifier la configuration g‚n‚rale du programme.

La seule option actuellement disponible sur cette page est la langue.
Par d‚faut, la langue est l'anglais, sauf si la variable d'environnement
:hp2.LANG:ehp2. en d‚signe une autre.
Les valeurs possibles de la variable :hp2.LANG:ehp2. sont&rbl.&colon.
:dl compact tsize=15.
:dthd.:hp5.valeur de LANG:ehp5.
:ddhd.:hp5.Langue:ehp5.
:dt.USA
:dd.English
:dt.UK
:dd.English
:dt.GERM
:dd.Deutsch
:dt.FRAN
:dd.Fran‡ais
:edl.
:p.La langue peut ˆtre chang‚e grƒce … l' :link reftype=hd
refid=IDDE_GENERAL_LANGUAGE.Encart de choix de la langue:elink..
.*-----------------------------------------------------------------------------
:h3 res=10601 name=IDDE_GENERAL_LANGUAGE.Choix de la langue
:i1.Langue
:p.La langue utilis‚e par le programme peut ˆtre choisie grƒce
… cet encart.
:link refid=NewLanguages reftype=fn.Le choix de langue est limit‚.:elink.

Par d‚faut, la langue utilis‚e est l'anglais, sauf si la variable
d'environnement :hp2.LANG:ehp2. en d‚signe une autre. Veuillez consulter
la rubrique :link reftype=hd refid=IDD_GENERAL.ParamŠtres divers:elink.
pour plus d'informations.

Les langues actuellement disponibles sont&rbl.&colon.
:ul.
:li.l'allemand,
:li.l'anglais,
:li.le fran‡ais.
:eul.

:p.Le changement ne sera pris en compte qu'au prochain lancement du
programme.

:p.Cet encart se trouve dans le
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Bloc-notes de configuration:elink.
sur la page concernant les
:link reftype=hd refid=IDD_GENERAL.ParamŠtres divers:elink..
:fn id=NewLanguages.Ce qui peut ‚voluer si quelqu'un effectue la traduction
de ce truc&rbl.!:efn.
.******************************************************************************
:h1 name=XMP_GENERAL.Exemples et syntaxe des entr‚es Crontabs
:p.Ce chapitre d‚crit la :link reftype=hd
refid=XMP_SYNTAX.syntaxe:elink. des entr‚es du fichier Crontabs et leur
utilisation … travers des :link reftype=hd
refid=XMP_XMP.exemples:elink..

:p.Contrairement … la syntaxe du programme :hp2.crontabs:ehp2. et … ce
qui est visible dans la liste des :link reftype=hd
refid=IDD_CRONTABS.entr‚es Crontabs:elink. du Bloc-notes de
configuration, il est possible d'utiliser des commentaires.

:p.Les lignes de commentaires sont des lignes vides ou commen‡ant par
un caractŠre point-virgule (:hp2.;:ehp2.) ou diŠse (:hp2.#:ehp2.).
.*=============================================================================
:h2 name=XMP_SYNTAX.Syntaxe des entr‚es Crontabs
:i1.Syntaxe
:p.La syntaxe des entr‚es Crontabs est assez simple. Chaque entr‚e est
plac‚e sur une ligne. La casse n'a pas d'importance. Chaque ligne est de
la forme suivante
:xmp.[indicateurs] heures commande:exmp.
:p.Les :hp2.indicateurs:ehp2. sont optionnels. Les indicateurs suivants
sont actuellement reconnus&rbl.&colon.
:dl compact.
:dthd.:hp5.Indicateur:ehp5.
:ddhd.:hp5.Effet:ehp5.
:dt.:hp2.Once:ehp2.
:dd.La commande ne sera ex‚cut‚e qu'une fois … l'heure donn‚e. Elle sera
ensuite effac‚e.
:edl.

:p.Normalement, le champ :hp2.heures:ehp2. est constitu‚ de cinq
‚l‚ments&rbl.&colon.
:hp2.chacun:ehp2. d'entre eux doit passer le test d'horaire pour que la
commande soit ex‚cut‚e&rbl.&colon.
:ol.
:li.:hp2.minutes:ehp2.
:p.Cette :link refid=NumberSpecification reftype=fn.sp‚cification de
nombre:elink. d‚finit la minute (dans une heure donn‚e) … laquelle la
commande doit ˆtre ex‚cut‚e. Les valeurs possibles sont&rbl.&colon. de 0 …
59.

:li.:hp2.heures:ehp2.
:p.Cette :link refid=NumberSpecification reftype=fn.sp‚cification de
nombre:elink. d‚finit l'heure … laquelle la commande doit ˆtre
exc‚cut‚e. Les valeurs possibles sont&rbl.&colon. de 0 … 23.

:li.:hp2.jour du mois:ehp2.
:p.Cette :link refid=NumberSpecification reftype=fn.sp‚cification de
nombre:elink. d‚finit le jour du mois o— la commande doit ˆtre ex‚cut‚e.
Les valeurs possibles sont&rbl.&colon. de 1 … 31. Ces valeurs ne sont pas
fonction du mois. Une commande ne sera jamais ex‚cut‚e le 31 f‚vrier,
bien que cela soit une entr‚e valide.

:li.:hp2.mois:ehp2.
:p.Cette :link refid=NumberSpecification reftype=fn.sp‚cification de
nombre:elink. d‚finit le mois pendant lequel la commande sera ex‚cut‚e.
Les valeurs possibles sont&rbl.&colon. de 1 … 12.

:li.:hp2.jour de la semaine:ehp2.
:p.Cette :link refid=NumberSpecification reftype=fn.sp‚cification de
nombre:elink. d‚finit le jour de la semaine o— la commande sera
ex‚cut‚e. Les valeurs possibles sont&rbl.&colon. de 0 (dimanche) … 6.
:eol.

:p.Il existe trois horaires sp‚ciaux qui remplacent les cinq
‚l‚ments&rbl.&colon.
:dl compact tsize=15.
:dthd.:hp5.horaire:ehp5.
:ddhd.:hp5.effet:ehp5.
:dt.:hp2.Cronstart:ehp2.
:dd.La commande sera ex‚cut‚e au d‚marrage du d‚mon. La commande ne sera
ex‚cut‚e que si vous avez coch‚ la case
:link reftype=hd refid=IDD_CRON_FILE.Lancer les Cronstart:elink..
:dt.:hp2.Cronstop:ehp2.
:dd.La commande sera ex‚cut‚e … l'arrˆt du d‚mon. La commande ne sera
ex‚cut‚e que si vous avez coch‚ la case
:link reftype=hd refid=IDD_CRON_FILE.Lancer les Cronstop:elink..
.br
:hp1.ATTENTION&rbl.&colon. Le d‚lai avant l'arrˆt du d‚mon est trŠs court.
Dans le cas d'un arrˆt du systŠme ce d‚lai peut ˆtre trop court pour
ex‚cuter de longues commandes. Une version ult‚rieure impl‚mentera
peut-ˆtre un d‚lai d‚finissable par l'utilisateur. R‚clamez si cette
fonctionnalit‚ vous int‚resse.
:ehp1.
:dt.:hp2.Daily:ehp2.
:dd.La commande sera ex‚cut‚e une fois par jour, … la premiŠre heure.
Notez que la commande ne sera :hp2.pas:ehp2. ex‚cut‚e le jour de son
ajout … la liste des ‚v‚nements, mais … partir du jour suivant.
:edl.

:p.N'importe quelle commande OS/2 valide peut ˆtre sp‚cifi‚e comme
:hp2.commande:ehp2.. Il n'existe actuellement aucune possibilit‚ de
pr‚ciser le type du programme, cela va changer&rbl.! Dans le cas d'un
programme DOS, vous devez utiliser la commande OS/2 :hp2.start:ehp2.. La
sortie de la commande sera collect‚e dans un fichier temporaire puis
ajout‚e au fichier de :link reftype=hd refid=IDD_OUTPUT.sortie:elink. …
la fin du programme.
Une nouvelle session peut ˆtre ouverte grƒce … la commande :hp2.start:ehp2..
Cette session ne sera pas sous contr“le du d‚mon, la sortie devra donc
ˆtre redirig‚e en ajoutant
:xmp. >fichier 2>&amp.1:exmp.
:p.… la fin de la ligne de commande. Pour ignorer la sortie, utilisez le
fichier :hp2.nul:ehp2.. Les programmes du Gestionnaire de Pr‚sentation
n'utilisent normalement pas le fichier de sortie standard.

:p.Les fichiers temporaires sont cr‚‚s dans le r‚pertoire o— se trouve
le programme du D‚mon Cron. Il est conseill‚ de d‚truire p‚riodiquement
ces fichiers par les commandes Crontabs
:xmp.
Cronstart del ?&colon.\chemin\*.tmp >nul 2>&amp.1
0 3 * * 0 del ?&colon.\chemin\*.tmp >nul 2>&amp.1
:exmp.
:p.Le point d'interrogation doit ˆtre remplac‚ par la lettre
repr‚sentant l'unit‚ de disque.

:fn id=NumberSpecification.Une sp‚cification de nombre est une liste de
nombres s‚par‚s par des virgules. Une ‚toile (:hp2.*:ehp2.) signifie
®toutes les valeurs possibles¯. Exemples&rbl.&colon.
:xmp.1,2,5:exmp.
:p.ou
:xmp.8-12,14-18:exmp.
:p.ou
:xmp.*:exmp.:efn.
.*=============================================================================
:h2 name=XMP_XMP.Exemples d'entr‚es Crontabs
:i1.Exemple
:p.Il existe une infinit‚ d'utilisations du D‚mon Cron. Certaines sont
illustr‚e dans les exemples. Certaines commandes utilis‚es ne sont pas
disponibles sur tous les systŠmes, mais leur nom illustre leur objet.

:ul.
:li.:font facename=Courier size=18x10.* * * * * echo Une minute s'est ‚coul‚e.:font
facename=default size=0x0.
:p.Chaque minute de chaque heure de chaque jour la ligne :hp2.Une minute
s'est ‚coul‚e.:ehp2. sera affich‚e. La sortie de la commande sera copi‚e
dans le fichier de :link reftype=hd refid=IDD_OUTPUT.sortie:elink. du
D‚mon Cron.

:li.:font facename=Courier size=18x10.0 * * * * echo Une heure s'est ‚coul‚e. >nul 2>&amp.1:font
facename=default size=0x0.
:p.Au d‚but de chaque heure de chaque jour la ligne :hp2.Une heure s'est
‚coul‚e.:ehp2. sera affich‚e. Comme la sortie est redirig‚e sur
:hp2.nul:ehp2., cette commande est parfaitement inutile. Le fichier de
sortie du d‚mon contiendra juste une ligne indiquant que la commande a
‚t‚ lanc‚e.

:li.:font facename=Courier size=18x10.0,30 * * * * EnvoyerLeCourrier:font
facename=default size=0x0.
:p.Toutes les demi-heures, le programme EnvoyerLeCourrier sera lanc‚. La
sortie sera envoy‚e dans le fichier de sortie du d‚mon.

:li.:font facename=Courier size=18x10.0,30 * * * * start EnvoiCourrierPM
>nul 2>&amp.1:font facename=default size=0x0.
:p.Toutes les demi-heures le programme EnvoiCourrierPM sera lanc‚. Les
sorties seront ignor‚es. Le programme est lanc‚ dans une session s‚par‚e
(n‚cessaire pour les programmes Gestionnaire de Pr‚sentation&rbl.!).

:li.:font facename=Courier size=18x10.0 3 * * 0 MaSauvegarde /hebdomadaire:font
facename=default size=0x0.
:p.Chaque dimanche … trois heures du matin le programme MaSauvegarde
sera lanc‚ avec le paramŠtre /hebdomadaire.

:li.:font facename=Courier size=18x10.0 3 1 * * MaSauvegarde
/mensuelle:font facename=default size=0x0.
:p.Chaque premier jour du mois … trois heures du matin le programme
MaSauvegarde sera lanc‚ avec le paramŠtre /mensuelle.

:li.:font facename=Courier size=18x10.Once 0 3 1 4 * time 4:font
facename=default size=0x0.
:p.Le premier avril … trois heures du matin, une heure sera ajout‚e …
l'heure du systŠme. Cette commande ne sera ex‚cut‚e qu'une fois.

:li.:font facename=Courier size=18x10.Once * * * * * setboot /b >nul 2>&amp.1:font
facename=default size=0x0.
:p.La machine est r‚initialis‚e au d‚but de la minute suivante. La
commande n'est ex‚cut‚e qu'une fois (important si le D‚mon Cron est
lanc‚ automatiquement au lancement du sustŠme&rbl.!) La sortie est ignor‚e
(l'effet est assez visible). N'essayez pas cette commande … titre
d'exercice, :hp2.setboot /b:ehp2. est l'‚quivalent de la pression sur
les touches Ctrl-Alt-Suppr. Cette commande peut ˆtre utile en cas
d'arrˆt pr‚vu de l'alimentation ‚lectrique (contr“le de l'alimentation
de secours) ou si l'alimentation vous parraŒt d‚faillante. Il est plus
rapide de taper une commande :hp2.crontab:ehp2. que de courir arrˆter un
serveur deux ‚tages plus haut.

:li.:font facename=Courier size=18x10.0 3 * * * deltree f&colon.\tmp
&amp. mkdir f&colon.\tmp &amp. echo Ce r‚pertoire sera effac‚ … 3h du
matin ! > f&colon.\tmp\Attention.!!!:font facename=default size=0x0.
:p.A trois heures du matin la commande :hp2.deltree:ehp2. sera ex‚cut‚e
avec le paramŠtre f&colon.\tmp, puis ce r‚pertoire est re-cr‚‚ et un
fichier d'avertissement est cr‚‚ dans ce r‚pertoire. Le tout est fait en
une seule commande, dont la sortie est chronologiquement ajout‚e au
fichier de sortie du d‚mon.

:li.:font facename=Courier size=18x10.Cronstart del c&colon.\cron\*.tmp >nul 2>&amp.1:font
facename=default size=0x0.
:p.A chaque d‚marrage du d‚mon les fichiers temporaires pr‚sents dans le
r‚pertoire du programme sont effac‚s, sans garder trace de la sortie.
:p.Cette commande ne sera ex‚cut‚e que si vous avez coch‚ la
case :hp2.Lancer les Cronstart:ehp2. sur la page
:link reftype=hd refid=IDD_CRON_FILE.Traitement des Crontabs:elink..
:p.Cette commande est utile car, l'exp‚rience le montre, certains
fichiers temporaires ne sont pas effac‚s. Ceci n'est pas un bug du
programme. La plupart du temps, vous aurez oubli‚ de rediriger la sortie
d'un programme lanc‚ par la commande :hp2.start:ehp2.. Dans cette
situation, les fichiers temporaires sont verrouill‚s pendant l'ex‚cution
du programme lanc‚, ce qui empˆche le d‚mon de les copier ou de les
effacer.

:li.:font facename=Courier size=18x10.Cronstop echo Arrˆt&rbl.&colon. >> \Times\Rapha‰l &amp. time >> \Times\Rapha‰l:font
facename=default size=0x0.
:p.A chaque arrˆt du d‚mon un message et l'heure actuelle seront ajout‚s
… un fichier. L'entr‚e standard est redirig‚e sur :hp2.nul:ehp2.. La
commande :hp2.time:ehp2. ne bloquera donc pas l'ex‚cution ni ne d‚finira
une nouvelle heure.

:p.Cette commande ne sera ex‚cut‚e que si vous avez coch‚ la case
:hp2.Lancer les Cronstop:ehp2. sur la page
:link reftype=hd refid=IDD_CRON_FILE.Traitement des Crontabs:elink..

:p.Conjointement avec une commande similaire au lancement du d‚mon, une
comptabilit‚ simple peut ˆtre impl‚ment‚e si vous placez une copie du
d‚mon dans le dossier de lancement.

:hp1.ATTENTION&rbl.&colon. Le d‚lai avant l'arrˆt du d‚mon est trŠs court.
Dans le cas d'un arrˆt du systŠme ce d‚lai peut ˆtre trop court pour
ex‚cuter de longues commandes. Une version ult‚rieure impl‚mentera
peut-ˆtre un d‚lai d‚finissable par l'utilisateur. R‚clamez si cette
fonctionnalit‚ vous int‚resse.:ehp1.

:li.:font facename=Courier size=18x10.Daily ControleVirus:font
facename=default size=0x0.
:p.Le programme ControleVirus sera ex‚cut‚ une fois par jour,
aussi t“t que possible. Notez que la commande ne sera pas ex‚cut‚e
le jour de son ajout … la liste, mais dŠs le jour suivant.
:li.:font facename=Courier size=18x10.0 8-12,14-18 * * 1-5 start PMGong >nul 2>&amp.1:font
facename=default size=0x0.
:p.Du lundi au vendredi, … chaque heure pleine entre 8h du matin et
midi, et entre 14h et 18h, le programme PMGong sera ex‚cut‚. Le
programme sera lanc‚ dans une session s‚par‚e, et les sorties seront
ignor‚es.
:eul.
.******************************************************************************
:h1 name=LAN_NETWORK.Utilisation dans un r‚seau LAN ou NetWare
:i1.R‚seau
:i1.Novell Netware
:i1.LAN
:p.La m‚thode de communication id‚ale avec le D‚mon Cron se fait au
moyen d'un ®named pipe¯, ce type de communication ‚tant directement g‚r‚
par OS/2.

:p.Il est possible d'utiliser ce type de communication au travers d'un
r‚seau. LAN et NetWare fournissent tous les deux les m‚canismes
n‚cessaires, y compris quant … la s‚curit‚.

:p.Pour que l'accŠs au d‚mon … travers un r‚seau soit possible, vous
devez l'avoir autoris‚ grƒce au programme ad-hoc de votre r‚seau (par
exemple, :hp2.net access:ehp2. pour LAN). Le nom de la ressource …
partager est le pipe
:xmp.
\pipe\Cron-Daemon-Request
:exmp.
:p.Ceci fait, l'accŠs au d‚mon est possible depuis une machine distante
en utilisant le programme :hp2.crontab:ehp2..
.******************************************************************************
:h1.Nouveaut‚s de cette version
:p.Cette section contient la liste des ‚volutions majeures entre les
versions.
:ul.
:li.:link reftype=hd refid=CHANG_120_112.Diff‚rences entre la version
1.12 et la version 1.20:elink.
:eul.
.*=============================================================================
:h2 name=CHANG_120_112.Diff‚rences entre la version 1.12 et la version 1.20
:p.Modifications apport‚es dans la version 1.20&rbl.&colon.
:ul compact.
:li.Modifications cosm‚tiques et corrections de bugs.
:li.Gestion des messages et de l'aide en fran‡ais.
:li.Possibilit‚ de modifier un ‚v‚nement depuis le d‚mon Cron.
:li.Possibilit‚ de sp‚cifier des intervales dans les champs heures des
‚v‚nements.
:hp2.0,4,5,6,9:ehp2. est ‚quivalent … :hp2.0,4-6,9:ehp2..
:li.L'horaire sp‚cial :hp2.Daily:ehp2. permet de lancer une commande
aussi t“t que possible une fois par jour.
:li.Les couleurs et la police de la fenˆtre peuvent ˆtre modifi‚s en
utilisant les palettes du dossier de configuration du systŠme.
:li.Sauvegarde le mode d'affichage de la fenˆtre (r‚duit, agrandi,
normal).
:eul.
.******************************************************************************
:h1.Auto-congratulation
:p.Ce programme a ‚t‚ d‚velopp‚ sous OS/2, pour le Gestionnaire de
Pr‚sentation, avec EMX. Conseils, messages d'erreurs et recommendations
seront accept‚es avec grattitude. (SVP, jetez un oeil sur le fichier
:hp2.program!:ehp2.)

:p.Si vous avez un souhait … formuler, veuillez v‚rifier qu'il
n'est pas d‚j… exprim‚ dans le fichier :hp2.wishlist:ehp2..

:p.Vous avez la permission expresse de r‚cup‚rer tout ou partie du
source du programme. Si vous utilisez des fichiers entriers pas ou peu
modifi‚s, la license g‚n‚rale s'applique. Vous devez laisser le texte du
copyright et signaler vos modifications pour ne pas induire d'autres
utilisateurs en erreur. Une donation de la part d'un organisme social
sera accept‚e, un courrier (‚lectronique) disant ®pas mail¯ est
suffisant.

:p.Remerciements particuliers … Leonard Hesker, qui a v‚rifi‚ la
version anglaise de l'aide en ligne.

:p.Merci … ceux qui ont effectu‚ la traduction du d‚mon cron vers
d'autres langues, … savoir&rbl.&colon.
:parml compact tsize=10 break=none.
:pt.Fran‡ais
:pd.Rapha‰l Vanney
:eparml.

:p.Florian Groáe-Coosmann
.br
Courrier ‚lectronique via Internet&rbl.&colon.
:font facename=Courier size=18x10.coosman@uni-muenster.de:font
facename=default size=0x0.
:euserdoc.
.* RCS depending informations
.*
.* $Id: crondfra.ipf 1.2 1995/10/18 11:13:33 Florian Rel $
.*
.* $Name: Version121 $
.*
.* $Log: crondfra.ipf $
.* Revision 1.2  1995/10/18 11:13:33  Florian
.* Control pushbuttons added.
.*
.* Revision 1.1  1995/10/18 10:28:37  Florian
.* Initial revision translated by Rapha‰l Vanney.
.*
.*
