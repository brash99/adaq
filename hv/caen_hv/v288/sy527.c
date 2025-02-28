/***************************************************************************/
/*                                                                         */
/*   ------  C . A . E . N . SpA  ------                                   */
/*                                                                        */
/* VMESY527.C - Demonstration on the use of Caenet Routines in             */
/* communication between V288 module and SY527 Universal                   */
/* Multichannel Power Supply System Version 1.05                           */
/*                                                                         */
/* 03/17/93 - Created                                                      */
/* 12 Mar 2015 R.P. modified                                               */
/***************************************************************************/
#include <stdio.h> 
#include <ctype.h> 
#include <string.h>
#include <ioLib.h>
#include <tickLib.h>
#include <assert.h>    /* assert() */
#include <semLib.h>
#include <taskLib.h>
#include <vxWorks.h>
 
#include "vmcaenet.h"

#ifndef DBG_LEVEL
#define DBG_LEVEL 0
#endif

#define EUROCOM  0xfa000000

#define MAX_CRATES     15 /* max connected crates by caenet */

#define ONE_TICK       60 /* VxWorks ticks frequency  */
#define GS_TIME_SLICE  180*ONE_TICK /* sleep time (in s) for GS number update */
#define PS_TIME_SLICE  5*ONE_TICK /* sleep time (in s) for PS number update */

/* commands codes */
#define   IDENT             0
#define   READ_STATUS       1
#define   READ_SETTINGS     2
#define   READ_BOARD_INFO   3
#define   READ_CR_CONF      4
#define   READ_GEN_STATUS                          5
#define   READ_HVMAX                               6

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

int kk=0; /* for semaphore tests */

unsigned int v288addr=0xe0600000;
unsigned short craten=12;
ushort code=0; 

char sy527ident[12];       /* system ident */
ushort cr_conf;            /* crate config (each bit=1 in position repersents module in corresponded slot */
struct ccrate crates[MAX_CRATES];  /* crates parameters  */
struct board  boards[10];  /* modules parameters */
struct hvch   ch_set[25];  /* Settings of 25 chs.*/
struct hvrd   ch_read[25]; /* Status of 25 chs.  */


unsigned int con_crates; /* bit mask  of connected crates, filled during initialization init_sy527() */

float pow_10[]={ 1.0, 10.0, 100.0};

SEM_ID semMutex=NULL;  /* binary semaphore */

int set_dbg(int level) {
  if (level>=0) dbg=level;
  return dbg;
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
  /*  *addr=datovme;*/
  /*  printf("done\n"); */

  while(i!=TIMEOUT && q!=Q)
    {
      /*      if(!vme_write(vmeaddress,&datovme,WORD)) */
      if(!vmeWrite16(addr, datovme))
	return E_BUSERR;
      q=vmeRead16(stat);
      /*vme_read(STATUS,&q,WORD);*/
      i++; }
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
int caenet_read2(crt, cod, dest_buff,byte_count)
     ushort crt;
     ushort cod;
     uchar *dest_buff;
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
      if(read_data(&datatemp) == TIMEOUT && i<byte_count-1)
	return E_LESSDATA;
      dest_buff[i] = HIBYTE(datatemp);
      dest_buff[i+1] = LOBYTE(datatemp);
    }
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
  int i,esito;
  ushort mstident=V288,datatemp;
  short dato;

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
  int i;
  ushort datatemp;
  int ret;
  for(i=0;i<byte_count;i+=2)
    {
      ret=read_data(&datatemp);
      /*            printf("read_caen_buffer():%0X  byte_cnt=%d\n",ret, i); */
      if(ret == TIMEOUT && i<byte_count-1) 
	{
	  printf("... i=%d  byte_count=%d\n",i, byte_count);
	return E_LESSDATA;
      }
      dest_buff[i] = HIBYTE(datatemp);
      dest_buff[i+1] = LOBYTE(datatemp);

    }
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


