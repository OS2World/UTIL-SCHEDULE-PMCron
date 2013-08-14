.* Copyright (c) 1995 Florian Gro·e-Coosmann, RCS section at the eof
.* Use &rbl. for a required blank ( ), &colon. for a colon (:)
:userdoc.
:docprof.
:ctrldef.
:ctrl ctrlid=CtrlID1 controls='ESC SEARCH PRINT INDEX CONTENTS BACK FORWARD'
coverpage.
:ectrldef.
.******************************************************************************
:h1 res=100 name=ID_CRON.The Cron Daemon (Summary)
:i1 global.Cron Daemon
:p.The cron daemon was born in the unix world. This program is a port to
OS/2. Some unix specific characteristics have been omitted and some new
OS/2 specific functions have been added, but in principle there are no major
changes in its functionality or calling facilities.

:p.Cron is a PM program and within the meaning of unix not a daemon, although
there is a possibility to start it as a daemon with the command line
:hp1.detach crond:ehp1..
Unfortunately there is not the full functionality under OS/2, because
foreground processes can't be started from the background. The usage is on your
own risk.

:p.The Cron Daemon doesn't accept any calling parameters. All
:link refid=IDD_NOTEBOOK reftype=hd.Settings:elink. will be made on the
desktop. Errorneous entries (e.g. after a path change) will be resetted to
their defaults, thus a program start should be possible in all cases.

:p.As in unix there is the possibility to change cron entries with the
program :hp2.crontab:ehp2.. OS/2 is a single user operation system. Because of
this the crontab options :hp2.-r:ehp2. and :hp2.-l:ehp2. have been omitted.
Instead, each cron job must be given directly to the program. For a full list
of calling conventions call :hp2.crontab:ehp2. without any parameters.
All important parameters may be found in the
:link reftype=hd refid=XMP_SYNTAX.syntax description:elink. or in the
:link reftype=hd refid=XMP_XMP.examples:elink..

:h4.:hp2.What is the main purpose of the Cron Daemon?:ehp2.
:p.The Cron Daemon is able to start programs at different times. It is possible
to choice the minutes, hours, month days, months and weekdays.
The program will be started when the time is reached. All combinations of
possible choices are allowed. Thus, mostly all chronological tasks may be done
by the system. You don't need any
:link refid=alarmclock reftype=fn.alarm clock:elink..
:fn id=alarmclock.Of course, you may use the cron daemon as a alarm clock!:efn.

:p.Cron redirects all output of a called program into an output file.
This can be used to analyse errors in the programs afterwards.
The unix programs mails the output (of each started program) to its owner.
This can't be done in OS/2. There is only one user and no standard mailing
program. The current output file contains all necessary informations about
cron, the start of programs and their output.

:p.A builtin mechanism suppresses an overflow of the file system on
rarely maintained systems. This mechanism limits the size of output files.
Several output files will be used. A new output file is used either when
reaching the maximum file size or on an
:link refid=IDME_NEXTFILE reftype=hd.user action:elink..

:p.The Cron Daemon takes over all tasks of the unix program :hp2.at:ehp2..
This command starts a program on a certain time but it will never be repeated.
To save a new communication program the keyword :hp2.Once:ehp2. was introduced.

:p.Moreover, the daemon has the ability to start programs on startup or
shutdown of the cron daemon, because many OS/2 machines won't run longer than
some hours. Programs which should run in the night may be run when starting the
machine. (data backup, data refreshing, clearing workspace, etc.)

:p.The Cron Daemon may be served over a network. This will be done by the
program :hp2.crontab:ehp2.. Supported networks are
:link refid=LAN_NETWORK reftype=hd.LAN/Netware:elink. and
:link refid=IDD_TCP reftype=hd.TCP/IP:elink..
.******************************************************************************
:h1.Colors and font
:i1.Colors
:i1.Font
:p.
The colors and the font of the display window may be changed. To do this
you should open in the folder :hp2.system configuration:ehp2. the color
or font palette and draw the requested item to the window.
:p.The background color is changed by using the second mouse button. While
holding down the key [Ctrl] the foreground color is changed.
:p.The values will be stored and retrieved on the next program start.
.******************************************************************************
:h1 res=101 name=IDME_NEXTFILE.Next output file
:i1.Next output file
:p.From the menu entry :hp2.Next output file:ehp2. it is possible to close
the current output file and to change to the next one. If there is already
such a file, it will be deleted before.

:p.When the maximum
:link refid=IDDE_OUTPUT_COUNT reftype=hd.number of output files:elink.
is reached it will be continued with number 0.

:p.The actual output file will be displayed in the main window of the program.

:p.The menu entry is inselectable if
:link refid=IDDE_OUTPUT_IGNORE_OUTPUT reftype=hd.Ignore output:elink. is
selected.

