# Boot file for CODA 2.2 VME Crate
# Motorola 2300 version

hostAdd "sbs2.jlab.org","129.57.36.67"
hostAdd "sbs2","129.57.36.67"

# Add Routes for Multicast Support
mRouteAdd("224.0.0.0","129.57.36.0",0xf0000000,0,0)

# Setup environment / load coda_roc
putenv "MSQL_TCP_HOST=sbs2"
putenv "EXPID=cdetfb"
putenv "TCL_LIBRARY=/site/coda/2.6.2/common/lib/tcl7.4"
putenv "ITCL_LIBRARY=/site/coda/2.6.2/common/lib/itcl2.0"
putenv "DP_LIBRARY=/site/coda/2.6.2/common/lib/dp"
putenv "TOKEN_PORT=5677"

# Define CMLOG Server PORT (Halla 8228 - rich.jlab.org)
putenv "CMLOG_PORT=8228"

# Get around Clock returning 0 bug
mytv = malloc(8)
clock_gettime(0,mytv)

# Load sfi fb
ld < /site/coda/2.6.2/VXWORKSPPC55/lib/libsfifb.o
ADDR=0
sysBusToLocalAdrs(0x39,0xe00000,&ADDR)
fb_init_1(ADDR)

ld < /home/adaq/vxworks/test_1877.o
