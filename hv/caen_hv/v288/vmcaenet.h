/***************************************************************************/
/*                                                                         */
/* ------ C . A . E . N . SpA ------                                       */
/*                                                                         */
/* VMCAENET.H - Declarations for communication with V288 Module            */
/*                                                                         */
/* 30-Mar-2015 R.P. modified (a lot) for sy527.c to use with Java GUI      */
/***************************************************************************/
#ifndef uchar 
#define uchar unsigned char
#endif

#ifndef ushort
#define ushort unsigned short
#endif 

#ifndef ulong
#define ulong unsigned long
#endif 

/* Constants for vme_cycles routines */
#define BYTE 1
#define WORD 2
#define LWORD 4

/*
Errors returned by caenet_read and caenet_write; the positive ones
are depending from V288 Module and not from CAENET network
*/
#define TUTTOK 0
#define E_NO_Q_IDENT 1
#define E_NO_Q_CRATE 2
#define E_NO_Q_CODE 3
#define E_NO_Q_DATA 4
#define E_NO_Q_TX 5
#define E_NO_Q_RX 6
#define E_LESSDATA 7
#define E_BUSERR 8

/* Number of iterations before deciding that V288 does not answer */
#define TIMEOUT -1
#define Q (ushort)0xfffe
#define V288 1

/* Registers of V288 Module */
#define STATUS (v288addr+0x02)
#define TXMIT (v288addr+0x04)
#define LOBYTE(x) (uchar)((x)&0xff)
#define HIBYTE(x) (uchar)(((x)&0xff00) >> 8)

#define BASE_ADDR 0e600000


/*
Interface between the user program and V288; these functions are defined
in file Vmcaenet.c
*/
int caenet_read();
int caenet_write();
int read_caenet_buffer();

/*
  The following structure contains all the useful information about
  the settings of a channel
*/

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

/*
  The following structure contains all the useful information about
  the status of a channel
*/
/*
struct hvrd
{
  long vread;
  ushort status;
  short iread;

};
*/
struct hvrd
{
 long   vread;
 short  hvmax;
 short  iread;
 unsigned short status;
};


/*
The following structure contains all the useful information about the
characteristics of every board
*/
struct board
{
  char   name[5];
  char   curr_mis;
  ushort sernum;
  char   vermaior;
  char   verminor;
  char   reserved[20];
  uchar  numch;
  ulong  dummy;
  long   vmax;
  short  imax;  
  short  rmin; 
  short  rmax;
  short  resv, resi, decv, deci;
  uchar  dummy1;
};


/* PS numbers for properties */
struct ps_numbers {
  int prop_num; /* number of properties */
  unsigned int psn[15]; /* Property summary numbers  */
  ULONG time; /* time (tickGet()) of last changing psn[] */
};

struct chan_set {
  struct hvrd read[25];
  struct hvch set[25];
};

/*
  The following structure contains all the useful information about
  the status of a one crate
*/
struct ccrate
{
  int number;               /* crate number */
  int bd_num;               /* number of inserted boards */
  unsigned short config;    /* crate occupation (by bits set) */
  char ident[12];           /* name, version */
  unsigned short connected; /* connected = 1, or not = -1) */
  struct board brds[10];    /* boards parameters */
  struct chan_set bch[10];   /* channel parameters (in struct hvrd, struct hvch */ 
/* 
   Summary numbers used for update data from 
   crate by client program.
*/
	  /** gsn - global summary number: 5 words( 4-digits hex format) 
	  *   1 - measured value summary number (incrementd each time a measaured value in any module is changed)
	  *   2 - settable value summary number (incrementd each time a settable value in any module is changed)
	  *   3 - configuration summary number (incrementd each time a configuration value of crate is changed: 
	  *                                     power on/off, front keys, limits, interlocks, ...)
	  *   4 - crate activity summary number (incremented each time when words 1 and 2 are updated)
	      5 - host activity summary number (incremented on each processed GS command)
	  **/
	  /** lsn - logical summary number 
	  *   2 hex words 4-digit for each module(unit) in crate: 
	  *   1 - measured value summary number for unit
	  *   2 - settable value summary number for unit 
	  **/
          /** psn - property summary numbers, series of 4 hex digits one for each propery **/

  ULONG gs_time; /* time in ticks from last GS numbers update */
  ULONG ps_time; /* time in ticks from last PS numbers update */
  unsigned short next_bd; /* store board number for properties MC, MV  updates */
  unsigned int gsn[5];  /* Global summary numbers */
  unsigned int lsn[20]; /* Logical summary numbers */
  struct ps_numbers ps[10];  /* Property summary numbers for every board */
};

/* Declarations of Global Variables defined in the user program */
/* extern unsigned v288addr,craten;
   extern ushort code; */