:p.This choice may be helpful to access the current output file since the file
is in a permanent access by the program. Reading the file is permitted (TYPE
works) but most editors need full access rights.
.******************************************************************************
:h1 hide res=105 name=IDME_HELPKEYS.Help on keys
:p.There are no specific "hotkeys" in this program. The general
:link reftype=hd database='wphelp.hlp' refid=193.keyboard assignments:elink.
may be found through the :hp2.Main Index:ehp2. in the
:hp2.Information Folder:ehp2. on the desktop.
.******************************************************************************
:h1 res=200 name=IDD_NOTEBOOK.Settings
:i1.Settings
:p.The
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. may be
opened from the menu by selecting the :hp2.Settings:ehp2. entry.
All user defineable settings may be changed there.
.*=============================================================================
:h2 res=201 name=IDDE_NOTEBOOK_PAGES.Settings notebook
:i1.Notebook
:p.Five different notebook pages may be selected and serviced in the normal
manner. Each page has a context sensitive help, of course.
The five pages are describing
:ul.
:li.the general :link
 reftype=hd refid=IDD_CRON_FILE.Crontabs processing:elink.
:li.the individual :link
 reftype=hd refid=IDD_CRONTABS.Crontabs entries:elink.
:li.the informations to place :link
 reftype=hd refid=IDD_OUTPUT.Output:elink. into a file
:li.the access restrictions to other computers using a :link
 reftype=hd refid=IDD_TCP.TCP/IP:elink. connection
:li.:link reftype=hd refid=IDD_GENERAL.general settings:elink. like the
language
:eul.
.*=============================================================================
:h2 res=300 name=IDD_CRON_FILE.Crontabs processing
:i1.Crontabs Processing
:p.This page from the :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink.
allows to change the processing of the Crontabs entries and to change the
filename of the Crontabs file.

:p.By selecting the :link
reftype=hd refid=IDDE_CRON_ATSTARTUP.Cronstart processing:elink. topic the
daemon will execute all Crontabs entries with a starting time of
:hp2.CronStart:ehp2.. These entries will be ignored if this topic isn't
selected.

:p.By selecting the :link
reftype=hd refid=IDDE_CRON_ATEXIT.Cronstop processing:elink. topic the
daemon will execute all Crontabs entries with a starting time of
:hp2.CronStop:ehp2.. These entries will be ignored if this topic isn't
selected.

:p.The filename and path of the Crontabs file is been displayed, too. You have
to select the
:link reftype=hd refid=IDDE_CRON_FILENAMECHANGE.Change:elink. pushbutton to
change it.
.*-----------------------------------------------------------------------------
:h3 res=301 name=IDDE_CRON_ATSTARTUP.Cronstart processing
:i1.Cronstart processing
:p.In difference to the unix version, the daemon is able to execute programs
at the time when the daemon starts. This isn't very useful in (greater) unix
systems but under OS/2 it may be very nice since programs which must be started
periodically may be started afterwards. (Of course, you can use the standard
mechanism in Startup.CMD or the system start folder.)
In addition it is possible to implement an user accounting together with the
:link reftype=hd refid=IDDE_CRON_ATEXIT.Cronstop processing:elink. if a
reference of the daemon is placed in the system start folder.

:p.More hints to use Crontabs entries may been found in the
:link reftype=hd refid=XMP_SYNTAX.syntax description:elink. or in the
:link reftype=hd refid=XMP_XMP.examples:elink..

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_CRON_FILE.Crontabs processing:elink..
.*-----------------------------------------------------------------------------
:h3 res=302 name=IDDE_CRON_ATEXIT.Cronstop processing
:i1.Cronstop processing
:p.In difference to the unix version the daemon is able to execute programs
at the shutdown of the daemon. This ability must be used very carefully since
the daemon will die closed after the closing command. You should use the
OS/2 command :hp2.start:ehp2. because the daemon dies after about 1 second.
In this short period much useful work can be done (user accounting,
network shutdown, for example).

:p.More hints to use Crontabs entries may been found in the
:link reftype=hd refid=XMP_SYNTAX.syntax description:elink. or in the
:link reftype=hd refid=XMP_XMP.examples:elink..

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_CRON_FILE.Crontabs processing:elink..
.*-----------------------------------------------------------------------------
:h3 res=303 name=IDDE_CRON_FILENAME.file
:i1.Filename (Crontabs)
:p.The filename of the Crontabs data may be changed by selecting the button
:link reftype=hd refid=IDDE_CRON_FILENAMECHANGE.Change filename:elink..
The actual path and filename is displayed. The default setting of this field is
selected on the first call of the program. It is the filename
:hp2.Crontabs:ehp2. and the starting path of the program. If any errors
occurs while checking the (new) file, (a read-only file or a directory of
the same name exists) the filename is changed to :hp2.nul:ehp2..

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_CRON_FILE.Crontabs processing:elink..
.*-----------------------------------------------------------------------------
:h3 res=304 name=IDDE_CRON_FILENAMECHANGE.Change filename
:i1.Changing the filename (Crontabs)
:p.Selecting this pushbutton let you change the path and filename of the
Crontabs file. The standard window of the system is used to change and
select another file name.

:p.The file will be created if it doesn't exist. A normal filename must be
selected. There is one exception&colon. The file :hp2.nul:ehp2. is used as an
"emergency file". The contents of an existing file is checked. Every line must
pass the
:link reftype=hd refid=XMP_SYNTAX.syntax:elink. check. If it succeeds, the new
filename is used and the old one is closed, otherwise the old filename
persists.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_CRON_FILE.Crontabs processing:elink..
.*=============================================================================
:h2 res=310 name=IDD_CRONTABS.Crontabs entries
:i1.Crontabs entries
:i1.Entries of the Crontabs file
:p.This page from the :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink.
permits to change and view the entries of the Crontabs file.

