/***************************************************************************/
/*                                                                         */
/*   ------  C . A . E . N . SpA  ------                                   */
/*                                                                         */
/* VMESY527.C - Demonstration on the use of Caenet Routines in             */
/* communication between V288 module and SY527 Universal                   */
/* Multichannel Power Supply System Version 1.05                           */
/*                                                                         */
/* 12 Mar 2015 R.P. modified to simulate LeCroy-1458 mainframe             */
/*                 for work with Java HV GUI                               */
/***************************************************************************/
/* 12 Apr 2015 (i) add semphores (mutex) to crate_map() and read_ident() functions
 *             (ii) clear alaram every time when channel is OFF 
 *
 */
#include <stdio.h> 
#include <ctype.h> 
#include <string.h>
#include <ioLib.h>
#include <tickLib.h>
#include <sysLib.h>
#include <assert.h>    /* assert() */
#include <semLib.h>
#include <taskLib.h>
#include <vxWorks.h>
 
#include "vmcaenet.h"

#ifndef DBG_LEVEL
#define DBG_LEVEL 0
#endif

#define EUROCOM  0xfa000000

#define MAX_CRATES     15           /* max connected crates on one caenet V288 board */

#define ONE_TICK       60           /* VxWorks ticks frequency  */
#define GS_TIME_SLICE  180*ONE_TICK /* sleep time (in s) for GS number update */
#define PS_TIME_SLICE  5*ONE_TICK   /* sleep time (in s) for PS number update */

/* commands codes */
#define   IDENT             0
#define   READ_STATUS       1
#define   READ_SETTINGS     2
#define   READ_BOARD_INFO   3
#define   READ_CR_CONF      4
#define   READ_GEN_STATUS                          5
#define   READ_HVMAX                               6
#define   READ_HWSTAT                              7 /* read hardware ststus */

#define   SET_STATUS_ALARM                      0x1a
#define   FORMAT_EEPROM_1                       0x30
#define   FORMAT_EEPROM_2                       0x31
#define   CLEAR_ALARM                           0x32
#define   LOCK_KEYBOARD                         0x33
#define   UNLOCK_KEYBOARD                       0x34
#define   KILL_CHANNELS_1                       0x35
#define   KILL_CHANNELS_2                       0x36


#define PROP_NUM    12 /* number of properties in 'PROP[]' */
/**  Caen properties **   Comment    **	  Units  Lecroy */
#define IMON   1   /* current readout     uA     MC    */
#define VMON   2   /* voltage readout     V      MV    */
#define V0SET  3   /* voltage set 0   	  V      DV    */ 
#define I0SET  4   /* current set 0   	  uA     ----  */ 
#define ST     5   /* mask enable/disable On/OFF ???   */
#define SVMAX  6   /* soft voltage limit  V      ----  */
#define RUP    7   /* rump up speed       V/s    RUP   */
#define RDWN   8   /* rump down speed     V/s    RDN   */
#define TRIP   9   /* trip time           0.1s   TC  (trip current)  */
#define STAT   10  /* channel status bits --     STAT   */  
#define FLAG   11  /* mask & flag bits    --     CE,..  */

#define HVMAX  12   /* hardware voltage limit V   HVL   */
#define V1SET  13   /* voltage set 1   	  V      ----  */ 
#define I1SET  14   /* current set 1       uA     ----  */

/*** extracted from channel FLAG and STAT (STATUS) words ****/
#define PW     0   /* set channel power On/OFF   CE    */
#define PON    15  /* On or Off of channel when power up       */
#define PRON   16  /* set prioritty for channel when power On  */
#define PROFF  17  /* set prioritty for channel when power Off */
#define ONOFF  18  /* mask enable/disable On/OFF ???           */


/**** ALARMS bits(if=1) from STATUS word *****/
#define A_OVV    0x0400 /* over-voltage  */
#define A_UVV    0x0800 /* under-voltage */
#define A_OVC    0x1000 /* over-current  */ 
#define A_KILL   0x0040 /* Kill ? */
#define A_VMAX   0x0100 /* excieds Vmax  */
#define A_ITRIP  0x0020 /* internal Trip */
#define A_ETRIP  0x0200 /* external Trip */

/*** STATUS of channel from STATUS word (if bit=1) *****/
#define CH_DOWN  0x2000 /* channel voltage goes down  */
#define CH_UP    0x4000 /* channel voltage goes UP  */
#define CH_ON    0x8000 /* channel ON  */


/*
  The following macro transforms the V288 input address in a "good"
  VME address for Standard Accesses by Eltec CPU board
*/
/*#define AUPDATE(addr) ((unsigned)EUROCOM + addr)*/
#define AUPDATE(addr) (addr)

/*
  Globals
*/
const char PROP[] = "CE MC MV DV DC ST SVL RUP RDN TC STAT FLAG"; /* properties list */

int dbg=DBG_LEVEL;
int mon_delay1=60; /* taskSDelay(mon_delay1) between board scans */

int kk=0; /* for semaphore tests */

unsigned int v288addr=0xe0600000;
unsigned short craten=12;
ushort code=0; 


struct ccrate crates[MAX_CRATES];  /* crates parameters  */
/* struct board  boards[10];*/  /* modules parameters */
 struct hvch   ch_set[25];  /* Settings of 25 chs.*/
 struct hvrd   ch_read[25]; /* Status of 25 chs.  */


ushort con_crates=0; /* bit mask  of connected crates, filled during initialization init_sy527() */
int crates_num[MAX_CRATES]; /* array  of hv crates numbers, filled during start up of monitor() */
int num_crate=0; /* number of hv crates (in array crates_num ) */

/* float pow_10[]={ 1.0, 10.0, 100.0, 1000.0}; *//* this is scale I and V converison powers of 10 */
float pow_10[]={ 1.0, 10.0, 1000.0, 1000.0}; /* FIXME!!! this is temporary for board with few channels type */

SEM_ID semMutex=NULL;  /* binary semaphore to synchronize access to V288 board*/

/* "dead zone" values used in comp_dzset() and comp_dzread()*/
int dzset[9]={1, 1, 1, 1, 1, 1, 1, 1, 1};
int dzread[3]={3, 2, 1}; /* dmz: Vmon, Imon, Status */
/*int dzread[3]={5, 1, 1}; */ /* dmz: Vmon, Imon, Status */


/****** Utility functions ********/
int set_dbg(int level) {
  if (level>=0) dbg=level;
  return dbg;
}

int set_mdelay(int del) {
   mon_delay1=del;
  return mon_delay1;
}

/* return size of structure 'hvrd'  
*  just sizeof(struct hvrd) returns wrong number 
*/
int size_hvrd(void) {
  struct hvrd c;
  return (sizeof(c.vread)+
	  sizeof(c.hvmax)+
	  sizeof(c.iread)+
	  sizeof(c.status)
	  );
}

/* returns size of structure 'hvch */
int size_hvch(void)
{
  struct  hvch ch;  
  return   sizeof(ch.chname) + sizeof(ch.dummy) + sizeof(ch.flag) + sizeof(ch.i0set)
           + sizeof(ch.i1set)  + sizeof(ch.rdwn)  + sizeof(ch.rup)  + sizeof(ch.trip) 
           + sizeof(ch.v0set)  + sizeof(ch.v1set)  + sizeof(ch.vmax);
}

/******** From vmcaenet.c ****************************************************/
/***------------------------------------------------------------------------
Read_data
--------------------------------------------------------------------***/
int vmeWrite16(volatile unsigned short *addr, unsigned short val)
{
  /*  val = SSWAP(val);*/
  *addr = val;
  return 1;
}

unsigned short vmeRead16(volatile unsigned short *addr)
{
  unsigned short rval;
  rval = *addr;
  /*  rval = SSWAP(rval);*/
  return rval;
}

int read_data(datovme)
     ushort *datovme;
{
  unsigned short * addr= (unsigned short *)v288addr;
  unsigned short * stat= (unsigned short *) STATUS;
  ushort q=0;
  *datovme =  vmeRead16(addr);
  q=vmeRead16(stat);
  /*  vme_read(v288addr,datovme,WORD);
      vme_read(STATUS,&q,WORD);
  */
 return((q == Q) ? TUTTOK : TIMEOUT);
 
 
}

/***------------------------------------------------------------------------
    Wait_resp
    --------------------------------------------------------------------***/
int wait_resp(datovme)
     ushort *datovme;
{
  int i=0;
  ushort q=0;
  unsigned short * addr= (unsigned short *) v288addr;
  unsigned short * stat= (unsigned short *) STATUS;

  while(i!=TIMEOUT && q!=Q)
    {
      *datovme =  vmeRead16(addr);
      q=vmeRead16(stat);
      /*      vme_read(v288addr,datovme,WORD);
      vme_read(STATUS,&q,WORD);
      */
      i++;
    }
  return((i == TIMEOUT) ? TIMEOUT : TUTTOK);
}

/***------------------------------------------------------------------------
Send_comm
--------------------------------------------------------------------***/
int send_comm(vmeaddress,datovme)
     unsigned int vmeaddress;
     ushort datovme;
{
  int i=0;
  ushort q=0;
  unsigned short * addr= (unsigned short *)vmeaddress;
  unsigned short * stat= (unsigned short *) STATUS;

  /*  printf("send_comm(): addr=0x%X data=%d (0x%X)\n",vmeaddress,datovme,datovme);
   */  

  while(i!=TIMEOUT && q!=Q)
    {
      /*      if(!vme_write(vmeaddress,&datovme,WORD)) */
      if(!vmeWrite16(addr, datovme))
	return E_BUSERR;
      q=vmeRead16(stat);
      /*vme_read(STATUS,&q,WORD);*/
      i++; 
    }
  return((i == TIMEOUT) ? TIMEOUT : TUTTOK);
}

