include "../tiLib.h"
unsigned int *tiData=NULL;
  tiSetFiberLatencyOffset_preInit(0x40); 
  tiSetCrateID_preInit(0x2); /* ROC 2 */
  tiInit(TRIG_ADDR,TRIG_MODE,0);
  tiDisableBusError();
      tiDisableDataReadout();
      tiDisableA32();
  tiSetEventFormat(1);
  tiSetBlockBufferLevel(BUFFERLEVEL);
  tiSetBusySource(0,1);
  tiStatus(0);
  tiDisableVXSSignals();
  link async trig source GEN 1 to titrig and titrig_done
  tiEnableVXSSignals();
  tiStatus(0);
  printf("Interrupt Count: %8d \n",tiGetIntCount());
  /* Free up the allocated memory for the tiData */
  if(tiData!=NULL)
      free(tiData);
      tiData=NULL;
  blockLevel = tiGetCurrentBlockLevel();
  tiData = (unsigned int*)malloc((8+5*blockLevel)*sizeof(unsigned int));
  tiStatus(0);
begin trigger titrig
  variable dCnt,ev_type,ii,tibready,timeout
      dCnt = tiReadBlock(tiData,8+5*blockLevel,0);
	  ev_type = tiDecodeTriggerType(tiData, dCnt, 1);
	    *rol->dabufp++ = tiData[ii];
begin done titrig
