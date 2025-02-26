#!/bin/bash
#   5 Dec 2016 R.P.
#  Loads the same value for all channels for settable properties.
#  NOTE!!! There is no any checks for property name and value
#

function usage {
echo 
echo " Usage:"
echo "         $0 <slot> <property> <value>"
echo " <slot> - module slot number (0-9)"
echo " <property> - property (to set):"
echo "     DV  - demmand voltage (0 - 2500) V"
echo "     CE  - enable channel -(1); disable channel -(0)"
echo "     SVL - software voltage limit (0 - 2500) V"
echo "     DC  - current limit (0 - 0.05) ???"
echo "     RUP - rump up speed (10 - 500) V/s  "
echo "     RDN - rump down speed (10 - 500) V/s "
echo   
}

# java GUI server, port
jsrv=hvsrv3
jp=5555

# VME HV server, port
srv=sbscdet
p=2012

val=0

# number of channels, channel #25 is primary channel.
chn=25

# hv client program
cmd=~/hvg.Apr2015/hvcli

# command for Java GUI server
jcli="$cmd -h $jsrv -p $jp -m "
# direct command to VME HV server
cli="$cmd -h $srv -p $p -m "

#check command line arguments
if [[ $# -ne 3 ]]; then
    echo " Error: Illegal number of parameters"
    usage
    exit 1
fi

slot=$1
prop=$2
val=$3
pval=$val

if [ $prop == "DV" ] ; then 
    pval=$(($val + 55))
fi

mes="LD $slot $prop $val $val $val $val $val $val $val $val $val $val $val $val $val $val $val $val $val $val $val $val $val $val $val $val $pval"

# send command using Java GUI server
sss="$jcli '$srv:$p $mes'"
echo $sss 

# send command using VME HV server
sss="$cli '$mes'"

echo $sss 
 

