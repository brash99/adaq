/************************************************************************
 * v288.c  - code for board type of                                     *
 *   CAEN V288 CaeNet controller dor HV crate SY527                     *
 *                                                                      *  
 * Author: Roman Pomatsalyuk   TJNAF HallA Moller polarimeter           *
 * Begin history:                                                       *
 * Date  : 03 Mar 2015                                                  *   
 * Modification:                                                        *
 *                                                                      *
 ************************************************************************/

#include        <vxWorks.h>
#include        <types.h>
#include        <vme.h>
#include        <cacheLib.h>
#include        <stdioLib.h>
#include        <wdLib.h>
#include        <logLib.h>
#include        <math.h>
#include        <string.h>
#include        <ioLib.h>
#include        <tyLib.h>
/*#include        <ansiStdio.h>*/

#define ONBOARDADDR 0x00600000 /* define BASE address */

/****************************************************************************/
#define DBG 0 /* for debug goals */
/************************************************************************/

#define Q (ushort)0xfffe

/* Define a Structure for access */
/* all addressing is A24, all data D16 */
typedef struct v288_struct
{
  /*0x0000*/ volatile unsigned short data;   /* data buffer in/out R/W */
  /*0x0002*/ volatile unsigned short status; /* status register R */
  /*0x0004*/ volatile unsigned short send;   /* transmition register W */
  /*0x0006*/ volatile unsigned short reset;  /* reset register W */
  /*0x0008*/ volatile unsigned short intr;   /* interrupt vector register W */

} V288;

V288 *pcr_288;

                                                                                          
/* get bit status(0/1) from registers value */
int gbit(
         unsigned short regval, /* value from register (16bit) */
         unsigned short mask)  /* select bit number 0-15 */
{
  int temp=0;
  if(mask<16) temp = (regval>>mask) & 0x0001;
  else return ERROR;
  return temp;
};


/* map csr of board to VMEbus */
int init_288(int zero)
{
  int              status,i;
  short            readback;
  unsigned short   readv;
  unsigned short   temp;
  
  status=sysBusToLocalAdrs(0x39, ONBOARDADDR , &pcr_288);
  if (status != OK)
    {
      printf("Failed to map VME A24 base address in init_v288() \n");
      return ERROR;
    }
  
  if(vxMemProbe(pcr_288, VX_READ, sizeof(readback), &readback) != OK )
    {
      printf("CSR not found in 'init_v288' \n");
      return ERROR;
    }
  printf("\n V288: onBoard Addr: %x, LocalAdd: %x\n", ONBOARDADDR, pcr_288);
  return OK;
}

/* dump registers to screen */
int dump_288()
{
  unsigned short temp;
  int            i;
                                                                                          
  printf("\nData Register         (Base+%02X) = %04X\n",0x0000, pcr_288->data);
  printf("Status Register       (Base+%02X) = %04X\n",0x0002, pcr_288->status);
  /*  getchar();*/
  return 0;
}

/* get crate name (IDENT) */
int getID(unsigned short crN) {
  int i=0;
  unsigned short rd=0;
  pcr_288->data=1;
  pcr_288->data=crN;
  pcr_288->data=0;
  pcr_288->send=1;
  taskDelay(30);
  for(i=0;i<12;i++) {
    rd=pcr_288->data;
    printf("rd= %d (%c) \n", rd, rd);
    taskDelay(3);
  }    

  if(0) { 
    vmeWrite16(&pcr_288->data, 0x1); /* controller ID=1 */
    vmeWrite16(&pcr_288->data, crN); /* create number   */
    vmeWrite16(&pcr_288->data, 0);   /* code=0 (IDENT)  */
    vmeWrite16(&pcr_288->send, 1);   /* send  */
    taskDelay(30);
    getchar();
    for(i=0;i<12;i++) {
      rd=vmeRead16(&pcr_288->data);
      printf("rd= %d (%c) \n", rd, rd);
      taskDelay(3);
    }
  }    
}
