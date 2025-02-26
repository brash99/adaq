/* taskLib.h - generic kernel interface header */

/* Copyright 1984-2002 Wind River Systems, Inc. */

/*
modification history
--------------------
04t,16jan02,to   add fpStatus to TCB for ARM
04s,19nov01,aeg  added td_events to TASK_DESC.
04r,09nov01,jhw  Revert WDB_INFO to reside inside WIND_TCB.
04q,06nov01,gls  cleaned up compilation warnings
04p,06nov01,dee  fix offsets in coldfire specific portion
		 for EXC_INFO and REG_SET per sh-coldfire.tcl
04o,30oct01,jhw  Removed wdbInfo and wdbExitHook from WIND_TCB.
	    gsl  Reused wdbExitHook for pPthread.
04n,22oct01,dee  Merge from T2.1.0 ColdFire
04m,04oct01,yvp  Fix SPR69306: added member pCoprocCtx to PPC section for 
                 Altivec support.
04l,27sep01,aeg  changed comment on WIND_TCB reservedX fields.
04k,24sep01,s_l  Fixed a systax error on line "UINT32 regSetPad[2]" missing ';'
04j,18sep01,aeg  deleted selectSem, pad1, & pSelWakeupNode from WIND_TCB,
		 and added pSelContext, events, pWdbTaskInfo, & wdbExitHook;
		 added code barriers to TASK_LOCK and TASK_SAFE macros.
04i,01mar00,frf  Add SH4 support for T2.
04e,10sep98,kab  expanded WDB_INFO struct to match TOR2.x., moved pDspContext
		 to end of TCB, replaced spare4.
04d,22jul98,mem  added pDspContext to TCB, removed spare4. Added VX_DSP_TASK.
04c,23apr97,hk   added SH support.
04i,14mar01,pcs  Added VX_ALTIVEC_TASK.
04h,12mar99,dbs  add COM task-local storage to TCB
04g,29may98,cym  added windowsContextValid flag for SIMNT.
04f,24apr98,kkk  moved wdbInfo after ARM stuff in TCB. (bug created by merge)
04e,19aug97,cym  added SIMNT support.
04d,28nov96,cdp  added ARM support.
04c,06nov97,dbt  modified WIND_TCB for new debugger scheme. Removed Am29K 
		 family support.
04b,22jul96,jmb  merge ease patch for HP Sim
04a,26may94,yao  added PPC support.
03z,26may95,ms	 added wdbExitHook field to the TCB
03y,16may95,rrr  add WDB_INFO structure to TCB.
03y,12jul95,ism  added simsolaris support
03x,19mar95,dvs  removed #ifdef TRON - tron no longer supported.
03w,15mar94,smb  modified TASK_SAFE macro
03v,24jan94,smb  added instrumentation macros
03u,10dec93,smb  instrumented TASK_LOCK macro
03t,02dec93,pme  added Am29K family support
03s,12nov93,hdn  added support for I80X86
03r,15oct93,cd   removed junk values from MIPS TCB.
03q,16sep93,jmm  added S_taskLib_ILLEGAL_PRIORITY and taskPriRangeCheck
03p,11aug93,gae  vxsim hppa.
03o,20jun93,rrr  vxsim.
03m,11feb93,jcf  added __PROTOTYPE_5_0 for compatibility.
03l,08feb93,smb  added a null check to taskIdVerify.
03k,13nov92,dnw  changed declaration of pSmObjTcb to struct sm_obj_tcb *
		 removed include of smObjTcbP.h (SPR #1768)
		 moved typedef of DBG_INFO here from dbgLib.h
		 removed include of dbgLib.h (SPR #1768)
03j,22sep92,rrr  added support for c++
03i,21sep92,smb  removed exit prototype and added include of stdlib.h
03h,02aug92,jcf  changed reserved3 field to pExcRegSet for exception handling.
03g,29jul92,smb  made modification for the stdio library.
03f,28jul92,jcf  added windxLock/reserved[12]; moved dbgInfo/pSmTcbObj;
		 removed funcRestart.
03e,12jul92,yao  replace pDbgState pointer with data structure DBG_INFO
		 in WIND_TCB.  added dbgPCWSave to i960.
03d,19jul92,pme  added shared memory objects support.
03c,10jul92,jwt  moved padding out of REG_SET in into WIND_TCB for SPARC.
03b,06jul92,ajm  removed taskSummary from forward declarations
03a,04jul92,jcf  cleaned up.
02y,16jun92,yao  made pDbgState available for all architectures.
02x,26may92,rrr  the tree shuffle
		  -changed includes to have absolute path from h/
02w,19mar92,yao  added ANSI prototype for taskStackAllot().
02v,12mar92,yao  removed ifdef CPU.  added taskRegsShow().
02u,10jan92,jwt  added CPU_FAMILY==SPARC architecture dependent prototypes.
02t,11nov91,rrr  Added funcRestart to tcb for signals.
02s,28oct91,wmd  Added changes for i960KB from Intel.
02r,04oct91,rrr  passed through the ansification filter
		  -fixed #else and #endif
		  -changed VOID to void
		  -changed ASMLANGUAGE to _ASMLANGUAGE
		  -changed copyright notice
02q,20sep91,hdn  deleted foroff from WIND_TCB for TRON.
		 added pDbgState to WIND_TCB for TRON.
02p,20aug91,ajm  made architecture independant.
01o,10jun91,del  added pragma for gnu960 alignment.
01n,23may91,wmd  added defines and macros for SPARC architecture.
01m,29apr91,hdn  added defines and macros for TRON architecture.
01l,08apr91,jdi  added NOMANUAL to prevent mangen.
02k,24mar91,del  added pDbgState for use with new dbgLib. And I960 defines.
02j,16oct90,shl  made #else ANSI style.
02i,05oct90,dnw  deleted private functions.
		 made taskSpawn, taskInit, taskCreate take var args.
02h,05oct90,shl  added ANSI function prototypes.
                 made #endif ANSI style.
                 added copyright notice.
02g,01oct90,jcf  added addtional spares to WIND_TCB.
02f,03aug90,jcf  moved arch dependent portion of WIND_TCB to end of struct
02e,13jul90,rdc  added support for environment variables and additional
		 select functionality.
02d,30jun90,jcf  added assembly language defines.
02c,26jun90,jcf  removed obsolete generic status codes
		 changed inheritance protocol.
		 changed safetyQSem to a safetyQHead.
		 reworked priority mananagement.
		 changed topOfStack to endOfStack.
02b,17may90,rdc  changed select semaphores in tcb to be binary semaphores.
02a,17apr90,jcf  added error codes.
		 changed to wind 2.0.
01l,16mar90,rdc  added select semaphore to tcbx.
01k,25may89,gae  added VX_FORTRAN option.
01j,21apr89,jcf  added KERNEL_{UNINIT,VRTX,PSOS,WIND}.
01i,07nov88,rdc  added VX_ADA_DEBUG to task options.
01h,22jun88,dnw  name tweaks.
01g,30may88,dnw  changed to v4 names.
01f,28may88,dnw  deleted obsolete status values.
		 added EXC_INFO to tcbx.
01e,18may88,jcf  added psos semaphore head to tcbx.
		  extended maximum number of hooks to 10.
01d,13may88,rdc  added signal info to tcbx.
01c,28apr88,ecs  added IMPORTs of idle & taskName.
01b,13apr88,gae  added function declarations; option bit VX_STDIO;
		 taskStd[] to TCB extension.
01a,25jan88,jcf  written by extracting from vxLib.h v02l.
*/

