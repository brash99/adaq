#
# Description:  Makefile for sysVmeDma Library for the Universe Chip
#   This driver is specific to the MVME 2300/2600/3600 Series of CPUs.
#   Compiling requires access to vxWorks headers.
#

#Check Operating system we are using
ifndef OSTYPE
  OSTYPE := $(subst -,_,$(shell uname))
endif

ifndef ARCH
  ARCH = VXWORKSPPC
endif

ifeq ($(OSTYPE),SunOS)
LIBDIR = $(CODA)/$(ARCH)/lib
CFLAGS = -O $(DEFS)

ifeq ($(ARCH),VXWORKSPPC)
INCDIR=$(WIND_BASE)/target/h
CC = ccppc

DEFS = -mcpu=604 -DCPU=PPC604 -DVXWORKS -D_GNU_TOOL -DVXWORKSPPC -DEXTENDED_VME -DMV5100
#DEFS = -mcpu=604 -DCPU=PPC604 -DVXWORKS -D_GNU_TOOL -DVXWORKSPPC -DEXTENDED_VME
#DEFS = -mcpu=604 -DCPU=PPC604 -DVXWORKS -D_GNU_TOOL -DVXWORKSPPC

INCS = -fno-for-scope -fno-builtin -fvolatile -fstrength-reduce -mlongcall -I. -I$(INCDIR)

endif

endif

all: universeDma.o

universeDma.o: universeDma.c universeDma.h 
	$(CC) -c $(CFLAGS) $(INCS) -o $@ universeDma.c

clean:
	rm -f *.o

