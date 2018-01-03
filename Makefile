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
CFLAGS=-std=c++11 -I src/libsrc -I src/test -O3
LIBS= pthread
LDOPTS=$(patsubst %, -l%, $(LIBS))


TESTFMCKPDSRCS=$(wildcard src/testFMCKeypad/*.cpp)
TESTFMCKPDOBJS=$(patsubst %.cpp, %.o, $(TESTFMCKPDSRCS))
TESTFMCKPDLIBS=wiringPi
TESTFMCKPDEXE=testFMCKeypad


TESTFMCLEDSRCS=$(wildcard src/testFMCLED/*.cpp)
TESTFMCLEDOBJS=$(patsubst %.cpp, %.o, $(TESTFMCLEDSRCS))
TESTFMCLEDLIBS=wiringPi
TESTFMCLEDEXE=testFMCLED

world: all

%.o:%cpp
	@echo "\t[CC] $<"
	$(CC) $(CFLAGS) -c -o $@ $<


testFMCKeypad: $(TESTFMCKPDOBJS)
	$(CC) -o $@ $(TESTFMCKPDOBJS) $(patsubst %, -l%, $(TESTFMCKPDLIBS))

testFMCLED: $(TESTFMCLEDOBJS)
	$(CC) -o $@ $(TESTFMCLEDOBJS) $(patsubst %, -l%, $(TESTFMCLEDLIBS))

all: testFMCLED testFMCKeypad

install: testFMCLED testFMCKeypad
	install -D testFMCLED $(PREFIX)/bin/testFMCLED
	install -D testFMCKeypad $(PREFIX)/bin/testFMCKeypad

clean: 
	rm -f $(TESTFMCLEDOBJS) $(TESTFMCKPDOBJS) testFMCLED testFMCKeypad
	
distclean: clean


