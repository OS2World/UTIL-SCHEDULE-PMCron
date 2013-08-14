/* Copyright (c) 1995 Florian Groáe-Coosmann, RCS section at the eof         */
/* This is the PM frontend. This frontend should be preferred.               */
/* Look to the function ClientWndProc at the very bottom of the module.      */
/* It is the standard user interface.                                        */
/* Many interesting functions belonging to this frontend are placed in the   */
/* module cronmain.c, since they are shared with the cron_det.c module.      */
/*                                                                           */
/* FIXME: We are using Presentation Parameters (PresParam) to change and set */
/*        the current font. This isn't the right way I think. In the function*/
/*        CheckMetric the metric claims about a system proportional (e.g.)   */
/*        but GpiQueryLogicalFont persists in a courier font. That's wrong.  */
/*        Therefore, there must be an error in the GPI or the functions      */
/*        uses different sources of their output (I think they do so).       */
/*        Thus, we can't use logical fonts to do the three things together:  */
/*        1) Figuring out the current font                                   */
/*        2) Setting the new font                                            */
/*        3) Allow of a font setting by an external font manager (must work) */
/*        Are there any gurus which can resolve the problems? Ideas may help,*/
/*        too.  Florian                                                      */
#define INCL_NOCOMMON
#define INCL_DOSFILEMGR
#define INCL_DOSRESOURCES
#define INCL_DOSMEMMGR
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_WINDIALOGS
#define INCL_WINMESSAGEMGR
#define INCL_WINWINDOWMGR
#define INCL_WINERRORS
#define INCL_WINSYS
#define INCL_WINLISTBOXES
#define INCL_WINENTRYFIELDS
#define INCL_WINBUTTONS
#define INCL_WINSTDFILE
#define INCL_WINSTDBOOK
#define INCL_WINFRAMEMGR
#define INCL_WINMENUS
#define INCL_WININPUT
#define INCL_WINHELP
#define INCL_WINCLIPBOARD
#define INCL_GPILOGCOLORTABLE

#define INCL_DOSSEMAPHORES

#include <os2.h>

#include <sys/types.h>
#include <sys/timeb.h>
#include <dirent.h>
#include <ctype.h>
#include <fnmatch.h>
#include <io.h>
#include <string.h>
#include <signal.h>
#include "server.h"

static LONG  LineStart;                 /* font depenging values, see        */
static LONG  FontHeight,FontBase;       /* CheckMetric()                     */
static ULONG CurrentState = IDS_StatusAtStartup;   /* buffered states of     */
static time_t NextStartTime = (time_t) -1;   /* the program                  */
static HWND  CrontabsWindow = (HWND) 0; /* identifies the crontabs entries   */
                                        /* window                            */
static HWND hwndHelpInstance = (HWND) 0;  /* identifies the help window      */
static COLOR Foreground = CLR_NEUTRAL,Background = CLR_BACKGROUND;
                                        /* Colors                            */

/*****************************************************************************/
/*  function name : PMError                                                  */
/*                                                                           */
/*  arguments     : string describing an error, owner of the error window    */
/*                                                                           */
/*  description   : prints the error string to the desktop. If the owner     */
/*                  is not set (owner == (HWND) 0) either the hwndFrame or   */
/*                  the HWND_DESKTOP is used (depends on the existence if    */
/*                  hwndFrame)                                               */
/*****************************************************************************/
void PMError(const char *s,HWND owner)
{
   if (owner == (HWND) 0)
      owner = (hwndFrame != (HWND) 0) ? hwndFrame : HWND_DESKTOP;

   WinMessageBox(HWND_DESKTOP,
                 owner,
                 s,
                 Get(IDS_Title),
                 0,
                 MB_OK | MB_ICONHAND | MB_APPLMODAL);
}

/*****************************************************************************/
/*  function name : PMFatalError                                             */
/*                                                                           */
/*  arguments     : see Error.                                               */
/*                                                                           */
/*  description   : see Error but terminates the program.                    */
/*****************************************************************************/
void PMFatalError(const char *s,HWND owner)
{
   Error(s,owner);
   WinDestroyMsgQueue(hmq);             /* clean up used resources           */
   WinTerminate(hab);
   exit(3);
}

/*****************************************************************************/
/*  function name : WinError                                                 */
/*                                                                           */
/*  arguments     : owner of the error window                                */
/*                                                                           */
/*  description   : figures out the description of the latest error that     */
/*                  relates to the GPI and prints it to the desktop.         */
/*                  If the owner is not set (owner == (HWND) 0) either       */
/*                  the hwndFrame or the HWND_DESKTOP is used (depends       */
/*                  on the existence if hwndFrame)                           */
/*****************************************************************************/
void WinError(HWND owner)
{
   PERRINFO perr;
   static PCSZ pcsz = "unknown error";
   PULONG pul;

   if (owner == (HWND) 0)
      owner = (hwndFrame != (HWND) 0) ? hwndFrame : HWND_DESKTOP;
   if ((perr = WinGetErrorInfo(hab)) != NULL) {
      pul = (PULONG) ((PSZ) perr + perr->offaoffszMsg);  /* only get the 1st */
      pcsz = (PCSZ) perr + *pul;        /* description, I've never seen more */
   }

   WinMessageBox(HWND_DESKTOP,
                 owner,
                 pcsz,
                 Get(IDS_Title),
                 0,
                 MB_OK | MB_ICONHAND | MB_APPLMODAL);
   if (perr != NULL)
      WinFreeErrorInfo(perr);
}

/*****************************************************************************/
/*  function name : FatalWinError                                            */
/*                                                                           */
/*  arguments     : see WinError.                                            */
/*                                                                           */
/*  description   : see WinError but terminates the program.                 */
/*****************************************************************************/
void FatalWinError(HWND owner)
{
   WinError(owner);
   WinDestroyMsgQueue (hmq);
   WinTerminate (hab);
   exit(3);
}

