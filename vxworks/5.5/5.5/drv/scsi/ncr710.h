/* ncr710.h - NCR 710 Script SCSI Controller header file */

/* Copyright 1984-2002 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,26apr02,dat  Adding cplusplus protection, SPR 75017
01a,21oct94,jds  Created for backward compatability
*/

#ifndef __INCncr710h
#define __INCncr710h

#ifndef INCLUDE_SCSI2

#include "ncr710_1.h"

#else

#include "ncr710_2.h"

#endif /* INCLUDE_SCSI2 */

#endif /* __INCncr710h */
