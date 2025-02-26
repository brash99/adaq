/* # 1 "newele_test.c" */
/* # 1 "/site/coda/2.6.2/common/include/rol.h" 1 */
typedef void (*VOIDFUNCPTR) ();
typedef int (*FUNCPTR) ();
typedef unsigned long time_t;
typedef struct semaphore *SEM_ID;
static void __download ();
static void __prestart ();
static void __end ();
static void __pause ();
static void __go ();
static void __done ();
static void __status ();
static int theIntHandler ();
/* # 1 "/site/coda/2.6.2/common/include/libpart.h" 1 */
/* # 1 "/site/coda/2.6.2/common/include/mempart.h" 1 */
typedef struct danode			       
{
  struct danode         *n;	               
  struct danode         *p;	               
  struct rol_mem_part   *part;	                   
  int                    fd;		       
  char                  *current;	       
  unsigned long          left;	               
  unsigned long          type;                 
  unsigned long          source;               
  void                   (*reader)();          
  long                   nevent;               
  unsigned long          length;	       
  unsigned long          data[1];	       
} DANODE;
typedef struct alist			       
{
  DANODE        *f;		               
  DANODE        *l;		               
  int            c;			       
  int            to;
  void          (*add_cmd)(struct alist *li);      
  void          *clientData;                 
} DALIST;
typedef struct rol_mem_part *ROL_MEM_ID;
typedef struct rol_mem_part{
    DANODE	 node;		 
    DALIST	 list;		 
    char	 name[40];	 
    void         (*free_cmd)();  
    void         *clientData;     
    int		 size;		 
    int		 incr;		 
    int		 total;		 
    long         part[1];	 
} ROL_MEM_PART;
/* # 62 "/site/coda/2.6.2/common/include/mempart.h" */
/* # 79 "/site/coda/2.6.2/common/include/mempart.h" */
/* # 106 "/site/coda/2.6.2/common/include/mempart.h" */
extern void partFreeAll();  
extern ROL_MEM_ID partCreate (char *name, int size, int c, int incr);
/* # 21 "/site/coda/2.6.2/common/include/libpart.h" 2 */
/* # 67 "/site/coda/2.6.2/common/include/rol.h" 2 */
/* # 1 "/site/coda/2.6.2/common/include/rolInt.h" 1 */
typedef struct rolParameters *rolParam;
typedef struct rolParameters
  {
    char          *name;	      
    char          tclName[20];	       
    char          *listName;	      
    int            runType;	      
    int            runNumber;	      
    VOIDFUNCPTR    rol_code;	      
    int            daproc;	      
    void          *id;		      
    int            nounload;	      
    int            inited;	      
    long          *dabufp;	      
    long          *dabufpi;	      
    ROL_MEM_PART  *pool;              
    ROL_MEM_PART  *output;	      
    ROL_MEM_PART  *input;             
    ROL_MEM_PART  *dispatch;          
    volatile ROL_MEM_PART  *dispQ;    
    unsigned long  recNb;	      
    unsigned long *nevents;           
    int           *async_roc;         
    char          *usrString;	      
    void          *private;	      
    int            pid;               
    int            poll;              
    int primary;		      
    int doDone;			      
  } ROLPARAMS;