#ifdef DEBUG
void DumpMessage(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   static struct { USHORT number; char *name; } msgs[] = {
      {0x0000, "WM_NULL"                      },
      {0x0001, "WM_CREATE"                    },
      {0x0002, "WM_DESTROY"                   },
      {0x0004, "WM_ENABLE"                    },
      {0x0005, "WM_SHOW"                      },
      {0x0006, "WM_MOVE"                      },
      {0x0007, "WM_SIZE"                      },
      {0x0008, "WM_ADJUSTWINDOWPOS"           },
      {0x0009, "WM_CALCVALIDRECTS"            },
      {0x000a, "WM_SETWINDOWPARAMS"           },
      {0x000b, "WM_QUERYWINDOWPARAMS"         },
      {0x000c, "WM_HITTEST"                   },
      {0x000d, "WM_ACTIVATE"                  },
      {0x000f, "WM_SETFOCUS"                  },
      {0x0010, "WM_SETSELECTION"              },
      {0x0011, "WM_PPAINT"                    },
      {0x0012, "WM_PSETFOCUS"                 },
      {0x0013, "WM_PSYSCOLORCHANGE"           },
      {0x0014, "WM_PSIZE"                     },
      {0x0015, "WM_PACTIVATE"                 },
      {0x0016, "WM_PCONTROL"                  },
      {0x0020, "WM_COMMAND"                   },
      {0x0021, "WM_SYSCOMMAND"                },
      {0x0022, "WM_HELP"                      },
      {0x0023, "WM_PAINT"                     },
      {0x0024, "WM_TIMER"                     },
      {0x0025, "WM_SEM1"                      },
      {0x0026, "WM_SEM2"                      },
      {0x0027, "WM_SEM3"                      },
      {0x0028, "WM_SEM4"                      },
      {0x0029, "WM_CLOSE"                     },
      {0x002a, "WM_QUIT"                      },
      {0x002b, "WM_SYSCOLORCHANGE"            },
      {0x002d, "WM_SYSVALUECHANGED"           },
      {0x002e, "WM_APPTERMINATENOTIFY"        },
      {0x002f, "WM_PRESPARAMCHANGED"          },
      {0x0030, "WM_CONTROL"                   },
      {0x0031, "WM_VSCROLL"                   },
      {0x0032, "WM_HSCROLL"                   },
      {0x0033, "WM_INITMENU"                  },
      {0x0034, "WM_MENUSELECT"                },
      {0x0035, "WM_MENUEND"                   },
      {0x0036, "WM_DRAWITEM"                  },
      {0x0037, "WM_MEASUREITEM"               },
      {0x0038, "WM_CONTROLPOINTER"            },
      {0x003a, "WM_QUERYDLGCODE"              },
      {0x003b, "WM_INITDLG"                   },
      {0x003c, "WM_SUBSTITUTESTRING"          },
      {0x003d, "WM_MATCHMNEMONIC"             },
      {0x003e, "WM_SAVEAPPLICATION"           },
      {0x0040, "WM_FLASHWINDOW"               },
      {0x0041, "WM_FORMATFRAME"               },
      {0x0042, "WM_UPDATEFRAME"               },
      {0x0043, "WM_FOCUSCHANGE"               },
      {0x0044, "WM_SETBORDERSIZE"             },
      {0x0045, "WM_TRACKFRAME"                },
      {0x0046, "WM_MINMAXFRAME"               },
      {0x0047, "WM_SETICON"                   },
      {0x0048, "WM_QUERYICON"                 },
      {0x0049, "WM_SETACCELTABLE"             },
      {0x004a, "WM_QUERYACCELTABLE"           },
      {0x004b, "WM_TRANSLATEACCEL"            },
      {0x004c, "WM_QUERYTRACKINFO"            },
      {0x004d, "WM_QUERYBORDERSIZE"           },
      {0x004e, "WM_NEXTMENU"                  },
      {0x004f, "WM_ERASEBACKGROUND"           },
      {0x0050, "WM_QUERYFRAMEINFO"            },
      {0x0051, "WM_QUERYFOCUSCHAIN"           },
      {0x0052, "WM_OWNERPOSCHANGE"            },
      {0x0053, "WM_CALCFRAMERECT"             },
      {0x0055, "WM_WINDOWPOSCHANGED"          },
      {0x0056, "WM_ADJUSTFRAMEPOS"            },
      {0x0059, "WM_QUERYFRAMECTLCOUNT"        },
      {0x005B, "WM_QUERYHELPINFO"             },
      {0x005C, "WM_SETHELPINFO"               },
      {0x005D, "WM_ERROR"                     },
      {0x005E, "WM_REALIZEPALETTE"            },
      {0x0060, "WM_RENDERFMT"                 },
      {0x0061, "WM_RENDERALLFMTS"             },
      {0x0062, "WM_DESTROYCLIPBOARD"          },
      {0x0063, "WM_PAINTCLIPBOARD"            },
      {0x0064, "WM_SIZECLIPBOARD"             },
      {0x0065, "WM_HSCROLLCLIPBOARD"          },
      {0x0066, "WM_VSCROLLCLIPBOARD"          },
      {0x0067, "WM_DRAWCLIPBOARD"             },
      {0x0070, "WM_MOUSEMOVE"                 },
      {0x0071, "WM_BUTTON1DOWN"               },
      {0x0072, "WM_BUTTON1UP"                 },
      {0x0073, "WM_BUTTON1DBLCLK"             },
      {0x0074, "WM_BUTTON2DOWN"               },
      {0x0075, "WM_BUTTON2UP"                 },
      {0x0076, "WM_BUTTON2DBLCLK"             },
      {0x0077, "WM_BUTTON3DOWN"               },
      {0x0078, "WM_BUTTON3UP"                 },
      {0x0079, "WM_BUTTON3DBLCLK"             },
      {0x007a, "WM_CHAR"                      },
      {0x007b, "WM_VIOCHAR"                   },
      {0x007c, "WM_JOURNALNOTIFY"             },
      {0x007D, "WM_MOUSEMAP"                  },
      {0x00A0, "WM_DDE_INITIATE"              },
      {0x00A1, "WM_DDE_REQUEST"               },
      {0x00A2, "WM_DDE_ACK"                   },
      {0x00A3, "WM_DDE_DATA"                  },
      {0x00A4, "WM_DDE_ADVISE"                },
      {0x00A5, "WM_DDE_UNADVISE"              },
      {0x00A6, "WM_DDE_POKE"                  },
      {0x00A7, "WM_DDE_EXECUTE"               },
      {0x00A8, "WM_DDE_TERMINATE"             },
      {0x00A9, "WM_DDE_INITIATEACK"           },
      {0x00AF, "WM_DDE_LAST"                  },
      {0x00b0, "WM_DBCSFIRST"                 },
      {0x00cf, "WM_DBCSLAST"                  },
      {0x0100, "SM_SETHANDLE"                 },
      {0x0101, "SM_QUERYHANDLE"               },
      {0x0120, "BM_CLICK"                     },
      {0x0121, "BM_QUERYCHECKINDEX"           },
      {0x0122, "BM_QUERYHILITE"               },
      {0x0123, "BM_SETHILITE"                 },
      {0x0124, "BM_QUERYCHECK"                },
      {0x0125, "BM_SETCHECK"                  },
      {0x0126, "BM_SETDEFAULT"                },
      {0x0140, "EM_QUERYCHANGED"              },
      {0x0141, "EM_QUERYSEL"                  },
      {0x0142, "EM_SETSEL"                    },
      {0x0143, "EM_SETTEXTLIMIT"              },
      {0x0144, "EM_CUT"                       },
      {0x0145, "EM_COPY"                      },
      {0x0146, "EM_CLEAR"                     },
      {0x0147, "EM_PASTE"                     },
      {0x0148, "EM_QUERYFIRSTCHAR"            },
      {0x0149, "EM_SETFIRSTCHAR"              },
      {0x014a, "EM_QUERYREADONLY"             },
      {0x014b, "EM_SETREADONLY"               },
      {0x014c, "EM_SETINSERTMODE"             },
      {0x0160, "LM_QUERYITEMCOUNT"            },
      {0x0161, "LM_INSERTITEM"                },
      {0x0162, "LM_SETTOPINDEX"               },
      {0x0163, "LM_DELETEITEM"                },
      {0x0164, "LM_SELECTITEM"                },
      {0x0165, "LM_QUERYSELECTION"            },
      {0x0166, "LM_SETITEMTEXT"               },
      {0x0167, "LM_QUERYITEMTEXTLENGTH"       },
      {0x0168, "LM_QUERYITEMTEXT"             },
      {0x0169, "LM_SETITEMHANDLE"             },
      {0x016a, "LM_QUERYITEMHANDLE"           },
      {0x016b, "LM_SEARCHSTRING"              },
      {0x016c, "LM_SETITEMHEIGHT"             },
      {0x016d, "LM_QUERYTOPINDEX"             },
      {0x016e, "LM_DELETEALL"                 },
      {0x0170, "CBM_SHOWLIST"                 },
      {0x0171, "CBM_HILITE"                   },
      {0x0172, "CBM_ISLISTSHOWING"            },
      {0x0180, "MM_INSERTITEM"                },
      {0x0181, "MM_DELETEITEM"                },
      {0x0182, "MM_QUERYITEM"                 },
      {0x0183, "MM_SETITEM"                   },
      {0x0184, "MM_QUERYITEMCOUNT"            },
      {0x0185, "MM_STARTMENUMODE"             },
      {0x0186, "MM_ENDMENUMODE"               },
      {0x0188, "MM_REMOVEITEM"                },
      {0x0189, "MM_SELECTITEM"                },
      {0x018a, "MM_QUERYSELITEMID"            },
      {0x018b, "MM_QUERYITEMTEXT"             },
      {0x018c, "MM_QUERYITEMTEXTLENGTH"       },
      {0x018d, "MM_SETITEMHANDLE"             },
      {0x018e, "MM_SETITEMTEXT"               },
      {0x018f, "MM_ITEMPOSITIONFROMID"        },
      {0x0190, "MM_ITEMIDFROMPOSITION"        },
      {0x0191, "MM_QUERYITEMATTR"             },
      {0x0192, "MM_SETITEMATTR"               },
      {0x0193, "MM_ISITEMVALID"               },
      {0x0194, "MM_QUERYITEMRECT"             },
      {0x01a0, "SBM_SETSCROLLBAR"             },
      {0x01a1, "SBM_SETPOS"                   },
      {0x01a2, "SBM_QUERYPOS"                 },
      {0x01a3, "SBM_QUERYRANGE"               },
      {0x01a6, "SBM_SETTHUMBSIZE"             },
      {0x01b0, "MLM_SETTEXTLIMIT"             },
      {0x01b1, "MLM_QUERYTEXTLIMIT"           },
      {0x01b2, "MLM_SETFORMATRECT"            },
      {0x01b3, "MLM_QUERYFORMATRECT"          },
      {0x01b4, "MLM_SETWRAP"                  },
      {0x01b5, "MLM_QUERYWRAP"                },
      {0x01b6, "MLM_SETTABSTOP"               },
      {0x01b7, "MLM_QUERYTABSTOP"             },
      {0x01b8, "MLM_SETREADONLY"              },
      {0x01b9, "MLM_QUERYREADONLY"            },
      {0x01ba, "MLM_QUERYCHANGED"             },
      {0x01bb, "MLM_SETCHANGED"               },
      {0x01bc, "MLM_QUERYLINECOUNT"           },
      {0x01bd, "MLM_CHARFROMLINE"             },
      {0x01be, "MLM_LINEFROMCHAR"             },
      {0x01bf, "MLM_QUERYLINELENGTH"          },
      {0x01c0, "MLM_QUERYTEXTLENGTH"          },
      {0x01c1, "MLM_FORMAT"                   },
      {0x01c2, "MLM_SETIMPORTEXPORT"          },
      {0x01c3, "MLM_IMPORT"                   },
      {0x01c4, "MLM_EXPORT"                   },
      {0x01c6, "MLM_DELETE"                   },
      {0x01c7, "MLM_QUERYFORMATLINELENGTH"    },
      {0x01c8, "MLM_QUERYFORMATTEXTLENGTH"    },
      {0x01c9, "MLM_INSERT"                   },
      {0x01ca, "MLM_SETSEL"                   },
      {0x01cb, "MLM_QUERYSEL"                 },
      {0x01cc, "MLM_QUERYSELTEXT"             },
      {0x01cd, "MLM_QUERYUNDO"                },
      {0x01ce, "MLM_UNDO"                     },
      {0x01cf, "MLM_RESETUNDO"                },
      {0x01d0, "MLM_QUERYFONT"                },
      {0x01d1, "MLM_SETFONT"                  },
      {0x01d2, "MLM_SETTEXTCOLOR"             },
      {0x01d3, "MLM_QUERYTEXTCOLOR"           },
      {0x01d4, "MLM_SETBACKCOLOR"             },
      {0x01d5, "MLM_QUERYBACKCOLOR"           },
      {0x01d6, "MLM_QUERYFIRSTCHAR"           },
      {0x01d7, "MLM_SETFIRSTCHAR"             },
      {0x01d8, "MLM_CUT"                      },
      {0x01d9, "MLM_COPY"                     },
      {0x01da, "MLM_PASTE"                    },
      {0x01db, "MLM_CLEAR"                    },
      {0x01dc, "MLM_ENABLEREFRESH"            },
      {0x01dd, "MLM_DISABLEREFRESH"           },
      {0x01de, "MLM_SEARCH"                   },
      {0x01df, "MLM_QUERYIMPORTEXPORT"        },
      {0x01e3, "TBM_SETHILITE"                },
      {0x01e4, "TBM_QUERYHILITE"              },
      {0x0200, "SPBM_OVERRIDESETLIMITS"       },
      {0x0201, "SPBM_QUERYLIMITS"             },
      {0x0202, "SPBM_SETTEXTLIMIT"            },
      {0x0203, "SPBM_SPINUP"                  },
      {0x0204, "SPBM_SPINDOWN"                },
      {0x0205, "SPBM_QUERYVALUE"              },
      {0x0206, "SPBM_SETARRAY"                },
      {0x0207, "SPBM_SETLIMITS"               },
      {0x0208, "SPBM_SETCURRENTVALUE"         },
      {0x0209, "SPBM_SETMASTER"               },
      {0x0220, "HM_MSG_BASE"                  },
      {0x0221, "HM_DISMISS_WINDOW"            },
      {0x0222, "HM_DISPLAY_HELP"              },
      {0x0223, "HM_GENERAL_HELP(EXT_HELP)"    },
      {0x0224, "HM_SET_ACTIVE_WINDOW"         },
      {0x0225, "HM_LOAD_HELP_TABLE"           },
      {0x0226, "HM_CREATE_HELP_TABLE"         },
      {0x0227, "HM_SET_HELP_WINDOW_TITLE"     },
      {0x0228, "HM_SET_SHOW_PANEL_ID"         },
      {0x0229, "HM_REPLACE_USING_HELP"        },
      {0x022a, "HM_HELP_INDEX"                },
      {0x022b, "HM_HELP_CONTENTS"             },
      {0x022c, "HM_KEYS_HELP"                 },
      {0x022d, "HM_SET_HELP_LIBRARY_NAME"     },
      {0x022e, "HM_ERROR"                     },
      {0x022f, "HM_HELPSUBITEM_NOT_FOUND"     },
      {0x0230, "HM_QUERY_KEYS_HELP"           },
      {0x0231, "HM_TUTORIAL"                  },
      {0x0232, "HM_EXT_HELP_UNDEFINED"        },
      {0x0233, "HM_ACTIONBAR_COMMAND"         },
      {0x0234, "HM_INFORM"                    },
      {0x0238, "HM_SET_OBJCOM_WINDOW"         },
      {0x0239, "HM_UPDATE_OBJCOM_WINDOW_CHAIN"},
      {0x023a, "HM_QUERY_DDF_DATA"            },
      {0x023b, "HM_INVALIDATE_DDF_DATA"       },
      {0x023c, "HM_QUERY"                     },
      {0x023d, "HM_SET_COVERPAGE_SIZE"        },
      {0x0242, "HM_NOTIFY"                    },
      {0x0243, "HM_SET_USERDATA"              },
      {0x0244, "HM_CONTROL"                   },
      {0x0310, "WM_DRAGFIRST"                 },
      {0x031f, "DM_DISCARDOBJECT"             },
      {0x0320, "DM_PRINTOBJECT"               },
      {0x0321, "DM_DRAGOVERNOTIFY"            },
      {0x0322, "DM_RENDERFILE"                },
      {0x0323, "DM_FILERENDERED"              },
      {0x0324, "DM_DRAGERROR"                 },
      {0x0325, "DM_EMPHASIZETARGET"           },
      {0x0326, "DM_DRAGFILECOMPLETE"          },
      {0x0327, "DM_RENDERPREPARE"             },
      {0x0328, "DM_RENDERCOMPLETE"            },
      {0x0329, "DM_RENDER"                    },
      {0x032a, "DM_PRINT"                     },
      {0x032b, "DM_ENDCONVERSATION"           },
      {0x032c, "DM_DROPHELP"                  },
      {0x032d, "DM_DRAGLEAVE"                 },
      {0x032e, "DM_DRAGOVER"                  },
      {0x032f, "DM_DROP"                      },
      {0x0330, "CM_ALLOCDETAILFIELDINFO"      },
      {0x0331, "CM_ALLOCRECORD"               },
      {0x0332, "CM_ARRANGE"                   },
      {0x0333, "CM_ERASERECORD"               },
      {0x0334, "CM_FILTER"                    },
      {0x0335, "CM_FREEDETAILFIELDINFO"       },
      {0x0336, "CM_FREERECORD"                },
      {0x0337, "CM_HORZSCROLLSPLITWINDOW"     },
      {0x0338, "CM_INSERTDETAILFIELDINFO"     },
      {0x0339, "CM_INSERTRECORD"              },
      {0x033a, "CM_INVALIDATEDETAILFIELDINFO" },
      {0x033b, "CM_INVALIDATERECORD"          },
      {0x033c, "CM_PAINTBACKGROUND"           },
      {0x033d, "CM_QUERYCNRINFO"              },
      {0x033e, "CM_QUERYDETAILFIELDINFO"      },
      {0x033f, "CM_QUERYDRAGIMAGE"            },
      {0x0340, "CM_QUERYRECORD"               },
      {0x0341, "CM_QUERYRECORDEMPHASIS"       },
      {0x0342, "CM_QUERYRECORDFROMRECT"       },
      {0x0343, "CM_QUERYRECORDRECT"           },
      {0x0344, "CM_QUERYVIEWPORTRECT"         },
      {0x0345, "CM_REMOVEDETAILFIELDINFO"     },
      {0x0346, "CM_REMOVERECORD"              },
      {0x0347, "CM_SCROLLWINDOW"              },
      {0x0348, "CM_SEARCHSTRING"              },
      {0x0349, "CM_SETCNRINFO"                },
      {0x034a, "CM_SETRECORDEMPHASIS"         },
      {0x034b, "CM_SORTRECORD"                },
      {0x034c, "CM_OPENEDIT"                  },
      {0x034d, "CM_CLOSEEDIT"                 },
      {0x034e, "CM_COLLAPSETREE"              },
      {0x034f, "CM_EXPANDTREE"                },
      {0x0350, "CM_QUERYRECORDINFO"           },
      {0x0353, "BKM_CALCPAGERECT"             },
      {0x0354, "BKM_DELETEPAGE"               },
      {0x0355, "BKM_INSERTPAGE"               },
      {0x0356, "BKM_INVALIDATETABS"           },
      {0x0357, "BKM_TURNTOPAGE"               },
      {0x0358, "BKM_QUERYPAGECOUNT"           },
      {0x0359, "BKM_QUERYPAGEID"              },
      {0x035a, "BKM_QUERYPAGEDATA"            },
      {0x035b, "BKM_QUERYPAGEWINDOWHWND"      },
      {0x035c, "BKM_QUERYTABBITMAP"           },
      {0x035d, "BKM_QUERYTABTEXT"             },
      {0x035e, "BKM_SETDIMENSIONS"            },
      {0x035f, "BKM_SETPAGEDATA"              },
      {0x0360, "BKM_SETPAGEWINDOWHWND"        },
      {0x0361, "BKM_SETSTATUSLINETEXT"        },
      {0x0362, "BKM_SETTABBITMAP"             },
      {0x0363, "BKM_SETTABTEXT"               },
      {0x0364, "BKM_SETNOTEBOOKCOLORS"        },
      {0x0365, "BKM_QUERYPAGESTYLE"           },
      {0x0366, "BKM_QUERYSTATUSLINETEXT"      },
      {0x0369, "SLM_ADDDETENT"                },
      {0x036a, "SLM_QUERYDETENTPOS"           },
      {0x036b, "SLM_QUERYSCALETEXT"           },
      {0x036c, "SLM_QUERYSLIDERINFO"          },
      {0x036d, "SLM_QUERYTICKPOS"             },
      {0x036e, "SLM_QUERYTICKSIZE"            },
      {0x036f, "SLM_REMOVEDETENT"             },
      {0x0370, "SLM_SETSCALETEXT"             },
      {0x0371, "SLM_SETSLIDERINFO"            },
      {0x0372, "SLM_SETTICKSIZE"              },
      {0x0375, "VM_QUERYITEM"                 },
      {0x0376, "VM_QUERYITEMATTR"             },
      {0x0377, "VM_QUERYMETRICS"              },
      {0x0378, "VM_QUERYSELECTEDITEM"         },
      {0x0379, "VM_SELECTITEM"                },
      {0x037a, "VM_SETITEM"                   },
      {0x037b, "VM_SETITEMATTR"               },
      {0x037c, "VM_SETMETRICS"                },
      {0x0410, "WM_CHORD"                     },
      {0x0411, "WM_BUTTON1MOTIONSTART"        },
      {0x0412, "WM_BUTTON1MOTIONEND"          },
      {0x0413, "WM_BUTTON1CLICK"              },
      {0x0414, "WM_BUTTON2MOTIONSTART"        },
      {0x0415, "WM_BUTTON2MOTIONEND"          },
      {0x0416, "WM_BUTTON2CLICK"              },
      {0x0417, "WM_BUTTON3MOTIONSTART"        },
      {0x0418, "WM_BUTTON3MOTIONEND"          },
      {0x0419, "WM_BUTTON3CLICK"              },
      {0x0420, "WM_BEGINDRAG"                 },
      {0x0421, "WM_ENDDRAG"                   },
      {0x0422, "WM_SINGLESELECT"              },
      {0x0423, "WM_OPEN"                      },
      {0x0424, "WM_CONTEXTMENU"               },
      {0x0425, "WM_CONTEXTHELP"               },
      {0x0426, "WM_TEXTEDIT"                  },
      {0x0427, "WM_BEGINSELECT"               },
      {0x0428, "WM_ENDSELECT"                 },
      {0x0431, "MM_QUERYDEFAULTITEMID"        },
      {0x0432, "MM_SETDEFAULTITEMID"          },
      {0x04C0, "WM_PENFIRST"                  },
      {0x04FF, "WM_PENLAST"                   },
      {0x0500, "WM_MMPMFIRST"                 },
      {0x05FF, "WM_MMPMLAST"                  },
      {0x0F00, "WM_HELPBASE"                  },
      {0x0FFF, "WM_HELPTOP"                   },
      {0x1000, "WM_USER"                      },
      {0x1028, "FDM_FILTER"                   },
      {0x1029, "FDM_VALIDATE"                 },
      {0x102A, "FDM_ERROR"                    },
      {0x1032, "FNTM_FACENAMECHANGED"         },
      {0x1033, "FNTM_POINTSIZECHANGED"        },
      {0x1034, "FNTM_STYLECHANGED"            },
      {0x1035, "FNTM_COLORCHANGED"            },
      {0x1036, "FNTM_UPDATEPREVIEW"           },
      {0x1037, "FNTM_FILTERLIST"              },
      {0xFFFF, "last possible message"        }
   };
   char *name,buf[10];
   struct timeb t;
   static int cnt = 0;
   int first = 0,last = (sizeof(msgs) / sizeof(msgs[0])) - 1,mid;
                                        /* we use int to detect underruns    */
   while (last - first > 0) {
      mid = first + (last - first) / 2;
      if ((USHORT) msg == msgs[mid].number) {
         last = first = mid;
         break;
      } else if ((USHORT) msg < msgs[mid].number)
         last = mid - 1;
      else
         first = mid + 1;
   }
   if ((last != first) || ((USHORT) msg != msgs[first].number))
      name = "unknown message";
   else
      name = msgs[first].name;
   if (cnt == 0)
      Dprintf("    Time    |  Window  | Msg  |  Param1  |  Param2  | Msg-Name\n");
   ftime(&t);
   strftime(buf,9,"%X",localtime(&t.time));
   Dprintf("%s,%02d | %08lX | %04lX | %08lX | %08lX | %s\n",
           buf,t.millitm / 10,
           (ULONG) hwnd,msg,(ULONG) mp1,(ULONG) mp2,name);
   if (cnt == 15)
      cnt = 0;
     else
      cnt++;
}
#endif