#ifndef __INCtaskLibh
#define __INCtaskLibh

#ifdef __cplusplus
extern "C" {
#endif

#include "vxWorks.h"
#include "vwModNum.h"
#include "classLib.h"
#include "qLib.h"
#include "regs.h"
#include "excLib.h"
#include "private/eventP.h"
#include "private/semLibP.h"
#include "private/objLibP.h"
#include "private/eventLibP.h"
#include "stdlib.h"


/* generic status codes */

#define S_taskLib_NAME_NOT_FOUND		(M_taskLib | 101)
#define S_taskLib_TASK_HOOK_TABLE_FULL		(M_taskLib | 102)
#define S_taskLib_TASK_HOOK_NOT_FOUND		(M_taskLib | 103)
#define S_taskLib_TASK_SWAP_HOOK_REFERENCED	(M_taskLib | 104)
#define S_taskLib_TASK_SWAP_HOOK_SET		(M_taskLib | 105)
#define S_taskLib_TASK_SWAP_HOOK_CLEAR		(M_taskLib | 106)
#define S_taskLib_TASK_VAR_NOT_FOUND		(M_taskLib | 107)
#define S_taskLib_TASK_UNDELAYED		(M_taskLib | 108)
#define S_taskLib_ILLEGAL_PRIORITY		(M_taskLib | 109)


/* miscellaneous */

#define MAX_TASK_ARGS		10	/* max args passed to a task */
#define VX_MAX_TASK_SWITCH_RTNS	16	/* max task switch callout routines */
#define VX_MAX_TASK_SWAP_RTNS	16	/* max task swap callout routines */
#define VX_MAX_TASK_DELETE_RTNS	16	/* max task delete callout routines */
#define VX_MAX_TASK_CREATE_RTNS	16	/* max task create callout routines */

/* task option bits */

#define VX_SUPERVISOR_MODE	0x0001	/* OBSOLETE: tasks always in sup mode */
#define VX_UNBREAKABLE	  	0x0002	/* INTERNAL: breakpoints ignored */
#define VX_DEALLOC_STACK  	0x0004	/* INTERNAL: deallocate stack */
#define VX_FP_TASK	   	0x0008	/* 1 = f-point coprocessor support */
#define VX_STDIO	   	0x0010	/* OBSOLETE: need not be set for stdio*/
#define VX_ADA_DEBUG	   	0x0020	/* 1 = VADS debugger support */
#define VX_FORTRAN	   	0x0040	/* 1 = NKR FORTRAN support */
#define VX_PRIVATE_ENV 		0x0080	/* 1 = private environment variables */
#define VX_NO_STACK_FILL	0x0100	/* 1 = avoid stack fill of 0xee */
#define VX_DSP_TASK		0x0200	/* 1 = dsp coprocessor support */
#define VX_ALTIVEC_TASK         0x0400  /* 1 = ALTIVEC coprocessor support */

/* typedefs */

#if CPU_FAMILY==I960
#pragma align 1			/* tell gcc960 not to optimize alignments */
#endif	/* CPU_FAMILY==I960 */

#ifndef	_ASMLANGUAGE

typedef struct wdb_info		/* task debugging state info */
    {
    int			wdbState;	/* debug state */
    REG_SET *		wdbRegisters;	/* cause of trace exception */
    struct
        {
	void *	wdb1;
	void *	wdb2;
	} wdbEvtList;			/* dll_t of ptr to breakpoint info */
    int			bpAddr;		/* address of last breakpoint */
					/* encountered */
    int			taskBpAddr;	/* task breakpoint address */
    int			taskPc;		/* task program couner */
    int			taskFp;		/* task frame pointer */
    int			taskSp;		/* task stack pointer */
    VOIDFUNCPTR		wdbExitHook;	/* WDB notify routine on task exit */
  } WDB_INFO;

typedef struct windTcb		/* WIND_TCB - task control block */
    {
    Q_NODE		qNode;		/* 0x00: multiway q node: rdy/pend q */
    Q_NODE		tickNode;	/* 0x10: multiway q node: tick q */
    Q_NODE		activeNode;	/* 0x20: multiway q node: active q */

    OBJ_CORE		objCore;	/* 0x30: object management */
    char *		name;		/* 0x34: pointer to task name */
    int			options;	/* 0x38: task option bits */
    UINT		status;		/* 0x3c: status of task */
    UINT		priority;	/* 0x40: task's current priority */
    UINT		priNormal;	/* 0x44: task's normal priority */
    UINT		priMutexCnt;	/* 0x48: nested priority mutex owned */
    struct semaphore *	pPriMutex;	/* 0x4c: pointer to inheritance mutex */

    UINT		lockCnt;	/* 0x50: preemption lock count */
    UINT		tslice;		/* 0x54: current count of time slice */

    UINT16		swapInMask;	/* 0x58: task's switch in hooks */
    UINT16		swapOutMask;	/* 0x5a: task's switch out hooks */

    Q_HEAD *		pPendQ;		/* 0x5c: q head pended on (if any) */

    UINT		safeCnt;	/* 0x60: safe-from-delete count */
    Q_HEAD		safetyQHead;	/* 0x64: safe-from-delete q head */

    FUNCPTR		entry;		/* 0x74: entry point of task */

    char *		pStackBase;	/* 0x78: points to bottom of stack */
    char *		pStackLimit;	/* 0x7c: points to stack limit */
    char *		pStackEnd;	/* 0x80: points to init stack limit */

    int			errorStatus;	/* 0x84: most recent task error */
    int			exitCode;	/* 0x88: error passed to exit () */

    struct sigtcb *	pSignalInfo;	/* 0x8c: ptr to signal info for task */
    struct selContext *	pSelectContext;	/* 0x90: ptr to select info for task */

    UINT		taskTicks;	/* 0x94: total number of ticks */
    UINT		taskIncTicks;	/* 0x98: number of ticks in slice */

    struct taskVar *	pTaskVar;	/* 0x9c: ptr to task variable list */
    struct rpcModList *	pRPCModList;	/* 0xa0: ptr to rpc module statics */
    struct fpContext *	pFpContext;	/* 0xa4: fpoint coprocessor context */

    struct __sFILE *	taskStdFp[3];	/* 0xa8: stdin,stdout,stderr fps */
    int			taskStd[3];	/* 0xb4: stdin,stdout,stderr fds */

    char **		ppEnviron;	/* 0xc0: environment var table */
    int                 envTblSize;     /* 0xc4: number of slots in table */
    int                 nEnvVarEntries; /* 0xc8: num env vars used */
    struct sm_obj_tcb *	pSmObjTcb;	/* 0xcc: shared mem object TCB */
    int			windxLock;	/* 0xd0: lock for windX */
    void *		pComLocal;	/* 0xd4: COM task-local storage ptr */
    REG_SET *		pExcRegSet;	/* 0xd8: exception regSet ptr or NULL */
    EVENTS		events;		/* 0xdc: event info for the task */
    WDB_INFO *		pWdbInfo;	/* 0xe8: ptr to WDB info - future use */
    void *		pPthread;	/* 0xec: ptr to pthread data structs */
    int			reserved1;	/* 0xf0: possible WRS extension */
    int			reserved2;	/* 0xf4: possible WRS extension */
    int			spare1;		/* 0xf8: possible user extension */
    int			spare2;		/* 0xfc: possible user extension */
    int			spare3;		/* 0x100: possible user extension */
    int			spare4;		/* 0x104: possible user extension */

    /* ARCHITECTURE DEPENDENT */

#if (CPU_FAMILY==MC680X0)
    EXC_INFO		excInfo;	/* 0x108: exception info */

    /* REG_SET must be aligned on a 4 byte boundary */

    REG_SET		regs;		/* 0x11c: register set */
    UINT16		foroff;		/* 0x164: format/offset from frame */
    UINT16		pad2;		/* 0x?  : pad format/offset to UINT */
#endif	/* CPU_FAMILY==MC680X0 */

#if (CPU_FAMILY==COLDFIRE)
    EXC_INFO		excInfo;	/* 0x108: exception info */
    REG_SET		regs;		/* 0x11c: register set */
#endif	/* CPU_FAMILY==COLDFIRE */

#if (CPU_FAMILY==MIPS) 
    EXC_INFO		excInfo;	/* 0x108: exception info */

    /* REG_SET must be aligned on a ?? byte boundary */

    REG_SET		regs;		/* 0x128: register set */
#endif	/* CPU_FAMILY==MIPS */

#if (CPU_FAMILY == PPC)
    EXC_INFO            excInfo;        /* 0x108: exception info */
    UINT                pCoprocCtx;     /* 0x12c: pointer to Coprocessor Ctx */

    /* REG_SET must be aligned on a 16 byte boundary */

    REG_SET             regs;           /* 0x130: register set */
#endif /* (CPU_FAMILY == PPC) */

#if (CPU_FAMILY == SPARC)
    EXC_INFO		excInfo;	/* 0x0108:  exception info */

    /* REG_SET must be aligned on an 8 byte boundary */

    UINT		regSetPad;	/* 0x011c:  double-word alignment */
    REG_SET		regs;		/* 0x0120:  register set */
#endif	/* CPU_FAMILY == SPARC */

#if (CPU_FAMILY==SIMSPARCSUNOS || CPU_FAMILY==SIMSPARCSOLARIS)
    EXC_INFO		excInfo;	/* 0x0108:  exception info */

    /* REG_SET must be aligned on an 8 byte boundary */

    REG_SET		regs;		/* 0x0128:  register set */
#endif /* CPU_FAMILY==SIMSPARCSUNOS || CPU_FAMILY==SIMSPARCSOLARIS*/

#if (CPU_FAMILY==SIMNT)
    EXC_INFO            excInfo;        /* 0x0108:  exception info */

    /* REG_SET must be aligned on a 4 byte boundary */

    REG_SET             regs;           /* 0x011c:  register set */
    int                 args[MAX_TASK_ARGS];
    HANDLE              windowsThread;
    unsigned int        windowsThreadId;
    CONTEXT             windowsContext;
    unsigned int	windowsContextValid;
#endif /* CPU_FAMILY==SIMNT */

#if (CPU_FAMILY==SIMHPPA)
    EXC_INFO          excInfo;        /* 0x0108:  exception info */

    /* REG_SET must be aligned on a ?? byte boundary */

    REG_SET           regs;           /* 0x0268:  public register set */
#endif /* CPU_FAMILY==SIMHPPA */

#if CPU_FAMILY==I960			/* ARCHITECTURE DEPENDENT */
    EXC_INFO		excInfo;	/* 0x108: exception info */

    /* REG_SET must be aligned on a 16 byte boundary */

    UINT32		pad0[2];	/* 0x128: pad to align REG_SET */
    REG_SET		regs;		/* 0x130: register set */
    UINT32		dbgPCWSave;	/* 0x?: PCW saved for debugger */
#if CPU==I960KB
    UINT8		intResumptionRec[16];
#endif
#endif /* CPU_FAMILY==I960 */

#if (CPU_FAMILY==I80X86)
    EXC_INFO            excInfo;        /* 0x108: exception info */

    /* REG_SET must be aligned on an 8 byte boundary for PENTIUM */

    REG_SET             regs;           /* 0x160: register set */
#endif /* CPU_FAMILY==I80X86 */

#if (CPU_FAMILY==SH)
    EXC_INFO		excInfo;	/* 0x108: exception info */

    /* REG_SET must be aligned on a 4 byte boundary */

    REG_SET		regs;		/* 0x11c: register set */
#endif	/* CPU_FAMILY==SH */

#if (CPU_FAMILY==ARM)
    EXC_INFO            excInfo;        /* 0x108: exception info */

    UINT32		fpStatus;	/* 0x118: fp status word */

    /* REG_SET must be aligned on a 16 byte boundary */

    UINT32		regSetPad;	/* 0x11c: register set padding */
    REG_SET             regs;           /* 0x120: register set */
#endif /* CPU_FAMILY==ARM */

    struct dspContext *	pDspContext;	/* dsp coprocessor context */
    WDB_INFO		wdbInfo;	/* target debug info */
    } WIND_TCB;

typedef struct 			/* TASK_DESC - information structure */
    {
    int			td_id;		/* task id */
    char *		td_name;	/* name of task */
    int			td_priority;	/* task priority */
    int			td_status;	/* task status */
    int			td_options;	/* task option bits (see below) */
    FUNCPTR		td_entry;	/* original entry point of task */
    char *		td_sp;		/* saved stack pointer */
    char *		td_pStackBase;	/* the bottom of the stack */
    char *		td_pStackLimit;	/* the effective end of the stack */
    char *		td_pStackEnd;	/* the actual end of the stack */
    int			td_stackSize;	/* size of stack in bytes */
    int			td_stackCurrent;/* current stack usage in bytes */
    int			td_stackHigh;	/* maximum stack usage in bytes */
    int			td_stackMargin;	/* current stack margin in bytes */
    int			td_errorStatus;	/* most recent task error status */
    int			td_delay;	/* delay/timeout ticks */
    EVENTS		td_events;	/* VxWorks events information */
    } TASK_DESC;

#if CPU_FAMILY==I960
#pragma align 0			/* turn off alignment requirement */
#endif	/* CPU_FAMILY==I960 */

/*******************************************************************************
*
* TASK_ID_VERIFY - check the validity of a task
*
* This macro verifies the existence of the specified object by comparing the
* task's classId with the expected class id.  If the task does not check
* out, errno is set with the appropriate invalid id status.
*
* RETURNS: OK or ERROR if invalid task id
*
* NOMANUAL
*/

#define TASK_ID_VERIFY(tid)						      \
    (		  							      \
    ((WIND_TCB *)(tid) == NULL) ? (ERROR)                                     \
          : (OBJ_VERIFY (&((WIND_TCB *)(tid))->objCore, taskClassId)) \
    )

/*******************************************************************************
*
* TASK_LOCK - lock preemption for calling task
*
* This macro performs the taskLock (2) routine.
* windview - this marco has been instrumented for windview.
*
* NOMANUAL
*/

#define TASK_LOCK()							     \
    do 									     \
        {								     \
	WRS_ASM ("");							     \
        EVT_CTX_0 (EVENT_TASKLOCK);					     \
	taskIdCurrent->lockCnt++;					     \
	WRS_ASM ("");							     \
        } while (0)

/*******************************************************************************
*
* TASK_UNLOCK - unlock preemption for calling task
*
* This macro performs the taskUnlock (2) routine.
*
* NOMANUAL
*/

#define TASK_UNLOCK()							\
    (									\
    taskUnlock ()							\
    )

/*******************************************************************************
*
* TASK_SAFE - guard self from deletion
*
* This macro performs the taskSafe (2) routine.
* windview - this marco has been instrumented for windview.
*
* NOMANUAL
*/

#define TASK_SAFE()							     \
    do 									     \
        {								     \
	WRS_ASM ("");							     \
        EVT_OBJ_2 (TASK, (WIND_TCB *)taskIdCurrent,		             \
           ((OBJ_ID)(&((WIND_TCB *)taskIdCurrent)->objCore))->pObjClass,     \
                    EVENT_TASKSAFE, taskIdCurrent, taskIdCurrent->safeCnt);  \
        taskIdCurrent->safeCnt++;					     \
	WRS_ASM ("");							     \
        } while (0)

/*******************************************************************************
*
* TASK_UNSAFE - unguard self from deletion
*
* This macro performs the taskUnsafe (2) routine.
*
* NOMANUAL
*/

#define TASK_UNSAFE()							\
    (									\
    taskUnsafe ()							\
    )


/* variable declarations */

extern CLASS_ID	taskClassId;		/* task class id */
extern CLASS_ID taskInstClassId;  	/* instrumented task class id */
extern WIND_TCB *taskIdCurrent;		/* always current executing task */
extern BOOL     taskPriRangeCheck;      /* limit priorities to 0-255 */

/* function declarations */

#if defined(__STDC__) || defined(__cplusplus)

extern STATUS 	taskLibInit (void);

#ifdef	__PROTOTYPE_5_0
extern int 	taskSpawn (char *name, int priority, int options, int stackSize,
		      	   FUNCPTR entryPt, ...);
#else
extern int 	taskSpawn (char *name, int priority, int options, int stackSize,
		      	   FUNCPTR entryPt, int arg1, int arg2, int arg3,
		      	   int arg4, int arg5, int arg6, int arg7,
		      	   int arg8, int arg9, int arg10);
#endif	/* __PROTOTYPE_5_0 */

extern STATUS 	taskInit (WIND_TCB *pTcb, char *name, int priority, int options,
			  char *pStackBase, int stackSize, FUNCPTR entryPt,
			  int arg1, int arg2, int arg3, int arg4, int arg5,
			  int arg6, int arg7, int arg8, int arg9, int arg10);
extern STATUS 	taskActivate (int tid);
extern STATUS 	taskDelete (int tid);
extern STATUS 	taskDeleteForce (int tid);
extern STATUS 	taskSuspend (int tid);
extern STATUS 	taskResume (int tid);
extern STATUS 	taskRestart (int tid);
extern STATUS 	taskPrioritySet (int tid, int newPriority);
extern STATUS 	taskPriorityGet (int tid, int *pPriority);
extern STATUS 	taskLock (void);
extern STATUS 	taskUnlock (void);
extern STATUS 	taskSafe (void);
extern STATUS 	taskUnsafe (void);
extern STATUS 	taskDelay (int ticks);
extern STATUS 	taskOptionsSet (int tid, int mask, int newOptions);
extern STATUS 	taskOptionsGet (int tid, int *pOptions);
extern char *	taskName (int tid);
extern int 	taskNameToId (char *name);
extern STATUS 	taskIdVerify (int tid);
extern int 	taskIdSelf (void);
extern int 	taskIdDefault (int tid);
extern BOOL 	taskIsReady (int tid);
extern BOOL 	taskIsSuspended (int tid);
extern WIND_TCB *taskTcb (int tid);
extern int 	taskIdListGet (int idList [ ], int maxTasks);
extern STATUS 	taskInfoGet (int tid, TASK_DESC *pTaskDesc);
extern STATUS 	taskStatusString (int tid, char *pString);
extern STATUS 	taskOptionsString (int tid, char *pString);
extern STATUS 	taskRegsGet (int tid, REG_SET *pRegs);
extern STATUS 	taskRegsSet (int tid, REG_SET *pRegs);
extern void 	taskRegsShow (int tid);
extern void *	taskStackAllot (int tid, unsigned nBytes);
extern void 	taskShowInit (void);
extern STATUS 	taskShow (int tid, int level);

#else	/* __STDC__ */

extern STATUS 	taskLibInit ();
extern int 	taskSpawn ();
extern STATUS 	taskInit ();
extern STATUS 	taskActivate ();
extern STATUS 	taskDelete ();
extern STATUS 	taskDeleteForce ();
extern STATUS 	taskSuspend ();
extern STATUS 	taskResume ();
extern STATUS 	taskRestart ();
extern STATUS 	taskPrioritySet ();
extern STATUS 	taskPriorityGet ();
extern STATUS 	taskLock ();
extern STATUS 	taskUnlock ();
extern STATUS 	taskSafe ();
extern STATUS 	taskUnsafe ();
extern STATUS 	taskDelay ();
extern STATUS 	taskOptionsSet ();
extern STATUS 	taskOptionsGet ();
extern char *	taskName ();
extern int 	taskNameToId ();
extern STATUS 	taskIdVerify ();
extern int 	taskIdSelf ();
extern int 	taskIdDefault ();
extern BOOL 	taskIsReady ();
extern BOOL 	taskIsSuspended ();
extern WIND_TCB *taskTcb ();
extern int 	taskIdListGet ();
extern STATUS 	taskInfoGet ();
extern STATUS 	taskStatusString ();
extern STATUS 	taskOptionsString ();
extern STATUS 	taskRegsGet ();
extern STATUS 	taskRegsSet ();
extern void 	taskRegsShow ();
extern void *	taskStackAllot ();
extern void 	taskShowInit ();
extern STATUS 	taskShow ();
#endif	/* __STDC__ */

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCtaskLibh */
