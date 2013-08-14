.* Copyright (c) 1995 Florian Gro�e-Coosmann, RCS section at the eof
.* Use &rbl. for a required blank ( ), &colon. for a colon (:)
:userdoc.
:docprof.
:ctrldef.
:ctrl ctrlid=CtrlID1 controls='ESC SEARCH PRINT INDEX CONTENTS BACK FORWARD'
coverpage.
:ectrldef.
.******************************************************************************
:h1 res=10100 name=ID_CRON.Der Cron D�mon (�bersicht)
:i1 global.Cron D�mon
:p.Der Cron D�mon stammt eigentlich aus der Unix-Welt. Dieses Programm ist
eine Portierung auf OS/2, wobei Unix-spezifische Eigenschaften weggelassen
und OS/2-spezifische hinzugef�gt wurden, ohne da� eine prinzipielle �nderung
der Funktionsweise oder Aufrufm�glichkeiten zugelassen werden mu�te.

:p.Cron ist ein PM-Programm und im Unix-Sinn kein D�mon, obwohl crond auch
als D�mon �ber die Kommandozeile :hp1. detach crond :ehp1.
gestartet werden kann. Leider hat er dann nicht seine volle Funktionalit�t
unter OS/2, da aus dem Hintergrund keine Vordergrundprogramme gestartet werden
k�nnen. Die Benutzung ist dann auf eigene Gefahr.

:p.Der Cron D�mon hat keine Aufrufparameter. Alle
:link refid=IDD_NOTEBOOK reftype=hd.Einstellungen:elink.
erfolgen �ber die Oberfl�che. Fehlerhafte Eintr�ge (z.B. bei einem Pfadwechsel)
werden auf Voreinstellungen gesetzt, so da� ein Programmstart immer m�glich
sein sollte.

:p.Wie unter Unix besteht die M�glichkeit, Cron-Eintr�ge �ber das
Hilfsprogramm :hp2.crontab:ehp2. zu manipulieren. Da OS/2 ein
Single-User-Betriebssystem ist, entfallen die Optionen :hp2.-r:ehp2. und
:hp2.-l:ehp2. der Unix-Version. Stattdessen werden die einzelnen Befehle
direkt angegeben. F�r eine vollst�ndige Liste der Aufrufvariationen rufe man
am besten :hp2.crontab:ehp2. ohne Parameter auf. Alle wichtigen Parameter
findet man auch in der
:link reftype=hd refid=XMP_SYNTAX.Syntaxbeschreibung:elink. und bei den
:link reftype=hd refid=XMP_XMP.Beispielen:elink..

:h4.:hp2.Was leistet der Cron D�mon?:ehp2.
:p.Der Cron D�mon kann zu gewissen Zeiten Programme starten. Dabei ist eine
Auswahl der Minuten, Stunden, Monatstage, Monate und Wochentage m�glich.
Zu den angegebenen Zeiten wird das Programm dann gestartet. Alle Kombinationen
der Zeiten sind dabei m�glich, so da� man einen repektablen Umfang von
chronologischen Aufgaben durch das System erledigen lassen kann, ohne da� man
ein :link refid=Wecker reftype=fn.Weckerprogramm:elink. benutzen mu�.
:fn id=Wecker.Als Wecker eignet sich der Cron D�mon selbstverst�ndlich
auch!:efn.

:p.Cron leitet alle Ausgaben der aufgerufenen Programme in eine Ausgabedatei.
Damit lassen sich dann im Nachhinein Fehler in den Programmen analysieren.
Der Unix-D�mon schickt diese Ausgaben (pro Programmstart) dem entsprechenden
Benutzer mit der Post zu. Diese Vorgehensweise ist aber unter OS/2 recht
ungeschickt, da es weder mehrere Benutzer, noch ein Postsystem gibt. Die
jeweilige Ausgabedatei enth�lt deshalb alle n�tigen Informationen �ber Cron,
Programmstarts, sowie deren Ausgaben.

:p.Um auch auf unregelm��ig gewarteten Maschinen nicht als Speicherfresser zu
fungieren, wurde ein Mechanismus eingebaut, der es erm�glicht, den
Ausgabeumfang zu begrenzen. Dabei werden mehrere Ausgabedateien verwendet,
zwischen denen nach dem Erreichen einer gewissen Gr��e oder auf Benutzerwunsch
durch den entsprechenden
:link refid=IDME_NEXTFILE reftype=hd.Men�eintrag:elink.
gewechselt wird.

:p.Der Cron D�mon �bernimmt weiterhin alle Aufgaben des Unix-Befehls
:hp2.at:ehp2.. Dieser Befehl startet ebenfalls ein Programm zu einer gewissen
Zeit, jedoch wird die Programmausf�hrung nie wiederholt. Um ein weiteres
Kommunikationsprogramm zu sparen, wurde das Schl�sselwort :hp2.Once:ehp2.
eingef�hrt.

:p.Weiterhin beherrscht der D�mon spezifische Funktionen, die es
erlauben, beim Start und beim Ende des Cron D�mons Programme zu starten, da
OS/2-Maschinen naturgem�� selten l�nger als ein paar Stunden arbeiten.
Programme, die eigentlich in der Nacht arbeiten sollen, k�nnen so nachgefahren
werden. (Datensicherung und -auffrischung, Aufr�umarbeiten, etc.)

:p.Der Cron D�mon kann auch �ber das Netzwerk bedient werden. Dies geschieht
durch das Programm :hp2.crontab:ehp2.. Unterst�tzte Netzwerkvarianten sind
:link refid=LAN_NETWORK reftype=hd.LAN/Netware:elink. und
:link refid=IDD_TCP reftype=hd.TCP/IP:elink..
.******************************************************************************
:h1.Farben und Schriftart
:i1.Farben
:i1.Schriftart
:p.Man kann die Farben und die Schriftart des Anzeigefensters ver�ndern.
Dazu �ffnet man in dem Ordner :hp2.Systemkonfiguration:ehp2. die Farb- oder
Schriftartpalette und zieht das gew�nschte Element auf das Fenster.
:p.Bei den Farben kann man mit der zweiten Maustaste die Hintergrundfarbe
�ndern. H�lt man dabei die [Strg]-Taste fest, �ndert sich die Vordergrundfarbe.
:p.Die Werte werden gespeichert und bei einem erneuten Programmstart
�bernommen.
.******************************************************************************
:h1 res=10101 name=IDME_NEXTFILE.N�chste Ausgabedatei
:i1.N�chste Ausgabedatei
:p.�ber den Men�eintrag :hp2.N�chste Ausgabedatei:ehp2. kann man die aktuelle
Ausgabedatei schlie�en und zur n�chsten �berwechseln. Diese wird vorher
gel�scht, falls sie schon existiert.

:p.Wird die maximale
:link refid=IDDE_OUTPUT_COUNT reftype=hd.Dateianzahl:elink.
erreicht, wird mit der Datei der Nummer 0 fortgefahren.

:p.Die aktuelle Ausgabedatei wird im Hauptfenster angezeigt.

:p.Der Men�eintrag ist nicht anw�hlbar, falls die
:link refid=IDDE_OUTPUT_IGNORE_OUTPUT reftype=hd.Ausgabe ignoriert:elink. wird.