/*****************************************************************************/
/*  function name : SetPosOfWindow                                           */
/*                                                                           */
/*  arguments     : handle of the frame window                               */
/*                                                                           */
/*  description   : figures out the size and the position of the frame and   */
/*                  writes the position to the getenv(USER_INI) file.        */
/*****************************************************************************/
void SetPosOfWindow(HWND framewin)
{
   SWP swp;
   ULONG LocPrgFlags;
   if (WinQueryWindowPos(framewin,&swp)) {
      if ((swp.fl & (SWP_MINIMIZE | SWP_MAXIMIZE)) == 0) {
         FrameRect.xLeft   = swp.x;
         FrameRect.yBottom = swp.y;
         FrameRect.xRight  = swp.cx;
         FrameRect.yTop    = swp.cy;
         LocPrgFlags = 0;
      } else {                          /* if we are minimized or maximized  */
                                        /* we can't use the swp. Instead, we */
                                        /* ask the window for its restore    */
                                        /* values.                           */
         FrameRect.xLeft   = WinQueryWindowUShort(framewin,QWS_XRESTORE);
         FrameRect.yBottom = WinQueryWindowUShort(framewin,QWS_YRESTORE);
         FrameRect.xRight  = WinQueryWindowUShort(framewin,QWS_CXRESTORE);
         FrameRect.yTop    = WinQueryWindowUShort(framewin,QWS_CYRESTORE);
         if (swp.fl & SWP_MINIMIZE)
            LocPrgFlags = PRG_START_MINIMIZED;
         else
            LocPrgFlags = PRG_START_MAXIMIZED;
      }
                                        /* We don't want unneeded writes,    */
                                        /* thus we compare with the old pos. */
      if (memcmp(&FrameRect,&SaveFrameRect,sizeof(FrameRect)) != 0)
         ReWritePrfFlags |= REWRITE_SCREEN;
      if ((ProgramFlags & (PRG_START_MINIMIZED | PRG_START_MAXIMIZED)) !=
                                                                 LocPrgFlags) {
         ProgramFlags &= (ULONG) ~(PRG_START_MINIMIZED | PRG_START_MAXIMIZED);
         ProgramFlags |= LocPrgFlags;
         ReWritePrfFlags |= REWRITE_FLAGS;
      }
   }
   if (ReWritePrf())
      SaveFrameRect = FrameRect;
}

/*****************************************************************************/
/*  function name : CheckMetric                                              */
/*                                                                           */
/*  arguments     : handle of a presentation space                           */
/*                                                                           */
/*  description   : set the global variables used to write strings to the    */
/*                  client window. These vars depend on the used font.       */
/*                                                                           */
/*  note          : should be called very time the default font changes      */
/*****************************************************************************/
static void CheckMetric(HPS hps)
{
   FONTMETRICS metric;
   ULONG Pointsize,proof;
   static ULONG ScreenYResolution = 0;
   char LocFontname[sizeof(Fontname)];

   if (!GpiQueryFontMetrics(hps,
                            sizeof(metric),
                            &metric))
      FatalWinError((HWND) 0);

                                        /* first, compute the (new) font     */
                                        /* description string. It may have   */
                                        /* changed from outside.             */

   if (ScreenYResolution == 0) {        /* some outline fonts don't have the */
      if (!DevQueryCaps(GpiQueryDevice(hps), /* point size directly coded in */
                        CAPS_VERTICAL_FONT_RES, /* the sNominalPointsize.    */
                        1,                   /* We need this to compute the  */
                        &ScreenYResolution)) /* point size of the font.      */
                                        /* We could use CAPS_GRAPHICS_CHAR_- */
                                        /* HEIGHT, but that's not exact.     */
         ScreenYResolution = 120;       /* this is the default in case of    */
                                        /* 1024x768. The measurement is pels */
                                        /* per inch.                         */

   }

   if (metric.fsDefn & FM_DEFN_OUTLINE) {
                                        /*           EmHeight*PointsPerInch  */
                                        /* Points =  ----------------------  */
                                        /*                 PelsPerInch       */
      Pointsize = metric.lEmHeight * 72;  /* (one point is 1/72 inch) Better */
      Pointsize /= ScreenYResolution;   /* use two steps to avoid integer    */
                                        /* division problems                 */
      proof = Pointsize * ScreenYResolution;
      proof /= 72;
      if (proof != metric.lEmHeight)
         Pointsize++;                   /* there was a rounding problem      */
   } else                               /* with bitmap fonts the nominal     */
      Pointsize = metric.sNominalPointSize / 10;   /* point size is correct. */

   sprintf(LocFontname,"%lu.%s",Pointsize,metric.szFacename);
                                        /* we ignore Outline, Strikeout und  */
                                        /* Underscore!                       */
   if (strcmp(Fontname,LocFontname) != 0) {
      strcpy(Fontname,LocFontname);
      ReWritePrfFlags |= REWRITE_FONT;  /* ReWritePrf is done in RePaintAll  */
      ReWritePrf();
   }
                                        /* compute the values we need to run */

   LineStart  = metric.lAveCharWidth;   /* left margin                       */
                                        /* total height of on line           */
   FontHeight = metric.lMaxBaselineExt + metric.lInternalLeading +
                                                       metric.lExternalLeading;
   FontBase   = metric.lMaxDescender;   /* where to position strings in one  */
                                        /* line box                          */
}

/*****************************************************************************/
/*  function name : PaintLine                                                */
/*                                                                           */
/*  arguments     : line box to fill, handle of a presentation space,        */
/*                  number of the line, flag: line needs an overwrite        */
/*                                                                           */
/*  return value  : 0 if nothing has been paint (line out of display),       */
/*                  1 otherwise                                              */
/*                                                                           */
/*  description   : paints one of the (four?) lines of the client window.    */
/*                  The first argument describes the size and the position   */
/*                  of the line, the third the contents to use. If the       */
/*                  fourth is set, the line is filled with the default       */
/*                  background color before.                                 */
/*                  Valid line numbers are:                                  */
/*                                      (update this comment if changed!)    */
/*                  0: current state of program, changed by the function     */
/*                     NewProgramStatus                                      */
/*                  1: next job start time, changed by the function          */
/*                     NewStartTime                                          */
/*                  2: current Crontabs filename                             */
/*                  3: current output filename                               */
/*                                                                           */
/*  note          : clear is set if the calling function has already done    */
/*                  the "ClearBackground".                                   */
/*****************************************************************************/
static int PaintLine(RECTL *line, HPS hps, int lineno, int clear)
{
   POINTL ptl;
   PCSZ name,val;
   char buf[40];

   ptl.x = LineStart;
   ptl.y = line->yBottom + FontBase;
                                        /* now, we have to display the line. */
                                        /* Figure out the values:            */
   switch (lineno) {
      case 0:
         name = Get(IDS_StatusLine);
         val  = Get(CurrentState);
         break;

      case 1:
         name = Get(IDS_NextstartLine);
         if (NextStartTime == (time_t) -1)
            val = Get(IDS_NextstartUnknown);
         else {
            GetTime(buf,sizeof(buf),NextStartTime);
            val = buf;
         }
         break;

      case 2:
         name = Get(IDS_CurrCrontabsLine);
         val  = Crontabs;
         break;

      case 3:
         name = Get(IDS_CurrOutputLine);
         val  = CurrFilename;
         break;

      default:
         if (clear)
            WinFillRect(hps,line,CLR_BACKGROUND);
         return(0);
   }
                                        /* display the line                  */
   if (clear)
      WinFillRect(hps,line,CLR_BACKGROUND);
   GpiCharStringAt(hps,&ptl,strlen(name),(PSZ) name);
   GpiCharString(hps,strlen(val),(PSZ) val);
   return(1);
}

/*****************************************************************************/
/*  function name : CheckColor                                               */
/*                                                                           */
/*  arguments     : handle of a presentation space, presentation parameter   */
/*                  Id of color, system color Id of color, base number       */
/*                  of color within Colors                                   */
/*                                                                           */
/*  return value  : color with to use for displaying text (foreground or     */
/*                  background depending on parameters)                      */
/*                                                                           */
/*  description   : figures out which color to use. First, the system        */
/*                  color is checked. If it has changed, it will become      */
/*                  the new color and the corresponding presentation         */
/*                  parameter is deleted. If the presentation parameter      */
/*                  is set it will become the new color.                     */
/*                  Otherwise the sstored color from Colors will be          */
/*                  returned.                                                */
/*                  The ReWritePrfFlags variable is set in case of           */
/*                  changes.                                                 */
/*****************************************************************************/
static COLOR CheckColor(HPS hps,ULONG PresIndex,ULONG SysIndex,ULONG BaseIndex)
{
   COLOR clr;
   COLOR sysclr;
   ULONG dummy;
   int save = 0;
   clr =
   sysclr = WinQuerySysColor(HWND_DESKTOP,SysIndex,0);
   if (sysclr != Colors[BaseIndex]) {   /* The system color has changed!     */
      WinRemovePresParam(hwndClient,PresIndex); /* This overwrites a presen- */
      save = 1;                         /* tation parameter, delete it!      */
      clr = Colors[BaseIndex] = Colors[BaseIndex + 1] = sysclr;
   }
   else if (WinQueryPresParam(hwndClient,
                              PresIndex,
                              0,
                              &dummy,
                              sizeof(COLOR),
                              &clr,
                              /* QPF_PURERGBCOLOR */ 0) != sizeof(COLOR))
      clr = Colors[BaseIndex + 1];      /* use stored color                  */
   if (clr != Colors[BaseIndex + 1]) {  /* User defined color has changed!   */
      save = 1;
      Colors[BaseIndex + 1] = clr;
   }
   if (save)
      ReWritePrfFlags |= REWRITE_COLORS;
   return(clr);
}

/*****************************************************************************/
/*  function name : SwitchColors                                             */
/*                                                                           */
/*  arguments     : handle of a presentation space                           */
/*                                                                           */
/*  description   : sets the foreground and the background color of the      */
/*                  presentation space to Background and Foreground.         */
/*****************************************************************************/
static void SwitchColors(HPS hps)
{
   static LONG buf[4];
   buf[0] = CLR_BACKGROUND;             /* remap the default background color*/
   buf[1] = Background;                 /* to the right color                */
   buf[2] = CLR_NEUTRAL;                /* we won't remap the default color. */
   buf[3] = Foreground;                 /* Instead, we use the neutral color */
                                        /* for normal printing, but we take  */
                                        /* the default color as our default. */
   GpiCreateLogColorTable(hps,0,LCOLF_INDRGB,0,4,buf);
   GpiSetColor(hps,CLR_NEUTRAL);        /* switch to neutral as default color*/
}

/*****************************************************************************/
/*  function name : RePaintLine                                              */
/*                                                                           */
/*  arguments     : handle of the client window, number of the line to       */
/*                  update                                                   */
/*                                                                           */
/*  return value  : 0 if nothing has been paint (window out of display),     */
/*                  1 otherwise                                              */
/*                                                                           */
/*  description   : paints one of the lines of the client window. Prepares   */
/*                  the paint and calls PaintLine. (look there for a         */
/*                  description of any line).                                */
/*                                                                           */
/*  note          : never call on WM_PAINT, use RePaintAll instead! This     */
/*                  function is called by an user defined change of the      */
/*                  program status (WM_USER, see bottom)                     */
/*****************************************************************************/
static int RePaintLine(HWND hwnd,int lineno)
{
   HPS hps;
   RECTL rcl,rclline;
   int ret = 0;
   if ((hps = WinGetPS(hwnd)) == (HPS) 0)
      return(0);
   SwitchColors(hps);
   if (WinQueryWindowRect (hwnd, &rcl)) { /* compute the line                */
      rclline = rcl;
      rclline.yTop -= lineno * FontHeight;
      rclline.yBottom = rclline.yTop - FontHeight;
      rclline.yTop--;
      if (rclline.yTop >= rcl.yBottom)  /* displayable?                      */
         ret = PaintLine(&rclline, hps, lineno, 1);
   }
   WinReleasePS (hps);
   return(ret);
}

/*****************************************************************************/
/*  function name : RePaintAll                                               */
/*                                                                           */
/*  arguments     : handle of the client window, message number causing      */
/*                  the update                                               */
/*                                                                           */
/*  description   : paints all lines of the visible part of the client       */
/*                  window. Prepares the paint and calls PaintLine.          */
/*                  (look there for a description of any line).              */
/*                  The message number is checked to determine wether        */
/*                  called by a WM_PAINT. In this case we use other          */
/*                  techniques to get an PS.                                 */
/*****************************************************************************/
static void RePaintAll(HWND hwnd,ULONG msg)
{
   HPS hps;
   RECTL rcl,rclline;
   int i;
   static int first = 1;
   if (first) {
      first = 0;                        /* This MUST be placed first, be make*/
      if (Fontname[0] != '\0')          /* a recursive call!!!               */
         WinSetPresParam(hwnd,
                         PP_FONTNAMESIZE,
                         strlen(Fontname) + 1,
                         Fontname);
   }
   if (msg == WM_PAINT)                 /* don't use GetPS: endless recursion*/
      hps = WinBeginPaint(hwnd,NULLHANDLE,NULL);
   else
      hps = WinGetPS(hwnd);
   if (hps == (HPS) 0)                  /* can't get a PS?                   */
     return;

   Background = CheckColor(hps,         /* get default background color      */
                           PP_BACKGROUNDCOLOR,
                           SYSCLR_WINDOW,
                           MYCLR_SYSBACKGROUND);
   Foreground = CheckColor(hps,         /* get default foreground color      */
                           PP_FOREGROUNDCOLOR,
                           SYSCLR_WINDOWTEXT,
                           MYCLR_SYSFOREGROUND);
   SwitchColors(hps);

   CheckMetric(hps);                    /* figure out current font, may have */
                                        /* changed                           */
   ReWritePrf();
   if (!WinQueryWindowRect(hwnd,&rcl)) {  /* window invisible (minimized)?   */
      WinEndPaint(hps);
      return;
   }
   WinFillRect(hps,&rcl,CLR_BACKGROUND);  /* clear background                */
   rclline = rcl;
   rclline.yBottom = rclline.yTop - FontHeight; /* 0. line                   */
   rclline.yTop--;
   for (i = 0;rclline.yTop >= rcl.yBottom;i++) {
      if (!PaintLine(&rclline, hps, i, 0))   /* only some line my be dis-    */
         break;                         /* played ignore invisible lines     */
      rclline.yTop -= FontHeight;
      rclline.yBottom -= FontHeight;
   }

   if (msg == WM_PAINT)
      WinEndPaint(hps);                 /* counterpart of BeginPaint         */
   else
      WinReleasePS(hps);                /* counterpart of GetPS              */
}