:p.By selecting the :link
reftype=hd refid=IDDE_CRONTABS_ADD.Add:elink. topic the contents of the
previously edited field :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.New command:elink. will be appended to the
current crontabs file and executed when needed. The new command must pass the
:link reftype=hd refid=XMP_SYNTAX.syntax:elink. check.

:p.Selecting of :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.Change:elink. is equivalent to a
deletion und a simultanous addition of the editing entry.

:p.By selecting the :link
reftype=hd refid=IDDE_CRONTABS_ABORT.Abort:elink. topic the edit of the change
of an entry is rejected.

:p.By selecting the :link
reftype=hd refid=IDDE_CRONTABS_DELETE.Delete:elink. all marked lines of the
:link
reftype=hd refid=IDDE_CRONTABS_LIST.Existing entries:elink. list will be
deleted.

:p.By selecting the :link
reftype=hd refid=IDDE_CRONTABS_EDIT.Edit:elink. the marked element of the
:link reftype=hd refid=IDDE_CRONTABS_LIST.Existing entries:elink. list
will be copied to the edit field. After a successful rewrite the element is
replaced by pressing the :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.Change:elink. button.

:p.All push button are accessible only if a valid operation is possible.
.*-----------------------------------------------------------------------------
:h3 res=311 name=IDDE_CRONTABS_LIST.Existing entries
:i1.Entry list of existing Crontabs commands
:p.The list displays all entries of the Crontabs file.
Each line may be (de)selected by the left mouse button or the space key.
After selecting the use of the pushbutton :link
reftype=hd refid=IDDE_CRONTABS_DELETE.Delete:elink. deletes all selected
entries.
:hp2.Be careful, there is no "undo" function.:ehp2.

:p.Alternatively an entry may be changed by pressing :link
reftype=hd refid=IDDE_CRONTABS_EDIT.Edit:elink..

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_CRONTABS.Crontabs entries:elink..
.*-----------------------------------------------------------------------------
:h3 res=312 name=IDDE_CRONTABS_ENTRY.New command
:i1.Crontabs entry (New)
:p.A new Crontabs command may be entered in this field. It must be in
compliance with the
:link reftype=hd refid=XMP_SYNTAX.syntax:elink. rules. After editing the
use of the button :link
reftype=hd refid=IDDE_CRONTABS_ADD.Add:elink. adds the command to the Crontabs
file. If no error occurs the field is deleted and the command will be
transferred to the end of the :link
reftype=hd refid=IDDE_CRONTABS_LIST.entry list:elink.. On an error the
contents persists and an error message is displayed.

:p.If the edit field has been filled by using of the :link
reftype=hd refid=IDDE_CRONTABS_EDIT.Edit:elink. button the entry may be changed
by the use of :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.Change:elink.. The change request may
be rejected by the :link
reftype=hd refid=IDDE_CRONTABS_ABORT.Abort:elink. button.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_CRONTABS.Crontabs entries:elink..
.*-----------------------------------------------------------------------------
:h3 res=313 name=IDDE_CRONTABS_ADD.Add entry
:i1.Adding a Crontabs entry
:p.Selecting this pushbutton let you transfer the content of the field :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.New command:elink. to the Crontabs list.

:p.The entry must comply with the
:link reftype=hd refid=XMP_SYNTAX.syntax:elink. rules.
If an error occurs the content of the field persists and an error message
is displayed.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_CRONTABS.Crontabs entries:elink..
.*-----------------------------------------------------------------------------
:h3 res=316 name=IDDE_CRONTABS_CHANGE.Changing entry
:i1.Changing a Crontabs entry
:p.Selecting this pushbutton let you transfer the content of the field :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.New command:elink. to the Crontabs list.
In doing so you replace the entry of which the contents of the field is
generated.

:p.The entry must comply with the
:link reftype=hd refid=XMP_SYNTAX.syntax:elink. rules.
If an error occurs the content of the field persists and an error message
is displayed.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_CRONTABS.Crontabs entries:elink..
.*-----------------------------------------------------------------------------
:h3 res=317 name=IDDE_CRONTABS_ABORT.Aborting edition of an entry
:i1.Aborting the edition of a Crontabs entry
:p.Selecting this pushbutton will abort the edition of an existing entry.
The edit field will be filled with an empty string.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_CRONTABS.Crontabs entries:elink..
.*-----------------------------------------------------------------------------
:h3 res=314 name=IDDE_CRONTABS_DELETE.Delete entry
:i1.Deleting Crontabs entries
:p.Selecting this pushbutton let you delete all marked lines of the :link
reftype=hd refid=IDDE_CRONTABS_ENTRY.Existing entries:elink. list.

:p.All marked lines will be deleted irretrievably.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_CRONTABS.Crontabs entries:elink..
.*-----------------------------------------------------------------------------
:h3 res=315 name=IDDE_CRONTABS_EDIT.Edit entry
:i1.Editing a Crontabs entry
:p.Selecting this pushbutton will copy the marked line of the :link
reftype=hd refid=IDDE_CRONTABS_LIST.Extsing entries:elink. list to the field
:link reftype=hd refid=IDDE_CRONTABS_ENTRY.New Command:elink. to change it.
The entry will remain in the list caused by security reasons and will be
replaced by pressing the :link
reftype=hd refid=IDDE_CRONTABS_CHANGE.Change:elink. button. Using the :link
reftype=hd refid=IDDE_CRONTABS_ABORT.Abort:elink. button let you reject the
changing of the entry.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_CRONTABS.Crontabs entries:elink..
.*=============================================================================
:h2 res=400 name=IDD_OUTPUT.Output
:i1.Output
:p.This page from the :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink.
permits to change the output path and the output-related settings of the
daemon and its child processes.

