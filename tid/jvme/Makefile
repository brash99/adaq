#
# File:
#    Makefile
#
# Description:
#    Makefile for the JLAB extensions to the VME Controller API (GEFANUC and others)
#
#
#DEBUG=1
OS=VXWORKS
ifndef ARCH
	ifdef LINUXVME_LIB
		ARCH=$(shell uname -m)
		OS=Linux
	else
		ARCH=VXWORKSPPC
		OS=VXWORKS
	endif
endif

# Defs and build for VxWorks
ifeq ($(OS),VXWORKS)
VXWORKS_ROOT = /site/vxworks/5.5/ppc/target

CC			= ccppc
LD			= ldppc
AR			= arppc
RANLIB			= ranlibppc
DEFS			= -mcpu=604 -DCPU=PPC604 -DVXWORKS -D_GNU_TOOL -mlongcall \
				-fno-for-scope -fno-builtin -fvolatile -DVXWORKSPPC \
				-fstrength-reduce
ifdef DEBUG
DEFS  			+= -Wall -g
endif
INCS			= -I. -I$(VXWORKS_ROOT)/h -I$(VXWORKS_ROOT)/h/rpc -I$(VXWORKS_ROOT)/h/net
CFLAGS			= $(INCS) $(DEFS)

SRC			= jvme.c
HDRS			= ${SRC:.c=.h}
OBJ			= ${SRC:.c=.o}
LIBS			= vxjvme.o vxjvme_univ.o

all: $(LIBS)

jvme.o: jvme.c
	$(CC) $(CFLAGS) -c -o $@ $<

jvme_univ.o: jvme.c
	$(CC) $(CFLAGS) -c -o $@ -DVXWORKS_UNIV $<

usrTempeDma.o: usrTempeDma.c sysTempeDma.c tempe.h
	$(CC) $(CFLAGS) -c -o $@ usrTempeDma.c 

universeDma.o: universeDma.c universeDma.h
	$(CC) $(CFLAGS) -c -o $@ universeDma.c 

vxjvme.o: jvme.o usrTempeDma.o
	$(LD) -o $@ -r jvme.o usrTempeDma.o

vxjvme_univ.o: jvme_univ.o universeDma.o
	$(LD) -o $@ -r jvme_univ.o universeDma.o

clean:
	@rm -vf *.o *.a

endif #OS=VXWORKS

# Defs and build for Linux
ifeq ($(OS),Linux)
# Some LINUXVME install defaults, if not already defined.
LINUXVME_LIB		?= ../lib
LINUXVME_INC		?= ../include

CROSS_COMPILE		=
CC			= $(CROSS_COMPILE)gcc
AR                      = ar
RANLIB                  = ranlib
ifdef DEBUG
CFLAGS			= -Wall -g
else
CFLAGS			= -O2
endif
CFLAGS		       += -I. -I/usr/include
CFLAGS                 += -DGEFANUC -DARCH_${ARCH}=1

SRC			= jvme.c jlabgef.c jlabgefDMA.c dmaPList.c

OBJS			= $(SRC:%.c=%.o)
DEPS			= $(SRC:%.c=%.d)

LIBS			= libjvme.a
SHLIBS			= libjvme.so

ifeq ($(ARCH),x86_64)
	libdir:=/usr/lib64/gef
else
	libdir:=/usr/lib/gef
endif

# End in error, if the gefvme-kmod-common RPM is not installed.
ifeq (,$(wildcard $(libdir)/libvme.a))
$(error gefvme-kmod-common RPM not installed!)
endif

all: $(SHLIBS) $(LIBS)

libjvme.so: $(SRC) $(libdir)/libvme.a Makefile
	$(AR) x $(libdir)/libvme.a
	$(CC) -fpic -shared $(CFLAGS) -lrt -lpthread -o $@ $(SRC) gefcmn_*.o
	@rm gefcmn_*.o

libjvme.a: $(OBJS) $(libdir)/libvme.a Makefile
	$(AR) x $(libdir)/libvme.a
	$(AR) rv $@ $(OBJS)
	$(AR) rv $@ gefcmn_*.o
	@rm gefcmn_*.o

clean distclean:
	@rm -f $(OBJS) $(DEPS) $(LIBS) $(SHLIBS) *~

links: libjvme.a libjvme.so
	ln -sf $(PWD)/libjvme.so $(LINUXVME_LIB)/libjvme.so
	ln -sf $(PWD)/libjvme.a $(LINUXVME_LIB)/libjvme.a
	ln -sf $(PWD)/dmaPList.h $(LINUXVME_INC)/dmaPList.h
	ln -sf $(PWD)/jvme.h $(LINUXVME_INC)/jvme.h

install: libjvme.a libjvme.so
	cp -v $(PWD)/libjvme.so $(LINUXVME_LIB)/libjvme.so
	cp -v $(PWD)/libjvme.a $(LINUXVME_LIB)/libjvme.a
	cp -v $(PWD)/dmaPList.h $(LINUXVME_INC)/dmaPList.h
	cp -v $(PWD)/jvme.h $(LINUXVME_INC)/jvme.h

codainstall: checkcoda libjvme.a libjvme.so
	cp -v $(PWD)/libjvme.so $(CODA_LIB)/libjvme.so
	cp -v $(PWD)/libjvme.a $(CODA_LIB)/libjvme.a
	cp -v $(PWD)/dmaPList.h $(CODA)/common/include/dmaPList.h
	cp -v $(PWD)/jvme.h $(CODA)/common/include/jvme.h

checkcoda:
ifndef CODA
	$(error CODA Environment Variable not defined!)
endif

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.d: %.c
	$(CC) -M $(CFLAGS) $< > $@

-include $(DEPS)

.PHONY: all clean distclean checkcoda
endif #OS=Linux