/*****************************************************************************/
/*  function name : SetCheckBox                                              */
/*                                                                           */
/*  arguments     : handle of the dialog, mask (PRG_???-flag), id of the     */
/*                  dialog entry of the flag in the dialog                   */
/*                                                                           */
/*  description   : sets the dialog entry to checked/not checked depending   */
/*                  on the (ProgramFlags & mask) state.                      */
/*                                                                           */
/*  note          : if called directly, don't forget to write to the         */
/*                  INI-file if needed.                                      */
/*****************************************************************************/
static void SetCheckBox(HWND dlg,ULONG mask,USHORT id)
{
   USHORT flag = 0;                     /* 0 == button unchecked             */
   if (ProgramFlags & mask)
      flag = 1;                         /* 1 == button checked               */
   WinSendDlgItemMsg(dlg,
                     id,
                     BM_SETCHECK,
                     MPFROMSHORT(flag),
                     MPVOID);
}

/*****************************************************************************/
/*  function name : DestroyHelpInstance                                      */
/*                                                                           */
/*  description   : destroys the help window if it has been opened           */
/*****************************************************************************/
void DestroyHelpInstance(void)
{
   if (hwndHelpInstance != (HWND) 0)
      WinDestroyHelpInstance(hwndHelpInstance);
   hwndHelpInstance = (HWND) 0;
}

/*****************************************************************************/
/*  function name : InitHelp                                                 */
/*                                                                           */
/*  return value  : 1 on success, 0 in case of an error                      */
/*                                                                           */
/*  description   : initializes the help: the help file is opened and        */
/*                  associated to the main window. An error message is       */
/*                  displayed in case of an error.                           */
/*****************************************************************************/
int InitHelp(void)
{
   HELPINIT hini;
   char fn[CCHMAXPATH + 2];
   char basename[8+1+3+1];
   USHORT i;

   memset(&hini,0,sizeof(hini));
   hini.cb = sizeof(HELPINIT);
   hini.phtHelpTable = (PHELPTABLE) MAKELONG(NLS(ID_CRON),0xFFFF);
   hini.pszHelpWindowTitle = (char *) Get(IDS_Title);

#ifdef DEBUG
   hini.fShowPanelId = CMIC_SHOW_PANEL_ID;
#else
   hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
#endif
   strcpy(fn,"\"");                     /* The filename of the help file must*/
   strcat(fn,DefIOPath);                /* be quoted if it includes blanks   */
   for (i = 0;i < LanguagesCount;i++)
      if (Languages[i].code == Language)
         break;
   if (i >= (int) LanguagesCount)       /* Unknown value                     */
      i = 0;
   sprintf(basename,"crond%.3s.hlp",Languages[i].description);
   AppendPathComponent(fn,basename);
   strcat(fn,"\"");
   hini.pszHelpLibraryName = fn;

                                        /* create the instance               */
   hwndHelpInstance = WinCreateHelpInstance(hab,&hini);
   if ((hwndHelpInstance == (HWND) 0) || (hini.ulReturnCode != 0)) {
      DestroyHelpInstance();            /* window may exist!                 */
      PMError(Get(IDS_HelpCantLoad),(HWND) 0);
      return(0);
   }
                                        /* associate to our master frame     */
   if (!WinAssociateHelpInstance(hwndHelpInstance,hwndFrame)) {
      WinError((HWND) 0);
      DestroyHelpInstance();
      PMError(Get(IDS_HelpCantLoad),(HWND) 0);
      return(0);
   }
   return(1);
}

/*****************************************************************************/
/*  function name : DefaultHelp                                              */
/*                                                                           */
/*  arguments     : message id of the help instance (HM_...)                 */
/*                                                                           */
/*  description   : this function displays standard help topics (help for    */
/*                  keys, etc.)                                              */
/*                  An error message is displayed in case of an error.       */
/*****************************************************************************/
void DefaultHelp(ULONG msg)
{
   if (hwndHelpInstance != (HWND) 0)
      if (WinSendMsg(hwndHelpInstance,msg,MPVOID,MPVOID) != (MRESULT) FALSE)
         PMError(Get(IDS_HelpCantLoad),(HWND) 0);
}

/*****************************************************************************/
/*  function name : DisplayHelpPanel                                         */
/*                                                                           */
/*  arguments     : id of the panel to display                               */
/*                                                                           */
/*  description   : this function displays a specific help panel. This is    */
/*                  a helper function for system dialogs which will send     */
/*                  a WM_HELP. This message won't be correctly resolved      */
/*                  by the standard help mechanism.                          */
/*                  An error message is displayed in case of an error.       */
/*****************************************************************************/
void DisplayHelpPanel(ULONG idPanel)
{
   if (hwndHelpInstance != (HWND) 0)
      if (WinSendMsg(hwndHelpInstance,
                     HM_DISPLAY_HELP,
                     MPFROMLONG(idPanel),
                     MPFROMSHORT(HM_RESOURCEID)) != (MRESULT) FALSE)
         PMError(Get(IDS_HelpCantLoad),(HWND) 0);
}

/*****************************************************************************/
/*  function name : DriveIsLocalAndNotRemovable                              */
/*                                                                           */
/*  arguments     : drive number (0 = A:)                                    */
/*                                                                           */
/*  return value  : 1 if drive is local and not removable, 0 otherwise       */
/*                                                                           */
/*  description   : see return value                                         */
/*****************************************************************************/
static int DriveIsLocalAndNotRemovable(ULONG DriveNo)
{
   BYTE cmd[2];
   ULONG plen,dlen;
   BIOSPARAMETERBLOCK bpb;
                                        /* There is no need to check it the  */
                                        /* drive is a local drive. We use    */
                                        /* DevIOCtl. This function returns   */
                                        /* an error in case of a remote file */
                                        /* system.                           */
   cmd[0] = BUILD_BPB_FROM_MEDIUM;      /* Return default BPB                */
   cmd[1] = (BYTE) DriveNo;             /* Drivenumber (0 = A:)              */
   plen = sizeof(cmd);
   dlen = sizeof(bpb);
   if (DosDevIOCtl((HFILE) -1,
                   IOCTL_DISK,
                   DSK_GETDEVICEPARAMS,
                   cmd,
                   plen,
                   &plen,
                   &bpb,
                   dlen,
                   &dlen) != 0)
      return(0);
   if ((bpb.fsDeviceAttr & 1) &&        /* removable?                        */
       (bpb.bDeviceType == DEVTYPE_FIXED))   /* and fixed disk?              */
      return(1);
   return(0);
}

/*****************************************************************************/
/*  function name : PathChangeDlgProc                                        */
/*                                                                           */
/*  arguments     : see WinDefDlgProc                                        */
/*                                                                           */
/*  return value  : see WinDefDlgProc                                        */
/*                                                                           */
/*  description   : see WinDefDlgProc, we process some special events        */
/*                  here. The associated dialog is IDD_OUTPUT_PATH.          */
/*                  This function changes the output path.                   */
/*                                                                           */
/*  note          : don't call directly. This function is called by PM.      */
/*****************************************************************************/
static MRESULT EXPENTRY PathChangeDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,
                                                                    MPARAM mp2)
{
   char *buf,component[CCHMAXPATH];
   HWND list;
   DIR *dir;
   struct dirent *ent;
   static char Drive[sizeof("[-?-] ") + sizeof(VOLUMELABEL)];
   FSINFO info;
   ULONG i,Disks;
   SHORT sel;
   MRESULT retval;

   switch (msg) {
      case WM_INITDLG:
         WinSendDlgItemMsg(hwnd,        /* set the maximum output path we    */
                           NLS(IDDE_OUTPUT_PATH_ENTRY),  /* can process      */
                           EM_SETTEXTLIMIT,
                           MPFROMLONG(CCHMAXPATH),
                           MPVOID);
         WinSetWindowULong(hwnd,QWL_USER,(ULONG) mp2);
         WinPostMsg(hwnd,WM_USER,MPVOID,MPVOID);   /* send a refresh to      */
         break;                         /* ourself                           */

      case WM_USER:
         buf = (char *) WinQueryWindowULong(hwnd,QWL_USER);
         FullName(buf);                 /* make a fully qualified pathname   */
         WinSetDlgItemText(hwnd,        /* copy the current output path to   */
                           NLS(IDDE_OUTPUT_PATH_ENTRY),  /* the dialog       */
                           (PSZ) buf);

         list = WinWindowFromID(hwnd,NLS(IDDE_OUTPUT_PATH_LIST));
         WinSendMsg(list,LM_DELETEALL,MPVOID,MPVOID); /* delete all entries  */
         if ((dir = opendir(buf)) != NULL) { /* fill up with the current     */
                                        /* possible subdirectories           */
            while ((ent = readdir(dir)) != NULL) {
                                        /* We allow plain subdirs and the    */
                                        /* parent only                       */
               if ((ent->d_attr | A_ARCHIVE) != (A_DIR | A_ARCHIVE))
                  continue;
               if (strcmp(ent->d_name,".") == 0)
                  continue;
               WinSendMsg(list,
                          LM_INSERTITEM,
                          MPFROMLONG(LIT_SORTASCENDING),
                          MPFROMP(ent->d_name));
            }
            closedir(dir);
         }
                                        /* now, insert possible drives. We   */
                                        /* don't allow readonly or removable */
                                        /* drives, but we can't determine    */
                                        /* them. Instead we exclude the      */
                                        /* diskette drives only.             */
         DosQueryCurrentDisk(&i,&Disks);  /* i is a dummy                    */
         for (i = 0;i < 26;i++)
            if (Disks & (1ul << i)) {
               if (!DriveIsLocalAndNotRemovable(i))
                  continue;
               if (DosQueryFSInfo(i + 1,FSIL_VOLSER,&info,sizeof(info)) != 0)
                  info.vol.szVolLabel[0] = '\0';
               sprintf(Drive,"[-%c-] %s",(char) ('A' + i),info.vol.szVolLabel);
               WinSendMsg(list,
                          LM_INSERTITEM,
                          MPFROMLONG(LIT_END),
                          MPFROMP(Drive));
            }

         return(MPFROMSHORT(TRUE));     /* we've processed the message       */

      case WM_CONTROL:
         if (mp1 == MPFROM2SHORT(NLS(IDDE_OUTPUT_PATH_LIST),LN_ENTER)) {
            retval = WinDefDlgProc(hwnd,msg,mp1,mp2);
            sel = (SHORT) (LONG) WinSendDlgItemMsg(hwnd,
                                                   NLS(IDDE_OUTPUT_PATH_LIST),
                                                   LM_QUERYSELECTION,
                                                   MPFROMSHORT(LIT_FIRST),
                                                   MPVOID);
            if (sel == LIT_NONE)
               return(retval);
            buf = (char *) WinQueryWindowULong(hwnd,QWL_USER);
            WinSendDlgItemMsg(hwnd,
                              NLS(IDDE_OUTPUT_PATH_LIST),
                              LM_QUERYITEMTEXT,
                              MPFROM2SHORT(sel,CCHMAXPATH),
                              MPFROMP(component));
            if ((component[0] == '[') && (component[1] == '-') &&
                (component[3] == '-') && (component[4] == ']') &&
                (component[5] == ' ')) {
               buf[0] = component[2];
               strcpy(buf + 1,":\\");
            } else
               AppendPathComponent(buf,component);
            WinPostMsg(hwnd,WM_USER,MPVOID,MPVOID);   /* send a refresh to   */
                                        /* ourself                           */
            return(retval);             /* message processed                 */
         }
         break;

      case WM_COMMAND:
         switch(LOUSHORT(mp1)) {
            case DID_OK:
               buf = (char *) WinQueryWindowULong(hwnd,QWL_USER);
               WinQueryDlgItemText(hwnd,
                                   NLS(IDDE_OUTPUT_PATH_ENTRY),
                                   CCHMAXPATH,
                                   component);
               FullName(component);
               if (_fncmp(component,OutputPath) == 0) /* path not changed?   */
                                        /* process if has been cancelled     */
                  WinDismissDlg(hwnd,DID_CANCEL);  /* kill the dialog        */
               else if (!CheckOutputPath(component))
                  PMError(Get(IDS_EnterValidOutput),hwnd);
               else {
                  strcpy(buf,component);
                  WinDismissDlg(hwnd,DID_OK);   /* kill the dialog           */
               }
               return(MPFROMSHORT(TRUE));  /* we've processed the message    */

            case DID_CANCEL:
               WinDismissDlg(hwnd,DID_CANCEL);  /* kill the dialog           */
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */
         }
         break;

   }
   return(WinDefDlgProc(hwnd,msg,mp1,mp2));

}