/***------------------------------------------------------------------------
Caenet_read: Called by user programs to load "byte_count" bytes from
CAENET into the buffer pointed by "*dest_buff".
The VME address of V288, the CAENET crate number and the
CAENET code are found in global variables.
Caenet_read returns TUTTOK = 0 if everything has worked;
It returns one from seven different errors (defined as
positive constants in Vmcaenet.h) if it has received one
error which strictly depends from V288 Module;
It returns a negative error (depending from the CAENET slave
module) if the CAENET communication has not worked.
Remember: Module V288 can return three "general" negative errors
related to the CAENET network that this routine does not
handle separately from the "slave specific" ones.
--------------------------------------------------------------------***/
int caenet_read(dest_buff,byte_count)
     uchar *dest_buff;
     int byte_count;
{
  int i,esito;
  ushort mstident=V288, datatemp;
  short dato;
  if((esito=send_comm(v288addr,mstident)) == TIMEOUT)
    return E_NO_Q_IDENT;
  else if(esito == E_BUSERR)
    return esito;
  /* Transmit Crate Number */
  if((esito=send_comm(v288addr,(ushort)craten)) == TIMEOUT)
    return E_NO_Q_CRATE;
  else if(esito == E_BUSERR)
    return esito;
  /* Transmit Code */
  if((esito=send_comm(v288addr,(ushort)code)) == TIMEOUT)
    return E_NO_Q_CODE;
  else if(esito == E_BUSERR)
    return esito;
  /* Start Transmission */
  if((esito=send_comm(TXMIT,mstident)) == TIMEOUT)
    return E_NO_Q_TX;
  else if(esito == E_BUSERR)
    return esito;
  if(wait_resp(&dato) == TIMEOUT)
    return E_NO_Q_RX;
  if(dato == TUTTOK) /* Test on the operation */
    for(i=0;i<byte_count;i+=2)
      {
	if(read_data(&datatemp) == TIMEOUT && i<byte_count-1)
	  return E_LESSDATA;
	dest_buff[i] = HIBYTE(datatemp);
	dest_buff[i+1] = LOBYTE(datatemp);
      }
  return dato;
}
/***--------------------------------------------------------------------
Caenet_read: Called by user programs to load "byte_count" bytes from
CAENET into the buffer pointed by "*dest_buff".
--------------------------------------------------------------------***/
int caenet_read2(crt, cod, dest_buff, byte_count)
     ushort crt;
     ushort cod;
     uchar *dest_buff;
     int byte_count;
{
  int i=0, esito=0;
  ushort mstident=1, datatemp=0;
  short dato=0;

  if((esito=send_comm(v288addr,mstident)) == TIMEOUT)
    return E_NO_Q_IDENT;
  else if(esito == E_BUSERR)
    return esito;
  /* Transmit Crate Number */
  if((esito=send_comm(v288addr,(ushort)crt)) == TIMEOUT)
    return E_NO_Q_CRATE;
  else if(esito == E_BUSERR)
    return esito;
  /* Transmit Code */
  if((esito=send_comm(v288addr,(ushort)cod)) == TIMEOUT)
    return E_NO_Q_CODE;
  else if(esito == E_BUSERR)
    return esito;
  /* Start Transmission */
  if((esito=send_comm(TXMIT,mstident)) == TIMEOUT)
    return E_NO_Q_TX;
  else if(esito == E_BUSERR)
    return esito;
  if(wait_resp(&dato) == TIMEOUT)
    return E_NO_Q_RX;
  if(dato == TUTTOK) /* Test on the operation */
  for(i=0;i<byte_count;i+=2)
    {
      esito=read_data(&datatemp);
      if(esito == TIMEOUT && i<byte_count-1)
	return E_LESSDATA;
      dest_buff[i] = HIBYTE(datatemp);
      dest_buff[i+1] = LOBYTE(datatemp);
    }
  /*  printf("caenet_read2(): esito=%x (%d) byte_cnt=%d  i=%d\n", esito, esito, byte_count, i);*/
 return dato;
}

/***------------------------------------------------------------------------
Caenet_write: Called by user programs to transfer "byte_count" bytes to
CAENET from the buffer pointed by "*source_buff".
--------------------------------------------------------------------***/
int caenet_write2(crt, cod, source_buff, byte_count)
     ushort crt;
     ushort cod;
     uchar  *source_buff;
     int    byte_count;
{
  int i=0, esito=0;
  ushort mstident=1, datatemp=0;
  short dato=0;
  
  if((esito=send_comm(v288addr,mstident)) == TIMEOUT)
    return E_NO_Q_IDENT;
  else if(esito == E_BUSERR)
    return esito;
  /* Transmit Crate Number */
  if((esito=send_comm(v288addr,(ushort)crt)) == TIMEOUT)
    return E_NO_Q_CRATE;
  else if(esito == E_BUSERR)
    return esito;
  /* Transmit Code */
  if((esito=send_comm(v288addr,(ushort)cod)) == TIMEOUT)
    return E_NO_Q_CODE;
  else if(esito == E_BUSERR)
    return esito;
  /* Transmit data */
  for(i=0;i<byte_count;i+=2)
    {
      datatemp=(ushort)source_buff[i]<<8 | source_buff[i+1];
      if((esito=send_comm(v288addr,datatemp)) == TIMEOUT)
	return E_NO_Q_DATA;
      else if(esito == E_BUSERR)
	return esito;
    }
  /* Start transmission */
  if((esito=send_comm(TXMIT,mstident)) == TIMEOUT)
    return E_NO_Q_TX;
  else if(esito == E_BUSERR)
    return esito;
  if(wait_resp(&dato) == TIMEOUT)
    return E_NO_Q_RX;
  return dato;
}
/***------------------------------------------------------------------------
Caenet_write: Called by user programs to transfer "byte_count" bytes to
CAENET from the buffer pointed by "*source_buff".
The VME address of V288, the CAENET crate number and the
CAENET code are found in global variables.
Caenet_write returns TUTTOK = 0 if everything has worked;
It returns one from seven different errors (defined as
positive constants in Vmcaenet.h) if it has received one
error which strictly depends from V288 Module;
It returns a negative error (depending from the CAENET slave
module) if the CAENET communication has not worked.
Remember: Module V288 can return three "general" negative errors
related to the CAENET network that this routine does not
handle separately from the "slave specific" ones.
--------------------------------------------------------------------***/
int caenet_write(source_buff,byte_count)
     uchar *source_buff;
     int byte_count;
{
  int i,esito;
  ushort mstident=V288,datatemp;
  short dato;

  if((esito=send_comm(v288addr,mstident)) == TIMEOUT)
    return E_NO_Q_IDENT;
  else if(esito == E_BUSERR)
    return esito;
  /* Transmit Crate Number */
  if((esito=send_comm(v288addr,(ushort)craten)) == TIMEOUT)
    return E_NO_Q_CRATE;
  else if(esito == E_BUSERR)
    return esito;
  /* Transmit Code */
  if((esito=send_comm(v288addr,(ushort)code)) == TIMEOUT)
    return E_NO_Q_CODE;
  else if(esito == E_BUSERR)
    return esito;
  /* Transmit data */
  for(i=0;i<byte_count;i+=2)
    {
      datatemp=(ushort)source_buff[i]<<8 | source_buff[i+1];
      if((esito=send_comm(v288addr,datatemp)) == TIMEOUT)
	return E_NO_Q_DATA;
      else if(esito == E_BUSERR)
	return esito;
    }
  /* Start transmission */
  if((esito=send_comm(TXMIT,mstident)) == TIMEOUT)
    return E_NO_Q_TX;
  else if(esito == E_BUSERR)
    return esito;
  if(wait_resp(&dato) == TIMEOUT)
    return E_NO_Q_RX;
  return dato;
}

/***------------------------------------------------------------------------
    Read_caenet_buffer: Called by user programs to load "byte_count" bytes from
    CAENET buffer into the buffer pointed by "*dest_buff".
    --------------------------------------------------------------------***/
int read_caenet_buffer(dest_buff,byte_count)
     uchar *dest_buff;
     int byte_count;
{
  int i=0;
  ushort datatemp=0;
  int ret=0;

  for(i=0;i<byte_count;i+=2)
    {
      ret=read_data(&datatemp);
      /*            printf("read_caen_buffer():%0X  byte_cnt=%d\n",ret, i); */
      if(ret == TIMEOUT && i<byte_count-1) 
	{
	  printf("read_caenet_buffer(): Error.. i=%d  byte_count=%d\n",i, byte_count);
	return E_LESSDATA;
      }
      dest_buff[i] = HIBYTE(datatemp);
      dest_buff[i+1] = LOBYTE(datatemp);

    }
  /*  printf("read_caenet_buffer(): ret=%x (%d) byte_cnt=%d  i=%d\n", ret, ret, byte_count, i);*/
  return TUTTOK;
}

/***** END of vmcaenet.c *******************************************************/

/***------------------------------------------------------------------------
    Makemenu
--------------------------------------------------------------------***/
int makemenu()
{
  int c;
  /*  clrscr();*/
  /*  highvideo(); */
  puts("\n - MAIN MENU - \n\n");
  /*  normvideo(); */
  printf("  ----- Crate # %0d -----\n",craten);
  puts(" [0] - Read Module Identifier ");
  puts(" [1] - Crate Map ");
  puts(" [2] - Channels Monitor ");
  puts(" [3] - Speed test ");
  puts(" [4] - Parameter Setting ");
  puts(" [5] - Change Crate Number ");
  puts("\n [9] - Quit ");
  while((c=getchar()-'0') < 0 && c > 5);
  return c;
}

/****************************************************************************************
 *
 * chck_hvstat(int crate) - check status (HV ON or HV OFF) in all boards
 *                         returns 1 -if any channel reports HV ON in status word (bit 15)
 *                                 0 - if all channels report HV Off in status word (bit 15)
 *    Note!! for A932A board channel# 24 is primary (master) channel generates HV
 ******************************************************************************************/
int chck_hvstat(int crate)
{
  int stat=0;
  int bdi=0;
  int ch=0;
  
  for(bdi=0; bdi<10; bdi++)
    {
      if( (crates[crate].config>>bdi) &1) {
	if( strncmp(crates[crate].brds[bdi].name,"A932A", 5)==0 ) {
	  /* check only chanel 24 status */
	  if(dbg)  printf("crate=%d board=%d  name=%s\n", crate, bdi, crates[crate].brds[bdi].name);
	  ch=24;
	    stat = stat | crates[crate].bch[bdi].read[ch].status;
	}
	else     /* check all channels status */
	  for(ch=0; ch < crates[crate].brds[bdi].numch; ch++)
	    stat = stat | crates[crate].bch[bdi].read[ch].status;
      }
    }
     
  printf("crate=%d  hv_status=%d (%x) \n", crate, (stat>>15)&1, stat);
  if( (stat & 0x8000) ==0x8000 ) stat=1;
  else stat=0;
  
  return stat;
}

/*********************************************************************
* 
* kill_all(crate) - kill(HV OFF) all channels in crate
**********************************************************/
int kill_all(int crate)
{
  int response;
  ushort cod;
  ushort cr_indx;

  /* take semaphore */
  if(semMutex!=NULL) {
    semTake (semMutex, WAIT_FOREVER); 
  } else return ERROR;
  
  cr_indx=crate;
  cod=KILL_CHANNELS_1;

  printf("kill_all(): addr=%x  crate=%d  code=%x \n",v288addr, cr_indx, cod);
  taskDelay(6); /* it is not good here, but ...*/

 /* kill command #1 */
  if((response=caenet_read2(cr_indx, cod, NULL, 0)) != TUTTOK && response != E_LESSDATA)
    {
      printf(" kill_all(1): Error number %d (%x) received ( crate=%d )\n",
	     response, 0xFFFF&response, cr_indx);
      /* give semaphore */
      semGive (semMutex);
      return ERROR; /* not present */
    }

  cod=KILL_CHANNELS_2;
  /*  taskDelay(1);*/
 /* kill command #2 */
  if((response=caenet_read2(cr_indx, cod, NULL, 0)) != TUTTOK && response != E_LESSDATA)
    {
      printf(" kill_all(2): Error number %d (%x) received ( crate=%d )\n",
	     response, 0xFFFF&response, cr_indx);
      /* give semaphore */
      semGive (semMutex);
      return ERROR; /* not present */
    }

  /* give semaphore */
  semGive (semMutex);
  return 0;
}


