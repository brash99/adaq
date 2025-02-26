#define ROL_NAME__ "GEN_USER"
#define MAX_EVENT_LENGTH 4096
#define MAX_EVENT_POOL   512
#define SFI
#define INIT_NAME ti_test__init
#include <rol.h>
#include <SFI_source.h>
#define WC_MASK 0xffffff
#define SFI_ADDR 0xe00000
#define IRQ_SOURCE_MASK 0x02
#define NTDC 1
#define NTDC1 0
#define LTDCSLOT 0
#define UTDCSLOT 0
#define NMOD 1
#include "sfi.h"
unsigned long scan_mask;
#include "GEN_source.h"
#include "../tiLib.h"
#define TRIG_ADDR 0x080000
#define TRIG_MODE 0
#define SFI_ADDR 0xe00000
#define BLOCKLEVEL 1
#define BUFFERLEVEL 1
#define USE_PULSER 0
#define READOUT_TI 1
int tdcslots[NTDC];
int modslots[NMOD];
int nhitmod[NMOD];
int csr0[NMOD];
  float* onTime=0;
  float* deadTime=0;
  int  ADDR=0;
  float  liveTime=0;
  float ratio=0;
  int* t1=0;
 int* t2=0;
 float* tsTrigger=0;
//static int pedsup;
static int buffered;
static int phelicity;
extern int bigendian_out;
int blockLevel=1;
extern int tiA32Base;
static void __download()
{
    daLogMsg("INFO","Readout list compiled %s", DAYTIME);
#ifdef POLLING___
   rol->poll = 1;
#endif
/* Get offset for local DRAM as seen from VME bus */
  if (sysLocalToBusAdrs(0x09,0,&sfi_cpu_mem_offset)) { 
     printf("**ERROR** in sysLocalToBusAdrs() call \n"); 
     printf("sfi_cpu_mem_offset=0 FB Block Reads may fail \n"); 
  } else { 
     printf("sfi_cpu_mem_offset = 0x%x \n",sfi_cpu_mem_offset); 
  } 
    *(rol->async_roc) = 0; /* Normal ROC */
  {  /* begin user */
unsigned long adc_id;
unsigned long res, laddr, jj, kk, ll;
bigendian_out = 0;
{/* inline c-code */
 
  res = (unsigned long) sysBusToLocalAdrs(0x39,SFI_ADDR,&laddr);
  if (res != 0) {
     printf("Error Initializing SFI res=%d \n",res);
  } else {
     printf("Calling InitSFI() routine with laddr=0x%x.\n",laddr);
     InitSFI(laddr);
  }
 
 }/*end inline c-code */
{/* inline c-code */
 
{
  /* measured longest fiber length */
  tiSetFiberLatencyOffset_preInit(0x40); 

  /* Set crate ID */
  tiSetCrateID_preInit(0x1); /* ROC 1 */

  /* TI Setup */
  tiInit(TRIG_ADDR,TRIG_MODE,0);
  tiDisableBusError();

  if(READOUT_TI==0) /* Disable data readout */
    {
      tiDisableDataReadout();

      /* Disable A32... where that data would have been stored on the TI */
      tiDisableA32();
    }
  
  /* Override library default busy source */
  tiSetBusySource(TI_BUSY_LOOPBACK,1);

  if(USE_PULSER==1)
    tiSetTriggerSource(TI_TRIGGER_PULSER);
  else
    tiSetTriggerSource(TI_TRIGGER_TSINPUTS);


  /* Set needed TS input bits */
  tiEnableTSInput( TI_TSINPUT_1 | TI_TSINPUT_2 );

  /* Load the trigger table
   *  - 0:
   *    - TS#1,2,3,4,5 generates Trigger1 (physics trigger),
   *    - TS#6 generates Trigger2 (playback trigger),
   *    - No SyncEvent;
   */
  tiLoadTriggerTable(0);

  tiSetTriggerHoldoff(1,10,0);
  tiSetTriggerHoldoff(2,10,0);

  tiSetBlockBufferLevel(BUFFERLEVEL);


  tiStatus(0);
}
 
 }/*end inline c-code */
{/* inline c-code */
 
/* setup the arrays containing the slotnumbers */ 
  ll=0;
  if (NTDC>0) {
    kk=0;
    for(jj=LTDCSLOT;jj<=UTDCSLOT;jj++) {
       tdcslots[kk]=jj;
       modslots[ll]=jj;
       csr0[ll]=0x400;
       kk++;
       ll++;
    }
  }
 
 }/*end inline c-code */
{/* inline c-code */
 
  init_strings();
 /* buffered=getflag(BUFFERED);*/
  buffered=0;	
  phelicity=0;
//pedsuppression=0;

/* calculate the scan_mask */
  scan_mask = 0;


  for (jj=0; jj<NMOD; jj++)
     scan_mask |= (1<<modslots[jj]);
  printf ("Crate Scan mask = %x\n",scan_mask);
 
 }/*end inline c-code */
{/* inline c-code */
 
{
  /* Perform these at the end of DOWNLOAD */
  tiDisableVXSSignals();
  tiClockReset();
  taskDelay(2);
  tiTrigLinkReset();
}


  sysBusToLocalAdrs(0x39,0x0800A8,&ADDR);
  onTime =ADDR;
  t1=ADDR;
  sysBusToLocalAdrs(0x39,0x0800AC,&ADDR);
  deadTime =ADDR;
  t2=ADDR;

 
 }/*end inline c-code */
    daLogMsg("INFO","User Download Executed");

  }  /* end user */
    return;
   fooy: 
    SFI_REPORT_ERROR;
    RECOVER;
} /*end download */     

