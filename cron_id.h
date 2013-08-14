/* ENGLISH is the default language and has no extension (10000th-digit 0) */
#define ID_CRON                           100

#define IDME_NEXTFILE                     101
#define IDME_HELPINDEX                    102
#define IDME_HELPGENERAL                  103
#define IDME_HELPUSINGHELP                104
#define IDME_HELPKEYS                     105

#define IDD_NOTEBOOK                      200
#define IDDE_NOTEBOOK_PAGES               201

#define IDD_CRON_FILE                     300
#define IDDE_CRON_ATSTARTUP               301
#define IDDE_CRON_ATEXIT                  302
#define IDDE_CRON_FILENAME                303
#define IDDE_CRON_FILENAMECHANGE          304
#define IDDE_CRON_HELP                    305

#define IDD_CRONTABS                      310
#define IDDE_CRONTABS_LIST                311
#define IDDE_CRONTABS_ENTRY               312
#define IDDE_CRONTABS_ADD                 313
#define IDDE_CRONTABS_DELETE              314
#define IDDE_CRONTABS_EDIT                315
#define IDDE_CRONTABS_CHANGE              316
#define IDDE_CRONTABS_ABORT               317
#define IDDE_CRONTABS_HELP                318

#define IDD_OUTPUT                        400
#define IDDE_OUTPUT_THRES                 401
#define IDDE_OUTPUT_COUNT                 402
#define IDDE_OUTPUT_PATH                  403
#define IDDE_OUTPUT_PATHCHANGE            404
#define IDDE_OUTPUT_IGNORE_OUTPUT         405
#define IDDE_OUTPUT_HELP                  406

#define IDD_OUTPUT_PATH                   410
#define IDDE_OUTPUT_PATH_ENTRY            411
#define IDDE_OUTPUT_PATH_LIST             412

#define IDD_TCP                           500
#define IDDE_TCP_FILENAME                 501
#define IDDE_TCP_FILENAMECHANGE           502
#define IDDE_TCP_ALLOWACCESS              503
#define IDDE_TCP_HELP                     504

#define IDD_GENERAL                       600
#define IDDE_GENERAL_LANGUAGE             601
#define IDDE_GENERAL_LANGUAGE_WARNING     602
#define IDDE_GENERAL_HELP                 603

#define IDD_ABOUT                         900

#define IDS_Title                        1008
#define IDS_NotEnoughMemory              1009
#define IDS_NoOutputFile                 1010
#define IDS_StatusAtStartup              1011
#define IDS_StatusAtExit                 1012
#define IDS_StatusNormal                 1013
#define IDS_StatusCommunication          1014
#define IDS_DieOnSigSig                  1015

#define IDS_NoCrontabsFile               1024
#define IDS_ErrCrontabsFile              1025
#define IDS_DefaultCrontabsFile          1026
#define IDS_DefaultOutput                1027
#define IDS_NoTCPIPFile                  1028
#define IDS_DefaultTCPIPFile             1029
#define IDS_ErrInitialize                1030
#define IDS_ErrMsgQueue                  1031
#define IDS_ErrReassignStdIO             1032
#define IDS_AlreadyStarted               1033
#define IDS_CantCallMyself               1034
#define IDS_ErrCreateSem                 1035
#define IDS_ErrQMod                      1036
#define IDS_ErrNamedPipe                 1037
#define IDS_ErrStartDaemon               1038
#define IDS_ErrStartPipe                 1039
#define IDS_ErrStartSocket               1040
#define IDS_CronStarts                   1041
#define IDS_CronEnds                     1042

#define IDS_StatusLine                   1056
#define IDS_NextstartLine                1057
#define IDS_NextstartUnknown             1058
#define IDS_CurrCrontabsLine             1059
#define IDS_CurrOutputLine               1060
#define IDS_EnterValidOutput             1061
#define IDS_InvalidFormat                1062
#define IDS_NoTCPIPFound                 1063
#define IDS_TitleNewCron                 1064
#define IDS_TitleNewTCPIP                1065
#define IDS_HelpCantLoad                 1066
#define IDS_JobJustDeleted               1067
#define IDS_EntryInClipboard             1068

#define IDS_ThisIsTheJobList             1072
#define IDS_NoJobList                    1073
#define IDS_JobDeletedNotWritten         1074
#define IDS_JobDeleted                   1075
#define IDS_JobSavedNotWritten           1076
#define IDS_JobSaved                     1077
#define IDS_AutoCloseingPipe             1078
#define IDS_AutoCloseingSocket           1079