/*********************************************************************
* 
* clr_alarm() - clear alarm
**********************************************************/
int clr_alarm(int crate)
{
  int response;
  ushort cod;
  ushort cr_indx;

  /* take semaphore */
  if(semMutex!=NULL) {
    semTake (semMutex, WAIT_FOREVER); 
  } else return ERROR;
  
  cr_indx=crate;
  cod=CLEAR_ALARM;

  printf("clr_alarm(): addr=%x  crate=%d  code=%x \n",v288addr, cr_indx, cod);
  taskDelay(6); /* it is not good here, but ...*/

 /* send command */
  if((response=caenet_read2(cr_indx, cod, NULL, 0)) != TUTTOK && response != E_LESSDATA)
    {
      printf(" clr_alarm(1): Error number %d (%x) received ( crate=%d )\n",
	     response, 0xFFFF&response, cr_indx);
      /* give semaphore */
      semGive (semMutex);
      return ERROR; /* not present */
    }

  /* give semaphore */
  semGive (semMutex);
  return 0;
}

/*********************************************************************
* 
* read_gstat() - read general status words and hardware status word
**********************************************************/
int read_gstat(int crate)
{
  int response;
  unsigned short tempbuff[2];
  ushort cod;
  ushort cr_indx;

  /* take semaphore */
  if(semMutex!=NULL) {
    semTake (semMutex, WAIT_FOREVER); 
  } else return ERROR;
  
  cr_indx=crate;
  cod=READ_GEN_STATUS;

  printf("read_gstat(): addr=%x  crate=%d  code=%x \n",v288addr, cr_indx, cod);
  taskDelay(6); /* it is not good here, but ...*/

 /* read general status word */
  if((response=caenet_read2(cr_indx, cod, tempbuff, sizeof(tempbuff))) != TUTTOK && response != E_LESSDATA)
    {
      printf(" read_gstat(1): Error number %d (%x) received ( crate=%d )\n",
	     response, 0xFFFF&response, cr_indx);
      /*      puts(" Press any key to continue ");
	      getchar(); */

      /* give semaphore */
      semGive (semMutex);
      return ERROR; /* not present */
    }

  crates[cr_indx].gstatus[0]=tempbuff[0];
  crates[cr_indx].gstatus[1]=tempbuff[1];
  printf("  Status Alalrm = %x \n General status = %x\n", crates[cr_indx].gstatus[0], crates[cr_indx].gstatus[1]);
  
  /* read hardware status word */
  cod=READ_HWSTAT;
  if((response=caenet_read2(cr_indx, cod, tempbuff, sizeof(tempbuff[0]))) != TUTTOK && response != E_LESSDATA)
    {
      printf(" read_gstat(2): Error number %d (%x) received ( crate=%d )\n",
	     response, 0xFFFF&response, cr_indx);
      /*      puts(" Press any key to continue ");
	      getchar(); */

      /* give semaphore */
      semGive (semMutex);
      return ERROR; /* not present */
    }

  crates[cr_indx].hwstatus=tempbuff[0];

  printf(" Hardware status word : %x\n", crates[cr_indx].hwstatus);

  /* give semaphore */
  semGive (semMutex);
  return 0;
}


/***------------------------------------------------------------------------
    Read_Ident
--------------------------------------------------------------------***/
int read_ident(int crate)
{
  int i,response;
  char sy527ident[12]; 
  char tempbuff[22];
  ushort cod;
  ushort cr_indx;

  /* take semaphore */
  if(semMutex!=NULL) {
    semTake (semMutex, WAIT_FOREVER); 
  } else return ERROR;
  
  cr_indx=crate;
  cod=IDENT;

  printf("read_ident(): addr=%x  crate=%d  code=%x \n",v288addr, cr_indx, cod);
  taskDelay(6); /* it is not good here, but ...*/
  /* To see if sy527 is present */
  if((response=caenet_read2(cr_indx, cod, tempbuff,22)) != TUTTOK && response != E_LESSDATA)
    {
      printf(" read_ident(): Error number %d (%x) received ( crate=%d )\n",
	     response, 0xFFFF&response, cr_indx);
      /*      puts(" Press any key to continue ");
	      getchar(); */
      sprintf(crates[cr_indx].ident,"Not present");
      crates[cr_indx].number=-1;
      crates[cr_indx].connected=0;

      /* give semaphore */
      semGive (semMutex);
      return ERROR; /* not present */
    }
  /* crate is here */
  for(i=0;i<11;i++) {
      sy527ident[i]=tempbuff[2*i+1];
  }

  sy527ident[i]='\0';
  memcpy(crates[cr_indx].ident, sy527ident, sizeof(sy527ident));
  crates[cr_indx].number=cr_indx;
  crates[cr_indx].connected=1;
  /*  crates[cr_indx].ident = sy527ident;sy527ident*/
  /*  printf(" The module has answered : %s\n",sy527ident);*/
  printf(" The module has answered : %s\n", crates[cr_indx].ident);

  /* give semaphore */
  semGive (semMutex);
  return 0;
}

/***------------------------------------------------------------------------
    Get_Bd_Info
--------------------------------------------------------------------***/
int get_bd_info(bd)
     struct board *bd;
{
  char    bd_info[54];
  int   response;
  
  if((response=read_caenet_buffer((char *)bd_info,sizeof(bd_info))) != TUTTOK)
    return response;
  
  strncat(bd->name,bd_info,5);
  bd->curr_mis = bd_info[5];
  memcpy(&(bd->sernum),&bd_info[6],2);
  bd->vermaior = bd_info[8];
  bd->verminor = bd_info[9];
  bd->numch = bd_info[30];
  memcpy(&(bd->omog),&bd_info[31],4);
  memcpy(&(bd->vmax),&bd_info[35],4);
  memcpy(&(bd->imax),&bd_info[39],2);
  memcpy(&(bd->rmin),&bd_info[41],2);
  memcpy(&(bd->rmax),&bd_info[43],2);
  memcpy(&(bd->resv),&bd_info[45],2);
  memcpy(&(bd->resi),&bd_info[47],2);
  memcpy(&(bd->decv),&bd_info[49],2);
  memcpy(&(bd->deci),&bd_info[51],2);
  return TUTTOK;
}

/***------------------------------------------------------------------------
    Get_Cr_Info
--------------------------------------------------------------------***/
int get_cr_info(cr_cnf, cr_num,  brds)
     ushort *cr_cnf;
struct board *brds;
{
  int    i,response;
  ushort bd;
  ushort cod;

  cod=READ_CR_CONF;

  if((response=caenet_read2(cr_num, cod, (char *)cr_cnf,sizeof(ushort))) != TUTTOK)
    {
      printf(" get_cr_info();  Caenet_read: Error number %d received\n",response);

      return response;
    }

  cod=READ_BOARD_INFO;

  for( bd=0, i=1 ; bd<10 ; bd++, i = i << 1 )
    if(*cr_cnf & i)
      {
	if((response=caenet_write2(cr_num, cod, (char *)&bd,sizeof(ushort))) != TUTTOK)
	  {
	    printf(" get_cr_info(): Caenet_write: Error number %d received\n",response);
	    /*	    puts(" Press any key to continue ");*/
	    /*  getchar(); */
	    return response;
	  }
	
	else
	  {
	    response=get_bd_info((struct board *)&brds[bd]);
	    if(response != TUTTOK)
	      {
		printf(" get_cr_info(): Read_Caenet_Buffer: Error number %d received\n",response);
		return response;
	      }
	  }
      }

  return TUTTOK;
}

/***------------------------------------------------------------------------
Crate_Map
--------------------------------------------------------------------***/
int crate_map(int cr_num)
{
  static char *curr_umis[] = 
    {
      " A",
      "mA",
      "uA",
      "nA"
    };
  
  int i, bcnt=0;
  float im;
  ushort bd;
  ushort cr_conf; /* crate config (each bit=1 in position repersents module in corresponded slot */
  struct board  lboards[10];  /* modules parameters */

  /* take semaphore */
  if(semMutex!=NULL) {
    semTake (semMutex, WAIT_FOREVER); 
  } else {
    printf("crate_map(): ERROR semMuitex=NULL\n");
    return ERROR;
  }

  memset((char *) lboards, 0, sizeof(lboards));
  
  if(get_cr_info(&cr_conf, cr_num, lboards) != TUTTOK) /* Get information about the Crate Configuration */
    {
      printf(" crate_map(): get_cr_info(): Error received\n");
      /* give semaphore */
      semGive (semMutex);
      return ERROR;
    } 
 /* clrscr();*/
 printf(" cr_conf=%x ", cr_conf);
 printf("\n\n --- Crate Map ---# %0d \n\n\n\n\n ", cr_num);
 for( bd=0, i=1 ; bd<10 ; bd++, i = i << 1 )
   {     
     printf(" Slot %d - ",bd);
     if(cr_conf & i)
       {
	 /*crates[cr_num].brds[bd].name = boards[bd].name ;*/ /* wrong!!! */

	 memcpy(crates[cr_num].brds[bd].name, lboards[bd].name, sizeof( lboards[bd].name));
	 crates[cr_num].brds[bd].numch = lboards[bd].numch ;
	 crates[cr_num].brds[bd].omog = lboards[bd].omog;
	 crates[cr_num].brds[bd].vermaior = lboards[bd].vermaior;
	 crates[cr_num].brds[bd].verminor = lboards[bd].verminor;
	 crates[cr_num].brds[bd].vmax = lboards[bd].vmax;
	 crates[cr_num].brds[bd].imax = lboards[bd].imax;
	 crates[cr_num].brds[bd].rmin = lboards[bd].rmin;
	 crates[cr_num].brds[bd].rmax = lboards[bd].rmax;
	 crates[cr_num].brds[bd].resv = lboards[bd].resv;
	 crates[cr_num].brds[bd].resi = lboards[bd].resi;
	 crates[cr_num].brds[bd].decv = lboards[bd].decv;
	 crates[cr_num].brds[bd].deci = lboards[bd].deci;
	 crates[cr_num].brds[bd].curr_mis = lboards[bd].curr_mis;
	 crates[cr_num].brds[bd].sernum = lboards[bd].sernum;

	 printf("lboards[bd].omog=%8X\n", (int) lboards[bd].omog);
	  if( lboards[bd].omog & (1L<<17) )      /* The board is not homogeneous */
	 /*if( lboards[bd].omog & (1L<<9) )*/      /* The board is not homogeneous */
	   {
	     printf(" The board=%0d is not homogeneous: %0X \n", bd, (int)lboards[bd].omog );
	   }
	 /*memcpy(crates[cr_num].brds, boards, sizeof(boards));*/
	 crates[cr_num].ps[bd].prop_num = PROP_NUM;

	 printf(" Mod. %s %3d CH ",crates[cr_num].brds[bd].name, lboards[bd].numch);
	 printf(" %4dV", (int) lboards[bd].vmax);
	 im = (float)lboards[bd].imax/pow_10[lboards[bd].deci];
	 printf(" %8.2f",im);
	 printf("%s",curr_umis[lboards[bd].curr_mis]);
	 printf(" %3d_%3dV", lboards[bd].rmin, lboards[bd].rmax);

	 printf(" --- Ser. %3d, Rel. %d.%02d\n",
		lboards[bd].sernum, lboards[bd].vermaior, lboards[bd].verminor);

	 bcnt++;
       }

     else
       printf(" Not Present \n");     
   }
 
 crates[cr_num].bd_num = bcnt; /* store number of inserted boards */
 crates[cr_num].config=cr_conf;
 printf(" crates[%0d].config=%x \n", cr_num, crates[cr_num].config);
 puts("\n\n");
 /* puts("\n\n\n Press any key to continue ");
 getchar(); */

 /* give semaphore */
 semGive (semMutex);
 return 0;
}