/***------------------------------------------------------------------------
    Read_Ident
--------------------------------------------------------------------***/
int read_ident(int cr_indx)
{
  int i,response;
  /*  char sy527ident[12]; */
  char tempbuff[22];

  /* take semaphore */
  if(semMutex!=NULL) {
    semTake (semMutex, WAIT_FOREVER); 
  } else return ERROR;

  craten=cr_indx;
  code=IDENT;
  
  printf("read_ident(): addr=%x  crate=%d  code=%x \n",v288addr, craten, code);
  /* To see if sy527 is present */
  if((response=caenet_read(tempbuff,22)) != TUTTOK && response != E_LESSDATA)
    {
      printf(" read_ident(): Error number %d received\n",response);
      /*      puts(" Press any key to continue ");
	      getchar(); */
      sprintf(crates[cr_indx].ident,"Not present");
      crates[cr_indx].number=-1;
      crates[cr_indx].connected=0;

      /* give semaphore */
      semGive (semMutex);
      return ERROR; /* not present */
    }
  for(i=0;i<11;i++)
    sy527ident[i]=tempbuff[2*i+1];

  sy527ident[i]='\0';
  memcpy(crates[cr_indx].ident, sy527ident, sizeof(sy527ident));
  crates[cr_indx].number=craten;
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
int get_cr_info(cr_cnf)
     ushort *cr_cnf;
{
  int    i,response;
  ushort bd;

  code=READ_CR_CONF;

  if((response=caenet_read((char *)cr_cnf,sizeof(ushort))) != TUTTOK)
    {
      printf(" Caenet_read: Error number %d received\n",response);
      puts(" Press any key to continue ");
      /*getchar(); */
      return response;
    }
  code=READ_BOARD_INFO;
  for( bd=0, i=1 ; bd<10 ; bd++, i = i << 1 )
    if(*cr_cnf & i)
      {
	if((response=caenet_write((char *)&bd,sizeof(ushort))) != TUTTOK)
	  {
	    printf(" Caenet_write: Error number %d received\n",response);
	    puts(" Press any key to continue ");
	    /*  getchar(); */
	    return response;
	  }
	
	else
	  {
	    response=get_bd_info((struct board *)&boards[bd]);
	    if(response != TUTTOK)
	      {
		printf(" Read_Caenet_Buffer: Error number %d received\n",response);
		puts(" Press any key to continue ");
		/*		getchar(); */
		return response;
	      }
	  }
      }
  return TUTTOK;
}

/***------------------------------------------------------------------------
Crate_Map
--------------------------------------------------------------------***/
int crate_map()
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
/*  ushort cr_conf; */
 
 
 if(get_cr_info(&cr_conf) != TUTTOK) /* Get information about the Crate Configuration */
   return ERROR;
 
 /* clrscr();*/
 printf(" cr_conf=%x ", cr_conf);
 puts("\n\n --- Crate Map --- \n\n\n\n\n ");
 for( bd=0, i=1 ; bd<10 ; bd++, i = i << 1 )
   {     
     printf(" Slot %d - ",bd);
     crates[craten].config=cr_conf;
     if(cr_conf & i)
       {

	 /*crates[craten].brds[bd].name = boards[bd].name ;*/ /* wrong!!! */

	 memcpy(crates[craten].brds[bd].name, boards[bd].name, sizeof( boards[bd].name));
	 crates[craten].brds[bd].numch = boards[bd].numch ;
	 crates[craten].brds[bd].vermaior = boards[bd].vermaior;
	 crates[craten].brds[bd].verminor = boards[bd].verminor;
	 crates[craten].brds[bd].vmax = boards[bd].vmax;
	 crates[craten].brds[bd].imax = boards[bd].imax;
	 crates[craten].brds[bd].rmin = boards[bd].rmin;
	 crates[craten].brds[bd].rmax = boards[bd].rmax;
	 crates[craten].brds[bd].resv = boards[bd].resv;
	 crates[craten].brds[bd].resi = boards[bd].resi;
	 crates[craten].brds[bd].decv = boards[bd].decv;
	 crates[craten].brds[bd].deci = boards[bd].deci;
	 crates[craten].brds[bd].curr_mis = boards[bd].curr_mis;
	 crates[craten].brds[bd].sernum = boards[bd].sernum;
	 
	 /*memcpy(crates[craten].brds, boards, sizeof(boards));*/
	 crates[craten].ps[bd].prop_num = PROP_NUM;

	 printf(" Mod. %s %3d CH ",boards[bd].name,boards[bd].numch);
	 printf(" %4dV",boards[bd].vmax);
	 im = (float)boards[bd].imax/pow_10[boards[bd].deci];
	 printf(" %8.2f",im);
	 printf("%s",curr_umis[boards[bd].curr_mis]);
	 printf(" %3d_%3dV", boards[bd].rmin, boards[bd].rmax);

	 printf(" --- Ser. %3d, Rel. %d.%02d\n",
		boards[bd].sernum,boards[bd].vermaior,boards[bd].verminor);

	 bcnt++;
       }

     else
       printf(" Not Present \n");     
   }
 crates[craten].bd_num = bcnt; /* store number of inserted boards */

 puts("\n\n");
 /* puts("\n\n\n Press any key to continue ");
 getchar(); */
 return 0;
}


