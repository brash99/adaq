# Boot file for CODA 2.2 VME Crate
# Motorola 2300 version

roc_tid = 1

hostAdd "rich.jlab.org","129.57.36.81"
hostAdd "rich","129.57.36.81"

hostAdd "shortrange-tc","129.57.36.16"
hostAdd "shortrange-tc.jlab.org","129.57.36.16"
hostAdd "eel-18.jlab.org","129.57.36.16"
hostAdd "eel-18","129.57.36.16"
hostAdd "eel-8.jlab.org","129.57.36.247"
hostAdd "eel-8","129.57.36.247"
hostAdd "yer122.jlab.org","129.57.36.45"
hostAdd "yer122","129.57.36.45"
hostAdd "atedf3.jlab.org","129.57.37.107"
hostAdd "atedf3","129.57.37.107"

# Add Routes for Multicast Support
mRouteAdd("224.0.0.0","129.57.36.0",0xf0000000,0,0)

# Setup environment / load coda_roc
putenv "MSQL_TCP_HOST=atedf3"
putenv "EXPID=atedf"
putenv "TCL_LIBRARY=/home/vx/2.6.2/common/lib/tcl7.4"
putenv "ITCL_LIBRARY=/home/vx/2.6.2/common/lib/itcl2.0"
putenv "DP_LIBRARY=/home/vx/2.6.2/common/lib/dp"
putenv "TOKEN_PORT=5677"

# Define CMLOG Server PORT (Halla 8228 - rich.jlab.org)
putenv "CMLOG_PORT=8228"

# Get around Clock returning 0 bug
mytv = malloc(8)
clock_gettime(0,mytv)


buf = malloc (21*4*512)


ld < /home/vx/2.5/extensions/universeDma/universeDma.o.vw54
#ld < /home/vx/2.5/extensions/tempeDma/usrTempeDma.o

sysVmeDmaShow();

#initialize (1 no interrupts) or (0 interrupts enabled)
sysVmeDmaInit(0) 
# Set for 64bit PCI transfers
sysVmeDmaSet(4,1)
# A24 VME Slave (1) or A32 Slave (2)
sysVmeDmaSet(11,2)
# BLK32 (4) or MBLK(64) (5) VME transfers
sysVmeDmaSet(12,4)

#usrVmeDmaConfig(1,2)

# Download Message logging libraries/client
#ld < /home/vx/CMLOG/2.1/bin/ppc-vw55/cmlogClientD
#ld < /home/vx/CMLOG/2.1/lib/ppc-vw55/libcmlog.a
#ld < /home/vx/CMLOG/2.1/bin/ppc-vw55/cmlogVxLogMsg

ld < /home/vx/2.6.2/cMsg/vxworks-ppc/lib/libcmsgRegex.o
ld < /home/vx/2.6.2/cMsg/vxworks-ppc/lib/libcmsg.o
ld < /home/vx/2.6.2/cMsg/vxworks-ppc/lib/libcmsgxx.o


# download ROC

#ld < /home/vx/2.5/VXWORKSPPC55/bin/coda_roc
ld < /home/vx/2.6.2/VXWORKSPPC55/bin/coda_roc_rc3.5

# load the CMRAMS Library (D. Abbott)
# On rich.jlab.org it is kept in /home/adaq/C-RAM/dev
cd "/home/vx"

# Load sfi fb
ld < /site/coda/2.6.2/VXWORKSPPC55/lib/libsfifb.o
ADDR=0
sysBusToLocalAdrs(0x39,0xe00000,&ADDR)
fb_init_1(ADDR)

vmeSetMaximumVMESlots=30

#ld < /home/vx/jessica/tinew/tiLib.o

#cd "jessica"
ld < /home/vx/test_1877.o
ld< /home/vx/bbite/v1495_fanout.o


#ld< /home/vx/bbite/c775/c775Lib.o
#ld< /site/coda/2.5/extensions/v775/c775Lib.o
#c775Init(0x09910000,0,1,0)
#c775Init(0x09920000,0,1,0)
#c775Clear(0)
#c775Clear(1)


#ld</home/vx/bbite/c792/c792Lib.o
#ld< /site/coda/2.5/extensions/v792/c792Lib.o
#c792Init(0x0AA10000,0,1,0)
#c792Init(0x09930000,0,1,0)
#c792Clear(0)
#c792Sparse(0,0,0)

#ld < 5.5/551/c-ramsLib.o
LSWAP=1 
ld < /home/vx/jessica/peppo/ti/tiLib.o

ld < /home/vx/CMLOG/2.1/bin/ppc-vw55/cmlogClientD
# start up tasks 
sp cmlogClientD
taskDelay(60*10)

#roc_tid = taskSpawn("ROC",200,0,120000,coda_roc,"-i","-s","richtest","-objects","ROC1 ROC")

taskSpawn "ROC",200,spTaskOptions,200000,coda_roc,"-session","compton","-objects","ROC6 ROC"

#roc_tid = taskSpawn("ROC",200,0,120000,coda_roc,"-s","rich","-objects","ROC5 ROC")
#taskSuspend roc_tid