/*******________* My staff (R.P.) *______________*******/

/*---------*******************************************
 * returns properties list fdefined as string in 'PROP'
 *---------*******************************************/
int get_PROP(char *sprop) 
{
  strncpy(sprop, PROP, sizeof(PROP));
  return 0;
}
/*---------*************************************
 * returns number of properties defined in 'PROP' 
 *         and crates[cr_num].ps[bd].prop_num
 *---------*************************************/ 
int get_nprop(int cr_num, int bd) 
{
  return crates[cr_num].ps[bd].prop_num;
}

/*---------******************************
 * summary numbers update routinews
 *---------******************************/
int update_GS(int cr_num, int flag) 
{
  int ii=0;
  if(flag<0) { /* update all GS */
    for(ii=0;ii<5;ii++) crates[cr_num].gsn[ii]++;
  } else crates[cr_num].gsn[flag]++;  
  return 0;
}

/*** get_GS ****/
int get_GS(unsigned short cr_num, char *gss) 
{
  ULONG c_time;
  int i, kk, jj;
  
  /*  set_crnum(cr_num);  set craten = cr_num */
  /*  craten=cr_num;*/
  c_time = tickGet();

  if(0)/* if(0) ----------------------*/
    {
  if( (c_time - crates[cr_num].gs_time) >  GS_TIME_SLICE) /* update summary numbers */
    {
      for( kk=0, i=1 ; kk<10 ; kk++, i = i << 1 )
	{     
	  if(crates[cr_num].config & i)
	    {
	      for( jj=0 ; jj<PROP_NUM ; jj++ )
		{     
		  update_PS(kk, jj, 0);  /* update ALL properties summary number*/
		}
	    }
	}
      crates[cr_num].gs_time=tickGet();
      crates[cr_num].ps_time=tickGet();
    } 

  else if ( (c_time - crates[cr_num].ps_time) >  PS_TIME_SLICE)
    {
      update_PS(crates[cr_num].next_bd, 1, 1); /* Imon */
      update_PS(crates[cr_num].next_bd, 2, 1); /* Vmon */
      update_PS(crates[cr_num].next_bd, 5, 1); /* ST */
      /*      update_PS(crates[cr_num].next_bd, 10,1); */ /* STATUS */
      /* increment 'next_bd'  to next board */
      if(crates[cr_num].next_bd==9) crates[cr_num].next_bd=0;
      else
	{
	  for( kk=crates[cr_num].next_bd+1, i= (1<<(crates[cr_num].next_bd+1)) ; kk<10 ; kk++, i = i << 1 ) 
	    {
	      if(crates[cr_num].config & i)
		{
		  crates[cr_num].next_bd=kk;
		break;
		}
	    }
	}
      crates[cr_num].ps_time=tickGet();   
    }

  else /* check if crate connected and status */
    {
      if(crates[cr_num].connected==1)
	{
	  if(read_ident(cr_num)<0) 
	    {
	      /* crate disconnected */
	      printf("update_GS(1); ERROR: crate# %d diconnected or power OFF\n",cr_num);
	      update_GS(cr_num, 2); /* configuration summary number */
	    }
	}
      else 	  /* check if crate connected again */
	{
	if(read_ident(cr_num)==0) 
	  {
	    /* crate connected */
	    printf("update_GS(2): crate# %d connected or powered ON\n",cr_num);
	    update_GS(cr_num, 2); /* configuration summary number */
	    for( jj=0 ; jj<PROP_NUM ; jj++ )
	      {     
		update_PS(-1, jj, 0);  /* update ALL properties summary number*/
	      }
	  } 
	else 	     
	  printf("update_GS(3);  crate# %d diconnected or power OFF\n",cr_num);
	}     
    }
    } /* if(0) ----------------------*/

  /*  update_GS(cr_num, 3); *//* update(incremetn) host activitty number */
  update_GS(cr_num, 4); /* update(incremetn) host activitty number */
  
  sprintf(gss, "%4X %4x %4X %4x %4x", crates[cr_num].gsn[0], crates[cr_num].gsn[1], 
	  crates[cr_num].gsn[2], crates[cr_num].gsn[3], crates[cr_num].gsn[4]);
  
  return 0;
}

/**** LS ****/
int update_LS(int cr_num, int bd, int meas) 
{
  int i, kk ;
 
  if(bd<0) /* for All boards */
    {
      for( kk=0, i=1 ; kk<10 ; kk++, i = i << 1 )
	{     
	  if(crates[cr_num].config & i)
	    {
	      crates[cr_num].lsn[2*kk]++ ;
	      crates[cr_num].lsn[2*kk+1]++ ;	      
	    }
	}
      update_GS(cr_num, 0); /* measured */
      update_GS(cr_num, 1); /* settable */    
    } 
  else /* for ONE board */
    {
      if(meas==1) 
	{
	  crates[cr_num].lsn[2*bd]++;   /* measurable LS */
	  update_GS(cr_num, 0); /* measured */
	}
      else 
	{
	  crates[cr_num].lsn[2*bd+1]++; /* settable LS */
	  update_GS(cr_num, 1); /* settable */  
	} 
    }
  
  return 0;
}

/*** get_LS ****/
int get_LS(unsigned short cr_num, char *lss) 
{ 
  int nboard=0;
  int ii=0;
  char tmp1[10];

  nboard=crates[cr_num].bd_num;
  /* nboard=10;*/ /**** TEST ONLY ***/
  for(ii=0; ii < 2*nboard; ii++) {
    sprintf(tmp1," %04X", crates[cr_num].lsn[ii]);                
    strcat(lss, tmp1);
  }
  return 0;
}

/**** PS ****/
int update_PS(int cr_num, int bd, int prop, int meas) 
{
  int kk, i;

  if( (bd<10) && ((crates[cr_num].config >> bd ) &1) ) /* check if board in slot=bd is present */ 
    {    
      crates[cr_num].ps[bd].psn[prop]++;
      if(meas==1)
	update_LS(cr_num, bd, meas); /* only status (measured */
      else if(meas==2) {
	update_LS(cr_num, bd, 1); 
	update_LS(cr_num, bd, 0);
      } 
      else if(meas==0) {
	update_LS(cr_num, bd, 0); /* only settings */
      }
    }
  /* for All boards */  
  if(bd<0) 
    {
      for( kk=0, i=1 ; kk<10 ; kk++, i = i << 1 )
	{     
	  if(crates[cr_num].config & i)
	    {
	      crates[cr_num].ps[kk].psn[prop]++;
	      update_LS(cr_num, kk, 0); 
	      update_LS(cr_num, kk, 1);
	    }
	}
    }

  return 0; 
}

int get_PS(unsigned short cr_num, char *pss, int bd) 
{
  int nprop=0;
  int ii=0;
  char tmp1[10];

  nprop=PROP_NUM;
  if( (bd<10) && ((crates[cr_num].config >> bd ) &1) ) /* check if board in slot=bd is present */ 
    {  
      for(ii=0; ii < nprop; ii++) 
	{
	  sprintf(tmp1," %04X", crates[cr_num].ps[bd].psn[ii]);                
	  strcat(pss, tmp1);
	}
      return 0;
    }
  else 
    return ERROR;
}

/*******************/
int clr_mem() {
  bzero((char *) crates, sizeof(crates));
  bzero((char *) crates_num, sizeof(crates_num));
  /*  bzero((char *) boards, sizeof(boards));*/
  bzero((char *) ch_set, sizeof(ch_set)); 
  bzero((char *) ch_read, sizeof(ch_read)); 
 /*  bzero((char *) ch_set, size_hvch); */
 /*  bzero((char *) ch_read, size_hvrd); */
  num_crate=0;
  return 0;
}
/* ***********************************************************************
 * init_sy527(int crt) - initialize crates (struct crates[], borads[],..)
 * Params: int crt - crate number, if crt=0, then check all crates.
 * Returns: if crt=0, number of connected crates; if crt=crate#,
 *          OK if crate connected and ERROR if crate with crate# not connected
 ************************************************************************/
int init_sy527(int crt) 
{
  int   err=0;
  char *to_print="0";
  char stemp[4];
  int   cnt=0;

  if(crt==0)
    {
      clr_mem();
    }
    
  if(crt==0) /* check connection to all crates 1-15 */
    {
    con_crates=0;
    for(kk=0; kk<MAX_CRATES; kk++)
	{
	  craten=kk;
	  err=read_ident(kk);
	  if(err==ERROR) {
	    printf("\ninit_sy527(): Crate %d NOT present/connected!\n", kk);
	  } else {
	    con_crates = con_crates | (1<<(kk)) ;		
	    crates_num[num_crate]=kk;
	    read_gstat(kk);
	    num_crate++;
	    printf("\nCrate %d connected!\n", kk);
	    sprintf(stemp," %0d", kk);
	    strcat(to_print, stemp);
	    /* get crate map */
	    if(crate_map(kk)) {	      
	      cnt++;
	    } else {
	      printf("\n init_sy527(): crate_map(): ERROR\n");
	    }
	  }
	  taskDelay(1);
	}
      printf("\n\nCrate mask %08X : connected crates: %s \n", con_crates, to_print);      
    }

  else if(crt<=MAX_CRATES) /* init one crate */
    {
      craten=crt;
      err=read_ident(crt);
      if(err==ERROR) {
	printf("init_sy527(): Crate %d NOT present/connected!\n", crt);
	return ERROR;
      } else {
	con_crates = con_crates | (1<<crt) ;		
	printf("Crate# %d connected!\n", crt);		
	read_gstat(crt);
	if((err=crate_map(crt))==0) {
	  printf("Crate# %d initialized!\n", crt);		
	  return OK;
	} else {
	  printf("\n init_sy527(): crate_map(): ERROR\n");
	  return ERROR;
	}
      }
    }

  printf("\n\nCrate mask %08X \n", con_crates);      

  return cnt;

}

/*********************
 * set crate number to global var 'craten'
 *********************/
int set_crnum(unsigned short cr) {

  /** take semaphor **/
  craten=cr;
  /** give semaphore **/
  return craten;
}