/***------------------------------------------------------------------------
Ch_monitor
--------------------------------------------------------------------***/
void ch_monitor()
{
  int
    temp,
    caratt='P',
    response,
    cnt_u=0;
  float    scalei,scalev;
  ushort    bd,ch,ch_addr;
  static int    page=0;
  
  do
    {
      /* clrscr();*/
      printf(" Input Board Number [0 ... 9]: ");
      scanf("%d",&temp);
    }
  while(temp < 0 || temp > 9);
  bd = temp;
  code=READ_BOARD_INFO;
  if((response=caenet_write((char *)&bd,sizeof(ushort))) != TUTTOK)
    {
      printf(" Caenet_write: Error number %d received\n",response);
      puts(" Press any key to continue ");
      getchar();
      return;
    }
  else
    {
      response=get_bd_info((struct board *)&boards[bd]);
      if(response != TUTTOK)
	{
	  printf(" Read_Caenet_Buffer: Error number %d received\n",response);
	  puts(" Press any key to continue ");
	  getchar();
	  return;
	}
    }
  scalev=pow_10[boards[bd].decv];
  
  scalei=pow_10[boards[bd].deci];
  /* highvideo();*/
  if(!page)
    puts
      ("\n Channel    Vmon    Imon    V0set    I0set    V1set    I1set   Flag   Ch# ");
  else
    puts
      ("\n Channel    Vmax    Rup   Rdwn   Trip   Status    Flag  Ch# ");
  /*  normvideo(); */
  /*  gotoxy(1,23); */
  puts(" Press 'P' to change page, any other key to exit ");

  while(caratt == 'P') /* Loops until someone presses a key different from P */
    {
      printf(" -----> LOOP %0d <-----\n", cnt_u);
      /* First update from Caenet the information about the channels */
      for( ch=0 ; ch < 25 && ch < boards[bd].numch ; ch++ )
	{
	  code = READ_STATUS;
	  ch_addr = (bd<<8) | ch;
	  if((response=caenet_write((char *)&ch_addr,sizeof(ushort))) != TUTTOK)
	    {
	      printf(" Caenet_write: Error number %d received\n",response);
	      puts(" Press any key to continue ");
	      getchar();
	      return;
	    }
	  else
	    {
	      response=read_caenet_buffer((char *)&ch_read[ch],sizeof(struct hvrd));
	      if(response != TUTTOK)
		{
		  printf(" Read_Caenet_Buffer: Error number %d received\n",response);
		  puts(" Press any key to continue ");
		  getchar();
		  return;
		}
	    }
	  code = READ_SETTINGS;
	  if((response=caenet_write((char *)&ch_addr,sizeof(ushort))) != TUTTOK)
	    {
	      printf(" Caenet_write: Error number %d received\n",response);
	      puts(" Press any key to continue ");
	      getchar();
	      return;
	    }
	  else
	    {
	      response=read_caenet_buffer((char *)&ch_set[ch],sizeof(struct hvch));
	      if(response != TUTTOK)
		{
		  
		  printf(" Read_Caenet_Buffer: Error number %d received\n",response);
		  puts(" Press any key to continue ");
		  getchar();
		  return;
		}
	    }
	}
      /* end for( ch ) */
      if(!page)
	/* Page 0 of display */
	for( ch=0 ; ch < 25 && ch < boards[bd].numch ; ch++ )
	  {
	    /*	    gotoxy(1,ch+5); */
	    /* printf("<%9s>\n",ch_set[ch].chname); 
	    printf(">%07.2f<\n", ch_read[ch].vread/scalev);
	    */
	    printf(" %12s  ",ch_set[ch].chname);

	    /*	    gotoxy(12,ch+5); */
	    printf
	      ("%07.2f  %07.2f  %07.2f  %07.2f  %07.2f  %07.2f  %4x     %2d \n",
	       ch_read[ch].vread/scalev,ch_read[ch].iread/scalei,ch_set[ch].v0set/scalev,
	       ch_set[ch].i0set/scalei,ch_set[ch].v1set/scalev,ch_set[ch].i1set/scalei,
	       ch_set[ch].flag,ch);
	  }
      else
	/* Page 1 of display */
	for( ch=0 ; ch < 25 && ch < boards[bd].numch ; ch++ )
	  {
	    /*	    gotoxy(1,ch+5); */
	    printf(" %12s  ",ch_set[ch].chname);
	    /*	    gotoxy(14,ch+5); */
	    printf
	      ("%4d    %3d    %3d    %05.1f    %4x    %2d \n",
	       ch_set[ch].vmax,ch_set[ch].rup,ch_set[ch].rdwn,ch_set[ch].trip/10.0,
	       ch_read[ch].status,ch);
	  }
      if(cnt_u++ == 5)   caratt='E'; /** R.P. **/
      /* Test the keyboard */
      if(0)
	/*      if(_gs_rdy(0) != -1) */
	/* A key has been pressed */
	
	if((caratt=toupper(getchar())) == 'P') /* They want to change page */
	  {
	    page = !page;
	    /* clrscr(); */
	    printf(" Input Board Number [0 ... 9]: %d\n",bd);
	    if(page == 0)
	      puts ("\n Channel    Vmon    Imon    V0set    I0set    V1set    I1set   Flag   Ch# ");
	    
	    else
	      puts  ("\n Channel    Vmax    Rup   Rdwn   Trip   Status     Ch# ");
	    
	    /*    gotoxy(1,23); */
	    puts(" Press 'P' to change page, any other key to exit ");
	  }
    }
  /* End while */
}

/***-----------------------
    Par_set
--------------------------------------------------------------------***/
void par_set()
{
  float
    input_value,
    scale;
  ushort
    bd,ch,channel,
    cnet_buff[2];
  int
    i,temp,
    response,
    par=0;
  char
    choiced_param[10];
  static char *param[] =
    {
      "v0set", "v1set", "i0set", "i1set", "vmax", "rup", "rdwn", "trip"
    };

  /*  clrscr(); */
  printf("\n\n Board: ");
  /* Choice the board
   */
  scanf("%d",&temp);
  bd = temp;
  code=READ_BOARD_INFO;
  if((response=caenet_write((char *)&bd,sizeof(ushort))) != TUTTOK)
    {
      printf(" Caenet_write: Error number %d received\n",response);
      puts(" Press any key to continue ");
      getchar();
      return;
    }
  else
    {
      response=get_bd_info((struct board *)&boards[bd]);
      if(response != TUTTOK)
	{
	  printf(" Read_Caenet_Buffer: Error number %d received\n",response);
	  puts(" Press any key to continue ");
	  getchar();
	  return;
	}
    }
  
  printf("\n\n Channel: ");    /* Choice the channel */
  scanf("%d",&temp);
  ch = temp;
  channel = (bd<<8) | ch;
  puts(" Allowed parameters (lowercase only) are:");
  for(i=0;i<8;i++)
    puts(param[i]);
  while(!par)
    {
      printf("\n Parameter to set: ");   /* Choice the parameter */
      scanf("%s",choiced_param);
      for(i=0;i<8;i++)
   
	if(!strcmp(param[i],choiced_param))
	  {
	    par=1;
	    break;
	  }
      if(i==8)
	puts(" Sorry, this parameter is not allowed");
    }
  printf(" New value :");
  scanf("%f",&input_value);
  /* Choice the value */ 
  cnet_buff[0] = channel;
  switch(i)                          /* Decode the par. */
    
    {
    case V0SET:
      code=16;
      scale=pow_10[boards[bd].decv];
      input_value*=scale;
      cnet_buff[1]=(ushort)input_value;
      break;
    case V1SET:
      code=17;
      scale=pow_10[boards[bd].decv];
      input_value*=scale;
      cnet_buff[1]=(ushort)input_value;
      break;
    case I0SET:
      code=18;
      scale=pow_10[boards[bd].deci];
      input_value*=scale;
      cnet_buff[1]=(ushort)input_value;
      break;
    case I1SET:
      code=19;
      scale=pow_10[boards[bd].deci];
      input_value*=scale;
      cnet_buff[1]=(ushort)input_value;
      break;
    case SVMAX:
      code=20;
      cnet_buff[1]=(ushort)input_value;
      break;
    case RUP:
      code=21;
      cnet_buff[1]=(ushort)input_value;
      break;
    case RDWN:
      code=22;
      cnet_buff[1]=(ushort)input_value;
      break;
    case TRIP:
      code=23;
      input_value*=10; /* Trip is in 10-th of sec */
      cnet_buff[1]=(ushort)input_value;
      break;
    case FLAG:
      code=24;
      cnet_buff[1]=(ushort)input_value;
      break;
    }

  if((response=caenet_write((char *)cnet_buff,sizeof(cnet_buff))) != TUTTOK)
    {
      printf(" Caenet_write: Error number %d received\n",response);
      puts(" Press any key to continue ");
      getchar();
    }
}