:p.Dieser Men�punkt kann n�tzlich sein, um die aktuelle Ausgabedatei anzusehen
oder zu bearbeiten, da der D�mon die aktuelle Ausgabedatei permanent im Zugriff
hat. Das Lesen dieser Datei ist zwar zu jedem Zeitpunkt erlaubt (TYPE
funktioniert), aber viele Editoren kommen damit nicht zurecht.
.******************************************************************************
:h1 hide res=10105 name=IDME_HELPKEYS.Hilfe zur Tastatur
:p.Es gibt keine spezifischen "Hotkeys" in diesem Programm. Die allgemeinen
:link reftype=hd database='wphelp.hlp' refid=193.Tastenzuordnungen:elink.
kann man sich z.B. �ber den :hp2.Hauptindex:ehp2. im
:hp2.Informationsordner:ehp2. auf der Oberfl�che anzeigen lassen.
.******************************************************************************
:h1 res=10200 name=IDD_NOTEBOOK.Einstellungen
:i1.Einstellungen
:p.Durch das Ausw�hlen des Men�eintrags :hp2.Einstellungen:ehp2. �ffnet man das
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink..
Dort werden alle benutzerdefinierbaren Setzungen f�r das Programm
eingegeben.
.*=============================================================================
:h2 res=10201 name=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen
:i1.Notizbuch
:p.Es gibt zur Zeit f�nf verschiedene Notizbuchseiten, die wie gewohnt
bedient werden k�nnen. Selbstverst�ndlich kann auch zu jedem Seiteneintrag
die Hilfe kontextsensitiv angefordert werden.
Die f�nf Seiten beschreiben
:ul.
:li.die allgemeine :link
 reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink.
:li.die einzelnen :link
 reftype=hd refid=IDD_CRONTABS.Crontabs-Eintr�ge:elink.
:li.das Daten f�r die :link
 reftype=hd refid=IDD_OUTPUT.Ausgabe:elink. in eine Datei
:li.die M�glichkeiten f�r den :link
 reftype=hd refid=IDD_TCP.TCP/IP-Zugriff:elink. von anderen Rechnern
:li.:link reftype=hd refid=IDD_GENERAL.allgemeine Einstellungen:elink., wie
z.B. die Sprache
:eul.
.*=============================================================================
:h2 res=10300 name=IDD_CRON_FILE.Crontabs-Bearbeitung
:i1.Crontabs-Bearbeitung
:p.Diese Seite aus dem :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink.
erlaubt es, die Abarbeitung der Crontabs-Eintr�ge und deren Datei anzugeben.

:p.Durch das Ausw�hlen der :link
reftype=hd refid=IDDE_CRON_ATSTARTUP.CronStart-Abarbeitung:elink. f�hrt der
D�mon alle Crontabs-Eintr�ge beim D�monstart aus, deren Startzeit mit
:hp2.CronStart:ehp2. angegeben wurde. Ist die Abarbeitung nicht ausgew�hlt,
werden die Eintr�ge ignoriert.

:p.Durch das Ausw�hlen der :link
reftype=hd refid=IDDE_CRON_ATEXIT.CronStop-Abarbeitung:elink. f�hrt der
D�mon alle Crontabs-Eintr�ge beim Ende des D�mon aus, deren Startzeit mit
:hp2.CronStop:ehp2. angegeben wurde. Ist die Abarbeitung nicht ausgew�hlt,
werden die Eintr�ge ignoriert.

:p.Der Dateiname und Pfad der Crontabs-Datei wird ebenfalls angezeigt; zur
:link reftype=hd refid=IDDE_CRON_FILENAMECHANGE.�nderung:elink.
mu� aber eine Schaltfl�che benutzt werden.
.*-----------------------------------------------------------------------------
:h3 res=10301 name=IDDE_CRON_ATSTARTUP.CronStart-Abarbeitung
:i1.CronStart-Abarbeitung
:p.Der D�mon beherrscht im Unterschied zur Unixversion auch die
Programmausf�hrung beim Start des D�mon. W�hrend in einem (gr��eren)
Unixsystem diese Option unsinnig ist, ist sie unter OS/2 eine willkommene
Bereicherung der M�glichkeiten, da regelm��ig zu startende Programme
nachgezogen werden k�nnen. (Selbstverst�ndlich kann man auch die
Standardm�glichkeiten von Startup.CMD oder den Systemstart-Ordner nutzen.)
Es ist auch eine Benutzerabrechnung im Verbund mit
:link reftype=hd refid=IDDE_CRON_ATEXIT.CronStop-Abarbeitung:elink. m�glich,
sofern das Programm (als Referenz) im Systemstart-Ordner plaziert wurde oder
anderweitig beim Hochfahren des OS/2 gestartet wird.

:p.Weitere Hinweise f�r die Gestaltung der Crontabs-Eintr�ge findet man in der
:link reftype=hd refid=XMP_SYNTAX.Syntaxbeschreibung:elink. und bei den
:link reftype=hd refid=XMP_XMP.Beispielen:elink..

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink..
.*-----------------------------------------------------------------------------
:h3 res=10302 name=IDDE_CRON_ATEXIT.CronStop-Abarbeitung
:i1.CronStop-Abarbeitung
:p.Der D�mon beherrscht im Unterschied zur Unixversion auch die
Programmausf�hrung beim Ende des D�mon. Diese M�glichkeit ist mit Bedacht
zu benutzen, da der D�mon sich kurz nach der Aufforderung zum Beenden wirklich
beendet. Es empfiehlt sich, die Programme mittels dem OS/2-Befehl
:hp2.start:ehp2. aufzurufen, da der D�mon nur eine kurze Verz�gerung akzeptiert
(ca. 1 Sekunde), um dann zu sterben.
In dieser Zeit lassen sich jedoch noch sinnvolle Arbeiten f�r den
Systemabschlu� starten, die man h�ufiger mal vergi�t. Dazu k�nnen sowohl
Benutzerabrechnungen als auch externe, kompliziertere Programmbeendigungen
z.B. im Netzwerk z�hlen.

:p.Weitere Hinweise f�r die Gestaltung der Crontabs-Eintr�ge findet man in der
:link reftype=hd refid=XMP_SYNTAX.Syntaxbeschreibung:elink. und bei den
:link reftype=hd refid=XMP_XMP.Beispielen:elink..

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink..
.*-----------------------------------------------------------------------------
:h3 res=10303 name=IDDE_CRON_FILENAME.Dateiname
:i1.Dateiname (Crontabs)
:p.Der Dateiname f�r die Crontabs-Datei l��t sich nur durch das Anw�hlen der
Schaltfl�che
:link reftype=hd refid=IDDE_CRON_FILENAMECHANGE.Dateiname �ndern:elink.
ver�ndern. Es wird der aktuelle Pfad und Dateiname der Crontabs-Datei
angezeigt. Nach dem ersten Aufruf des Programms wird standardgem�� die
Datei :hp2.Crontabs:ehp2. aus dem gleichen Verzeichnis gew�hlt, wo auch der
Cron D�mon steht. Treten dabei Fehler auf (Schreibschutz, Verzeichnis namens
:hp2.Crontabs:ehp2. existiert, etc.), so wird die Datei :hp2.nul:ehp2. gew�hlt.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink..
.*-----------------------------------------------------------------------------
:h3 res=10304 name=IDDE_CRON_FILENAMECHANGE.Dateiname �ndern
:i1.�ndern des Dateinamens (Crontabs)
:p.Mit dieser Schaltfl�che l��t sich der Dateiname und der Pfad der
Crontabs-Datei ver�ndern. Dabei wird ein Standardfenster des Systems ge�ffnet,
das auch von anderen Programmen genutzt wird, um Dateien einzulesen.