/*****************************************************************************/
/*  function name : OutputDlgProc                                            */
/*                                                                           */
/*  arguments     : see WinDefDlgProc                                        */
/*                                                                           */
/*  return value  : see WinDefDlgProc                                        */
/*                                                                           */
/*  description   : see WinDefDlgProc, we process some special events        */
/*                  here. The associated dialog is IDD_OUTPUT.               */
/*                  This function changes all output file related and user   */
/*                  changable values.                                        */
/*                                                                           */
/*  note          : don't call directly. This function is called by PM.      */
/*****************************************************************************/
static MRESULT EXPENTRY OutputDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,
                                                                    MPARAM mp2)
{
   char buf[CCHMAXPATH];
   unsigned i;
   PCSZ errortext;
   ULONG NewThres,NewMax,NewNul;
   USHORT result;
   MPARAM mresult;
   HWND focus;
   USHORT id;
   static int AllowChanges = 1;

   switch (msg) {
      case WM_HELP:                     /* there are problems displaying the */
                                        /* correct help panel from the combo */
                                        /* boxes, since the boxes seen are   */
                                        /* childs of the original dlgitem.   */
                                        /* Thus, we look for owner (or       */
                                        /* parent) of the current focus      */
                                        /* window and display the associated */
                                        /* help panel if the focus window    */
                                        /* is a combo box window.            */
         focus = WinQueryFocus(HWND_DESKTOP);
         id = WinQueryWindowUShort(focus,QWS_ID);
         if ((id != CBID_LIST) && (id != CBID_EDIT))  /* focus not on a combo*/
            break;                      /* box? proceed with the default...  */
         DisplayHelpPanel(WinQueryWindowUShort(WinQueryWindow(focus,QW_OWNER),
                                                                      QWS_ID));
         return(MPFROMLONG(0));         /* we've processed the message       */

      case WM_INITDLG:
         AllowChanges = 0;              /* Don't allow automatically         */
                                        /* generated WM_USERs to ourself     */
         for (i = 2;i <= 10;i++) {      /* 2 - 10 different output files are */
            sprintf(buf,"%u",i);        /* acceptable, put the numbers in a  */
            WinSendDlgItemMsg(hwnd,     /* list box of a combo box           */
                              NLS(IDDE_OUTPUT_COUNT),
                              LM_INSERTITEM,
                              MPFROMSHORT(LIT_END),
                              buf);
         }
         sprintf(buf,"%u",MaxOutputs);  /* select current value as default   */
         WinSetDlgItemText(hwnd,
                           NLS(IDDE_OUTPUT_COUNT),
                           buf);

         for (i = 12;i <= 31;i++) {     /* put some good selections of a     */
            sprintf(buf,"%lu",1ul << i);  /* threshold in the related list   */
            WinSendDlgItemMsg(hwnd,     /* box of the combo box              */
                              NLS(IDDE_OUTPUT_THRES),
                              LM_INSERTITEM,
                              MPFROMSHORT(LIT_END),
                              buf);
         }
         sprintf(buf,"%lu",Threshold);  /* put the current value as default  */
         WinSetDlgItemText(hwnd,        /* into the box                      */
                           NLS(IDDE_OUTPUT_THRES),
                           buf);

         WinSendDlgItemMsg(hwnd,        /* set the maximum output path we    */
                           NLS(IDDE_OUTPUT_PATH),  /* can process            */
                           EM_SETTEXTLIMIT,
                           MPFROMLONG(CCHMAXPATH),
                           MPVOID);
         WinSetDlgItemText(hwnd,        /* copy the current output path to   */
                           NLS(IDDE_OUTPUT_PATH),  /* the dialog             */
                           OutputPath);
                                        /* set the button "ignore output" to */
                                        /* the current value                 */
         SetCheckBox(hwnd,PRG_OUTPUT_IS_NUL,NLS(IDDE_OUTPUT_IGNORE_OUTPUT));
         AllowChanges = 1;
         break;

      case WM_USER:                     /* send from ourself to set all      */
                                        /* changes                           */
         errortext = NULL;              /* no error is the default           */
         NewThres = Threshold;          /* in case of an unknown mistake     */
         if (WinQueryDlgItemText(hwnd,
                                 NLS(IDDE_OUTPUT_THRES),
                                 sizeof(buf),
                                 buf))
            ScanNumber(buf,&NewThres);  /* should success always since user  */
                                        /* can't change the possible values  */

         NewMax = MaxOutputs;           /* in case of an unknown mistake     */
         if (WinQueryDlgItemText(hwnd,
                                 NLS(IDDE_OUTPUT_COUNT),
                                 sizeof(buf),
                                 buf))
            ScanNumber(buf,&NewMax);    /* should success always since user  */
                                        /* can't change the possible values  */

         if (!WinQueryDlgItemText(hwnd, /* read and check output path        */
                                  NLS(IDDE_OUTPUT_PATH),
                                  sizeof(buf),
                                  buf))
            errortext = Get(IDS_EnterValidOutput);
         else if (!CheckOutputPath(buf))
            errortext = Get(IDS_EnterValidOutput);

         if (errortext == NULL) {       /* 'til now no errors ? accept the   */
                                        /* user selection set values         */
            i = 0;                      /* flag: no new output file          */
            if (_fncmp(buf,OutputPath) != 0) {
               strcpy(OutputPath,buf);  /* output path different?            */
               ReWritePrfFlags |= REWRITE_OUTPUTPATH;
               i = 1;
            }
            if (NewThres != Threshold) {  /* next Message call will change   */
               Threshold = NewThres;    /* the output file itself            */
               ReWritePrfFlags |= REWRITE_THRESHOLD;
            }
            if (NewMax != MaxOutputs) {
               MaxOutputs = (UCHAR) NewMax;
               ReWritePrfFlags |= REWRITE_MAXIMUM;
               if ((CurrOutput >= MaxOutputs) &&
                   ((ProgramFlags & PRG_OUTPUT_IS_NUL) == 0))
                  i = 1;                /* no longer in a valid interval     */
            }
            NewNul = (ULONG) WinQueryButtonCheckstate(hwnd,
                                               NLS(IDDE_OUTPUT_IGNORE_OUTPUT));
            if ((NewNul && ((ProgramFlags & PRG_OUTPUT_IS_NUL) == 0))||
                (!NewNul && ((ProgramFlags & PRG_OUTPUT_IS_NUL) != 0))) {
                                        /* the state has changed             */
               if (NewNul)
                  ProgramFlags |= PRG_OUTPUT_IS_NUL;
               else
                  ProgramFlags &= ~((ULONG) PRG_OUTPUT_IS_NUL);
               ReWritePrfFlags |= REWRITE_FLAGS;
               i = 1;
            }
            if (i == 1) {               /* select another output file?       */
               BlockOutput();
               NextOutputFile(0);       /* select a new one (no append)      */
               UnBlockOutput();
            }
            ReWritePrf();
            return(MPFROMLONG(0));      /* we have processed the message     */
         }
                                        /* else: errortext != NULL           */
         Error(errortext,hwnd);
         return(MPFROMLONG(0));         /* we have processed the message     */

      case WM_CONTROL:
         mresult = WinDefDlgProc(hwnd,msg,mp1,mp2);
         if (((SHORT2FROMMP(mp1) == CBN_EFCHANGE) &&
              ((SHORT1FROMMP(mp1) == NLS(IDDE_OUTPUT_THRES)) ||
               (SHORT1FROMMP(mp1) == NLS(IDDE_OUTPUT_COUNT)))) ||
             (SHORT1FROMMP(mp1) == NLS(IDDE_OUTPUT_IGNORE_OUTPUT)))
            WinPostMsg(hwnd,WM_USER,MPVOID,MPVOID);   /* update values       */
         return(mresult);

      case WM_COMMAND:
         switch(NONLS(LOUSHORT(mp1))) {
            case IDDE_OUTPUT_PATHCHANGE:
               strcpy(buf,OutputPath);
               result = WinDlgBox(HWND_DESKTOP,
                                  hwnd,
                                  PathChangeDlgProc,
                                  0,
                                  NLS(IDD_OUTPUT_PATH),
                                  buf);
               if (result == DID_ERROR)
                  WinError((HWND) 0);
               else if (result == DID_OK) {
                  WinSetDlgItemText(hwnd, /* save the new path               */
                                    NLS(IDDE_OUTPUT_PATH),
                                    buf);
                  WinPostMsg(hwnd,WM_USER,MPVOID,MPVOID);   /* update values */
               }
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            case IDDE_OUTPUT_HELP:
               DisplayHelpPanel(NLS(IDD_OUTPUT));
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            default:                    /* don't expect any other commands!  */
                                        /* It may be the result of an ESC    */
                                        /* or other unsupported keys.        */
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */
         }
         break;

   }
   return(WinDefDlgProc(hwnd,msg,mp1,mp2));
}

/*****************************************************************************/
/*  function name : GeneralDlgProc                                           */
/*                                                                           */
/*  arguments     : see WinDefDlgProc                                        */
/*                                                                           */
/*  return value  : see WinDefDlgProc                                        */
/*                                                                           */
/*  description   : see WinDefDlgProc, we process some special events        */
/*                  here. The associated dialog is IDD_GENERAL.              */
/*                  This function changes all general and unspecific user    */
/*                  changable values.                                        */
/*                                                                           */
/*  note          : don't call directly. This function is called by PM.      */
/*****************************************************************************/
static MRESULT EXPENTRY GeneralDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,
                                                                    MPARAM mp2)
{
   char buf[64];                        /* Hope, this is enough to hold the  */
                                        /* Language specifier                */
   USHORT i,j,NewLanguage;
   char *current;
   MPARAM mresult;
   HWND focus;
   USHORT id;
   SHORT len;
   static CHAR *Warning = NULL;
   static int AllowChanges = 1,new;


   switch (msg) {
      case WM_HELP:                     /* there are problems displaying the */
                                        /* correct help panel from the combo */
                                        /* boxes, since the boxes seen are   */
                                        /* childs of the original dlgitem.   */
                                        /* Thus, we look for owner (or       */
                                        /* parent) of the current focus      */
                                        /* window and display the associated */
                                        /* help panel if the focus window    */
                                        /* is a combo box window.            */
         focus = WinQueryFocus(HWND_DESKTOP);
         id = WinQueryWindowUShort(focus,QWS_ID);
         if ((id != CBID_LIST) && (id != CBID_EDIT))  /* focus not on a combo*/
            break;                      /* box? proceed with the default...  */
         DisplayHelpPanel(WinQueryWindowUShort(WinQueryWindow(focus,QW_OWNER),
                                                                      QWS_ID));
         return(MPFROMLONG(0));         /* we've processed the message       */

      case WM_INITDLG:
         AllowChanges = 0;              /* Don't allow automatically         */
                                        /* generated WM_USERs to ourself     */
         for (i = 0;i < LanguagesCount;i++) {   /* Insert all different      */
            new = 1;                    /* language specifiers               */
            for (j = 0;j < i;j++)
               if (Languages[j].code == Languages[i].code)
                  new = 0;
            if (new)
               WinSendDlgItemMsg(hwnd,  /* list box of a combo box           */
                                 NLS(IDDE_GENERAL_LANGUAGE),
                                 LM_INSERTITEM,
                                 MPFROMSHORT(LIT_SORTASCENDING),
                                 Languages[i].description);
         }
         for (current = "???",i = 0;i < LanguagesCount;i++)
            if (Languages[i].code == Language)
               current = Languages[i].description;
                                        /* select current value as default   */
         WinSetDlgItemText(hwnd,
                           NLS(IDDE_GENERAL_LANGUAGE),
                           current);
         if (Warning == NULL) {         /* First time?                       */
            len = WinQueryDlgItemTextLength(hwnd,
                                            NLS(IDDE_GENERAL_LANGUAGE_WARNING));
            if ((Warning = malloc(len + 1)) == NULL)
               PMFatalError(Get(IDS_NotEnoughMemory),hwnd);
            else
               WinQueryDlgItemText(hwnd,
                                   NLS(IDDE_GENERAL_LANGUAGE_WARNING),
                                   len + 1,
                                   Warning);
         }
         WinSetDlgItemText(hwnd,
                           NLS(IDDE_GENERAL_LANGUAGE_WARNING),
                           (CurrentLanguage == Language) ? "" : Warning);
         AllowChanges = 1;
         break;

      case WM_USER:                     /* send from ourself to set all      */
                                        /* changes                           */
         if (WinQueryDlgItemText(hwnd,
                                 NLS(IDDE_GENERAL_LANGUAGE),
                                 sizeof(buf),
                                 buf)) {
                                        /* should success always since user  */
                                        /* can't change the possible values  */
            NewLanguage = Language;
            for (i = 0;i < LanguagesCount;i++)
               if (strcmp(Languages[i].description,buf) == 0)
                  NewLanguage = Languages[i].code;
            if (NewLanguage != Language) {
               Language = NewLanguage;
               ReWritePrfFlags |= REWRITE_LANGUAGE;
               ReWritePrf();
            }
            WinSetDlgItemText(hwnd,
                              NLS(IDDE_GENERAL_LANGUAGE_WARNING),
                              (CurrentLanguage == Language) ? "" : Warning);
         }
         return(MPFROMLONG(0));         /* we have processed the message     */

      case WM_CONTROL:
         mresult = WinDefDlgProc(hwnd,msg,mp1,mp2);
         if ((SHORT2FROMMP(mp1) == CBN_EFCHANGE) &&
             (SHORT1FROMMP(mp1) == NLS(IDDE_GENERAL_LANGUAGE)))
            WinPostMsg(hwnd,WM_USER,MPVOID,MPVOID);   /* update values       */
         return(mresult);

      case WM_COMMAND:
         switch(NONLS(LOUSHORT(mp1))) {
            case IDDE_GENERAL_HELP:
               DisplayHelpPanel(NLS(IDD_GENERAL));
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            default:                    /* don't expect any other commands!  */
                                        /* It may be the result of an ESC    */
                                        /* or other unsupported keys.        */
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */
         }
         break;

   }
   return(WinDefDlgProc(hwnd,msg,mp1,mp2));
}

/*****************************************************************************/
/*  function name : TCPFileDlgProc                                           */
/*                                                                           */
/*  arguments     : see WinDefFileDlgProc                                    */
/*                                                                           */
/*  return value  : see WinDefFileDlgProc                                    */
/*                                                                           */
/*  description   : see WinDefFileDlgProc, we process some special events    */
/*                  here. The associated dialog is called by WinFileDlg.     */
/*                  This function changes the TCP/IP allow file name.        */
/*                                                                           */
/*  note          : don't call directly. This function is called by PM.      */
/*****************************************************************************/
static MRESULT EXPENTRY TCPFileDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,
                                                                    MPARAM mp2)
{

   switch (msg) {
      case WM_HELP:
         DisplayHelpPanel(NLS(IDDE_TCP_FILENAMECHANGE));
         return(MPFROMLONG(0));         /* we've processed the message       */

      case FDM_VALIDATE:
         FullName((PSZ) mp1);           /* make a fully qualified filename   */
         if (_fncmp(TCPIP_Allow,(PSZ) mp1) == 0)   /* filename identical?    */
               return(MPFROMSHORT(TRUE)); /* we've processed the message, we */
                                        /* don't change anything             */
         if (!ReadAllow((PSZ) mp1)) {   /* can't read the file?              */
            Error(Get(IDS_InvalidFormat),hwnd);
            return(MPFROMSHORT(FALSE)); /* we've processed the message, the  */
                                        /* user must select another file     */
         }
         /* else */                     /* file has been read successfully   */
         ReWritePrfFlags |= REWRITE_TCPIP_ALLOW;
         strcpy(TCPIP_Allow,(PSZ) mp1);
         ReWritePrf();
         return(MPFROMSHORT(TRUE));     /* we've processed the message       */
                                        /* the system will close the dialog  */
   }
   return(WinDefFileDlgProc(hwnd,msg,mp1,mp2));
}

