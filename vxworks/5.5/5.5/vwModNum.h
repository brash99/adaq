/* vwModNum.h - VxWorks module numbers */

/* Copyright 1984-2002 Wind River Systems, Inc. */

/*
modification history
--------------------
04q,01nov02,vvv  added M_fastUdpLib
04p,20jun02,jws  Added HA related numbers. This creates a gap.
04o,14mar02,elr  Added M_ftpDrv
04n,12oct01,rae  Added M_fastPathLib
04m,09oct01,bwa  Added M_eventLib
04l,24sep01,jkf  added M_cbioLib.
04k,02may01,wef  changed usb error codes to M_usbHostLib and M_usbTargLib
04j,05dec00,wef  added usb error codes
04i,04may01,dlk  Added M_qPriMaskLib.
04h,12dec00,agf  Adding #define for M_devCfgLib, required as part of the
                 es.coretools merge
04g,24apr00,gnn  merged in from tor2_0.open_stack
04f,28mar00,rae  merging in IGMPv2 changes
04e,14jan00,brx  added M_rarpLib definition 
04d,03sep99,mas  removed dmsObjLib following merge of it with dmsLib.
04c,08jul99,mas  added M_poolLib, M_setLib, M_dmsObjLib, M_dmsLib
04b,14jun99,cn   added M_miiLib.
04a,05oct98,jmp  added M_if_ul definition (SPR #22585).
03z,19may98,drm  merged 3rd party changes for distributed message queues
                 - added M_msgQDistLib, M_distNameLib, M_msgQDistGrpLib,
                   and M_distLib
                 - merged code was originally based on version 03s
03y,04May98,cym  added M_loadPecoffLib
03x,13nov97,vin  added M_netBufLib
03w,15jul97,spm  added M_sntpcLib and M_sntpsLib
03v,16may97,vin  added M_dhcpsLib.
03u,27apr97,rjc  added M_ospfLib.
03t,07apr97,gnn  added M_endLib, M_muxLib, and M_m2RipLib.
03s,14feb97,rjc  added M_snmpdLib
03r,13dec96,jag  added M_resolvLib 
03q,17oct96,spm  added M_dhcpcLib
03s,02jul97,ms   moved M_cdromFs here from cdromFsLib.h
03q,29nov96,p_m  added to a comment to tell people to go edit errnoTbl.tcl
03r,01nov96,hdn  added M_pcmciaLib.
03q,22aug95,jds  added M_tapeFsLib
03p,23may96,rjc  added M_snmpdLib
03o,30nov95,vin  added M_pppHookLib.
03n,09may95,dzb  added M_pppSecretLib.
03m,29mar95,sgv  added M_strmLib.
03l,04nov94,dzb  added M_mbufLib and M_pingLib.
03k,15oct93,cd   added M_loadElfLib.
03j,04nov93,yao  added M_loadSomCoffLib.
03i,22apr94,jmm  added M_nfsdLib and M_mountLib
03h,14feb94,jag  added M_ for MIB-II library.
03g,22sep92,rrr  added support for c++
03f,01aug92,srh  added M_cxxLib
03e,19jul92,pme  added M_smObjLib and M_smNameLib.
03d,15jul92,jmm  added M_moduleLib and M_unldLib.
03c,08jul92,rdc  added M_vmLib and M_mmuLib.
03b,30jun92,wmd  added M_loadCoffLib.
03a,09jun92,ajm  added M_loadEcoffLib, M_loadAoutLib, M_loadBoutLib,
		  and M_bootLoadLib
02z,02jun92,elh  added M_smPktLib, and M_smUtilLib, renamed M_shMemLib
		 to M_smLib.
02y,26may92,rrr  the tree shuffle
02x,28feb92,elh  added M_bootpLib, M_tftpLib, and M_icmpLib
02w,04feb92,elh  added M_shMemLib, changed year on copyright
02v,07jan92,elh  added M_arpTblLib
02u,04oct91,rrr  passed through the ansification filter
		  -changed copyright notice
02t,05oct90,shl  added copyright notice.
                 made #endif ANSI style.
02s,02oct90,kdl	 added M_rawFsLib.
02r,09aug90,kdl	 changed M_msdosLib to M_dosFsLib, changed M_rt11Lib to
		 M_rt11FsLib.
02q,18jul90,jcf	 added M_cacheLib
02p,26jun90,jcf	 removed M_semCore, M_semILib. added M_semLib.
02o,04may90,kdl	 added M_dirLib.
02n,18mar90,jcf  added M_hashLib, M_qLib, M_tickLib, M_objLib,
		  M_qPriHeapLib, M_qPriBMapLib, M_bufLib,
		  M_msgQLib, M_classLib
02m,16mar90,kdl  added M_scsiLib, M_msdosLib.
	    rdc  added M_selectLib.
02l,13mar90,shl  added M_loginLib.
02k,27jul89,hjb  added M_if_sl.
02j,25may89,gae  added M_ftnLib.
02i,04jun88,dnw  changed ldLib->loadLib, rtLib->rt11Lib, stsLib->errnoLib.
		 changed M_errno, M_taskLib, M_vrtx, M_psos.
02h,29may88,dnw  added M_sigLib.
		 deleted some obsolete modules.
02g,28may88,dnw  added M_hostLib.
02f,29apr88,gae  added M_stsLib.
02e,19apr88,llk  added M_nfsDrv, M_nfsLib, M_rpcClntStat, M_nfsStat.
02d,25jan88,jcf  added M_taskLib.
02c,01nov87,llk  added M_inetLib and M_routeLib.
...pre 1987 mod history removed.  See RCS.
*/

