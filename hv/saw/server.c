/*
 * server.c
 * 7 Apr 2015
 * server - runs on VME CPU with V288 caenet board to control CAENET HV crate type of SY527.
 *        It process messages from HV GUI transfer command to hvcrate and sends responses from hv crate.
 *        Start parameters: <port_number> <crate_number>
 */
#ifdef VXWORKS
#include <sockLib.h> 
#include <inetLib.h> 
#include <stdioLib.h>
#include <usrLib.h>
#include <ioLib.h>
#include <taskLib.h>
#include <semLib.h>
#include <vxWorks.h>
#include <stdio.h>
#include <signal.h>
#endif

#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*#include "../sy527/vmcaenet.h"*/
#include "vmcaenet.h"
#ifdef VXWORKS
/************** defined in sy527.c ***********/
/*extern int init_sy527(int crt);*/
extern int read_ident(int crt);      /* in sy527.c  */
extern int crate_map(int crt);       /* get crate occupation, fills struct boards[10]; */
/*extern unsigned short craten; *//*  crate number(1-99) in sy527.c */
extern unsigned int v288addr; /*  v2888 board address in sy527.c */

extern struct ccrate crates[15];  /* crates parameters  */

extern int init_sy527(int crate); /* initialization of crate, fill array crates[] */
extern int clr_alarm(int crate); /* clear alarm in CAEN crate */
extern int kill_all(int crate); /* kill (turn immediate OFF) all channels in crate */

extern int get_chprop(char *stmp, ushort cr_num, ushort bd, int chan, int prop);
extern int set_chprop(char *stmp, ushort cr_num, ushort bd, int chan, int prop);
extern int prop_to_indx(char *tmp);
extern int get_PROP(char *prop);
extern int get_GS(unsigned short cr_num, char *gss);
extern int get_LS(unsigned short cr_num, char *lss);
extern int get_PS(unsigned short cr_num, char *pss, int slot);
extern int addr_ltos(unsigned short cr_num, unsigned short indx);

/************************************************************************/
#endif

#ifndef VXWORKS
#define ERROR     -1
#endif

#define DEF_PORT   2001


#define  L16          16
#define  L256         256
#define  L1024        1024
#define  L4096        4096


extern int get_val(int val); 


/* Network variables */
/* char nio_RXbuff[L1024];*/ /* receive buffer */
/* char nio_TXbuff[L1024];*/ /* transmit buffer */
int           nio_RXlen; /* received message length */
int           nio_TXlen; /* Network - transmit buffer length */
unsigned char nio_quit = 0; /* close connection */


/*
 * int get_slot(char *s) - returns slot number (0-10) from input command or ERROR
 */
int get_slot(char *s) {  
  char tmp_str[6];
  int slot=-1;

  strncpy(tmp_str, s, 2); /* store Slot number to tmp */
  if (strlen(tmp_str)>0) {
    slot=atoi(tmp_str);
    printf("SLOT=%0d\n",slot);
    return slot;
  }
  return ERROR;
}


/*
 * int get_ch(char *s) - returns channel number (0-24) from input command or ERROR
 */
int get_ch(char *s) {
  
  char tmp_str[6];
  char *dot_pos;
  int ch=-1;
  strncpy(tmp_str, s, 5); /* store Slot number(2 chars) plus 'dot' plus channel number(2 chars) (SLOT.CHANNEL) to tmp */

  if ((dot_pos=strchr(tmp_str, '.')) != NULL) {
    if (strlen(&dot_pos[1])>0) {
      /*      ch = (int) strtol(&dot_pos[1], (char **)NULL, 10);    
      printf("CH_str=%0d\n",ch);
      */
      ch=atoi(&dot_pos[1]);
      printf("CH=%0d\n",ch);
      return ch;
    } 
  } 
  return ERROR;
}
/*
 *
 ******************************************/
int get_chaddr(char *s, int *slot, int *ch, char *p) 
{  
  char str1[15]; /*command*/
  char str2[10]; /* S.slot.chan */
  char str3[10]; /* property name */
  int slen1,slen2, slen3;

  sscanf(s, "%s %s %s",  str1, str2, str3);
  slen1=strlen(str1);
  slen2=strlen(str2);
  slen3=strlen(str3);

  printf("str1=<%s> (%d) str2=<%s> (%d) str3=<%s> (%d)\n", 
	 str1, slen1, str2, slen2, str3, slen3);
  if(slen2>1) {
    if(strncmp(str2,"L",1) == 0) { /** L - logical unit addressing */
      *slot=get_slot(&str2[1]);
      *ch=get_ch(&str2[1]);
      
      if(slen3>1) {
	memcpy(p, str3, slen3);
      }

      return 1;
    }
    else if(strncmp(str2,"S",1) == 0) { /** S - slot  addressing */
      *slot=get_slot(&str2[1]);
      *ch=get_ch(&str2[1]);

      if(slen3>1) {
	memcpy(p, str3, slen3);
      }

      return 0;
    }
    else return ERROR; /* wrong address */
  }
  
  return 0;
}

/*
 *
 *********************************************************/
