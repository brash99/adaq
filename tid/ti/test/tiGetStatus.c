/*
 * File:
 *    tiLibTest.c
 *
 * Description:
 *    Test Vme TI interrupts with GEFANUC Linux Driver
 *    and TI library
 *
 *
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "jvme.h"
#include "tiLib.h"

int 
main(int argc, char *argv[]) 
{

  int stat;
  int slot;

  if(argc>1)
    {
      slot = atoi(argv[1]);
      if(slot<1 || slot>22)
	{
	  printf("invalid slot... using 21");
	  slot=21;
	}
    }
  else 
    slot=21;

  printf("\nJLAB TI Status... slot = %d\n",slot);
  printf("----------------------------\n");

  vmeOpenDefaultWindows();

  /* Set the TI structure pointer */
  tiInit(slot,TI_READOUT_EXT_POLL,TI_INIT_NO_INIT);
  tiCheckAddresses();
  printf("Firmware version = 0x%x\n",tiGetFirmwareVersion());
  tiStatus(1);

  int i;
  for(i=1; i<=18; i++)
    printf("PP %2d = VME %2d\n",i,vxsPayloadPort2vmeSlot(i));

  for(i=1; i<=21; i++)
    printf("VME %2d = PP %2d\n",i,vmeSlot2vxsPayloadPort(i));

 CLOSE:

  vmeCloseDefaultWindows();

  exit(0);
}