/***------------------------------------------------------------------------
    Speed_test
--------------------------------------------------------------------***/
void speed_test()
{
  int i,response, cnt_u;
  char sy527ident[12],loopdata[12];
  char tempbuff[22];
  code=IDENT;  /* To see if sy527 is present */

  if((response=caenet_read(tempbuff,22)) != TUTTOK && response != E_LESSDATA)
    {
      printf(" Caenet_read: Error number %d received\n",response);
      puts(" Press any key to continue ");
      getchar();
      return;
    }
  for(i=0;i<11;i++)
    sy527ident[i]=tempbuff[2*i+1];

  sy527ident[i]='\0';
  puts(" Looping, press any key to exit ... ");
  /* Loop until one presses a key */

  cnt_u=0; /** R.P. **/
  while(cnt_u<10)
    /*  while(_gs_rdy(0) == -1) */
    {
      if((response=caenet_read(tempbuff,22)) != TUTTOK && response != E_LESSDATA)
	{
	  printf(" Caenet_read: Error number %d received\n",response);
	  puts(" Press any key to continue ");
	  getchar();
	  return;
	}
      for(i=0;i<11;i++)
	loopdata[i]=tempbuff[2*i+1];
      loopdata[i]='\0';
      if(strcmp(sy527ident,loopdata)) /* Data read in loop are not good */
	{
	  printf(" Test_loop error: String read = %s\n",loopdata);
	  puts(" Press any key to continue ");
	  getchar();
	  return;
	  
	}
      cnt_u++;
      printf("----> LOOP %0d <-----\n",cnt_u);
    } /* end while */
  printf(" Test DONE\n");
 getchar();
}


/** hit keyboard **/
int kbHit(int flush)
{
  int inputFd;
  int options, rc, nread=-1;

  inputFd = ioGlobalStdGet(0);
  /* Save options */
  options = ioctl (0, FIOGETOPTIONS, 0);
  /* Enter raw mode */
  ioctl (0, FIOSETOPTIONS, OPT_TERMINAL & ~OPT_LINE & ~OPT_ECHO);
  
  /*ioctl (inputFd, FIOSETOPTIONS, OPT_RAW);*/
  /*  for (i=0; i<50; i++)
      {
      printf("%d ",i); */
  
  /* probe STDIN to see if operator entered any characters */
  rc = ioctl(0, FIONREAD, (int) &nread);
  assert(rc != ERROR);
  /*if (nread)*/
    /*ioctl (0, FIONREAD, (int) &bytesRead);
      if (bytesRead > 0) */
    {
      /*	  printf("----->Key Pressed!\n"); */
      if(flush)	  ioctl (0, FIOFLUSH, 0); 
      /*	  break; */
    }
  taskDelay(10);
  /*    } */
 /* Leave raw mode */
  ioctl (0, FIOSETOPTIONS, options);
 /*  ioctl (inputFd, FIOSETOPTIONS, OPT_TERMINAL);*/
  return nread;
}

/*******________* My staff *______________******************************/
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
 *         and crates[craten].ps[bd].prop_num
 *---------*************************************/ 
int get_nprop(int bd) 
{
  return crates[craten].ps[bd].prop_num;
}

/*---------******************************
 * summary numbers update routinews
 *---------******************************/
int update_GS(int flag) 
{
  int ii=0;
  if(flag<0) { /* update all GS */
    for(ii=0;ii<5;ii++) crates[craten].gsn[ii]++;
  } else crates[craten].gsn[flag]++;  
  return 0;
}

/*** get_GS ****/
int get_GS(unsigned short cr_num, char *gss) 
{
  ULONG c_time;
  int i, kk, jj;
  
  /*  set_crnum(cr_num);  set craten = cr_num */
  craten=cr_num;
  c_time = tickGet();

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
	      update_GS(2); /* configuration summary number */
	    }
	}
      else 	  /* check if crate connected again */
	{
	if(read_ident(cr_num)==0) 
	  {
	    /* crate connected */
	    printf("update_GS(2): crate# %d connected or powered ON\n",cr_num);
	    update_GS(2); /* configuration summary number */
	    for( jj=0 ; jj<PROP_NUM ; jj++ )
	      {     
		update_PS(-1, jj, 0);  /* update ALL properties summary number*/
	      }
	  } 
	else 	     
	  printf("update_GS(3);  crate# %d diconnected or power OFF\n",cr_num);
	}     
    }

  update_GS(3); /* update(incremetn) host activitty number */
  update_GS(4); /* update(incremetn) host activitty number */
  
  sprintf(gss, "%4X %4x %4X %4x %4x", crates[craten].gsn[0], crates[craten].gsn[1], 
	  crates[craten].gsn[2], crates[craten].gsn[3], crates[craten].gsn[4]);
  
  return 0;
}

