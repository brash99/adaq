#!/bin/bash
# Take text file adaq.data created by swexpt.com
# and create the msql database
# Script to run from adaq on adaql2, R. Michaels, May 2001.

if (ps -ef | grep -i rcgui | grep -v grep >> /dev/null) ; then
  echo "runcontrol is running, cannot overwrite database"
  exit
else
  echo "Overwriting msql database on $HOST at `date`"
fi

msqladmin -q drop sbsfb
msqladmin -q create sbsfb
msql sbsfb < sbsfb.data

echo " "
echo "All done !"
