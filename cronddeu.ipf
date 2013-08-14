.* Copyright (c) 1995 Florian Gro·e-Coosmann, RCS section at the eof
.* Use &rbl. for a required blank ( ), &colon. for a colon (:)
:userdoc.
:docprof.
:ctrldef.
:ctrl ctrlid=CtrlID1 controls='ESC SEARCH PRINT INDEX CONTENTS BACK FORWARD'
coverpage.
:ectrldef.
.******************************************************************************
:h1 res=10100 name=ID_CRON.Der Cron DÑmon (öbersicht)
:i1 global.Cron DÑmon
:p.Der Cron DÑmon stammt eigentlich aus der Unix-Welt. Dieses Programm ist
eine Portierung auf OS/2, wobei Unix-spezifische Eigenschaften weggelassen
und OS/2-spezifische hinzugefÅgt wurden, ohne da· eine prinzipielle énderung
der Funktionsweise oder Aufrufmîglichkeiten zugelassen werden mu·te.

:p.Cron ist ein PM-Programm und im Unix-Sinn kein DÑmon, obwohl crond auch
als DÑmon Åber die Kommandozeile :hp1. detach crond :ehp1.
gestartet werden kann. Leider hat er dann nicht seine volle FunktionalitÑt
unter OS/2, da aus dem Hintergrund keine Vordergrundprogramme gestartet werden
kînnen. Die Benutzung ist dann auf eigene Gefahr.

:p.Der Cron DÑmon hat keine Aufrufparameter. Alle
:link refid=IDD_NOTEBOOK reftype=hd.Einstellungen:elink.
erfolgen Åber die OberflÑche. Fehlerhafte EintrÑge (z.B. bei einem Pfadwechsel)
werden auf Voreinstellungen gesetzt, so da· ein Programmstart immer mîglich
sein sollte.

:p.Wie unter Unix besteht die Mîglichkeit, Cron-EintrÑge Åber das
Hilfsprogramm :hp2.crontab:ehp2. zu manipulieren. Da OS/2 ein
Single-User-Betriebssystem ist, entfallen die Optionen :hp2.-r:ehp2. und
:hp2.-l:ehp2. der Unix-Version. Stattdessen werden die einzelnen Befehle
direkt angegeben. FÅr eine vollstÑndige Liste der Aufrufvariationen rufe man
am besten :hp2.crontab:ehp2. ohne Parameter auf. Alle wichtigen Parameter
findet man auch in der
:link reftype=hd refid=XMP_SYNTAX.Syntaxbeschreibung:elink. und bei den
:link reftype=hd refid=XMP_XMP.Beispielen:elink..

:h4.:hp2.Was leistet der Cron DÑmon?:ehp2.
:p.Der Cron DÑmon kann zu gewissen Zeiten Programme starten. Dabei ist eine
Auswahl der Minuten, Stunden, Monatstage, Monate und Wochentage mîglich.
Zu den angegebenen Zeiten wird das Programm dann gestartet. Alle Kombinationen
der Zeiten sind dabei mîglich, so da· man einen repektablen Umfang von
chronologischen Aufgaben durch das System erledigen lassen kann, ohne da· man
ein :link refid=Wecker reftype=fn.Weckerprogramm:elink. benutzen mu·.
:fn id=Wecker.Als Wecker eignet sich der Cron DÑmon selbstverstÑndlich
auch!:efn.

:p.Cron leitet alle Ausgaben der aufgerufenen Programme in eine Ausgabedatei.
Damit lassen sich dann im Nachhinein Fehler in den Programmen analysieren.
Der Unix-DÑmon schickt diese Ausgaben (pro Programmstart) dem entsprechenden
Benutzer mit der Post zu. Diese Vorgehensweise ist aber unter OS/2 recht
ungeschickt, da es weder mehrere Benutzer, noch ein Postsystem gibt. Die
jeweilige Ausgabedatei enthÑlt deshalb alle nîtigen Informationen Åber Cron,
Programmstarts, sowie deren Ausgaben.

:p.Um auch auf unregelmÑ·ig gewarteten Maschinen nicht als Speicherfresser zu
fungieren, wurde ein Mechanismus eingebaut, der es ermîglicht, den
Ausgabeumfang zu begrenzen. Dabei werden mehrere Ausgabedateien verwendet,
zwischen denen nach dem Erreichen einer gewissen Grî·e oder auf Benutzerwunsch
durch den entsprechenden
:link refid=IDME_NEXTFILE reftype=hd.MenÅeintrag:elink.
gewechselt wird.

:p.Der Cron DÑmon Åbernimmt weiterhin alle Aufgaben des Unix-Befehls
:hp2.at:ehp2.. Dieser Befehl startet ebenfalls ein Programm zu einer gewissen
Zeit, jedoch wird die ProgrammausfÅhrung nie wiederholt. Um ein weiteres
Kommunikationsprogramm zu sparen, wurde das SchlÅsselwort :hp2.Once:ehp2.
eingefÅhrt.

:p.Weiterhin beherrscht der DÑmon spezifische Funktionen, die es
erlauben, beim Start und beim Ende des Cron DÑmons Programme zu starten, da
OS/2-Maschinen naturgemÑ· selten lÑnger als ein paar Stunden arbeiten.
Programme, die eigentlich in der Nacht arbeiten sollen, kînnen so nachgefahren
werden. (Datensicherung und -auffrischung, AufrÑumarbeiten, etc.)

:p.Der Cron DÑmon kann auch Åber das Netzwerk bedient werden. Dies geschieht
durch das Programm :hp2.crontab:ehp2.. UnterstÅtzte Netzwerkvarianten sind
:link refid=LAN_NETWORK reftype=hd.LAN/Netware:elink. und
:link refid=IDD_TCP reftype=hd.TCP/IP:elink..
.******************************************************************************
:h1.Farben und Schriftart
:i1.Farben
:i1.Schriftart
:p.Man kann die Farben und die Schriftart des Anzeigefensters verÑndern.
Dazu îffnet man in dem Ordner :hp2.Systemkonfiguration:ehp2. die Farb- oder
Schriftartpalette und zieht das gewÅnschte Element auf das Fenster.
:p.Bei den Farben kann man mit der zweiten Maustaste die Hintergrundfarbe
Ñndern. HÑlt man dabei die [Strg]-Taste fest, Ñndert sich die Vordergrundfarbe.
:p.Die Werte werden gespeichert und bei einem erneuten Programmstart
Åbernommen.
.******************************************************************************
:h1 res=10101 name=IDME_NEXTFILE.NÑchste Ausgabedatei
:i1.NÑchste Ausgabedatei
:p.öber den MenÅeintrag :hp2.NÑchste Ausgabedatei:ehp2. kann man die aktuelle
Ausgabedatei schlie·en und zur nÑchsten Åberwechseln. Diese wird vorher
gelîscht, falls sie schon existiert.

:p.Wird die maximale
:link refid=IDDE_OUTPUT_COUNT reftype=hd.Dateianzahl:elink.
erreicht, wird mit der Datei der Nummer 0 fortgefahren.

:p.Die aktuelle Ausgabedatei wird im Hauptfenster angezeigt.

:p.Der MenÅeintrag ist nicht anwÑhlbar, falls die
:link refid=IDDE_OUTPUT_IGNORE_OUTPUT reftype=hd.Ausgabe ignoriert:elink. wird.