int addr_to_s(int ls, int crnum, int *slot, char *cmd, char *buff)	{
  int err=0;
  

  if(ls==0) {
    /* S - slot address */ /* FIXME! need to check module in slot! */
    sprintf(buff,"     1 %s S%d ", cmd, *slot);
    err=ls;
  }
  if(ls==1)
    { /* L - logical unit address */
      sprintf(buff,"     1 %s L%d ", cmd, *slot);
		  
#ifdef VXWORKS
      err=addr_ltos(crnum, *slot); /* convert from L to S */

      if(err!=ERROR)
	*slot=err; /* slot number from logical unit number */
#endif
    }

  return err;
}

/* =====================================================================================
 *
 * Command Task - commands arriving through the network connection are re-directed to
 * the high-voltage modules. Module responses are sent back through the same network
 * connection
 *
 * =====================================================================================
 */
static void cmdTSK(int connection_fd, char *nio_RXbuff, char *nio_TXbuff, int crnum)
 {
  char tmp[L256];
  char tmp1[80];
  char tmp2[10];
  int slot=-1, ch=-1;
  char *pchar;
  int c_off=0; /* command offset (in chars) */
  int ii=0, nm=0;
  int err;

  for(;;)
    {
        memset(nio_RXbuff, '\0', sizeof(nio_RXbuff)); 
        memset(nio_TXbuff, '\0', sizeof(nio_TXbuff)); 
        nio_RXlen=0;
        nio_RXlen=read(connection_fd, nio_RXbuff, L1024-1);
	if(nio_RXlen>L1024) {
	  printf("cmdTSK(%0d): ERROR: receiving message to large: %d\n", crnum, nio_RXlen);
	  break;
	}
/***        do {
          len = read(connection_fd, &nio_RXbuff[len], L4096-1);
          nio_RXlen+=len;
        }
        while( (len>0) && (nio_RXlen<=256));
***/
        /* nio_RXlen = read(fd, nio_RXbuff, 1); */
        
        if(nio_RXlen <= 0)
          {
          /* either the client closed the connection before sending any data or
           * there is a problem with the connection - bail out
           *
           *
	   * printf("cmdTSK - error receiving message....\n");
	   */	    
          break;
          }
      else
      {
	/********************* pass command to module (emulate)  ********************/
	
	/*   time(&now); 
	     printf(" %s ", ctime(&now));            */
        printf("\ncmdTSK(%0d) : received (%d): %s\n", crnum, nio_RXlen, nio_RXbuff);

        memset(tmp, '\0', sizeof(tmp));                 
	memset(tmp1, '\0', sizeof(tmp1));                 
	memset(tmp2, '\0', sizeof(tmp2));                 

	/*	sscanf(&nio_RXbuff[c_off],"%s %s", tmp1, tmp);
	if(strncmp(&nio_RXbuff[c_off],"RC", 2) == 0) {
	  sscanf(tmp, "%s%d.%d %s", tmp2,  &slot, &ch, tmp1);
	  printf("sscanf:<%s %d.%d> <%s>\n", tmp2, slot, ch, tmp1);
	} 

        memset(tmp, '\0', sizeof(tmp));                 
	memset(tmp1, '\0', sizeof(tmp1));                 
	memset(tmp2, '\0', sizeof(tmp2));                 
	*/

  /*********************************** 
 ------- ONE word commands  ---------
************************************/
/**___ LL _____ ****/
	if(strncmp(&nio_RXbuff[c_off],"LL",2) == 0) { /** LL command (Module list) */

#ifdef VXWORKS	  
	  /** NOTE!!! Crate number 'craten' hase to be defined before this call !!! **/
	  crate_map(crnum);
	  printf("crates[ %0d ].config=0x%0X \n",crnum, crates[crnum].config);
	  if((crates[crnum].config>0) && crates[crnum].connected)
	    {
	      strcpy(nio_TXbuff,"     1 LL");
	      for( nm=0, ii=1 ; nm<10 ; nm++, ii = ii << 1 )
		{
		  if(crates[crnum].config & ii) {
		    sprintf(tmp1," S%d",nm);
		    strcat(nio_TXbuff,tmp1);
		  }	    
		}
	    } 
	  else /* no module in slot or crate is disconnected/powered OFF*/
	    {
	       strcpy(nio_TXbuff,"   124 ERROR Logical units not found!");
	    }
#else
	  strcpy(nio_TXbuff,"     1 LL S0"); /* emulation mode */
#endif
	}

	else if(strncmp(&nio_RXbuff[c_off],"Hi",2) == 0) { /** Hi **/
	  strcpy(nio_TXbuff,"     1 Hi Hi! How are You?\r\n");   
	}

/**___ CONFIG _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"CONFIG",6) == 0) { /** CONFIG command **/

#ifdef VXWORKS
	  if(crates[crnum].connected)
	    if(crates[crnum].hvstatus)
	      strcpy(nio_TXbuff,"     1 CONFIG 2174 0001 0044 0081 0001"); /* HV ON */
	    else
	      strcpy(nio_TXbuff,"     1 CONFIG 0174 0001 0044 0081 0001"); /* HV OFF */
	  else 
	    strcpy(nio_TXbuff,"     1 CONFIG 6004 0001 0044 0081 0001");   /* set ERRORS & ALARMS */
	  
#endif
	}


/**___ ENET _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"ENET",4) == 0) { /** ENET command (does nothing for CAEN) **/
	  /**** NOTE!!! 
	   * 'strcat' -removes terminated '\0' from destination string, 
	   * and does not guaranties terminated '\0' in return string!
	   */
	  strcpy(nio_TXbuff,"     1 ENET PORT 6002");
	}

/**___ SYSINFO _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"SYSINFO",7) == 0) { /** SYSINFO command **/
#ifdef VXWORKS
	  if(read_ident(crnum)==0)  
	    sprintf(nio_TXbuff,"     1 SYSINFO CRATE %0d CAEN %s", crnum, crates[crnum].ident);
	  else sprintf(nio_TXbuff,"     1 SYSINFO CRATE %0d NOT PRESENT", crnum);
#else
	  sprintf(nio_TXbuff,"     1 SYSINFO CRATE 0 CAEN SY527");
#endif

	  /*  strcat(nio_TXbuff,"C    1 LeCroy Model:      1458\r\n\0");
	  */
	}