#ifndef __INCvwModNumh
#define __INCvwModNumh

#ifdef __cplusplus
extern "C" {
#endif

/* module numbers - DO NOT CHANGE NUMBERS! Add or delete only! */
#define M_errno			(0 << 16)	/* THIS MUST BE ZERO! */
#define M_kernel		(1 << 16)
#define M_taskLib		(3 << 16)
#define M_dbgLib		(4 << 16)
#define M_dsmLib		(7 << 16)
#define M_fioLib		(9 << 16)
#define M_ioLib			(12 << 16)
#define M_iosLib		(13 << 16)
#define M_loadLib		(14 << 16)
#define M_lstLib		(15 << 16)
#define M_memLib		(17 << 16)
#define M_rngLib		(19 << 16)
#define M_rt11FsLib		(20 << 16)
#define M_rt11ULib		(21 << 16)
#define M_semLib		(22 << 16)
#define M_vwModNum		(27 << 16)
#define M_symLib		(28 << 16)
#define M_tyLib			(31 << 16)
#define M_wdLib			(34 << 16)
#define M_usrLib		(35 << 16)
#define M_remLib		(37 << 16)
#define M_netDrv		(41 << 16)
#define M_inetLib		(43 << 16)
#define M_routeLib		(44 << 16)
#define M_nfsDrv		(45 << 16)
#define M_nfsLib		(46 << 16)
#define M_rpcClntStat		(47 << 16)
#define M_nfsStat		(48 << 16)
#define M_errnoLib		(49 << 16)
#define M_hostLib		(50 << 16)
#define M_sigLib		(51 << 16)
#define M_ftnLib		(52 << 16)
#define M_if_sl			(53 << 16)
#define M_loginLib		(54 << 16)
#define M_scsiLib		(55 << 16)
#define M_dosFsLib		(56 << 16)
#define M_selectLib		(57 << 16)
#define M_hashLib		(58 << 16)
#define M_qLib			(59 << 16)
#define M_tickLib		(60 << 16)
#define M_objLib		(61 << 16)
#define M_qPriHeapLib		(62 << 16)
#define M_qPriBMapLib		(63 << 16)
#define M_bufLib		(64 << 16)
#define M_msgQLib		(65 << 16)
#define M_classLib		(66 << 16)
#define M_intLib		(67 << 16)
#define M_dirLib		(68 << 16)
#define M_cacheLib		(69 << 16)
#define M_rawFsLib		(70 << 16)
#define M_arpLib		(71 << 16)
#define M_smLib			(72 << 16)
#define M_bootpLib		(73 << 16)
#define M_icmpLib		(74 << 16)
#define M_tftpLib		(75 << 16)
#define M_proxyArpLib		(76 << 16)
#define M_smUtilLib		(77 << 16)
#define M_smPktLib		(78 << 16)
#define M_loadEcoffLib		(79 << 16)
#define M_loadAoutLib		(80 << 16)
#define M_loadBoutLib		(81 << 16)
#define M_bootLoadLib		(82 << 16)
#define M_loadCoffLib		(83 << 16)
#define M_vmLib			(84 << 16)
#define M_mmuLib		(85 << 16)
#define M_moduleLib		(86 << 16)
#define M_unldLib		(87 << 16)
#define M_smObjLib		(88 << 16)
#define M_smNameLib		(89 << 16)
#define M_cplusLib		(90 << 16)
#define M_m2Lib			(91 << 16)
#define M_aioPxLib		(92 << 16)
#define M_loadAoutHppaLib	(93 << 16)
#define M_mountLib		(94 << 16)
#define M_nfsdLib		(95 << 16)
#define M_loadSomCoffLib	(96 << 16)
#define M_loadElfLib		(97 << 16)
#define M_mbufLib		(98 << 16)
#define M_pingLib		(99 << 16)
#define M_strmLib		(100 << 16)
#define M_pppSecretLib		(101 << 16)
#define M_pppHookLib		(102 << 16)
#define M_tapeFsLib		(103 << 16)
#define M_snmpdLib		(104 << 16)
#define M_pcmciaLib		(105 << 16)
#define M_dhcpcLib		(106 << 16)
#define M_resolvLib		(107 << 16)
#define M_endLib		(108 << 16)
#define M_muxLib		(109 << 16)
#define M_m2RipLib		(110 << 16)
#define M_ospfLib		(111 << 16)
#define M_dhcpsLib		(112 << 16)
#define M_sntpcLib		(113 << 16)
#define M_sntpsLib		(114 << 16)
#define M_netBufLib		(115 << 16)
#define M_cdromFsLib		(116 << 16)
#define M_loadPecoffLib		(117 << 16)
#define M_distLib		(118 << 16)
#define M_distNameLib		(119 << 16)
#define M_msgQDistGrpLib	(120 << 16)
#define M_msgQDistLib		(121 << 16)
#define M_if_ul			(122 << 16)
#define M_miiLib                (123 << 16)
#define M_poolLib		(124 << 16)
#define M_setLib		(125 << 16)
#define M_dmsLib		(126 << 16)
#define M_rarpLib		(127 << 16)
#define M_igmpRouterLib         (128 << 16)
#define M_devCfgLib             (129 << 16)
#define M_qPriMaskLib		(130 << 16)
#define M_usbHostLib		(131 << 16)
#define M_usbPeriphLib		(132 << 16)
#define M_cbioLib		(133 << 16)
#define M_eventLib		(134 << 16)
#define M_fastPathLib           (135 << 16)
#define M_ftpLib           	(136 << 16)
#define M_fastUdpLib            (137 << 16)

#define M_alarmLib              (153 << 16)
#define M_amsLib                (154 << 16)
#define M_fmsLib                (155 << 16)
#define M_hsiLib                (156 << 16)
#define M_hsmsLib               (157 << 16)
#define M_omsLib                (158 << 16)
#define M_rpmLib                (159 << 16)
#define M_umsLib                (160 << 16)
#define M_cmsLib                (161 << 16)
#define M_mmsLib                (162 << 16)
#define M_pspLib                (163 << 16)
#define M_xcomLib               (164 << 16)
#define M_ipsLib                (165 << 16)
#define M_rdsLib                (166 << 16)
#define M_grmLib                (167 << 16)
#define M_mtpLib                (168 << 16)
#define M_exEngLib              (169 << 16)
#define M_tplLib		(170 << 16)
#define M_smmLib		(171 << 16)
#define M_graLib		(172 << 16)


/* 
 * WARNING: If you add a module number above please add the new module
 * error strings in the file: host/resource/tcl/errnoTbl.tcl so that
 * windsh knows about the new error codes.
 */

#ifdef __cplusplus
}
#endif

#endif /* __INCvwModNumh */