/*** semaphore init **/
int sem_init() {
/* Create a binary semaphore that is initially full. 
 * Tasks blocked on semaphore wait in FIFO (not priority) order. 
*/ 
         
  if(semMutex==NULL) {
    /*semMutex = semBCreate (SEM_Q_PRIORITY, SEM_FULL);*/
    semMutex = semBCreate (SEM_Q_FIFO, SEM_FULL);
    if(!semMutex) {
      return ERROR;
    }
  }
  return 0;
}
/*****************/
int get_val(int val) 
{
  if(semMutex!=NULL) {
    semTake (semMutex, WAIT_FOREVER); 
    kk++;
    if(kk>1000) kk=0;
    taskDelay(5);
    semGive (semMutex);
  } else return ERROR;
  
  return kk;
}

/**************************************************
 * addr_ltos(unsigned short indx) -  returns slot number 
 * from logical number =indx (in command module address, like: L0, L0.1, ...)
 * or ERROR if logical number not found.
 **************************************************/
int addr_ltos(unsigned short cr_num, int indx) 
{
  unsigned short ii;
  int cnt=0;

  for(ii=0;ii<10;ii++) 
    {
      if((crates[cr_num].config >> ii) & 1) 
	cnt++;
      if(indx==(cnt-1)) 
	return (ii);       
    }
  return ERROR;
};

/************************************************************
 * converts bits positin in channel STATUS word of CAEN sy527
 * to  corresponded bits position in STATUS word of LeCroy-1458
 **************************************************************/
unsigned short stat_conv(unsigned short st) 
{
  unsigned short i;
  unsigned short le_st=0;

  for(i=0;i<16;i++) 
    {
      if((st>>i) & 1) 
	switch (i) 
	  {
	  case 15: le_st= le_st | 0x0001; /* On */   /* set bit 0 - channel ON */ 
	    break;
	  case 14: le_st= le_st | 0x0002; /* up */   /* set bit 1 - ramp UP */ 
	    break;      
	  case 13: le_st= le_st | 0x0004; /* down */   /* set bit 2 - ramp Down */ 
	    break;      
	  case 12: le_st= le_st | 0x0020; /* overcurrent */   /* set bit 5  - trip : user current limit */ 
	    break;      
	    /*	  case 11: */ /* undervoltage */
	  case 10: le_st= le_st | 0x0080; /* overvoltage */ /* set bit 7 - trip: voltage limit */ 
	    break;
	  case 9: /* ext. trip */
	  case 5: le_st= le_st | 0x0040;  /* internal trip */ /* set bit 6 - trip: voltage error*/ 
	    break;
	  case 8: le_st= le_st | 0x0080;  /* Vmax */   /* set bit 7 - trip: voltage limit */ 
	    break;
	  }
    }
  return le_st;
}


/*-------------------------------------------------------------------------------
 *
 *  code_to_indx() - converts from command code to property index in array PROP[]
 *                  called from do_ch().
 *------------------------------------------------------------------------------*/
int code_to_indx(unsigned short code) 
{
  int indx=0;

  switch(code)                          /* Decode the par. */
    {
    case 16: indx=V0SET;
      break;
    case 17: indx=V1SET;
      break;
    case 18: indx=I0SET;
      break;
    case 19: indx=I1SET;
      break;
    case 20: indx=SVMAX;
      break;
    case 21: indx=RUP;
      break;
    case 22: indx=RDWN;
      break;
    case 23: indx=TRIP;
      break;
    case 24: indx=FLAG;
      break;
    default: indx=V0SET;
    }
  return indx;
}

/*****************************************************
| 
* prop_to_indx() - returns index of property in 'tmp' 
*                  or ERROR if property not found.
|
******************************************************/
int prop_to_indx(char *tmp) 
{
  if(strncmp(tmp,"DV",2) == 0)       /** DV property **/           
    return V0SET;
  else if(strncmp(tmp,"DC",2) == 0)  /** DC property **/
    return I0SET;
  else if(strncmp(tmp,"CE",2) == 0)  /** CE property **/      
    return PW;  
  else if(strncmp(tmp,"RUP",3) == 0)  /** RUP property **/            
    return RUP;
  else if(strncmp(tmp,"RDN",3) == 0)  /** RDN property **/            
    return RDWN;  
  else if(strncmp(tmp,"STAT",4) == 0) /** STAT property **/      
    return STAT;  
  else if(strncmp(tmp,"ST",2) == 0)   /** ST property **/      
    return ST;  
  /*  else if(strncmp(tmp,"MVDZ",4) == 0)         
      return V0set;
      else if(strncmp(tmp,"MCDZ",4) == 0)           
      return V0set; 
*/ 
  else if(strncmp(tmp,"TC",2) == 0)  /** TC property **/      
    return TRIP;
  else if(strncmp(tmp,"HVL",3) == 0)  /** HVL property **/            
    return HVMAX;
  else if(strncmp(tmp,"SVL",3) == 0)  /** SVL property **/            
    return SVMAX;  
  else if(strncmp(tmp,"MV",2) == 0)   /** MV property **/     
    return VMON;  
  else if(strncmp(tmp,"MC",2) == 0)   /** MC property **/      
    return IMON;
  else if(strncmp(tmp,"FLAG",4) == 0) /** FLAG property **/      
    return FLAG;

  return -1;
}


/********************************************************
 * get_prop() get value for one channel, one property
 *******************************************************/
int no_get_prop(char *tmp1, ushort bd, ushort ch, int prop) 
{
  float    scalei,scalev;
  scalev = pow_10[crates[craten].brds[bd].decv];
  scalei = pow_10[crates[craten].brds[bd].deci];
  switch(prop) {
  case V0SET: sprintf(tmp1," %7.1f", ch_set[ch].v0set/scalev);
    break;
  case V1SET: sprintf(tmp1," %7.1f", ch_set[ch].v1set/scalev);
    break;
  case I0SET: sprintf(tmp1," %7.1f", ch_set[ch].i0set/scalei);
    break;
  case I1SET: sprintf(tmp1," %7.1f", ch_set[ch].i1set/scalei);
    break;
  case SVMAX: sprintf(tmp1," %4d",   ch_set[ch].vmax);
    break;
  case RUP:   sprintf(tmp1," %3d",   ch_set[ch].rup);
    break;
  case RDWN:  sprintf(tmp1," %3d",   ch_set[ch].rdwn);
    break;
  case TRIP:  sprintf(tmp1," %5.1f", ch_set[ch].trip/10.0);
    break;
  case VMON:  sprintf(tmp1," %7.1f", ch_read[ch].vread/scalev);
    break;
  case IMON:  sprintf(tmp1," %7.1f", ch_read[ch].iread/scalei);
    break;
  case FLAG:  sprintf(tmp1," %04X",  ch_set[ch].flag );
    break;
  case STAT:  sprintf(tmp1," %04X",  ch_read[ch].status);
    break;
    /* bits combination */
  case ST:    sprintf(tmp1," %04X", stat_conv(ch_read[ch].status)); /* converted for STATUS word of LeCroy*/
    break;
  case PW:    sprintf(tmp1," %1d",  1 & (ch_set[ch].flag >> 11));
    break;
  default:    sprintf(tmp1," n/a");
    break;   
  }   

  return 0;
}
/********************************************************
 * get_prop() get value for one channel, one property
 *******************************************************/
int get_prop(char *tmp1, ushort crt, ushort bd, ushort ch, int prop) 
{
  float    scalei, scalev;
  
  scalev = pow_10[crates[crt].brds[bd].decv];
  scalei = pow_10[crates[crt].brds[bd].deci];

  switch(prop) {
  case V0SET: sprintf(tmp1," %7.1f", crates[crt].bch[bd].set[ch].v0set/scalev);
    break;
  case V1SET: sprintf(tmp1," %7.1f", crates[crt].bch[bd].set[ch].v1set/scalev);
    break;
  case I0SET: sprintf(tmp1," %7.2f", crates[crt].bch[bd].set[ch].i0set/scalei);
    break;
  case I1SET: sprintf(tmp1," %7.2f", crates[crt].bch[bd].set[ch].i1set/scalei);
    break;
  case SVMAX: sprintf(tmp1," %4d",   crates[crt].bch[bd].set[ch].vmax);
    break;
  case RUP:   sprintf(tmp1," %3d",   crates[crt].bch[bd].set[ch].rup);
    break;
  case RDWN:  sprintf(tmp1," %3d",   crates[crt].bch[bd].set[ch].rdwn);
    break;
  case TRIP:  sprintf(tmp1," %5.1f", crates[crt].bch[bd].set[ch].trip/10.0);
    break;
  case VMON:  sprintf(tmp1," %7.1f", crates[crt].bch[bd].read[ch].vread/scalev);
    break;
  case IMON:  sprintf(tmp1," %7.3f", crates[crt].bch[bd].read[ch].iread/scalei);
    break;
  case FLAG:  sprintf(tmp1," %04X",  crates[crt].bch[bd].set[ch].flag );
    break;
  case STAT:  sprintf(tmp1," %04X",  crates[crt].bch[bd].read[ch].status);
    break;
    /* bits combination */
  case ST:    sprintf(tmp1," %04X", stat_conv(crates[crt].bch[bd].read[ch].status)); /* converted for STATUS word of LeCroy*/
    break;
  case PW:    sprintf(tmp1," %1d",  1 & (crates[crt].bch[bd].set[ch].flag >> 11)); /* channel ON/OFF status */
    break;
  default:    sprintf(tmp1," n/a");
    break;   
  }   

  if(dbg==2)  printf(" get_prop(): crate=%d board=%d channel=%d prop=%d val=%s\n", crt, bd, ch, prop, tmp1); 
  return 0;
}
/********************************************************************
 *
 *
 ********************************************************************/
int comp_dzread(int crt, ushort brd, ushort chan, struct hvrd lch_read) 
{
  int ret=0;
  if(dbg==1) {
    printf(" compare: stored_val   monitored_val \n");
    printf(" V_MON :      %d            %d \n",(int) crates[crt].bch[brd].read[chan].vread, (int)lch_read.vread);
    printf(" I_MON :      %d            %d \n", crates[crt].bch[brd].read[chan].iread, lch_read.iread);
    printf(" STATUS:      %d            %d \n", crates[crt].bch[brd].read[chan].status, lch_read.status);
  }

  if( abs (crates[crt].bch[brd].read[chan].vread - lch_read.vread) >dzread[0]) 
    {
      update_PS(crt, brd, VMON, 1);
      ret++;
    }
  if( abs (crates[crt].bch[brd].read[chan].iread - lch_read.iread) >dzread[1]) 
    {
      update_PS(crt, brd, IMON, 1);
      ret++;
    }
  if( abs (crates[crt].bch[brd].read[chan].status - lch_read.status) >dzread[2]) 
    { 
      update_PS(crt, brd, STAT, 1);
      update_PS(crt, brd, ST, 1);
      update_PS(crt, brd, PW, 0);
     ret++;
    }
  
  update_GS(crt, 3); /* update crate activity summary number */
  return ret;
}

/********************************************************************
 *
 *
 ********************************************************************/