:p.Falls die angegebene Datei noch nicht existiert, so wird sie angelegt.
Es mu� ein normaler Dateiname angegeben werden. Die einzige Ausnahme ist die
Datei :hp2.nul:ehp2., die als "Notdatei" f�r Fehler fungiert. Falls die
Datei bereits existiert, so wird nach der Best�tigung der Inhalt �berpr�ft.
Dabei mu� f�r jede Zeile die
:link reftype=hd refid=XMP_SYNTAX.Syntax:elink. eingehalten werden. Nur nach
dem erfolgreichen Einlesen der Datei wird die urspr�ngliche Datei geschlossen
und die neue verwendet.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink..
.*=============================================================================
:h2 res=10310 name=IDD_CRONTABS.Crontabs-Eintr�ge
:i1.Crontabs-Eintr�ge
:i1.Eintr�ge
:p.Diese Seite aus dem :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink.
erlaubt es, die einzelnen Eintr�ge aus der Crontabs-Datei zu ver�ndern und
anzusehen.

:p.Durch das Ausw�hlen von :link
reftype=hd refid=IDDE_CRONTABS_ADD.Hinzuf�gen:elink. wird der im
Feld :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.Neuer Befehl:elink. eingesetzte Befehl
an die bestehende Datei angeh�ngt und zur gegebenen Zeit ausgef�hrt. Der Befehl
mu� die
:link reftype=hd refid=XMP_SYNTAX.Syntax:elink. eingehalten.

:p.Das Ausw�hlen von :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.�ndern:elink. entspricht einem L�schen
und gleichzeitigem Hinzuf�gen des zu bearbeitenden Eintrags.

:p.Durch das Ausw�hlen von :link
reftype=hd refid=IDDE_CRONTABS_ABORT.Abbruch:elink. wird ein Bearbeitungswunsch
eines Eintrags widerrufen.

:p.Durch das Ausw�hlen von :link
reftype=hd refid=IDDE_CRONTABS_DELETE.L�schen:elink. werden alle
markierten Elemente der :link
reftype=hd refid=IDDE_CRONTABS_LIST.Eintragsliste:elink. gel�scht.

:p.Durch das Ausw�hlen von :link
reftype=hd refid=IDDE_CRONTABS_EDIT.Bearbeiten:elink. wird das markierte
Element der :link reftype=hd refid=IDDE_CRONTABS_LIST.Eintragsliste:elink.
in das Editierfeld kopiert. Nach erfolgter Bearbeitung kann man durch das
Dr�cken von :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.�ndern:elink. das Element ersetzen.

:p.Alle Druckkn�pfe sind nur dann ausf�hrbar, wenn eine g�ltige Operation
damit m�glich ist.
.*-----------------------------------------------------------------------------
:h3 res=10311 name=IDDE_CRONTABS_LIST.Eintragsliste
:i1.Eintragsliste
:p.In dieser Liste sind alle vorhandenen Eintr�ge der Crontabs-Datei
aufgef�hrt. Man kann einzelne Elemente mit der linken Maustaste oder der
Leertaste ausw�hlen und die dann �ber die Schaltfl�che :link
reftype=hd refid=IDDE_CRONTABS_DELETE.L�schen:elink. entfernen.
:hp2.Vorsicht, es gibt keine Funktion zum Zur�cknehmen des L�schvorgangs.:ehp2.

:p.Alternativ kann man auch einen Eintrag :link
reftype=hd refid=IDDE_CRONTABS_EDIT.bearbeiten:elink..

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-Eintr�ge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10312 name=IDDE_CRONTABS_ENTRY.Crontabs-Eintrag
:i1.Crontabs-Eintrag (Neu)
:p.In diesem Feld kann ein neuer Crontabs-Befehl eingesetzt werden. Er mu� die
:link reftype=hd refid=XMP_SYNTAX.Syntax:elink.konventionen einhalten. Nach dem
Editieren dr�ckt man die Schaltfl�che :link
reftype=hd refid=IDDE_CRONTABS_ADD.Hinzuf�gen:elink., um den Eintrag
in die Crontabs-Datei zu �bertragen. Treten keine Fehler auf, wird das Feld
gel�scht und der Eintrag an das Ende der :link
reftype=hd refid=IDDE_CRONTABS_LIST.Eintragsliste:elink. gesetzt. Bei einem
Fehler verbleibt die Eingabe im Feld und es wird eine Fehlermeldung angezeigt.

:p.Wurde das Eintragsfeld durch das :link
reftype=hd refid=IDDE_CRONTABS_EDIT.Bearbeiten:elink. eines vorhandenen
Eintrags gef�llt, kann man den alten Eintrag mit :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.�ndern:elink. ersetzen oder mit :link
reftype=hd refid=IDDE_CRONTABS_ABORT.Abbruch:elink. den �nderungswunsch
zur�ckweisen.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-Eintr�ge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10313 name=IDDE_CRONTABS_ADD.Eintrag hinzuf�gen
:i1.Crontabs-Eintrag hinzuf�gen
:p.Durch das Benutzen dieser Schaltfl�che wird der im Feld :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.Neuer Befehl:elink. eingegebene Eintrag
�berpr�ft und gegebenenfalls in die Liste der vorhandenen Eintr�ge �bernommen.

:p.Der Eintrag mu� die
:link reftype=hd refid=XMP_SYNTAX.Syntax:elink.konventionen einhalten.
Bei einem Fehler verbleibt die Eingabe im Feld und es wird eine Fehlermeldung
angezeigt.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-Eintr�ge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10316 name=IDDE_CRONTABS_CHANGE.Eintrag �ndern
:i1.Crontabs-Eintrag �ndern
:p.Durch das Benutzen dieser Schaltfl�che wird der im Feld :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.Neuer Befehl:elink. eingegebene Eintrag
�berpr�ft und gegebenenfalls in die Liste der vorhandenen Eintr�ge �bernommen.
Er ersetzt dabei den Eintrag, aus dem der Inhalt des Eintragsfeld entstanden
ist.

:p.Der Eintrag mu� die
:link reftype=hd refid=XMP_SYNTAX.Syntax:elink.konventionen einhalten.
Bei einem Fehler verbleibt die Eingabe im Feld und es wird eine Fehlermeldung
angezeigt.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-Eintr�ge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10317 name=IDDE_CRONTABS_ABORT.Eintragsbearbeitung abbrechen
:i1.Crontabs-Eintrags�nderung abbrechen
:p.Durch das Benutzen dieser Schaltfl�che wird die Bearbeitung eines
existierenden Eintrags abgebrochen. Das Eintragsfeld wird dabei gel�scht.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-Eintr�ge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10314 name=IDDE_CRONTABS_DELETE.Eintr�ge l�schen
:i1.Crontabs-Eintr�ge l�schen
:p.Alle markierten Eintr�ge aus der
:link reftype=hd refid=IDDE_CRONTABS_LIST.Eintragsliste:elink. werden
unwiderruflich gel�scht.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-Eintr�ge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10315 name=IDDE_CRONTABS_EDIT.Eintrag bearbeiten
:i1.Crontabs-Eintrag bearbeiten
:p.Der markierte Eintrag aus der
:link reftype=hd refid=IDDE_CRONTABS_LIST.Eintragsliste:elink. wird zur
Ver�nderung in das Feld :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.Neuer Befehl:elink. kopiert. Der Eintrag
verbleibt aus Sicherheitsgr�nden in der Liste und wird durch das Dr�cken
von :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.�ndern:elink. ersetzt. Mit :link
reftype=hd refid=IDDE_CRONTABS_ABORT.Abbruch:elink. kann man den
�nderungswunsch zur�ckweisen.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-Eintr�ge:elink..
.*=============================================================================
:h2 res=10400 name=IDD_OUTPUT.Ausgabe
:i1.Ausgabe
:p.Diese Seite aus dem :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink.
erlaubt es, den Ausgabepfad und die Eigenschaften der Ausgabe vom D�mon und
den aufgerufenen Programmen festzulegen.

