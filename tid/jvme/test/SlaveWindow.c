/*
 * File:
 *    SlaveWindow.c
 *
 * Description:
 *    Test JLab Vme Driver
 *
 *
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../jvme.h"
#include "common_struct.h"

volatile struct vmecpumem *vmecpu;
extern void *a32slave_window;

int 
main(int argc, char *argv[]) 
{
  unsigned int laddr;
  unsigned int taddr = 0x0ed0;
  int stat;
  int inputchar=10;

  vmeOpenDefaultWindows();

/*   vmeSetDebugFlags(0xffffffff); */

  stat = vmeOpenSlaveA32(0x09000000,0x10000);
  if(stat == GEF_STATUS_SUCCESS)
    {
      vmecpu = (struct vmecpumem *)a32slave_window;
      vmecpu->id = 0xd00fface;
    }
  else
    goto CLOSE;

  while(1 && (inputchar==10))
    {
	
      printf("Grab mutex lock\n");
      vmeBusLock();
      printf("Press return to unlock mutex..\n");
      getchar();
      vmeBusUnlock();
	
      printf("I think it's unlocked now... return to kill it\n");
      inputchar = getchar();
    }

  stat = vmeCloseA32Slave();

 CLOSE:

  vmeCloseDefaultWindows();

  exit(0);
}

