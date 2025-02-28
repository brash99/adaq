/*
 * File:
 *    vme_modmem.c
 *
 * Description:
 *    Command line program to modify registers from the vmeBUS.
 *    Much code here shamelessly borrowed from vxWorks
 *
 *    We'll probably release this as binary only.
 *
 * SVN: $Rev: 501 $
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/signal.h>
#include "jvme.h"

#define MAXLINE         16	/* max line length for input to 'm' routine */
#define EOS           '\0'    /* C string terminator */

unsigned int A16MemOffset = 0x0;

void
usage()
{
  printf("Usage: ");
  printf("vme_modmem [OPTION]... [VMEADDRESS]... [WIDTH]\n");
  printf("Access and modify memory from VMEADDRESS with data WIDTH\n");
  printf("\n");
  printf("If WIDTH (in bytes) is not specified, 2 will be assumed\n"); 
  printf("Valid values of WIDTH are 2, 4, and 8 bytes.");
  printf("\n");
  printf("OPTIONs:\n");
  printf("  -a ADDRSPACE                Specify Address Space.  Valid values are\n");
  printf("                                a16, a24, and a32.  If Address space is\n");
  printf("                                not specified, it will be inferred from\n");
  printf("                                VMEADDRESS.\n");
  printf("\n\n");
}

void vmeModMem(volatile void *adrs, int width, unsigned int offset);
STATUS getHex(char *pStr,UINT32 *pHiValue, UINT32 *pLoValue);
void sig_handler(int signo);
void closeup();
int
main(int argc, char *argv[]) 
{
  int iarg=1;
  int stat;
  int nunits=0;
  unsigned int userAddr=0;
  unsigned int userWidth=0;
  unsigned int laddr;
  unsigned short amcode=0;
  unsigned int memOffset;
  char *addrString;

  signal(SIGINT,sig_handler);

  /* Evaluate the command line arguments */
  while (iarg < argc) 
    {
      if (strcmp(argv[iarg],"-a") == 0)  /* Address space specified */
	{
	  if (iarg < argc-1) 
	    {
	      addrString = argv[++iarg];
	      if(strcasecmp(addrString,"a16") == 0)
		amcode = 0x29;
	      else if(strcasecmp(addrString,"a24") == 0)
		amcode = 0x39;
	      else if(strcasecmp(addrString,"a32") == 0)
		amcode = 0x09;
	      else
		{
		  printf("  Invalid address space specified (%s)\n",addrString);
		  usage();
		  return -1;
		}
	      ++iarg;
	      continue;
	    }
	}
      /* otherwise, assume that arguments are <addr> <width> */
      if (iarg == argc-2) 
	{
	  userAddr = strtol(argv[iarg],NULL,16);
	  iarg++;
	  userWidth = atoi(argv[iarg]);
	  break;
	}
      else if (iarg == argc-1)
	{
	  userAddr = strtol(argv[iarg],NULL,16);
	  userWidth = 2;
	  break;
	}
      else
	{
	  usage();
	  return -1;
	}
    }

  if(userAddr==0)
    {
      usage();
      return -1;
    }

  if((userWidth != 2) && (userWidth != 4) && (userWidth != 8) )
    {
      printf("  Invalid data width (%d)\n",userWidth);
      usage();
      return -1;
    }

  if(amcode==0)
    {
      /* Determine the address space from the userAddr */
      if(userAddr<0x10000)
	{
	  amcode = 0x29;
	}
      else if (userAddr<0x01000000)
	{
	  amcode = 0x39;
	}
      else
	{
	  amcode = 0x09;
	}
    }

  /* Initialize the VME bridge... quietly */
  vmeSetQuietFlag(1);
  stat = vmeOpenDefaultWindows();
  if(stat != OK)
    {
      printf("Failed to open default VME windows\n");
      goto CLOSE;
    }
  
  /* Determine the local address */
  stat = vmeBusToLocalAdrs(amcode,(char *)userAddr,(char **)&laddr);
  if (stat != 0) 
    {
      printf("ERROR: Error in vmeBusToLocalAdrs(0x%x,0x%x,&laddr) res=%d \n",
	     amcode,userAddr,stat);
      goto CLOSE;
    } 
  memOffset = laddr - userAddr;

  /* Display the requested memory */
  printf("\n");
  vmeModMem((void *)laddr, userWidth, memOffset);

  /* Clear any Execptions, if they occurred */
  vmeClearException(0);

 CLOSE:
    
  printf("\n");
  closeup();

  return 0;
}

void closeup()
{
  vmeCloseDefaultWindows();
}