/* # 70 "/site/coda/2.6.2/common/include/rol.h" 2 */
extern ROLPARAMS rolP;
static rolParam rol;
extern int global_env[];
extern long global_env_depth;
extern char *global_routine[100];
extern long data_tx_mode;
extern int cacheInvalidate();
extern int cacheFlush();
static int syncFlag;
static int lateFail;
/* # 1 "/site/coda/2.6.2/common/include/BankTools.h" 1 */
static int EVENT_type;
long *StartOfEvent[32 ],event_depth__, *StartOfUEvent;
/* # 78 "/site/coda/2.6.2/common/include/BankTools.h" */
/* # 95 "/site/coda/2.6.2/common/include/BankTools.h" */
/* # 107 "/site/coda/2.6.2/common/include/BankTools.h" */
/* # 120 "/site/coda/2.6.2/common/include/BankTools.h" */
/* # 152 "/site/coda/2.6.2/common/include/BankTools.h" */
/* # 186 "/site/coda/2.6.2/common/include/BankTools.h" */
/* # 198 "/site/coda/2.6.2/common/include/BankTools.h" */
/* # 217 "/site/coda/2.6.2/common/include/BankTools.h" */
/* # 92 "/site/coda/2.6.2/common/include/rol.h" 2 */
/* # 1 "/site/coda/2.6.2/common/include/trigger_dispatch.h" 1 */
static unsigned char dispatch_busy; 
static int intLockKey,trigId;
static int poolEmpty;
static unsigned long theEvMask, currEvMask, currType, evMasks[16 ];
static VOIDFUNCPTR wrapperGenerator;
static FUNCPTR trigRtns[32 ], syncTRtns[32 ], doneRtns[32 ], ttypeRtns[32 ];
static unsigned long Tcode[32 ];
static DANODE *__the_event__, *input_event__, *__user_event__;
/* # 50 "/site/coda/2.6.2/common/include/trigger_dispatch.h" */
/* # 141 "/site/coda/2.6.2/common/include/trigger_dispatch.h" */
static void cdodispatch()
{
  unsigned long theType,theSource;
  int ix, go_on;
  DANODE *theNode;
  dispatch_busy = 1;
  go_on = 1;
  while ((rol->dispQ->list.c) && (go_on)) {
{ ( theNode ) = 0; if (( &rol->dispQ->list )->c){ ( &rol->dispQ->list )->c--; ( theNode ) = ( 
&rol->dispQ->list )->f; ( &rol->dispQ->list )->f = ( &rol->dispQ->list )->f->n; }; if (!( &rol->dispQ->list 
)->c) { ( &rol->dispQ->list )->l = 0; }} ;     theType = theNode->type;
    theSource = theNode->source;
    if (theEvMask) { 
      if ((theEvMask & (1<<theSource)) && (theType == currType)) {
	theEvMask = theEvMask & ~(1<<theSource);
	intUnlock(intLockKey); ;
	(*theNode->reader)(theType, Tcode[theSource]);
	intLockKey = intLock(); ;
	if (theNode)
	if (!theEvMask) {
	 if (wrapperGenerator) {event_depth__--; *StartOfEvent[event_depth__] = (long) (((char *) 
(rol->dabufp)) - ((char *) StartOfEvent[event_depth__]));	if ((*StartOfEvent[event_depth__] & 1) != 0) { 
(rol->dabufp) = ((long *)((char *) (rol->dabufp))+1); *StartOfEvent[event_depth__] += 1; }; if 
((*StartOfEvent[event_depth__] & 2) !=0) { *StartOfEvent[event_depth__] = *StartOfEvent[event_depth__] + 2; (rol->dabufp) = 
((long *)((short *) (rol->dabufp))+1);; };	*StartOfEvent[event_depth__] = ( 
(*StartOfEvent[event_depth__]) >> 2) - 1;}; ;	 	 rol->dabufp = ((void *) 0) ; if (__the_event__) { if (rol->output) { {if(! ( 
&(rol->output->list) )->c ){( &(rol->output->list) )->f = ( &(rol->output->list) )->l = ( __the_event__ );( 
__the_event__ )->p = 0;} else {( __the_event__ )->p = ( &(rol->output->list) )->l;( &(rol->output->list) 
)->l->n = ( __the_event__ );( &(rol->output->list) )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( 
&(rol->output->list) )->c++;	if(( &(rol->output->list) )->add_cmd != ((void *) 0) ) (*(( &(rol->output->list) 
)->add_cmd)) (( &(rol->output->list) )); } ; } else { { if (( __the_event__ )->part == 0) { free( __the_event__ 
); __the_event__ = 0; } else { {if(! ( & __the_event__ ->part->list )->c ){( & __the_event__ 
->part->list )->f = ( & __the_event__ ->part->list )->l = ( __the_event__ );( __the_event__ )->p = 0;} else {( 
__the_event__ )->p = ( & __the_event__ ->part->list )->l;( & __the_event__ ->part->list )->l->n = ( 
__the_event__ );( & __the_event__ ->part->list )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( & 
__the_event__ ->part->list )->c++;	if(( & __the_event__ ->part->list )->add_cmd != ((void *) 0) ) (*(( & 
__the_event__ ->part->list )->add_cmd)) (( & __the_event__ ->part->list )); } ; } if( __the_event__ 
->part->free_cmd != ((void *) 0) ) { (*( __the_event__ ->part->free_cmd)) ( __the_event__ ->part->clientData); 
} } ;	} __the_event__ = (DANODE *) 0; } if(input_event__) { { if (( input_event__ )->part == 0) { 
free( input_event__ ); input_event__ = 0; } else { {if(! ( & input_event__ ->part->list )->c ){( & 
input_event__ ->part->list )->f = ( & input_event__ ->part->list )->l = ( input_event__ );( input_event__ 
)->p = 0;} else {( input_event__ )->p = ( & input_event__ ->part->list )->l;( & input_event__ 
->part->list )->l->n = ( input_event__ );( & input_event__ ->part->list )->l = ( input_event__ );} ( 
input_event__ )->n = 0;( & input_event__ ->part->list )->c++;	if(( & input_event__ ->part->list )->add_cmd 
!= ((void *) 0) ) (*(( & input_event__ ->part->list )->add_cmd)) (( & input_event__ ->part->list 
)); } ; } if( input_event__ ->part->free_cmd != ((void *) 0) ) { (*( input_event__ 
->part->free_cmd)) ( input_event__ ->part->clientData); } } ; input_event__ = (DANODE *) 0; } {int ix; if 
(currEvMask) {	for (ix=0; ix < trigId; ix++) {	if (currEvMask & (1<<ix)) (*doneRtns[ix])(); } if 
(rol->pool->list.c) { currEvMask = 0; __done(); rol->doDone = 0; } else { poolEmpty = 1; rol->doDone = 1; } } } ; 	}
      } else {
	{if(! ( &rol->dispQ->list )->c ){( &rol->dispQ->list )->f = ( &rol->dispQ->list )->l = ( 
theNode );( theNode )->p = 0;} else {( theNode )->p = ( &rol->dispQ->list )->l;( &rol->dispQ->list 
)->l->n = ( theNode );( &rol->dispQ->list )->l = ( theNode );} ( theNode )->n = 0;( &rol->dispQ->list 
)->c++;	if(( &rol->dispQ->list )->add_cmd != ((void *) 0) ) (*(( &rol->dispQ->list )->add_cmd)) (( 
&rol->dispQ->list )); } ; 	go_on = 0;
      }
    } else { 
      if ((1<<theSource) & evMasks[theType]) {
	currEvMask = theEvMask = evMasks[theType];
	currType = theType;
      } else {
        currEvMask = (1<<theSource);
      }
      if (wrapperGenerator) {
	(*wrapperGenerator)(theType);
      }
      (*(rol->nevents))++;
      intUnlock(intLockKey); ;
      (*theNode->reader)(theType, Tcode[theSource]);
      intLockKey = intLock(); ;
      if (theNode)
	{ if (( theNode )->part == 0) { free( theNode ); theNode = 0; } else { {if(! ( & theNode ->part->list 
)->c ){( & theNode ->part->list )->f = ( & theNode ->part->list )->l = ( theNode );( theNode )->p = 0;} 
else {( theNode )->p = ( & theNode ->part->list )->l;( & theNode ->part->list )->l->n = ( theNode );( & 
theNode ->part->list )->l = ( theNode );} ( theNode )->n = 0;( & theNode ->part->list )->c++;	if(( & 
theNode ->part->list )->add_cmd != ((void *) 0) ) (*(( & theNode ->part->list )->add_cmd)) (( & theNode 
->part->list )); } ; } if( theNode ->part->free_cmd != ((void *) 0) ) { (*( theNode ->part->free_cmd)) ( theNode 
->part->clientData); } } ;       if (theEvMask) {
	theEvMask = theEvMask & ~(1<<theSource);
      } 
      if (!theEvMask) {
	rol->dabufp = ((void *) 0) ; if (__the_event__) { if (rol->output) { {if(! ( 
&(rol->output->list) )->c ){( &(rol->output->list) )->f = ( &(rol->output->list) )->l = ( __the_event__ );( 
__the_event__ )->p = 0;} else {( __the_event__ )->p = ( &(rol->output->list) )->l;( &(rol->output->list) 
)->l->n = ( __the_event__ );( &(rol->output->list) )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( 
&(rol->output->list) )->c++;	if(( &(rol->output->list) )->add_cmd != ((void *) 0) ) (*(( &(rol->output->list) 
)->add_cmd)) (( &(rol->output->list) )); } ; } else { { if (( __the_event__ )->part == 0) { free( __the_event__ 
); __the_event__ = 0; } else { {if(! ( & __the_event__ ->part->list )->c ){( & __the_event__ 
->part->list )->f = ( & __the_event__ ->part->list )->l = ( __the_event__ );( __the_event__ )->p = 0;} else {( 
__the_event__ )->p = ( & __the_event__ ->part->list )->l;( & __the_event__ ->part->list )->l->n = ( 
__the_event__ );( & __the_event__ ->part->list )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( & 
__the_event__ ->part->list )->c++;	if(( & __the_event__ ->part->list )->add_cmd != ((void *) 0) ) (*(( & 
__the_event__ ->part->list )->add_cmd)) (( & __the_event__ ->part->list )); } ; } if( __the_event__ 
->part->free_cmd != ((void *) 0) ) { (*( __the_event__ ->part->free_cmd)) ( __the_event__ ->part->clientData); 
} } ;	} __the_event__ = (DANODE *) 0; } if(input_event__) { { if (( input_event__ )->part == 0) { 
free( input_event__ ); input_event__ = 0; } else { {if(! ( & input_event__ ->part->list )->c ){( & 
input_event__ ->part->list )->f = ( & input_event__ ->part->list )->l = ( input_event__ );( input_event__ 
)->p = 0;} else {( input_event__ )->p = ( & input_event__ ->part->list )->l;( & input_event__ 
->part->list )->l->n = ( input_event__ );( & input_event__ ->part->list )->l = ( input_event__ );} ( 
input_event__ )->n = 0;( & input_event__ ->part->list )->c++;	if(( & input_event__ ->part->list )->add_cmd 
!= ((void *) 0) ) (*(( & input_event__ ->part->list )->add_cmd)) (( & input_event__ ->part->list 
)); } ; } if( input_event__ ->part->free_cmd != ((void *) 0) ) { (*( input_event__ 
->part->free_cmd)) ( input_event__ ->part->clientData); } } ; input_event__ = (DANODE *) 0; } {int ix; if 
(currEvMask) {	for (ix=0; ix < trigId; ix++) {	if (currEvMask & (1<<ix)) (*doneRtns[ix])(); } if 
(rol->pool->list.c) { currEvMask = 0; __done(); rol->doDone = 0; } else { poolEmpty = 1; rol->doDone = 1; } } } ;       }
    }  
  }
  dispatch_busy = 0;
}
static int theIntHandler(int theSource)
{
  if (theSource == 0) return(0);
  {  
    DANODE *theNode;
    intLockKey = intLock(); ;
{{ ( theNode ) = 0; if (( &( rol->dispatch ->list) )->c){ ( &( rol->dispatch ->list) )->c--; ( 
theNode ) = ( &( rol->dispatch ->list) )->f; ( &( rol->dispatch ->list) )->f = ( &( rol->dispatch ->list) 
)->f->n; }; if (!( &( rol->dispatch ->list) )->c) { ( &( rol->dispatch ->list) )->l = 0; }} ;} ;     theNode->source = theSource;
    theNode->type = (*ttypeRtns[theSource])(Tcode[theSource]);
    theNode->reader = trigRtns[theSource]; 
{if(! ( &rol->dispQ->list )->c ){( &rol->dispQ->list )->f = ( &rol->dispQ->list )->l = ( 
theNode );( theNode )->p = 0;} else {( theNode )->p = ( &rol->dispQ->list )->l;( &rol->dispQ->list 
)->l->n = ( theNode );( &rol->dispQ->list )->l = ( theNode );} ( theNode )->n = 0;( &rol->dispQ->list 
)->c++;	if(( &rol->dispQ->list )->add_cmd != ((void *) 0) ) (*(( &rol->dispQ->list )->add_cmd)) (( 
&rol->dispQ->list )); } ;     if (!dispatch_busy)
      cdodispatch();
    intUnlock(intLockKey); ;
  }
}
static int cdopolldispatch()
{
  unsigned long theSource, theType;
  int stat = 0;
  DANODE *theNode;
  if (!poolEmpty) {
    for (theSource=1;theSource<trigId;theSource++){
      if (syncTRtns[theSource]){
	if ( theNode = (*syncTRtns[theSource])(Tcode[theSource])) {
	  stat = 1;
	  {  
	    intLockKey = intLock(); ;
	    if (theNode) 
	 {{ ( theNode ) = 0; if (( &( rol->dispatch ->list) )->c){ ( &( rol->dispatch ->list) )->c--; ( 
theNode ) = ( &( rol->dispatch ->list) )->f; ( &( rol->dispatch ->list) )->f = ( &( rol->dispatch ->list) 
)->f->n; }; if (!( &( rol->dispatch ->list) )->c) { ( &( rol->dispatch ->list) )->l = 0; }} ;} ; 	    theNode->source = theSource; 
	    theNode->type = (*ttypeRtns[theSource])(Tcode[theSource]); 
	    theNode->reader = trigRtns[theSource]; 
	 {if(! ( &rol->dispQ->list )->c ){( &rol->dispQ->list )->f = ( &rol->dispQ->list )->l = ( 
theNode );( theNode )->p = 0;} else {( theNode )->p = ( &rol->dispQ->list )->l;( &rol->dispQ->list 
)->l->n = ( theNode );( &rol->dispQ->list )->l = ( theNode );} ( theNode )->n = 0;( &rol->dispQ->list 
)->c++;	if(( &rol->dispQ->list )->add_cmd != ((void *) 0) ) (*(( &rol->dispQ->list )->add_cmd)) (( 
&rol->dispQ->list )); } ; 	    if (!dispatch_busy) 
	      cdodispatch();
	    intUnlock(intLockKey); ;
	  }
	}
      }
    }   
  } else {
    stat = -1;
  }
  return (stat);
}
/* # 99 "/site/coda/2.6.2/common/include/rol.h" 2 */
static char rol_name__[40];
static char temp_string__[132];
static void __poll()
{
    {cdopolldispatch();} ;
}
void newele_test__init (rolp)
     rolParam rolp;
{
      if ((rolp->daproc != 7 )&&(rolp->daproc != 6 )) 
	printf("rolp->daproc = %d\n",rolp->daproc);
      switch(rolp->daproc) {
      case 0 :
	{
	  char name[40];
	  rol = rolp;
	  rolp->inited = 1;
	  strcpy(rol_name__, "VME1" );
	  rolp->listName = rol_name__;
	  printf("Init - Initializing new rol structures for %s\n",rol_name__);
	  strcpy(name, rolp->listName);
	  strcat(name, ":pool");
	  rolp->pool  = partCreate(name, 1024  , 500 ,1);
	  if (rolp->pool == 0) {
	    rolp->inited = -1;
	    break;
	  }
	  strcpy(name, rolp->listName);
	  strcat(name, ":dispatch");
	  rolp->dispatch  = partCreate(name, 0, 32, 0);
	  if (rolp->dispatch == 0) {
	    rolp->inited = -1;
	    break;
	  }
	  strcpy(name, rolp->listName);
	  strcat(name, ":dispQ");
	  rolp->dispQ = partCreate(name, 0, 0, 0);
	  if (rolp->dispQ == 0) {
	    rolp->inited = -1;
	    break;
	  }
	  rolp->inited = 1;
	  printf("Init - Done\n");
	  break;
	}
      case 9 :
	  rolp->inited = 0;
	break;
      case 1 :
	__download();
	break;
      case 2 :
	__prestart();
	break;
      case 4 :
	__pause();
	break;
      case 3 :
	__end();
	break;
      case 5 :
	__go();
	break;
      case 6 :
	__poll();
	break;
      case 7 :
	__done();
	break;
      default:
	printf("WARN: unsupported rol action = %d\n",rolp->daproc);
	break;
      }
}
/* # 6 "newele_test.c" 2 */
/* # 1 "/site/coda/2.6.2/common/include/VME_source.h" 1 */
static int VME_handlers,VMEflag;
static int VME_isAsync;
static unsigned long VME_prescale = 1;
static unsigned long VME_count = 0;
extern int sysBusToLocalAdrs(int space,long *localA,long **busA);
volatile struct vme_ts {
    volatile unsigned long csr;       
    volatile unsigned long csr2;       
    volatile unsigned long trig;
    volatile unsigned long roc;
    volatile unsigned long sync;
    volatile unsigned long trigCount;
    volatile unsigned long trigData;
    volatile unsigned long lrocData;
    volatile unsigned long prescale[8];
    volatile unsigned long timer[5];
    volatile unsigned long intVec;
    volatile unsigned long rocBufStatus;
    volatile unsigned long lrocBufStatus;
    volatile unsigned long rocAckStatus;
    volatile unsigned long userData1;
    volatile unsigned long userData2;
    volatile unsigned long state;
    volatile unsigned long test;
    volatile unsigned long blank1;
    volatile unsigned long scalAssign;
    volatile unsigned long scalControl;
    volatile unsigned long scaler[18];
    volatile unsigned long scalEvent;
    volatile unsigned long scalLive1;
    volatile unsigned long scalLive2;
    volatile unsigned long id;
  } VME_TS;
struct vme_tir {
    unsigned short tir_csr;
    unsigned short tir_vec;
    unsigned short tir_dat;
    unsigned short tir_oport;
    unsigned short tir_iport;
  };
struct vme_scal {
    unsigned short reset;
    unsigned short blank1[7];
    unsigned short bim[8];
    unsigned short blank2[16];
    unsigned long preset[16];
    unsigned long scaler[16];
    unsigned short blank3[29];
    unsigned short id[3];
  };
volatile struct vme_ts  *ts;
struct vme_tir *tir[2];
struct vme_scal *vscal[32];
struct vme_scal *vlscal[32];
volatile unsigned long *tsmem;
unsigned long ts_memory[4096];
/* # 1 "/site/coda/2.6.2/common/include/tsUtil.h" 1 */
volatile struct vme_ts2 {
    volatile unsigned int csr;       
    volatile unsigned int csr2;       
    volatile unsigned int trig;
    volatile unsigned int roc;
    volatile unsigned int sync;
    volatile unsigned int trigCount;
    volatile unsigned int trigData;
    volatile unsigned int lrocData;
    volatile unsigned int prescale[8];
    volatile unsigned int timer[5];
    volatile unsigned int intVec;
    volatile unsigned int rocBufStatus;
    volatile unsigned int lrocBufStatus;
    volatile unsigned int rocAckStatus;
    volatile unsigned int userData[2];
    volatile unsigned int state;
    volatile unsigned int test;
    volatile unsigned int blank1;
    volatile unsigned int scalAssign;
    volatile unsigned int scalControl;
    volatile unsigned int scaler[18];
    volatile unsigned int scalEvent;
    volatile unsigned int scalLive[2];
    volatile unsigned int id;
  } VME_TS2;
struct ts_state {
  char go[12];
  char trigger[12];
  int ready;
  int busy;
  int seq;
  int sync;
  int feb;
  int inhibit;
  int clear;
  int l1e;
  char buffers[8];
  int branch[5];
  int strobe[5];
  int ack[5];
};    
int tsInit(unsigned int addr, int iflag);
int tsMemInit();
int tsLive(int sflag);
void tsLiveClear();
unsigned int tsCsr(unsigned int Val);
unsigned int tsCsr2Set(unsigned int cval);
unsigned int tsCsr2Clear(unsigned int cval);
void tsClearStatus();
unsigned int tsEnableInput(unsigned int Val, int tflag);
unsigned int tsRoc(unsigned char b4,unsigned char b3,unsigned char b2,unsigned char b1); unsigned int tsSync(unsigned int Val);
unsigned int tsPrescale(int Num, unsigned int Val);
unsigned int tsTimerWrite(int Num, unsigned int Val);
unsigned int tsMemWrite(int Num, unsigned int Val);
void tsGo(int iflag);
void tsStop(int iflag);
void tsReset(int iflag);
unsigned int tsStatus (int iflag);
void tsTimerStatus();
unsigned int tsState(int iflag);
unsigned int tsScalRead(int Num, int sflag);
void tsScalClear(unsigned int mask);
void tsScalLatch();
void tsScalClearAll();
/* # 125 "/site/coda/2.6.2/common/include/VME_source.h" 2 */
extern struct vme_ts2 *tsP;
/* # 1 "/site/coda/2.6.2/common/include/vme_triglib.h" 1 */
unsigned int tirVersion = 0;  
int
tirInit(unsigned int tAddr)
{
  int stat = 0;
  unsigned long laddr;
  unsigned short rval;
  if (tAddr == 0) {
    tAddr = 0x0ed0 ;
  }
  stat = sysBusToLocalAdrs(0x29,tAddr,&laddr);
  if (stat != 0) {
     printf("tirInit: ERROR: Error in sysBusToLocalAdrs res=%d \n",stat);
  } else {
     printf("TIR address = 0x%x\n",laddr);
     tir[1] = (struct vme_tir *)laddr;
  }
  stat = vxMemProbe((char *)laddr,0,2,(char *)&rval);
  if (stat != 0) {
    printf("tirInit: ERROR: TIR card not addressable\n");
    return(-1);
  } else {
    tir[1]->tir_csr = 0x80;  
    tirVersion = (unsigned int)rval;
    printf("tirInit: tirVersion = 0x%x\n",tirVersion);
  }
  return(0);
}
static inline void 
vmetriglink(int code, VOIDFUNCPTR isr)
{
  switch (code) {
  case 1 :
    if(tir[1]) {
      tir[1]->tir_csr = 0x80;
      tir[1]->tir_vec = 0xec ;
    }else{
      printf(" vmetriglink: ERROR: tir[1] uninitialized\n");
      return;
    }
    break;
  case 2 :
    if(tsP) {
      tsP->intVec = 0xec ;
    }else{
      printf(" vmetriglink: ERROR: ts uninitialized\n");
      return;
    }    
    break;
  default:
    printf(" vmetriglink: ERROR: source type %d undefined\n",code);
    return;
  }
  if((intDisconnect(((VOIDFUNCPTR *) ( 0xec  )) ) !=0))
     printf("Error disconnecting Interrupt\n");
  intConnect(((VOIDFUNCPTR *) ( 0xec  )) ,isr,0);
}
static inline void 
vmetenable(int code, unsigned int intMask)
{
 int lock_key;
  lock_key = intLock();
 if(code == 1 )
   sysIntEnable(5 );
 if(code == 2 ) {
   tsP->intVec = 0xec ;
   sysIntEnable(5 );
 }
  intEnable(11);          
 if(code == 1 )
   tir[1]->tir_csr = 0x7;
 if(code == 2 )
   tsP->csr2 |= 0x0000;
  intUnlock(lock_key);
/* # 189 "/site/coda/2.6.2/common/include/vme_triglib.h" */
}
static inline void 
vmetdisable(int code,unsigned int intMask)
{
 if(code == 1 )
   tir[1]->tir_csr = 0x80;
 if(code == 2 )
   tsP->csr2 &= ~(0x1800);
}
static inline void 
vmetack(int code, unsigned int intMask)
{
 if(code == 1 )
   tir[1]->tir_dat = 0x8000;
 if(code == 2 )
   tsP->lrocBufStatus = 0x100;
}
static inline unsigned long 
vmettype(int code)
{
 unsigned long tt;
/* # 229 "/site/coda/2.6.2/common/include/vme_triglib.h" */
  tt=1;
  return(tt);
}
static inline int 
vmettest(int code)
{
  unsigned short sval=0;
  unsigned int   lval=0;
 if(code == 1 ) {
   sval = tir[1]->tir_csr;
   if( (sval != 0xffff) && ((sval&0x8000) != 0) ) {
     return (1);
   } else {
     return (0);
   }
 }
 if(code == 2 ) {
   lval = tsP->lrocBufStatus;
   if( (lval != 0xffffffff) && ((lval&0x8000) != 0) ) {
     return (1);
   } else {
     return (0);
   }
 }
  return(0);
}
/* # 129 "/site/coda/2.6.2/common/include/VME_source.h" 2 */
void VME_int_handler()
{
  theIntHandler(VME_handlers);                    
}
/* # 7 "newele_test.c" 2 */
/* # 1 "c775Lib.h" 1 */
struct c775_struct {
  volatile unsigned long  data[512];
  unsigned long blank1[512];
  volatile unsigned short rev;
  volatile unsigned short geoAddr;
  volatile unsigned short cbltAddr;
  volatile unsigned short bitSet1;
  volatile unsigned short bitClear1;
  volatile unsigned short intLevel;
  volatile unsigned short intVector;
  volatile unsigned short status1;
  volatile unsigned short control1;
  volatile unsigned short aderHigh;
  volatile unsigned short aderLow;
  volatile unsigned short ssReset;
  unsigned short blank2;
  volatile unsigned short cbltControl;
  unsigned short blank3[2];
  volatile unsigned short evTrigger;
  volatile unsigned short status2;
  volatile unsigned short evCountL;
  volatile unsigned short evCountH;
  volatile unsigned short incrEvent;
  volatile unsigned short incrOffset;
  volatile unsigned short loadTest;
  unsigned short blank4;
  volatile unsigned short fclrWindow;
  volatile unsigned short bitSet2;
  volatile unsigned short bitClear2;
  volatile unsigned short wMemTestAddr;
  volatile unsigned short memTestWordH;
  volatile unsigned short memTestWordL;
  volatile unsigned short crateSelect;
  volatile unsigned short testEvWrite;
  volatile unsigned short evCountReset;
  unsigned short blank5[15];
  volatile unsigned short iped;
  unsigned short blank6;
  volatile unsigned short rTestAddr;
  unsigned short blank7;
  volatile unsigned short swComm;
  volatile unsigned short slideConst;
  unsigned short blank8[2];
  volatile unsigned short AAD;
  volatile unsigned short BAD;
  unsigned short blank9[6];
  volatile unsigned short threshold[32 ];
};
struct c775_ROM_struct {
  volatile unsigned short OUI_3;
  unsigned short blank1;
  volatile unsigned short OUI_2;
  unsigned short blank2;
  volatile unsigned short OUI_1;
  unsigned short blank3;
  volatile unsigned short version;
  unsigned short blank4;
  volatile unsigned short ID_3;
  unsigned short blank5;
  volatile unsigned short ID_2;
  unsigned short blank6;
  volatile unsigned short ID_1;
  unsigned short blank7[7];
  volatile unsigned short revision;
};
/* # 20 "newele_test.c" 2 */
/* # 1 "c792Lib.h" 1 */
struct c792_struct {
  volatile unsigned long  data[512];
  unsigned long blank1[512];
  volatile unsigned short rev;
  volatile unsigned short geoAddr;
  volatile unsigned short cbltAddr;
  volatile unsigned short bitSet1;
  volatile unsigned short bitClear1;
  volatile unsigned short intLevel;
  volatile unsigned short intVector;
  volatile unsigned short status1;
  volatile unsigned short control1;
  volatile unsigned short aderHigh;
  volatile unsigned short aderLow;
  volatile unsigned short ssReset;
  unsigned short blank2;
  volatile unsigned short cbltControl;
  unsigned short blank3[2];
  volatile unsigned short evTrigger;
  volatile unsigned short status2;
  volatile unsigned short evCountL;
  volatile unsigned short evCountH;
  volatile unsigned short incrEvent;
  volatile unsigned short incrOffset;
  volatile unsigned short loadTest;
  unsigned short blank4;
  volatile unsigned short fclrWindow;
  volatile unsigned short bitSet2;
  volatile unsigned short bitClear2;
  volatile unsigned short wMemTestAddr;
  volatile unsigned short memTestWordH;
  volatile unsigned short memTestWordL;
  volatile unsigned short crateSelect;
  volatile unsigned short testEvWrite;
  volatile unsigned short evCountReset;
  unsigned short blank5[15];
  volatile unsigned short iped;
  unsigned short blank6;
  volatile unsigned short rTestAddr;
  unsigned short blank7;
  volatile unsigned short swComm;
  volatile unsigned short slideConst;
  unsigned short blank8[2];
  volatile unsigned short AAD;
  volatile unsigned short BAD;
  unsigned short blank9[6];
  volatile unsigned short threshold[32 ];
};
struct c792_ROM_struct {
  volatile unsigned short OUI_3;
  unsigned short blank1;
  volatile unsigned short OUI_2;
  unsigned short blank2;
  volatile unsigned short OUI_1;
  unsigned short blank3;
  volatile unsigned short version;
  unsigned short blank4;
  volatile unsigned short ID_3;
  unsigned short blank5;
  volatile unsigned short ID_2;
  unsigned short blank6;
  volatile unsigned short ID_1;
  unsigned short blank7[7];
  volatile unsigned short revision;
};
volatile struct c792_struct *c792p[20];          
/* # 21 "newele_test.c" 2 */
/* # 1 "scaler1.h" 1 */
struct lecroy_1151_scaler {
    unsigned short reset;
    unsigned short blank1[7];
    unsigned short bim[8];
    unsigned short blank2[16];
    unsigned long preset[16];
    unsigned long scaler[16];
    unsigned short blank3[29];
    unsigned short id[3];
  };
struct str_7200_scaler {
  unsigned long CSR             ;  
  unsigned long IntVerR         ;  
  unsigned long empty0          ;  
  unsigned long disCntR         ;  
  unsigned long empty1          ;  
  unsigned long clrFIFOR        ;  
  unsigned long wrToFIFOR       ;  
  unsigned long empty2          ;  
  unsigned long clrR            ;  
  unsigned long clkShadowR      ;  
  unsigned long gEnCntR         ;  
  unsigned long gDisCntR        ;  
  unsigned long bclrCntR        ;  
  unsigned long bclkShadowR     ;  
  unsigned long bgEnCntR        ;  
  unsigned long bgDisCntR       ;  
  unsigned long grClrR[4]       ;  
  unsigned long empty3[4]       ;  
  unsigned long resetR          ;  
  unsigned long empty4          ;  
  unsigned long testclkR        ;  
  unsigned long empty5[5]       ;  
  unsigned long readFIFOR       ;  
  unsigned long empty6[31]      ;  
  unsigned long clrCntR[32]     ;  
  unsigned long clrOverR[32]    ;  
  unsigned long readShR[32]     ;  
  unsigned long readCntR[32]    ;  
  unsigned long readClrCntR[32] ;  
  unsigned long grOverR[4]      ;  
 };
struct caen_v560_scaler {
  unsigned short empty1[2];  
  unsigned short IntVectR ;  
  unsigned short IntLevR  ;  
  unsigned short EnIntR   ;  
  unsigned short DisIntR  ;  
  unsigned short ClrIntR  ;  
  unsigned short RequestR ;  
  unsigned long  CntR[16] ;  
  unsigned short ClrR     ;  
  unsigned short setVETOR ;  
  unsigned short clrVETOR ;  
  unsigned short incCntR  ;  
  unsigned short StatusR  ;  
  unsigned short empty2[80];  
  unsigned short FixCodeR ;  
  unsigned short ModTypeR ;  
  unsigned short VersionR ;  
 };
static struct lecroy_1151_scaler *slecroy1151;
static struct str_7200_scaler *sstr7200;
static struct caen_v560_scaler *scaen560;
/* # 22 "newele_test.c" 2 */
/* # 1 "v767_def.h" 1 */
struct v767_struct {
  volatile unsigned long  fifo;
  volatile unsigned short geoAddr;
  volatile unsigned short bitSet;
  volatile unsigned short bitClear;
  volatile unsigned short intLevel;
  volatile unsigned short intVector;
  volatile unsigned short status1;
  volatile unsigned short control1;
  volatile unsigned short ader32;
  volatile unsigned short ader24;
  volatile unsigned short mcstBaseAddr;
  volatile unsigned short SSReset;
  unsigned short blank0[3];
  volatile unsigned short mcstCtrl;
  unsigned short blank1[19];
  volatile unsigned short status2;
  volatile unsigned short control2;
  volatile unsigned short evCount;
  volatile unsigned short clearCount;
  volatile unsigned short microHandshake;
  volatile unsigned short microReg;
  volatile unsigned short clear;
  volatile unsigned short testH;
  volatile unsigned short testL;
  volatile unsigned short trigger;
  unsigned short blank2[2002];
  volatile unsigned short cr[2048];
  volatile unsigned long data[14335];
};
/* # 23 "newele_test.c" 2 */
static int icnt=0;
unsigned long laddr;
extern int bigendian_out;
static int debug=0;
static int icntbad7 = 0;
extern int vxTicks;
static void __download()
{
    daLogMsg("INFO","Readout list compiled %s", DAYTIME);
    *(rol->async_roc) = 0;  
  {   
unsigned long res, iadc, itdc, ich;
  unsigned short channelmask[8];
    daLogMsg("INFO","Entering User Download");
  printf("Setup TIR\n");
  tirInit(0x0ed0 );
{ 
  bigendian_out=0;
  printf("Setup ADCs\n");
  if (1 >0) {
    c792Init(0x220000 ,0x010000 ,1 ,0);
    for (iadc=0; iadc< 1  ; iadc++) { 
      c792Reset(iadc); 
      c792Clear(iadc);
      c792ClearThresh(iadc);
      c792p[iadc]->bitSet2=0x00000800;   
      c792p[iadc]->iped=180;
      printf("v792 ADC[%d] pedestal register: %hd\n", iadc, c792p[iadc]->iped);
      c792Sparse(iadc, 0, 0);  
      for(ich=0; ich<32; ich++)
      {	
	c792SetThresh(iadc, ich, 1);
      }	
      c792Disable(iadc);
      c792Status(iadc);
    }
  }
  printf("Setup TDCs\n");
  if(1 >0) {
	  printf("Setup T0 T1 CAEN 775 TDCs\n");
    c775Init(0x110000 ,0x010000 ,1 ,0);
    for (itdc= 0 ;itdc< 0 + 1 ;itdc++) 
      {
	c775Clear(itdc);
	c775Disable(itdc);
	c775BitSet2(itdc,(0x0880) | (1<<12));   
	c775ClearThresh(itdc);
        c775CommonStop(itdc); 	
	c775SetFSR(itdc,1000);  
	c775Sparse(itdc,0,0);
	c775Status(itdc);
      }
  }
 } 
    daLogMsg("INFO","User Download Executed");
  }   
}       
static void __prestart()
{
{ dispatch_busy = 0; bzero((char *) evMasks, sizeof(evMasks)); bzero((char *) syncTRtns, 
sizeof(syncTRtns)); bzero((char *) ttypeRtns, sizeof(ttypeRtns)); bzero((char *) Tcode, sizeof(Tcode)); 
wrapperGenerator = 0; theEvMask = 0; currEvMask = 0; trigId = 1; poolEmpty = 0; __the_event__ = (DANODE *) 0; 
input_event__ = (DANODE *) 0; } ;     *(rol->nevents) = 0;
  {   
unsigned long itdc;
    daLogMsg("INFO","Entering User Prestart");
    { VME_handlers =0;VME_isAsync = 0;VMEflag = 0;} ;
{ void usrtrig ();void usrtrig_done (); doneRtns[trigId] = (FUNCPTR) ( usrtrig_done ) ; 
trigRtns[trigId] = (FUNCPTR) ( usrtrig ) ; Tcode[trigId] = ( 1 ) ; ttypeRtns[trigId] = vmettype ; {printf("Linking async VME trigger to id %d 
\n", trigId ); VME_handlers = ( trigId );VME_isAsync = 1;vmetriglink( 1 ,VME_int_handler);} 
;trigId++;} ;     {evMasks[ 1 ] |= (1<<( VME_handlers ));} ;
  icntbad7=0;
    daLogMsg("INFO","User Prestart Executed");
  }   
if (__the_event__) rol->dabufp = ((void *) 0) ; if (__the_event__) { if (rol->output) { {if(! ( 
&(rol->output->list) )->c ){( &(rol->output->list) )->f = ( &(rol->output->list) )->l = ( __the_event__ );( 
__the_event__ )->p = 0;} else {( __the_event__ )->p = ( &(rol->output->list) )->l;( &(rol->output->list) 
)->l->n = ( __the_event__ );( &(rol->output->list) )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( 
&(rol->output->list) )->c++;	if(( &(rol->output->list) )->add_cmd != ((void *) 0) ) (*(( &(rol->output->list) 
)->add_cmd)) (( &(rol->output->list) )); } ; } else { { if (( __the_event__ )->part == 0) { free( __the_event__ 
); __the_event__ = 0; } else { {if(! ( & __the_event__ ->part->list )->c ){( & __the_event__ 
->part->list )->f = ( & __the_event__ ->part->list )->l = ( __the_event__ );( __the_event__ )->p = 0;} else {( 
__the_event__ )->p = ( & __the_event__ ->part->list )->l;( & __the_event__ ->part->list )->l->n = ( 
__the_event__ );( & __the_event__ ->part->list )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( & 
__the_event__ ->part->list )->c++;	if(( & __the_event__ ->part->list )->add_cmd != ((void *) 0) ) (*(( & 
__the_event__ ->part->list )->add_cmd)) (( & __the_event__ ->part->list )); } ; } if( __the_event__ 
->part->free_cmd != ((void *) 0) ) { (*( __the_event__ ->part->free_cmd)) ( __the_event__ ->part->clientData); 
} } ;	} __the_event__ = (DANODE *) 0; } if(input_event__) { { if (( input_event__ )->part == 0) { 
free( input_event__ ); input_event__ = 0; } else { {if(! ( & input_event__ ->part->list )->c ){( & 
input_event__ ->part->list )->f = ( & input_event__ ->part->list )->l = ( input_event__ );( input_event__ 
)->p = 0;} else {( input_event__ )->p = ( & input_event__ ->part->list )->l;( & input_event__ 
->part->list )->l->n = ( input_event__ );( & input_event__ ->part->list )->l = ( input_event__ );} ( 
input_event__ )->n = 0;( & input_event__ ->part->list )->c++;	if(( & input_event__ ->part->list )->add_cmd 
!= ((void *) 0) ) (*(( & input_event__ ->part->list )->add_cmd)) (( & input_event__ ->part->list 
)); } ; } if( input_event__ ->part->free_cmd != ((void *) 0) ) { (*( input_event__ 
->part->free_cmd)) ( input_event__ ->part->clientData); } } ; input_event__ = (DANODE *) 0; } {int ix; if 
(currEvMask) {	for (ix=0; ix < trigId; ix++) {	if (currEvMask & (1<<ix)) (*doneRtns[ix])(); } if 
(rol->pool->list.c) { currEvMask = 0; __done(); rol->doDone = 0; } else { poolEmpty = 1; rol->doDone = 1; } } } ;     *(rol->nevents) = 0;
    rol->recNb = 0;
}       
static void __end()
{
  {   
unsigned long iadc, itdc, iwait, nevt, nwords;
   vmetdisable(  1  ,  0  );  ;
{ 
  if (1 >0) {
    for (iadc=0; iadc< 1  ; iadc++) { 
      c792Disable(iadc);
      c792Clear(iadc);
    }
  }
  if(1 >0) {
    for (itdc= 0 ;itdc< 0 + 1 ;itdc++) 
      {
	c775Disable(itdc);
	c775Clear(itdc);
      }
  }
 } 
    daLogMsg("INFO","User End Executed");
  }   
if (__the_event__) rol->dabufp = ((void *) 0) ; if (__the_event__) { if (rol->output) { {if(! ( 
&(rol->output->list) )->c ){( &(rol->output->list) )->f = ( &(rol->output->list) )->l = ( __the_event__ );( 
__the_event__ )->p = 0;} else {( __the_event__ )->p = ( &(rol->output->list) )->l;( &(rol->output->list) 
)->l->n = ( __the_event__ );( &(rol->output->list) )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( 
&(rol->output->list) )->c++;	if(( &(rol->output->list) )->add_cmd != ((void *) 0) ) (*(( &(rol->output->list) 
)->add_cmd)) (( &(rol->output->list) )); } ; } else { { if (( __the_event__ )->part == 0) { free( __the_event__ 
); __the_event__ = 0; } else { {if(! ( & __the_event__ ->part->list )->c ){( & __the_event__ 
->part->list )->f = ( & __the_event__ ->part->list )->l = ( __the_event__ );( __the_event__ )->p = 0;} else {( 
__the_event__ )->p = ( & __the_event__ ->part->list )->l;( & __the_event__ ->part->list )->l->n = ( 
__the_event__ );( & __the_event__ ->part->list )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( & 
__the_event__ ->part->list )->c++;	if(( & __the_event__ ->part->list )->add_cmd != ((void *) 0) ) (*(( & 
__the_event__ ->part->list )->add_cmd)) (( & __the_event__ ->part->list )); } ; } if( __the_event__ 
->part->free_cmd != ((void *) 0) ) { (*( __the_event__ ->part->free_cmd)) ( __the_event__ ->part->clientData); 
} } ;	} __the_event__ = (DANODE *) 0; } if(input_event__) { { if (( input_event__ )->part == 0) { 
free( input_event__ ); input_event__ = 0; } else { {if(! ( & input_event__ ->part->list )->c ){( & 
input_event__ ->part->list )->f = ( & input_event__ ->part->list )->l = ( input_event__ );( input_event__ 
)->p = 0;} else {( input_event__ )->p = ( & input_event__ ->part->list )->l;( & input_event__ 
->part->list )->l->n = ( input_event__ );( & input_event__ ->part->list )->l = ( input_event__ );} ( 
input_event__ )->n = 0;( & input_event__ ->part->list )->c++;	if(( & input_event__ ->part->list )->add_cmd 
!= ((void *) 0) ) (*(( & input_event__ ->part->list )->add_cmd)) (( & input_event__ ->part->list 
)); } ; } if( input_event__ ->part->free_cmd != ((void *) 0) ) { (*( input_event__ 
->part->free_cmd)) ( input_event__ ->part->clientData); } } ; input_event__ = (DANODE *) 0; } {int ix; if 
(currEvMask) {	for (ix=0; ix < trigId; ix++) {	if (currEvMask & (1<<ix)) (*doneRtns[ix])(); } if 
(rol->pool->list.c) { currEvMask = 0; __done(); rol->doDone = 0; } else { poolEmpty = 1; rol->doDone = 1; } } } ; }  
static void __pause()
{
  {   
   vmetdisable(  1  ,  0  );  ;
    daLogMsg("INFO","User Pause Executed");
  }   
if (__the_event__) rol->dabufp = ((void *) 0) ; if (__the_event__) { if (rol->output) { {if(! ( 
&(rol->output->list) )->c ){( &(rol->output->list) )->f = ( &(rol->output->list) )->l = ( __the_event__ );( 
__the_event__ )->p = 0;} else {( __the_event__ )->p = ( &(rol->output->list) )->l;( &(rol->output->list) 
)->l->n = ( __the_event__ );( &(rol->output->list) )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( 
&(rol->output->list) )->c++;	if(( &(rol->output->list) )->add_cmd != ((void *) 0) ) (*(( &(rol->output->list) 
)->add_cmd)) (( &(rol->output->list) )); } ; } else { { if (( __the_event__ )->part == 0) { free( __the_event__ 
); __the_event__ = 0; } else { {if(! ( & __the_event__ ->part->list )->c ){( & __the_event__ 
->part->list )->f = ( & __the_event__ ->part->list )->l = ( __the_event__ );( __the_event__ )->p = 0;} else {( 
__the_event__ )->p = ( & __the_event__ ->part->list )->l;( & __the_event__ ->part->list )->l->n = ( 
__the_event__ );( & __the_event__ ->part->list )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( & 
__the_event__ ->part->list )->c++;	if(( & __the_event__ ->part->list )->add_cmd != ((void *) 0) ) (*(( & 
__the_event__ ->part->list )->add_cmd)) (( & __the_event__ ->part->list )); } ; } if( __the_event__ 
->part->free_cmd != ((void *) 0) ) { (*( __the_event__ ->part->free_cmd)) ( __the_event__ ->part->clientData); 
} } ;	} __the_event__ = (DANODE *) 0; } if(input_event__) { { if (( input_event__ )->part == 0) { 
free( input_event__ ); input_event__ = 0; } else { {if(! ( & input_event__ ->part->list )->c ){( & 
input_event__ ->part->list )->f = ( & input_event__ ->part->list )->l = ( input_event__ );( input_event__ 
)->p = 0;} else {( input_event__ )->p = ( & input_event__ ->part->list )->l;( & input_event__ 
->part->list )->l->n = ( input_event__ );( & input_event__ ->part->list )->l = ( input_event__ );} ( 
input_event__ )->n = 0;( & input_event__ ->part->list )->c++;	if(( & input_event__ ->part->list )->add_cmd 
!= ((void *) 0) ) (*(( & input_event__ ->part->list )->add_cmd)) (( & input_event__ ->part->list 
)); } ; } if( input_event__ ->part->free_cmd != ((void *) 0) ) { (*( input_event__ 
->part->free_cmd)) ( input_event__ ->part->clientData); } } ; input_event__ = (DANODE *) 0; } {int ix; if 
(currEvMask) {	for (ix=0; ix < trigId; ix++) {	if (currEvMask & (1<<ix)) (*doneRtns[ix])(); } if 
(rol->pool->list.c) { currEvMask = 0; __done(); rol->doDone = 0; } else { poolEmpty = 1; rol->doDone = 1; } } } ; }  
static void __go()
{
  {   
unsigned long iadc, itdc, iwait, nevt, nwords;
    daLogMsg("INFO","Entering User Go");
   vmetenable(  1  ,  0  );  ;
{ 
  if (1 >0) {
    for (iadc=0; iadc< 1  ; iadc++) { 
      c792Clear(iadc);
      c792Enable(iadc);
    }
  }
  if(1 >0) {
    for (itdc= 0 ;itdc< 0 + 1 ;itdc++) 
      {
	c775Clear(itdc);
	c775Enable(itdc);
      }
  }
 } 
  }   
if (__the_event__) rol->dabufp = ((void *) 0) ; if (__the_event__) { if (rol->output) { {if(! ( 
&(rol->output->list) )->c ){( &(rol->output->list) )->f = ( &(rol->output->list) )->l = ( __the_event__ );( 
__the_event__ )->p = 0;} else {( __the_event__ )->p = ( &(rol->output->list) )->l;( &(rol->output->list) 
)->l->n = ( __the_event__ );( &(rol->output->list) )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( 
&(rol->output->list) )->c++;	if(( &(rol->output->list) )->add_cmd != ((void *) 0) ) (*(( &(rol->output->list) 
)->add_cmd)) (( &(rol->output->list) )); } ; } else { { if (( __the_event__ )->part == 0) { free( __the_event__ 
); __the_event__ = 0; } else { {if(! ( & __the_event__ ->part->list )->c ){( & __the_event__ 
->part->list )->f = ( & __the_event__ ->part->list )->l = ( __the_event__ );( __the_event__ )->p = 0;} else {( 
__the_event__ )->p = ( & __the_event__ ->part->list )->l;( & __the_event__ ->part->list )->l->n = ( 
__the_event__ );( & __the_event__ ->part->list )->l = ( __the_event__ );} ( __the_event__ )->n = 0;( & 
__the_event__ ->part->list )->c++;	if(( & __the_event__ ->part->list )->add_cmd != ((void *) 0) ) (*(( & 
__the_event__ ->part->list )->add_cmd)) (( & __the_event__ ->part->list )); } ; } if( __the_event__ 
->part->free_cmd != ((void *) 0) ) { (*( __the_event__ ->part->free_cmd)) ( __the_event__ ->part->clientData); 
} } ;	} __the_event__ = (DANODE *) 0; } if(input_event__) { { if (( input_event__ )->part == 0) { 
free( input_event__ ); input_event__ = 0; } else { {if(! ( & input_event__ ->part->list )->c ){( & 
input_event__ ->part->list )->f = ( & input_event__ ->part->list )->l = ( input_event__ );( input_event__ 
)->p = 0;} else {( input_event__ )->p = ( & input_event__ ->part->list )->l;( & input_event__ 
->part->list )->l->n = ( input_event__ );( & input_event__ ->part->list )->l = ( input_event__ );} ( 
input_event__ )->n = 0;( & input_event__ ->part->list )->c++;	if(( & input_event__ ->part->list )->add_cmd 
!= ((void *) 0) ) (*(( & input_event__ ->part->list )->add_cmd)) (( & input_event__ ->part->list 
)); } ; } if( input_event__ ->part->free_cmd != ((void *) 0) ) { (*( input_event__ 
->part->free_cmd)) ( input_event__ ->part->clientData); } } ; input_event__ = (DANODE *) 0; } {int ix; if 
(currEvMask) {	for (ix=0; ix < trigId; ix++) {	if (currEvMask & (1<<ix)) (*doneRtns[ix])(); } if 
(rol->pool->list.c) { currEvMask = 0; __done(); rol->doDone = 0; } else { poolEmpty = 1; rol->doDone = 1; } } } ; }
void usrtrig(unsigned long EVTYPE,unsigned long EVSOURCE)
{
    long EVENT_LENGTH;
  {   
unsigned long event_no, event_len, event_cnt, event_ty, time1, time2;
  int iii,clear_tdc,status2,chip_err[4];
  static int icnt=0;
  int i,iadc,itdc,isca,iwait,nevt,nwords;
  int adc_wait[3],tdc_wait[3];
  clear_tdc=0;
  event_len = 0;
  event_cnt = -1;
  event_ty = EVTYPE;
  event_no = *rol->nevents;
  time1 = vxTicks;
 rol->dabufp = (long *)0;
{	{if(__the_event__ == (DANODE *) 0 && rol->dabufp == ((void *) 0) ) { {{ ( __the_event__ ) = 0; if (( 
&( rol->pool ->list) )->c){ ( &( rol->pool ->list) )->c--; ( __the_event__ ) = ( &( rol->pool 
->list) )->f; ( &( rol->pool ->list) )->f = ( &( rol->pool ->list) )->f->n; }; if (!( &( rol->pool ->list) 
)->c) { ( &( rol->pool ->list) )->l = 0; }} ;} ; if(__the_event__ == (DANODE *) 0) { logMsg ("TRIG ERROR: no pool buffer 
available\n"); return; } rol->dabufp = (long *) &__the_event__->length; if (input_event__) { 
__the_event__->nevent = input_event__->nevent; } else { __the_event__->nevent = *(rol->nevents); } } } ; 
StartOfEvent[event_depth__++] = (rol->dabufp); if(input_event__) {	*(++(rol->dabufp)) = (( EVTYPE ) << 16) | (( 0x01 ) << 8) | 
(0xff & (input_event__->nevent));	} else {	*(++(rol->dabufp)) = (syncFlag<<24) | (( EVTYPE ) << 16) 
| (( 0x01 ) << 8) | (0xff & *(rol->nevents));	}	((rol->dabufp))++;} ;     {*(rol->dabufp)++ = ( 0xda000abc );} ; 
{ 
 if(1 >0) {
   for (iadc=0; iadc< 1  ; iadc++) 
     { 
       adc_wait[iadc]=0;
       do 
	 {
  nevt = c792Dready(iadc);
	   iwait++;
	 } while ((nevt<=0) && (iwait<=1000));
	adc_wait[iadc]=iwait;
       if (nevt>0) 
	 {
	   *rol->dabufp++=0xda00adc0+iadc;
	   nwords = c792ReadEvent(iadc,rol->dabufp+1);
	   *rol->dabufp=nwords;
	   rol->dabufp+=nwords+1;
	 }
       else 
	 {
	   *rol->dabufp++=0xbad0adc0+iadc;
	 }
     }
 }
if(1 >0) {
  for (itdc= 0 ; itdc< 0 + 1 ; itdc++) 
    {
      iwait=0;
      do
	{
	  nevt = c775Dready(itdc);
	  iwait++;
	} while ((nevt<=0) && (iwait<=1000));
	tdc_wait[itdc]=iwait;
      if (nevt>0)
	{
	  *rol->dabufp++=0xda00ddc0+itdc;
	  nwords = c775ReadEvent(itdc,rol->dabufp+1);
	  *rol->dabufp=nwords;
	  rol->dabufp+=nwords+1;
	}
      else
	{
	  logMsg("trigger: ERROR: Data Not Ready for tdc %d\n",itdc);
	  *rol->dabufp++=0xbad0ddc0+itdc;
	}
    }
}
 time2 = vxTicks;
 *(rol->dabufp)++ =  0xfabc0006;
 icnt = icnt + 1;
 if(icnt > 20000) icnt = 0;
 *rol->dabufp++ = icnt;
 *rol->dabufp++=event_no;
 *rol->dabufp++=event_cnt;
 *rol->dabufp++=time1;
 *rol->dabufp++=event_ty;
 *rol->dabufp++=time2;
 *(rol->dabufp)++ =  0xfabcffff;
 if(1 >0) {
   for (iadc=0; iadc< 1  ; iadc++) 
	   *rol->dabufp++=adc_wait[iadc];
}
if(1 >0) {
  for (itdc= 0 ; itdc< 0 + 1 ; itdc++) 
	   *rol->dabufp++=tdc_wait[itdc];
}
 } 
    {*(rol->dabufp)++ = ( 0xda0000ff );} ; 
{event_depth__--; *StartOfEvent[event_depth__] = (long) (((char *) (rol->dabufp)) - ((char 
*) StartOfEvent[event_depth__]));	if ((*StartOfEvent[event_depth__] & 1) != 0) { 
(rol->dabufp) = ((long *)((char *) (rol->dabufp))+1); *StartOfEvent[event_depth__] += 1; }; if 
((*StartOfEvent[event_depth__] & 2) !=0) { *StartOfEvent[event_depth__] = *StartOfEvent[event_depth__] + 2; (rol->dabufp) = 
((long *)((short *) (rol->dabufp))+1);; };	*StartOfEvent[event_depth__] = ( 
(*StartOfEvent[event_depth__]) >> 2) - 1;}; ;   }   
}  
void usrtrig_done()
{
  {   
  }   
}  
void __done()
{
poolEmpty = 0;  
  {   
   vmetack(  1  ,  0  );  ;
  }   
}  
static void __status()
{
  {   
  }   
}  