/**___ HVOFF_____ ****/	
	else if(strncmp(&nio_RXbuff[c_off],"HVOFF",5) == 0) { /** HVOFF command (ON/OFF) **/
	  printf("Crate=%d HVOFF command \n",crnum);
	  ii=kill_all(crnum); /* kill (HV OFF) all channels */
	  printf("Crate=%d KILL ALL channels: %d \n",crnum, ii);
	  strcpy(nio_TXbuff,"     1 ");
	  strcat(nio_TXbuff, nio_RXbuff);
	}

/**___ HVON_____ ****/	
	else if(strncmp(&nio_RXbuff[c_off],"HVON",4) == 0) { /** HVON command  **/
	  printf("Crate=%d HVON command \n",crnum);
	  ii=clr_alarm(crnum); /* clear any alarms */
	  printf("Crate=%d Clear Alarms returns: %d \n",crnum, ii);
	  strcpy(nio_TXbuff,"     1 ");
	  strcat(nio_TXbuff, nio_RXbuff);
	}

/**___ HVSTATUS _____ ****/	
	else if(strncmp(&nio_RXbuff[c_off],"HVSTATUS",8) == 0) { /** HVSTATUS command (ON/OFF) **/
#ifdef VXWORKS
	  if(read_ident(crnum)==0) { 	  
	    if(crates[crnum].hvstatus)
	      strcpy(nio_TXbuff,"     1 HVSTATUS HVON");   /* state ON */
	    else 
	      strcpy(nio_TXbuff,"     1 HVSTATUS HVOFF");   /* state OFF */	      
	  } else {
	    strcpy(nio_TXbuff,"     1 HVSTATUS HV???");   /* state unknown */
	  }
#endif
	}

/**___ PUPSTATUS _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"PUPSTATUS",9) == 0) { /** PUPSTATUS command (power up status) **/
#ifdef VXWORKS
	  sprintf(nio_TXbuff,"     1 PUPSTATUS %4X %4X %4X", 
		  crates[crnum].gstatus[0], crates[crnum].gstatus[1], crates[crnum].hvstatus);
#else
	  strcpy(nio_TXbuff,"     1 PUPSTATUS 1 1 1");   
#endif
	}

/**___ PS _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"PS",2) == 0) { /** PS command (property summary number **/
	  /* returns series of 4 hex digits one for each propery in PROP command */

	  /*  pchar=&nio_RXbuff[4];*/ /* get Slot number */	  
	  slot=-1;
	  ch=-1;
	  err=get_chaddr(&nio_RXbuff[c_off], &slot, &ch, tmp1);	  
	  printf("get_chaddr=%d slot=%d chan=%d  prop=%s\n", err,slot, ch, tmp1);

	  if((err!=ERROR) && (slot>=0))   
	    /* if((slot=get_slot(pchar)) >=0)*/
	    {
	      if (err==0) /* S - slot address */
		sprintf(nio_TXbuff,"     1 PS S%d ", slot);
	      if(err==1)
		{ /* L - logical unit address */
		  sprintf(nio_TXbuff,"     1 PS L%d ", slot);
#ifdef VXWORKS
		  err=addr_ltos(crnum, slot);
#endif
		  if(err!=ERROR)
		    slot=err; /* slot number from logical unit number */
		}

	    /***	    sprintf(tmp,"%04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X", 
			    psn[0], psn[1], psn[2], psn[3], psn[4], psn[5], psn[6], psn[7], psn[8],psn[9], psn[10], psn[11], psn[12]);
	    ***/
#ifdef VXWORKS
	      if(err!=ERROR)
		{

		  get_PS(crnum, tmp, slot);
		  strcat(nio_TXbuff,tmp);
		}
	      else {
		sprintf(tmp, "   124 ERROR Logical unit L%d not found!", slot);
		strcpy(nio_TXbuff, tmp);
	      }
#endif
	    } else {
		sprintf(tmp, "   111 ERROR Wrong slot number!");
		strcpy(nio_TXbuff, tmp);	    
	  }
	}

/**___ GS _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"GS",2) == 0) { 
	  /** GS command (global summary number: 5 words( 4-digits hex format)) 
	  *   1 - measured value summary number (incrementd each time a measaured value in any module is changed)
	  *   2 - settable value summary number (incrementd each time a settable value in any module is changed)
	  *   3 - configuration summary number (incrementd each time a configuration value of crate is changed: 
	  *                                     power on/off, front keys, limits, interlocks, ...)
	  *   4 - crate activity summary number (incremented each time when words 1 and 2 are updated)
	      5 - host activity summary number (incremented on each processed GS command)
	  **/
	  /****	  sprintf(nio_TXbuff," 1     GS %04X %04X %04X %04X %04X", gsn[0], gsn[1], gsn[2], gsn[3]++, gsn[4]++); ****/
                