:p.Dieser MenÅpunkt kann nÅtzlich sein, um die aktuelle Ausgabedatei anzusehen
oder zu bearbeiten, da der DÑmon die aktuelle Ausgabedatei permanent im Zugriff
hat. Das Lesen dieser Datei ist zwar zu jedem Zeitpunkt erlaubt (TYPE
funktioniert), aber viele Editoren kommen damit nicht zurecht.
.******************************************************************************
:h1 hide res=10105 name=IDME_HELPKEYS.Hilfe zur Tastatur
:p.Es gibt keine spezifischen "Hotkeys" in diesem Programm. Die allgemeinen
:link reftype=hd database='wphelp.hlp' refid=193.Tastenzuordnungen:elink.
kann man sich z.B. Åber den :hp2.Hauptindex:ehp2. im
:hp2.Informationsordner:ehp2. auf der OberflÑche anzeigen lassen.
.******************************************************************************
:h1 res=10200 name=IDD_NOTEBOOK.Einstellungen
:i1.Einstellungen
:p.Durch das AuswÑhlen des MenÅeintrags :hp2.Einstellungen:ehp2. îffnet man das
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink..
Dort werden alle benutzerdefinierbaren Setzungen fÅr das Programm
eingegeben.
.*=============================================================================
:h2 res=10201 name=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen
:i1.Notizbuch
:p.Es gibt zur Zeit fÅnf verschiedene Notizbuchseiten, die wie gewohnt
bedient werden kînnen. SelbstverstÑndlich kann auch zu jedem Seiteneintrag
die Hilfe kontextsensitiv angefordert werden.
Die fÅnf Seiten beschreiben
:ul.
:li.die allgemeine :link
 reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink.
:li.die einzelnen :link
 reftype=hd refid=IDD_CRONTABS.Crontabs-EintrÑge:elink.
:li.das Daten fÅr die :link
 reftype=hd refid=IDD_OUTPUT.Ausgabe:elink. in eine Datei
:li.die Mîglichkeiten fÅr den :link
 reftype=hd refid=IDD_TCP.TCP/IP-Zugriff:elink. von anderen Rechnern
:li.:link reftype=hd refid=IDD_GENERAL.allgemeine Einstellungen:elink., wie
z.B. die Sprache
:eul.
.*=============================================================================
:h2 res=10300 name=IDD_CRON_FILE.Crontabs-Bearbeitung
:i1.Crontabs-Bearbeitung
:p.Diese Seite aus dem :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink.
erlaubt es, die Abarbeitung der Crontabs-EintrÑge und deren Datei anzugeben.

:p.Durch das AuswÑhlen der :link
reftype=hd refid=IDDE_CRON_ATSTARTUP.CronStart-Abarbeitung:elink. fÅhrt der
DÑmon alle Crontabs-EintrÑge beim DÑmonstart aus, deren Startzeit mit
:hp2.CronStart:ehp2. angegeben wurde. Ist die Abarbeitung nicht ausgewÑhlt,
werden die EintrÑge ignoriert.

:p.Durch das AuswÑhlen der :link
reftype=hd refid=IDDE_CRON_ATEXIT.CronStop-Abarbeitung:elink. fÅhrt der
DÑmon alle Crontabs-EintrÑge beim Ende des DÑmon aus, deren Startzeit mit
:hp2.CronStop:ehp2. angegeben wurde. Ist die Abarbeitung nicht ausgewÑhlt,
werden die EintrÑge ignoriert.

:p.Der Dateiname und Pfad der Crontabs-Datei wird ebenfalls angezeigt; zur
:link reftype=hd refid=IDDE_CRON_FILENAMECHANGE.énderung:elink.
mu· aber eine SchaltflÑche benutzt werden.
.*-----------------------------------------------------------------------------
:h3 res=10301 name=IDDE_CRON_ATSTARTUP.CronStart-Abarbeitung
:i1.CronStart-Abarbeitung
:p.Der DÑmon beherrscht im Unterschied zur Unixversion auch die
ProgrammausfÅhrung beim Start des DÑmon. WÑhrend in einem (grî·eren)
Unixsystem diese Option unsinnig ist, ist sie unter OS/2 eine willkommene
Bereicherung der Mîglichkeiten, da regelmÑ·ig zu startende Programme
nachgezogen werden kînnen. (SelbstverstÑndlich kann man auch die
Standardmîglichkeiten von Startup.CMD oder den Systemstart-Ordner nutzen.)
Es ist auch eine Benutzerabrechnung im Verbund mit
:link reftype=hd refid=IDDE_CRON_ATEXIT.CronStop-Abarbeitung:elink. mîglich,
sofern das Programm (als Referenz) im Systemstart-Ordner plaziert wurde oder
anderweitig beim Hochfahren des OS/2 gestartet wird.

:p.Weitere Hinweise fÅr die Gestaltung der Crontabs-EintrÑge findet man in der
:link reftype=hd refid=XMP_SYNTAX.Syntaxbeschreibung:elink. und bei den
:link reftype=hd refid=XMP_XMP.Beispielen:elink..

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink..
.*-----------------------------------------------------------------------------
:h3 res=10302 name=IDDE_CRON_ATEXIT.CronStop-Abarbeitung
:i1.CronStop-Abarbeitung
:p.Der DÑmon beherrscht im Unterschied zur Unixversion auch die
ProgrammausfÅhrung beim Ende des DÑmon. Diese Mîglichkeit ist mit Bedacht
zu benutzen, da der DÑmon sich kurz nach der Aufforderung zum Beenden wirklich
beendet. Es empfiehlt sich, die Programme mittels dem OS/2-Befehl
:hp2.start:ehp2. aufzurufen, da der DÑmon nur eine kurze Verzîgerung akzeptiert
(ca. 1 Sekunde), um dann zu sterben.
In dieser Zeit lassen sich jedoch noch sinnvolle Arbeiten fÅr den
Systemabschlu· starten, die man hÑufiger mal vergi·t. Dazu kînnen sowohl
Benutzerabrechnungen als auch externe, kompliziertere Programmbeendigungen
z.B. im Netzwerk zÑhlen.

:p.Weitere Hinweise fÅr die Gestaltung der Crontabs-EintrÑge findet man in der
:link reftype=hd refid=XMP_SYNTAX.Syntaxbeschreibung:elink. und bei den
:link reftype=hd refid=XMP_XMP.Beispielen:elink..

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink..
.*-----------------------------------------------------------------------------
:h3 res=10303 name=IDDE_CRON_FILENAME.Dateiname
:i1.Dateiname (Crontabs)
:p.Der Dateiname fÅr die Crontabs-Datei lÑ·t sich nur durch das AnwÑhlen der
SchaltflÑche
:link reftype=hd refid=IDDE_CRON_FILENAMECHANGE.Dateiname Ñndern:elink.
verÑndern. Es wird der aktuelle Pfad und Dateiname der Crontabs-Datei
angezeigt. Nach dem ersten Aufruf des Programms wird standardgemÑ· die
Datei :hp2.Crontabs:ehp2. aus dem gleichen Verzeichnis gewÑhlt, wo auch der
Cron DÑmon steht. Treten dabei Fehler auf (Schreibschutz, Verzeichnis namens
:hp2.Crontabs:ehp2. existiert, etc.), so wird die Datei :hp2.nul:ehp2. gewÑhlt.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink..
.*-----------------------------------------------------------------------------
:h3 res=10304 name=IDDE_CRON_FILENAMECHANGE.Dateiname Ñndern
:i1.éndern des Dateinamens (Crontabs)
:p.Mit dieser SchaltflÑche lÑ·t sich der Dateiname und der Pfad der
Crontabs-Datei verÑndern. Dabei wird ein Standardfenster des Systems geîffnet,
das auch von anderen Programmen genutzt wird, um Dateien einzulesen.