/*****************************************************************************/
/*  function name : TCPIPDlgProc                                             */
/*                                                                           */
/*  arguments     : see WinDefDlgProc                                        */
/*                                                                           */
/*  return value  : see WinDefDlgProc                                        */
/*                                                                           */
/*  description   : see WinDefDlgProc, we process some special events        */
/*                  here. The associated dialog is IDD_TCPIP.                */
/*                  This function changes all TCP/IP related and user        */
/*                  changable values.                                        */
/*                                                                           */
/*  note          : don't call directly. This function is called by PM.      */
/*****************************************************************************/
static MRESULT EXPENTRY TCPIPDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   FILEDLG fild;
   USHORT id;
   ULONG ButtonState;
   MRESULT mresult;

   switch (msg) {
      case WM_INITDLG:
         if (!TCPIPAvail) {
            if (ProgramFlags & PRG_ALLOW_TCPIP) {
               ProgramFlags &= ~PRG_ALLOW_TCPIP;
               ReWritePrf();
               Error(Get(IDS_NoTCPIPFound),hwnd);
            }
            SetCheckBox(hwnd,PRG_ALLOW_TCPIP,NLS(IDDE_TCP_ALLOWACCESS));
            WinEnableControl(hwnd,NLS(IDDE_TCP_ALLOWACCESS),FALSE);
         } else
            SetCheckBox(hwnd,PRG_ALLOW_TCPIP,NLS(IDDE_TCP_ALLOWACCESS));
         WinSetDlgItemText(hwnd,        /* copy the current output path to   */
                           NLS(IDDE_TCP_FILENAME), /* the dialog             */
                           TCPIP_Allow);
         break;

      case WM_CONTROL:
         mresult = WinDefDlgProc(hwnd,msg,mp1,mp2);
         id = SHORT1FROMMP(mp1);
         if (id == NLS(IDDE_TCP_ALLOWACCESS)) {
            ButtonState = (ULONG) WinQueryButtonCheckstate(hwnd,id);
            if ((ButtonState && ((ProgramFlags & PRG_ALLOW_TCPIP) == 0)) ||
                (!ButtonState && ((ProgramFlags & PRG_ALLOW_TCPIP) != 0))) {
                                        /* the state has changed             */
               if (ButtonState)
                  ProgramFlags |= PRG_ALLOW_TCPIP;
               else
                  ProgramFlags &= ~((ULONG) PRG_ALLOW_TCPIP);
               ReWritePrfFlags |= REWRITE_FLAGS;
               ReWritePrf();
            }
         }
         return(mresult);

      case WM_COMMAND:
         switch(NONLS(LOUSHORT(mp1))) {
            case IDDE_TCP_FILENAMECHANGE:
               memset(&fild,0,sizeof(fild));
               fild.cbSize            = sizeof(fild);
               fild.fl                = FDS_CENTER |
                                        FDS_OPEN_DIALOG |
                                        FDS_HELPBUTTON |
                                        0;
               fild.pszTitle          = (PSZ) Get(IDS_TitleNewTCPIP);
               fild.pfnDlgProc        = TCPFileDlgProc;
               strcpy(fild.szFullFile,  TCPIP_Allow);
               if (!WinFileDlg(HWND_DESKTOP,hwnd,&fild))
                  WinError((HWND) 0);
               WinSetDlgItemText(hwnd,  /* copy the current output path to   */
                                 NLS(IDDE_TCP_FILENAME), /* the dialog       */
                                 TCPIP_Allow);
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            case IDDE_TCP_HELP:
               DisplayHelpPanel(NLS(IDD_TCP));
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            default:                    /* don't expect any other commands!  */
                                        /* It may be the result of an ESC    */
                                        /* or other unsupported keys.        */
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

         }
         break;

   }
   return(WinDefDlgProc(hwnd,msg,mp1,mp2));
}

/*****************************************************************************/
/*  function name : CronFileDlgProc                                          */
/*                                                                           */
/*  arguments     : see WinDefFileDlgProc                                    */
/*                                                                           */
/*  return value  : see WinDefFileDlgProc                                    */
/*                                                                           */
/*  description   : see WinDefFileDlgProc, we process some special events    */
/*                  here. The associated dialog is called by WinFileDlg.     */
/*                  This function changes the Crontabs file name.            */
/*                                                                           */
/*  note          : don't call directly. This function is called by PM.      */
/*****************************************************************************/
static MRESULT EXPENTRY CronFileDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,
                                                                    MPARAM mp2)
{
   switch (msg) {
      case WM_HELP:
         DisplayHelpPanel(NLS(IDDE_CRON_FILENAMECHANGE));
         return(MPFROMLONG(0));         /* we've processed the message       */

      case FDM_VALIDATE:
         FullName((PSZ) mp1);           /* make a fully qualified filename   */
         if (_fncmp(Crontabs,(PSZ) mp1) == 0)   /* filename identical?       */
               return(MPFROMSHORT(TRUE)); /* we've processed the message, we */
                                        /* don't change anything             */
         if (!ReReadCrontabs((PSZ) mp1)) {   /* can't read the file?         */
            Error(Get(IDS_InvalidFormat),hwnd);
            return(MPFROMSHORT(FALSE)); /* we've processed the message, the  */
                                        /* user must select another file     */
         }
         /* else */                     /* file has been read successfully   */
         ReWritePrfFlags |= REWRITE_CRONFILENAME;
         strcpy(Crontabs,(PSZ) mp1);
         ReWritePrf();
         return(MPFROMSHORT(TRUE));     /* we've processed the message       */
                                        /* the system will close the dialog  */
   }
   return(WinDefFileDlgProc(hwnd,msg,mp1,mp2));
}

/*****************************************************************************/
/*  function name : CronDlgProc                                              */
/*                                                                           */
/*  arguments     : see WinDefDlgProc                                        */
/*                                                                           */
/*  return value  : see WinDefDlgProc                                        */
/*                                                                           */
/*  description   : see WinDefDlgProc, we process some special events        */
/*                  here. The associated dialog is IDD_CRON_FILE             */
/*                  This function changes all Crontabs related and user      */
/*                  changable values.                                        */
/*                                                                           */
/*  note          : don't call directly. This function is called by PM.      */
/*****************************************************************************/
static MRESULT EXPENTRY CronDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   FILEDLG fild;
   USHORT id;
   ULONG prgflag,ButtonState;
   MRESULT mresult;

   switch (msg) {
      case WM_INITDLG:
         SetCheckBox(hwnd,PRG_RUNATSTARTUP,NLS(IDDE_CRON_ATSTARTUP));
         SetCheckBox(hwnd,PRG_RUNATEXIT,   NLS(IDDE_CRON_ATEXIT));
         WinSetDlgItemText(hwnd,        /* copy the current output path to   */
                           NLS(IDDE_CRON_FILENAME),   /* the dialog          */
                           Crontabs);
         break;

      case WM_CONTROL:
         mresult = WinDefDlgProc(hwnd,msg,mp1,mp2);
         id = SHORT1FROMMP(mp1);
         if ((id == NLS(IDDE_CRON_ATSTARTUP)) ||
             (id == NLS(IDDE_CRON_ATEXIT))) {
            if (id == NLS(IDDE_CRON_ATSTARTUP))
               prgflag = PRG_RUNATSTARTUP;
            else
               prgflag = PRG_RUNATEXIT;
            ButtonState = (ULONG) WinQueryButtonCheckstate(hwnd,id);
            if ((ButtonState && ((ProgramFlags & prgflag) == 0)) ||
                (!ButtonState && ((ProgramFlags & prgflag) != 0))) {
                                        /* the state has changed             */
               if (ButtonState)
                  ProgramFlags |= prgflag;
               else
                  ProgramFlags &= ~prgflag;
               ReWritePrfFlags |= REWRITE_FLAGS;
               ReWritePrf();
            }
         }
         return(mresult);

      case WM_COMMAND:
         switch(NONLS(LOUSHORT(mp1))) {
            case IDDE_CRON_FILENAMECHANGE:
               memset(&fild,0,sizeof(fild));
               fild.cbSize            = sizeof(fild);
               fild.fl                = FDS_CENTER |
                                        FDS_OPEN_DIALOG |
                                        FDS_HELPBUTTON |
                                        0;
               fild.pszTitle          = (PSZ) Get(IDS_TitleNewCron);
               fild.pfnDlgProc        = CronFileDlgProc;
               strcpy(fild.szFullFile,  Crontabs);
               if (!WinFileDlg(HWND_DESKTOP,hwnd,&fild))
                  WinError((HWND) 0);
               WinSetDlgItemText(hwnd,  /* copy the current output path to   */
                                 NLS(IDDE_CRON_FILENAME),   /* the dialog    */
                                 Crontabs);
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            case IDDE_CRON_HELP:
               DisplayHelpPanel(NLS(IDD_CRON_FILE));
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            default:                    /* don't expect any other commands!  */
                                        /* It may be the result of an ESC    */
                                        /* or other unsupported keys.        */
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

         }
         break;

   }
   return(WinDefDlgProc(hwnd,msg,mp1,mp2));
}