:p.By selecting the :link
reftype=hd refid=IDDE_OUTPUT_COUNT.Number of output files:elink. and the :link
reftype=hd refid=IDDE_OUTPUT_THRES.Filesize threshold:elink. the maximum used
disk space is selected.

:p.The path of the output is displayed, too. You may select
:link reftype=hd refid=IDDE_OUTPUT_PATHCHANGE.Change path:elink.
to select another path.

:p.It is possible to :link
reftype=hd refid=IDDE_OUTPUT_IGNORE_OUTPUT.Ignore output:elink. by selecting
the check box.

:p.The unix version of Cron mails the output of each started job to the
user. Under OS/2 there is no standard mailing system and OS/2 is a single user
system. Therefore, all messages and the output of the programs will be
redirected to a file. This may cause a file system overflow on poorly
maintained systems. To stop this behaviour a new file is selected if a
specific threshold of the file size is reached. All output files are named
:hp2.Cron000?.out:ehp2.. The question mark is replaced by the cyphers 0
upto the maximum selected number of output files.

:p.The current output file is accessed permanently if the output isn't
ignored totally.
Thus, some programs can't access the output file to display the contents.
(The standard command TYPE works.)
.br
You may select :link
reftype=hd refid=IDME_NEXTFILE.Next output file:elink. from the menu to
close the current output file while opening the next one. Now you can edit or
display the file.
.*-----------------------------------------------------------------------------
:h3 res=401 name=IDDE_OUTPUT_THRES.Threshold
:i1.Threshold of the file size
:p.The current output file is changed if its size grows beyond a threshold.
Only specific values may be selected.

:p.The relation to other values is described in the help of the
:link reftype=hd refid=IDD_OUTPUT.Output:elink..

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_OUTPUT.Output:elink..
.*-----------------------------------------------------------------------------
:h3 res=402 name=IDDE_OUTPUT_COUNT.Number of output files
:i1.Number of output files
:p.The output files are named :hp2.Cron000?.out:ehp2.. The question mark is
replaced by the cyphers 0 upto the selected number of output files.

:p.The relation to other values is described in the help of the
:link reftype=hd refid=IDD_OUTPUT.Output:elink..

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_OUTPUT.Output:elink..
.*-----------------------------------------------------------------------------
:h3 res=403 name=IDDE_OUTPUT_PATH.Path of output files
:i1.Output path
:p.This field displays the current output path. The default value on the first
program start is not the current directory. The path points to the directory
from which the daemon is started.

:p.To change this value press the button
:link reftype=hd refid=IDDE_OUTPUT_PATHCHANGE.Change path:elink..
A corresponding
:link reftype=hd refid=IDD_OUTPUT_PATH.select box:elink. will be opened.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_OUTPUT.Output:elink..
.*-----------------------------------------------------------------------------
:h3 res=404 name=IDDE_OUTPUT_PATHCHANGE.Change path
:i1.Changing output path (pushbutton)
:p.Selecting this pushbutton let you change the path of the output files.
If you press the pushbutton a
:link reftype=hd refid=IDD_OUTPUT_PATH.select box:elink. will be opened.
After changing the path the
:link reftype=hd refid=IDDE_OUTPUT_PATH.display field:elink. will be refreshed.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_OUTPUT.Output:elink..
.*-----------------------------------------------------------------------------
:h3 res=405 name=IDDE_OUTPUT_IGNORE_OUTPUT.Ignore output
:i1.Output suppression
:p.It is possible to suppress all output from the daemon and its child
processes. The output won't be written to a file.

:p.This is the default setting on the first program start. It is recommended to
change the values on this page to the local requirements and to allow the
printing of the output to detect errors in the programs.

:p.If this field is selected the menu entry :link
reftype=hd refid=IDME_NEXTFILE.Next output file:elink. in the main menu can't
be selected.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_OUTPUT.Output:elink..
.*-----------------------------------------------------------------------------
:h3 toc=4 res=410 name=IDD_OUTPUT_PATH.Changing the output path
:i1.Changing the output path (dialog)
:p.This dialog box will be opened by selecting the button
:link reftype=hd refid=IDDE_OUTPUT_PATHCHANGE.Change path:elink..

:p.The current path of the output files will be displayed in the :link
reftype=hd refid=IDDE_OUTPUT_PATH_ENTRY.Output path entry field:elink..
Every double mouse click into the :link
reftype=hd refid=IDDE_OUTPUT_PATH_LIST.Output path list:elink. will copy
the content of the line to the entry field.

:p.Of course, the new path may be typed directly.

:p.This dialog box is called from the page
:link reftype=hd refid=IDD_OUTPUT.Output:elink. in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink..
.*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
:h4 res=411 name=IDDE_OUTPUT_PATH_ENTRY.Output path entry field
:i1.Output path entry field
:p.This entry field is changed either due to a user input or the using of the
:link reftype=hd refid=IDDE_OUTPUT_PATH_LIST.Output path list:elink..
Initially, the value will be the current output path.