:p.Falls die angegebene Datei noch nicht existiert, so wird sie angelegt.
Es mu· ein normaler Dateiname angegeben werden. Die einzige Ausnahme ist die
Datei :hp2.nul:ehp2., die als "Notdatei" fÅr Fehler fungiert. Falls die
Datei bereits existiert, so wird nach der BestÑtigung der Inhalt ÅberprÅft.
Dabei mu· fÅr jede Zeile die
:link reftype=hd refid=XMP_SYNTAX.Syntax:elink. eingehalten werden. Nur nach
dem erfolgreichen Einlesen der Datei wird die ursprÅngliche Datei geschlossen
und die neue verwendet.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink..
.*=============================================================================
:h2 res=10310 name=IDD_CRONTABS.Crontabs-EintrÑge
:i1.Crontabs-EintrÑge
:i1.EintrÑge
:p.Diese Seite aus dem :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink.
erlaubt es, die einzelnen EintrÑge aus der Crontabs-Datei zu verÑndern und
anzusehen.

:p.Durch das AuswÑhlen von :link
reftype=hd refid=IDDE_CRONTABS_ADD.HinzufÅgen:elink. wird der im
Feld :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.Neuer Befehl:elink. eingesetzte Befehl
an die bestehende Datei angehÑngt und zur gegebenen Zeit ausgefÅhrt. Der Befehl
mu· die
:link reftype=hd refid=XMP_SYNTAX.Syntax:elink. eingehalten.

:p.Das AuswÑhlen von :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.éndern:elink. entspricht einem Lîschen
und gleichzeitigem HinzufÅgen des zu bearbeitenden Eintrags.

:p.Durch das AuswÑhlen von :link
reftype=hd refid=IDDE_CRONTABS_ABORT.Abbruch:elink. wird ein Bearbeitungswunsch
eines Eintrags widerrufen.

:p.Durch das AuswÑhlen von :link
reftype=hd refid=IDDE_CRONTABS_DELETE.Lîschen:elink. werden alle
markierten Elemente der :link
reftype=hd refid=IDDE_CRONTABS_LIST.Eintragsliste:elink. gelîscht.

:p.Durch das AuswÑhlen von :link
reftype=hd refid=IDDE_CRONTABS_EDIT.Bearbeiten:elink. wird das markierte
Element der :link reftype=hd refid=IDDE_CRONTABS_LIST.Eintragsliste:elink.
in das Editierfeld kopiert. Nach erfolgter Bearbeitung kann man durch das
DrÅcken von :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.éndern:elink. das Element ersetzen.

:p.Alle Druckknîpfe sind nur dann ausfÅhrbar, wenn eine gÅltige Operation
damit mîglich ist.
.*-----------------------------------------------------------------------------
:h3 res=10311 name=IDDE_CRONTABS_LIST.Eintragsliste
:i1.Eintragsliste
:p.In dieser Liste sind alle vorhandenen EintrÑge der Crontabs-Datei
aufgefÅhrt. Man kann einzelne Elemente mit der linken Maustaste oder der
Leertaste auswÑhlen und die dann Åber die SchaltflÑche :link
reftype=hd refid=IDDE_CRONTABS_DELETE.Lîschen:elink. entfernen.
:hp2.Vorsicht, es gibt keine Funktion zum ZurÅcknehmen des Lîschvorgangs.:ehp2.

:p.Alternativ kann man auch einen Eintrag :link
reftype=hd refid=IDDE_CRONTABS_EDIT.bearbeiten:elink..

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-EintrÑge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10312 name=IDDE_CRONTABS_ENTRY.Crontabs-Eintrag
:i1.Crontabs-Eintrag (Neu)
:p.In diesem Feld kann ein neuer Crontabs-Befehl eingesetzt werden. Er mu· die
:link reftype=hd refid=XMP_SYNTAX.Syntax:elink.konventionen einhalten. Nach dem
Editieren drÅckt man die SchaltflÑche :link
reftype=hd refid=IDDE_CRONTABS_ADD.HinzufÅgen:elink., um den Eintrag
in die Crontabs-Datei zu Åbertragen. Treten keine Fehler auf, wird das Feld
gelîscht und der Eintrag an das Ende der :link
reftype=hd refid=IDDE_CRONTABS_LIST.Eintragsliste:elink. gesetzt. Bei einem
Fehler verbleibt die Eingabe im Feld und es wird eine Fehlermeldung angezeigt.

:p.Wurde das Eintragsfeld durch das :link
reftype=hd refid=IDDE_CRONTABS_EDIT.Bearbeiten:elink. eines vorhandenen
Eintrags gefÅllt, kann man den alten Eintrag mit :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.éndern:elink. ersetzen oder mit :link
reftype=hd refid=IDDE_CRONTABS_ABORT.Abbruch:elink. den énderungswunsch
zurÅckweisen.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-EintrÑge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10313 name=IDDE_CRONTABS_ADD.Eintrag hinzufÅgen
:i1.Crontabs-Eintrag hinzufÅgen
:p.Durch das Benutzen dieser SchaltflÑche wird der im Feld :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.Neuer Befehl:elink. eingegebene Eintrag
ÅberprÅft und gegebenenfalls in die Liste der vorhandenen EintrÑge Åbernommen.

:p.Der Eintrag mu· die
:link reftype=hd refid=XMP_SYNTAX.Syntax:elink.konventionen einhalten.
Bei einem Fehler verbleibt die Eingabe im Feld und es wird eine Fehlermeldung
angezeigt.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-EintrÑge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10316 name=IDDE_CRONTABS_CHANGE.Eintrag Ñndern
:i1.Crontabs-Eintrag Ñndern
:p.Durch das Benutzen dieser SchaltflÑche wird der im Feld :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.Neuer Befehl:elink. eingegebene Eintrag
ÅberprÅft und gegebenenfalls in die Liste der vorhandenen EintrÑge Åbernommen.
Er ersetzt dabei den Eintrag, aus dem der Inhalt des Eintragsfeld entstanden
ist.