:p.Durch das Ausw�hlen von :link
reftype=hd refid=IDDE_OUTPUT_COUNT.Anzahl der Ausgabedateien:elink. und :link
reftype=hd refid=IDDE_OUTPUT_THRES.Dateigr��e-Schwellenwert:elink. legt man
den maximal benutzten Plattenplatz fest.

:p.Der Pfad der Ausgabedateien wird ebenfalls angezeigt; zur
:link reftype=hd refid=IDDE_OUTPUT_PATHCHANGE.Pfad�nderung:elink.
mu� aber eine Schaltfl�che benutzt werden.

:p.Es besteht die M�glichkeit, alle :link
reftype=hd refid=IDDE_OUTPUT_IGNORE_OUTPUT.Ausgaben zu ignorieren:elink..

:p.Die Unixversion von Cron schickt jedem Benutzer zu jedem Auftrag dessen
Ausgabe und den Programmstatus per Post zu. Da OS/2 weder ein standardisiertes
Postsystem, noch Multiuserf�higkeit besitzt, schien es geschickter, alle
Ausgaben und Meldungen in eine Datei zu lenken. Bei unregelm��iger Wartung
w�rde aber nach und nach die Plattenkapazit�t �berschritten. Deshalb wird
immer dann auf eine neue Datei gewechselt, wenn der Schwellenwert �berschritten
wurde. Alle Ausgabedateien haben den Dateinamen :hp2.Cron000?.out:ehp2..
Dabei durchl�uft das Fragezeichen alle Ziffern von 0 bis zur gegebenen Anzahl
der Ausgabedateien.

:p.Die aktuelle Ausgabedatei ist im permanenten Zugriff, sofern die Ausgabe
nicht komplett ignoriert wird. Nicht alle Programme kommen mit diesem Zugriff
zurecht, um die Datei anzuzeigen. (Der Befehl TYPE funktioniert aber.)
.br
Deshalb kann man im Men� die :link
reftype=hd refid=IDME_NEXTFILE.n�chste Ausgabedatei:elink. ausw�hlen, um die
aktuelle Ausgabedatei freizugeben. Diese kann dann editiert oder angesehen
werden.
.*-----------------------------------------------------------------------------
:h3 res=10401 name=IDDE_OUTPUT_THRES.Schwellenwert
:i1.Schwellenwert
:p.Die aktuelle Ausgabedatei wird nach dem Erreichen des Schwellenwertes
gewechselt. Es k�nnen nur vorgew�hlte Schwellenwerte benutzt werden.

:p.Der Zusammenhang mit den anderen Werten ist in der Hilfe zur
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink. beschrieben.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink..
.*-----------------------------------------------------------------------------
:h3 res=10402 name=IDDE_OUTPUT_COUNT.Dateianzahl
:i1.Ausgabe-Dateianzahl
:p.Die Ausgabedateien durchlaufen die Namen :hp2.Cron000?.out:ehp2., wobei das
Fragezeichen die Werte von 0 bis zur Anzahl annimmt.

:p.Der Zusammenhang mit den anderen Werten ist in der Hilfe zur
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink. beschrieben.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink..
.*-----------------------------------------------------------------------------
:h3 res=10403 name=IDDE_OUTPUT_PATH.Pfad
:i1.Ausgabepfad
:p.Dieses Feld zeigt den Pfad f�r die Ausgabedateien. Der voreingestellte Wert
ist der Pfad, von dem der D�mon gestartet wurde, also nicht das jeweils
aktuelle Verzeichnis.

:p.Zur �nderung mu� die Schaltfl�che
:link reftype=hd refid=IDDE_OUTPUT_PATHCHANGE.Pfad �ndern:elink. benutzt
werden. Dann wird die zugeh�rige
:link reftype=hd refid=IDD_OUTPUT_PATH.Auswahlbox:elink. ge�ffnet.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink..
.*-----------------------------------------------------------------------------
:h3 res=10404 name=IDDE_OUTPUT_PATHCHANGE.Pfad �ndern
:i1.Ausgabepfad �ndern (Schaltfl�che)
:p.Mit dieser Schaltfl�che �ffnet sich eine
:link reftype=hd refid=IDD_OUTPUT_PATH.Auswahlbox:elink., mit der der Pfad
f�r die Ausgabedateien ver�ndert werden kann. Nach einer erfolgreichen
Ver�nderung wird der Pfad im
:link reftype=hd refid=IDDE_OUTPUT_PATH.Anzeigefeld:elink. aufgefrischt.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink..
.*-----------------------------------------------------------------------------
:h3 res=10405 name=IDDE_OUTPUT_IGNORE_OUTPUT.Ausgabe ignorieren
:i1.Ausgabe ignorieren
:p.Es ist m�glich, alle Ausgaben sowohl des D�mons, als auch der aufgerufenen
Programme zu ignorieren, sie werden dann nicht in eine Datei geschrieben.

:p.Diese Einstellung ist die Voreinstellung beim ersten Programmstart. Es wird
empfohlen, die Einstellungen auf dieser Seite den lokalen Bed�rfnissen
anzupassen und die Ausgabe dann freizugeben, um Fehler bei den Programmen zu
erkennen.

:p.Falls diese Auswahl gesetzt ist, ist die Wahl der :link
reftype=hd refid=IDME_NEXTFILE.n�chsten Ausgabedatei:elink. im Hauptmen� nicht
m�glich.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink..
.*-----------------------------------------------------------------------------
:h3 toc=4 res=10410 name=IDD_OUTPUT_PATH.Ausgabepfad �ndern
:i1.Ausgabepfad �ndern (Dialog)
:p.Dieses Dialogfenster �ffnet sich durch das Bedienen der Schaltfl�che zum
:link reftype=hd refid=IDDE_OUTPUT_PATHCHANGE.Pfad �ndern:elink..

:p.Der aktuelle Pfad der Ausgabedateien wird im :link
reftype=hd refid=IDDE_OUTPUT_PATH_ENTRY.Ausgabepfad-Eingabefeld:elink.
angezeigt. Bei jedem doppelten Mausklick in die :link
reftype=hd refid=IDDE_OUTPUT_PATH_LIST.Ausgabepfad-Liste:elink. wird der
entstehende Pfad neu angezeigt.

:p.Selbstverst�ndlich kann man den neuen Pfad auch per Hand eingeben.

:p.Diese Dialogbox wird von der Seite
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink. im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink.
aufgerufen.
.*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
:h4 res=10411 name=IDDE_OUTPUT_PATH_ENTRY.Ausgabepfad-Eingabefeld
:i1.Ausgabepfad-Eingabefeld
:p.Das Eingabefeld f�r die Ausgabe wird entweder durch eine Benutzereingabe
oder durch das Bedienen der
:link reftype=hd refid=IDDE_OUTPUT_PATH_LIST.Ausgabepfad-Liste:elink.
ver�ndert. Initial wird der Wert auf den aktuellen Ausgabepfad gesetzt.