#ifdef VXWORKS
	  get_GS(crnum, tmp);
	  sprintf(nio_TXbuff," 1     GS %s", tmp);
#endif
	}

/**___ LS _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"LS",2) == 0) { 
	  /** LS command (logical summary number) 
	  *   2 hex words 4-digit for each module(unit) in crate: 
	  *   1 - measured value summary number for unit
	  *   2 - settable value summary number fro unit 
	  **/
	  /*****
	  sprintf(nio_TXbuff," 1     LS");
	 
          nboard=1;
	  for(ii=0;ii<2*nboard;ii++) {
	    sprintf(tmp1," %04X", lsn[ii]);                
	    strcat(tmp, tmp1);
	  }
	  strcat(nio_TXbuff, tmp);
	  ****/
#ifdef VXWORKS
	  get_LS(crnum, tmp);
	  sprintf(nio_TXbuff,"     1 LS %s",tmp);
#endif
	}

/**___ CRATE _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"CRATE",5) == 0) { /** Crate command (for test only) **/
	  pchar=&nio_RXbuff[6]; /* get Crate number */	  	 
#ifdef VXWORKS
	  if((slot=get_slot(pchar)) >=0) {
	    crnum=slot;
	  }
	  sprintf(nio_TXbuff,"     1 CRATE %d",crnum);   
#else
	  sprintf(nio_TXbuff,"     1 CRATE %d", 0);  
#endif	
	}

/**___ INIT _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"INIT",4) == 0) { /** Init command (for test only) **/
	  /*** for test **/
#ifdef VXWORKS
	  /*	  v288addr=0xe0600000;
		  crnum=12;*/
	  if((err=init_sy527(crnum))<0) {
	    printf("Warning: server: init_sy527(): crate=%0d not present\n",crnum);
	  } else {
	    printf("Crate=%0d type: %s\n", crnum,crates[crnum].ident);
	  }
	  /*	  crate_map();*/
	  printf("crate_conf=%0X\n", crates[crnum].config);
#endif
	  sprintf(nio_TXbuff,"     1 INIT %d", err);   
	}

  /******************************************
 ------------ Few words commands ----------
******************************************/
/**___ ID _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"ID",2) == 0) { /** ID command (module identification) **/
	  /* type sub_module sub_mod-number number_of_properties number_of_channels serial_number */
	  /*  pchar=&nio_RXbuff[4];*/ /* get Slot number */
	  slot=-1;
	  ch=-1;
	  err=get_chaddr(&nio_RXbuff[c_off], &slot, &ch, tmp1);	  
	  printf("get_chaddr=%d slot=%d chan=%d  prop=%s\n", err,slot, ch, tmp1);
	  
	  if((err!=ERROR) && (slot>=0))   
	    /*if((slot=get_slot(pchar)) >=0)*/
	    {
	      if (err==0) /* S - slot address */
		sprintf(nio_TXbuff,"     1 ID S%d ", slot);	    		
	      if(err==1)
		{ /* L - logical unit address */
		  sprintf(nio_TXbuff,"     1 ID L%d ", slot);	    		
#ifdef VXWORKS
		  err=addr_ltos(crnum, slot);
#endif
		  if(err!=ERROR)
		    slot=err; /* slot number from logical unit number */
		}
	      
	      
	      /*sprintf(nio_TXbuff,"     1 ID S%d ", slot);	    */
	      
	      /** NOTE !!!! function 'crat_map()' has to be called before !!!! ***/
	      if(err!=ERROR)
		{
#ifdef VXWORKS
		  sprintf(tmp,"%s 0 1 %d %d %3d %d.%02d 1000 1.135", 
			  crates[crnum].brds[slot].name,
			  crates[crnum].ps[slot].prop_num,
			  crates[crnum].brds[slot].numch,
			  crates[crnum].brds[slot].sernum,
			  crates[crnum].brds[slot].vermaior,
			  crates[crnum].brds[slot].verminor ); 
		  
		  strcat(nio_TXbuff, tmp);		  
#else
		  strcat(nio_TXbuff,"A932A 0 1 12 12 B51884 -1 1000 1.135\r\n"); 
#endif
		}	      	    
	      else {
		sprintf(tmp, "   124 ERROR Logical unit L%d not found!", slot);
		strcpy(nio_TXbuff, tmp);
	      }
	      
	    }
	  else {
	    sprintf(tmp, "   111 ERROR Wrong slot number!");
	    strcpy(nio_TXbuff, tmp);	    
	  }
	  
	}