#define IDS_0ByteCommunication           1088
#define IDS_JobNotFound                  1089
#define IDS_ErrRequest                   1090
#define IDS_ErrRequestIncomplete         1091
#define IDS_ErrList                      1092
#define IDS_ErrListIncomplete            1093

#define IDS_TempfileNotDeleted           1104
#define IDS_JobEndsNormal                1105
#define IDS_JobEndsDueSignal             1106
#define IDS_TempfileInaccessable         1107
#define IDS_TempfileEmpty                1108
#define IDS_TempfileFollows              1109
#define IDS_NoComSpec                    1110
#define IDS_NoMemOrNoTempfile            1111
#define IDS_CantStartJob                 1112
#define IDS_JobStarted                   1113
#define IDS_JobYetActive                 1114

#define IDS_UnknownHost                  1120
#define IDS_NameServerProblem            1121
#define IDS_TCPIPConnection              1122
#define IDS_TCPIPRejected                1123

#define IDS_lastnumber_mustbe_last_IDS   1135

/* GERMAN has a 10000th-digit of 1 */
#define ID_CRON_G                       10100

#define IDME_NEXTFILE_G                 10101
#define IDME_HELPINDEX_G                10102
#define IDME_HELPGENERAL_G              10103
#define IDME_HELPUSINGHELP_G            10104
#define IDME_HELPKEYS_G                 10105

#define IDD_NOTEBOOK_G                  10200
#define IDDE_NOTEBOOK_PAGES_G           10201

#define IDD_CRON_FILE_G                 10300
#define IDDE_CRON_ATSTARTUP_G           10301
#define IDDE_CRON_ATEXIT_G              10302
#define IDDE_CRON_FILENAME_G            10303
#define IDDE_CRON_FILENAMECHANGE_G      10304
#define IDDE_CRON_HELP_G                10305

#define IDD_CRONTABS_G                  10310
#define IDDE_CRONTABS_LIST_G            10311
#define IDDE_CRONTABS_ENTRY_G           10312
#define IDDE_CRONTABS_ADD_G             10313
#define IDDE_CRONTABS_DELETE_G          10314
#define IDDE_CRONTABS_EDIT_G            10315
#define IDDE_CRONTABS_CHANGE_G          10316
#define IDDE_CRONTABS_ABORT_G           10317
#define IDDE_CRONTABS_HELP_G            10318

#define IDD_OUTPUT_G                    10400
#define IDDE_OUTPUT_THRES_G             10401
#define IDDE_OUTPUT_COUNT_G             10402
#define IDDE_OUTPUT_PATH_G              10403
#define IDDE_OUTPUT_PATHCHANGE_G        10404
#define IDDE_OUTPUT_IGNORE_OUTPUT_G     10405
#define IDDE_OUTPUT_HELP_G              10406

#define IDD_OUTPUT_PATH_G               10410
#define IDDE_OUTPUT_PATH_ENTRY_G        10411
#define IDDE_OUTPUT_PATH_LIST_G         10412

#define IDD_TCP_G                       10500
#define IDDE_TCP_FILENAME_G             10501
#define IDDE_TCP_FILENAMECHANGE_G       10502
#define IDDE_TCP_ALLOWACCESS_G          10503
#define IDDE_TCP_HELP_G                 10504

#define IDD_GENERAL_G                   10600
#define IDDE_GENERAL_LANGUAGE_G         10601
#define IDDE_GENERAL_LANGUAGE_WARNING_G 10602
#define IDDE_GENERAL_HELP_G             10603

#define IDD_ABOUT_G                     10900

#define IDS_Title_G                     11008
#define IDS_NotEnoughMemory_G           11009
#define IDS_NoOutputFile_G              11010
#define IDS_StatusAtStartup_G           11011
#define IDS_StatusAtExit_G              11012
#define IDS_StatusNormal_G              11013
#define IDS_StatusCommunication_G       11014
#define IDS_DieOnSigSig_G               11015