static void __prestart()
{
CTRIGINIT;
    *(rol->nevents) = 0;
  {  /* begin user */
unsigned long jj, adc_id, kk, slot_num, csr_0, pedsuppress;
    daLogMsg("INFO","Entering User Prestart");

    GEN_INIT;
    CTRIGRSA(GEN,1,titrig,titrig_done);
    CRTTYPE(1,GEN,1);
{/* inline c-code */
 
{ 
 /* Turn VXS signals on, beginning of prestart */
  tiEnableVXSSignals();

  /* Set number of events per block (broadcasted to all connected TI Slaves)*/
  tiSetBlockLevel(BLOCKLEVEL);
  printf("rocPrestart: Block Level set to %d\n",BLOCKLEVEL);
}
 
 }/*end inline c-code */
  sfiTrigSource = IRQ_SOURCE_MASK;
    fb_init_1(0);
kk = 0 ;
while( ( kk <  NTDC) ){ 
    slot_num = tdcslots[kk];
    padr   = slot_num; 
    /* here */
    fb_fwc_1(padr,0,0x40000000,1,1,0,1,0,0,0);
 
    sfi_error_decode();
    padr   = slot_num; 
    sadr = 1 ;
    /* here */
    fb_fwc_1(padr,sadr,0x40000001,1,1,0,0,0,0,0);
 
    sfi_error_decode();
    padr   = slot_num; 
    sadr = 18 ;
    /* here */
    fb_fwc_1(padr,sadr,0xbb6,1,1,0,0,0,0,0);
 
    sfi_error_decode();
    padr   = slot_num; 
    sadr = 7 ;
    /* here */
    fb_fwc_1(padr,sadr,2,1,1,0,0,0,0,0);
 
    sfi_error_decode();
    kk++ ;
    fprel(); 
    sfi_error_decode();
}/* end while loop */

kk = 0 ;
while( ( kk <  NTDC) ){ 
    slot_num = tdcslots[kk];
    padr   = slot_num; 
    /* here */
    fb_frc_1(padr,0,&adc_id,1,1,0,1,0,0,0);
    daLogMsg("INFO","TDC %d ID = %x ",slot_num,adc_id);
    kk++ ;
    fprel(); 
    sfi_error_decode();
}/* end while loop */

{/* inline c-code */
 
{
  /* Send a SyncReset, at the end of prestart */
  tiSyncReset(1);
  taskDelay(2);
}
 
 }/*end inline c-code */
    daLogMsg("INFO","User Prestart Executed");

  }  /* end user */
    if (__the_event__) WRITE_EVENT_;
    *(rol->nevents) = 0;
    rol->recNb = 0;
    return;
   fooy: 
    SFI_REPORT_ERROR;
    RECOVER;
} /*end prestart */     

static void __end()
{
  {  /* begin user */
{/* inline c-code */
 


{
  CDODISABLE(GEN,1,0);

  if(USE_PULSER==1)
    tiSoftTrig(1,0,0x1123,1);

  tiStatus(0);
  printf("Interrupt Count: %8d \n",tiGetIntCount());
}
 
 }/*end inline c-code */
    daLogMsg("INFO","User End Executed");

  }  /* end user */
    if (__the_event__) WRITE_EVENT_;
    return;
   fooy: 
    SFI_REPORT_ERROR;
    RECOVER;
} /* end end block */

static void __pause()
{
  {  /* begin user */
  CDODISABLE(GEN,1,0);
    daLogMsg("INFO","User Pause Executed");

  }  /* end user */
    if (__the_event__) WRITE_EVENT_;
    return;
   fooy: 
    SFI_REPORT_ERROR;
    RECOVER;
} /*end pause */
static void __go()
{

  {  /* begin user */
    daLogMsg("INFO","Entering User Go");

{/* inline c-code */
 
{
  /* Get the current Block Level */
  blockLevel = tiGetCurrentBlockLevel();
  printf("rocGo: Block Level set to %d\n",blockLevel);
}
 
 }/*end inline c-code */
{/* inline c-code */
 
{


  CDOENABLE(GEN,1,0);

  if(USE_PULSER==1)
    tiSoftTrig(1,3000,0x1123,1);

  tiStatus(0);
}
 
 }/*end inline c-code */
    daLogMsg("INFO","User Go Executed");

  }  /* end user */
    if (__the_event__) WRITE_EVENT_;
    return;
   fooy: 
    SFI_REPORT_ERROR;
    RECOVER;
}

void titrig(unsigned long EVTYPE,unsigned long EVSOURCE)
{
    long EVENT_LENGTH;
  {  /* begin user */
unsigned long dCnt, res;
unsigned long ii, jj, datascan, event_ty, event_no, slot_num, fbres;
unsigned long sync_or_unbuff;
unsigned long itimeout, icnt;
unsigned long temp_var;
  tiLatchTimers();
  *sfi.sequencerEnable = 1;
 sync_or_unbuff = syncFlag || !buffered;
  rol->dabufp = (long *) 0;
    CEOPEN(1, BT_BANK);
    CBOPEN(1, BT_UI4, 0);
    CBWRITE32(0x0da00ddc4); 
*rol->dabufp++ = *t1;
*rol->dabufp++ = *t2;
ii = 0 ;
datascan = 0 ;
while( (( ii < 20) )&&(( scan_mask != (datascan&scan_mask)) )){ 
    padr   = 9;
    /* here */
    fb_frcm_1(padr,0,&datascan,1,0,1,0,0,0);
    ii++ ;
}/* end while loop */

if(( ii <  20) ) {
   padr = 0x15;
   fb_fwcm_1(0x15,0,0x400,1,0,1,0,0,0);
    CBWRITE32(0x0da00ddc0); 
    padr   = UTDCSLOT ;
    /* here */
    fpbr(padr,200); 
}
else{
    CBWRITE32(0xdc0000ff); 
{/* inline c-code */
 

//   for (jj=LTDCSLOT;jj<=UTDCSLOT;jj++) {
//     if(datascan & (1>>tdcslots[jj])) {
   for (jj=0;jj<=NTDC;jj++) {
     if(datascan & (1<<tdcslots[jj])) {
       padr=tdcslots[jj];
       fb_fwc_1(padr,0,0x400,1,1,0,1,0,0,0);
     }
   }
 
 }/*end inline c-code */

}/*endif whole if block*/

    CBWRITE32(0xfabc0003); 
   icnt = icnt + 1;
   if(icnt > 20000) icnt = 0;
   *rol->dabufp++ = icnt;
    CBWRITE32(ii); 
    CBWRITE32(datascan); 
    CBWRITE32(0xfaaa0001); 
{/* inline c-code */
 
/* Words to flag if buffered/unbuffered */
   if (buffered) { 
        *rol->dabufp++ = 0xfafbbf00 + syncFlag;
     }
      else 
     {
        *rol->dabufp++ = 0xfabb0000;
     }               
/* Word to flag the helicity mode (superperiod or not) */
   if (phelicity) { 
        *rol->dabufp++ = 0xdfab0000;
     }
 
 }/*end inline c-code */
{/* inline c-code */
 
    for(ii=0;ii<NMOD;ii++) {
      nhitmod[ii]=0;
    }
 
 }/*end inline c-code */
if(( sync_or_unbuff != 0) ) {
  itimeout=0;
{/* inline c-code */
 
    do {
      if(itimeout++>10000) break;
      datascan = 0;
      padr = 9;
      fb_frcm_1(9,0,&datascan,1,0,1,0,0,0);
      if(datascan & scan_mask) {
        for(ii=0;ii<NMOD;ii++){
          if(datascan & (1<<modslots[ii])) {
            nhitmod[ii]++;
            padr = modslots[ii];
            fb_fwc_1(padr,0,csr0[ii],1,1,0,1,0,0,0);
          }
        }
      }
    } while(datascan & scan_mask);
 
 }/*end inline c-code */
ii = 0 ;
while( ( ii <  NMOD) ){ 
      temp_var = nhitmod[ii];
if(( temp_var != 0) ) {
        datascan = 0xdcfe0000 + (modslots[ii] << 11) + (nhitmod[ii] & 0x7ff);
    CBWRITE32(datascan); 

}/*endif whole if block*/

    ii++ ;
}/* end while loop */


}/*endif whole if block*/

    CBCLOSE;
{/* inline c-code */
 
{
 
  if(READOUT_TI==1)
    {
      /* Open a CODA Bank of 4byte unsigned integers, type=4 */
      CBOPEN(4,BT_UI4,0);
  *rol->dabufp++ = 0x12345888;
      dCnt = tiReadBlock(rol->dabufp,50,0);
      if(dCnt<=0)
	{
	  logMsg("No data or error.  dCnt = %d\n",dCnt);
	}
      else
	{
	  rol->dabufp += dCnt;
	}
      CBCLOSE;
    }
  
  CBOPEN(3,BT_UI4,0);
  *rol->dabufp++ = 0x12345678;
  CBCLOSE;
}

 
 }/*end inline c-code */
    CECLOSE;
  }  /* end user */
    return;
   fooy: 
    SFI_REPORT_ERROR;
    RECOVER;
} /*end trigger */

void titrig_done()
{
  {  /* begin user */
{/* inline c-code */
 
  sysBusIntAck(5);       /* Acknowledge Universe chip */
 
 }/*end inline c-code */
  }  /* end user */
    return;
   fooy: 
    SFI_REPORT_ERROR;
    RECOVER;
} /*end done */

void __done()
{
poolEmpty = 0; /* global Done, Buffers have been freed */
  {  /* begin user */
  CDOACK(GEN,1,0);
  }  /* end user */
    return;
   fooy: 
    SFI_REPORT_ERROR;
    RECOVER;
} /*end done */

static void __status()
{
  {  /* begin user */
  }  /* end user */
} /* end status */

/* inline c-code */
 

void livetime()
{
              
 
  //get the real/livetime of TI
  sysBusToLocalAdrs(0x39,0x0800A8,&ADDR);
  onTime =ADDR;
  t1=ADDR;
 
  //get the deadtime of the TI
  sysBusToLocalAdrs(0x39,0x0800AC,&ADDR);
  deadTime=ADDR;
  t2=ADDR;

 //get the ts input trgger counter TI
  sysBusToLocalAdrs(0x39,0x080018,&ADDR);
  tsTrigger =ADDR;
  


  //find total livetime
 // liveTime=*t1+*t2;

// ratio = *deadTime / (*onTime + *deadTime);

 // printf ("livetime:  %f\n",liveTime);
// printf ("deadtime:  %d\n",*t2); 
//printf ("realtime:  %d\n",*t1);
//printf ("deadtime/livetime:  %f\n",ratio);
// printf ("TS trigger:  %d\n",*tsTrigger);

 }
 
 /*end inline c-code */