/**___ PROP _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"PROP",4) == 0) { /** PROP command (list of module properties **/
	  /* pchar=&nio_RXbuff[6]; */ /* get Slot number */	  	 
	  slot=-1;
	  ch=-1;
	  err=get_chaddr(&nio_RXbuff[c_off], &slot, &ch, tmp1);	  
	  printf("get_chaddr=%d slot=%d chan=%d  prop=%s\n", err,slot, ch, tmp1);
	  
	  /*	  err=addr_to_s(err, crnum, &slot, "PROP", nio_TXbuff);
	  printf("addr_to_s=%d slot=%d  txbuff=%s\n", err,slot, nio_TXbuff);
	  */
	  if((err!=ERROR) && (slot>=0))   	
	    {
	      if(err==0) /* S - slot address */
		sprintf(nio_TXbuff,"     1 PROP S%d ", slot);
	      
	      if(err==1)
		{ /* L - logical unit address */
		  sprintf(nio_TXbuff,"     1 PROP L%d ", slot);
		  
#ifdef VXWORKS
		  err=addr_ltos(crnum, slot); /* convert from L to S */
#endif
		  if(err!=ERROR)
		    slot=err; /* slot number from logical unit number */
		}
	      /*	      sprintf(nio_TXbuff,"     1 PROP S%d ", slot); 	  */
	      /*	  strcpy(nio_TXbuff,"     1 PROP S0 MC MV DV RUP RDN TC CE ST MVDZ MCDZ HVL\r\n");      */
	      if(err!=ERROR)
		{	      
#ifdef VXWORKS
		  get_PROP(tmp);
		  strcat(nio_TXbuff, tmp);
#else	    
		  strcat(nio_TXbuff,"CE MC MV DV DC RUP RDN TC ST SVL STAT FLAG\r\n"); 
#endif	      
		}
	      else {
		sprintf(tmp, "   124 ERROR Logical unit L%d not found!", slot);
		strcpy(nio_TXbuff, tmp);
	      }
	    }
	  else {
	    sprintf(tmp, "   111 ERROR Wrong slot number!");
	    strcpy(nio_TXbuff, tmp);	    
	  }
	}

/**___ ATTR _____ ****/
	else if(strncmp(&nio_RXbuff[c_off],"ATTR",4) == 0) { /** ATTR command (list of attributes for property **/
	  /*  pchar=&nio_RXbuff[6]; */ /* get Slot number */	  
	  slot=-1;
	  ch=-1;
	  err=get_chaddr(&nio_RXbuff[c_off], &slot, &ch, tmp1);	  
	  printf("get_chaddr=%d slot=%d chan=%d  prop=%s\n", err,slot, ch, tmp1);
	  
	  if((err!=ERROR) && (slot>=0))   	
	    /*if((slot=get_slot(pchar)) >=0) */
	    {
	      if(err==0) /* S - slot address */
		sprintf(nio_TXbuff,"     1 ATTR S%d %s", slot, tmp1); 	  		
	      if(err==1)
		{ /* L - logical unit address */
		  sprintf(nio_TXbuff,"     1 ATTR L%d %s", slot, tmp1); 	  		  
#ifdef VXWORKS
		  err=addr_ltos(crnum, slot);
#endif
		  if(err!=ERROR)
		    slot=err; /* slot number from logical unit number */
		}
	      	      
	      if( strlen(tmp1) >0 )	      
		{
		  printf(" ATTR=%s\n", tmp1);
		  strncpy(tmp, tmp1, strlen(tmp1));
	       
			/* sprintf(nio_TXbuff,"     1 ATTR S%d %s", slot, tmp); */ 	  
	 
		  if(strncmp(tmp,"DV",2) == 0)   /** DV property **/           
		    /*	    strcat(nio_TXbuff," V0set_V V N N -2500.0_0.0_0.5 %7.1lf\r\n"); */ /* set of attributes for DV */ 
		    strcat(nio_TXbuff," V0set_V V N N 0.0_2500.0_0.2 %7.1lf\r\n"); /* set of attributes for DV (only positive!!!)*/
		  else if(strncmp(tmp,"DC",2) == 0)  /** DC property **/
		    strcat(nio_TXbuff," I0set_mA mA N N 0.0_500.0_0.5 %7.2lf\r\n"); /* set of attributes for DC (only positive!!!) */   
		  else if(strncmp(tmp,"CE",2) == 0)  /** CE property **/      
		    strcat(nio_TXbuff," CE_En na P N En_Ds %2s\r\n"); /* set of attributes for CE */
		  else if(strncmp(tmp,"RUP",3) == 0)  /** RUP property **/            
		    strcat(nio_TXbuff," RUP_V/s V/s P N 10_500_1 %3d\r\n"); /* set of attributes for RUP */
		  else if(strncmp(tmp,"RDN",3) == 0)  /** RDN property **/            
		    strcat(nio_TXbuff," RDN_V/s V/s P N 10_500_1 %3d\r\n"); /* set of attributes for RDN */
		  else if(strncmp(tmp,"STAT",4) == 0)  /** STAT property **/      
		    strcat(nio_TXbuff," Status na M N 2 %4x\r\n"); /* set of attributes for ST */     
		  else if(strncmp(tmp,"ST",2) == 0)  /** ST property **/      
		    strcat(nio_TXbuff," CH_Stat na M N 2 %4x\r\n"); /* set of attributes for ST */     
		  else if(strncmp(tmp,"MVDZ",4) == 0)  /** MVDZ property **/          
		    strcat(nio_TXbuff," MV_Zone V N N 0_300 %8f\r\n"); /* set of attributes for MVDZ */      
		  else if(strncmp(tmp,"MCDZ",4) == 0)  /** MCDZ property **/          
		    strcat(nio_TXbuff," MC_Zone uA N N 0_300 %8f\r\n"); /* set of attributes for MCDZ */     
		  else if(strncmp(tmp,"TC",2) == 0)  /** TC property **/      
		    strcat(nio_TXbuff," Trip_s ms P N 0_1000_1 %5.1f\r\n"); /* set of attributes for TC */  
		  else if(strncmp(tmp,"HVL",3) == 0)  /** HVL property **/            
		    strcat(nio_TXbuff," HVmax_V V P N 0_2500_1 %4d\r\n"); /* set of attributes for HVL */  
		  else if(strncmp(tmp,"SVL",3) == 0)  /** SVL property **/            
		    strcat(nio_TXbuff," SVmax_V V P N 0_2500_1 %4d\r\n"); /* set of attributes for SVL */  
		  else if(strncmp(tmp,"MV",2) == 0)  /** MV property **/     
		    strcat(nio_TXbuff," Vmon_V V M N 7 %7.1lf\r\n"); /* set of attributes for MV */	  
		  else if(strncmp(tmp,"MC",2) == 0)  /** MC property **/      
		    strcat(nio_TXbuff," Imon_mA mA M N 7 %7.3lf\r\n"); /* set of attributes for MC */	  
		  else if(strncmp(tmp,"FLAG",4) == 0)  /** FLAG property **/      
		    strcat(nio_TXbuff," FLAG_N uA M N 7 %4x\r\n"); /* set of attributes for FLAG */	  
		  /** NOTE !!! remove or modify!!! **/
		  else        
		    strcat(nio_TXbuff," None_N N M N 7 %7.1lf\r\n"); /* one set of attributes for all properties */
		}
	      else {
		sprintf(tmp, "   110 ERROR Unknown property name!");
		strcpy(nio_TXbuff, tmp);	    
	      }

	      if(err==ERROR) {
		sprintf(tmp, "   124 ERROR Logical unit L%d not found!", slot);
		strcpy(nio_TXbuff, tmp);	    
	      }
	      
	    }
	  else {
	    sprintf(tmp, "   111 ERROR Wrong slot number!");
	    strcpy(nio_TXbuff, tmp);	    
	  }  
	}

