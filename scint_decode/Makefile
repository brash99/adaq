# Makefile for analysis of fastbus data in SBS test stand
#
# Use this if compiling online code (ET system)
# User must have LD_LIBRARY_PATH = $CODA/$OSNAME/lib:$LD_LIBRARY_PATH
# export ONLINE = 1

# Use this if profiling (note: it slows down the code)
# export PROFILE = 1
# 
# HA_INSTALL is location of Hall A Analyzer
#
HA_INSTALL := /home/adaq/analyzer

# To make standalone, independent of root CINT macros
export STANDALONE = 1

MACHINE := $(shell uname -s)
ARCH	:= linux
ifeq ($(MACHINE),Darwin)
	ARCH := macosx
endif
   
ifdef OLDROOT
   ROOTLIBS      = -L$(ROOTSYS)/lib -lNew -lBase -lCint -lClib -lCont -lFunc \
                    -lGraf -lGraf3d -lHist -lHtml -lMatrix -lMeta -lMinuit -lNet \
                    -lPhysics -lPostscript -lProof -lRint -lTree -lUnix -lZip
   ROOTGLIBS     = -lGpad -lGui -lGX11 -lX3d
else
   ROOTLIBS      = $(shell root-config --libs)
   ROOTGLIBS     = $(shell root-config --glibs)
endif

EVIO_INSTALL := $(HA_INSTALL)/evio/evio-4.4.6
EVIO_ARCH := $(shell uname -s)-$(shell uname -m)
export EVIO_LIBDIR := $(EVIO_INSTALL)/$(EVIO_ARCH)/lib
export EVIO_INCDIR := $(EVIO_INSTALL)/$(EVIO_ARCH)/include
export HA_INCLUDES := $(HA_INSTALL)/hana_decode

ifeq ($(ARCH),linux)

   MODERNGPPVERSION := $(shell expr `g++ -dumpversion` \> 4.4.7)

# Linux with egcs
   INCLUDES      = -I$(ROOTSYS)/include 
   INCLUDES	+= -I$(EVIO_INCDIR)
   INCLUDES	+= -I$(HA_INCLUDES)
   CXX           = g++
   ifneq ($(MODERNGPPVERSION),1)
   	CXXFLAGS      = -O -Wall  -fno-exceptions -std=c++0x -fPIC $(INCLUDES)
   	CXXFLAGS     += -Wno-deprecated 
   else
   	CXXFLAGS      = -O -Wall  -fno-exceptions -std=c++11 -fPIC $(INCLUDES)
   	CXXFLAGS     += -Wno-deprecated
   endif

   LD            = g++
   LDFLAGS       = 
   SOFLAGS       = -shared

   ifdef OLDROOT
      LIBS          = $(ROOTLIBS) -lm -ldl -rdynamic
      GLIBS         = $(ROOTLIBS) $(ROOTGLIBS) -L/usr/X11R6/lib \
                      -lXpm -lX11 -lm -ldl -rdynamic
      CXXFLAGS     += -DOLDROOT
   else
      LIBS          = $(ROOTLIBS)
      GLIBS         = $(ROOTGLIBS) -L/usr/lib -lXpm -lX11
   endif

   HA_LIBS = -L$(HA_INSTALL) -lHallA -ldc 
   EVIO_LIB= -L$(EVIO_LIBDIR) -levio
   ALL_LIBS = $(HA_LIBS) $(EVIO_LIB) $(GLIBS) $(ROOTLIBS) 

# ONLIBS is needed for ET
   ET_AC_FLAGS = -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS
   ET_CFLAGS = -02 -fPIC $(ET_AC_FLAGS) -DLINUXVERS
# CODA may be an environment variable.  Typical examples
#  CODA = /adaqfs/coda/2.2
#  CODA = /data7/user/coda/2.2
   LIBET = $(CODA)/Linux/lib/libet.so
   ONLIBS = $(LIBET) -lieee -lpthread -ldl -lresolv

   ifdef ONLINE
     ALL_LIBS += $(ONLIBS)
   endif

   ifdef PROFILE
     CXXFLAGS += -pg
   endif

endif

ifeq ($(ARCH),macosx)

   MODERNGPPVERSION := $(shell expr `clang++ -dumpversion` \>= 4.2.1)

   ROOTINC	 := $(shell root-config --incdir)
   INCLUDES      = -I$(ROOTINC)
   CXX           = clang++
   ifneq ($(MODERNGPPVERSION),1)
   	CXXFLAGS      = -O -Wall  -Wno-c++11-narrowing -Woverloaded-virtual -pthread -std=c++0x -stdlib=libc++ -fPIC $(INCLUDES)
   	CXXFLAGS     += -Wno-deprecated 
   else
   	CXXFLAGS      = -O -Wall  -Wno-c++11-narrowing -Woverloaded-virtual -pthread -std=c++11 -stdlib=libc++ -fPIC $(INCLUDES)
   	CXXFLAGS     += -Wno-deprecated
   endif

   LD            = clang++
   LDFLAGS       = 
   SOFLAGS       = -shared

   ifdef OLDROOT
      LIBS          = $(ROOTLIBS) -lm -ldl -rdynamic
      GLIBS         = $(ROOTLIBS) $(ROOTGLIBS) -L/usr/X11R6/lib \
                      -lXpm -lX11 -lm -ldl -rdynamic
      CXXFLAGS     += -DOLDROOT
   else
      LIBS          = $(ROOTLIBS)
      GLIBS         = $(ROOTGLIBS)
   endif

   ALL_LIBS = $(EVIO_LIBDIR)/libevio.dylib $(GLIBS) $(ROOTLIBS) 

endif

ifdef STANDALONE 
  CXXFLAGS += -DSTANDALONE
endif

all: fbanareal

fbanareal: Fastbus_main1.o 
	$(CXX) -g $(CXXFLAGS) -o $@ Fastbus_main1.C $(ALL_LIBS) 

# END of EDITED

# Below is the evio library, which comes rather directly 
# from CODA group with minor tweaking by R. Michaels & O. Hansen.

clean:  
	rm -f *.o *.a core *~ *.d *.out adcana

realclean:  clean
	rm -f *.d

.SUFFIXES:
.SUFFIXES: .c .cc .cpp .C .o .d

%.o:	%.C
	$(CXX) $(CXXFLAGS) -c $<

%.d:	%.C
	@echo Creating dependencies for $<
	@$(SHELL) -ec '$(CXX) -MM $(CXXFLAGS) -c $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

-include $(DEPS)