void vmeModMem
(
 volatile void *adrs,		/* address to change */
 int  width, 		/* width of unit to be modified (1, 2, 4, 8) */
 unsigned int offset
 )
{
  static void *lastAdrs;	/* last location modified */
  static int  lastWidth = 2;	/* last width - default to 2 */
  char line[MAXLINE + 1];	/* leave room for EOS */
  char *pLine;		/* ptr to current position in line */
  UINT32 hiValue;		/* high part of value found in line */
  UINT32 loValue;		/* low part of value found in line */
  UINT32 lvalue=0;
  UINT16 svalue=0;

  if (adrs != 0)		/* set default address */
    lastAdrs = adrs;

  if (width != 0)		/* check valid width and set the default */
    {
      if (width != 1 && width != 2 && width != 4 && width != 8)
	width = 1;
      lastWidth = width;
    }

  /* round down to appropriate boundary */

  lastAdrs = (void *)((int)lastAdrs & ~(lastWidth - 1));

  for (;; lastAdrs = (void *)((int)lastAdrs + lastWidth))
    {
      /* prompt for substitution according to width */

      switch (lastWidth)
	{
	case 1:
	  printf ("%08x:  %02x-", (int) (lastAdrs-offset), *(u_char *)lastAdrs);
	  break;
	case 2:
	  svalue = *(UINT16 *)lastAdrs;
	  printf ("%08x:  %04x-", (int) (lastAdrs-offset), SSWAP(svalue));
	  break;
	case 4:
	  lvalue = *(UINT32 *)lastAdrs;
	  printf ("%08x:  %08lx-", (int) (lastAdrs-offset), LSWAP(lvalue));
	  break;
	case 8:
#if _BYTE_ORDER==_LITTLE_ENDIAN
	  printf ("%08x:  %08lx%08lx-", (int) (lastAdrs-offset), *((UINT32 *)lastAdrs+1),*(UINT32 *)lastAdrs);
#endif
#if _BYTE_ORDER==_BIG_ENDIAN
	  printf ("%08x:  %08lx%08lx-", (int) (lastAdrs-offset), *(UINT32 *)lastAdrs, *((UINT32 *)lastAdrs+1));
#endif
	  break;
	default:
	  printf ("%08x:  %08x-", (int) (lastAdrs-offset), *(u_char *)lastAdrs);
	  break;
	}

      /* get substitution value:
       *   skip empty lines (CR only);
       *   quit on end of file or invalid input;
       *   otherwise put specified value at address
       */

      if (fgets (line, MAXLINE, stdin) == EOF)
	{
	  break;
	}
/*       if (fioRdString (STD_IN, line, MAXLINE) == EOF) */
/* 	break; */

      line[MAXLINE] = EOS;	/* make sure input line has EOS */

      for (pLine = line; isspace (*(u_char *)pLine); ++pLine)	
	/* skip leading spaces*/
	;

      if (*pLine == EOS)			/* skip field if just CR */
	{
	  continue;
	}

      if (getHex (pLine, &hiValue, &loValue) != OK)
	{
	  break;
	}

      /* assign new value */

      switch (lastWidth)
	{
	case 1:
	  *(u_char *)lastAdrs = (u_char) loValue;
	  break;
	case 2:
	  *(UINT16 *)lastAdrs = (UINT16) SSWAP(loValue);
	  break;
	case 4:
	  *(UINT32 *)lastAdrs = (UINT32) LSWAP(loValue);
	  break;
	case 8:
#if _BYTE_ORDER==_LITTLE_ENDIAN
	  *(UINT32 *)lastAdrs = (UINT32) loValue;
	  *((UINT32 *)lastAdrs+1) = (UINT32) hiValue;
#endif
#if _BYTE_ORDER==_BIG_ENDIAN
	  *(UINT32 *)lastAdrs = (UINT32) hiValue;
	  *((UINT32 *)lastAdrs+1) = (UINT32) loValue;
#endif
	  break;
	default:
	  *(u_char *)lastAdrs = (u_char) loValue;
	  break;
	}
    }

  printf ("\n");
}

STATUS getHex
(
 char *pStr,		/* string to parse */
 UINT32 *pHiValue,	/* where to store high part of result */
 UINT32 *pLoValue	/* where to store low part of result */
 )
{
  int         dig;                    /* current digit */
  BOOL        neg     = FALSE;        /* negative or positive? */
  volatile char * pCh     = pStr;         /* pointer to current character */
  volatile int    ch      = *pCh;         /* current character */
  volatile UINT32  hiValue = 0;		/* high part of value accumulator */
  volatile UINT32  loValue = 0;		/* low part of value accumulator */
 
  /* check for sign */
 
  if (ch == '+' || (neg = (ch == '-')))
    ch = *++pCh;
 
  /* check for optional or 0x */
 
  if (ch == '0')
    {
      ch = *++pCh;
      if (ch == 'x' || ch == 'X')
	ch = *++pCh;
    }
 
  /* scan digits */
  while (ch != '\0')
    {
      if (isdigit (ch))
	dig = ch - '0';
      else if (islower (ch))
	dig = ch - 'a' + 10;
      else if (isupper (ch))
	dig = ch - 'A' + 10;
      else
	  break;

      if (dig >= 16)
	break;
      
      /* assume that accumulator parts are 32 bits long */
      hiValue = (hiValue * 16) + (loValue >> 28);
      loValue = loValue * 16 + dig;
 
      ch = *++pCh;
    }
 
 
  /* check that we scanned at least one character */
 
  if (pCh == pStr) {
    return (ERROR);
  }
 
  /* return value to caller */
 
  if (neg) {
    /* assume 2's complement arithmetic */
    hiValue = ~hiValue;
    loValue = ~loValue;
    if (++loValue == 0)
      hiValue++;
  }

  *pHiValue = hiValue;
  *pLoValue = loValue;

  return OK;

/*   return (ch != '\0' ? ERROR : OK); */
}

void sig_handler(int signo)
{
  int i, status;

  switch (signo) {
  case SIGINT:
    printf("\n\n");
    closeup();
    exit(1);  /* exit if CRTL/C is issued */
  }
  return;
}