#define IDS_NoCrontabsFile_G            11024
#define IDS_ErrCrontabsFile_G           11025
#define IDS_DefaultCrontabsFile_G       11026
#define IDS_DefaultOutput_G             11027
#define IDS_NoTCPIPFile_G               11028
#define IDS_DefaultTCPIPFile_G          11029
#define IDS_ErrInitialize_G             11030
#define IDS_ErrMsgQueue_G               11031
#define IDS_ErrReassignStdIO_G          11032
#define IDS_AlreadyStarted_G            11033
#define IDS_CantCallMyself_G            11034
#define IDS_ErrCreateSem_G              11035
#define IDS_ErrQMod_G                   11036
#define IDS_ErrNamedPipe_G              11037
#define IDS_ErrStartDaemon_G            11038
#define IDS_ErrStartPipe_G              11039
#define IDS_ErrStartSocket_G            11040
#define IDS_CronStarts_G                11041
#define IDS_CronEnds_G                  11042

#define IDS_StatusLine_G                11056
#define IDS_NextstartLine_G             11057
#define IDS_NextstartUnknown_G          11058
#define IDS_CurrCrontabsLine_G          11059
#define IDS_CurrOutputLine_G            11060
#define IDS_EnterValidOutput_G          11061
#define IDS_InvalidFormat_G             11062
#define IDS_NoTCPIPFound_G              11063
#define IDS_TitleNewCron_G              11064
#define IDS_TitleNewTCPIP_G             11065
#define IDS_HelpCantLoad_G              11066
#define IDS_JobJustDeleted_G            11067
#define IDS_EntryInClipboard_G          11068

#define IDS_ThisIsTheJobList_G          11072
#define IDS_NoJobList_G                 11073
#define IDS_JobDeletedNotWritten_G      11074
#define IDS_JobDeleted_G                11075
#define IDS_JobSavedNotWritten_G        11076
#define IDS_JobSaved_G                  11077
#define IDS_AutoCloseingPipe_G          11078
#define IDS_AutoCloseingSocket_G        11079

#define IDS_0ByteCommunication_G        11088
#define IDS_JobNotFound_G               11089
#define IDS_ErrRequest_G                11090
#define IDS_ErrRequestIncomplete_G      11091
#define IDS_ErrList_G                   11092
#define IDS_ErrListIncomplete_G         11093

#define IDS_TempfileNotDeleted_G        11104
#define IDS_JobEndsNormal_G             11105
#define IDS_JobEndsDueSignal_G          11106
#define IDS_TempfileInaccessable_G      11107
#define IDS_TempfileEmpty_G             11108
#define IDS_TempfileFollows_G           11109
#define IDS_NoComSpec_G                 11110
#define IDS_NoMemOrNoTempfile_G         11111
#define IDS_CantStartJob_G              11112
#define IDS_JobStarted_G                11113
#define IDS_JobYetActive_G              11114

#define IDS_UnknownHost_G               11120
#define IDS_NameServerProblem_G         11121
#define IDS_TCPIPConnection_G           11122
#define IDS_TCPIPRejected_G             11123

/* FRENCH has a 10000th-digit of 2 */
#define ID_CRON_F                       20100

#define IDME_NEXTFILE_F                 20101
#define IDME_HELPINDEX_F                20102
#define IDME_HELPGENERAL_F              20103
#define IDME_HELPUSINGHELP_F            20104
#define IDME_HELPKEYS_F                 20105

#define IDD_NOTEBOOK_F                  20200
#define IDDE_NOTEBOOK_PAGES_F           20201

#define IDD_CRON_FILE_F                 20300
#define IDDE_CRON_ATSTARTUP_F           20301
#define IDDE_CRON_ATEXIT_F              20302
#define IDDE_CRON_FILENAME_F            20303
#define IDDE_CRON_FILENAMECHANGE_F      20304
#define IDDE_CRON_HELP_F                20305

#define IDD_CRONTABS_F                  20310
#define IDDE_CRONTABS_LIST_F            20311
#define IDDE_CRONTABS_ENTRY_F           20312
#define IDDE_CRONTABS_ADD_F             20313
#define IDDE_CRONTABS_DELETE_F          20314
#define IDDE_CRONTABS_EDIT_F            20315
#define IDDE_CRONTABS_CHANGE_F          20316
#define IDDE_CRONTABS_ABORT_F           20317
#define IDDE_CRONTABS_HELP_F            20318

#define IDD_OUTPUT_F                    20400
#define IDDE_OUTPUT_THRES_F             20401
#define IDDE_OUTPUT_COUNT_F             20402
#define IDDE_OUTPUT_PATH_F              20403
#define IDDE_OUTPUT_PATHCHANGE_F        20404
#define IDDE_OUTPUT_IGNORE_OUTPUT_F     20405
#define IDDE_OUTPUT_HELP_F              20406