/**___ RC _____ ****/
	/*****____-----_____-----_______-----______------_____-----_____-----*****/
	else if(strncmp(&nio_RXbuff[c_off],"RC",2) == 0) { /** RC command (recall)**/

	  slot=-1;
	  ch=-1;
	  err=get_chaddr(&nio_RXbuff[c_off], &slot, &ch, tmp1);
	  printf("get_chaddr=%d slot=%d chan=%d  prop=%s\n", err,slot, ch, tmp1);


	  /*	  if ((pchar=strrchr(&nio_RXbuff[c_off], ' ')) != NULL) *{ */ /* locate last blank (before property name) */
	    /*   strcpy(tmp1, &pchar[1]);*/ /* copy property name */
	  
	  if((err!=ERROR) && (slot>=0))   	
	    {
	      if(err==0) /* S - slot address */
		sprintf(nio_TXbuff,"     1 RC S%d", slot); 	  		
	      if(err==1)
		{ /* L - logical unit address */
		  sprintf(nio_TXbuff,"     1 RC L%d", slot); 	  		  
#ifdef VXWORKS
		  err=addr_ltos(crnum, slot);
#endif
		  if(err!=ERROR)
		    slot=err; /* slot number from logical unit number */
		}

	      
	      printf("PROPERTY=%s\n",tmp1);
	      
	      if(strlen(tmp1)>0) 
		{
		  ii=prop_to_indx(tmp1);
		  if (ii >= 0)  
		    {

		if(ch<0 && slot>=0) { /* only slot number */
		  /*		sprintf(nio_TXbuff,"     1 RC S%d %s", slot, tmp1); */
		  sprintf(tmp," %s", tmp1);
		  strcat(nio_TXbuff, tmp); /* add property name */
#ifdef VXWORKS

		    memset(tmp, '\0', sizeof(tmp));                 
		    printf("\nPROP=%s index=%d\n", tmp1, ii);
		    err=get_chprop(tmp, crnum, slot, ch, ii); /* get property value for all channels */
		    if(err==ERROR) { /* get_chprop() returns error! */
		      printf(" cmdTsk(%0d): ERROR RC command: get_chprop(1) returns ERROR !\n", crnum);
		      sprintf(nio_TXbuff,"   111 ERROR RC command");
		    }

#endif		
		}
		else if(ch>=0 && slot >=0) { /* slot and channel numbers*/
		  /*		sprintf(nio_TXbuff,"     1 RC S%d.%d %s", slot, ch, tmp1);*/
		  sprintf(tmp,".%d %s", ch, tmp1);
		  strcat(nio_TXbuff, tmp); /* add channel and property name */
#ifdef VXWORKS

		    memset(tmp, '\0', sizeof(tmp));                 
		    printf("\nPROP=%s index=%d\n",tmp1, ii);
		    err=get_chprop(tmp,crnum, slot, ch, ii); /* get property value for ONE channel */
		    if(err==ERROR) { /* get_chprop() returns error! */
		      printf(" cmdTsk(%0d): ERROR RC command: get_chprop(2) returns ERROR !\n", crnum);
		      sprintf(nio_TXbuff,"   111 ERROR RC command");
		    }
#endif
		}
		
		else {
		  err=ERROR;
		  printf(" cmdTsk(%0d): ERROR RC command: no Slot/Ch number\n", crnum);
		  sprintf(nio_TXbuff, "   110 ERROR no Slot/Ch number");
		}
		    
		    } else {
		    err=ERROR;
		    printf(" cmdTsk(%0d): ERROR RC command: property name=%s ERROR\n", crnum, tmp1);
		    sprintf(nio_TXbuff, "   110 ERROR Unknown property name!");
		  }
		} else {
		  err=ERROR;
		  printf(" cmdTsk(%0d): ERROR RC command: property ERROR\n", crnum);
		  sprintf(nio_TXbuff, "   110 ERROR No property!");
		}

		if(err >= 0) {
		  strcat(nio_TXbuff,tmp); /* OK- copy response to tx buff */
	      }
	    }
	  else { 
	    printf("cmdTsk(%0d): ERROR RC command:\n",crnum);	    
	  }	  
	}

