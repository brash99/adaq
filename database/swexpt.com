#!/bin/bash
# Extract msql database into text file and substitute:
# experiment1 -> experiment2
# where experiment1 = arg1
# and   experiment2 = arg2
# Script to run from adaq account from adaql2, R. Michaels, May 2001
# 
# 
DB1=sbsfb
DB2=
DB3=
temp1=/home/adaq/database/temp1.data

if [ $# -ne 2 ] ; then
   echo "USAGE: swexpt expt1 expt2"
   exit
fi
expt1=$1
expt2=$2


for database in $DB1 $DB2 $DB3 ; do
  msqldump $database > $temp1
  sed -e s/$expt1/$expt2/g $temp1 > $database.data
  rm $temp1
done