:p.Der Eintrag mu· die
:link reftype=hd refid=XMP_SYNTAX.Syntax:elink.konventionen einhalten.
Bei einem Fehler verbleibt die Eingabe im Feld und es wird eine Fehlermeldung
angezeigt.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-EintrÑge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10317 name=IDDE_CRONTABS_ABORT.Eintragsbearbeitung abbrechen
:i1.Crontabs-EintragsÑnderung abbrechen
:p.Durch das Benutzen dieser SchaltflÑche wird die Bearbeitung eines
existierenden Eintrags abgebrochen. Das Eintragsfeld wird dabei gelîscht.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-EintrÑge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10314 name=IDDE_CRONTABS_DELETE.EintrÑge lîschen
:i1.Crontabs-EintrÑge lîschen
:p.Alle markierten EintrÑge aus der
:link reftype=hd refid=IDDE_CRONTABS_LIST.Eintragsliste:elink. werden
unwiderruflich gelîscht.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-EintrÑge:elink..
.*-----------------------------------------------------------------------------
:h3 res=10315 name=IDDE_CRONTABS_EDIT.Eintrag bearbeiten
:i1.Crontabs-Eintrag bearbeiten
:p.Der markierte Eintrag aus der
:link reftype=hd refid=IDDE_CRONTABS_LIST.Eintragsliste:elink. wird zur
VerÑnderung in das Feld :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.Neuer Befehl:elink. kopiert. Der Eintrag
verbleibt aus SicherheitsgrÅnden in der Liste und wird durch das DrÅcken
von :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.éndern:elink. ersetzt. Mit :link
reftype=hd refid=IDDE_CRONTABS_ABORT.Abbruch:elink. kann man den
énderungswunsch zurÅckweisen.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_CRONTABS.Crontabs-EintrÑge:elink..
.*=============================================================================
:h2 res=10400 name=IDD_OUTPUT.Ausgabe
:i1.Ausgabe
:p.Diese Seite aus dem :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink.
erlaubt es, den Ausgabepfad und die Eigenschaften der Ausgabe vom DÑmon und
den aufgerufenen Programmen festzulegen.

:p.Durch das AuswÑhlen von :link
reftype=hd refid=IDDE_OUTPUT_COUNT.Anzahl der Ausgabedateien:elink. und :link
reftype=hd refid=IDDE_OUTPUT_THRES.Dateigrî·e-Schwellenwert:elink. legt man
den maximal benutzten Plattenplatz fest.

:p.Der Pfad der Ausgabedateien wird ebenfalls angezeigt; zur
:link reftype=hd refid=IDDE_OUTPUT_PATHCHANGE.PfadÑnderung:elink.
mu· aber eine SchaltflÑche benutzt werden.

:p.Es besteht die Mîglichkeit, alle :link
reftype=hd refid=IDDE_OUTPUT_IGNORE_OUTPUT.Ausgaben zu ignorieren:elink..

:p.Die Unixversion von Cron schickt jedem Benutzer zu jedem Auftrag dessen
Ausgabe und den Programmstatus per Post zu. Da OS/2 weder ein standardisiertes
Postsystem, noch MultiuserfÑhigkeit besitzt, schien es geschickter, alle
Ausgaben und Meldungen in eine Datei zu lenken. Bei unregelmÑ·iger Wartung
wÅrde aber nach und nach die PlattenkapazitÑt Åberschritten. Deshalb wird
immer dann auf eine neue Datei gewechselt, wenn der Schwellenwert Åberschritten
wurde. Alle Ausgabedateien haben den Dateinamen :hp2.Cron000?.out:ehp2..
Dabei durchlÑuft das Fragezeichen alle Ziffern von 0 bis zur gegebenen Anzahl
der Ausgabedateien.

:p.Die aktuelle Ausgabedatei ist im permanenten Zugriff, sofern die Ausgabe
nicht komplett ignoriert wird. Nicht alle Programme kommen mit diesem Zugriff
zurecht, um die Datei anzuzeigen. (Der Befehl TYPE funktioniert aber.)
.br
Deshalb kann man im MenÅ die :link
reftype=hd refid=IDME_NEXTFILE.nÑchste Ausgabedatei:elink. auswÑhlen, um die
aktuelle Ausgabedatei freizugeben. Diese kann dann editiert oder angesehen
werden.
.*-----------------------------------------------------------------------------
:h3 res=10401 name=IDDE_OUTPUT_THRES.Schwellenwert
:i1.Schwellenwert
:p.Die aktuelle Ausgabedatei wird nach dem Erreichen des Schwellenwertes
gewechselt. Es kînnen nur vorgewÑhlte Schwellenwerte benutzt werden.

:p.Der Zusammenhang mit den anderen Werten ist in der Hilfe zur
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink. beschrieben.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
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
Seite fÅr die
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink..
.*-----------------------------------------------------------------------------
:h3 res=10403 name=IDDE_OUTPUT_PATH.Pfad
:i1.Ausgabepfad
:p.Dieses Feld zeigt den Pfad fÅr die Ausgabedateien. Der voreingestellte Wert
ist der Pfad, von dem der DÑmon gestartet wurde, also nicht das jeweils
aktuelle Verzeichnis.

:p.Zur énderung mu· die SchaltflÑche
:link reftype=hd refid=IDDE_OUTPUT_PATHCHANGE.Pfad Ñndern:elink. benutzt
werden. Dann wird die zugehîrige
:link reftype=hd refid=IDD_OUTPUT_PATH.Auswahlbox:elink. geîffnet.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink..
.*-----------------------------------------------------------------------------
:h3 res=10404 name=IDDE_OUTPUT_PATHCHANGE.Pfad Ñndern
:i1.Ausgabepfad Ñndern (SchaltflÑche)
:p.Mit dieser SchaltflÑche îffnet sich eine
:link reftype=hd refid=IDD_OUTPUT_PATH.Auswahlbox:elink., mit der der Pfad
fÅr die Ausgabedateien verÑndert werden kann. Nach einer erfolgreichen
VerÑnderung wird der Pfad im
:link reftype=hd refid=IDDE_OUTPUT_PATH.Anzeigefeld:elink. aufgefrischt.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink..
.*-----------------------------------------------------------------------------
:h3 res=10405 name=IDDE_OUTPUT_IGNORE_OUTPUT.Ausgabe ignorieren
:i1.Ausgabe ignorieren
:p.Es ist mîglich, alle Ausgaben sowohl des DÑmons, als auch der aufgerufenen
Programme zu ignorieren, sie werden dann nicht in eine Datei geschrieben.

:p.Diese Einstellung ist die Voreinstellung beim ersten Programmstart. Es wird
empfohlen, die Einstellungen auf dieser Seite den lokalen BedÅrfnissen
anzupassen und die Ausgabe dann freizugeben, um Fehler bei den Programmen zu
erkennen.

:p.Falls diese Auswahl gesetzt ist, ist die Wahl der :link
reftype=hd refid=IDME_NEXTFILE.nÑchsten Ausgabedatei:elink. im HauptmenÅ nicht
mîglich.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink..
.*-----------------------------------------------------------------------------
:h3 toc=4 res=10410 name=IDD_OUTPUT_PATH.Ausgabepfad Ñndern
:i1.Ausgabepfad Ñndern (Dialog)
:p.Dieses Dialogfenster îffnet sich durch das Bedienen der SchaltflÑche zum
:link reftype=hd refid=IDDE_OUTPUT_PATHCHANGE.Pfad Ñndern:elink..

:p.Der aktuelle Pfad der Ausgabedateien wird im :link
reftype=hd refid=IDDE_OUTPUT_PATH_ENTRY.Ausgabepfad-Eingabefeld:elink.
angezeigt. Bei jedem doppelten Mausklick in die :link
reftype=hd refid=IDDE_OUTPUT_PATH_LIST.Ausgabepfad-Liste:elink. wird der
entstehende Pfad neu angezeigt.

:p.SelbstverstÑndlich kann man den neuen Pfad auch per Hand eingeben.

