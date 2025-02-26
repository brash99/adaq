#
# File:
#    Makefile
#
# Description:
#    Makefile for miscellaneous command line programs that access the
#    VMEbus using a Intel-Based Controller running Linux.
#
#

LINUXVME_BIN	?= ../lib
LINUXVME_LIB	?= ../lib
LINUXVME_INC	?= ../include

CROSS_COMPILE		=
CC			= $(CROSS_COMPILE)gcc
AR                      = ar
RANLIB                  = ranlib
CFLAGS			= -Wall -g -I${LINUXVME_INC} -I. -I/usr/include \
			  -L${LINUXVME_LIB} -L.

PROGS			= vme_display vme_modmem vme_sysreset

all: $(PROGS)

clean distclean:
	@rm -f $(PROGS) *~ *.so

%: %.c
	$(CC) $(CFLAGS) -o $@ $(@:%=%.c) -ljvme -ldl

install: $(PROGS)
	cp -v $(PROGS) $(LINUXVME_BIN)/

links: ${PROGS}
	for file in $(PROGS); do \
	ln -sf ${PWD}/$${file} ${LINUXVME_BIN}/ ; \
	done

.PHONY: all clean distclean