/*****************************************************************************/
/*  function name : CrontabsDlgProc                                          */
/*                                                                           */
/*  arguments     : see WinDefDlgProc                                        */
/*                                                                           */
/*  return value  : see WinDefDlgProc                                        */
/*                                                                           */
/*  description   : see WinDefDlgProc, we process some special events        */
/*                  here. The associated dialog is IDD_CRONTABS              */
/*                  This function allows the user to change Crontabs         */
/*                  elements online.                                         */
/*                                                                           */
/*  note          : don't call directly. This function is called by PM.      */
/*****************************************************************************/
static MRESULT EXPENTRY CrontabsDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,
                                                                    MPARAM mp2)
{
   ULONG len,err,err2;
   HWND list;
   SHORT sel,deleted;
   char *buf,*ptr,*line;
   char tmpbuf[1024];
   static ULONG CurrentlyEditedNumber = (ULONG) -1;
   LIST_ENTRY *aktentry;
   BOOL ok;
   PVOID ClipData;
   MRESULT mresult;

   switch (msg) {
      case WM_INITDLG:
         CrontabsWindow = hwnd;
         WinPostMsg(hwnd,WM_USER,MPVOID,MPVOID);
         WinSendDlgItemMsg(hwnd,        /* set the maximum command line we   */
                           NLS(IDDE_CRONTABS_ENTRY),  /* allow               */
                           EM_SETTEXTLIMIT,
                           MPFROMLONG(4095),
                           MPVOID);
         break;

      case WM_DESTROY:
         CrontabsWindow = (HWND) 0;
         EditEntryNumber = CurrentlyEditedNumber = (ULONG) -1;
         break;

      case WM_USER:
                                        /* don't allow user to use the edit  */
                                        /* button                            */
         WinEnableControl(hwnd,NLS(IDDE_CRONTABS_EDIT),FALSE);
         if (CurrentlyEditedNumber != (ULONG) -1)  /* We must aware of a     */
            if (EditEntryNumber == (ULONG) -1) {   /* currently edited job   */
               WinEnableControl(hwnd,NLS(IDDE_CRONTABS_CHANGE),FALSE);
               WinEnableControl(hwnd,NLS(IDDE_CRONTABS_ABORT),FALSE);
               PMError(Get(IDS_JobJustDeleted),hwnd);
               CurrentlyEditedNumber = (ULONG) -1;
            }
         WinEnableControl(hwnd,NLS(IDDE_CRONTABS_DELETE),FALSE);
         list = WinWindowFromID(hwnd,NLS(IDDE_CRONTABS_LIST));
         WinSendMsg(list,LM_DELETEALL,MPVOID,MPVOID); /* delete all entries  */
         BlockProcess();
         buf = ptr = CreateList(CL_PM);
         UnBlockProcess();
         while (ptr != NULL) {
            line = ptr;
            while (isspace(*line))
               line++;
            if ((ptr = strpbrk(line,"\r\n")) != NULL)
               *ptr++ = '\0';
            if (*line != '\0')          /* Don't display empty lines         */
               WinSendMsg(list,
                          LM_INSERTITEM,
                          MPFROMLONG(LIT_END),
                          MPFROMP(line));
         }
         if (buf != NULL)
            free(buf);
         break;

      case WM_CONTROL:
                                        /* figure out the selection status   */
                                        /* of the list if modified           */
         if (mp1 == MPFROM2SHORT(NLS(IDDE_CRONTABS_LIST),LN_SELECT)) {
            list = WinWindowFromID(hwnd,NLS(IDDE_CRONTABS_LIST));
            mresult = WinDefDlgProc(hwnd,msg,mp1,mp2);
                                        /* the selection is modified. If only*/
                                        /* one item is selected, we'll allow */
                                        /* to edit the entry.                */
            ok = FALSE;
            sel = (SHORT) (LONG) WinSendMsg(list,
                                            LM_QUERYSELECTION,
                                            MPFROMSHORT(LIT_FIRST),
                                            MPVOID);
            if (sel != LIT_NONE) {
               WinEnableControl(hwnd,NLS(IDDE_CRONTABS_DELETE),TRUE);
               if (LIT_NONE == (SHORT) (LONG) WinSendMsg(list,
                                                         LM_QUERYSELECTION,
                                                         MPFROMSHORT(sel),
                                                         MPVOID))
                  ok = TRUE;
            } else
               WinEnableControl(hwnd,NLS(IDDE_CRONTABS_DELETE),FALSE);
            WinEnableControl(hwnd,NLS(IDDE_CRONTABS_EDIT),ok);
            return(mresult);
         } else if (mp1 == MPFROM2SHORT(NLS(IDDE_CRONTABS_ENTRY),EN_CHANGE)) {
            if (WinQueryDlgItemTextLength(hwnd,NLS(IDDE_CRONTABS_ENTRY)) == 0)
               WinEnableControl(hwnd,NLS(IDDE_CRONTABS_ADD),FALSE);
            else
               WinEnableControl(hwnd,NLS(IDDE_CRONTABS_ADD),TRUE);
         }
         break;

      case WM_COMMAND:
         switch(NONLS(LOUSHORT(mp1))) {
            case IDDE_CRONTABS_ADD:
               len = (ULONG) WinQueryDlgItemTextLength(hwnd,
                                                     NLS(IDDE_CRONTABS_ENTRY));
               if ((buf = malloc(len + 1)) == NULL) {
                  PMError(Get(IDS_NotEnoughMemory),hwnd);
                  return(MPFROMSHORT(TRUE)); /* we've processed the message  */
               }
               WinQueryDlgItemText(hwnd,
                                   NLS(IDDE_CRONTABS_ENTRY),
                                   len + 1,
                                   buf);
               BlockProcess();
               err = NewEntry(buf,len,tmpbuf);
               UnBlockProcess();
               if (err == 0) {
                  WinPostMsg(hwnd,WM_USER,MPVOID,MPVOID);
                  DosPostEventSem(CronSem);  /* compute next starting time   */
                  WinSetDlgItemText(hwnd,NLS(IDDE_CRONTABS_ENTRY),"");
                  EditEntryNumber = CurrentlyEditedNumber = (ULONG) -1;
                  WinEnableControl(hwnd,NLS(IDDE_CRONTABS_CHANGE),FALSE);
                  WinEnableControl(hwnd,NLS(IDDE_CRONTABS_ABORT),FALSE);
               } else
                  PMError(GetError(err),hwnd);
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            case IDDE_CRONTABS_CHANGE:
               len = (ULONG) WinQueryDlgItemTextLength(hwnd,
                                                     NLS(IDDE_CRONTABS_ENTRY));
               if ((buf = malloc(len + 1)) == NULL) {
                  PMError(Get(IDS_NotEnoughMemory),hwnd);
                  return(MPFROMSHORT(TRUE)); /* we've processed the message  */
               }
               WinQueryDlgItemText(hwnd,
                                   NLS(IDDE_CRONTABS_ENTRY),
                                   len + 1,
                                   buf);
               BlockProcess();
               if ((err = NewEntry(buf,len,tmpbuf)) == 0)
                  err2 = DeleteEntryNum(EditEntryNumber + 1,tmpbuf);
               else
                  err2 = 0;             /* make compiler happy               */
               UnBlockProcess();
               if (err == 0) {
                  WinPostMsg(hwnd,WM_USER,MPVOID,MPVOID);
                  DosPostEventSem(CronSem);  /* compute next starting time   */
                  WinSetDlgItemText(hwnd,NLS(IDDE_CRONTABS_ENTRY),"");
                  EditEntryNumber = CurrentlyEditedNumber = (ULONG) -1;
                  WinEnableControl(hwnd,NLS(IDDE_CRONTABS_CHANGE),FALSE);
                  WinEnableControl(hwnd,NLS(IDDE_CRONTABS_ABORT),FALSE);
                  if (err2 != 0)
                     PMError(GetError(err),hwnd);
               } else
                  PMError(GetError(err),hwnd);
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            case IDDE_CRONTABS_ABORT:
               WinSetDlgItemText(hwnd,NLS(IDDE_CRONTABS_ENTRY),"");
               EditEntryNumber = CurrentlyEditedNumber = (ULONG) -1;
               WinEnableControl(hwnd,NLS(IDDE_CRONTABS_CHANGE),FALSE);
               WinEnableControl(hwnd,NLS(IDDE_CRONTABS_ABORT),FALSE);
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            case IDDE_CRONTABS_DELETE:
               list = WinWindowFromID(hwnd,NLS(IDDE_CRONTABS_LIST));
               BlockProcess();
               for (sel = LIT_FIRST,deleted = 0;;) {
                  sel = (SHORT) (LONG) WinSendMsg(list,
                                                  LM_QUERYSELECTION,
                                                  MPFROMSHORT(sel),
                                                  MPVOID);
                  if (sel != LIT_NONE) {
                     if ((err = DeleteEntryNum(sel + 1 - deleted,tmpbuf)) != 0)
                        break;
                     deleted++;
                  } else
                     break;
               }
               UnBlockProcess();
               if (deleted) {
                  WinPostMsg(hwnd,WM_USER,MPVOID,MPVOID);
                  DosPostEventSem(CronSem);  /* compute next starting time   */
               }
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            case IDDE_CRONTABS_EDIT:
               list = WinWindowFromID(hwnd,NLS(IDDE_CRONTABS_LIST));
               BlockProcess();
               sel = (SHORT) (LONG) WinSendMsg(list,
                                               LM_QUERYSELECTION,
                                               MPFROMSHORT(LIT_FIRST),
                                               MPVOID);
               if (sel == LIT_NONE) {   /* oops, this should never happen    */
                  UnBlockProcess();     /* since the push-button should      */
                  return(MPFROMSHORT(TRUE)); /* be disabled in this case!    */
               }
               if ((aktentry = LookupEntryNum((ULONG) (LONG) sel + 1,0)) ==
                                                                        NULL) {
                  UnBlockProcess();     /* this will be a true error, it may */
                  return(MPFROMSHORT(TRUE)); /* be generated due race        */
               }                             /* conditions                   */

               EditEntryNumber = CurrentlyEditedNumber = (ULONG) (LONG) sel;
               WinSetDlgItemText(hwnd,
                                 NLS(IDDE_CRONTABS_ENTRY),
                                 aktentry->s);
                                        /* allow user to use the Change butt.*/
               WinEnableControl(hwnd,NLS(IDDE_CRONTABS_CHANGE),TRUE);
               WinEnableControl(hwnd,NLS(IDDE_CRONTABS_ABORT),TRUE);
                                        /* put data into clipboard           */
               if (WinOpenClipbrd(hab)) {
                  ok = FALSE;
                  if (DosAllocSharedMem(&ClipData,
                                        NULL,
                                        strlen(aktentry->s) + 1,
                                        PAG_READ | PAG_WRITE | PAG_COMMIT |
                                                 OBJ_TILE | OBJ_GIVEABLE) == 0)
                     ok = TRUE;
                  if (ok) {
                     strcpy(ClipData,aktentry->s);
                     ok = WinSetClipbrdData(hab,
                                            (ULONG) ClipData,
                                            CF_TEXT,
                                            CFI_POINTER);
                     if (!ok)
                        DosFreeMem(ClipData);
                  }
                  WinCloseClipbrd(hab);
                  if (ok)
                     WinMessageBox(HWND_DESKTOP,
                                   hwnd,
                                   Get(IDS_EntryInClipboard),
                                   Get(IDS_Title),
                                   0,
                                   MB_OK | MB_ICONASTERISK | MB_MOVEABLE);
               }
               UnBlockProcess();
               WinSetFocus(HWND_DESKTOP,
                           WinWindowFromID(hwnd,NLS(IDDE_CRONTABS_ENTRY)));
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            case IDDE_CRONTABS_HELP:
               DisplayHelpPanel(NLS(IDD_CRONTABS));
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */

            default:                    /* don't expect any other commands!  */
                                        /* It may be the result of an ESC    */
                                        /* or other unsupported keys.        */
               return(MPFROMSHORT(TRUE)); /* we've processed the message     */
         }
         break;

   }
   return(WinDefDlgProc(hwnd,msg,mp1,mp2));
}

/*****************************************************************************/
/*  function name : InsertDlgIntoNoteBook                                    */
/*                                                                           */
/*  arguments     : Handle of the notebook, Id of the dialog, dialog         */
/*                  procedure, statusline, major tab text, buffer to         */
/*                  update the maximum major tab text size, BKA_MAJOR,       */
/*                  BKA_MINOR or 0 reflecting the page style,                */
/*                  buffer to hold the pagebutton size.                      */
/*                                                                           */
/*  return value  : window handle of the newly created notebook page or      */
/*                  NULLHANDLE in case of an error.                          */
/*                                                                           */
/*  description   : inserts the page into the notebook with the given        */
/*                  handle. The statusline text and the tab text is used     */
/*                  in case of an error, only. On default, the strings       */
/*                  are taken from the resource of the current module. We    */
/*                  figure out the title of the dialog. The first part of    */
/*                  it defines the tab text, the second one the status       */
/*                  line text. The separator is a '|'.                       */
/*                  In case of a major tab this function determines the      */
/*                  size needed to display the tab text. If it needs more    */
/*                  space than the current value these values are updated.   */
/*****************************************************************************/
static HWND InsertDlgIntoNoteBook(HWND pages,USHORT DlgId,PFNWP DlgProc,
                                  PSZ Statusline,PSZ Tabtext,PPOINTL maxtext,
                                  USHORT majorminor,PSHORT pgbutton)
{
   HWND hwndPage;
   ULONG PageId;
   HPS hps;
   POINTL box[TXTBOX_TOPRIGHT + 1];
   MPARAM mp2;
   FONTMETRICS metric;
   char buf[256];
   ULONG size;
   DLGTEMPLATE *pdlg;
   DLGTITEM *pitem;

   mp2 = MPFROM2SHORT((BKA_STATUSTEXTON | BKA_AUTOPAGESIZE | majorminor),
                                                                     BKA_LAST);
   PageId = (ULONG) WinSendMsg(pages,
                               BKM_INSERTPAGE,
                               MPVOID,  /* not used, we call with BKA_LAST   */
                               mp2);
   if (PageId == 0)
      return(NULLHANDLE);

   hwndPage = WinLoadDlg(pages,         /* parent                            */
                         NULLHANDLE,    /* owner                             */
                         DlgProc,
                         0,             /* module                            */
                         DlgId,
                         NULL);         /* creation params                   */
   if (hwndPage == (HWND) 0) {
      WinSendMsg(pages,                 /* in case of an error we delete     */
                 BKM_DELETEPAGE,        /* the newly created page            */
                 MPFROMLONG(PageId),
                 MPFROMSHORT(BKA_SINGLE));
      return(NULLHANDLE);
   }

   if (!WinSendMsg(pages,               /* can't assign the dialog to the    */
                   BKM_SETPAGEWINDOWHWND, /* page?                           */
                   MPFROMLONG(PageId),
                   MPFROMHWND(hwndPage))) {
      WinSendMsg(pages,
                 BKM_DELETEPAGE,
                 MPFROMLONG(PageId),
                 MPFROMSHORT(BKA_SINGLE));
      return(NULLHANDLE);
   }

                                        /* We wanna set the TabText of the   */
                                        /* notebook to the dialog title      */
                                        /* because of NLS. We get this       */
                                        /* information from the system, thus,*/
                                        /* we get the resource from the      */
                                        /* program module and look for the   */
                                        /* title value.                      */
   DosQueryResourceSize(0,
                        RT_DIALOG,
                        DlgId,
                        &size);
   if (size >= sizeof(DLGTEMPLATE)) {   /* values acceptable?                */
      DosGetResource(0,                 /* load dialog                       */
                     RT_DIALOG,
                     DlgId,
                     (PPVOID)(&pdlg));
      if (pdlg->offadlgti + sizeof(DLGTITEM) <= size) {  /* first item       */
                                        /* within valid memory?              */
         pitem = (PDLGTITEM) ((char *) pdlg + pdlg->offadlgti);
                                        /* pitem should be the title element */
         if (pitem->offText + pitem->cchText <= size) {  /* title accessable?*/
            strncpy(buf,(char *) pdlg + pitem->offText,255);
            buf[255] = 0;
            Tabtext = buf;
            if ((Statusline = strchr(buf,'|')) != NULL)
               *Statusline++ = 0;
         }
      }
      DosFreeResource(pdlg);
   }

   if (Statusline != NULL)              /* Statusline defined?               */
      WinSendMsg(pages,
                 BKM_SETSTATUSLINETEXT,
                 MPFROMLONG(PageId),
                 MPFROMP(Statusline));

   if (majorminor == 0)                 /* don't process any tab related     */
      return(hwndPage);                 /* statements und return gracefully  */

   WinSendMsg(pages,                    /* set the new tabtext               */
              BKM_SETTABTEXT,
              MPFROMLONG(PageId),
              MPFROMP(Tabtext));
                                        /* calculate the length and height   */
   hps = WinGetPS(pages);
   if (!GpiQueryFontMetrics(hps,
                            sizeof(metric),
                            &metric))
      FatalWinError((HWND) 0);
   GpiQueryTextBox(hps,strlen(Tabtext),Tabtext,TXTBOX_TOPRIGHT + 1,box);
   box[TXTBOX_TOPRIGHT].x += 2 * metric.lAveCharWidth;   /* some space around*/
   if (maxtext->x < box[TXTBOX_TOPRIGHT].x)
      maxtext->x = box[TXTBOX_TOPRIGHT].x;
   maxtext->y = metric.lMaxBaselineExt + 2 * (metric.lInternalLeading +
                                                      metric.lExternalLeading);
   *pgbutton = (SHORT) metric.lMaxBaselineExt;
   WinReleasePS(hps);
   return(hwndPage);
}

/*****************************************************************************/
/*  function name : StartNoteBook                                            */
/*                                                                           */
/*  arguments     : handle of the notebook dialog window                     */
/*                                                                           */
/*  return value  : window handle of the first notebook page or NULLHANDLE   */
/*                  in case of an error.                                     */
/*                                                                           */
/*  description   : prepares the notebook. All sub-dialogs will be           */
/*                  included and the colors will be set to default dialog    */
/*                  colors.                                                  */
/*****************************************************************************/
static ULONG StartNoteBook(HWND hwnd)
{
   HWND hwndPages;
   POINTL maxmajor,maxminor;
   SHORT pgbutton;
   HWND FirstPage;

   hwndPages = WinWindowFromID(hwnd,NLS(IDDE_NOTEBOOK_PAGES));
   WinSendMsg(hwndPages,
              BKM_SETNOTEBOOKCOLORS,
              MPFROMLONG(SYSCLR_DIALOGBACKGROUND),
              MPFROMSHORT(BKA_BACKGROUNDPAGECOLORINDEX));
   WinSendMsg(hwndPages,
              BKM_SETNOTEBOOKCOLORS,
              MPFROMLONG(SYSCLR_DIALOGBACKGROUND),
              MPFROMSHORT(BKA_BACKGROUNDMAJORCOLORINDEX));
   WinSendMsg(hwndPages,
              BKM_SETNOTEBOOKCOLORS,
              MPFROMLONG(SYSCLR_DIALOGBACKGROUND),
              MPFROMSHORT(BKA_BACKGROUNDMINORCOLORINDEX));
   maxmajor.x = maxmajor.y = 0;
   maxminor.x = maxminor.y = 0;
                                        /* now, insert the pages             */
   if ((FirstPage = InsertDlgIntoNoteBook(hwndPages,
                                          NLS(IDD_CRON_FILE),
                                          CronDlgProc,
                                          "1 (2)", /* not used               */
                                          "1",  /* not used                  */
                                          &maxmajor,
                                          BKA_MAJOR,
                                          &pgbutton)) != NULLHANDLE)
      InsertDlgIntoNoteBook(hwndPages,  /* insert the sub-page on success    */
                            NLS(IDD_CRONTABS),  /* only                      */
                            CrontabsDlgProc,
                            "2 (2)",    /* not used                          */
                            "",         /* not used                          */
                            &maxminor,
                            BKA_MINOR,
                            &pgbutton);

   InsertDlgIntoNoteBook(hwndPages,
                         NLS(IDD_OUTPUT),
                         OutputDlgProc,
                         NULL,
                         "2",           /* not used                          */
                         &maxmajor,
                         BKA_MAJOR,
                         &pgbutton);
   InsertDlgIntoNoteBook(hwndPages,
                         NLS(IDD_TCP),
                         TCPIPDlgProc,
                         NULL,
                         "3",           /* not used                          */
                         &maxmajor,
                         BKA_MAJOR,
                         &pgbutton);
   InsertDlgIntoNoteBook(hwndPages,
                         NLS(IDD_GENERAL),
                         GeneralDlgProc,
                         NULL,
                         "4",           /* not used                          */
                         &maxmajor,
                         BKA_MAJOR,
                         &pgbutton);

   WinSendMsg(hwndPages,                /* set the tab size                  */
              BKM_SETDIMENSIONS,
              MPFROM2SHORT((SHORT) maxmajor.x,(SHORT) maxmajor.y),
              MPFROMSHORT(BKA_MAJORTAB));
   WinSendMsg(hwndPages,                /* set the minor tab size            */
              BKM_SETDIMENSIONS,
              MPFROM2SHORT((SHORT) maxminor.x,(SHORT) maxminor.y),
              MPFROMSHORT(BKA_MINORTAB));
   WinSendMsg(hwndPages,                /* set the button size               */
              BKM_SETDIMENSIONS,
              MPFROM2SHORT(pgbutton,pgbutton),
              MPFROMSHORT(BKA_PAGEBUTTON));
   return(FirstPage);
}

