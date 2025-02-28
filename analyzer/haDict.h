/********************************************************************
* haDict.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error haDict.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
#include "FastAllocString.h"
extern "C" {
extern void G__cpp_setup_tagtablehaDict();
extern void G__cpp_setup_inheritancehaDict();
extern void G__cpp_setup_typetablehaDict();
extern void G__cpp_setup_memvarhaDict();
extern void G__cpp_setup_globalhaDict();
extern void G__cpp_setup_memfunchaDict();
extern void G__cpp_setup_funchaDict();
extern void G__set_cpp_environmenthaDict();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "src/THaFormula.h"
#include "src/THaVform.h"
#include "src/THaVhist.h"
#include "src/THaVar.h"
#include "src/THaVarList.h"
#include "src/THaCut.h"
#include "src/THaNamedList.h"
#include "src/THaCutList.h"
#include "src/THaInterface.h"
#include "src/THaRunBase.h"
#include "src/THaCodaRun.h"
#include "src/THaRun.h"
#include "src/THaRunParameters.h"
#include "src/THaDetMap.h"
#include "src/THaApparatus.h"
#include "src/THaDetector.h"
#include "src/THaSpectrometer.h"
#include "src/THaSpectrometerDetector.h"
#include "src/THaHRS.h"
#include "src/THaDecData.h"
#include "src/BdataLoc.h"
#include "src/THaOutput.h"
#include "src/THaString.h"
#include "src/THaTrackingDetector.h"
#include "src/THaNonTrackingDetector.h"
#include "src/THaPidDetector.h"
#include "src/THaSubDetector.h"
#include "src/THaAnalysisObject.h"
#include "src/THaDetectorBase.h"
#include "src/THaRTTI.h"
#include "src/THaPhysicsModule.h"
#include "src/THaVertexModule.h"
#include "src/THaTrackingModule.h"
#include "src/THaAnalyzer.h"
#include "src/THaPrintOption.h"
#include "src/THaBeam.h"
#include "src/THaIdealBeam.h"
#include "src/THaRasteredBeam.h"
#include "src/THaRaster.h"
#include "src/THaBeamDet.h"
#include "src/THaBPM.h"
#include "src/THaUnRasteredBeam.h"
#include "src/THaTrack.h"
#include "src/THaPIDinfo.h"
#include "src/THaParticleInfo.h"
#include "src/THaCluster.h"
#include "src/THaArrayString.h"
#include "src/THaScintillator.h"
#include "src/THaShower.h"
#include "src/THaTotalShower.h"
#include "src/THaCherenkov.h"
#include "src/THaEvent.h"
#include "src/THaTrackID.h"
#include "src/THaVDC.h"
#include "src/THaVDCPlane.h"
#include "src/THaVDCWire.h"
#include "src/THaVDCHit.h"
#include "src/THaVDCCluster.h"
#include "src/THaVDCTimeToDistConv.h"
#include "src/THaVDCTrackID.h"
#include "src/THaVDCAnalyticTTDConv.h"
#include "src/VDCeff.h"
#include "src/THaElectronKine.h"
#include "src/THaReactionPoint.h"
#include "src/THaReacPointFoil.h"
#include "src/THaTwoarmVertex.h"
#include "src/THaAvgVertex.h"
#include "src/THaExtTarCor.h"
#include "src/THaDebugModule.h"
#include "src/THaTrackInfo.h"
#include "src/THaGoldenTrack.h"
#include "src/THaPrimaryKine.h"
#include "src/THaSecondaryKine.h"
#include "src/THaCoincTime.h"
#include "src/THaS2CoincTime.h"
#include "src/THaTrackProj.h"
#include "src/THaPostProcess.h"
#include "src/THaFilter.h"
#include "src/THaElossCorrection.h"
#include "src/THaTrackEloss.h"
#include "src/THaBeamModule.h"
#include "src/THaBeamInfo.h"
#include "src/THaEpicsEbeam.h"
#include "src/THaBeamEloss.h"
#include "src/THaTrackOut.h"
#include "src/THaTriggerTime.h"
#include "src/THaHelicityDet.h"
#include "src/THaG0HelicityReader.h"
#include "src/THaG0Helicity.h"
#include "src/THaADCHelicity.h"
#include "src/THaHelicity.h"
#include "src/THaPhotoReaction.h"
#include "src/THaSAProtonEP.h"
#include "src/THaTextvars.h"
#include "src/THaQWEAKHelicity.h"
#include "src/THaQWEAKHelicityReader.h"
#include "src/THaEvtTypeHandler.h"
#include "src/THaScalerEvtHandler.h"
#include "src/THaEpicsEvtHandler.h"
#include "src/THaEvt125Handler.h"
#include "src/THaVDCChamber.h"
#include "src/THaVDCPoint.h"
#include "src/THaVDCPointPair.h"
#include "src/THaGlobals.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__haDictLN_TClass;
extern G__linked_taginfo G__haDictLN_TBuffer;
extern G__linked_taginfo G__haDictLN_TMemberInspector;
extern G__linked_taginfo G__haDictLN_TObject;
extern G__linked_taginfo G__haDictLN_TNamed;
extern G__linked_taginfo G__haDictLN_TCollection;
extern G__linked_taginfo G__haDictLN_TString;
extern G__linked_taginfo G__haDictLN_vectorlEshortcOallocatorlEshortgRsPgR;
extern G__linked_taginfo G__haDictLN_vectorlEshortcOallocatorlEshortgRsPgRcLcLiterator;
extern G__linked_taginfo G__haDictLN_vectorlElongsPlongcOallocatorlElongsPlonggRsPgR;
extern G__linked_taginfo G__haDictLN_vectorlEunsignedsPshortcOallocatorlEunsignedsPshortgRsPgR;
extern G__linked_taginfo G__haDictLN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR;
extern G__linked_taginfo G__haDictLN_vectorlEfloatcOallocatorlEfloatgRsPgR;
extern G__linked_taginfo G__haDictLN_vectorlEdoublecOallocatorlEdoublegRsPgR;
extern G__linked_taginfo G__haDictLN_vectorlEboolcOallocatorlEboolgRsPgR;
extern G__linked_taginfo G__haDictLN_basic_ofstreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__haDictLN_string;
extern G__linked_taginfo G__haDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_va_list;
extern G__linked_taginfo G__haDictLN_TList;
extern G__linked_taginfo G__haDictLN_TObjArray;
extern G__linked_taginfo G__haDictLN_TClonesArray;
extern G__linked_taginfo G__haDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_TSeqCollection;
extern G__linked_taginfo G__haDictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR;
extern G__linked_taginfo G__haDictLN_TFormula;
extern G__linked_taginfo G__haDictLN_THaVarList;
extern G__linked_taginfo G__haDictLN_THaCutList;
extern G__linked_taginfo G__haDictLN_THaRunBase;
extern G__linked_taginfo G__haDictLN_THaDB;
extern G__linked_taginfo G__haDictLN_THaTextvars;
extern G__linked_taginfo G__haDictLN_THaVar;
extern G__linked_taginfo G__haDictLN_THaFormula;
extern G__linked_taginfo G__haDictLN_THaFormulacLcLdA;
extern G__linked_taginfo G__haDictLN_THaFormulacLcLEVariableType;
extern G__linked_taginfo G__haDictLN_vectorlETHaFormulacLcLFVarDef_tcOallocatorlETHaFormulacLcLFVarDef_tgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETHaFormulacLcLFVarDef_tcOallocatorlETHaFormulacLcLFVarDef_tgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_maplEstringcOfloatcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOfloatgRsPgRsPgR;
extern G__linked_taginfo G__haDictLN_pairlEdoublecOintgR;
extern G__linked_taginfo G__haDictLN_TH1F;
extern G__linked_taginfo G__haDictLN_THaVform;
extern G__linked_taginfo G__haDictLN_THaVhist;
extern G__linked_taginfo G__haDictLN_THaEvData;
extern G__linked_taginfo G__haDictLN_TTree;
extern G__linked_taginfo G__haDictLN_THaEvtTypeHandler;
extern G__linked_taginfo G__haDictLN_THaOdata;
extern G__linked_taginfo G__haDictLN_THaEpicsEvtHandler;
extern G__linked_taginfo G__haDictLN_THaOutput;
extern G__linked_taginfo G__haDictLN_vectorlEstringcOallocatorlEstringgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlEstringcOallocatorlEstringgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_vectorlETHaVarmUcOallocatorlETHaVarmUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETHaVarmUcOallocatorlETHaVarmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_vectorlETHaVformmUcOallocatorlETHaVformmUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETHaVformmUcOallocatorlETHaVformmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_vectorlETHaVhistmUcOallocatorlETHaVhistmUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETHaVhistmUcOallocatorlETHaVhistmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_vectorlETHaOdatamUcOallocatorlETHaOdatamUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETHaOdatamUcOallocatorlETHaOdatamUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_vectorlETHaEpicsKeymUcOallocatorlETHaEpicsKeymUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETHaEpicsKeymUcOallocatorlETHaEpicsKeymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_THaOutputcLcLEId;
extern G__linked_taginfo G__haDictLN_TFile;
extern G__linked_taginfo G__haDictLN_TArrayD;
extern G__linked_taginfo G__haDictLN_TArrayI;
extern G__linked_taginfo G__haDictLN_TDataMember;
extern G__linked_taginfo G__haDictLN_TRealData;
extern G__linked_taginfo G__haDictLN_TMethodCall;
extern G__linked_taginfo G__haDictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR;
extern G__linked_taginfo G__haDictLN_THaCut;
extern G__linked_taginfo G__haDictLN_THaVformcLcLFTy;
extern G__linked_taginfo G__haDictLN_THaVformcLcLFAr;
extern G__linked_taginfo G__haDictLN_THaVformcLcLFAp;
extern G__linked_taginfo G__haDictLN_THaVformcLcLFEr;
extern G__linked_taginfo G__haDictLN_vectorlEintcOallocatorlEintgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_vectorlETHaFormulamUcOallocatorlETHaFormulamUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETHaFormulamUcOallocatorlETHaFormulamUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_vectorlETHaCutmUcOallocatorlETHaCutmUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETHaCutmUcOallocatorlETHaCutmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_THaVhistcLcLFEr;
extern G__linked_taginfo G__haDictLN_vectorlETH1mUcOallocatorlETH1mUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETH1mUcOallocatorlETH1mUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_THaArrayString;
extern G__linked_taginfo G__haDictLN_THaArrayStringcLcLEStatus;
extern G__linked_taginfo G__haDictLN_VarType;
extern G__linked_taginfo G__haDictLN_THashList;
extern G__linked_taginfo G__haDictLN_VarDef;
extern G__linked_taginfo G__haDictLN_RVarDef;
extern G__linked_taginfo G__haDictLN_DBRequest;
extern G__linked_taginfo G__haDictLN_THaCutcLcLEvalMode;
extern G__linked_taginfo G__haDictLN_THaNamedList;
extern G__linked_taginfo G__haDictLN_THaPrintOption;
extern G__linked_taginfo G__haDictLN_THaHashList;
extern G__linked_taginfo G__haDictLN_THaCutListcLcLEWarnMode;
extern G__linked_taginfo G__haDictLN_TQObject;
extern G__linked_taginfo G__haDictLN_TApplication;
extern G__linked_taginfo G__haDictLN_TRint;
extern G__linked_taginfo G__haDictLN_Decoder;
extern G__linked_taginfo G__haDictLN_DecodercLcLTHaEpics;
extern G__linked_taginfo G__haDictLN_DecodercLcLTHaCodaData;
extern G__linked_taginfo G__haDictLN_DecodercLcLTHaCodaFile;
extern G__linked_taginfo G__haDictLN_THaInterface;
extern G__linked_taginfo G__haDictLN_TDatime;
extern G__linked_taginfo G__haDictLN_THaRunParameters;
extern G__linked_taginfo G__haDictLN_THaRunBasecLcLdA;
extern G__linked_taginfo G__haDictLN_THaRunBasecLcLEInfoType;
extern G__linked_taginfo G__haDictLN_THaCodaRun;
extern G__linked_taginfo G__haDictLN_THaRun;
extern G__linked_taginfo G__haDictLN_THaDetMap;
extern G__linked_taginfo G__haDictLN_THaDetMapcLcLModule;
extern G__linked_taginfo G__haDictLN_THaDetMapcLcLECountMode;
extern G__linked_taginfo G__haDictLN_THaDetMapcLcLEFillFlags;
extern G__linked_taginfo G__haDictLN_TVector3;
extern G__linked_taginfo G__haDictLN_THaAnalysisObject;
extern G__linked_taginfo G__haDictLN_THaAnalysisObjectcLcLEStatus;
extern G__linked_taginfo G__haDictLN_THaAnalysisObjectcLcLEType;
extern G__linked_taginfo G__haDictLN_THaAnalysisObjectcLcLEMode;
extern G__linked_taginfo G__haDictLN_THaAnalysisObjectcLcLEProperties;
extern G__linked_taginfo G__haDictLN_THaDetector;
extern G__linked_taginfo G__haDictLN_THaApparatus;
extern G__linked_taginfo G__haDictLN_TMatrixTBaselEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTBaselEdoublegR;
extern G__linked_taginfo G__haDictLN_TVectorTlEfloatgR;
extern G__linked_taginfo G__haDictLN_TVectorTlEdoublegR;
extern G__linked_taginfo G__haDictLN_TElementActionTlEfloatgR;
extern G__linked_taginfo G__haDictLN_TElementPosActionTlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTRow_constlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTRowlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTDiag_constlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTColumn_constlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTFlat_constlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTSub_constlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTSparseRow_constlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTSparseDiag_constlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTColumnlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTDiaglEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTFlatlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTSublEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTSparseRowlEfloatgR;
extern G__linked_taginfo G__haDictLN_TMatrixTSparseDiaglEfloatgR;
extern G__linked_taginfo G__haDictLN_TRotation;
extern G__linked_taginfo G__haDictLN_THaDetectorBase;
extern G__linked_taginfo G__haDictLN_TRef;
extern G__linked_taginfo G__haDictLN_THaVertexModule;
extern G__linked_taginfo G__haDictLN_THaTrack;
extern G__linked_taginfo G__haDictLN_THaSpectrometer;
extern G__linked_taginfo G__haDictLN_THaTrackInfo;
extern G__linked_taginfo G__haDictLN_THaTrackingModule;
extern G__linked_taginfo G__haDictLN_THaParticleInfo;
extern G__linked_taginfo G__haDictLN_THaSpectrometerDetector;
extern G__linked_taginfo G__haDictLN_THaNonTrackingDetector;
extern G__linked_taginfo G__haDictLN_THaPidDetector;
extern G__linked_taginfo G__haDictLN_THaSpectrometercLcLEStagesDone;
extern G__linked_taginfo G__haDictLN_THaHRS;
extern G__linked_taginfo G__haDictLN_BdataLoc;
extern G__linked_taginfo G__haDictLN_BdataLoccLcLBdataLocType;
extern G__linked_taginfo G__haDictLN_setlEBdataLoccLcLBdataLocTypecOlesslEBdataLoccLcLBdataLocTypegRcOallocatorlEBdataLoccLcLBdataLocTypegRsPgR;
extern G__linked_taginfo G__haDictLN_setlEBdataLoccLcLBdataLocTypecOlesslEBdataLoccLcLBdataLocTypegRcOallocatorlEBdataLoccLcLBdataLocTypegRsPgRcLcLiterator;
extern G__linked_taginfo G__haDictLN_BdataLoccLcLdA;
extern G__linked_taginfo G__haDictLN_CrateLoc;
extern G__linked_taginfo G__haDictLN_CrateLocMulti;
extern G__linked_taginfo G__haDictLN_WordLoc;
extern G__linked_taginfo G__haDictLN_RoclenLoc;
extern G__linked_taginfo G__haDictLN_TrigBitLoc;
extern G__linked_taginfo G__haDictLN_THaDecData;
extern G__linked_taginfo G__haDictLN_THaTrackID;
extern G__linked_taginfo G__haDictLN_THaTrackingDetector;
extern G__linked_taginfo G__haDictLN_THaSubDetector;
extern G__linked_taginfo G__haDictLN_THaRTTI;
extern G__linked_taginfo G__haDictLN_THaRTTIcLcLEArrayType;
extern G__linked_taginfo G__haDictLN_THaPhysicsModule;
extern G__linked_taginfo G__haDictLN_THaPhysicsModulecLcLESpecialRetval;
extern G__linked_taginfo G__haDictLN_THaEvent;
extern G__linked_taginfo G__haDictLN_THaBenchmark;
extern G__linked_taginfo G__haDictLN_THaPostProcess;
extern G__linked_taginfo G__haDictLN_THaAnalyzer;
extern G__linked_taginfo G__haDictLN_THaAnalyzercLcLERetVal;
extern G__linked_taginfo G__haDictLN_THaAnalyzercLcLdA;
extern G__linked_taginfo G__haDictLN_THaAnalyzercLcLStage_t;
extern G__linked_taginfo G__haDictLN_THaAnalyzercLcLCounter_t;
extern G__linked_taginfo G__haDictLN_THaAnalyzercLcLECountMode;
extern G__linked_taginfo G__haDictLN_THaBeam;
extern G__linked_taginfo G__haDictLN_THaBeamInfo;
extern G__linked_taginfo G__haDictLN_THaBeamModule;
extern G__linked_taginfo G__haDictLN_THaIdealBeam;
extern G__linked_taginfo G__haDictLN_THaRasteredBeam;
extern G__linked_taginfo G__haDictLN_THaBeamDet;
extern G__linked_taginfo G__haDictLN_THaRaster;
extern G__linked_taginfo G__haDictLN_THaBPM;
extern G__linked_taginfo G__haDictLN_THaUnRasteredBeam;
extern G__linked_taginfo G__haDictLN_vectorlETVector3cOallocatorlETVector3gRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETVector3cOallocatorlETVector3gRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_THaPIDinfo;
extern G__linked_taginfo G__haDictLN_THaCluster;
extern G__linked_taginfo G__haDictLN_THaTrackcLcLdA;
extern G__linked_taginfo G__haDictLN_THaScintillator;
extern G__linked_taginfo G__haDictLN_THaScintillatorcLcLDataDest;
extern G__linked_taginfo G__haDictLN_THaScintillatorcLcLESide;
extern G__linked_taginfo G__haDictLN_THaShower;
extern G__linked_taginfo G__haDictLN_vectorlEvectorlEunsignedsPshortcOallocatorlEunsignedsPshortgRsPgRcOallocatorlEvectorlEunsignedsPshortcOallocatorlEunsignedsPshortgRsPgRsPgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlEvectorlEunsignedsPshortcOallocatorlEunsignedsPshortgRsPgRcOallocatorlEvectorlEunsignedsPshortcOallocatorlEunsignedsPshortgRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_THaTotalShower;
extern G__linked_taginfo G__haDictLN_THaCherenkov;
extern G__linked_taginfo G__haDictLN_THaEventHeader;
extern G__linked_taginfo G__haDictLN_THaEventcLcLDataMap;
extern G__linked_taginfo G__haDictLN_THaVDCChamber;
extern G__linked_taginfo G__haDictLN_THaVDCPoint;
extern G__linked_taginfo G__haDictLN_THaVDC;
extern G__linked_taginfo G__haDictLN_THaVDCcLcLdA;
extern G__linked_taginfo G__haDictLN_THaVDCcLcLECoordType;
extern G__linked_taginfo G__haDictLN_THaVDCcLcLEFPMatrixElemTag;
extern G__linked_taginfo G__haDictLN_vectorlETHaVDCcLcLTHaMatrixElementcOallocatorlETHaVDCcLcLTHaMatrixElementgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETHaVDCcLcLTHaMatrixElementcOallocatorlETHaVDCcLcLTHaMatrixElementgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_VDC;
extern G__linked_taginfo G__haDictLN_VDCcLcLTimeToDistConv;
extern G__linked_taginfo G__haDictLN_THaVDCWire;
extern G__linked_taginfo G__haDictLN_THaVDCHit;
extern G__linked_taginfo G__haDictLN_THaVDCPlane;
extern G__linked_taginfo G__haDictLN_THaVDCPointPair;
extern G__linked_taginfo G__haDictLN_vectorlETHaVDCHitmUcOallocatorlETHaVDCHitmUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlETHaVDCHitmUcOallocatorlETHaVDCHitmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_vectorlEVDCcLcLFitCoord_tcOallocatorlEVDCcLcLFitCoord_tgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlEVDCcLcLFitCoord_tcOallocatorlEVDCcLcLFitCoord_tgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_THaVDCCluster;
extern G__linked_taginfo G__haDictLN_THaVDCClustercLcLEMode;
extern G__linked_taginfo G__haDictLN_THaTriggerTime;
extern G__linked_taginfo G__haDictLN_THaVDCTrackID;
extern G__linked_taginfo G__haDictLN_VDCcLcLAnalyticTTDConv;
extern G__linked_taginfo G__haDictLN_VDCeff;
extern G__linked_taginfo G__haDictLN_vectorlEVDCeffcLcLVDCvar_tcOallocatorlEVDCeffcLcLVDCvar_tgRsPgR;
extern G__linked_taginfo G__haDictLN_vectorlEVDCeffcLcLVDCvar_tcOallocatorlEVDCeffcLcLVDCvar_tgRsPgRcLcLiterator;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlEVDCeffcLcLVDCvar_tcOallocatorlEVDCeffcLcLVDCvar_tgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_TLorentzVector;
extern G__linked_taginfo G__haDictLN_THaPrimaryKine;
extern G__linked_taginfo G__haDictLN_THaElectronKine;
extern G__linked_taginfo G__haDictLN_THaReactionPoint;
extern G__linked_taginfo G__haDictLN_THaReacPointFoil;
extern G__linked_taginfo G__haDictLN_THaTwoarmVertex;
extern G__linked_taginfo G__haDictLN_THaAvgVertex;
extern G__linked_taginfo G__haDictLN_THaExtTarCor;
extern G__linked_taginfo G__haDictLN_THaDebugModule;
extern G__linked_taginfo G__haDictLN_THaDebugModulecLcLEFlags;
extern G__linked_taginfo G__haDictLN_vectorlEconstsPTObjectmUcOallocatorlEconstsPTObjectmUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlEconstsPTObjectmUcOallocatorlEconstsPTObjectmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_THaGoldenTrack;
extern G__linked_taginfo G__haDictLN_THaSecondaryKine;
extern G__linked_taginfo G__haDictLN_THaCoincTime;
extern G__linked_taginfo G__haDictLN_THaS2CoincTime;
extern G__linked_taginfo G__haDictLN_THaTrackProj;
extern G__linked_taginfo G__haDictLN_THaFilter;
extern G__linked_taginfo G__haDictLN_THaElossCorrection;
extern G__linked_taginfo G__haDictLN_THaTrackEloss;
extern G__linked_taginfo G__haDictLN_THaEpicsEbeam;
extern G__linked_taginfo G__haDictLN_THaBeamEloss;
extern G__linked_taginfo G__haDictLN_THaTrackOut;
extern G__linked_taginfo G__haDictLN_THaHelicityDet;
extern G__linked_taginfo G__haDictLN_THaHelicityDetcLcLEHelicity;
extern G__linked_taginfo G__haDictLN_THaG0HelicityReader;
extern G__linked_taginfo G__haDictLN_THaG0HelicityReadercLcLROCinfo;
extern G__linked_taginfo G__haDictLN_THaG0HelicityReadercLcLEROC;
extern G__linked_taginfo G__haDictLN_THaG0Helicity;
extern G__linked_taginfo G__haDictLN_THaG0HelicitycLcLdA;
extern G__linked_taginfo G__haDictLN_THaADCHelicity;
extern G__linked_taginfo G__haDictLN_THaADCHelicitycLcLChanDef_t;
extern G__linked_taginfo G__haDictLN_THaHelicity;
extern G__linked_taginfo G__haDictLN_THaPhotoReaction;
extern G__linked_taginfo G__haDictLN_THaSAProtonEP;
extern G__linked_taginfo G__haDictLN_maplEstringcOvectorlEstringcOallocatorlEstringgRsPgRcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOvectorlEstringcOallocatorlEstringgRsPgRsPgRsPgRsPgR;
extern G__linked_taginfo G__haDictLN_THaQWEAKHelicityReader;
extern G__linked_taginfo G__haDictLN_THaQWEAKHelicityReadercLcLROCinfo;
extern G__linked_taginfo G__haDictLN_THaQWEAKHelicityReadercLcLEROC;
extern G__linked_taginfo G__haDictLN_THaQWEAKHelicityReadercLcLdA;
extern G__linked_taginfo G__haDictLN_THaQWEAKHelicity;
extern G__linked_taginfo G__haDictLN_THaScalerEvtHandler;
extern G__linked_taginfo G__haDictLN_vectorlEDecodercLcLGenScalermUcOallocatorlEDecodercLcLGenScalermUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlEDecodercLcLGenScalermUcOallocatorlEDecodercLcLGenScalermUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_vectorlEScalerLocmUcOallocatorlEScalerLocmUgRsPgR;
extern G__linked_taginfo G__haDictLN_reverse_iteratorlEvectorlEScalerLocmUcOallocatorlEScalerLocmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__haDictLN_THaEvt125Handler;
extern G__linked_taginfo G__haDictLN_PointCoords_t;

/* STUB derived class for protected member access */
