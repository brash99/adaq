# Boot file for CODA 2.2 VME Crate
# Motorola 2300 version

roc_tid = 1

hostAdd "sbs1.jlab.org","129.57.36.67"
hostAdd "sbs1","129.57.36.67"

# Add Routes for Multicast Support
mRouteAdd("224.0.0.0","129.57.36.0",0xf0000000,0,0)

# Setup environment / load coda_roc
putenv "MSQL_TCP_HOST=sbs1"
putenv "EXPID=sbsfb"
putenv "TCL_LIBRARY=/site/coda/2.6.2/common/lib/tcl7.4"
putenv "ITCL_LIBRARY=/site/coda/2.6.2/common/lib/itcl2.0"
putenv "DP_LIBRARY=/site/coda/2.6.2/common/lib/dp"
putenv "TOKEN_PORT=5677"

# Define CMLOG Server PORT (Halla 8228 - rich.jlab.org)
putenv "CMLOG_PORT=8228"

# Get around Clock returning 0 bug
mytv = malloc(8)
clock_gettime(0,mytv)


buf = malloc (21*4*512)


ld < /home/adaq/universeDma/universeDma.o.vw54

sysVmeDmaShow();

#initialize (1 no interrupts) or (0 interrupts enabled)
sysVmeDmaInit(0) 
# Set for 64bit PCI transfers
sysVmeDmaSet(4,1)
# A24 VME Slave (1) or A32 Slave (2)
sysVmeDmaSet(11,2)
# BLK32 (4) or MBLK(64) (5) VME transfers
sysVmeDmaSet(12,4)

ld < /site/coda/2.6.2/cMsg/vxworks-ppc/lib/libcmsgRegex.o
ld < /site/coda/2.6.2/cMsg/vxworks-ppc/lib/libcmsg.o
ld < /site/coda/2.6.2/cMsg/vxworks-ppc/lib/libcmsgxx.o


# download ROC

ld < /site/coda/2.6.2/VXWORKSPPC55/bin/coda_roc_rc3.5


# Load sfi fb
ld < /site/coda/2.6.2/VXWORKSPPC55/lib/libsfifb.o
ADDR=0
sysBusToLocalAdrs(0x39,0xe00000,&ADDR)
fb_init_1(ADDR)

vmeSetMaximumVMESlots=30

ld < /home/adaq/vxworks/test_1877.o

LSWAP=1 
#for the old version of firmware
#ld < /home/adaq/newti/ti/tiLib.o

# for the updated firmware (0x51)
ld < /home/adaq/tid/ti/tiLib.o

ld < /home/adaq/CMLOG/2.1/bin/ppc-vw55/cmlogClientD
# start up tasks 
sp cmlogClientD
taskDelay(60*10)

taskSpawn "ROC",200,spTaskOptions,200000,coda_roc,"-session","sbsfb1","-objects","ROC4 ROC"

#taskSuspend roc_tid







