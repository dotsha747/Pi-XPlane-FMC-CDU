# This is the name of the project. We get it from the basename of 
# the project folder. It may contain mixed-case, and "-" as word
# separators. CamelCase is discouraged.
#
# The MAJORVER and MINORVER here are place holders; during a 
# gitpkgtool build they are substituted in-place
# by patchVersion.sh.

CWD=$(shell pwd)
PROJECT=$(shell basename $(CWD))
MAJORVER=1
MINORVER=0

# The debian package name. It may contain "-" as a word seperator
# but must be all lower case.

PACKAGE=$(shell echo $(PROJECT) | tr A-Z a-z)

# DESTDIR is overridden by debhelper.
PREFIX=$(DESTDIR)/usr
CC=g++
AR=ar
CFLAGS=-std=c++11 -O3 -I. -g
LIBS= pthread
LDOPTS=$(patsubst %, -l%, $(LIBS))
ARCH=$(shell arch)


TESTFMCKPDSRCS=$(wildcard src/testFMCKeypad/*.cpp)
TESTFMCKPDOBJS=$(patsubst %.cpp, %.o, $(TESTFMCKPDSRCS))
TESTFMCKPDLIBS=wiringPi
TESTFMCKPDEXE=testFMCKeypad


TESTFMCLEDSRCS=$(wildcard src/testFMCLED/*.cpp)
TESTFMCLEDOBJS=$(patsubst %.cpp, %.o, $(TESTFMCLEDSRCS))
TESTFMCLEDLIBS=wiringPi
TESTFMCLEDEXE=testFMCLED

MAINSRCS=$(wildcard src/piXPlaneFMCCDU/*.cpp)
MAINOBJS=$(patsubst %.cpp, %.o, $(MAINSRCS))
MAINLIBS=pthread SDL2 SDL2_ttf freetype z png pthread XPlaneUDPClient wiringPi XPlaneExtPlaneClient 
#bcm_host vchiq_arm vcos
MAINLDOPTS=-L /opt/vc/lib
MAINEXE=piXPlaneFMCCDU

world: all

.cpp.o:
	@echo "\t[CC] $<"
	$(CC) $(CFLAGS) -c -o $@ $<


$(TESTFMCKPDEXE): $(TESTFMCKPDOBJS)
	$(CC) -o $@ $(TESTFMCKPDOBJS) $(patsubst %, -l%, $(TESTFMCKPDLIBS))

$(TESTFMCLEDEXE): $(TESTFMCLEDOBJS)
	$(CC) -o $@ $(TESTFMCLEDOBJS) $(patsubst %, -l%, $(TESTFMCLEDLIBS))
	
$(MAINEXE): $(MAINOBJS)
	$(CC) -o $@ $(MAINOBJS) $(MAINLDOPTS) $(patsubst %, -l%, $(MAINLIBS))

all: $(TESTFMCKPDEXE) $(TESTFMCLEDEXE) $(MAINEXE) 


install: $(TESTFMCLEDEXE) $(TESTFMCKPDEXE) $(MAINEXE)
	install -D $(TESTFMCKPDEXE) $(PREFIX)/bin/$(TESTFMCKPDEXE)
	install -D $(TESTFMCLEDEXE) $(PREFIX)/bin/$(TESTFMCLEDEXE)
	install -D $(MAINEXE) $(PREFIX)/bin/$(MAINEXE)
	install -D root/etc/systemd/system/piXPlaneFMCCDU.service $(PREFIX)/../etc/systemd/system/piXPlaneFMCCDU.service

clean: 
	rm -f $(TESTFMCLEDOBJS) $(TESTFMCKPDOBJS) $(MAINOBJS) $(TESTFMCKPDEXE) $(TESTFMCLEDEXE) $(MAINEXE)
	
distclean: clean