/**___ LD _____ ****/	
	/****------*************--------***************-----***************-------*********-------*******/
	else if(strncmp(&nio_RXbuff[c_off],"LD",2) == 0) { /** LD command (load value)**/

	  err=get_chaddr(&nio_RXbuff[c_off], &slot, &ch, tmp1);
	  printf("get_chaddr=%d slot=%d chan=%d  prop=%s\n", err,slot, ch, tmp1);

	  sprintf(nio_TXbuff,"     1 ");

	  /* pchar=&nio_RXbuff[4];
	  slot=get_slot(pchar);
	  ch=get_ch(pchar);
	  */
	  
	  if((err!=ERROR) && (slot>=0))   	
	    {
	      if(err==0) /* S - slot address */
		; /*sprintf(nio_TXbuff,"     1 LD S%d", slot);*/ 	  		
	      if(err==1)
		{ /* L - logical unit address */

		  /* sprintf(nio_TXbuff,"     1 LD L%d", slot); */ 	  		  
#ifdef VXWORKS
		  err=addr_ltos(crnum, slot);
#endif
		  if(err!=ERROR) {
		    printf("L_to_S: L=%d, S=%d\n", slot, err);
		    slot=err; /* slot number from logical unit number */
		  }
		} /* else ??? */

	  if ((pchar=strchr(&nio_RXbuff[4], ' ')) != NULL) 
	    { /* locate blank (before property name) */

	      /* ii=pchar-nio_RXbuff; */ /* blank position(before property name) in nio_RXbuff */

	      strcpy(tmp, &pchar[1]); /* copy property name + values to set */
	      /* printf("prop+val=%s\n",tmp); */

	      /**	      if( (strlen(tmp)>0) && ((ii=prop_to_indx(tmp1)) >=0) ) {   ne rabotaet, i=1 vsegda **/

	      if(strlen(tmp1)>0) 
		{
		  ii=prop_to_indx(tmp1);
		  if (ii >= 0)  
		    {
		      
		      if ((pchar=strchr(tmp, ' ')) != NULL) 		  
			{ /* locate blank (after property name) */
			  nm=pchar-tmp; /* blank position(after property name) in tmp */
			  /*   printf("diff2=%d\n",nm);
			   */
			  if(nm>0) {
			    strncpy(tmp1, tmp, nm); /* copy property name */
			    printf("PROPERTY = <%s>\n",tmp1);							   
			    printf("VALS = <%s>\n", &tmp[nm+1]);
			  }
			  
			} else { printf("ERROR NO Values\n"); }
		      
		      if(ch<0 && slot>=0) 
			{ /* only slot number */
			  /*printf("     1 LD S%d %s", slot, tmp);*/
#ifdef VXWORKS

			  printf("PROP=%s index=%d\n",tmp1, ii);
			  err=set_chprop( &tmp[nm+1], crnum, slot, ch, ii); /* set property value for ALL channels */
			  if(err==ERROR) { /* set_chprop() returns error! */
			    printf(" cmdTsk(%d): ERROR LD command: set_chprop(1) returns ERROR !\n", crnum);			
			  }

#endif		    
			}
		      else if(ch>=0 && slot >=0) 
			{ /* slot and channel numbers*/
			  printf("     1 LD S%d.%d %s\n", slot, ch, tmp1);

#ifdef VXWORKS

			  printf("PROP=%s index=%d\n",tmp1, ii);
			  err=set_chprop( &tmp[nm+1], crnum, slot, ch, ii); /* set property value for ONE channel */
			  if(err==ERROR) { /* set_chprop() returns error! */
			    printf(" cmdTsk(%d): ERROR LD command: set_chprop(2) returns ERROR !\n", crnum);
			  }
#endif		    
			}
		      /*		  sprintf(nio_TXbuff,"     1 LD S%d.%d %s", slot, ch, tmp1); */
		      else {
			err=ERROR;
			printf(" cmdTsk(%0d): ERROR LD command: no Slot/Ch number\n", crnum);
			sprintf(nio_TXbuff, "   111 ERROR No Slot/Ch number!");
		      }
		      
		    } 
		  else {
		    err=ERROR;
		    printf(" cmdTsk(%0d): ERROR LD command:  property name=%s  ERROR\n", crnum, tmp1);
		  }	      	    
		  
		}
	      else {	
		err=ERROR;
		printf(" cmdTsk(%0d): ERROR RC command: property ERROR\n", crnum);
		sprintf(nio_TXbuff, "   110 ERROR No property!");
	      }
	      
	      if(err!=ERROR) strcat(nio_TXbuff, nio_RXbuff); /* OK - return input command */

	      else {
		printf(" cmdTsk(%d): ERROR LD command: set_chprop(2) returns ERROR !\n", crnum);
		sprintf(nio_TXbuff, "   110 ERROR Unknown property name!");
	    /*    strcat(nio_TXbuff, nio_RXbuff); */ /* NOTE !!!! Replace this: return input command */
	      };
	    }
	    }

	} /* end LD command */
	  
