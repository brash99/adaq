/* arpLib.h - VxWorks ARP table manipulation header file */

/* Copyright 1990 - 2003 Wind River Systems, Inc. */

/*
modification history
--------------------
02l,13jan03,rae  Merged from velocecp branch
02k,09oct01,rae  merge from truestack
02j,07feb01,spm  added merge record for 30jan01 update from version 02i of
                 tor2_0_x branch (base version 02h) and fixed modification
                 history; replaced printed error message with errno value
02i,30jan01,ijm  merged ARP automatic linking fix (SPR #7576)
02h,22sep92,rrr  added support for c++
02g,07sep92,smb  added include netinet/in.h to remove ANSI warnings
02f,04jul92,jcf  cleaned up.
02e,11jun92,elh  changed parameter to arpCmd.
02d,26may92,rrr  the tree shuffle
02c,04apr92,elh  added arpFlush.
02b,06jan92,elh  passed through the ansification filter
		  -fixed #else and #endif
		  -changed copyright notice
02a,18nov91,elh  written.
*/

#ifndef __INCarpLibh
#define __INCarpLibh

#ifdef __cplusplus
extern "C" {
#endif

#include "vwModNum.h"
#include "netinet/in.h"

#if defined(__STDC__) || defined(__cplusplus)

extern STATUS 	arpLibInit (void);
extern STATUS 	arpAdd (char *host, char *eaddr, int flags);
extern STATUS 	arpDelete (char *host);
extern STATUS 	arpCmd (int cmd, struct in_addr * pIpAddr, u_char *pHwAddr,
			int *pFlags);
extern void 	arpFlush (void);

#else	/* __STDC__ */

extern STATUS 	arpLibInit ();
extern STATUS 	arpAdd ();
extern STATUS 	arpDelete ();
extern STATUS 	arpCmd ();
extern void 	arpFlush ();

#endif	/* __STDC__ */

/* error values */

#define S_arpLib_INVALID_ARGUMENT		(M_arpLib | 1)
#define S_arpLib_INVALID_HOST 			(M_arpLib | 2)
#define S_arpLib_INVALID_ENET_ADDRESS 		(M_arpLib | 3)
#define S_arpLib_INVALID_FLAG			(M_arpLib | 4)

#ifdef __cplusplus
}
#endif

#endif /* __INCarpLibh */