:p.Diese Dialogbox wird von der Seite
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink. im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink.
aufgerufen.
.*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
:h4 res=10411 name=IDDE_OUTPUT_PATH_ENTRY.Ausgabepfad-Eingabefeld
:i1.Ausgabepfad-Eingabefeld
:p.Das Eingabefeld fÅr die Ausgabe wird entweder durch eine Benutzereingabe
oder durch das Bedienen der
:link reftype=hd refid=IDDE_OUTPUT_PATH_LIST.Ausgabepfad-Liste:elink.
verÑndert. Initial wird der Wert auf den aktuellen Ausgabepfad gesetzt.

:p.Nach dem DrÅcken der BestÑtigungstaste wird ÅberprÅft, ob der Pfad
tauglich ist. Dazu mu· er existieren und die Mîglichkeit bestehen, zehn
Dateien der Form :hp2.Cron000?.out:ehp2. zu erzeugen oder zu verÑndern.
Das Fragezeichen durchlÑuft dabei alle Dezimalziffern.
.br
Bei einem Fehler wird ein Mitteilungsfenster geîffnet und auf den Umstand
hingewiesen. Man mu· dann einen anderen Pfad wÑhlen, abbrechen oder mit
einem anderen Programm den Fehler beheben (z.B. den Pfad anlegen).

:p.Dieser Eintrag befindet sich in der Dialogbox zur
:link reftype=hd refid=IDD_OUTPUT_PATH.énderung des Pfades:elink., die von
der Seite
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink. im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink.
aufgerufen wird.
.*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
:h4 res=10412 name=IDDE_OUTPUT_PATH_LIST.Ausgabepfad-Liste
:i1.Ausgabepfad-Liste
:p.Mit der Liste fÅr Ausgabepfade hat man eine komfortable Mîglichkeit,
den aktuellen Ausgabepfad zu verÑndern.

:p.In der Liste sind die Laufwerksbuchstaben (ohne Floppies) und alle
Unterverzeichnisse des aktuellen Pfades vertreten. Durch ein doppeltes Klicken
der linken Maustaste auf eine dieser Komponenten wird sie ausgewÑhlt und an
den aktuellen Pfad angehÑngt, bzw. der aktuelle Pfad ersetzt.

:p.Das
:link reftype=hd refid=IDDE_OUTPUT_PATH_LIST.Ausgabepfad-Eingabefeld:elink.
pa·t seinen Wert automatisch an. Durch das Bedienen der Eingabetaste wird die
aktuelle Auswahl Åbernommen und der Wert des Eingabefeldes getestet.

:p.Dieser Eintrag befindet sich in der Dialogbox zur
:link reftype=hd refid=IDD_OUTPUT_PATH.énderung des Pfades:elink., die von
der Seite
:link reftype=hd refid=IDD_OUTPUT.Ausgabe:elink. im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink.
aufgerufen wird.
.*=============================================================================
:h2 toc=3 res=10500 name=IDD_TCP.TCP/IP
:i1.TCP/IP
:p.Diese Seite aus dem :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink.
erlaubt es, die Mîglichkeiten einer Bedienung durch TCP/IP-vernetzte
Fremdmaschinen festzulegen.

TCP/IP kennt keinen eigenen Zugriffsschutz. Es ist dem System nicht mîglich,
einzelne Kommunikationen zu verbieten. Um nicht eine riesige SicherheitslÅcke
aufzutun, erlaubt der Cron DÑmon nur die Kommunikation mit bestimmten
Partnerrechnern Åber TCP/IP. Die Namen der Partnerrechner stehen in einer
Datei, pro Rechner eine Zeile. Jeder Rechnername darf in der Nummernnotation
(z.B. 128.176.248.76) oder in der allgemeinen Namenskonvention (z.B.
Sauron@uni-muenster.de) angegeben werden.

:p.Der Dateiname und Pfad der Erlaubnisdatei wird in einem Feld angezeigt; zur
:link reftype=hd refid=IDDE_TCP_FILENAMECHANGE.énderung:elink.
mu· aber eine SchaltflÑche benutzt werden.

:p.Erst durch das AuswÑhlen von :link
reftype=hd refid=IDDE_TCP_ALLOWACCESS.Zugriff erlauben:elink. wird ein
prinzipieller Zugriff von Fremdrechnern mîglich.
.br
Falls auf dem aktuellen Rechner kein TCP/IP installiert ist, ist diese
SchaltflÑche nicht anwÑhlbar.

:p.Die Bearbeitung der Crontabs-Datei aus der Ferne erfolgt durch das
Hilfsprogramm :hp2.crontab:ehp2..

:p.Die Portnummer des Cron DÑmons ist 64020.
.*-----------------------------------------------------------------------------
:h3 res=10501 name=IDDE_TCP_FILENAME.Dateiname
:i1.Dateiname (TCP/IP-Zugriffserlaubnisse)
:p.Der Dateiname fÅr die TCP/IP-Zugriffserlaubnisse lÑ·t sich nur durch das
AnwÑhlen der SchaltflÑche
:link reftype=hd refid=IDDE_TCP_FILENAMECHANGE.Dateiname Ñndern:elink.
verÑndern. Es wird der aktuelle Pfad und Dateiname der Erlaubnisdatei-Datei
angezeigt. Nach dem ersten Aufruf des Programms wird standardgemÑ· die
Datei :hp2.nul:ehp2. benutzt.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_TCP.TCP/IP-Zugriffe:elink..
.*-----------------------------------------------------------------------------
:h3 res=10502 name=IDDE_TCP_FILENAMECHANGE.Dateiname Ñndern
:i1.éndern des Dateinamens (TCP/IP-Zugriffserlaubnisse)
:p.Mit dieser SchaltflÑche lÑ·t sich der Dateiname und der Pfad der
TCP/IP-Zugriffserlaubnis-Datei verÑndern. Dabei wird ein Standardfenster des
Systems geîffnet, das auch von anderen Programmen genutzt wird, um Dateien
einzulesen.