:p.Nach dem Dr�cken der Best�tigungstaste wird �berpr�ft, ob der Pfad
tauglich ist. Dazu mu� er existieren und die M�glichkeit bestehen, zehn
Dateien der Form :hp2.Cron000?.out:ehp2. zu erzeugen oder zu ver�ndern.
Das Fragezeichen durchl�uft dabei alle Dezimalziffern.
.br
Bei einem Fehler wird ein Mitteilungsfenster ge�ffnet und auf den Umstand
hingewiesen. Man mu� dann einen anderen Pfad w�hlen, abbrechen oder mit
einem anderen Programm den Fehler beheben (z.B. den Pfad anlegen).

:p.Dieser Eintrag befindet sich in der Dialogbox zur
:link reftype=hd refid=IDD_OUTPUT_PATH.�nderung des Pfades:elink., die von
der Seite
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink. im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink.
aufgerufen wird.
.*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
:h4 res=10412 name=IDDE_OUTPUT_PATH_LIST.Ausgabepfad-Liste
:i1.Ausgabepfad-Liste
:p.Mit der Liste f�r Ausgabepfade hat man eine komfortable M�glichkeit,
den aktuellen Ausgabepfad zu ver�ndern.

:p.In der Liste sind die Laufwerksbuchstaben (ohne Floppies) und alle
Unterverzeichnisse des aktuellen Pfades vertreten. Durch ein doppeltes Klicken
der linken Maustaste auf eine dieser Komponenten wird sie ausgew�hlt und an
den aktuellen Pfad angeh�ngt, bzw. der aktuelle Pfad ersetzt.

:p.Das
:link reftype=hd refid=IDDE_OUTPUT_PATH_LIST.Ausgabepfad-Eingabefeld:elink.
pa�t seinen Wert automatisch an. Durch das Bedienen der Eingabetaste wird die
aktuelle Auswahl �bernommen und der Wert des Eingabefeldes getestet.

:p.Dieser Eintrag befindet sich in der Dialogbox zur
:link reftype=hd refid=IDD_OUTPUT_PATH.�nderung des Pfades:elink., die von
der Seite
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink. im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink.
aufgerufen wird.
.*=============================================================================
:h2 toc=3 res=10500 name=IDD_TCP.TCP/IP
:i1.TCP/IP
:p.Diese Seite aus dem :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink.
erlaubt es, die M�glichkeiten einer Bedienung durch TCP/IP-vernetzte
Fremdmaschinen festzulegen.

TCP/IP kennt keinen eigenen Zugriffsschutz. Es ist dem System nicht m�glich,
einzelne Kommunikationen zu verbieten. Um nicht eine riesige Sicherheitsl�cke
aufzutun, erlaubt der Cron D�mon nur die Kommunikation mit bestimmten
Partnerrechnern �ber TCP/IP. Die Namen der Partnerrechner stehen in einer
Datei, pro Rechner eine Zeile. Jeder Rechnername darf in der Nummernnotation
(z.B. 128.176.248.76) oder in der allgemeinen Namenskonvention (z.B.
Sauron@uni-muenster.de) angegeben werden.

:p.Der Dateiname und Pfad der Erlaubnisdatei wird in einem Feld angezeigt; zur
:link reftype=hd refid=IDDE_TCP_FILENAMECHANGE.�nderung:elink.
mu� aber eine Schaltfl�che benutzt werden.

:p.Erst durch das Ausw�hlen von :link
reftype=hd refid=IDDE_TCP_ALLOWACCESS.Zugriff erlauben:elink. wird ein
prinzipieller Zugriff von Fremdrechnern m�glich.
.br
Falls auf dem aktuellen Rechner kein TCP/IP installiert ist, ist diese
Schaltfl�che nicht anw�hlbar.

:p.Die Bearbeitung der Crontabs-Datei aus der Ferne erfolgt durch das
Hilfsprogramm :hp2.crontab:ehp2..

:p.Die Portnummer des Cron D�mons ist 64020.
.*-----------------------------------------------------------------------------
:h3 res=10501 name=IDDE_TCP_FILENAME.Dateiname
:i1.Dateiname (TCP/IP-Zugriffserlaubnisse)
:p.Der Dateiname f�r die TCP/IP-Zugriffserlaubnisse l��t sich nur durch das
Anw�hlen der Schaltfl�che
:link reftype=hd refid=IDDE_TCP_FILENAMECHANGE.Dateiname �ndern:elink.
ver�ndern. Es wird der aktuelle Pfad und Dateiname der Erlaubnisdatei-Datei
angezeigt. Nach dem ersten Aufruf des Programms wird standardgem�� die
Datei :hp2.nul:ehp2. benutzt.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_TCP.TCP/IP-Zugriffe:elink..
.*-----------------------------------------------------------------------------
:h3 res=10502 name=IDDE_TCP_FILENAMECHANGE.Dateiname �ndern
:i1.�ndern des Dateinamens (TCP/IP-Zugriffserlaubnisse)
:p.Mit dieser Schaltfl�che l��t sich der Dateiname und der Pfad der
TCP/IP-Zugriffserlaubnis-Datei ver�ndern. Dabei wird ein Standardfenster des
Systems ge�ffnet, das auch von anderen Programmen genutzt wird, um Dateien
einzulesen.