int comp_dzset(int crt, ushort brd, ushort chan, struct hvch lch_set) 
{
  int ret=0;
  /*
struct hvch
{
  char chname[12];
  long v0set, v1set;
  ushort i0set, i1set;
  short vmax;
  short rup, rdwn;
  short trip, dummy;
  ushort flag;
};
  */
     if( abs (crates[crt].bch[brd].set[chan].v0set - lch_set.v0set) >dzset[0]) 
      {
	update_PS(crt, brd, V0SET, 0);
	ret++;
      }
    if( abs (crates[crt].bch[brd].set[chan].i0set - lch_set.i0set) >dzset[2]) 
      {
	update_PS(crt, brd, I0SET, 0);
	ret++;
      }
    if( abs (crates[crt].bch[brd].set[chan].vmax - lch_set.vmax) >dzset[4]) 
      {
	update_PS(crt, brd, SVMAX, 0);
	ret++;
      }
    if( abs (crates[crt].bch[brd].set[chan].rup - lch_set.rup) >dzset[5]) 
      {
	update_PS(crt, brd, RUP, 0);
	ret++;
      }
    if( abs (crates[crt].bch[brd].set[chan].rdwn - lch_set.rdwn) >dzset[6]) 
      {
	update_PS(crt, brd, RDWN, 0);
	ret++;
      }
    if( abs (crates[crt].bch[brd].set[chan].trip - lch_set.trip) >dzset[7]) 
      {
	update_PS(crt, brd, TRIP, 0);
	ret++;
      }
    if( abs (crates[crt].bch[brd].set[chan].flag - lch_set.flag) >dzset[8]) 
      {
	update_PS(crt, brd, FLAG, 0);
	update_PS(crt, brd, PW, 0);

	ret++;
      }
    return ret;
}

/****************************************************
 * do_ch - if set<3 - reads channel values (status and settings) 
 *           to structure ch_read[channel] and ch_set[].
 *           if set >3 set channel value that stored in buff
 *           set=0 read measured values, 1 - read settable values
 *           set=2 read both measured valuse and settable values
 *           set>15 set<25 then  set=commannd code
 *----------------------------------------------------*/
int do_ch(int cr_num, int board, int chan, int set, char *buff) 
{
  ushort com_code;
  ushort crt;
  int response; 
  ushort ch_addr;
  struct hvch   lch_set;  /* Settings of 25 chs.*/
  struct hvrd   lch_read; /* Status of 25 chs.  */
  ushort b1,b2;

  crt=cr_num;
  ch_addr = (board<<8) | chan;

  taskDelay(1);
  if(dbg>2) printf("do_ch(); crate#=%d, board#=%d chan=%d set=%d\n", cr_num, board, chan, set);

  if((set==0) || (set==2)) /* read channel Vmon, Imon, status */
    {
      com_code = READ_STATUS;	      

      if(dbg>1) printf("do_ch(1); crate#=%d, board#=%d chan=%d ch_addr=%x com_code=%x\n", 
		       cr_num, board, chan, ch_addr, com_code);
      if((response=caenet_write2(crt, com_code, (char *)&ch_addr, sizeof(ushort))) != TUTTOK)
	{
	  printf("read_ch(1): Caenet_write: Error number %d (%0X) (cr=%d bd=%d ch=%d) \n",
		 response, response, cr_num, board, chan);
	  /*puts(" Press any key to continue ");
	    getchar(); */
	  return ERROR;
	}
      else
	{
	  /* response=read_caenet_buffer((char *)&ch_read[ch],sizeof(struct hvrd)); */
	  response=read_caenet_buffer((char *)&lch_read, size_hvrd());
	  if(response != TUTTOK)
	    {
	      printf("read_ch(2): Read_Caenet_Buffer: Error number %d received\n",response);
	      return ERROR;
	    }

	  /* check if there difirence between readed value and value stored in crates[] */
	  if( comp_dzread(crt, board, chan, lch_read)>0) 
	    /* copy ch_read to crates[] */
	    memcpy( &(crates[crt].bch[board].read[chan]), &lch_read, size_hvrd());

	}
    }
  if((set==1) || (set==2))
    { /* read settings */	
      com_code = READ_SETTINGS;
      if((response=caenet_write2(crt, com_code,  (char *)&ch_addr,sizeof(ushort))) != TUTTOK)
	{

	  printf("read_ch(3): Caenet_write: Error number %d (%0X) (cr=%d bd=%d ch=%d) \n",
		 response, response, cr_num, board, chan);
	  return ERROR;
	}
      else
	{
	  /*	  response=read_caenet_buffer((char *)&ch_set[chan], sizeof(struct hvch)); */
	  response=read_caenet_buffer((char *)&lch_set, size_hvch());	    
	  if(response != TUTTOK)
	    {	      
	      printf("read_ch(4): Read_Caenet_Buffer: Error number %d received\n",response);
	      return ERROR;
	    }

	  /* check if there difirence between readed value and value stored in crates[] */
	  if(comp_dzset(crt, board, chan, lch_set)>0)	  	  
	    /* copy ch_set to crates[] */
	    memcpy( &(crates[crt].bch[board].set[chan]), &lch_set, size_hvch());

	}
    }
  /*  if((set>15) && (set<25))*/
  if(set>15)
      {   /* set - is code for channel settings or command */
	
	com_code=set;
	
	memcpy(&b1, &buff[0],2);
	memcpy(&b2, &buff[2],2);
	
	if(dbg>1) 
	  printf("do_ch():caenet_write: crate=%d, board=%d, code=%x (%d),  buff[0]= %x,  buff[1]= %x (%d) \n\n", 
	       crt, board, com_code, com_code,
	       b1, b2, b2);
	/*	printf("sizeof(buff)=%d \n", sizeof(buff));*/
	taskDelay(1); /* when set value, module is busy ~20mS */

	/* update PS numbers for setted property (!even if error will be generated!*/
	update_PS(crt, board, code_to_indx(com_code), 0);
	if((response=caenet_write2(crt, com_code, buff, sizeof(buff))) != TUTTOK)
	  {
	    printf("read_ch(5): Caenet_write2: Error number %d (%0X) received\n",response, response);
	    return 0xFFFF&response;
	    /*	    return ERROR; */
	  }        
      }
  if(set>24) return ERROR;
  
  return TUTTOK; /* successful */
}

/***********************************************
*** wrapper with semaphore for do_ch() *****
**********************************************/ 
int wdo_ch(int wcr_num, int wboard, int wchan, int wset, char *buff) 
{
  int err=0;
  int r_err=0;
  int ii=0;
  int cnt=0;
  int icnt=0;
  int cr_num=0; 
  int board=0;
  int chan=0;
  int set=0;

  if(semMutex != NULL) {
    /** semaphore take **/
    semTake (semMutex, WAIT_FOREVER); 

    cr_num=wcr_num;
    board=wboard;
    chan=wchan;
    set=wset;
    icnt=0;

    if(chan>=0) /* ONE channel */
      {
	taskDelay(1);
	/** r_err = do_ch(cr_num, board, chan, set, buff); **/
	do 
	  {
	    r_err = do_ch(cr_num, board, chan, set, buff);
	    printf("wdo_ch(1): repeated (i=%d): cr=%d brd=%d ch=%d  err=%d\n", icnt, cr_num, board, chan, r_err);
	    taskDelay(1);
	    icnt++;
	  } while (r_err==0xFF00 && icnt<10); /* FF00 - module Busy??? */
	
	/**
	if(r_err<0) 
	{
	taskDelay(6);
	r_err=do_ch(cr_num, board, chan, set, buff);
	printf("wdo_ch(1): repeated: cr=%d brd=%d ch=%d  err=%d\n",cr_num, board, chan, r_err);
	}
	**/
      }

    else /* ALL channels */
      {
	for( ii=0 ; ii < 25 && ii < crates[cr_num].brds[board].numch ; ii++ ) 
	  {
	    err=do_ch(cr_num, board, ii, set, buff); /*  process 'err' for every channel */	
	    if(err<0) /* repeat one more time */
	      {
		/*		taskDelay(1);*/
		err=do_ch(cr_num, board, ii, set, buff);
		printf("wdo_ch(2): repeated: cr=%d brd=%d ch=%d  err=%d\n",cr_num, board, ii, err);
	      }
	    if(err<0) 
	      {
		r_err=err;
		if(2 <= cnt++) {
		  printf("wdo_ch(): break due ERROR\n");
		  break; /* many channels error means connection lost or crate is off */ 
		}		
	      }
	  }	
      }
     
  /** semaphore give **/
    semGive (semMutex);
  } else 
    {
      printf("wdo_ch(): ERROR: No initialized semaphore!\n");
      return ERROR; 
    }

  return r_err;
}

/************************************************************** 
 * get_chprop() - get all channels values for property 'prop'
 * Params: *tmp - output string value(s) of property 'prop',
 *         bd   - board slot number;
 *         chan - channel number, if chan<0, then readout all channels;
 *         prop - property index (in #define)
 * Returns: 0 - if OK; -1 - if any error.
 ****************************************** *******************/