/**** LS ****/
int update_LS(int bd, int meas) 
{
  int i, kk ;
 
  if(bd<0) /* for All boards */
    {
      for( kk=0, i=1 ; kk<10 ; kk++, i = i << 1 )
	{     
	  if(crates[craten].config & i)
	    {
	      crates[craten].lsn[2*kk]++ ;
	      crates[craten].lsn[2*kk+1]++ ;	      
	    }
	}
      update_GS(0); /* measured */
      update_GS(1); /* settable */    
    } 
  else /* for ONE board */
    {
      if(meas==1) 
	{
	  crates[craten].lsn[2*bd]++;   /* measurable LS */
	  update_GS(0); /* measured */
	}
      else 
	{
	  crates[craten].lsn[2*bd+1]++; /* settable LS */
	  update_GS(1); /* settable */  
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
int update_PS(int bd, int prop, int meas) 
{
  int kk, i;

  if( (bd<10) && ((crates[craten].config >> bd ) &1) ) /* check if board in slot=bd is present */ 
    {    
      crates[craten].ps[bd].psn[prop]++;
      if(meas==1)
	update_LS(bd, meas); 
      else {
	update_LS(bd, meas); 
	update_LS(bd+1, meas);
      }
    }
  /* for All boards */  
  if(bd<0) 
    {
      for( kk=0, i=1 ; kk<10 ; kk++, i = i << 1 )
	{     
	  if(crates[craten].config & i)
	    {
	      crates[craten].ps[kk].psn[prop]++;
	      update_LS(2*kk, meas); 
	      update_LS(2*kk+1, meas);
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

/* ***********************************************************************
 * init_sy527(int crt) - initialize crates (struct crates[], borads[],..)
 * Params: int crt - crate number, if crt=0, then check all crates.
 * Returns: if crt=0, number of connected crates; if crt=crate#,
 *          OK if crate connected and ERROR if crate with crate# not connected
 ************************************************************************/
int init_sy527(int crt) 
{
  int err=0;
  char *to_print="0";
  char *stemp="0";
  int cnt=0;

  bzero((char *) crates, sizeof(crates));
  bzero((char *) boards, sizeof(boards));
  bzero((char *) sy527ident, sizeof(sy527ident));
  bzero((char *) ch_set,sizeof(ch_set));
  bzero((char *) ch_read,sizeof(ch_read));
  
  con_crates=0;
  
  if(crt==0) /* check connection to all crates 1-15 */
    {
      for(kk=0; kk<MAX_CRATES; kk++)
	{
	  craten=kk+1;
	  err=read_ident(kk);
	  if(err==ERROR) {
	    printf("\ninit_sy527(): Crate %d NOT present/connected!\n", craten);
	  } else {
	    con_crates |= (1<<kk) ;		
	    printf("\nCrate %d connected!\n", craten);
	    sprintf(stemp," %0d", craten);
	    strcat(to_print, stemp);
	    /* get crate map */
	    if(crate_map()) {
	      cnt++;
	    } else {
	      printf("\n init_sy527(): crate_map(): ERROR\n");
	    }
	  }
	}
      printf("\n\nCrate mask %08X : connected crates: %s \n", con_crates, to_print);      
    }

  else if(crt<=MAX_CRATES) /* init one crate */
    {
      craten=crt;
      err=read_ident(craten);
      if(err==ERROR) {
	printf("init_sy527(): Crate %d NOT present/connected!\n", craten);
	return ERROR;
      } else {
	printf("Crate# %d connected!\n", craten);		
	if((err=crate_map())==0) {
	  printf("Crate# %d initialized!\n", craten);		
	  return OK;
	} else {
	  printf("\n init_sy527(): crate_map(): ERROR\n");
	  return ERROR;
	}
      }
    }

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

/*** for semaphore testing **/
int sem_crate() {
/* Create a binary semaphore that is initially full. Tasks * 
* blocked on semaphore wait in priority order.            */ 
         
  if(semMutex==NULL) {
    if(semMutex = semBCreate (SEM_Q_PRIORITY, SEM_FULL)) {
      return 0;
    }
    else 
      return ERROR;
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

/*****************************************************
| 
* prop_to_indx() - returns index of property in 'tmp' 
*                  or ERROR if property not found.
|
******************************************************/
int prop_to_indx(char *tmp) 
{
  if(strncmp(tmp,"DV",2) == 0)   /** DV property **/           
    return V0SET;
  else if(strncmp(tmp,"DC",2) == 0)  /** DC property **/
    return I0SET;
  else if(strncmp(tmp,"CE",2) == 0)  /** CE property **/      
    return PW;  
  else if(strncmp(tmp,"RUP",3) == 0)  /** RUP property **/            
    return RUP;
  else if(strncmp(tmp,"RDN",3) == 0)  /** RDN property **/            
    return RDWN;  
  else if(strncmp(tmp,"STAT",4) == 0)  /** STAT property **/      
    return STAT;  
  else if(strncmp(tmp,"ST",2) == 0)  /** ST property **/      
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
  else if(strncmp(tmp,"MV",2) == 0)  /** MV property **/     
    return VMON;  
  else if(strncmp(tmp,"MC",2) == 0)  /** MC property **/      
    return IMON;
  else if(strncmp(tmp,"FLAG",4) == 0)  /** FLAG property **/      
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
  case SVMAX: sprintf(tmp1," %4d", ch_set[ch].vmax);
    break;
  case RUP:   sprintf(tmp1," %3d", ch_set[ch].rup);
    break;
  case RDWN:  sprintf(tmp1," %3d", ch_set[ch].rdwn);
    break;
  case TRIP:  sprintf(tmp1," %5.1f", ch_set[ch].trip/10.0);
    break;
  case VMON:  sprintf(tmp1," %7.1f", ch_read[ch].vread/scalev);
    break;
  case IMON:  sprintf(tmp1," %7.1f", ch_read[ch].iread/scalei);
    break;
  case FLAG:  sprintf(tmp1," %04X",  ch_set[ch].flag );
    break;
  case STAT:  sprintf(tmp1," %04X", ch_read[ch].status);
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
  
  scalev = pow_10[crates[craten].brds[bd].decv];
  scalei = pow_10[crates[craten].brds[bd].deci];

  switch(prop) {
  case V0SET: sprintf(tmp1," %7.1f", crates[crt].bch[bd].set[ch].v0set/scalev);
    break;
  case V1SET: sprintf(tmp1," %7.1f", crates[crt].bch[bd].set[ch].v1set/scalev);
    break;
  case I0SET: sprintf(tmp1," %7.1f", crates[crt].bch[bd].set[ch].i0set/scalei);
    break;
  case I1SET: sprintf(tmp1," %7.1f", crates[crt].bch[bd].set[ch].i1set/scalei);
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
  case IMON:  sprintf(tmp1," %7.1f", crates[crt].bch[bd].read[ch].iread/scalei);
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

  return 0;
}
/****************************************************
 * do_ch - if set<3 - reads channel values (status and settings) 
 *           to structure ch_read[channel] and ch_set[].
 *           if set >3 set channel value thatstored in buff
 *----------------------------------------------------*/
int do_ch(int cr_num, int board, int chan, int set, char *buff) 
{
  ushort com_code;
  ushort crt;
  int response; 
  ushort ch_addr;

  crt=cr_num;
  ch_addr = (board<<8) | chan;

  if((set==0) || (set==2)) /* read channel Vmon, Imon, status */
    {
      com_code = READ_STATUS;	      
      if((response=caenet_write2(crt, com_code, (char *)&ch_addr, sizeof(ushort))) != TUTTOK)
	{
	  printf("read_ch(1): Caenet_write: Error number %d received\n",response);
	  /*puts(" Press any key to continue ");
	    getchar(); */
	  return ERROR;
	}
      else
	{
	  /* response=read_caenet_buffer((char *)&ch_read[ch],sizeof(struct hvrd)); */
	  response=read_caenet_buffer((char *)&ch_read[chan], size_hvrd());
	  if(response != TUTTOK)
	    {
	      printf("read_ch(2): Read_Caenet_Buffer: Error number %d received\n",response);
	      return ERROR;
	    }
	  /* copy ch_read to crates[] */
	  memcpy( &(crates[crt].bch[board].read[chan]), &ch_read[chan], size_hvrd());
	}
    }
  if((set==1) || (set==2))
    { /* read settings */	
      com_code = READ_SETTINGS;
      if((response=caenet_write2(crt, com_code,  (char *)&ch_addr,sizeof(ushort))) != TUTTOK)
	{
	  printf("read_ch(3): Caenet_write: Error number %d received\n", response);
	  return ERROR;
	}
      else
	{
	  /*	  response=read_caenet_buffer((char *)&ch_set[chan], sizeof(struct hvch)); */
	  response=read_caenet_buffer((char *)&ch_set[chan], size_hvch());	    
	  if(response != TUTTOK)
	    {	      
	      printf("read_ch(4): Read_Caenet_Buffer: Error number %d received\n",response);
	      return ERROR;
	    }
	  /* copy ch_set to crates[] */
	  memcpy( &(crates[crt].bch[board].set[chan]), &ch_set[chan], size_hvch());
	}
    }
    else
      {   /* set - is code for channel settings */
	com_code=set;
	if((response=caenet_write2(crt, com_code, buff, sizeof(buff))) != TUTTOK)
	  {
	    printf("read_ch(5): Caenet_write2: Error number %d received\n",response);
	    return ERROR;
	  }        
      }
  return TUTTOK; /* successful */
}

/***********************************************
*** wrapper with semaphore for do_ch() *****
**********************************************/ 
int wdo_ch(int cr_num, int board, int chan, int set, char *buff) 
{
  int err;

  if(semMutex != NULL) {
    /** semaphore take **/
    semTake (semMutex, WAIT_FOREVER); 
    
    err=do_ch(cr_num, board, chan, set, buff);
    
  /** semaphore give **/
    semGive (semMutex);
  } else return ERROR; 

  return err;
}

/************************************************************** 
 * get_chprop() - get all channels values for property 'prop'
 * Params: *tmp - output string value(s) of property 'prop',
 *         bd   - board slot number;
 *         chan - channel number, if chan<0, then readout all channels;
 *         prop - property index (in #define)
 * Returns: 0 - if OK; -1 - if any error.
 ****************************************** *******************/
int get_chprop(char *stmp, ushort bd, int chan, int prop) {
  int
    response;

  float    scalei,scalev;
  ushort   ch, ch_addr;
  char tmp1[20];
  
  bzero((char *) tmp1, sizeof(tmp1));
  
  if( (bd<10) && ((crates[craten].config >> bd ) &1) ) /* check if board in slot=bd is present */
 {
  scalev=pow_10[crates[craten].brds[bd].decv];
  scalei=pow_10[crates[craten].brds[bd].deci];

  if( chan<0) /* scan ALL channels */
    {
  /* First update from Caenet the information about the channels */
      for( ch=0 ; ch < 25 && ch <crates[craten].brds[bd].numch ; ch++ ) 
	{
	  ch_addr = (bd<<8) | ch;
	  if(prop==1 || prop==2 || prop==5 ||  prop==10 ) /* read  Vmon, Imon, Status */
	    {
	      if( (response = wdo_ch(craten, bd, ch, 0, NULL)) != TUTTOK) {
		printf("get_chprop(1): wdo_ch: Error number %d received\n",response);
		return ERROR;
	      }
	       

	      if(0) /*---------------*/
		{ code = READ_STATUS;	      
		  if((response=caenet_write((char *)&ch_addr,sizeof(ushort))) != TUTTOK)
		    {
		      printf("get_chprop(1): Caenet_write: Error number %d received\n",response);
		      /*puts(" Press any key to continue ");
			getchar(); */
		      return ERROR;
		    }
		  else
		    {
		      /* response=read_caenet_buffer((char *)&ch_read[ch],sizeof(struct hvrd)); */
		      response=read_caenet_buffer((char *)&ch_read[ch], size_hvrd());
		      if(response != TUTTOK)
			{
			  printf("get_chprop(1): Read_Caenet_Buffer: Error number %d received\n",response);
			  /* puts(" Press any key to continue ");
			     getchar(); */
			  return ERROR;
			}
		    }
		}/*---------------*/

	    } /* if(prop==...) */
	  
	  else   /* read  channel Settings*/  
	    {
	      
	      if( (response = wdo_ch(craten, bd, ch, 1, NULL)) != TUTTOK) {
		printf("get_chprop(1): wdo_ch: Error number %d received\n",response);
		return ERROR;
	      }

	      if(0) /*-------------------*/
		{
		  code = READ_SETTINGS;
		  if((response=caenet_write((char *)&ch_addr,sizeof(ushort))) != TUTTOK)
		    {
		      printf("get_chprop(2): Caenet_write: Error number %d received\n",response);
		      return ERROR;
		    }
		  else
		    {
		      response=read_caenet_buffer((char *)&ch_set[ch],sizeof(struct hvch));
		      if(response != TUTTOK)
			{			  
			  printf("get_chprop(2): Read_Caenet_Buffer: Error number %d received\n",response);
			  /* puts(" Press any key to continue ");
			     getchar(); */
			  return ERROR;
			}
		    }
		  /*	  taskDelay(1);*/
		} /*--------------*/
	    }
	} 
      /* end for( ch ) */
      /* print */
      for( ch=0 ; ch < 25 && ch < crates[craten].brds[bd].numch ; ch++ )
	{
	  
	  if(dbg) 
	    {
	      printf(" %12s  ",ch_set[ch].chname);
	      
	      printf
		("%07.2f  %07.2f  %07.2f  %07.2f  %07.2f  %07.2f  %4x     %2d \n",
		 ch_read[ch].vread/scalev,ch_read[ch].iread/scalei,ch_set[ch].v0set/scalev,
		 ch_set[ch].i0set/scalei,ch_set[ch].v1set/scalev,ch_set[ch].i1set/scalei,
		 ch_set[ch].flag,ch);
		
	      printf
		("%4d    %3d    %3d    %05.1f    %4x    %2d \n\n",
		 ch_set[ch].vmax,ch_set[ch].rup,ch_set[ch].rdwn,ch_set[ch].trip/10.0,
		 ch_read[ch].status,ch);      
	      
	      printf(" I=%d (%x)  V=%d (%x)\n", ch_read[ch].iread,ch_read[ch].iread,
		     ch_read[ch].vread, ch_read[ch].vread );
	      printf(" Scale I=%f (%d), V=%f (%d)\n", scalei, crates[craten].brds[bd].deci, 
		     scalev, crates[craten].brds[bd].decv);
	    }

	  get_prop(tmp1, craten, bd, ch, prop);
	  strcat(stmp, tmp1);
	}  
    } /* end if(chan<0) */

  else 
    { /* only ONE channel */
    ch=chan;
    if(ch < crates[craten].brds[bd].numch)
    {

      /* First update from Caenet the information about the ONE channel */
      /*  for( ch=0 ; ch < 25 && ch <crates[craten].brds[bd].numch ; ch++ ) */
      {
	ch_addr = (bd<<8) | ch;

	if(prop==1 || prop==2 || prop==5 ||  prop==10 ) /* read  Vmon, Imon, Status */
	  {
	    code = READ_STATUS;
	    if((response=caenet_write((char *)&ch_addr,sizeof(ushort))) != TUTTOK)
	      {
		printf("get_chprop(3): Caenet_write: Error number %d received\n",response);
		/*puts(" Press any key to continue ");
		  getchar(); 
		*/
		return ERROR;
	      }
	  
	    else
	      {
		/* response=read_caenet_buffer((char *)&ch_read[ch], sizeof(struct hvrd)); */
		/*	response=read_caenet_buffer((char *)&ch_read[ch], sizeof(ch_read[0])); **/
		response=read_caenet_buffer((char *)&ch_read[ch], size_hvrd());
		if(response != TUTTOK)
		  {		    
		    printf("ERROR: get_chprop(3): Read_Caenet_Buffer: Error number %d received\n",response);
		    /*		    printf("------> sizeof(struct hvrd)=%0x (%0d) \n",sizeof(ch_read[0]), sizeof(struct hvrd) );
		    printf("------> size_hvrd=%0x (%0d) \n",size_hvrd(), size_hvrd() );
		    */
		    return ERROR;
		  }
	      }
	  } 
	else 
	  {
	    code = READ_SETTINGS;
	    if((response=caenet_write((char *)&ch_addr,sizeof(ushort))) != TUTTOK)
	      {
		printf("get_chprop(4): Caenet_write: Error number %d received\n",response);
		return ERROR;
	      }
	    else
	      {
		response=read_caenet_buffer((char *)&ch_set[ch],sizeof(struct hvch));
		if(response != TUTTOK)
		  {
		    
		    printf("get_chprop(4): Read_Caenet_Buffer: Error number %d received\n",response);
		    return ERROR;
		  }
	      }
	  }
      }
      /* end for( ch ) */
      /* print */
      /*      for( ch=0 ; ch < 25 && ch < crates[craten].brds[bd].numch ; ch++ ) */
      {
	if(dbg) 
	  {	
	    printf(" %12s  ",ch_set[ch].chname);	
	    printf
	      ("%07.2f  %07.2f  %07.2f  %07.2f  %07.2f  %07.2f  %4x     %2d \n",
	       ch_read[ch].vread/scalev,ch_read[ch].iread/scalei,ch_set[ch].v0set/scalev,
	       ch_set[ch].i0set/scalei,ch_set[ch].v1set/scalev,ch_set[ch].i1set/scalei,
	       ch_set[ch].flag,ch);
	    
	    printf
	      ("%4d    %3d    %3d    %05.1f    %4x    %2d \n\n",
	       ch_set[ch].vmax,ch_set[ch].rup,ch_set[ch].rdwn,ch_set[ch].trip/10.0,
	       ch_read[ch].status,ch);      

	      printf(" I=%d (%x)  V=%d (%x)\n", ch_read[ch].iread,ch_read[ch].iread,
		     ch_read[ch].vread, ch_read[ch].vread );
	    printf(" Scale I=%f (%d), V=%f (%d)\n", scalei, crates[craten].brds[bd].deci,
		   scalev, crates[craten].brds[bd].decv);
	  }
	get_prop(tmp1, craten, bd, ch, prop);
	strcat(stmp, tmp1);
      }  	  
    } else /* ch > crates[craten].brds[bd].numch */
      return ERROR; 
   }
 } /* end if(crates[craten]...) */
  else return ERROR;

  
  return 0;
}

/******** wrapper with semaphores for get_chprop() ********/ 
int get_wchprop(char *stmp, ushort cr_num, ushort bd, int chan, int prop) 
{
  int err;
  /** semaphore take **/
  craten=cr_num;
  err=get_chprop(stmp, bd, chan, prop);
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

  if(dbg)   printf("set_onr_chprop(); scaleV=%f, scaleI=%f\n",scalev, scalei); 

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
      if(cepw==0) cnet_buff[1]=0x0800; /*0x04D8;  OFF */
      break;
    }
  
  printf("to_caen_write: buff[0]=%x, buff[1]=%x(%d)\n", cnet_buff[0], cnet_buff[1], cnet_buff[1]);

  /*  if((response=caenet_write2( crt, cod, (char *)cnet_buff,sizeof(cnet_buff))) != TUTTOK) */
  if( (response = wdo_ch( crt, bd, chan, cod,  (char *) cnet_buff)) != TUTTOK)
    {
      printf(" Caenet_write: Error number %d received\n",response);
      return ERROR;
    }
  return 0;
}

/**********************************************************
|
* sets property value for ONE or ALL channels in slot
|
**********************************************************/
int set_chprop(char *tmp, ushort bd, int chan, int prop )
{
  int ch=0,
    err=1;
  float fval;
  float fv[25];


  if( (bd<10) && ((crates[craten].config >> bd) &1) ) /* check if board in slot=bd is present */
   {
     if(chan<0) { /* set ALL channels */
       err=sscanf(tmp,
		  "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
		  &fv[0],&fv[1],&fv[2],&fv[3],&fv[4],&fv[5],&fv[6],&fv[7],&fv[8],&fv[9],
		  &fv[10],&fv[11],&fv[12],&fv[13],&fv[14],&fv[15],&fv[16],&fv[17],&fv[18],&fv[19],
		  &fv[20],&fv[21],&fv[22],&fv[23],&fv[24]
		  );
       printf("sscanf() returns = %0x (%0d)\n",err,err);
       if(err < crates[craten].brds[bd].numch) 
	 return ERROR;
       
       for( ch=0 ; ch < 25 && ch < crates[craten].brds[bd].numch ; ch++ )
	 {  
	   /*  sscanf(tmp,"%s ", stmp); *//* replace !!! */

	   printf("SET prop(%0d) to %f\n", prop, fv[ch]);
	   if( set_one_chprop(fv[ch], craten, bd, ch, prop ) == ERROR) 
	     return ERROR;
	  
	   update_PS(bd, prop,0); /* increment PS, LS, GS */
	 }
     }
     /*----------------------------*/
     else { /* set ONE channel */
       if(chan < crates[craten].brds[bd].numch)
	 {
	   err=sscanf(tmp,"%f", &fval);
	   /*	   if(err<=0) {
	     err=sscanf(tmp,"%x", &hval); 
	     printf("sscanf hex: %d(0x%x)\n",hval,hval);
	     fval=1.0*hval;
	   }
	   */	   if(err<=0) 
	     return ERROR;
	   
	   if( set_one_chprop(fval, craten, bd, chan, prop ) == ERROR) 
	     return ERROR;    
	   printf("SET prop(%0d) to %s\n", prop, tmp);

	   update_PS(bd, prop, 0); /* increment PS, LS, GS */

	 } else 
	 return ERROR;
     }
   }
 else 
   return ERROR;

  /* update summary numbers */
  update_GS(-1);
 return 0;
}

/******** END of My staff ***********************/


/***-------------------------------------------------------------------
    Main Program
--------------------------------------------------------------------***/
int menu(ushort addr, ushort crate)
/*     int argc;
     char **argv;
*/
{
  int temp;

  /*  if(argc != 3) */
  if(addr==0)
    {
      puts(" Usage: vmesy527 <v288 vme address (in hex)> <sy527 Caenet number (in hex)>");
      return(0);
    }
  v288addr=addr;
  craten=crate;
  /*
  sscanf(*(++argv),"%x",&v288addr);
  sscanf(*(++argv),"%x",&craten);
  */
  /*  v288addr=AUPDATE(v288addr);*/  /* For Eltec E-6 VME Board */
  /*  init_buserr(); */             /* To handle Bus Error */
  
  /* while( (kb=kbHit(0))==0 ) {
    taskDelay(30);
  }
  */  
  printf("addr=<%x> crate=%d \n", v288addr, craten);
  
  /*
    Main Loop
  */
  for(;;) {
    switch(makemenu())
      {
      case 0:
	read_ident(0);
	break;
      case 1:
	crate_map();
	break;
      case 2:
	ch_monitor();
	break;
      case 3:
	speed_test();
	break;
      case 4:
	par_set();
	break;
      case 5:
	printf("--> Input crate number (1-99) (current=%0d) : ",craten);
	scanf("%d", &temp);
	craten=temp;
	printf("-->Selected crate number : %0d\n", craten); 
	break;
       case 9:
	/*	deinit_buserr(); */
	/*	exit(0); */
	return 0;
	break;
      default:
	break;
      }
    taskDelay(50);
  }
  return 0;
}
/*** The END ****/