:p.Falls die angegebene Datei noch nicht existiert, so wird sie angelegt.
Es mu� ein normaler Dateiname angegeben werden. Die einzige Ausnahme ist die
Datei :hp2.nul:ehp2., die als "Notdatei" f�r Fehler fungiert. Falls die
Datei bereits existiert, so wird nach der Best�tigung der Inhalt �berpr�ft.
Dabei mu� in jeder Zeile ein Rechnername in IP-Notation stehen. Leerzeilen und
Zeilen, die mit einem Doppelkreuz (:hp2.#:ehp2.) oder Semikolon (:hp2.;:ehp2.)
beginnen, werden als Kommentarzeilen angesehen und ignoriert.
Nur nach dem erfolgreichen Einlesen der Datei wird die urspr�ngliche Datei
geschlossen und die neue verwendet.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_TCP.TCP/IP-Zugriffe:elink..
.*-----------------------------------------------------------------------------
:h3 res=10503 name=IDDE_TCP_ALLOWACCESS.Zugriff erlauben
:i1.TCP/IP-Zugriff erlauben
:i1.Zugriff erlauben (TCP/IP)
:p.Diese Schaltfl�che regelt den allgemeinen TCP/IP-Zugang von Fremdrechner.
Ungeachtet der :link
reftype=hd refid=IDDE_TCP_FILENAME.Zugriffserlaubnisdatei:elink., die die
Fremdrechner auflistet, steht hier ein Mittel zur Verf�gung, um generell den
Zugriff zu verbieten.

:p.Diese Schaltfl�che ist nicht anw�hlbar, wenn der Rechner nicht mit TCP/IP
ausgestattet ist.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_TCP.TCP/IP-Zugriffe:elink..
.*=============================================================================
:h2 res=10600 name=IDD_GENERAL.Allgemeine Einstellungen
:i1.Allgemeine Einstellungen
:p.Diese Seite aus dem :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink.
erlaubt es, allgemeine Einstellungen des Programms zu ver�ndern.

Zur Zeit wird auf dieser Seite nur die bevorzugte Sprache ausgew�hlt.
Standardgem�� steht diese beim ersten Programmstart auf "English", sofern
nicht die Umgebungsvariable :hp2.LANG:ehp2. auf etwas anderes verweist.
Unterst�tzte :hp2.LANG:ehp2.-Werte sind&colon.
:dl compact tsize=15.
:dthd.:hp5.LANG-Wert:ehp5.
:ddhd.:hp5.Sprache:ehp5.
:dt.USA
:dd.English
:dt.UK
:dd.English
:dt.GERM
:dd.Deutsch
:dt.FRAN
:dd.Fran�ais
:edl.
:p.Ge�ndert werden die Sprache durch das Bedienen der :link
reftype=hd refid=IDDE_GENERAL_LANGUAGE.Sprachauswahlliste:elink..
.*-----------------------------------------------------------------------------
:h3 res=10601 name=IDDE_GENERAL_LANGUAGE.Bevorzugte Sprache
:i1.Sprache
:p.Die bevorzugte Sprache l��t sich durch das Bedienen dieser Liste ausw�hlen.
:link refid=NeueSprachen reftype=fn.Es steht nur eine begrenzte Auswahl zur
Verf�gung.:elink.

:p.Standardgem�� steht die Sprache beim ersten Programmstart auf "English",
sofern nicht die Umgebungsvariable :hp2.LANG:ehp2. auf etwas anderes verweist.
In der Hilfe zu den
:link reftype=hd refid=IDD_GENERAL.allgemeinen Einstellungen:elink. stehen dazu
weitere Informationen.


M�gliche Sprachen sind zur Zeit&colon.
:ul.
:li.Deutsch
:li.Englisch
:li.Franz�sisch
:eul.

:p.Eine �nderung wird erst nach einem Neustart des Programms wirksam.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite f�r die
:link reftype=hd refid=IDD_GENERAL.allgemeinen Einstellungen:elink..
:fn id=NeueSprachen.Das mag sich �ndern, falls ein paar Leute diesem Kram
�bersetzen!:efn.
.******************************************************************************
:h1 name=XMP_GENERAL.Beispiele und Syntax von Crontabs-Eintr�gen
:p.In diesem Kapitel wird die :link reftype=hd refid=XMP_SYNTAX.Syntax:elink.
der Crontabs-Eintr�ge beschrieben und die Benutzung anhand von
:link reftype=hd refid=XMP_XMP.Beispielen:elink. demonstriert.

:p.Im Gegensatz zur Syntax des Programms :hp2.crontabs:ehp2. und zu m�glichen
Eingaben von :link
reftype=hd refid=IDD_CRONTABS.Crontabs-Eintr�ge:elink. innerhalb dieses
Programms d�rfen in der Datei auch Kommentare auftauchen.

:p.Kommentarzeilen sind leere Zeilen oder Zeilen, die in der ersten Spalte ein
Semikolon (:hp2.;:ehp2.) oder Doppelkreuz (:hp2.#:ehp2.) stehen haben.
.*=============================================================================
:h2 name=XMP_SYNTAX.Syntax der Crontabs-Eintr�gen
:i1.Syntax der Crontabs-Eintr�gen
:p.Die Syntax von Crontabs-Eintr�gen ist recht einfach. Jeder Eintrag mu� in
einer Zeile stehen. Es wird nicht zwischen Gro�- und Kleinschreibung
unterschieden. Jede Zeile hat die Form
:xmp.[Flaggen] Zeiten Kommando:exmp.
:p.Die :hp2.Flaggen:ehp2. sind optional. M�gliche Flaggen sind zur Zeit&colon.
:dl compact.
:dthd.:hp5.Flagge:ehp5.
:ddhd.:hp5.Bedeutung:ehp5.
:dt.:hp2.Once:ehp2.
:dd.Das Kommando wird zur gegebenen Zeit einmal ausgef�hrt und danach aus der
Kommandoliste gel�scht.
:edl.

:p.Die :hp2.Zeiten:ehp2. haben in der Regel f�nf Eintr�ge, die :hp2.alle:ehp2.
zutreffen m�ssen, damit das Kommando ausgef�hrt wird&colon.
:ol.
:li.:hp2.Minuten:ehp2.
:p.Diese :link refid=Zahlenangabe reftype=fn.Zahlenangabe:elink.
beschreibt die Minute innerhalb der Stunde, in der das
Kommando gestartet werden soll. M�gliche Werte liegen zwischen 0 und 59.
:li.:hp2.Stunden:ehp2.
:lp.Diese :link refid=Zahlenangabe reftype=fn.Zahlenangabe:elink.
beschreibt die Stunde, zu der das
Kommando gestartet werden soll. M�gliche Werte liegen zwischen 0 und 23.
:li.:hp2.Monatstag:ehp2.
:lp.Diese :link refid=Zahlenangabe reftype=fn.Zahlenangabe:elink.
beschreibt den Monatstag, an dem das
Kommando gestartet werden soll. M�gliche Werte liegen zwischen 1 und 31, wobei
das Maximum nicht vom jeweiligen Monat abh�ngt. Im Februar werden also
Kommandos mit dem Monatstag 31 nicht ausgef�hrt, obwohl die Angabe legal ist.
:li.:hp2.Monat:ehp2.
:lp.Diese :link refid=Zahlenangabe reftype=fn.Zahlenangabe:elink.
beschreibt den Monat, in dem das
Kommando gestartet werden soll. M�gliche Werte liegen zwischen 1 und 12.
:li.:hp2.Wochentag:ehp2.
:lp.Diese :link refid=Zahlenangabe reftype=fn.Zahlenangabe:elink.
beschreibt den Wochentag, an dem das
Kommando gestartet werden soll. M�gliche Werte liegen zwischen 0 und 6. Der
Wert 0 beschreibt den Sonntag.
:eol.

:p.Es gibt noch drei Spezialzeiten, die jeweils alle 5 Angaben ersetzen&colon.
:dl compact tsize=15.
:dthd.:hp5.Sonderzeit:ehp5.
:ddhd.:hp5.Bedeutung:ehp5.
:dt.:hp2.Cronstart:ehp2.
:dd.Das Kommando wird beim Start des D�mon ausgef�hrt, sofern die Freigabe in
der :link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink. erfolgt
ist.
:dt.:hp2.Cronstop:ehp2.
:dd.Das Kommando wird beim Beenden des D�mon ausgef�hrt, sofern die Freigabe in
der :link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink. erfolgt
ist.
.br
:hp1.VORSICHT&colon. Es verbleibt nur eine sehr kurze Verz�gerungsfrist,
bevor der D�mon sich dann beendet. Im Falle eines Systemabschlusses kann die
verbliebene Zeit bis zum Booten ggf. nicht ausreichen, um komplizierte
Befehlsfolgen auszuf�hren. Eventuell wird in sp�teren Versionen eine
benutzerdefinierbare maximale Verz�gerungszeit angeboten, dazu sollten aber
Bedarfsw�nsche ge�u�ert werden.:ehp1.
:dt.:hp2.Daily:ehp2.
:dd.Das Kommando wird einmal t�glich bei der ersten M�glichkeit ausgef�hrt.
Man beachte, da� das Kommando :hp2.nicht:ehp2. beim Hinzuf�gen zur Liste
ausgef�hrt wird, sondern erst ab dem n�chsten Tag gestartet wird, falls der
D�mon am aktuellen Tag bereits gelaufen hat oder l�uft.
:edl.

:p.Als :hp2.Kommando:ehp2. ist jedes beliebige OS/2-Kommando erlaubt. Zur Zeit
gibt es noch keine genauere Spezifizierung des Programmtyps (das �ndert sich!),
so da� man bei DOS-Programmen am besten den OS/2-Befehl :hp2.start:ehp2. zur
Hilfe nimmt. Die Standardausgaben des Programms werden in einer tempor�ren
Datei gesammelt und beim Beenden des Programms in die :link
reftype=hd refid=IDD_OUTPUT.Ausgabe:elink.datei kopiert. Da bei der Benutzung
von :hp2.start:ehp2. ein weiterer, unabh�ngiger Proze� gestartet wird, der
sich der Kontrolle des D�mons entzieht, sollte die Ausgabe der Programme
gel�scht oder umgelenkt werden. Dazu gibt man am Ende der Zeile
:xmp. >Datei 2>&amp.1:exmp.
:p.an. Als Dateiname kann man die Datei :hp2.nul:ehp2. angeben, um die Ausgabe
zu ignorieren. Da PM-Programme normalerweise keine Ausgabe haben, sollte dies
bei solchen Programmen immer angegeben werden.

:p.Die tempor�ren Dateien werden im gleichen Unterverzeichnis erzeugt, wo auch
das Programm des Cron D�mon steht. Es empfiehlt sich, "Leichen" durch die
beiden Crontabs-Befehle
:xmp.
Cronstart del ?&colon.\Pfad\*.tmp >nul 2>&amp.1
0 3 * * 0 del ?&colon.\Pfad\*.tmp >nul 2>&amp.1
:exmp.
:p.regelm��ig zu entfernen. Das Fragezeichen und Pfad sollten durch die
entsprechenden Werte ersetzt werden.

:fn id=Zahlenangabe.Eine Zahlenangabe ist eine durch Kommata getrennte Liste
von Zahlen oder Zahlintervallen, die das Ereignis treffen m�ssen.
Ein Stern (:hp2.*:ehp2.) bedeutet&colon.
jede m�gliche Zahl. Beispiele&colon.
:xmp.1,2,5:exmp.
:p.oder
:xmp.8-12,14-18:exmp.
:p.oder
:xmp.*:exmp.:efn.
.*=============================================================================
:h2 name=XMP_XMP.Beispiele f�r Crontabs-Eintr�ge
:i1.Beispiele
:p.Es gibt eine Vielzahl von Einsatzm�glichkeiten des Cron D�mon, von denen
einige anhand von Beispielen vorgestellt werden sollen. Dabei werden als
Kommandos auch Programmnamen benutzt, die garantiert nicht auf jedem System
verf�gbar sind, aber durch ihre sprechenden Namen den Einsatz aufzeigen.

:ul.
:li.:font facename=Courier size=18x10.* * * * * echo Eine Minute ist um.:font
facename=default size=0x0.
:p.Jede Minute in jeder Stunde an jedem beliebigen Tag wird die Zeile
:hp2.Eine Minute ist um.:ehp2. ausgegeben. Diese Ausgabe des Kommandos echo
wird in die :link
reftype=hd refid=IDD_OUTPUT.Ausgabe:elink.datei des Cron D�mons kopiert.
:li.:font facename=Courier size=18x10.0 * * * * echo Eine Stunde ist um. >nul 2>&amp.1:font
facename=default size=0x0.
:p.Zu jeder vollen Stunde an jedem beliebigen Tag wird die Zeile
:hp2.Eine Stunde ist um.:ehp2. ausgegeben. Da die Ausgabe aber verworfen wird,
ist dieser Befehl unsinnig (echo ist ja nur f�r Ausgaben zust�ndig). Immerhin
erscheint noch in der Ausgabedatei des D�mons die Mitteilung, da� das Programm
gestartet wurde, sofern die Ausgabefunktionen prinzipiell benutzt werden.
:li.:font facename=Courier size=18x10.0,30 * * * * DeliverMail:font
facename=default size=0x0.
:p.Zu jeder vollen und halben Stunde wird das Programm DeliverMail gestartet.
Alle Ausgaben des Programms erscheinen auch in der Ausgabedatei des D�mons.
:li.:font facename=Courier size=18x10.0,30 * * * * start PMDeliverMail >nul 2>&amp.1:font
facename=default size=0x0.
:p.Zu jeder vollen und halben Stunde wird das Programm PMDeliverMail in einer
separaten Sitzung gestartet (n�tig bei PM-Programmen!). Alle Ausgaben werden
verworfen.
:li.:font facename=Courier size=18x10.0 3 * * 0 MyBackup /weekly:font
facename=default size=0x0.
:p.An jedem Sonntag um drei Uhr morgens wird das Programm MyBackup mit dem
Parameter /weekly gestartet.
:li.:font facename=Courier size=18x10.0 3 1 * * MyBackup /monthly:font
facename=default size=0x0.
:p.An jedem 1. eines Monats um drei Uhr morgens wird das Programm MyBackup mit
dem Parameter /monthly gestartet.
:li.:font facename=Courier size=18x10.Once 0 3 1 4 * time 4&colon.0&colon.0:font
facename=default size=0x0.
:p.Am 1. April um 3 Uhr morgens wird einmalig die Zeit um eine Stunde
vorgestellt.
:li.:font facename=Courier size=18x10.Once * * * * * setboot /b >nul 2>&amp.1:font
facename=default size=0x0.
:p.Die Maschine wird einmalig (wichtig!) zum n�chstm�glichen Zeitpunkt neu
gestartet. Alle Ausgaben des Programms werden ignoriert. Vorsicht bei einer
Nachahmung, :hp2.setboot /b:ehp2. entspricht einem Dr�cken von Alt-Strg-Entf.
Dieser Befehl kann z.B. n�tzlich sein, um einem Stromausfall zuvorzukommen.
(Na gut, das wei� man nicht immer vorher, aber wenn der/die Kollege/in gerade
an Kabeln von ungesch�tzten Stromkreisen rumfummelt, kann dies noch die letzte
Rettung f�r das Plattensystem eines Servers drei R�ume weiter sein. Alles
schon erlebt...)
:li.:font facename=Courier size=18x10.0 3 * * * deltree f&colon.\tmp &amp. mkdir f&colon.\tmp &amp. echo Das Verzeichnis wird morgens um drei gel�scht! > f&colon.\tmp\Warnung.!!!:font
facename=default size=0x0.
:p.Um drei Uhr morgens wird zuerst der Befehl deltree mit dem Parameter
f&colon.\tmp aufgerufen, dann das Verzeichnis neu erzeugt und eine
Warnungsdatei in diesem Verzeichnis generiert. Alles geschieht �ber einen
einzigen Befehlsinterpreter, so da� die Ausgaben der Programme sch�n
hintereinander in der Ausgabedatei des D�mon stehen.
:li.:font facename=Courier size=18x10.Cronstart del c&colon.\cron\*.tmp >nul 2>&amp.1:font
facename=default size=0x0.
:p.Bei jedem Start des D�mons werden alle tempor�ren Dateien aus dem
Cron-Verzeichnis auf Laufwerk C gel�scht. Alle Ausgaben werden ignoriert.

:p.Dieser Befehl wird nur dann ausgef�hrt, wenn auch die Freigabe in
der :link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink. erfolgt
ist.

:p.Dieser Befehl ist recht n�tzlich, da erfahrungsgem�� immer ein paar
tempor�re Dateien vom D�mon nicht gel�scht werden k�nnen. Das ist kein Fehler
des D�mons, sondern meistens eine fehlerhafte Crontabs-Zeile mit
:hp2.start:ehp2. aber ohne Ausgabeumlenkung. (Die tempor�re Ausgabedatei ist
dann f�r den D�mon blockiert.)
:li.:font facename=Courier size=18x10.Cronstop echo Abgemeldet&colon. >> \Zeiten\florian &amp. time >> \Zeiten\florian:font
facename=default size=0x0.
:p.Beim jedem Beenden des D�mons werden wird eine Meldung und die aktuelle
Uhrzeit an eine Datei angeh�ngt. Da der Befehl :hp2.time:ehp2. von der
Standardeingabe liest, diese aber vom D�mon standardgem�� auf :hp2.nul:ehp2.
gelegt wurde, entf�llt eine Umstellung der Uhrzeit, sie wird nur angezeigt.

:p.Dieser Befehl wird nur dann ausgef�hrt, wenn auch die Freigabe in
der :link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink. erfolgt
ist.

:p.Zusammen mit einem �hnlichen Befehl w�hrend des Hochfahrens l��t sich so
ein rudiment�res Abrechnungsprotokoll fahren. Als Voraussetzung dazu sollte man
aber auch den Cron D�mon schon beim Systemstart aktivieren. Das kann man z.B.
dadurch realisieren, indem man eine Referenz oder Kopie vom D�mon in den
Systemstart-Ordner bringt.

:p.:hp1.VORSICHT&colon. Es verbleibt nur eine sehr kurze Verz�gerungsfrist,
bevor der D�mon sich dann beendet. Im Falle eines Systemabschlusses kann die
verbliebene Zeit bis zum Booten ggf. nicht ausreichen, um komplizierte
Befehlsfolgen auszuf�hren. Eventuell wird in sp�teren Versionen eine
benutzerdefinierbare maximale Verz�gerungszeit angeboten, dazu sollten aber
Bedarfsw�nsche ge�u�ert werden.:ehp1.
:li.:font facename=Courier size=18x10.Daily VirusCheck:font
facename=default size=0x0.
:p.T�glich wird einmal zum fr�hestm�glichen Zeitpunkt das Programm
VirusCheck gestartet.
Man beachte, da� das Kommando :hp2.nicht:ehp2. beim Hinzuf�gen zur Liste
ausgef�hrt wird, sondern erst ab dem n�chsten Tag gestartet wird, falls der
D�mon am aktuellen Tag bereits gelaufen hat oder l�uft.
:li.:font facename=Courier size=18x10.0 8-12,14-18 * * 1-5 start PMBeep >nul 2>&amp.1:font
facename=default size=0x0.
:p.Von Montag bis Freitag wird zu jeder vollen Stunde zwischen 8 und 12 sowie
zwischen 14 und 18 Uhr das Programm PMBeep gestartet. Das Programm l�uft in
einer separaten Sitzung, Ausgaben werden ignoriert.
:eul.
.******************************************************************************
:h1 name=LAN_NETWORK.Benutzung im LAN und unter Netware
:i1.Netzwerk
:i1.Novell-Netware
:i1.LAN-Zugriff
:p.Die Kommunikation mit dem Cron D�mon funktioniert am besten durch den
LAN-Zugriff auf eine Named Pipe, da diese Kommunikationsart vom OS/2 direkt
unterst�tzt wird.

:p.Innerhalb eines Netzwerkes ist diese Kommunikationsart ebenfalls m�glich.
Sowohl das LAN, als auch Novell-Netware bieten dabei Pipe-Komponenten an.
Beide bieten explizite Zugriffsschutzmechanismen, so da� der D�mon darauf
verzichten kann.

:p.Um den D�mon innerhalb des Netzwerkes zur Verf�gung zu stellen, mu� man
mit den jeweiligen Zugriffsschutz-Programmen (z.B. :hp2.net access:ehp2. im
LAN) den Zugriff auf die Pipe
:xmp.
\pipe\Cron-Daemon-Request
:exmp.
:p.f�r die gew�nschten Benutzer(-gruppen) erlauben. Danach kann mit dem
Hilfsprogramm :hp2.crontab:ehp2. die Crontabs-Datei des Rechner fernbedient
werden.
.******************************************************************************
:h1.Neues zwischen den Versionen
:p.Dieses Kapitel enth�lt alle gr��eren �nderungen zwischen den Versionen.
:ul.
:li.:link
reftype=hd refid=CHANG_120_112.�nderungen zwischen der Version 1.20 und
Version 1.12:elink.
:eul.
.*=============================================================================
:h2 name=CHANG_120_112.�nderungen zwischen der Version 1.20 und Version 1.12
:p.Folgende �nderungen sind in Version 1.20 erfolgt&colon.
:ul compact.
:li.Kosmetische �nderungen und diverse Fehlerbeseitigungen.
:li.Unterst�tzung f�r franz�sische Meldungen und Hilfe.
:li.In der PM-Version des D�mons kann ein Job ver�ndert werden.
:li.In Kommandos k�nnen die Zeiten nun auch als Intervalle angegeben werden.
:hp2.0,4,5,6,9:ehp2. kann auch als :hp2.0,4-6,9:ehp2. angegeben werden.
:li.Die Spezialzeit :hp2.Daily:ehp2. erlaubt das Starten eines Kommandos zum
fr�hestm�glichen Zeitpunkt an einem Tag, aber nur einmal pro Tag.
:li.Farben und Font des Hauptfensters k�nnen nun �ber die Schriftartpalette
und die Farbpalette der Systemkonfiguration ge�ndert werden, die Werte
�berdauern einen Programmabschlu�.
.br
Zum �ndern der Vordergrundfarbe benutze man die Taste [Strg] mit der zweiten
Maustaste.
:li.Neben der Position wird nun auch die Darstellung des Fensters (minimiert,
maximiert oder normal) beim Programmabschlu� gespeichert und �bernommen.
:eul.
.******************************************************************************
:h1.Selbstbeweihr�ucherung
:p.Dieses Programm wurde unter OS/2 f�r EMX als PM-Programm entwickelt.
Tips, Fehlermeldungen und Verbesserungsvorschl�ge (siehe Datei
:hp2.program!:ehp2.) werden dankend entgegengenommen.

:p.Bei W�nschen sollte man :hp2.vorher:ehp2. die Datei :hp2.wishlist:ehp2.
durchsehen, ob der Wunsch bereits registriert ist.

:p.Der Klau von Funktionen, Teilen von Programmen oder ganzen Dateien ist
explizit erlaubt. Bei ganzen Dateien oder kaum unterscheidbaren Dateien
greifen aber die allgemeinen Lizenzbestimmungen, damit andere Benutzer nicht
durch defekte Dateien irregeleitet werden. Spenden werden von jedem Sozialamt
gerne entgegengenommen, bei mir reicht 'ne mail mit 'nem simplen
"nicht schlecht".

:p.Vielen Dank an Leonard H�esker. Er hat die englische Version der Hilfe
durchgesehen.

:p.Ebenfalls vielen Dank an die Folgenden. Sie haben den Cron D�mon in die
anderen Sprachen �bersetzt&colon.
:parml compact tsize=15 break=none.
:pt.Franz�sisch
:pd.Rapha�l Vanney
:eparml.

:p.Florian Gro�e-Coosmann
.br
E-Mail im Internet&colon. :font facename=Courier size=18x10.coosman@uni-muenster.de:font
facename=default size=0x0.
:euserdoc.
.* RCS depending informations
.*
.* $Id: cronddeu.ipf 1.3 1995/10/18 11:13:21 Florian Rel $
.*
.* $Name: Version121 $
.*
.* $Log: cronddeu.ipf $
.* Revision 1.3  1995/10/18 11:13:21  Florian
.* Control pushbuttons added.
.*
.* Revision 1.2  1995/10/18 09:46:06  Florian
.* Some cosmetic changes.
.* Chapters Farben und Schriftart and Neues zwischen den Versionen added.
.* Crontabs Chapter rewritten.
.* Daily and intervals explained in XMP and SYNTAX.
.* Added support of the French language.
.*
.* Revision 1.1  1995/08/03 07:57:13  Florian
.* Initial revision
.* Copied from crond.ipf, res-numbers changed, some bugs corrected
.*
.*