:p.After pressing the confirmation button the content will be checked if it
satisfies some restrictions&colon. The path must exist and it shall be possible
to create or manipulate ten files of the form :hp2.Cron000?.out:ehp2.. The
question-mark is replaced by the ten decimal cyphers.
.br
If an error occurs a message box is displayed showing the circumstances
of the error. A different path should be selected, you can abort or correct the
problem with another program (e.g. creating the path).

:p.This entry is found in the dialog box for
:link reftype=hd refid=IDD_OUTPUT_PATH.Changing the output path:elink. of the
page
:link reftype=hd refid=IDD_OUTPUT.Output:elink. in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink..
.*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
:h4 res=412 name=IDDE_OUTPUT_PATH_LIST.Output path list
:i1.Output path list
:p.The use of the output path list offers a comfortable way to change the
current output path.

:p.The list will contain all subdirectories of the current path and the
drives list without floppies. By doubleclicking the left mouse button the
component will be selected and appended to the current selection. (Drive
selection will replace the complete selection.)

:p.The
:link reftype=hd refid=IDDE_OUTPUT_PATH_LIST.Output path entry field:elink.
will redisplay on each selection. By pressing the OK button the current
selection will be checked and copied.

:p.This entry is found in the dialog box for
:link reftype=hd refid=IDD_OUTPUT_PATH.Changing the output path:elink. of the
page
:link reftype=hd refid=IDD_OUTPUT.Output:elink. in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink..
.*=============================================================================
:h2 toc=3 res=500 name=IDD_TCP.TCP/IP
:i1.TCP/IP
:p.This page of the :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink.
offers some facilities to access the daemon from a remote machine via TCP/IP.

TCP/IP has no access restrictions. It is impossible to the system to deny
specific communications. Generally, TCP/IP daemons are using the local system
security facilities on unix systems. OS/2 doesn't provide any security because
it is a single user operating system. The Cron daemon permits the communication
over TCP/IP only with specific computers, thus filling the security hole.
The names of the computers will be read from a file, one computer name on each
line. Valid computer names are either the dotted number specification (e.g.
128.176.248.76) or the (fully) qualified internet name (e.g. sauron or
sauron@uni-muenster.de).

:p.The filename and path of the file containing the permissions will be shown
in a field. If you want to change it, you have to use the button
:link reftype=hd refid=IDDE_TCP_FILENAMECHANGE.Change filename:elink..

:p.An access from a remote computer is only permitted after the selection of
the check box :link
reftype=hd refid=IDDE_TCP_ALLOWACCESS.Permit access:elink..
.br
The selection is only possible if TCP/IP is installed and running on the
computer.

:p.The processing of the Crontabs file from a remote machine is done by the
program :hp2.crontab:ehp2..

:p.The IP port number of the cron daemon is 64020.
.*-----------------------------------------------------------------------------
:h3 res=501 name=IDDE_TCP_FILENAME.filename
:i1.Filename (TCP/IP access permissions)
:p.The name of the file containing the names of the computer which are
allowed to access the daemon over TCP/IP can be changed by pressing the button
:link reftype=hd refid=IDDE_TCP_FILENAMECHANGE.Change filename:elink..
The current path and filename will be displayed. On the first start of the
program the file :hp2.nul:ehp2. is used by default.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_TCP.TCP/IP access restrictions:elink..
.*-----------------------------------------------------------------------------
:h3 res=502 name=IDDE_TCP_FILENAMECHANGE.Change filename
:i1.Changing the filename (TCP/IP access permissions)
:p.Selecting this pushbutton let you change the path and filename of the
TCP/IP access permissions. The standard window from the system is used to
change and select another file name.