:p.Falls die angegebene Datei noch nicht existiert, so wird sie angelegt.
Es mu· ein normaler Dateiname angegeben werden. Die einzige Ausnahme ist die
Datei :hp2.nul:ehp2., die als "Notdatei" fÅr Fehler fungiert. Falls die
Datei bereits existiert, so wird nach der BestÑtigung der Inhalt ÅberprÅft.
Dabei mu· in jeder Zeile ein Rechnername in IP-Notation stehen. Leerzeilen und
Zeilen, die mit einem Doppelkreuz (:hp2.#:ehp2.) oder Semikolon (:hp2.;:ehp2.)
beginnen, werden als Kommentarzeilen angesehen und ignoriert.
Nur nach dem erfolgreichen Einlesen der Datei wird die ursprÅngliche Datei
geschlossen und die neue verwendet.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_TCP.TCP/IP-Zugriffe:elink..
.*-----------------------------------------------------------------------------
:h3 res=10503 name=IDDE_TCP_ALLOWACCESS.Zugriff erlauben
:i1.TCP/IP-Zugriff erlauben
:i1.Zugriff erlauben (TCP/IP)
:p.Diese SchaltflÑche regelt den allgemeinen TCP/IP-Zugang von Fremdrechner.
Ungeachtet der :link
reftype=hd refid=IDDE_TCP_FILENAME.Zugriffserlaubnisdatei:elink., die die
Fremdrechner auflistet, steht hier ein Mittel zur VerfÅgung, um generell den
Zugriff zu verbieten.

:p.Diese SchaltflÑche ist nicht anwÑhlbar, wenn der Rechner nicht mit TCP/IP
ausgestattet ist.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_TCP.TCP/IP-Zugriffe:elink..
.*=============================================================================
:h2 res=10600 name=IDD_GENERAL.Allgemeine Einstellungen
:i1.Allgemeine Einstellungen
:p.Diese Seite aus dem :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Notizbuch mit den Einstellungen:elink.
erlaubt es, allgemeine Einstellungen des Programms zu verÑndern.

Zur Zeit wird auf dieser Seite nur die bevorzugte Sprache ausgewÑhlt.
StandardgemÑ· steht diese beim ersten Programmstart auf "English", sofern
nicht die Umgebungsvariable :hp2.LANG:ehp2. auf etwas anderes verweist.
UnterstÅtzte :hp2.LANG:ehp2.-Werte sind&colon.
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
:dd.Franáais
:edl.
:p.GeÑndert werden die Sprache durch das Bedienen der :link
reftype=hd refid=IDDE_GENERAL_LANGUAGE.Sprachauswahlliste:elink..
.*-----------------------------------------------------------------------------
:h3 res=10601 name=IDDE_GENERAL_LANGUAGE.Bevorzugte Sprache
:i1.Sprache
:p.Die bevorzugte Sprache lÑ·t sich durch das Bedienen dieser Liste auswÑhlen.
:link refid=NeueSprachen reftype=fn.Es steht nur eine begrenzte Auswahl zur
VerfÅgung.:elink.

:p.StandardgemÑ· steht die Sprache beim ersten Programmstart auf "English",
sofern nicht die Umgebungsvariable :hp2.LANG:ehp2. auf etwas anderes verweist.
In der Hilfe zu den
:link reftype=hd refid=IDD_GENERAL.allgemeinen Einstellungen:elink. stehen dazu
weitere Informationen.


Mîgliche Sprachen sind zur Zeit&colon.
:ul.
:li.Deutsch
:li.Englisch
:li.Franzîsisch
:eul.

:p.Eine énderung wird erst nach einem Neustart des Programms wirksam.

:p.Dieser Eintrag befindet sich im
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Einstellungsnotizbuch:elink. auf der
Seite fÅr die
:link reftype=hd refid=IDD_GENERAL.allgemeinen Einstellungen:elink..
:fn id=NeueSprachen.Das mag sich Ñndern, falls ein paar Leute diesem Kram
Åbersetzen!:efn.
.******************************************************************************
:h1 name=XMP_GENERAL.Beispiele und Syntax von Crontabs-EintrÑgen
:p.In diesem Kapitel wird die :link reftype=hd refid=XMP_SYNTAX.Syntax:elink.
der Crontabs-EintrÑge beschrieben und die Benutzung anhand von
:link reftype=hd refid=XMP_XMP.Beispielen:elink. demonstriert.

:p.Im Gegensatz zur Syntax des Programms :hp2.crontabs:ehp2. und zu mîglichen
Eingaben von :link
reftype=hd refid=IDD_CRONTABS.Crontabs-EintrÑge:elink. innerhalb dieses
Programms dÅrfen in der Datei auch Kommentare auftauchen.

:p.Kommentarzeilen sind leere Zeilen oder Zeilen, die in der ersten Spalte ein
Semikolon (:hp2.;:ehp2.) oder Doppelkreuz (:hp2.#:ehp2.) stehen haben.
.*=============================================================================
:h2 name=XMP_SYNTAX.Syntax der Crontabs-EintrÑgen
:i1.Syntax der Crontabs-EintrÑgen
:p.Die Syntax von Crontabs-EintrÑgen ist recht einfach. Jeder Eintrag mu· in
einer Zeile stehen. Es wird nicht zwischen Gro·- und Kleinschreibung
unterschieden. Jede Zeile hat die Form
:xmp.[Flaggen] Zeiten Kommando:exmp.
:p.Die :hp2.Flaggen:ehp2. sind optional. Mîgliche Flaggen sind zur Zeit&colon.
:dl compact.
:dthd.:hp5.Flagge:ehp5.
:ddhd.:hp5.Bedeutung:ehp5.
:dt.:hp2.Once:ehp2.
:dd.Das Kommando wird zur gegebenen Zeit einmal ausgefÅhrt und danach aus der
Kommandoliste gelîscht.
:edl.

:p.Die :hp2.Zeiten:ehp2. haben in der Regel fÅnf EintrÑge, die :hp2.alle:ehp2.
zutreffen mÅssen, damit das Kommando ausgefÅhrt wird&colon.
:ol.
:li.:hp2.Minuten:ehp2.
:p.Diese :link refid=Zahlenangabe reftype=fn.Zahlenangabe:elink.
beschreibt die Minute innerhalb der Stunde, in der das
Kommando gestartet werden soll. Mîgliche Werte liegen zwischen 0 und 59.
:li.:hp2.Stunden:ehp2.
:lp.Diese :link refid=Zahlenangabe reftype=fn.Zahlenangabe:elink.
beschreibt die Stunde, zu der das
Kommando gestartet werden soll. Mîgliche Werte liegen zwischen 0 und 23.
:li.:hp2.Monatstag:ehp2.
:lp.Diese :link refid=Zahlenangabe reftype=fn.Zahlenangabe:elink.
beschreibt den Monatstag, an dem das
Kommando gestartet werden soll. Mîgliche Werte liegen zwischen 1 und 31, wobei
das Maximum nicht vom jeweiligen Monat abhÑngt. Im Februar werden also
Kommandos mit dem Monatstag 31 nicht ausgefÅhrt, obwohl die Angabe legal ist.
:li.:hp2.Monat:ehp2.
:lp.Diese :link refid=Zahlenangabe reftype=fn.Zahlenangabe:elink.
beschreibt den Monat, in dem das
Kommando gestartet werden soll. Mîgliche Werte liegen zwischen 1 und 12.
:li.:hp2.Wochentag:ehp2.
:lp.Diese :link refid=Zahlenangabe reftype=fn.Zahlenangabe:elink.
beschreibt den Wochentag, an dem das
Kommando gestartet werden soll. Mîgliche Werte liegen zwischen 0 und 6. Der
Wert 0 beschreibt den Sonntag.
:eol.

:p.Es gibt noch drei Spezialzeiten, die jeweils alle 5 Angaben ersetzen&colon.
:dl compact tsize=15.
:dthd.:hp5.Sonderzeit:ehp5.
:ddhd.:hp5.Bedeutung:ehp5.
:dt.:hp2.Cronstart:ehp2.
:dd.Das Kommando wird beim Start des DÑmon ausgefÅhrt, sofern die Freigabe in
der :link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink. erfolgt
ist.
:dt.:hp2.Cronstop:ehp2.
:dd.Das Kommando wird beim Beenden des DÑmon ausgefÅhrt, sofern die Freigabe in
der :link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink. erfolgt
ist.
.br
:hp1.VORSICHT&colon. Es verbleibt nur eine sehr kurze Verzîgerungsfrist,
bevor der DÑmon sich dann beendet. Im Falle eines Systemabschlusses kann die
verbliebene Zeit bis zum Booten ggf. nicht ausreichen, um komplizierte
Befehlsfolgen auszufÅhren. Eventuell wird in spÑteren Versionen eine
benutzerdefinierbare maximale Verzîgerungszeit angeboten, dazu sollten aber
BedarfswÅnsche geÑu·ert werden.:ehp1.
:dt.:hp2.Daily:ehp2.
:dd.Das Kommando wird einmal tÑglich bei der ersten Mîglichkeit ausgefÅhrt.
Man beachte, da· das Kommando :hp2.nicht:ehp2. beim HinzufÅgen zur Liste
ausgefÅhrt wird, sondern erst ab dem nÑchsten Tag gestartet wird, falls der
DÑmon am aktuellen Tag bereits gelaufen hat oder lÑuft.
:edl.

:p.Als :hp2.Kommando:ehp2. ist jedes beliebige OS/2-Kommando erlaubt. Zur Zeit
gibt es noch keine genauere Spezifizierung des Programmtyps (das Ñndert sich!),
so da· man bei DOS-Programmen am besten den OS/2-Befehl :hp2.start:ehp2. zur
Hilfe nimmt. Die Standardausgaben des Programms werden in einer temporÑren
Datei gesammelt und beim Beenden des Programms in die :link
reftype=hd refid=IDD_OUTPUT.Ausgabe:elink.datei kopiert. Da bei der Benutzung
von :hp2.start:ehp2. ein weiterer, unabhÑngiger Proze· gestartet wird, der
sich der Kontrolle des DÑmons entzieht, sollte die Ausgabe der Programme
gelîscht oder umgelenkt werden. Dazu gibt man am Ende der Zeile
:xmp. >Datei 2>&amp.1:exmp.
:p.an. Als Dateiname kann man die Datei :hp2.nul:ehp2. angeben, um die Ausgabe
zu ignorieren. Da PM-Programme normalerweise keine Ausgabe haben, sollte dies
bei solchen Programmen immer angegeben werden.

:p.Die temporÑren Dateien werden im gleichen Unterverzeichnis erzeugt, wo auch
das Programm des Cron DÑmon steht. Es empfiehlt sich, "Leichen" durch die
beiden Crontabs-Befehle
:xmp.
Cronstart del ?&colon.\Pfad\*.tmp >nul 2>&amp.1
0 3 * * 0 del ?&colon.\Pfad\*.tmp >nul 2>&amp.1
:exmp.
:p.regelmÑ·ig zu entfernen. Das Fragezeichen und Pfad sollten durch die
entsprechenden Werte ersetzt werden.

:fn id=Zahlenangabe.Eine Zahlenangabe ist eine durch Kommata getrennte Liste
von Zahlen oder Zahlintervallen, die das Ereignis treffen mÅssen.
Ein Stern (:hp2.*:ehp2.) bedeutet&colon.
jede mîgliche Zahl. Beispiele&colon.
:xmp.1,2,5:exmp.
:p.oder
:xmp.8-12,14-18:exmp.
:p.oder
:xmp.*:exmp.:efn.
.*=============================================================================
:h2 name=XMP_XMP.Beispiele fÅr Crontabs-EintrÑge
:i1.Beispiele
:p.Es gibt eine Vielzahl von Einsatzmîglichkeiten des Cron DÑmon, von denen
einige anhand von Beispielen vorgestellt werden sollen. Dabei werden als
Kommandos auch Programmnamen benutzt, die garantiert nicht auf jedem System
verfÅgbar sind, aber durch ihre sprechenden Namen den Einsatz aufzeigen.

:ul.
:li.:font facename=Courier size=18x10.* * * * * echo Eine Minute ist um.:font
facename=default size=0x0.
:p.Jede Minute in jeder Stunde an jedem beliebigen Tag wird die Zeile
:hp2.Eine Minute ist um.:ehp2. ausgegeben. Diese Ausgabe des Kommandos echo
wird in die :link
reftype=hd refid=IDD_OUTPUT.Ausgabe:elink.datei des Cron DÑmons kopiert.
:li.:font facename=Courier size=18x10.0 * * * * echo Eine Stunde ist um. >nul 2>&amp.1:font
facename=default size=0x0.
:p.Zu jeder vollen Stunde an jedem beliebigen Tag wird die Zeile
:hp2.Eine Stunde ist um.:ehp2. ausgegeben. Da die Ausgabe aber verworfen wird,
ist dieser Befehl unsinnig (echo ist ja nur fÅr Ausgaben zustÑndig). Immerhin
erscheint noch in der Ausgabedatei des DÑmons die Mitteilung, da· das Programm
gestartet wurde, sofern die Ausgabefunktionen prinzipiell benutzt werden.
:li.:font facename=Courier size=18x10.0,30 * * * * DeliverMail:font
facename=default size=0x0.
:p.Zu jeder vollen und halben Stunde wird das Programm DeliverMail gestartet.
Alle Ausgaben des Programms erscheinen auch in der Ausgabedatei des DÑmons.
:li.:font facename=Courier size=18x10.0,30 * * * * start PMDeliverMail >nul 2>&amp.1:font
facename=default size=0x0.
:p.Zu jeder vollen und halben Stunde wird das Programm PMDeliverMail in einer
separaten Sitzung gestartet (nîtig bei PM-Programmen!). Alle Ausgaben werden
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
:p.Die Maschine wird einmalig (wichtig!) zum nÑchstmîglichen Zeitpunkt neu
gestartet. Alle Ausgaben des Programms werden ignoriert. Vorsicht bei einer
Nachahmung, :hp2.setboot /b:ehp2. entspricht einem DrÅcken von Alt-Strg-Entf.
Dieser Befehl kann z.B. nÅtzlich sein, um einem Stromausfall zuvorzukommen.
(Na gut, das wei· man nicht immer vorher, aber wenn der/die Kollege/in gerade
an Kabeln von ungeschÅtzten Stromkreisen rumfummelt, kann dies noch die letzte
Rettung fÅr das Plattensystem eines Servers drei RÑume weiter sein. Alles
schon erlebt...)
:li.:font facename=Courier size=18x10.0 3 * * * deltree f&colon.\tmp &amp. mkdir f&colon.\tmp &amp. echo Das Verzeichnis wird morgens um drei gelîscht! > f&colon.\tmp\Warnung.!!!:font
facename=default size=0x0.
:p.Um drei Uhr morgens wird zuerst der Befehl deltree mit dem Parameter
f&colon.\tmp aufgerufen, dann das Verzeichnis neu erzeugt und eine
Warnungsdatei in diesem Verzeichnis generiert. Alles geschieht Åber einen
einzigen Befehlsinterpreter, so da· die Ausgaben der Programme schîn
hintereinander in der Ausgabedatei des DÑmon stehen.
:li.:font facename=Courier size=18x10.Cronstart del c&colon.\cron\*.tmp >nul 2>&amp.1:font
facename=default size=0x0.
:p.Bei jedem Start des DÑmons werden alle temporÑren Dateien aus dem
Cron-Verzeichnis auf Laufwerk C gelîscht. Alle Ausgaben werden ignoriert.

:p.Dieser Befehl wird nur dann ausgefÅhrt, wenn auch die Freigabe in
der :link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink. erfolgt
ist.

:p.Dieser Befehl ist recht nÅtzlich, da erfahrungsgemÑ· immer ein paar
temporÑre Dateien vom DÑmon nicht gelîscht werden kînnen. Das ist kein Fehler
des DÑmons, sondern meistens eine fehlerhafte Crontabs-Zeile mit
:hp2.start:ehp2. aber ohne Ausgabeumlenkung. (Die temporÑre Ausgabedatei ist
dann fÅr den DÑmon blockiert.)
:li.:font facename=Courier size=18x10.Cronstop echo Abgemeldet&colon. >> \Zeiten\florian &amp. time >> \Zeiten\florian:font
facename=default size=0x0.
:p.Beim jedem Beenden des DÑmons werden wird eine Meldung und die aktuelle
Uhrzeit an eine Datei angehÑngt. Da der Befehl :hp2.time:ehp2. von der
Standardeingabe liest, diese aber vom DÑmon standardgemÑ· auf :hp2.nul:ehp2.
gelegt wurde, entfÑllt eine Umstellung der Uhrzeit, sie wird nur angezeigt.

:p.Dieser Befehl wird nur dann ausgefÅhrt, wenn auch die Freigabe in
der :link reftype=hd refid=IDD_CRON_FILE.Crontabs-Bearbeitung:elink. erfolgt
ist.

:p.Zusammen mit einem Ñhnlichen Befehl wÑhrend des Hochfahrens lÑ·t sich so
ein rudimentÑres Abrechnungsprotokoll fahren. Als Voraussetzung dazu sollte man
aber auch den Cron DÑmon schon beim Systemstart aktivieren. Das kann man z.B.
dadurch realisieren, indem man eine Referenz oder Kopie vom DÑmon in den
Systemstart-Ordner bringt.

:p.:hp1.VORSICHT&colon. Es verbleibt nur eine sehr kurze Verzîgerungsfrist,
bevor der DÑmon sich dann beendet. Im Falle eines Systemabschlusses kann die
verbliebene Zeit bis zum Booten ggf. nicht ausreichen, um komplizierte
Befehlsfolgen auszufÅhren. Eventuell wird in spÑteren Versionen eine
benutzerdefinierbare maximale Verzîgerungszeit angeboten, dazu sollten aber
BedarfswÅnsche geÑu·ert werden.:ehp1.
:li.:font facename=Courier size=18x10.Daily VirusCheck:font
facename=default size=0x0.
:p.TÑglich wird einmal zum frÅhestmîglichen Zeitpunkt das Programm
VirusCheck gestartet.
Man beachte, da· das Kommando :hp2.nicht:ehp2. beim HinzufÅgen zur Liste
ausgefÅhrt wird, sondern erst ab dem nÑchsten Tag gestartet wird, falls der
DÑmon am aktuellen Tag bereits gelaufen hat oder lÑuft.
:li.:font facename=Courier size=18x10.0 8-12,14-18 * * 1-5 start PMBeep >nul 2>&amp.1:font
facename=default size=0x0.
:p.Von Montag bis Freitag wird zu jeder vollen Stunde zwischen 8 und 12 sowie
zwischen 14 und 18 Uhr das Programm PMBeep gestartet. Das Programm lÑuft in
einer separaten Sitzung, Ausgaben werden ignoriert.
:eul.
.******************************************************************************
:h1 name=LAN_NETWORK.Benutzung im LAN und unter Netware
:i1.Netzwerk
:i1.Novell-Netware
:i1.LAN-Zugriff
:p.Die Kommunikation mit dem Cron DÑmon funktioniert am besten durch den
LAN-Zugriff auf eine Named Pipe, da diese Kommunikationsart vom OS/2 direkt
unterstÅtzt wird.

:p.Innerhalb eines Netzwerkes ist diese Kommunikationsart ebenfalls mîglich.
Sowohl das LAN, als auch Novell-Netware bieten dabei Pipe-Komponenten an.
Beide bieten explizite Zugriffsschutzmechanismen, so da· der DÑmon darauf
verzichten kann.

:p.Um den DÑmon innerhalb des Netzwerkes zur VerfÅgung zu stellen, mu· man
mit den jeweiligen Zugriffsschutz-Programmen (z.B. :hp2.net access:ehp2. im
LAN) den Zugriff auf die Pipe
:xmp.
\pipe\Cron-Daemon-Request
:exmp.
:p.fÅr die gewÅnschten Benutzer(-gruppen) erlauben. Danach kann mit dem
Hilfsprogramm :hp2.crontab:ehp2. die Crontabs-Datei des Rechner fernbedient
werden.
.******************************************************************************
:h1.Neues zwischen den Versionen
:p.Dieses Kapitel enthÑlt alle grî·eren énderungen zwischen den Versionen.
:ul.
:li.:link
reftype=hd refid=CHANG_120_112.énderungen zwischen der Version 1.20 und
Version 1.12:elink.
:eul.
.*=============================================================================
:h2 name=CHANG_120_112.énderungen zwischen der Version 1.20 und Version 1.12
:p.Folgende énderungen sind in Version 1.20 erfolgt&colon.
:ul compact.
:li.Kosmetische énderungen und diverse Fehlerbeseitigungen.
:li.UnterstÅtzung fÅr franzîsische Meldungen und Hilfe.
:li.In der PM-Version des DÑmons kann ein Job verÑndert werden.
:li.In Kommandos kînnen die Zeiten nun auch als Intervalle angegeben werden.
:hp2.0,4,5,6,9:ehp2. kann auch als :hp2.0,4-6,9:ehp2. angegeben werden.
:li.Die Spezialzeit :hp2.Daily:ehp2. erlaubt das Starten eines Kommandos zum
frÅhestmîglichen Zeitpunkt an einem Tag, aber nur einmal pro Tag.
:li.Farben und Font des Hauptfensters kînnen nun Åber die Schriftartpalette
und die Farbpalette der Systemkonfiguration geÑndert werden, die Werte
Åberdauern einen Programmabschlu·.
.br
Zum éndern der Vordergrundfarbe benutze man die Taste [Strg] mit der zweiten
Maustaste.
:li.Neben der Position wird nun auch die Darstellung des Fensters (minimiert,
maximiert oder normal) beim Programmabschlu· gespeichert und Åbernommen.
:eul.
.******************************************************************************
:h1.SelbstbeweihrÑucherung
:p.Dieses Programm wurde unter OS/2 fÅr EMX als PM-Programm entwickelt.
Tips, Fehlermeldungen und VerbesserungsvorschlÑge (siehe Datei
:hp2.program!:ehp2.) werden dankend entgegengenommen.

:p.Bei WÅnschen sollte man :hp2.vorher:ehp2. die Datei :hp2.wishlist:ehp2.
durchsehen, ob der Wunsch bereits registriert ist.

:p.Der Klau von Funktionen, Teilen von Programmen oder ganzen Dateien ist
explizit erlaubt. Bei ganzen Dateien oder kaum unterscheidbaren Dateien
greifen aber die allgemeinen Lizenzbestimmungen, damit andere Benutzer nicht
durch defekte Dateien irregeleitet werden. Spenden werden von jedem Sozialamt
gerne entgegengenommen, bei mir reicht 'ne mail mit 'nem simplen
"nicht schlecht".

:p.Vielen Dank an Leonard HÅesker. Er hat die englische Version der Hilfe
durchgesehen.

:p.Ebenfalls vielen Dank an die Folgenden. Sie haben den Cron DÑmon in die
anderen Sprachen Åbersetzt&colon.
:parml compact tsize=15 break=none.
:pt.Franzîsisch
:pd.Raphaâl Vanney
:eparml.

:p.Florian Gro·e-Coosmann
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