int get_chprop(char *stmp, ushort cr_num, ushort bd, int chan, int prop) {

  int response;
  float    scalei=1., scalev=1.;
  ushort   ch, ch_addr;
  char tmp1[20];
  unsigned short crate;
  
  bzero((char *) tmp1, sizeof(tmp1));
  if(dbg)  printf("get_chprop_(); crate#=%d, board#=%d\n", cr_num, bd);

  crate=cr_num;
  if( (bd<10) && ((crates[crate].config >> bd ) &1) ) /* check if board in slot=bd is present */
    {
      scalev=pow_10[crates[crate].brds[bd].decv];
      scalei=pow_10[crates[crate].brds[bd].deci];
      
      if( chan<0) /* scan ALL channels */
	{
	  /* First update from Caenet the information about the channels */
	  /*      for( ch=0 ; ch < 25 && ch <crates[craten].brds[bd].numch ; ch++ )  */

	  if(0)
	  {
	    if(prop==1 || prop==2 || prop==5 ||  prop==10 ) /* read channels  Vmon, Imon, Status */
	      {
		if( (response = wdo_ch(crate, bd, -1, 0, NULL)) != TUTTOK) {
		  printf("get_chprop(1): wdo_ch: Error number %d received\n",response);
		  return ERROR;
		}
	      } /* if(prop==...) */
	    
	    else   /* read  channels Settings*/  
	      {
		if( (response = wdo_ch(crate, bd, -1, 1, NULL)) != TUTTOK) {
		  printf("get_chprop(2): wdo_ch: Error number %d received\n",response);
		  return ERROR;
		}
	      }
	  }
     
	  /* print */
	  for( ch=0 ; ch < 25 && ch < crates[crate].brds[bd].numch ; ch++ )
	    {
	      if(dbg) 
		{
		  printf(" %12s  ", crates[crate].bch[bd].set[ch].chname);	
		  printf
		    ("%07.2f  %07.2f  %07.2f  %07.2f  %07.2f  %07.2f  %4x     %2d \n",
		     crates[crate].bch[bd].read[ch].vread/scalev,
		     crates[crate].bch[bd].read[ch].iread/scalei,
		     crates[crate].bch[bd].set[ch].v0set/scalev,
		     crates[crate].bch[bd].set[ch].i0set/scalei,
		     crates[crate].bch[bd].set[ch].v1set/scalev,
		     crates[crate].bch[bd].set[ch].i1set/scalei,
		     crates[crate].bch[bd].set[ch].flag, ch);
		  
		  printf
		    ("%4d    %3d    %3d    %05.1f    %4x    %2d \n\n",
		     crates[crate].bch[bd].set[ch].vmax,
		     crates[crate].bch[bd].set[ch].rup,
		     crates[crate].bch[bd].set[ch].rdwn, 
		     crates[crate].bch[bd].set[ch].trip/10.0,
		     crates[crate].bch[bd].read[ch].status,ch);      
		  
		  printf(" I=%d (%x)  V=%d (%x)\n", 
			 crates[crate].bch[bd].read[ch].iread,
			 crates[crate].bch[bd].read[ch].iread,
			 (int) crates[crate].bch[bd].read[ch].vread,
			 (int) crates[crate].bch[bd].read[ch].vread );
		  printf(" Scale I=%f (%d), V=%f (%d)\n", 
			 scalei, crates[crate].brds[bd].deci,
			 scalev, crates[crate].brds[bd].decv);

		}

	      get_prop(tmp1, crate, bd, ch, prop);
	      strcat(stmp, tmp1);
	    }  
	} /* end if(chan<0) */    
      else 
	{ /* only ONE channel */
	  ch=chan;

	  
	  if(ch < crates[crate].brds[bd].numch)
	    {
	      if(0)
		{
	      if(prop==1 || prop==2 || prop==5 ||  prop==10 ) /* read ONE channel  Vmon, Imon, Status */
		{
		  
		  if( (response = wdo_ch(crate, bd, ch, 0, NULL)) != TUTTOK) {
		    printf("get_chprop(3): wdo_ch: Error number %d received\n",response);
		    return ERROR;
		  }
		
		  
		} 
	      else /* read ONE channel Settings*/ 
		{		  
		  if( (response = wdo_ch(crate, bd, ch, 1, NULL)) != TUTTOK) {
		    printf("get_chprop(4): wdo_ch: Error number %d received\n",response);
		    return ERROR;
		  }
	      
		}
		}
	    
	      /* end ONE  ch   */
	      /* print */	  
	      if(dbg) 
		{	
		  printf(" %12s  ", crates[crate].bch[bd].set[ch].chname);	
		  printf
		    ("%07.2f  %07.2f  %07.2f  %07.2f  %07.2f  %07.2f  %4x     %2d \n",
		     crates[crate].bch[bd].read[ch].vread/scalev,
		     crates[crate].bch[bd].read[ch].iread/scalei,
		     crates[crate].bch[bd].set[ch].v0set/scalev,
		     crates[crate].bch[bd].set[ch].i0set/scalei,
		     crates[crate].bch[bd].set[ch].v1set/scalev,
		     crates[crate].bch[bd].set[ch].i1set/scalei,
		     crates[crate].bch[bd].set[ch].flag, ch);
		  
		  printf
		    ("%4d    %3d    %3d    %05.1f    %4x    %2d \n\n",
		     crates[crate].bch[bd].set[ch].vmax,
		     crates[crate].bch[bd].set[ch].rup,
		     crates[crate].bch[bd].set[ch].rdwn, 
		     crates[crate].bch[bd].set[ch].trip/10.0,
		     crates[crate].bch[bd].read[ch].status,ch);      
		  
		  printf(" I=%d (%x)  V=%d (%x)\n", 
			 crates[crate].bch[bd].read[ch].iread,
			 crates[crate].bch[bd].read[ch].iread,
			 (int) crates[crate].bch[bd].read[ch].vread,
			 (int) crates[crate].bch[bd].read[ch].vread );
		  printf(" Scale I=%f (%d), V=%f (%d)\n", 
			 scalei, crates[crate].brds[bd].deci,
			 scalev, crates[crate].brds[bd].decv);
		}

	      get_prop(tmp1, crate, bd, ch, prop);
	      strcat(stmp, tmp1);
	    }
      
	  else /* ch > crates[craten].brds[bd].numch */
	    return ERROR; 
	}
  
    } /* end if(crates[craten]...) */
  else return ERROR;  

  if(dbg>1) printf("get_chprop(): crate=%d, board=%d chan=%d prop=%d val=%s \n", crate, bd, ch, prop, stmp);
  return 0;
}

/******** wrapper with semaphores for get_chprop() ********/ 
int get_wchprop(char *stmp, ushort cr_num, ushort bd, int chan, int prop) 
{
  int err;
  /** semaphore take **/
  craten=cr_num;
  err=get_chprop(stmp, cr_num, bd, chan, prop);
  /** semaphore give **/
  return err;
}

/****************************____________********************
*
* sets property value for ONE channel in slot
*
***********************************************************/
/** int set_one_chprop(char *stmp, ushort bd, int chan, int prop ) **/
int set_one_chprop(float fval, ushort cr_num, ushort bd, int chan, int prop ) 
{
  float
    input_value,
    scalev,
    scalei;
  ushort
    ch,channel,
    crt,
    cnet_buff[2],
    cepw=0,
    cod=16;
  int
    response;

  crt=cr_num;

  scalev=pow_10[crates[crt].brds[bd].decv];
  scalei=pow_10[crates[crt].brds[bd].deci];

  if(dbg>1)   printf("set_onr_chprop(); scaleV=%f, scaleI=%f\n",scalev, scalei); 

  ch=chan;

  /**  err=sscanf(stmp,"%f", &input_value);
  if(err<=0) 
    return ERROR;
  **/
  if(fval<0) fval=-1.*fval; /* do all positive */

  input_value = fval;

  channel = (bd<<8) | chan;
  /* Choice the value */ 
  cnet_buff[0] = channel;
  switch(prop)                          /* Decode the par. */
    {
    case V0SET:
      cod=16;
      input_value*=scalev;
      cnet_buff[1]=(ushort)input_value;
      break;
    case V1SET:
      cod=17;
      input_value*=scalev;
      cnet_buff[1]=(ushort)input_value;
      break;
    case I0SET:
      cod=18;
      input_value*=scalei;
      cnet_buff[1]=(ushort)input_value;
      break;
    case I1SET:
      cod=19;
      input_value*=scalei;
      cnet_buff[1]=(ushort)input_value;
      break;
    case SVMAX:
      cod=20;
      cnet_buff[1]=(ushort)input_value;
      break;
    case RUP:
      cod=21;
      cnet_buff[1]=(ushort)input_value;
      break;
    case RDWN:
      cod=22;
      cnet_buff[1]=(ushort)input_value;
      break;
    case TRIP:
      cod=23;
      input_value*=10; /* Trip is in 10-th of sec */
      cnet_buff[1]=(ushort)input_value;
      break;
    case FLAG:
      cod=24;
      cnet_buff[1]=(ushort)input_value;
      break;
    case PW: /* channel power ON/OFF */
      cod=24;
      cepw=(ushort)input_value;
      if(cepw==1) cnet_buff[1]=0x0808; /* 0CDA ON  */
      if(cepw==0) 
	{
	  cnet_buff[1]=0x0800; /*0x04D8;  OFF */
	  /* call alarm clear here */
	  if(0)
	  if( (response = wdo_ch( crt, bd, chan,  CLEAR_ALARM,  NULL) != TUTTOK))
	    {
	      printf("  clear alarm: Caenet_write: Error number %d received\n",response);
	      /*  return ERROR; */
	    }

	}

      break;
    }
  
  if(dbg>1)
    printf("to_caen_write: cnet_buff[0]= %x,  cnet_buff[1]= %x (%d)\n", 
	   cnet_buff[0], cnet_buff[1], cnet_buff[1]);

  /*  if((response=caenet_write2( crt, cod, (char *)cnet_buff,sizeof(cnet_buff))) != TUTTOK) */
  if( (response = wdo_ch( crt, bd, chan, cod,  (char *) cnet_buff)) != TUTTOK)
    {
      printf(" set_one_chprop(cr=%d bd=%d ch=%d pr=%d): wdo_ch: Error number %d received\n",
	     cr_num, bd, chan, prop, response);
      return ERROR;
    }
  return 0;
}

/**********************************************************
|
* sets property value for ONE or ALL channels in slot
|
**********************************************************/
int set_chprop(char *tmp, ushort crate, ushort bd, int chan, int prop )
{
  int ch=0,
    err=1;
  float fval;
  float fv[25];


  if( (bd<10) && ((crates[crate].config >> bd) &1) ) /* check if board in slot=bd is present */
   {
     if(chan<0) { /* set ALL channels */
       err=sscanf(tmp,
		  "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
		  &fv[0],&fv[1],&fv[2],&fv[3],&fv[4],&fv[5],&fv[6],&fv[7],&fv[8],&fv[9],
		  &fv[10],&fv[11],&fv[12],&fv[13],&fv[14],&fv[15],&fv[16],&fv[17],&fv[18],&fv[19],
		  &fv[20],&fv[21],&fv[22],&fv[23],&fv[24]
		  );
       if(dbg>0)  printf(" sscanf() returns = %0x (%0d)\n",err,err);
       if(err < crates[crate].brds[bd].numch) {
	 printf("set_chprop(1): ERROR sscanf=%d\n", err);
	 return ERROR;
       }
       
       for( ch=0 ; ch < 25 && ch < crates[crate].brds[bd].numch ; ch++ )
	 {  
	   /*  sscanf(tmp,"%s ", stmp); *//* replace !!! */

	   printf("set_chprop(cr=%d bd=%d ch=%d): SET prop(%0d) to %f\n", 
		  crate, bd, ch, prop, fv[ch]);
	   if( set_one_chprop(fv[ch], crate, bd, ch, prop ) == ERROR) {	     
	     printf("set_chprop(1): ERROR set_one_chprop \n");
	     return ERROR;    
	   }
	  
	   /* update_PS(crate, bd, prop,0); */ /* increment PS, LS, GS */
	 }
     }
     /*----------------------------*/
     else { /* set ONE channel */
       if(chan < crates[crate].brds[bd].numch)
	 {
	   err=sscanf(tmp,"%f", &fval);
	   /*	   if(err<=0) {
	     err=sscanf(tmp,"%x", &hval); 
	     printf("sscanf hex: %d(0x%x)\n",hval,hval);
	     fval=1.0*hval;
	   }
	   */	   
	   if(err<=0) {
	     printf("set_chprop(2): ERROR sscanf=%d \n",err);
	     return ERROR;
	   }
	   
	   printf("set_chprop(cr=%d bd=%d ch=%d): SET prop(%0d) to %f\n", 
		  crate, bd, chan, prop, fval);

	   if( set_one_chprop(fval, crate, bd, chan, prop ) == ERROR) {
	     printf("set_chprop(2): ERROR set_one_chprop \n");
	     return ERROR;    
	   }
	   /*  update_PS(crate, bd, prop, 0);*/ /* increment PS, LS, GS */

	 } else 
	 return ERROR;
     }
   }
  else { 
    printf("set_chprop(): ERROR crate=%d, board=%d is not present\n",crate, bd);
    return ERROR;
  }

  /* update summary numbers */
  update_GS(crate, -1);
 return 0;
}