/*** ___ END_OF_COMMAND _____ *******/        
	  else {
	    strcpy(nio_TXbuff, "   127 ERROR Unknown command");
	    /*  strcpy(nio_TXbuff, "     1 RC S0 DV 0 0 0 0 0 0 0 0 0 0 0 0\r\n");*/
	  }
	 
	/*                else strcpy(nio_TXbuff, nio_RXbuff);*/ /* sent back incomming command */	
	/***            strcat(nio_TXbuff, nio_RXbuff);
			strcpy(nio_TXbuff, nio_RXbuff); **/		

	  nio_TXlen = strlen(nio_TXbuff);
	  /* printf(" cmdTsk: strncmp() got %s\n",nio_RXbuff); */
	        
	/* received command to quit - bail out of this loop */
	if(nio_quit)
	  {
            printf("cmdTSK(%0d) : quit\n",crnum);
            nio_quit = 0;
            return;
	  }
	
	/* send replay back */
	nio_TXlen = strlen(nio_TXbuff)+1; /* plus null terminated byte '\0' */
	if(nio_TXlen<=L1024) {
	  if(send(connection_fd, nio_TXbuff, nio_TXlen, 0) < 0)
	    {
	      printf("cmdTSK(%0d) - error sending message....\n", crnum);
	      {
		perror("send");       
	      }
	      break;
	    }

#ifndef VXWORKS
	  /*          time(&now); 
		      printf(" %s ", ctime(&now));    
	  */
#endif
          printf("cmdTSK(%0d) : sentback: %s\n", crnum, nio_TXbuff);
	} else {
	  printf("cmdTSK(%0d) : ERROR: sending message too long: %d\n", crnum, nio_TXlen);
	}	
      }
    }
  
  printf("cmdTSK(%0d) - exit loop....\n", crnum);

  return;
 }
/* END of cmdTSK() */


/*******************************************************/
/*** MAIN                         ***/
/**********************************/
#ifdef VXWORKS
int start (int port, int crate)
#else
int main (int argc, char *argv[])
#endif
{
  
  struct sockaddr_in serverAddr; 
  struct sockaddr_in clientAddr; 

  int sockAddrSize;              /* size of socket address structure */ 
  int sFd;                       /* socket file descriptor */ 
  int portnum = 1090; /* desired port number; can be changed if that number in use enc */
  int newFd;
  char *targ_address;
  unsigned short targ_port;

  int c_crate=-1;
  char nio_RXbuff[L1024]; /* receive buffer */
  char nio_TXbuff[L1024]; /* transmit buffer */

  struct linger linger;
  linger.l_onoff = 0;   /* was 1 */
  linger.l_linger = 1;  /* was 0 */

#ifdef VXWORKS
  if(port>0)
    portnum=port;
  if(crate>0)
    c_crate=crate;

#else
  if(argc>1){
    sscanf(*(++argv),"%d", &portnum);
    sscanf(*(++argv),"%d", &c_crate);    
  }
#endif  

 printf("Server Port=%0d  Crate=%0d\n", portnum, c_crate);

  /************** server part ************************/

  /*if(server_port>0) portnum = server_port; *//* port number from config file */

  /* some cleanup */
  sockAddrSize = sizeof(struct sockaddr_in); 
  memset((char *)&serverAddr, 0, sockAddrSize); 
  memset((char *)&clientAddr, 0, sockAddrSize); 


  /* creates an endpoint for communication and returns a socket file descriptor */
  if((sFd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
  {
    perror("socket"); 
    return(ERROR); 
  } 

  /* set up the local address */ 
  serverAddr.sin_family = AF_INET; 
  serverAddr.sin_port = htons(portnum); 
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* create a TCP-based socket */ 

  serverAddr.sin_port = htons(portnum);
  if( bind(sFd, (struct sockaddr *)&serverAddr, sockAddrSize)<0) {
      perror ("bind"); 
      close (sFd); 
      return (ERROR);
  }


  printf("bind on port %d\n", portnum);


  if(listen (sFd, 2) == ERROR)
    {
      perror ("listen"); 
      close (sFd); 
      return (ERROR);
    }

  /* prctl(PR_SET_NAME,"caen_server"); *//* set process name */

  while(1)
    {
      
      if((newFd = accept (sFd, (struct sockaddr *) &clientAddr, &sockAddrSize))
         == ERROR)
        {
          perror ("accept"); 
          close (sFd); 
          return (ERROR); 
        }
      
      targ_address = inet_ntoa(clientAddr.sin_addr);

      targ_port = ntohs (clientAddr.sin_port);
      
      printf("\n VME Server(%d): Accept connection from:  IP=>%s<  PORT=%d  socket=%d\n\n",
	     c_crate, targ_address, targ_port, newFd); fflush(stdout);
      
      while(1) {
	cmdTSK(newFd, nio_RXbuff, nio_TXbuff, c_crate);
	break;
      }      
      printf("VME Server(%d): Socket closed...\n", c_crate);
      close(newFd);

    }        

  close(sFd);
  return 0;
}