#define IDD_OUTPUT_PATH_F               20410
#define IDDE_OUTPUT_PATH_ENTRY_F        20411
#define IDDE_OUTPUT_PATH_LIST_F         20412

#define IDD_TCP_F                       20500
#define IDDE_TCP_FILENAME_F             20501
#define IDDE_TCP_FILENAMECHANGE_F       20502
#define IDDE_TCP_ALLOWACCESS_F          20503
#define IDDE_TCP_HELP_F                 20504

#define IDD_GENERAL_F                   20600
#define IDDE_GENERAL_LANGUAGE_F         20601
#define IDDE_GENERAL_LANGUAGE_WARNING_F 20602
#define IDDE_GENERAL_HELP_F             20603

#define IDD_ABOUT_F                     20900

#define IDS_Title_F                     21008
#define IDS_NotEnoughMemory_F           21009
#define IDS_NoOutputFile_F              21010
#define IDS_StatusAtStartup_F           21011
#define IDS_StatusAtExit_F              21012
#define IDS_StatusNormal_F              21013
#define IDS_StatusCommunication_F       21014
#define IDS_DieOnSigSig_F               21015

#define IDS_NoCrontabsFile_F            21024
#define IDS_ErrCrontabsFile_F           21025
#define IDS_DefaultCrontabsFile_F       21026
#define IDS_DefaultOutput_F             21027
#define IDS_NoTCPIPFile_F               21028
#define IDS_DefaultTCPIPFile_F          21029
#define IDS_ErrInitialize_F             21030
#define IDS_ErrMsgQueue_F               21031
#define IDS_ErrReassignStdIO_F          21032
#define IDS_AlreadyStarted_F            21033
#define IDS_CantCallMyself_F            21034
#define IDS_ErrCreateSem_F              21035
#define IDS_ErrQMod_F                   21036
#define IDS_ErrNamedPipe_F              21037
#define IDS_ErrStartDaemon_F            21038
#define IDS_ErrStartPipe_F              21039
#define IDS_ErrStartSocket_F            21040
#define IDS_CronStarts_F                21041
#define IDS_CronEnds_F                  21042

#define IDS_StatusLine_F                21056
#define IDS_NextstartLine_F             21057
#define IDS_NextstartUnknown_F          21058
#define IDS_CurrCrontabsLine_F          21059
#define IDS_CurrOutputLine_F            21060
#define IDS_EnterValidOutput_F          21061
#define IDS_InvalidFormat_F             21062
#define IDS_NoTCPIPFound_F              21063
#define IDS_TitleNewCron_F              21064
#define IDS_TitleNewTCPIP_F             21065
#define IDS_HelpCantLoad_F              21066
#define IDS_JobJustDeleted_F            21067
#define IDS_EntryInClipboard_F          21068

#define IDS_ThisIsTheJobList_F          21072
#define IDS_NoJobList_F                 21073
#define IDS_JobDeletedNotWritten_F      21074
#define IDS_JobDeleted_F                21075
#define IDS_JobSavedNotWritten_F        21076
#define IDS_JobSaved_F                  21077
#define IDS_AutoCloseingPipe_F          21078
#define IDS_AutoCloseingSocket_F        21079

#define IDS_0ByteCommunication_F        21088
#define IDS_JobNotFound_F               21089
#define IDS_ErrRequest_F                21090
#define IDS_ErrRequestIncomplete_F      21091
#define IDS_ErrList_F                   21092
#define IDS_ErrListIncomplete_F         21093

#define IDS_TempfileNotDeleted_F        21104
#define IDS_JobEndsNormal_F             21105
#define IDS_JobEndsDueSignal_F          21106
#define IDS_TempfileInaccessable_F      21107
#define IDS_TempfileEmpty_F             21108
#define IDS_TempfileFollows_F           21109
#define IDS_NoComSpec_F                 21110
#define IDS_NoMemOrNoTempfile_F         21111
#define IDS_CantStartJob_F              21112
#define IDS_JobStarted_F                21113
#define IDS_JobYetActive_F              21114

#define IDS_UnknownHost_F               21120
#define IDS_NameServerProblem_F         21121
#define IDS_TCPIPConnection_F           21122
#define IDS_TCPIPRejected_F             21123
