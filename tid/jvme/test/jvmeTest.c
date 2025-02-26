/*
 * File:
 *    jvmeTest.c
 *
 * Description:
 *    Test JLab Vme Driver
 *
 *
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gef/gefcmn_osa.h"
#include "gef/gefcmn_vme_tempe.h"
#include "jvme.h"

extern GEF_UINT32 jlabgefReadRegister(GEF_UINT32 offset);
extern GEF_UINT32 jlabgefReadRegister2(GEF_UINT32 offset);

int 
main(int argc, char *argv[]) {

  int stat, iaddr;
    unsigned int reg_veat=0;
    unsigned int laddr=0;
    unsigned int base = 0x00;
    unsigned int offset=0;

    vmeOpenDefaultWindows();

    stat = vmeBusToLocalAdrs(0x39, (char *)((21<<19)), (char **)&laddr);
    if(stat==OK)
      {
	printf("0x%08x \n",vmeRead32((volatile unsigned int *)laddr+0x0));
      }
    vmeCloseDefaultWindows();

    exit(0);
}