:p.The file will be created if it doesn't exist. A normal file name should be
selected. There is one exception&colon. The file :hp2.nul:ehp2. is used as an
"emergency file". The contents of an existing file is checked. Every line must
be the TCP/IP computer name either in dotted or named notation.
Empty lines and lines starting with a hash (:hp2.#:ehp2.) or semicolon
(:hp2.;:ehp2.) will be ignored.
The old file is closed after the new one is read und interpreted successfully.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_TCP.TCP/IP access restrictions:elink..
.*-----------------------------------------------------------------------------
:h3 res=503 name=IDDE_TCP_ALLOWACCESS.Permit access
:i1.Access, permitting over TCP/IP
:i1.TCP/IP access permission
:p.This check box gives control to the general access permission of remote
machines over TCP/IP.
This box is a fine method to deny the access over TCP/IP without changing
the contents of the :link
reftype=hd refid=IDDE_TCP_FILENAME.TCP/IP access permission file:elink..

:p.The selection is only possible if TCP/IP is installed and running on the
computer.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page of the
:link reftype=hd refid=IDD_TCP.TCP/IP access restrictions:elink..
.*=============================================================================
:h2 res=600 name=IDD_GENERAL.General settings
:i1.General settings
:p.This page of the :link
reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink.
let you change some general settings of the program.

Currently, on this page the only selectable value is the preferred language.
By default, the language is set to "English" on the first program start if
there isn't an environment variable called :hp2.LANG:ehp2. which points to
anything else.
Supported :hp2.LANG:ehp2. values are&colon.
:dl compact tsize=15.
:dthd.:hp5.LANG value:ehp5.
:ddhd.:hp5.Language:ehp5.
:dt.USA
:dd.English
:dt.UK
:dd.English
:dt.GERM
:dd.Deutsch
:dt.FRAN
:dd.Franáais
:edl.
:p.The language my be changed by using the :link
reftype=hd refid=IDDE_GENERAL_LANGUAGE.language selecting box:elink..
.*-----------------------------------------------------------------------------
:h3 res=601 name=IDDE_GENERAL_LANGUAGE.Preferred language
:i1.Language
:p.The preferred language may be selected by using this list box.
:link refid=NewLanguages reftype=fn.There are only some possible
languages.:elink.

By default, the language is set to "English" on the first program start if
there isn't an environment variable called :hp2.LANG:ehp2. which points to
anything else. Look at the
:link reftype=hd refid=IDD_GENERAL.General Settings:elink. to get more
informations.

Currently, supported languages are&colon.
:ul.
:li.German
:li.English
:li.French
:eul.

:p.Change only take effect after a restart of the program.

:p.This entry is found in the
:link reftype=hd refid=IDDE_NOTEBOOK_PAGES.Settings notebook:elink. on the
page with the
:link reftype=hd refid=IDD_GENERAL.General settings:elink..
:fn id=NewLanguages.This may change if there are some guys who translate
this stuff!:efn.
.******************************************************************************
:h1 name=XMP_GENERAL.Examples and syntax of Crontabs entries
:p.This chapter describes the :link
reftype=hd refid=XMP_SYNTAX.syntax:elink. of the Crontabs entries and their use
using :link reftype=hd refid=XMP_XMP.examples:elink..

:p.In contrast to the syntax of the program :hp2.crontabs:ehp2. and to possible
entries in the :link
reftype=hd refid=IDD_CRONTABS.Crontabs entries:elink.-field in the notebook it
is allowed to use comments.

:p.Comment lines are empty lines or lines beginning with a
semicolon (:hp2.;:ehp2.) or hash (:hp2.#:ehp2.) in the first column.
.*=============================================================================
:h2 name=XMP_SYNTAX.Syntax of Crontabs entries
:i1.Syntax of Crontabs entries
:p.The syntax of Crontabs entries is pretty simple. Each entry must be placed
on a separate line. There is no difference between upper and lower case.
Every line has the following form
:xmp.[flags] times command:exmp.
:p.The :hp2.flags:ehp2. are optional. Currently the following flags are
supported&colon.
:dl compact.
:dthd.:hp5.Flag:ehp5.
:ddhd.:hp5.Meaning:ehp5.
:dt.:hp2.Once:ehp2.
:dd.The command will only be executed once at the given time. After this, the
command is deleted.
:edl.

:p.Normally, the :hp2.times:ehp2.-field consists of five entries;
:hp2.each:ehp2. of them must pass the time check to run the command&colon.
:ol.
:li.:hp2.minutes:ehp2.
:p.This :link refid=NumberSpecification reftype=fn.number specification:elink.
describes the minute within an hour at which the command should be started.
Possible values are between 0 and 59.
:li.:hp2.hours:ehp2.
:p.This :link refid=NumberSpecification reftype=fn.number specification:elink.
describes the hour at which the command should be started.
Possible values are between 0 and 23.
:li.:hp2.month day:ehp2.
:p.This :link refid=NumberSpecification reftype=fn.number specification:elink.
describes the day of the month within a month at which the command should be
started. Possible values are between 1 and 31. These values don't depend on the
month. A command will never be executed on February, 31st, although, this is a
valid entry.
:li.:hp2.month:ehp2.
:p.This :link refid=NumberSpecification reftype=fn.number specification:elink.
describes the month at which the command should be started.
Possible values are between 1 and 12.
:li.:hp2.day of the week:ehp2.
:p.This :link refid=NumberSpecification reftype=fn.number specification:elink.
describes the day of the week at which the command should be started.
Possible values are between 0 (Sunday) and 6.
:eol.

:p.There are three more special times which replace the five entries&colon.
:dl compact tsize=15.
:dthd.:hp5.special time:ehp5.
:ddhd.:hp5.meaning:ehp5.
:dt.:hp2.Cronstart:ehp2.
:dd.The command will be executed at the start of the daemon. The command will
only be executed if you have permitted the
:link reftype=hd refid=IDD_CRON_FILE.Crontabs processing:elink..
:dt.:hp2.Cronstop:ehp2.
:dd.The command will be executed at the shutdown of the daemon. The command
will only be executed if you have permitted the
:link reftype=hd refid=IDD_CRON_FILE.Crontabs processing:elink..
.br
:hp1.BE CAREFUL&colon. There is only a very short time before the daemon stops.
In case of a system shutdown this time may be too short to execute complicated
or long commands. Maybe, a future version of the daemon will have a user
definable delay time on shutdown. You should ask for it if you want it.
:ehp1.
:dt.:hp2.Daily:ehp2.
:dd.The command wille be executed once a day at the first possible time.
Please note, the command :hp2.won't:ehp2. be executed while adding it to the
list. Instead it will be started on the next day if the daemon is running or
has run this day.
:edl.

:p.Every valid OS/2 command may be entered as :hp2.command:ehp2.. Currently,
it doesn't exist any specification of the program type, this will change! In
case of a DOS command you should use the OS/2 command :hp2.start:ehp2.. The
standard outputs of the command will be collected in a temporary file and
appended to the :link
reftype=hd refid=IDD_OUTPUT.output:elink. file at the end of the program.
A new session is started by using :hp2.start:ehp2.. The session will not be
under the control of the daemon. Because of this, the output of the program
should be redirected by appending
:xmp. >file 2>&amp.1:exmp.
:p.to the end of the command line. The use of the file :hp2.nul:ehp2. let the
output been ignored. PM programs normally don't use standard output. Thus, you
should append the redirection every time.

:p.The temporary files will been created in the same subdirectory where the
daemon is placed. It is recommended to delete this "zombies" periodically by
the Crontabs commands
:xmp.
Cronstart del ?&colon.\path\*.tmp >nul 2>&amp.1
0 3 * * 0 del ?&colon.\path\*.tmp >nul 2>&amp.1
:exmp.
:p.The question mark must been replaced by the correct drive letter.

:fn id=NumberSpecification.A number specification is a comma delimited list of
numbers or number intervals at which the event should happen. A star
(:hp2.*:ehp2.) has the meaning&colon.
every possible number. Examples&colon.
:xmp.1,2,5:exmp.
:p.or
:xmp.8-12,14-18:exmp.
:p.or
:xmp.*:exmp.:efn.
.*=============================================================================
:h2 name=XMP_XMP.Examples of Crontabs entries
:i1.Examples
:p.There is a plenty of possible usages of the Cron daemon. Some of them are
shown in the examples. Command names are used which doesn't really exist on
some systems. Regardlessly, the name of the commands shows its purpose.

:ul.
:li.:font facename=Courier size=18x10.* * * * * echo A minute has gone.:font
facename=default size=0x0.
:p.Every minute in every hour on every day the line :hp2.A minute has
gone.:ehp2. will be printed. This output of the command echo will be copied to
the :link
reftype=hd refid=IDD_OUTPUT.output:elink. file of the Cron daemon.
:li.:font facename=Courier size=18x10.0 * * * * echo An hour has gone. >nul 2>&amp.1:font
facename=default size=0x0.
:p.Every full hour on every day the line :hp2.An hour has gone.:ehp2. will be
printed. Because of the deletion of the output this is a useless command. Yet,
a message of the daemon is printed into the output file indicating that the
command has been started.
:li.:font facename=Courier size=18x10.0,30 * * * * DeliverMail:font
facename=default size=0x0.
:p.On every full and half hour the program DeliverMail will be started. All
output will be copied to the output file of the daemon.
:li.:font facename=Courier size=18x10.0,30 * * * * start PMDeliverMail >nul 2>&amp.1:font
facename=default size=0x0.
:p.On every full and half hour the program PMDeliverMail will be started. All
output will be ignored. The program is started in a separate session (needed
for PM programs!).
:li.:font facename=Courier size=18x10.0 3 * * 0 MyBackup /weekly:font
facename=default size=0x0.
:p.On every Sunday at 3am the program MyBackup with the parameter /weekly will
be started.
:li.:font facename=Courier size=18x10.0 3 1 * * MyBackup /monthly:font
facename=default size=0x0.
:p.On every first day of a month at 3am the program MyBackup with the parameter
/monthly will be started.
:li.:font facename=Courier size=18x10.Once 0 3 1 4 * time 4:font
facename=default size=0x0.
:p.At 3am on April, 1st one hour is added to the system time. This command is
only executed once.
:li.:font facename=Courier size=18x10.Once * * * * * setboot /b >nul 2>&amp.1:font
facename=default size=0x0.
:p.The machine is rebooted on the next possible minute. The command is executed
only once. This is very important! The output is ignored (you will see the
result). Don't try this command for exercises, :hp2.setboot /b:ehp2. is
equivalent to the pressing of Alt-Ctrl-Del. This command may be useful in case
of a predeterminable power loss in your house (emergency power check) or if you
see your colleague is working with an unsafed power circuit. A command with
:hp2.crontab:ehp2. is faster than your feet to safe the server 5 rooms farther.
(I've seen such situations!)
:li.:font facename=Courier size=18x10.0 3 * * * deltree f&colon.\tmp &amp. mkdir f&colon.\tmp &amp. echo This directory will be deleted at 3am! > f&colon.\tmp\Warning.!!!:font
facename=default size=0x0.
:p.At 3am the command deltree with the parameter f&colon.\tmp is invoked, after
this the directory is recreated and a warning file is put in the directory. All
is done by a single command interpreter. Thus, all output is printed
chronologically into the output file of the daemon.
:li.:font facename=Courier size=18x10.Cronstart del c&colon.\cron\*.tmp >nul 2>&amp.1:font
facename=default size=0x0.
:p.At every start of the daemon all temporary files in the cron directory will
been deleted without any output.

:p.This command will only been executed if you have permitted it in the
:link reftype=hd refid=IDD_CRON_FILE.Crontabs processing:elink. page.

:p.This command is very useful because some temporary files won't be deleted
in experience. This isn't a bug of the daemon. In most cases you have
forgotten to append a redirection to a line with a :hp2.start:ehp2.. The
temporary files were blocked in this cases, thus the daemon can't copy or
delete them.
:li.:font facename=Courier size=18x10.Cronstop echo Logoff&colon. >> \Times\florian &amp. time >> \Times\florian:font
facename=default size=0x0.
:p.At every shutdown of the daemon a message and the current time will be
appended to a file. The standard input is redirected to the file
:hp2.nul:ehp2.. Thus, the command :hp2.time:ehp2. won't block the input or set
a new time.

:p.This command will only been executed if you have permitted it in the
der :link reftype=hd refid=IDD_CRON_FILE.Crontabs processing:elink. page.

:p.Together with a similar command at startup of the daemon a simple accounting
protocol may be implemented if you put a reference of the daemon icon into the
system start folder.

:p.:hp1.BE CAREFUL&colon. There is only a very short time before the daemon
stops. In case of a system shutdown this time may be too short to execute
complicated or long commands. Maybe, a future version of the daemon will have a
user definable delay time on shutdown. You should ask for it if you want it.
:ehp1.
:li.:font facename=Courier size=18x10.Daily VirusCheck:font
facename=default size=0x0.
:p.The program VirusCheck will been executed once a day at the earliest
possible time.
Please note, the command :hp2.won't:ehp2. be executed while adding it to the
list. Instead it will be started on the next day if the daemon is running or
has run this day.
:li.:font facename=Courier size=18x10.0 8-12,14-18 * * 1-5 start PMBeep >nul 2>&amp.1:font
facename=default size=0x0.
:p.Between Monday and Friday at every full hour between 8 and 12 and
between 14 and 18 o'clock the program PMBeep will be executed. The program
will run in a separat session, all output will be ignored.
:eul.
.******************************************************************************
:h1 name=LAN_NETWORK.Using in a LAN and with Netware
:i1.Network
:i1.Novell Netware
:i1.LAN-Access
:p.The communication with the Cron daemon works at its best by access with a
LAN through a named pipe because this communication is directly supported by
OS/2.

:p.It is possible to use this communication within a network. Both LAN and
Netware offers named pipe components. Both of them have their own access
control mechanisms. Thus, the daemon can work without more security functions.

:p.To allow the access of the daemon within a network you have to allow it with
the network-specific program. (e.g. :hp2.net access:ehp2. with LAN) The name
of the offered resource is the pipe
:xmp.
\pipe\Cron-Daemon-Request
:exmp.
:p.After you have done this, the daemon can be accessed from a remote machine
by using the utility program :hp2.crontab:ehp2..
.******************************************************************************
:h1.News between the versions
:p.This chapter contains all major changes between the versions.
:ul.
:li.:link
reftype=hd refid=CHANG_120_112.Changes applied between version 1.20 and
version 1.12:elink.
:eul.
.*=============================================================================
:h2 name=CHANG_120_112.Changes applied between version 1.20 and version 1.12
:p.The following changes have been made in version 1.20&colon.
:ul compact.
:li.Cosmetic changes and some bug fixes.
:li.Support of french messages and help.
:li.The PM version of the daemon now is able to change a job.
:li.Intervals are now supported in the time fields of commands.
:hp2.0,4,5,6,9:ehp2. is equivalent to :hp2.0,4-6,9:ehp2..
:li.The special time :hp2.Daily:ehp2. allows starting of commands at the
earliest possible time of a day but only once a day.
:li.Colors and font of the display window may been changed by the color and
font palette of the system configuration. The values persists a program
termination.
.br
Hold down the [Ctrl]-key while using the second mouse button to change the
foreground color.
:li.Version 1.12 has saved the position and size of the window only. The
new version stores the display mode (minimized, maximized, restored), too.
:eul.
.******************************************************************************
:h1.Self-adulation
:p.This program has been developed under OS/2 with EMX as a PM program.
Tips, error messages and correcting recommendations will been accepted
gratefully. (Please, have a look into the file look :hp2.program!:ehp2.)

:p.If you have wishes please, look into the file :hp2.wishlist:ehp2..
Maybe, your wish is already registered.

:p.The pinch of functions, parts of programs or whole files is permitted
explicitely. If you use whole files or files with only some little changes
the general license take place. You must leave the copyright statement and
you must declare your changes, so you don't fool other users. Donations are
accepted by every social welfare center, a mail to me with "not bad" is enough.

:p.Special thanks to Leonard HÅesker. He has revised the English version
of the help.

:p.Many thanks to the following people. They have translated the cron daemon
to other languages&colon.
:parml compact tsize=10 break=none.
:pt.French
:pd.Raphaâl Vanney
:eparml.

:p.Florian Gro·e-Coosmann
.br
E-Mail within Internet&colon. :font facename=Courier size=18x10.coosman@uni-muenster.de:font
facename=default size=0x0.
:euserdoc.
.* RCS depending informations
.*
.* $Id: crondeng.ipf 1.3 1995/10/18 11:13:30 Florian Rel $
.*
.* $Name: Version121 $
.*
.* $Log: crondeng.ipf $
.* Revision 1.3  1995/10/18 11:13:30  Florian
.* Control pushbuttons added.
.*
.* Revision 1.2  1995/10/18 09:46:07  Florian
.* Some cosmetic changes.
.* Chapters Colors and font and News between the versions added.
.* Crontabs Chapter rewritten.
.* Daily and intervals explained in XMP and SYNTAX.
.* Added support of the French language.
.*
.* Revision 1.1  1995/08/03 07:52:06  Florian
.* Initial revision
.*
.*
