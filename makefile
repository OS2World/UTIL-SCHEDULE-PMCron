# Copyright (c) 1995 Florian Groáe-Coosmann, RCS section at the eof
# this makefile is for GNU's make with gcc under OS/2, only
#
# set the default model, valid values are debug and release, it may be
# overwritten by calling the targets debug and release

ifndef croncode
  croncode=debug
endif

.SUFFIXES: .dlg .rc .res .def .ico .h .ipf .hlp
.PHONY: all debug release run rundet

# programs and flags for compiler and linker:
CC=gcc
LD=gcc
CFLAGSALL=-Wall -I. -funsigned-char -m486
LFLAGSALL=-lsocket
# depending on the "debug"-value:
DEBUGgcc=-g -DDEBUG -O0
RELEASEgcc=-O6 -s
DEBUGlink=-g
RELEASElink=-O6 -s -lwrap
MULTI=-Zmtd -Zomf -Zcrtdll

ifeq ($(strip $(croncode)),debug)
  CFLAGS=$(CFLAGSALL) $(DEBUGgcc)
  LFLAGS=$(LFLAGSALL) $(DEBUGlink)
  OPATH=objd
else
  CFLAGS=$(CFLAGSALL) $(RELEASEgcc)
  LFLAGS=$(LFLAGSALL) $(RELEASElink)
  OPATH=obj
endif

###############################################################################
# standard rules

$(OPATH)/%.o : %.c
	$(CC) $(CFLAGS) $(MULTI) -c -o $@ $<

%:: RCS/%
	+$(patsubst $@-noexist,co $< $@,$(filter-out $@,$(firstword $(wildcard $@) $@-noexist)))
# note: this command has been stolen from the builtin command of GNU's make
# to extract file of the pattern "RCS/%,v"

##############################################################################
# definition of the modules:
MAIN=$(OPATH)/cronmain.o
CDET=$(OPATH)/cron_det.o
CPM=$(OPATH)/cron_pm.o
SERV=$(OPATH)/server.o
TABS=$(OPATH)/tables.o
PROC=$(OPATH)/process.o
TCPI=$(OPATH)/tcpip.o
CTAB=$(OPATH)/crontab.o

all: crond.exe crontab.exe cronddeu.hlp crondeng.hlp crondfra.hlp

OBJS=$(MAIN) $(CDET) $(CPM) $(SERV) $(TABS) $(PROC) $(TCPI)

crond.exe: $(OBJS) crond.res crond.def
	-kill crond
	$(LD) $(MULTI) $(LFLAGS) -o $@ $^

crontab.exe: $(CTAB) crontab.res crontab.def
	$(LD) $(LFLAGS) $(CTAB) -o $@ crontab.res crontab.def -lsocket

cronddeu.hlp: cronddeu.ipf
	ipfc /W3 /LANGUAGE=DEU /COUNTRY=049 /CODEPAGE=850 $^
	@-ren cronddeu.HLP cronddeu.hlp

crondeng.hlp: crondeng.ipf
	ipfc /W3 /LANGUAGE=ENG /COUNTRY=001 /CODEPAGE=850 $^
	@-ren crondeng.HLP crondeng.hlp

crondfra.hlp: crondfra.ipf
	ipfc /W3 /LANGUAGE=FRA /COUNTRY=033 /CODEPAGE=850 $^
	@-ren crondfra.HLP crondfra.hlp

server.h: common.h cron_id.h
	touch $@

client.h: common.h
	touch $@

$(MAIN): server.h

$(CDET): server.h

$(CPM): server.h

$(SERV): server.h

$(TABS): server.h

$(PROC): server.h

$(TCPI): server.h

$(CTAB): crontab.c client.h
	$(CC) $(CFLAGS) -DTCPIP_SUPPORT -c -o $@ crontab.c

crond.res: crond.rc help.rc cron_id.h crond.dlg crond.ico
	@-chcp 850
	rc -r -i $(C_INCLUDE_PATH) -i . crond.rc
	ren *.RES *.res

crontab.res: crontab.rc crontab.ico
	rc -r -i $(C_INCLUDE_PATH) -i . crontab.rc
	ren *.RES *.res

rundet: all
	-del crond.out
	start /N /FS crond

run: all
	-del crond.out
	start /N /PM crond.exe

debug:
	+$(MAKE) "croncode=debug" all

release:
	+$(MAKE) "croncode=release" all

# RCS depending informations
#
# $Id: makefile 1.7 1995/10/18 09:46:09 Florian Rel $
#
# $Name: Version121 $
#
# $Log: makefile $
# Revision 1.7  1995/10/18 09:46:09  Florian
# Country and codepage setting for compiling IPFs set.
# Added support of the French language.
#
# Revision 1.6	1995/08/03 07:44:15  Florian
# NLS supported (English and German)
#
# Revision 1.5	1995/03/30 17:01:28  Florian
# .HLP renamed to .hlp, may cause an error on HPFS when using GNU make.
#
# Revision 1.4	1995/03/15 09:07:34  Florian
# Some minor bugs fixed.
# TCP/IP support added.
#
# Revision 1.3	1995/03/06 11:52:18  Florian
# Many bugs fixed.
# Fully supported online help.
# Notebook layout revised.
#
# Revision 1.2	1995/02/20 12:53:23  Florian
# All dialogs are placed into a notebook.
# Some bugs fixed.
#
# Revision 1.1	1995/02/04 13:08:25  Florian
# Initial revision
#
#