/*****************************************************************************/
/*  function name : ResizePages                                              */
/*                                                                           */
/*  arguments     : frame window handle of the notebook dialog               */
/*                                                                           */
/*  description   : the frame and its client must been formatted.            */
/*                  the function computes the size of the client window      */
/*                  and sets the notebook to the full size.                  */
/*****************************************************************************/
static void ResizePages(HWND hwnd)
{
   SWP swp;
   RECTL rcl;
                                        /* The frame and its client must been*/
                                        /* formatted. (hwnd is the frame)    */
                                        /* Compute the size of the client    */
                                        /* window                            */
   WinQueryWindowPos(hwnd,&swp);
   rcl.xLeft = rcl.xRight = swp.x;
   rcl.yBottom = rcl.yTop = swp.y;
   rcl.xRight += swp.cx;
   rcl.yTop += swp.cy;
                                        /* rcl contents the frame RECTL      */
   WinCalcFrameRect(hwnd,&rcl,TRUE);
                                        /* now it contents the client RECTL  */

                                        /* query the notebook SWP            */
   WinQueryWindowPos(WinWindowFromID(hwnd,NLS(IDDE_NOTEBOOK_PAGES)),&swp);
                                        /* and set it back with the newly    */
                                        /* calculated size                   */
   WinSetWindowPos(WinWindowFromID(hwnd,NLS(IDDE_NOTEBOOK_PAGES)),
                   swp.hwndInsertBehind,
                   swp.x,
                   swp.y,
                   rcl.xRight - rcl.xLeft,
                   rcl.yTop - rcl.yBottom,
                   swp.fl);
}

/*****************************************************************************/
/*  function name : NotebookDlgProc                                          */
/*                                                                           */
/*  arguments     : see WinDefDlgProc                                        */
/*                                                                           */
/*  return value  : see WinDefDlgProc                                        */
/*                                                                           */
/*  description   : see WinDefDlgProc, we process some special events        */
/*                  here. The associated dialog is IDD_NOTEBOOK.             */
/*                  This function services the notebook related windows.     */
/*                                                                           */
/*  note          : don't call directly. This function is called by PM.      */
/*****************************************************************************/
static MRESULT EXPENTRY NotebookDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,
                                                                    MPARAM mp2)
{
   MRESULT result;

   switch (msg) {
      case WM_FORMATFRAME:              /* We want to adjust the client      */
                                        /* dialogs within the notebook to    */
                                        /* the full size.                    */
         result = WinDefDlgProc(hwnd,msg,mp1,mp2);
         ResizePages(hwnd);
         return(result);

      case WM_INITDLG:                  /* create the notebook and post its  */
         WinPostMsg(hwnd,               /* first window handle to ourself to */
                    WM_USER,            /* activate the window after all     */
                    MPFROMLONG(StartNoteBook(hwnd)),  /* initializations     */
                    MPVOID);
         ResizePages(hwnd);
         break;

      case WM_USER:
         WinSetActiveWindow(HWND_DESKTOP,(HWND) mp1);
         return(MPFROMSHORT(TRUE));     /* we've processed the message       */

   }
   return(WinDefDlgProc(hwnd,msg,mp1,mp2));
}

/*****************************************************************************/
/*  function name : DisableMenuEntry                                         */
/*                                                                           */
/*  arguments     : id of the item of the main menu                          */
/*                                                                           */
/*  description   : disables the access to the given menu entry.             */
/*****************************************************************************/
static void DisableMenuEntry(USHORT id)
{
   WinSendMsg(WinWindowFromID(hwndFrame,FID_MENU),
              MM_SETITEMATTR,
              MPFROM2SHORT(id,TRUE),
              MPFROM2SHORT(MIA_DISABLED,MIA_DISABLED));
}

/*****************************************************************************/
/*  function name : ClientWndProc                                            */
/*                                                                           */
/*  arguments     : see WinDefWindowProc                                     */
/*                                                                           */
/*  return value  : see WinDefWindowProc                                     */
/*                                                                           */
/*  description   : see WinDefFileDlgProc, we process some special events    */
/*                  here. The function is called by WinCreateStdWindow.      */
/*                  This is the user visible main process and interface.     */
/*                                                                           */
/*  note          : don't call directly. This function is called by PM.      */
/*****************************************************************************/
MRESULT EXPENTRY ClientWndProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   HWND menu;
#if 0
                                        /* let's catch all messages and look */
                                        /* for an interesting and unprocessed*/
                                        /* message                           */
   DumpMessage(hwnd,msg,mp1,mp2);
#endif
   switch (msg) {
      case WM_CREATE:                   /* we want to initialize the help    */
                                        /* window. I think, it'll look nicer */
                                        /* to open the window before         */
                                        /* displaying any errors...          */
         WinPostMsg(hwnd,WM_USER + 1,MPVOID,MPVOID);
         break;

      case WM_DESTROY:
         DestroyHelpInstance();
         break;

      case WM_CLOSE:                    /* don't close on the first time!    */
         GlobalStop++;                  /* set the stop flag and inform the  */
         if (CronSem != (HEV) 0)        /* cron thread to terminate. On its  */
            DosPostEventSem(CronSem);   /* termination it will send us one   */
         if (pipehandle != -1)          /* more WM_CLOSE. This will be       */
            close(pipehandle);          /* passed und the window and the     */
         pipehandle = -1;               /* process dies. Nice: double close  */
         if (GlobalStop == 1)           /* by the user is processed and we   */
            return(MPFROMLONG(0));      /* die at once.                      */
         break;

      case WM_ERASEBACKGROUND:          /* don't erase! WM_PAINT is send in  */
         return(MPFROMSHORT(FALSE));    /* the following and will do all.    */

      case WM_PRESPARAMCHANGED:         /* one of the presentation parameters*/
                                        /* has changed. We must repaint (and */
                                        /* use the new values) in case of    */
                                        /* foreground and background colors  */
                                        /* and if the font changes. Note:    */
                                        /* SysValue changes causes a WM_PAINT*/
                                        /* Thus, there is no need to look for*/
                                        /* a WM_SYSVALUECHANGED or a         */
                                        /* WM_SYSCOLORCHANGE                 */
         if (((ULONG) mp1 == PP_FOREGROUNDCOLOR) ||
             ((ULONG) mp1 == PP_BACKGROUNDCOLOR) ||
             ((ULONG) mp1 == PP_FONTNAMESIZE))
            RePaintAll(hwnd,msg);
         break;

      case WM_PAINT:                    /* repaint the complete window       */
         RePaintAll(hwnd,msg);
         return(MPFROMLONG(0));

      case WM_USER:                     /* One state of the user visible     */
         if ((ULONG) mp1 != WINDOW_REFRESH)  /* states has changed. Show it. */
            break;
         RePaintLine(hwnd,(int) mp2);   /* mp2 is the number of the status   */
         return(MPFROMLONG(0));         /* line which contents has changed   */

      case WM_USER + 1:                 /* initialize the help window        */
         if (!InitHelp()) {
            DisableMenuEntry(NLS(IDME_HELPINDEX));
            DisableMenuEntry(NLS(IDME_HELPGENERAL));
            DisableMenuEntry(NLS(IDME_HELPUSINGHELP));
            DisableMenuEntry(NLS(IDME_HELPKEYS));
         }
         return(MPFROMLONG(0));

      case WM_INITMENU:
         if (ProgramFlags & PRG_OUTPUT_IS_NUL)
            DisableMenuEntry(NLS(IDME_NEXTFILE));
         else
            WinSendMsg((HWND) mp2,
                       MM_SETITEMATTR,
                       MPFROM2SHORT(NLS(IDME_NEXTFILE),TRUE),
                       MPFROM2SHORT(MIA_DISABLED,0));
         break;

      case WM_MOVE:                     /* save the new window position      */
         SetPosOfWindow(hwndFrame);
         break;

      case HM_QUERY_KEYS_HELP:
                                        /*return id of key help panel        */
          return((MRESULT) (unsigned) NLS(IDME_HELPKEYS));
          break;

      case WM_COMMAND:
         menu = WinWindowFromID(hwndFrame,FID_MENU);
         switch(NONLS(LOUSHORT(mp1))) {
            case IDME_NEXTFILE:         /* set next output file              */
               BlockOutput();
               if (NextOutputFile(0) == -1) {
                  UnBlockOutput();
                  Error(Get(IDS_NoOutputFile),(HWND) 0);
               } else
                  UnBlockOutput();
               break;

            case IDD_NOTEBOOK:          /* Start the Notebook                */
               if (WinDlgBox(HWND_DESKTOP,
                             hwndFrame,
                             NotebookDlgProc,
                             0,
                             NLS(IDD_NOTEBOOK),
                             (PSZ) NULL) == DID_ERROR)
                  WinError((HWND) 0);
               break;

            case IDME_HELPINDEX:
               DefaultHelp(HM_HELP_INDEX);
               break;

            case IDME_HELPGENERAL:
               DefaultHelp(HM_EXT_HELP);
               break;

            case IDME_HELPUSINGHELP:
               DefaultHelp(HM_DISPLAY_HELP);
               break;

            case IDME_HELPKEYS:
               DefaultHelp(HM_KEYS_HELP);
               break;

            case IDD_ABOUT:             /* show the about box                */
               if (WinDlgBox(HWND_DESKTOP,
                             hwndFrame,
                             WinDefDlgProc,
                             0,
                             NLS(IDD_ABOUT),
                             (PSZ) NULL) == DID_ERROR)
                  WinError((HWND) 0);
               break;

         }
         break;
   }
   return(WinDefWindowProc(hwnd,msg,mp1,mp2));
}

/*****************************************************************************/
/*  function name : PMNewProgramStatus                                       */
/*                                                                           */
/*  arguments     : ID of the resource string which should be display as     */
/*                  the new program status, use IDS_Status...-strings        */
/*                                                                           */
/*  description   : displays the new program status (normal, at startup,     */
/*                  etc.) on the program status line.                        */
/*****************************************************************************/
void PMNewProgramStatus(ULONG NewStatus)
{                                       /* string acceptable?                */
   if ((NewStatus >= IDS_StatusAtStartup) &&
                                      (NewStatus <= IDS_StatusCommunication)) {
      CurrentState = NewStatus;
      if (hwndClient != 0)              /* client is up? send the client:    */
                                        /* line 0 has changed                */
         WinPostMsg(hwndClient,WM_USER,(MPARAM) WINDOW_REFRESH,(MPARAM) 0);
   }
}

/*****************************************************************************/
/*  function name : PMNewStartTime                                           */
/*                                                                           */
/*  arguments     : time of the job that will be started next.               */
/*                                                                           */
/*  description   : displays the time of the next starting job on the        */
/*                  related display line. The time may be (time_t) -1. In    */
/*                  this case there are either no periodically running       */
/*                  jobs or no job time is in the computing distance (24-    */
/*                  48 hours)                                                */
/*****************************************************************************/
void PMNewStartTime(time_t time)
{
   NextStartTime = time;
   if (hwndClient != 0)                 /* client is up? send the client:    */
                                        /* line 1 has changed                */
      WinPostMsg(hwndClient,WM_USER,(MPARAM) WINDOW_REFRESH,(MPARAM) 1);
}

/*****************************************************************************/
/*  function name : PMNewOutputFile                                          */
/*                                                                           */
/*  description   : displays the current output file on the related          */
/*                  display line.                                            */
/*****************************************************************************/
void PMNewOutputFile(void)
{
   if (hwndClient != 0)                 /* client is up? send the client:    */
                                        /* line 1 has changed                */
      WinPostMsg(hwndClient,WM_USER,(MPARAM) WINDOW_REFRESH,(MPARAM) 3);
}

/*****************************************************************************/
/*  function name : PMStopPM                                                 */
/*                                                                           */
/*  description   : Send the PM window a WM_CLOSE. The program should        */
/*                  terminate. The window has already received a WM_CLOSE    */
/*                  and will die now since StopPM is called from the         */
/*                  daemon thread.                                           */
/*                                                                           */
/*  note          : don't call if you don't know what you are doing!         */
/*****************************************************************************/
void PMStopPM(void)
{
   WinPostMsg(hwndClient,WM_CLOSE,MPVOID,MPVOID);
}

/*****************************************************************************/
/*  function name : PMJobsModified                                           */
/*                                                                           */
/*  description   : Send the notebook dialog "Crontabs" a WM_USER. This      */
/*                  will advise it to refresh since the list of jobs has     */
/*                  been changed.                                            */
/*****************************************************************************/
void PMJobsModified(void)
{
   if (CrontabsWindow != (HWND) 0)
      WinPostMsg(CrontabsWindow,WM_USER,MPVOID,MPVOID);
}

/*****************************************************************************/
/*  function name : PMStop                                                   */
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
void PMStop(int code)
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
   FatalError(Get(IDS_DieOnSigSig),(HWND) 0);
}

/* RCS depending informations
 *
 * $Name: Version121 $
 *
 * $Log: cron_pm.c $
 * Revision 1.6  1996/05/09 09:59:33  Florian
 * Problems with blanks in the help filename fixed.
 *
 * Revision 1.5  1995/10/18 09:46:08  Florian
 * Some cosmetic changes, of course.
 * Used font, colors and window state will be reused on restart.
 * Determination and displaying of possible output drives rewritten.
 * Crontabs window enhanced, user may change entries and may use the
 * clipboard, pushbutton selection changed to possible values.
 *
 * Revision 1.4  1995/08/03 07:43:46  Florian
 * NLS supported (English and German)
 *
 * Revision 1.3  1995/03/06 11:52:11  Florian
 * Many bugs fixed.
 * Fully supported online help.
 * Notebook layout revised.
 *
 * Revision 1.2  1995/02/20 12:53:23  Florian
 * All dialogs are placed into a notebook.
 * Some bugs fixed.
 *
 * Revision 1.1  1995/02/03 10:42:25  Florian
 * Initial revision
 *
 *
 */
static char rcsid[] = "@(#)$Id: cron_pm.c 1.6 1996/05/09 09:59:33 Florian Exp $";
