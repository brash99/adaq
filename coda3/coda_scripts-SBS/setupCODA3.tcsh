# CSH Environment setup for CODA 3

# Prune any previous CODA defs in PATH and LD_LIBRARY_PATH

set PATH=`echo $PATH | awk -v RS=: -v ORS=: '/coda/ {next} {print}' | sed 's/:*$//'`
if ($?LD_LIBRARY_PATH == "1") then
   set LD_LIBRARY_PATH=`echo $LD_LIBRARY_PATH | awk -v RS=: -v ORS=: '/coda/ {next} {print}' | sed 's/:*$//'`
endif

########################################
# User Specific Configuration
########################################
setenv CODA /site/coda/3.10
source $CODA/.setup

setenv SESSION dflay_test 
setenv EXPID cdet_vetroc

setenv COOL_HOME /home/adaq/coda/cool 
# setenv JAVA_HOME /group/da/Java/jdk/jdk1.8.0_152
setenv JAVA_HOME /u/group/da/Java/jdk/Linux-i586/jdk1.8.0_172

setenv REMEX_CMSG_HOST sbs2 
setenv REMEX_CMSG_PASSWORD ${EXPID}

setenv CODA_COMPONENT_TABLE ./config/${EXPID}/coda_component_table.cfg

# Add this script directory to environment PATH
set called=($_)

if ( "$called" != "" ) then  ### called by source 
   set script_fn=`readlink -f $called[2]`
else                         ### called by direct execution of the script
   set script_fn=`readlink -f $0`
endif

set script_dir=`dirname $script_fn`
setenv CODA_CONFIG $script_dir

# Add config scripts to path
set path = ($CODA_CONFIG $path)