/*****************************************
 * clear alarm(int crnum) 
 *********************************************/
/**
int clr_alarm(int crnum)
{
  if(wdo_ch(crnum,0,0, CLEAR_ALARM, NULL)==ERROR )
    {
      printf("clear_alarm(); wdo_ch get ERROR\n");
      return ERROR;
    }
  return 0;
}
**/

/*****************************************
 * kill all channels (int crnum) 
 *********************************************/
 /** 
int kill_all(int crnum)
{
  if(wdo_ch(crnum,0,0, KILL_CHANNELS_1, NULL)==ERROR) 
    {
      printf("kill_all(1); wdo_ch get ERROR\n");
      return ERROR;
   }
  if(wdo_ch(crnum,0,0, KILL_CHANNELS_2, NULL)==ERROR) 
    {
      printf("kill_all(2); wdo_ch get ERROR\n");
      return ERROR;      
    }

  return 0;  
}
**/
/********  MONITOR task *****************************
*
***************************************************/
int monitor(int num_crt, int crate1, int crate2, int crate3, int crate4) 
{
  int kk=0, ii=0; 
  int bdi=0; 
  int i=0;
  int response;
  static  int remap[15]; /* if>0 , need to call crate_map(crnum) after crate reconnected/powered */
  int err_cnt=0;

  /*  struct timespec nsTime;
  nsTime.tv_sec = 2;
  nsTime.tv_nsec = 0;
  nanosleep(&nsTime, NULL);
  */

  printf( "sysClkRate is %d\n", sysClkRateGet());



  bzero((char *) remap, sizeof(remap));
  clr_mem(); /* clear(set to 0) arrays and structures */

  /* crate semaphore */
  if(sem_init()==ERROR) {
    printf("monitor(): ERROR: init semaphore\n");
    return ERROR;
  }
  printf("monitor(): init semaphore done!\n");
  
  /*  init_sy527(0);*/ /* scan and init all crates connected to V288 by caenet */

  if(num_crt>0) {
    if(crate1>0) {
      init_sy527(crate1);
      crates_num[num_crate]=crate1;
      num_crate++;
    }
    if(crate2>0) {
      init_sy527(crate2);
      crates_num[num_crate]=crate2;
      num_crate++;
    }
    if(crate3>0) {
      init_sy527(crate3);
      crates_num[num_crate]=crate3;
      num_crate++;
    }
    if(crate4>0) {
      init_sy527(crate4);
      crates_num[num_crate]=crate4;
      num_crate++;
    }
  } else {
    init_sy527(0); /* scan and init all crates connected to V288 by caenet */
  }

  
  printf(" Crates list (%d): ", num_crate); 
  for(i=0;i<num_crate;i++) printf(" %0d", crates_num[i]);
  printf("\n  Crate mask= %0X \n", con_crates);

  /*  while(con_crates==0) {
    printf("monitor(): waiting for connected crates...\n");
    taskDelay(300);
  }
  */

  /*  if(con_crates>0) */
    { /* there is connected crates */
      for(;;) { /* start monitoring(readout) hv channes values by polling all of them */
	for(ii=0; ii<num_crate; ii++)
	  {
	    kk=crates_num[ii]; /* get crate number */
	    printf("crate=%d\n", kk);
	    /* check if crate connected */
	    if(read_ident(kk)==ERROR )	    
	      {
		update_GS(kk,2); /* increment configuration word in global summary number */
		remap[ii]=kk; /* set flag to init crate after reconnection */
		err_cnt=0;
	      }
	    else
	      {
		if(remap[ii]>0) {/* init crate */
		  remap[ii]=0;
		  taskDelay(20*sysClkRateGet());
		  bzero((char *)&crates[kk], sizeof(struct ccrate));
		  while (init_sy527(kk)==ERROR) {
		    taskDelay(sysClkRateGet());
		    if(10 < err_cnt++) 
		      break;
		  }
		    err_cnt=0;		   		    
		  
		  /*		  crate_map(kk);*/
		  update_GS(kk,2); /* increment configuration word in global summary number */
		}

	    
		if( (con_crates>>kk) &1) {
		  printf("\n CRATE=%d  BOARD=",kk);
		  for(bdi=0; bdi<10; bdi++)
		    {
		      
		      if( (crates[kk].config>>bdi) &1) {
			printf(" %0d ", bdi);
			
			if( (response = wdo_ch(kk, bdi, -1, 0, NULL)) != TUTTOK) { /* read ch status, Vmon, Imon */
			  err_cnt++;
			  printf("monitor(1): wdo_ch: Error number %d received (crate=%d board=%d err_cnt=%0d)\n",
				 response, kk, bdi, err_cnt);

			  /* return ERROR; */
			}		  
			taskDelay(6);
			if( (response = wdo_ch(kk, bdi, -1, 1, NULL)) != TUTTOK) { /* read ch settings */
			  err_cnt++;
			  printf("monitor(2): wdo_ch: Error number %d received (crate=%d board=%d err_cnt=%0d)\n",
				 response, kk, bdi, err_cnt);
			  if(err_cnt>=4) {
			    err_cnt=0;
			    break;
			  }
			  /* return ERROR; */
			}		  
			printf("\nmonitor(): wdo_ch: done: crate=%d board=%d \n", kk, bdi);		    
		      } 
		      else 
			printf("No board %d board_mask=%0X\n",bdi, crates[kk].config); /* if board present */
		      
		      taskDelay(mon_delay1);
		    } /* boards loop */
		} /* if crate is present */
	      } /* if(read_ident) else */
	    
	    taskDelay(18);
	  } /* looop by crates */
      } /* for(;;) endless loop */ 
    } 
  
  return 0; 
}

/*********************************** 
 * it is monitor for one crate
 *
 ************************************/
int monitor1( int crate1)
{
  int kk=0, ii=0; 
  int bdi=0; 
  int i=0;
  int response;
  static  int remap[15]; /* if>0 , need to call crate_map(crnum) after crate reconnected/powered */
  int hvstat=0;
  int crt=0;
  int err_cnt=0;
  int t_ID;

  /*  struct timespec nsTime;
  nsTime.tv_sec = 2;
  nsTime.tv_nsec = 0;
  nanosleep(&nsTime, NULL);
  */

  printf( "sysClkRate is %d\n", sysClkRateGet());
  t_ID=taskIdSelf();
  printf("monitor task ID=%d (%x)\n",t_ID, t_ID);

  bzero((char *) remap, sizeof(remap));
  clr_mem(); /* clear(set to 0) arrays and structures */

  /* crate semaphore */
  if(sem_init()==ERROR) {
    printf("monitor(%X): ERROR: init semaphore\n", t_ID);
    return ERROR;
  }
  printf("monitor(%X): init semaphore done!\n",t_ID);
  
  /*  init_sy527(0);*/ /* scan and init all crates connected to V288 by caenet */


  if(crate1>0) {
    init_sy527(crate1);
    crates_num[num_crate]=crate1;
    crt=crate1;
    num_crate++;
  }
  else {
    init_sy527(0); /* scan and init all crates connected to V288 by caenet */
  }

  
  printf(" Crates list (%d): ", num_crate); 
  for(i=0;i<num_crate;i++) printf(" %0d", crates_num[i]);
  printf("\n  Crate mask= %0X \n", con_crates);

  /*  while(con_crates==0) {
    printf("monitor(): waiting for connected crates...\n");
    taskDelay(300);
  }
  */

  taskDelay(10);
  /*  if(con_crates>0) */
    { /* there is connected crates */
      for(;;) { /* start monitoring(readout) hv channes values by polling all of them */
	for(ii=0; ii<num_crate; ii++) 
	  {	    
	    kk=crates_num[ii]; /* get crate number */
	    if(kk==crt)
	      {
		printf("(%X) crate=%d\n",t_ID , kk);
		/* check if crate connected */
		taskDelay(1);
		if(read_ident(kk)==ERROR )	    
		  {
		    update_GS(kk,2); /* increment configuration word in global summary number */
		    remap[ii]=kk; /* set flag to init crate after reconnection */
		    err_cnt=0;
		  }
		else
		  {
		    if(remap[ii]>0) {/* init crate */
		      remap[ii]=0;
		      taskDelay(20*sysClkRateGet());
		      bzero((char *)&crates[kk], sizeof(struct ccrate));
		      while (init_sy527(kk)==ERROR) {
			taskDelay(sysClkRateGet());
			if(10 < err_cnt++) 
			  break;
		      }
		      err_cnt=0;		   		    
		      
		      /*		  crate_map(kk);*/
		      update_GS(kk,2); /* increment configuration word 3 in global summary number */
		    }

		    /* check HV ON status */
		    hvstat=chck_hvstat(kk);
		    if(crates[kk].hvstatus!=hvstat) {
		      crates[kk].hvstatus=hvstat;
		      update_GS(kk,2); /* increment configuration word 3 in global summary number */
		    }

		    if( (con_crates>>kk) &1) {
		      printf("\n (%X) CRATE=%d  BOARD=", t_ID, kk);
		      for(bdi=0; bdi<10; bdi++)
			{
			  
			  if( (crates[kk].config>>bdi) &1) {
			    printf(" %0d ", bdi);
			    
			    if( (response = wdo_ch(kk, bdi, -1, 0, NULL)) != TUTTOK) { /* read ch status, Vmon, Imon */
			      err_cnt++;
			      printf("monitor(1): wdo_ch: Error number %d received (crate=%d board=%d err_cnt=%0d)\n",
				     response, kk, bdi, err_cnt);
			      
			      /* return ERROR; */
			    }		  
			    taskDelay(6);
			    if( (response = wdo_ch(kk, bdi, -1, 1, NULL)) != TUTTOK) { /* read ch settings */
			      err_cnt++;
			      printf("monitor(2): wdo_ch: Error number %d received (crate=%d board=%d err_cnt=%0d)\n",
				     response, kk, bdi, err_cnt);
			      if(err_cnt>=4) {
				err_cnt=0;
				break;
			      }
			  /* return ERROR; */
			    }		  
			    printf("\nmonitor(%X): wdo_ch: done: crate=%d board=%d \n", t_ID, kk, bdi);		    
			  } 
			  else 
			    printf("No board %d board_mask=%0X\n", bdi, crates[kk].config); /* if board present */
		      
			  taskDelay(mon_delay1);
			} /* boards loop */
		    } /* if crate is present */
		  } /* if(read_ident) else */
		
		taskDelay(18);
	      } /* if(kk==crate1) */

	  } /* looop by crates */
      } /* for(;;) endless loop */ 
    } 
  
  return 0; 
}
/* end monitor1() */
/******** END of My staff ***********************/


/*** The END ****/
