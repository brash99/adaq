// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME THaDecDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "hana_decode/THaUsrstrutils.h"
#include "hana_decode/THaCrateMap.h"
#include "hana_decode/THaCodaData.h"
#include "hana_decode/THaEpics.h"
#include "hana_decode/THaCodaFile.h"
#include "hana_decode/SimDecoder.h"
#include "hana_decode/THaSlotData.h"
#include "hana_decode/THaEvData.h"
#include "hana_decode/CodaDecoder.h"
#include "hana_decode/Module.h"
#include "hana_decode/VmeModule.h"
#include "hana_decode/FastbusModule.h"
#include "hana_decode/Lecroy1877Module.h"
#include "hana_decode/Lecroy1881Module.h"
#include "hana_decode/Lecroy1875Module.h"
#include "hana_decode/Fadc250Module.h"
#include "hana_decode/GenScaler.h"
#include "hana_decode/Scaler560.h"
#include "hana_decode/Scaler1151.h"
#include "hana_decode/Scaler3800.h"
#include "hana_decode/Scaler3801.h"
#include "hana_decode/F1TDCModule.h"
#include "hana_decode/Caen1190Module.h"
#include "hana_decode/Caen775Module.h"
#include "hana_decode/Caen792Module.h"
#include "hana_decode/THaBenchmark.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_DecodercLcLTHaUsrstrutils(void *p = nullptr);
   static void *newArray_DecodercLcLTHaUsrstrutils(Long_t size, void *p);
   static void delete_DecodercLcLTHaUsrstrutils(void *p);
   static void deleteArray_DecodercLcLTHaUsrstrutils(void *p);
   static void destruct_DecodercLcLTHaUsrstrutils(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::THaUsrstrutils*)
   {
      ::Decoder::THaUsrstrutils *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::THaUsrstrutils >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::THaUsrstrutils", ::Decoder::THaUsrstrutils::Class_Version(), "THaUsrstrutils.h", 20,
                  typeid(::Decoder::THaUsrstrutils), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::THaUsrstrutils::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::THaUsrstrutils) );
      instance.SetNew(&new_DecodercLcLTHaUsrstrutils);
      instance.SetNewArray(&newArray_DecodercLcLTHaUsrstrutils);
      instance.SetDelete(&delete_DecodercLcLTHaUsrstrutils);
      instance.SetDeleteArray(&deleteArray_DecodercLcLTHaUsrstrutils);
      instance.SetDestructor(&destruct_DecodercLcLTHaUsrstrutils);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::THaUsrstrutils*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::THaUsrstrutils*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::THaUsrstrutils*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLTHaCrateMap(void *p = nullptr);
   static void *newArray_DecodercLcLTHaCrateMap(Long_t size, void *p);
   static void delete_DecodercLcLTHaCrateMap(void *p);
   static void deleteArray_DecodercLcLTHaCrateMap(void *p);
   static void destruct_DecodercLcLTHaCrateMap(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::THaCrateMap*)
   {
      ::Decoder::THaCrateMap *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::THaCrateMap >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::THaCrateMap", ::Decoder::THaCrateMap::Class_Version(), "THaCrateMap.h", 28,
                  typeid(::Decoder::THaCrateMap), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::THaCrateMap::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::THaCrateMap) );
      instance.SetNew(&new_DecodercLcLTHaCrateMap);
      instance.SetNewArray(&newArray_DecodercLcLTHaCrateMap);
      instance.SetDelete(&delete_DecodercLcLTHaCrateMap);
      instance.SetDeleteArray(&deleteArray_DecodercLcLTHaCrateMap);
      instance.SetDestructor(&destruct_DecodercLcLTHaCrateMap);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::THaCrateMap*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::THaCrateMap*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::THaCrateMap*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_DecodercLcLTHaCodaData(void *p);
   static void deleteArray_DecodercLcLTHaCodaData(void *p);
   static void destruct_DecodercLcLTHaCodaData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::THaCodaData*)
   {
      ::Decoder::THaCodaData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::THaCodaData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::THaCodaData", ::Decoder::THaCodaData::Class_Version(), "THaCodaData.h", 36,
                  typeid(::Decoder::THaCodaData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::THaCodaData::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::THaCodaData) );
      instance.SetDelete(&delete_DecodercLcLTHaCodaData);
      instance.SetDeleteArray(&deleteArray_DecodercLcLTHaCodaData);
      instance.SetDestructor(&destruct_DecodercLcLTHaCodaData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::THaCodaData*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::THaCodaData*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::THaCodaData*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *_Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR_Dictionary();
   static void _Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR_TClassManip(TClass*);
   static void *new__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR(void *p = nullptr);
   static void *newArray__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR(Long_t size, void *p);
   static void delete__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR(void *p);
   static void deleteArray__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR(void *p);
   static void destruct__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::_Rb_tree_const_iterator<Decoder::Module::ModuleType>*)
   {
      ::_Rb_tree_const_iterator<Decoder::Module::ModuleType> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::_Rb_tree_const_iterator<Decoder::Module::ModuleType>));
      static ::ROOT::TGenericClassInfo 
         instance("_Rb_tree_const_iterator<Decoder::Module::ModuleType>", "map", 328,
                  typeid(::_Rb_tree_const_iterator<Decoder::Module::ModuleType>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &_Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR_Dictionary, isa_proxy, 4,
                  sizeof(::_Rb_tree_const_iterator<Decoder::Module::ModuleType>) );
      instance.SetNew(&new__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR);
      instance.SetNewArray(&newArray__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR);
      instance.SetDelete(&delete__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR);
      instance.SetDeleteArray(&deleteArray__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR);
      instance.SetDestructor(&destruct__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR);

      instance.AdoptAlternate(::ROOT::AddClassAlternate("_Rb_tree_const_iterator<Decoder::Module::ModuleType>","Decoder::Module::TypeIter_t"));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("_Rb_tree_const_iterator<Decoder::Module::ModuleType>","std::_Rb_tree_const_iterator<Decoder::Module::ModuleType>"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::_Rb_tree_const_iterator<Decoder::Module::ModuleType>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *_Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::_Rb_tree_const_iterator<Decoder::Module::ModuleType>*>(nullptr))->GetClass();
      _Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR_TClassManip(theClass);
   return theClass;
   }

   static void _Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLTHaEpics(void *p = nullptr);
   static void *newArray_DecodercLcLTHaEpics(Long_t size, void *p);
   static void delete_DecodercLcLTHaEpics(void *p);
   static void deleteArray_DecodercLcLTHaEpics(void *p);
   static void destruct_DecodercLcLTHaEpics(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::THaEpics*)
   {
      ::Decoder::THaEpics *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::THaEpics >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::THaEpics", ::Decoder::THaEpics::Class_Version(), "THaEpics.h", 61,
                  typeid(::Decoder::THaEpics), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::THaEpics::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::THaEpics) );
      instance.SetNew(&new_DecodercLcLTHaEpics);
      instance.SetNewArray(&newArray_DecodercLcLTHaEpics);
      instance.SetDelete(&delete_DecodercLcLTHaEpics);
      instance.SetDeleteArray(&deleteArray_DecodercLcLTHaEpics);
      instance.SetDestructor(&destruct_DecodercLcLTHaEpics);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::THaEpics*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::THaEpics*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::THaEpics*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLTHaCodaFile(void *p = nullptr);
   static void *newArray_DecodercLcLTHaCodaFile(Long_t size, void *p);
   static void delete_DecodercLcLTHaCodaFile(void *p);
   static void deleteArray_DecodercLcLTHaCodaFile(void *p);
   static void destruct_DecodercLcLTHaCodaFile(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::THaCodaFile*)
   {
      ::Decoder::THaCodaFile *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::THaCodaFile >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::THaCodaFile", ::Decoder::THaCodaFile::Class_Version(), "THaCodaFile.h", 26,
                  typeid(::Decoder::THaCodaFile), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::THaCodaFile::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::THaCodaFile) );
      instance.SetNew(&new_DecodercLcLTHaCodaFile);
      instance.SetNewArray(&newArray_DecodercLcLTHaCodaFile);
      instance.SetDelete(&delete_DecodercLcLTHaCodaFile);
      instance.SetDeleteArray(&deleteArray_DecodercLcLTHaCodaFile);
      instance.SetDestructor(&destruct_DecodercLcLTHaCodaFile);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::THaCodaFile*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::THaCodaFile*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::THaCodaFile*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_DecodercLcLModule(void *p);
   static void deleteArray_DecodercLcLModule(void *p);
   static void destruct_DecodercLcLModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Module*)
   {
      ::Decoder::Module *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Module >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Module", ::Decoder::Module::Class_Version(), "Module.h", 19,
                  typeid(::Decoder::Module), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Module::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Module) );
      instance.SetDelete(&delete_DecodercLcLModule);
      instance.SetDeleteArray(&deleteArray_DecodercLcLModule);
      instance.SetDestructor(&destruct_DecodercLcLModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Module*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Module*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Module*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *DecodercLcLModulecLcLModuleType_Dictionary();
   static void DecodercLcLModulecLcLModuleType_TClassManip(TClass*);
   static void *new_DecodercLcLModulecLcLModuleType(void *p = nullptr);
   static void *newArray_DecodercLcLModulecLcLModuleType(Long_t size, void *p);
   static void delete_DecodercLcLModulecLcLModuleType(void *p);
   static void deleteArray_DecodercLcLModulecLcLModuleType(void *p);
   static void destruct_DecodercLcLModulecLcLModuleType(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Module::ModuleType*)
   {
      ::Decoder::Module::ModuleType *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Decoder::Module::ModuleType));
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Module::ModuleType", "Module.h", 27,
                  typeid(::Decoder::Module::ModuleType), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &DecodercLcLModulecLcLModuleType_Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Module::ModuleType) );
      instance.SetNew(&new_DecodercLcLModulecLcLModuleType);
      instance.SetNewArray(&newArray_DecodercLcLModulecLcLModuleType);
      instance.SetDelete(&delete_DecodercLcLModulecLcLModuleType);
      instance.SetDeleteArray(&deleteArray_DecodercLcLModulecLcLModuleType);
      instance.SetDestructor(&destruct_DecodercLcLModulecLcLModuleType);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Module::ModuleType*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Module::ModuleType*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Module::ModuleType*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *DecodercLcLModulecLcLModuleType_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::Decoder::Module::ModuleType*>(nullptr))->GetClass();
      DecodercLcLModulecLcLModuleType_TClassManip(theClass);
   return theClass;
   }

   static void DecodercLcLModulecLcLModuleType_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLTHaSlotData(void *p = nullptr);
   static void *newArray_DecodercLcLTHaSlotData(Long_t size, void *p);
   static void delete_DecodercLcLTHaSlotData(void *p);
   static void deleteArray_DecodercLcLTHaSlotData(void *p);
   static void destruct_DecodercLcLTHaSlotData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::THaSlotData*)
   {
      ::Decoder::THaSlotData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::THaSlotData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::THaSlotData", ::Decoder::THaSlotData::Class_Version(), "THaSlotData.h", 35,
                  typeid(::Decoder::THaSlotData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::THaSlotData::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::THaSlotData) );
      instance.SetNew(&new_DecodercLcLTHaSlotData);
      instance.SetNewArray(&newArray_DecodercLcLTHaSlotData);
      instance.SetDelete(&delete_DecodercLcLTHaSlotData);
      instance.SetDeleteArray(&deleteArray_DecodercLcLTHaSlotData);
      instance.SetDestructor(&destruct_DecodercLcLTHaSlotData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::THaSlotData*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::THaSlotData*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::THaSlotData*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_THaEvData(void *p);
   static void deleteArray_THaEvData(void *p);
   static void destruct_THaEvData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaEvData*)
   {
      ::THaEvData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaEvData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("THaEvData", ::THaEvData::Class_Version(), "THaEvData.h", 22,
                  typeid(::THaEvData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaEvData::Dictionary, isa_proxy, 4,
                  sizeof(::THaEvData) );
      instance.SetDelete(&delete_THaEvData);
      instance.SetDeleteArray(&deleteArray_THaEvData);
      instance.SetDestructor(&destruct_THaEvData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaEvData*)
   {
      return GenerateInitInstanceLocal(static_cast<::THaEvData*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::THaEvData*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PoddcLcLMCTrack(void *p = nullptr);
   static void *newArray_PoddcLcLMCTrack(Long_t size, void *p);
   static void delete_PoddcLcLMCTrack(void *p);
   static void deleteArray_PoddcLcLMCTrack(void *p);
   static void destruct_PoddcLcLMCTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Podd::MCTrack*)
   {
      ::Podd::MCTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Podd::MCTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Podd::MCTrack", ::Podd::MCTrack::Class_Version(), "SimDecoder.h", 34,
                  typeid(::Podd::MCTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Podd::MCTrack::Dictionary, isa_proxy, 4,
                  sizeof(::Podd::MCTrack) );
      instance.SetNew(&new_PoddcLcLMCTrack);
      instance.SetNewArray(&newArray_PoddcLcLMCTrack);
      instance.SetDelete(&delete_PoddcLcLMCTrack);
      instance.SetDeleteArray(&deleteArray_PoddcLcLMCTrack);
      instance.SetDestructor(&destruct_PoddcLcLMCTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Podd::MCTrack*)
   {
      return GenerateInitInstanceLocal(static_cast<::Podd::MCTrack*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Podd::MCTrack*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PoddcLcLMCHitInfo(void *p = nullptr);
   static void *newArray_PoddcLcLMCHitInfo(Long_t size, void *p);
   static void delete_PoddcLcLMCHitInfo(void *p);
   static void deleteArray_PoddcLcLMCHitInfo(void *p);
   static void destruct_PoddcLcLMCHitInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Podd::MCHitInfo*)
   {
      ::Podd::MCHitInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Podd::MCHitInfo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Podd::MCHitInfo", ::Podd::MCHitInfo::Class_Version(), "SimDecoder.h", 85,
                  typeid(::Podd::MCHitInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Podd::MCHitInfo::Dictionary, isa_proxy, 4,
                  sizeof(::Podd::MCHitInfo) );
      instance.SetNew(&new_PoddcLcLMCHitInfo);
      instance.SetNewArray(&newArray_PoddcLcLMCHitInfo);
      instance.SetDelete(&delete_PoddcLcLMCHitInfo);
      instance.SetDeleteArray(&deleteArray_PoddcLcLMCHitInfo);
      instance.SetDestructor(&destruct_PoddcLcLMCHitInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Podd::MCHitInfo*)
   {
      return GenerateInitInstanceLocal(static_cast<::Podd::MCHitInfo*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Podd::MCHitInfo*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PoddcLcLMCTrackPoint(void *p = nullptr);
   static void *newArray_PoddcLcLMCTrackPoint(Long_t size, void *p);
   static void delete_PoddcLcLMCTrackPoint(void *p);
   static void deleteArray_PoddcLcLMCTrackPoint(void *p);
   static void destruct_PoddcLcLMCTrackPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Podd::MCTrackPoint*)
   {
      ::Podd::MCTrackPoint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Podd::MCTrackPoint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Podd::MCTrackPoint", ::Podd::MCTrackPoint::Class_Version(), "SimDecoder.h", 105,
                  typeid(::Podd::MCTrackPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Podd::MCTrackPoint::Dictionary, isa_proxy, 4,
                  sizeof(::Podd::MCTrackPoint) );
      instance.SetNew(&new_PoddcLcLMCTrackPoint);
      instance.SetNewArray(&newArray_PoddcLcLMCTrackPoint);
      instance.SetDelete(&delete_PoddcLcLMCTrackPoint);
      instance.SetDeleteArray(&deleteArray_PoddcLcLMCTrackPoint);
      instance.SetDestructor(&destruct_PoddcLcLMCTrackPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Podd::MCTrackPoint*)
   {
      return GenerateInitInstanceLocal(static_cast<::Podd::MCTrackPoint*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Podd::MCTrackPoint*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_PoddcLcLSimDecoder(void *p);
   static void deleteArray_PoddcLcLSimDecoder(void *p);
   static void destruct_PoddcLcLSimDecoder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Podd::SimDecoder*)
   {
      ::Podd::SimDecoder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Podd::SimDecoder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Podd::SimDecoder", ::Podd::SimDecoder::Class_Version(), "SimDecoder.h", 167,
                  typeid(::Podd::SimDecoder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Podd::SimDecoder::Dictionary, isa_proxy, 4,
                  sizeof(::Podd::SimDecoder) );
      instance.SetDelete(&delete_PoddcLcLSimDecoder);
      instance.SetDeleteArray(&deleteArray_PoddcLcLSimDecoder);
      instance.SetDestructor(&destruct_PoddcLcLSimDecoder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Podd::SimDecoder*)
   {
      return GenerateInitInstanceLocal(static_cast<::Podd::SimDecoder*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Podd::SimDecoder*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLCodaDecoder(void *p = nullptr);
   static void *newArray_DecodercLcLCodaDecoder(Long_t size, void *p);
   static void delete_DecodercLcLCodaDecoder(void *p);
   static void deleteArray_DecodercLcLCodaDecoder(void *p);
   static void destruct_DecodercLcLCodaDecoder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::CodaDecoder*)
   {
      ::Decoder::CodaDecoder *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::CodaDecoder >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::CodaDecoder", ::Decoder::CodaDecoder::Class_Version(), "CodaDecoder.h", 23,
                  typeid(::Decoder::CodaDecoder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::CodaDecoder::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::CodaDecoder) );
      instance.SetNew(&new_DecodercLcLCodaDecoder);
      instance.SetNewArray(&newArray_DecodercLcLCodaDecoder);
      instance.SetDelete(&delete_DecodercLcLCodaDecoder);
      instance.SetDeleteArray(&deleteArray_DecodercLcLCodaDecoder);
      instance.SetDestructor(&destruct_DecodercLcLCodaDecoder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::CodaDecoder*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::CodaDecoder*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::CodaDecoder*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_DecodercLcLVmeModule(void *p);
   static void deleteArray_DecodercLcLVmeModule(void *p);
   static void destruct_DecodercLcLVmeModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::VmeModule*)
   {
      ::Decoder::VmeModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::VmeModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::VmeModule", ::Decoder::VmeModule::Class_Version(), "VmeModule.h", 15,
                  typeid(::Decoder::VmeModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::VmeModule::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::VmeModule) );
      instance.SetDelete(&delete_DecodercLcLVmeModule);
      instance.SetDeleteArray(&deleteArray_DecodercLcLVmeModule);
      instance.SetDestructor(&destruct_DecodercLcLVmeModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::VmeModule*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::VmeModule*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::VmeModule*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLFastbusModule(void *p = nullptr);
   static void *newArray_DecodercLcLFastbusModule(Long_t size, void *p);
   static void delete_DecodercLcLFastbusModule(void *p);
   static void deleteArray_DecodercLcLFastbusModule(void *p);
   static void destruct_DecodercLcLFastbusModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::FastbusModule*)
   {
      ::Decoder::FastbusModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::FastbusModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::FastbusModule", ::Decoder::FastbusModule::Class_Version(), "FastbusModule.h", 16,
                  typeid(::Decoder::FastbusModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::FastbusModule::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::FastbusModule) );
      instance.SetNew(&new_DecodercLcLFastbusModule);
      instance.SetNewArray(&newArray_DecodercLcLFastbusModule);
      instance.SetDelete(&delete_DecodercLcLFastbusModule);
      instance.SetDeleteArray(&deleteArray_DecodercLcLFastbusModule);
      instance.SetDestructor(&destruct_DecodercLcLFastbusModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::FastbusModule*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::FastbusModule*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::FastbusModule*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLLecroy1877Module(void *p = nullptr);
   static void *newArray_DecodercLcLLecroy1877Module(Long_t size, void *p);
   static void delete_DecodercLcLLecroy1877Module(void *p);
   static void deleteArray_DecodercLcLLecroy1877Module(void *p);
   static void destruct_DecodercLcLLecroy1877Module(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Lecroy1877Module*)
   {
      ::Decoder::Lecroy1877Module *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Lecroy1877Module >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Lecroy1877Module", ::Decoder::Lecroy1877Module::Class_Version(), "Lecroy1877Module.h", 16,
                  typeid(::Decoder::Lecroy1877Module), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Lecroy1877Module::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Lecroy1877Module) );
      instance.SetNew(&new_DecodercLcLLecroy1877Module);
      instance.SetNewArray(&newArray_DecodercLcLLecroy1877Module);
      instance.SetDelete(&delete_DecodercLcLLecroy1877Module);
      instance.SetDeleteArray(&deleteArray_DecodercLcLLecroy1877Module);
      instance.SetDestructor(&destruct_DecodercLcLLecroy1877Module);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Lecroy1877Module*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Lecroy1877Module*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Lecroy1877Module*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLLecroy1881Module(void *p = nullptr);
   static void *newArray_DecodercLcLLecroy1881Module(Long_t size, void *p);
   static void delete_DecodercLcLLecroy1881Module(void *p);
   static void deleteArray_DecodercLcLLecroy1881Module(void *p);
   static void destruct_DecodercLcLLecroy1881Module(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Lecroy1881Module*)
   {
      ::Decoder::Lecroy1881Module *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Lecroy1881Module >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Lecroy1881Module", ::Decoder::Lecroy1881Module::Class_Version(), "Lecroy1881Module.h", 16,
                  typeid(::Decoder::Lecroy1881Module), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Lecroy1881Module::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Lecroy1881Module) );
      instance.SetNew(&new_DecodercLcLLecroy1881Module);
      instance.SetNewArray(&newArray_DecodercLcLLecroy1881Module);
      instance.SetDelete(&delete_DecodercLcLLecroy1881Module);
      instance.SetDeleteArray(&deleteArray_DecodercLcLLecroy1881Module);
      instance.SetDestructor(&destruct_DecodercLcLLecroy1881Module);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Lecroy1881Module*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Lecroy1881Module*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Lecroy1881Module*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLLecroy1875Module(void *p = nullptr);
   static void *newArray_DecodercLcLLecroy1875Module(Long_t size, void *p);
   static void delete_DecodercLcLLecroy1875Module(void *p);
   static void deleteArray_DecodercLcLLecroy1875Module(void *p);
   static void destruct_DecodercLcLLecroy1875Module(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Lecroy1875Module*)
   {
      ::Decoder::Lecroy1875Module *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Lecroy1875Module >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Lecroy1875Module", ::Decoder::Lecroy1875Module::Class_Version(), "Lecroy1875Module.h", 16,
                  typeid(::Decoder::Lecroy1875Module), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Lecroy1875Module::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Lecroy1875Module) );
      instance.SetNew(&new_DecodercLcLLecroy1875Module);
      instance.SetNewArray(&newArray_DecodercLcLLecroy1875Module);
      instance.SetDelete(&delete_DecodercLcLLecroy1875Module);
      instance.SetDeleteArray(&deleteArray_DecodercLcLLecroy1875Module);
      instance.SetDestructor(&destruct_DecodercLcLLecroy1875Module);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Lecroy1875Module*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Lecroy1875Module*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Lecroy1875Module*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_DecodercLcLPipeliningModule(void *p);
   static void deleteArray_DecodercLcLPipeliningModule(void *p);
   static void destruct_DecodercLcLPipeliningModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::PipeliningModule*)
   {
      ::Decoder::PipeliningModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::PipeliningModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::PipeliningModule", ::Decoder::PipeliningModule::Class_Version(), "PipeliningModule.h", 35,
                  typeid(::Decoder::PipeliningModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::PipeliningModule::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::PipeliningModule) );
      instance.SetDelete(&delete_DecodercLcLPipeliningModule);
      instance.SetDeleteArray(&deleteArray_DecodercLcLPipeliningModule);
      instance.SetDestructor(&destruct_DecodercLcLPipeliningModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::PipeliningModule*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::PipeliningModule*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::PipeliningModule*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLFadc250Module(void *p = nullptr);
   static void *newArray_DecodercLcLFadc250Module(Long_t size, void *p);
   static void delete_DecodercLcLFadc250Module(void *p);
   static void deleteArray_DecodercLcLFadc250Module(void *p);
   static void destruct_DecodercLcLFadc250Module(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Fadc250Module*)
   {
      ::Decoder::Fadc250Module *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Fadc250Module >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Fadc250Module", ::Decoder::Fadc250Module::Class_Version(), "Fadc250Module.h", 21,
                  typeid(::Decoder::Fadc250Module), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Fadc250Module::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Fadc250Module) );
      instance.SetNew(&new_DecodercLcLFadc250Module);
      instance.SetNewArray(&newArray_DecodercLcLFadc250Module);
      instance.SetDelete(&delete_DecodercLcLFadc250Module);
      instance.SetDeleteArray(&deleteArray_DecodercLcLFadc250Module);
      instance.SetDestructor(&destruct_DecodercLcLFadc250Module);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Fadc250Module*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Fadc250Module*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Fadc250Module*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLGenScaler(void *p = nullptr);
   static void *newArray_DecodercLcLGenScaler(Long_t size, void *p);
   static void delete_DecodercLcLGenScaler(void *p);
   static void deleteArray_DecodercLcLGenScaler(void *p);
   static void destruct_DecodercLcLGenScaler(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::GenScaler*)
   {
      ::Decoder::GenScaler *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::GenScaler >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::GenScaler", ::Decoder::GenScaler::Class_Version(), "GenScaler.h", 15,
                  typeid(::Decoder::GenScaler), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::GenScaler::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::GenScaler) );
      instance.SetNew(&new_DecodercLcLGenScaler);
      instance.SetNewArray(&newArray_DecodercLcLGenScaler);
      instance.SetDelete(&delete_DecodercLcLGenScaler);
      instance.SetDeleteArray(&deleteArray_DecodercLcLGenScaler);
      instance.SetDestructor(&destruct_DecodercLcLGenScaler);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::GenScaler*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::GenScaler*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::GenScaler*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLScaler560(void *p = nullptr);
   static void *newArray_DecodercLcLScaler560(Long_t size, void *p);
   static void delete_DecodercLcLScaler560(void *p);
   static void deleteArray_DecodercLcLScaler560(void *p);
   static void destruct_DecodercLcLScaler560(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Scaler560*)
   {
      ::Decoder::Scaler560 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Scaler560 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Scaler560", ::Decoder::Scaler560::Class_Version(), "Scaler560.h", 15,
                  typeid(::Decoder::Scaler560), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Scaler560::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Scaler560) );
      instance.SetNew(&new_DecodercLcLScaler560);
      instance.SetNewArray(&newArray_DecodercLcLScaler560);
      instance.SetDelete(&delete_DecodercLcLScaler560);
      instance.SetDeleteArray(&deleteArray_DecodercLcLScaler560);
      instance.SetDestructor(&destruct_DecodercLcLScaler560);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Scaler560*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Scaler560*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Scaler560*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLScaler1151(void *p = nullptr);
   static void *newArray_DecodercLcLScaler1151(Long_t size, void *p);
   static void delete_DecodercLcLScaler1151(void *p);
   static void deleteArray_DecodercLcLScaler1151(void *p);
   static void destruct_DecodercLcLScaler1151(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Scaler1151*)
   {
      ::Decoder::Scaler1151 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Scaler1151 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Scaler1151", ::Decoder::Scaler1151::Class_Version(), "Scaler1151.h", 15,
                  typeid(::Decoder::Scaler1151), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Scaler1151::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Scaler1151) );
      instance.SetNew(&new_DecodercLcLScaler1151);
      instance.SetNewArray(&newArray_DecodercLcLScaler1151);
      instance.SetDelete(&delete_DecodercLcLScaler1151);
      instance.SetDeleteArray(&deleteArray_DecodercLcLScaler1151);
      instance.SetDestructor(&destruct_DecodercLcLScaler1151);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Scaler1151*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Scaler1151*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Scaler1151*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLScaler3800(void *p = nullptr);
   static void *newArray_DecodercLcLScaler3800(Long_t size, void *p);
   static void delete_DecodercLcLScaler3800(void *p);
   static void deleteArray_DecodercLcLScaler3800(void *p);
   static void destruct_DecodercLcLScaler3800(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Scaler3800*)
   {
      ::Decoder::Scaler3800 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Scaler3800 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Scaler3800", ::Decoder::Scaler3800::Class_Version(), "Scaler3800.h", 15,
                  typeid(::Decoder::Scaler3800), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Scaler3800::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Scaler3800) );
      instance.SetNew(&new_DecodercLcLScaler3800);
      instance.SetNewArray(&newArray_DecodercLcLScaler3800);
      instance.SetDelete(&delete_DecodercLcLScaler3800);
      instance.SetDeleteArray(&deleteArray_DecodercLcLScaler3800);
      instance.SetDestructor(&destruct_DecodercLcLScaler3800);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Scaler3800*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Scaler3800*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Scaler3800*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLScaler3801(void *p = nullptr);
   static void *newArray_DecodercLcLScaler3801(Long_t size, void *p);
   static void delete_DecodercLcLScaler3801(void *p);
   static void deleteArray_DecodercLcLScaler3801(void *p);
   static void destruct_DecodercLcLScaler3801(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Scaler3801*)
   {
      ::Decoder::Scaler3801 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Scaler3801 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Scaler3801", ::Decoder::Scaler3801::Class_Version(), "Scaler3801.h", 15,
                  typeid(::Decoder::Scaler3801), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Scaler3801::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Scaler3801) );
      instance.SetNew(&new_DecodercLcLScaler3801);
      instance.SetNewArray(&newArray_DecodercLcLScaler3801);
      instance.SetDelete(&delete_DecodercLcLScaler3801);
      instance.SetDeleteArray(&deleteArray_DecodercLcLScaler3801);
      instance.SetDestructor(&destruct_DecodercLcLScaler3801);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Scaler3801*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Scaler3801*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Scaler3801*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLF1TDCModule(void *p = nullptr);
   static void *newArray_DecodercLcLF1TDCModule(Long_t size, void *p);
   static void delete_DecodercLcLF1TDCModule(void *p);
   static void deleteArray_DecodercLcLF1TDCModule(void *p);
   static void destruct_DecodercLcLF1TDCModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::F1TDCModule*)
   {
      ::Decoder::F1TDCModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::F1TDCModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::F1TDCModule", ::Decoder::F1TDCModule::Class_Version(), "F1TDCModule.h", 15,
                  typeid(::Decoder::F1TDCModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::F1TDCModule::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::F1TDCModule) );
      instance.SetNew(&new_DecodercLcLF1TDCModule);
      instance.SetNewArray(&newArray_DecodercLcLF1TDCModule);
      instance.SetDelete(&delete_DecodercLcLF1TDCModule);
      instance.SetDeleteArray(&deleteArray_DecodercLcLF1TDCModule);
      instance.SetDestructor(&destruct_DecodercLcLF1TDCModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::F1TDCModule*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::F1TDCModule*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::F1TDCModule*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLCaen1190Module(void *p = nullptr);
   static void *newArray_DecodercLcLCaen1190Module(Long_t size, void *p);
   static void delete_DecodercLcLCaen1190Module(void *p);
   static void deleteArray_DecodercLcLCaen1190Module(void *p);
   static void destruct_DecodercLcLCaen1190Module(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Caen1190Module*)
   {
      ::Decoder::Caen1190Module *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Caen1190Module >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Caen1190Module", ::Decoder::Caen1190Module::Class_Version(), "Caen1190Module.h", 15,
                  typeid(::Decoder::Caen1190Module), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Caen1190Module::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Caen1190Module) );
      instance.SetNew(&new_DecodercLcLCaen1190Module);
      instance.SetNewArray(&newArray_DecodercLcLCaen1190Module);
      instance.SetDelete(&delete_DecodercLcLCaen1190Module);
      instance.SetDeleteArray(&deleteArray_DecodercLcLCaen1190Module);
      instance.SetDestructor(&destruct_DecodercLcLCaen1190Module);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Caen1190Module*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Caen1190Module*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Caen1190Module*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLCaen775Module(void *p = nullptr);
   static void *newArray_DecodercLcLCaen775Module(Long_t size, void *p);
   static void delete_DecodercLcLCaen775Module(void *p);
   static void deleteArray_DecodercLcLCaen775Module(void *p);
   static void destruct_DecodercLcLCaen775Module(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Caen775Module*)
   {
      ::Decoder::Caen775Module *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Caen775Module >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Caen775Module", ::Decoder::Caen775Module::Class_Version(), "Caen775Module.h", 15,
                  typeid(::Decoder::Caen775Module), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Caen775Module::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Caen775Module) );
      instance.SetNew(&new_DecodercLcLCaen775Module);
      instance.SetNewArray(&newArray_DecodercLcLCaen775Module);
      instance.SetDelete(&delete_DecodercLcLCaen775Module);
      instance.SetDeleteArray(&deleteArray_DecodercLcLCaen775Module);
      instance.SetDestructor(&destruct_DecodercLcLCaen775Module);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Caen775Module*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Caen775Module*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Caen775Module*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DecodercLcLCaen792Module(void *p = nullptr);
   static void *newArray_DecodercLcLCaen792Module(Long_t size, void *p);
   static void delete_DecodercLcLCaen792Module(void *p);
   static void deleteArray_DecodercLcLCaen792Module(void *p);
   static void destruct_DecodercLcLCaen792Module(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Decoder::Caen792Module*)
   {
      ::Decoder::Caen792Module *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Decoder::Caen792Module >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Decoder::Caen792Module", ::Decoder::Caen792Module::Class_Version(), "Caen792Module.h", 15,
                  typeid(::Decoder::Caen792Module), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Decoder::Caen792Module::Dictionary, isa_proxy, 4,
                  sizeof(::Decoder::Caen792Module) );
      instance.SetNew(&new_DecodercLcLCaen792Module);
      instance.SetNewArray(&newArray_DecodercLcLCaen792Module);
      instance.SetDelete(&delete_DecodercLcLCaen792Module);
      instance.SetDeleteArray(&deleteArray_DecodercLcLCaen792Module);
      instance.SetDestructor(&destruct_DecodercLcLCaen792Module);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Decoder::Caen792Module*)
   {
      return GenerateInitInstanceLocal(static_cast<::Decoder::Caen792Module*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Decoder::Caen792Module*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_THaBenchmark(void *p = nullptr);
   static void *newArray_THaBenchmark(Long_t size, void *p);
   static void delete_THaBenchmark(void *p);
   static void deleteArray_THaBenchmark(void *p);
   static void destruct_THaBenchmark(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::THaBenchmark*)
   {
      ::THaBenchmark *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::THaBenchmark >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("THaBenchmark", ::THaBenchmark::Class_Version(), "THaBenchmark.h", 14,
                  typeid(::THaBenchmark), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::THaBenchmark::Dictionary, isa_proxy, 4,
                  sizeof(::THaBenchmark) );
      instance.SetNew(&new_THaBenchmark);
      instance.SetNewArray(&newArray_THaBenchmark);
      instance.SetDelete(&delete_THaBenchmark);
      instance.SetDeleteArray(&deleteArray_THaBenchmark);
      instance.SetDestructor(&destruct_THaBenchmark);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::THaBenchmark*)
   {
      return GenerateInitInstanceLocal(static_cast<::THaBenchmark*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::THaBenchmark*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr THaUsrstrutils::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *THaUsrstrutils::Class_Name()
{
   return "Decoder::THaUsrstrutils";
}

//______________________________________________________________________________
const char *THaUsrstrutils::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaUsrstrutils*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int THaUsrstrutils::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaUsrstrutils*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaUsrstrutils::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaUsrstrutils*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaUsrstrutils::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaUsrstrutils*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr THaCrateMap::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *THaCrateMap::Class_Name()
{
   return "Decoder::THaCrateMap";
}

//______________________________________________________________________________
const char *THaCrateMap::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCrateMap*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int THaCrateMap::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCrateMap*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaCrateMap::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCrateMap*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaCrateMap::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCrateMap*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr THaCodaData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *THaCodaData::Class_Name()
{
   return "Decoder::THaCodaData";
}

//______________________________________________________________________________
const char *THaCodaData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCodaData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int THaCodaData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCodaData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaCodaData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCodaData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaCodaData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCodaData*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr THaEpics::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *THaEpics::Class_Name()
{
   return "Decoder::THaEpics";
}

//______________________________________________________________________________
const char *THaEpics::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaEpics*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int THaEpics::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaEpics*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaEpics::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaEpics*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaEpics::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaEpics*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr THaCodaFile::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *THaCodaFile::Class_Name()
{
   return "Decoder::THaCodaFile";
}

//______________________________________________________________________________
const char *THaCodaFile::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCodaFile*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int THaCodaFile::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCodaFile*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaCodaFile::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCodaFile*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaCodaFile::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaCodaFile*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Module::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Module::Class_Name()
{
   return "Decoder::Module";
}

//______________________________________________________________________________
const char *Module::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Module*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Module::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Module*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Module::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Module*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Module::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Module*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr THaSlotData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *THaSlotData::Class_Name()
{
   return "Decoder::THaSlotData";
}

//______________________________________________________________________________
const char *THaSlotData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaSlotData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int THaSlotData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaSlotData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaSlotData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaSlotData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaSlotData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::THaSlotData*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
//______________________________________________________________________________
atomic_TClass_ptr THaEvData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *THaEvData::Class_Name()
{
   return "THaEvData";
}

//______________________________________________________________________________
const char *THaEvData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaEvData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int THaEvData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaEvData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaEvData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaEvData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaEvData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaEvData*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace Podd {
//______________________________________________________________________________
atomic_TClass_ptr MCTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MCTrack::Class_Name()
{
   return "Podd::MCTrack";
}

//______________________________________________________________________________
const char *MCTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MCTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MCTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MCTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Podd
namespace Podd {
//______________________________________________________________________________
atomic_TClass_ptr MCHitInfo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MCHitInfo::Class_Name()
{
   return "Podd::MCHitInfo";
}

//______________________________________________________________________________
const char *MCHitInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCHitInfo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MCHitInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCHitInfo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MCHitInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCHitInfo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MCHitInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCHitInfo*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Podd
namespace Podd {
//______________________________________________________________________________
atomic_TClass_ptr MCTrackPoint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MCTrackPoint::Class_Name()
{
   return "Podd::MCTrackPoint";
}

//______________________________________________________________________________
const char *MCTrackPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCTrackPoint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MCTrackPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCTrackPoint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MCTrackPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCTrackPoint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MCTrackPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Podd::MCTrackPoint*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Podd
namespace Podd {
//______________________________________________________________________________
atomic_TClass_ptr SimDecoder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SimDecoder::Class_Name()
{
   return "Podd::SimDecoder";
}

//______________________________________________________________________________
const char *SimDecoder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Podd::SimDecoder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SimDecoder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Podd::SimDecoder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SimDecoder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Podd::SimDecoder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SimDecoder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Podd::SimDecoder*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Podd
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr CodaDecoder::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CodaDecoder::Class_Name()
{
   return "Decoder::CodaDecoder";
}

//______________________________________________________________________________
const char *CodaDecoder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::CodaDecoder*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CodaDecoder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::CodaDecoder*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CodaDecoder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::CodaDecoder*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CodaDecoder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::CodaDecoder*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr VmeModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *VmeModule::Class_Name()
{
   return "Decoder::VmeModule";
}

//______________________________________________________________________________
const char *VmeModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::VmeModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int VmeModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::VmeModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *VmeModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::VmeModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *VmeModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::VmeModule*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr FastbusModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *FastbusModule::Class_Name()
{
   return "Decoder::FastbusModule";
}

//______________________________________________________________________________
const char *FastbusModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::FastbusModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int FastbusModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::FastbusModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *FastbusModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::FastbusModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *FastbusModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::FastbusModule*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Lecroy1877Module::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Lecroy1877Module::Class_Name()
{
   return "Decoder::Lecroy1877Module";
}

//______________________________________________________________________________
const char *Lecroy1877Module::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1877Module*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Lecroy1877Module::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1877Module*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Lecroy1877Module::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1877Module*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Lecroy1877Module::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1877Module*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Lecroy1881Module::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Lecroy1881Module::Class_Name()
{
   return "Decoder::Lecroy1881Module";
}

//______________________________________________________________________________
const char *Lecroy1881Module::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1881Module*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Lecroy1881Module::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1881Module*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Lecroy1881Module::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1881Module*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Lecroy1881Module::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1881Module*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Lecroy1875Module::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Lecroy1875Module::Class_Name()
{
   return "Decoder::Lecroy1875Module";
}

//______________________________________________________________________________
const char *Lecroy1875Module::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1875Module*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Lecroy1875Module::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1875Module*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Lecroy1875Module::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1875Module*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Lecroy1875Module::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Lecroy1875Module*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr PipeliningModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PipeliningModule::Class_Name()
{
   return "Decoder::PipeliningModule";
}

//______________________________________________________________________________
const char *PipeliningModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::PipeliningModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PipeliningModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::PipeliningModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PipeliningModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::PipeliningModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PipeliningModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::PipeliningModule*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Fadc250Module::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Fadc250Module::Class_Name()
{
   return "Decoder::Fadc250Module";
}

//______________________________________________________________________________
const char *Fadc250Module::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Fadc250Module*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Fadc250Module::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Fadc250Module*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Fadc250Module::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Fadc250Module*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Fadc250Module::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Fadc250Module*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr GenScaler::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *GenScaler::Class_Name()
{
   return "Decoder::GenScaler";
}

//______________________________________________________________________________
const char *GenScaler::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::GenScaler*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int GenScaler::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::GenScaler*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *GenScaler::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::GenScaler*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *GenScaler::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::GenScaler*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Scaler560::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Scaler560::Class_Name()
{
   return "Decoder::Scaler560";
}

//______________________________________________________________________________
const char *Scaler560::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler560*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Scaler560::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler560*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Scaler560::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler560*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Scaler560::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler560*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Scaler1151::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Scaler1151::Class_Name()
{
   return "Decoder::Scaler1151";
}

//______________________________________________________________________________
const char *Scaler1151::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler1151*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Scaler1151::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler1151*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Scaler1151::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler1151*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Scaler1151::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler1151*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Scaler3800::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Scaler3800::Class_Name()
{
   return "Decoder::Scaler3800";
}

//______________________________________________________________________________
const char *Scaler3800::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler3800*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Scaler3800::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler3800*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Scaler3800::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler3800*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Scaler3800::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler3800*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Scaler3801::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Scaler3801::Class_Name()
{
   return "Decoder::Scaler3801";
}

//______________________________________________________________________________
const char *Scaler3801::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler3801*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Scaler3801::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler3801*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Scaler3801::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler3801*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Scaler3801::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Scaler3801*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr F1TDCModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *F1TDCModule::Class_Name()
{
   return "Decoder::F1TDCModule";
}

//______________________________________________________________________________
const char *F1TDCModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::F1TDCModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int F1TDCModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::F1TDCModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *F1TDCModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::F1TDCModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *F1TDCModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::F1TDCModule*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Caen1190Module::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Caen1190Module::Class_Name()
{
   return "Decoder::Caen1190Module";
}

//______________________________________________________________________________
const char *Caen1190Module::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen1190Module*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Caen1190Module::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen1190Module*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Caen1190Module::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen1190Module*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Caen1190Module::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen1190Module*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Caen775Module::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Caen775Module::Class_Name()
{
   return "Decoder::Caen775Module";
}

//______________________________________________________________________________
const char *Caen775Module::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen775Module*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Caen775Module::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen775Module*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Caen775Module::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen775Module*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Caen775Module::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen775Module*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
namespace Decoder {
//______________________________________________________________________________
atomic_TClass_ptr Caen792Module::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Caen792Module::Class_Name()
{
   return "Decoder::Caen792Module";
}

//______________________________________________________________________________
const char *Caen792Module::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen792Module*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Caen792Module::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen792Module*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Caen792Module::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen792Module*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Caen792Module::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Decoder::Caen792Module*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace Decoder
//______________________________________________________________________________
atomic_TClass_ptr THaBenchmark::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *THaBenchmark::Class_Name()
{
   return "THaBenchmark";
}

//______________________________________________________________________________
const char *THaBenchmark::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaBenchmark*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int THaBenchmark::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::THaBenchmark*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *THaBenchmark::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaBenchmark*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *THaBenchmark::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::THaBenchmark*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace Decoder {
//______________________________________________________________________________
void THaUsrstrutils::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::THaUsrstrutils.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::THaUsrstrutils::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::THaUsrstrutils::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLTHaUsrstrutils(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Decoder::THaUsrstrutils : new ::Decoder::THaUsrstrutils;
   }
   static void *newArray_DecodercLcLTHaUsrstrutils(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Decoder::THaUsrstrutils[nElements] : new ::Decoder::THaUsrstrutils[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLTHaUsrstrutils(void *p) {
      delete (static_cast<::Decoder::THaUsrstrutils*>(p));
   }
   static void deleteArray_DecodercLcLTHaUsrstrutils(void *p) {
      delete [] (static_cast<::Decoder::THaUsrstrutils*>(p));
   }
   static void destruct_DecodercLcLTHaUsrstrutils(void *p) {
      typedef ::Decoder::THaUsrstrutils current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::THaUsrstrutils

namespace Decoder {
//______________________________________________________________________________
void THaCrateMap::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::THaCrateMap.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::THaCrateMap::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::THaCrateMap::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLTHaCrateMap(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Decoder::THaCrateMap : new ::Decoder::THaCrateMap;
   }
   static void *newArray_DecodercLcLTHaCrateMap(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Decoder::THaCrateMap[nElements] : new ::Decoder::THaCrateMap[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLTHaCrateMap(void *p) {
      delete (static_cast<::Decoder::THaCrateMap*>(p));
   }
   static void deleteArray_DecodercLcLTHaCrateMap(void *p) {
      delete [] (static_cast<::Decoder::THaCrateMap*>(p));
   }
   static void destruct_DecodercLcLTHaCrateMap(void *p) {
      typedef ::Decoder::THaCrateMap current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::THaCrateMap

namespace Decoder {
//______________________________________________________________________________
void THaCodaData::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::THaCodaData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::THaCodaData::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::THaCodaData::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrapper around operator delete
   static void delete_DecodercLcLTHaCodaData(void *p) {
      delete (static_cast<::Decoder::THaCodaData*>(p));
   }
   static void deleteArray_DecodercLcLTHaCodaData(void *p) {
      delete [] (static_cast<::Decoder::THaCodaData*>(p));
   }
   static void destruct_DecodercLcLTHaCodaData(void *p) {
      typedef ::Decoder::THaCodaData current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::THaCodaData

namespace ROOT {
   // Wrappers around operator new
   static void *new__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::_Rb_tree_const_iterator<Decoder::Module::ModuleType> : new ::_Rb_tree_const_iterator<Decoder::Module::ModuleType>;
   }
   static void *newArray__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::_Rb_tree_const_iterator<Decoder::Module::ModuleType>[nElements] : new ::_Rb_tree_const_iterator<Decoder::Module::ModuleType>[nElements];
   }
   // Wrapper around operator delete
   static void delete__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR(void *p) {
      delete (static_cast<::_Rb_tree_const_iterator<Decoder::Module::ModuleType>*>(p));
   }
   static void deleteArray__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR(void *p) {
      delete [] (static_cast<::_Rb_tree_const_iterator<Decoder::Module::ModuleType>*>(p));
   }
   static void destruct__Rb_tree_const_iteratorlEDecodercLcLModulecLcLModuleTypegR(void *p) {
      typedef ::_Rb_tree_const_iterator<Decoder::Module::ModuleType> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::_Rb_tree_const_iterator<Decoder::Module::ModuleType>

namespace Decoder {
//______________________________________________________________________________
void THaEpics::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::THaEpics.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::THaEpics::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::THaEpics::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLTHaEpics(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Decoder::THaEpics : new ::Decoder::THaEpics;
   }
   static void *newArray_DecodercLcLTHaEpics(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Decoder::THaEpics[nElements] : new ::Decoder::THaEpics[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLTHaEpics(void *p) {
      delete (static_cast<::Decoder::THaEpics*>(p));
   }
   static void deleteArray_DecodercLcLTHaEpics(void *p) {
      delete [] (static_cast<::Decoder::THaEpics*>(p));
   }
   static void destruct_DecodercLcLTHaEpics(void *p) {
      typedef ::Decoder::THaEpics current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::THaEpics

namespace Decoder {
//______________________________________________________________________________
void THaCodaFile::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::THaCodaFile.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::THaCodaFile::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::THaCodaFile::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLTHaCodaFile(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Decoder::THaCodaFile : new ::Decoder::THaCodaFile;
   }
   static void *newArray_DecodercLcLTHaCodaFile(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Decoder::THaCodaFile[nElements] : new ::Decoder::THaCodaFile[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLTHaCodaFile(void *p) {
      delete (static_cast<::Decoder::THaCodaFile*>(p));
   }
   static void deleteArray_DecodercLcLTHaCodaFile(void *p) {
      delete [] (static_cast<::Decoder::THaCodaFile*>(p));
   }
   static void destruct_DecodercLcLTHaCodaFile(void *p) {
      typedef ::Decoder::THaCodaFile current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::THaCodaFile

namespace Decoder {
//______________________________________________________________________________
void Module::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Module.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Module::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Module::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrapper around operator delete
   static void delete_DecodercLcLModule(void *p) {
      delete (static_cast<::Decoder::Module*>(p));
   }
   static void deleteArray_DecodercLcLModule(void *p) {
      delete [] (static_cast<::Decoder::Module*>(p));
   }
   static void destruct_DecodercLcLModule(void *p) {
      typedef ::Decoder::Module current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Module

namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLModulecLcLModuleType(void *p) {
      return  p ? new(p) ::Decoder::Module::ModuleType : new ::Decoder::Module::ModuleType;
   }
   static void *newArray_DecodercLcLModulecLcLModuleType(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Module::ModuleType[nElements] : new ::Decoder::Module::ModuleType[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLModulecLcLModuleType(void *p) {
      delete (static_cast<::Decoder::Module::ModuleType*>(p));
   }
   static void deleteArray_DecodercLcLModulecLcLModuleType(void *p) {
      delete [] (static_cast<::Decoder::Module::ModuleType*>(p));
   }
   static void destruct_DecodercLcLModulecLcLModuleType(void *p) {
      typedef ::Decoder::Module::ModuleType current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Module::ModuleType

namespace Decoder {
//______________________________________________________________________________
void THaSlotData::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::THaSlotData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::THaSlotData::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::THaSlotData::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLTHaSlotData(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Decoder::THaSlotData : new ::Decoder::THaSlotData;
   }
   static void *newArray_DecodercLcLTHaSlotData(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Decoder::THaSlotData[nElements] : new ::Decoder::THaSlotData[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLTHaSlotData(void *p) {
      delete (static_cast<::Decoder::THaSlotData*>(p));
   }
   static void deleteArray_DecodercLcLTHaSlotData(void *p) {
      delete [] (static_cast<::Decoder::THaSlotData*>(p));
   }
   static void destruct_DecodercLcLTHaSlotData(void *p) {
      typedef ::Decoder::THaSlotData current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::THaSlotData

//______________________________________________________________________________
void THaEvData::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaEvData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaEvData::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaEvData::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_THaEvData(void *p) {
      delete (static_cast<::THaEvData*>(p));
   }
   static void deleteArray_THaEvData(void *p) {
      delete [] (static_cast<::THaEvData*>(p));
   }
   static void destruct_THaEvData(void *p) {
      typedef ::THaEvData current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::THaEvData

namespace Podd {
//______________________________________________________________________________
void MCTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class Podd::MCTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Podd::MCTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(Podd::MCTrack::Class(),this);
   }
}

} // namespace Podd
namespace ROOT {
   // Wrappers around operator new
   static void *new_PoddcLcLMCTrack(void *p) {
      return  p ? new(p) ::Podd::MCTrack : new ::Podd::MCTrack;
   }
   static void *newArray_PoddcLcLMCTrack(Long_t nElements, void *p) {
      return p ? new(p) ::Podd::MCTrack[nElements] : new ::Podd::MCTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_PoddcLcLMCTrack(void *p) {
      delete (static_cast<::Podd::MCTrack*>(p));
   }
   static void deleteArray_PoddcLcLMCTrack(void *p) {
      delete [] (static_cast<::Podd::MCTrack*>(p));
   }
   static void destruct_PoddcLcLMCTrack(void *p) {
      typedef ::Podd::MCTrack current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Podd::MCTrack

namespace Podd {
//______________________________________________________________________________
void MCHitInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class Podd::MCHitInfo.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Podd::MCHitInfo::Class(),this);
   } else {
      R__b.WriteClassBuffer(Podd::MCHitInfo::Class(),this);
   }
}

} // namespace Podd
namespace ROOT {
   // Wrappers around operator new
   static void *new_PoddcLcLMCHitInfo(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Podd::MCHitInfo : new ::Podd::MCHitInfo;
   }
   static void *newArray_PoddcLcLMCHitInfo(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) ::Podd::MCHitInfo[nElements] : new ::Podd::MCHitInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_PoddcLcLMCHitInfo(void *p) {
      delete (static_cast<::Podd::MCHitInfo*>(p));
   }
   static void deleteArray_PoddcLcLMCHitInfo(void *p) {
      delete [] (static_cast<::Podd::MCHitInfo*>(p));
   }
   static void destruct_PoddcLcLMCHitInfo(void *p) {
      typedef ::Podd::MCHitInfo current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Podd::MCHitInfo

namespace Podd {
//______________________________________________________________________________
void MCTrackPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class Podd::MCTrackPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Podd::MCTrackPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(Podd::MCTrackPoint::Class(),this);
   }
}

} // namespace Podd
namespace ROOT {
   // Wrappers around operator new
   static void *new_PoddcLcLMCTrackPoint(void *p) {
      return  p ? new(p) ::Podd::MCTrackPoint : new ::Podd::MCTrackPoint;
   }
   static void *newArray_PoddcLcLMCTrackPoint(Long_t nElements, void *p) {
      return p ? new(p) ::Podd::MCTrackPoint[nElements] : new ::Podd::MCTrackPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_PoddcLcLMCTrackPoint(void *p) {
      delete (static_cast<::Podd::MCTrackPoint*>(p));
   }
   static void deleteArray_PoddcLcLMCTrackPoint(void *p) {
      delete [] (static_cast<::Podd::MCTrackPoint*>(p));
   }
   static void destruct_PoddcLcLMCTrackPoint(void *p) {
      typedef ::Podd::MCTrackPoint current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Podd::MCTrackPoint

namespace Podd {
//______________________________________________________________________________
void SimDecoder::Streamer(TBuffer &R__b)
{
   // Stream an object of class Podd::SimDecoder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Podd::SimDecoder::Class(),this);
   } else {
      R__b.WriteClassBuffer(Podd::SimDecoder::Class(),this);
   }
}

} // namespace Podd
namespace ROOT {
   // Wrapper around operator delete
   static void delete_PoddcLcLSimDecoder(void *p) {
      delete (static_cast<::Podd::SimDecoder*>(p));
   }
   static void deleteArray_PoddcLcLSimDecoder(void *p) {
      delete [] (static_cast<::Podd::SimDecoder*>(p));
   }
   static void destruct_PoddcLcLSimDecoder(void *p) {
      typedef ::Podd::SimDecoder current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Podd::SimDecoder

namespace Decoder {
//______________________________________________________________________________
void CodaDecoder::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::CodaDecoder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::CodaDecoder::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::CodaDecoder::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLCodaDecoder(void *p) {
      return  p ? new(p) ::Decoder::CodaDecoder : new ::Decoder::CodaDecoder;
   }
   static void *newArray_DecodercLcLCodaDecoder(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::CodaDecoder[nElements] : new ::Decoder::CodaDecoder[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLCodaDecoder(void *p) {
      delete (static_cast<::Decoder::CodaDecoder*>(p));
   }
   static void deleteArray_DecodercLcLCodaDecoder(void *p) {
      delete [] (static_cast<::Decoder::CodaDecoder*>(p));
   }
   static void destruct_DecodercLcLCodaDecoder(void *p) {
      typedef ::Decoder::CodaDecoder current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::CodaDecoder

namespace Decoder {
//______________________________________________________________________________
void VmeModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::VmeModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::VmeModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::VmeModule::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrapper around operator delete
   static void delete_DecodercLcLVmeModule(void *p) {
      delete (static_cast<::Decoder::VmeModule*>(p));
   }
   static void deleteArray_DecodercLcLVmeModule(void *p) {
      delete [] (static_cast<::Decoder::VmeModule*>(p));
   }
   static void destruct_DecodercLcLVmeModule(void *p) {
      typedef ::Decoder::VmeModule current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::VmeModule

namespace Decoder {
//______________________________________________________________________________
void FastbusModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::FastbusModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::FastbusModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::FastbusModule::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLFastbusModule(void *p) {
      return  p ? new(p) ::Decoder::FastbusModule : new ::Decoder::FastbusModule;
   }
   static void *newArray_DecodercLcLFastbusModule(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::FastbusModule[nElements] : new ::Decoder::FastbusModule[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLFastbusModule(void *p) {
      delete (static_cast<::Decoder::FastbusModule*>(p));
   }
   static void deleteArray_DecodercLcLFastbusModule(void *p) {
      delete [] (static_cast<::Decoder::FastbusModule*>(p));
   }
   static void destruct_DecodercLcLFastbusModule(void *p) {
      typedef ::Decoder::FastbusModule current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::FastbusModule

namespace Decoder {
//______________________________________________________________________________
void Lecroy1877Module::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Lecroy1877Module.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Lecroy1877Module::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Lecroy1877Module::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLLecroy1877Module(void *p) {
      return  p ? new(p) ::Decoder::Lecroy1877Module : new ::Decoder::Lecroy1877Module;
   }
   static void *newArray_DecodercLcLLecroy1877Module(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Lecroy1877Module[nElements] : new ::Decoder::Lecroy1877Module[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLLecroy1877Module(void *p) {
      delete (static_cast<::Decoder::Lecroy1877Module*>(p));
   }
   static void deleteArray_DecodercLcLLecroy1877Module(void *p) {
      delete [] (static_cast<::Decoder::Lecroy1877Module*>(p));
   }
   static void destruct_DecodercLcLLecroy1877Module(void *p) {
      typedef ::Decoder::Lecroy1877Module current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Lecroy1877Module

namespace Decoder {
//______________________________________________________________________________
void Lecroy1881Module::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Lecroy1881Module.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Lecroy1881Module::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Lecroy1881Module::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLLecroy1881Module(void *p) {
      return  p ? new(p) ::Decoder::Lecroy1881Module : new ::Decoder::Lecroy1881Module;
   }
   static void *newArray_DecodercLcLLecroy1881Module(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Lecroy1881Module[nElements] : new ::Decoder::Lecroy1881Module[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLLecroy1881Module(void *p) {
      delete (static_cast<::Decoder::Lecroy1881Module*>(p));
   }
   static void deleteArray_DecodercLcLLecroy1881Module(void *p) {
      delete [] (static_cast<::Decoder::Lecroy1881Module*>(p));
   }
   static void destruct_DecodercLcLLecroy1881Module(void *p) {
      typedef ::Decoder::Lecroy1881Module current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Lecroy1881Module

namespace Decoder {
//______________________________________________________________________________
void Lecroy1875Module::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Lecroy1875Module.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Lecroy1875Module::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Lecroy1875Module::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLLecroy1875Module(void *p) {
      return  p ? new(p) ::Decoder::Lecroy1875Module : new ::Decoder::Lecroy1875Module;
   }
   static void *newArray_DecodercLcLLecroy1875Module(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Lecroy1875Module[nElements] : new ::Decoder::Lecroy1875Module[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLLecroy1875Module(void *p) {
      delete (static_cast<::Decoder::Lecroy1875Module*>(p));
   }
   static void deleteArray_DecodercLcLLecroy1875Module(void *p) {
      delete [] (static_cast<::Decoder::Lecroy1875Module*>(p));
   }
   static void destruct_DecodercLcLLecroy1875Module(void *p) {
      typedef ::Decoder::Lecroy1875Module current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Lecroy1875Module

namespace Decoder {
//______________________________________________________________________________
void PipeliningModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::PipeliningModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::PipeliningModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::PipeliningModule::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrapper around operator delete
   static void delete_DecodercLcLPipeliningModule(void *p) {
      delete (static_cast<::Decoder::PipeliningModule*>(p));
   }
   static void deleteArray_DecodercLcLPipeliningModule(void *p) {
      delete [] (static_cast<::Decoder::PipeliningModule*>(p));
   }
   static void destruct_DecodercLcLPipeliningModule(void *p) {
      typedef ::Decoder::PipeliningModule current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::PipeliningModule

namespace Decoder {
//______________________________________________________________________________
void Fadc250Module::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Fadc250Module.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Fadc250Module::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Fadc250Module::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLFadc250Module(void *p) {
      return  p ? new(p) ::Decoder::Fadc250Module : new ::Decoder::Fadc250Module;
   }
   static void *newArray_DecodercLcLFadc250Module(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Fadc250Module[nElements] : new ::Decoder::Fadc250Module[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLFadc250Module(void *p) {
      delete (static_cast<::Decoder::Fadc250Module*>(p));
   }
   static void deleteArray_DecodercLcLFadc250Module(void *p) {
      delete [] (static_cast<::Decoder::Fadc250Module*>(p));
   }
   static void destruct_DecodercLcLFadc250Module(void *p) {
      typedef ::Decoder::Fadc250Module current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Fadc250Module

namespace Decoder {
//______________________________________________________________________________
void GenScaler::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::GenScaler.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::GenScaler::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::GenScaler::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLGenScaler(void *p) {
      return  p ? new(p) ::Decoder::GenScaler : new ::Decoder::GenScaler;
   }
   static void *newArray_DecodercLcLGenScaler(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::GenScaler[nElements] : new ::Decoder::GenScaler[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLGenScaler(void *p) {
      delete (static_cast<::Decoder::GenScaler*>(p));
   }
   static void deleteArray_DecodercLcLGenScaler(void *p) {
      delete [] (static_cast<::Decoder::GenScaler*>(p));
   }
   static void destruct_DecodercLcLGenScaler(void *p) {
      typedef ::Decoder::GenScaler current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::GenScaler

namespace Decoder {
//______________________________________________________________________________
void Scaler560::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Scaler560.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Scaler560::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Scaler560::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLScaler560(void *p) {
      return  p ? new(p) ::Decoder::Scaler560 : new ::Decoder::Scaler560;
   }
   static void *newArray_DecodercLcLScaler560(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Scaler560[nElements] : new ::Decoder::Scaler560[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLScaler560(void *p) {
      delete (static_cast<::Decoder::Scaler560*>(p));
   }
   static void deleteArray_DecodercLcLScaler560(void *p) {
      delete [] (static_cast<::Decoder::Scaler560*>(p));
   }
   static void destruct_DecodercLcLScaler560(void *p) {
      typedef ::Decoder::Scaler560 current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Scaler560

namespace Decoder {
//______________________________________________________________________________
void Scaler1151::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Scaler1151.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Scaler1151::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Scaler1151::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLScaler1151(void *p) {
      return  p ? new(p) ::Decoder::Scaler1151 : new ::Decoder::Scaler1151;
   }
   static void *newArray_DecodercLcLScaler1151(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Scaler1151[nElements] : new ::Decoder::Scaler1151[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLScaler1151(void *p) {
      delete (static_cast<::Decoder::Scaler1151*>(p));
   }
   static void deleteArray_DecodercLcLScaler1151(void *p) {
      delete [] (static_cast<::Decoder::Scaler1151*>(p));
   }
   static void destruct_DecodercLcLScaler1151(void *p) {
      typedef ::Decoder::Scaler1151 current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Scaler1151

namespace Decoder {
//______________________________________________________________________________
void Scaler3800::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Scaler3800.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Scaler3800::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Scaler3800::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLScaler3800(void *p) {
      return  p ? new(p) ::Decoder::Scaler3800 : new ::Decoder::Scaler3800;
   }
   static void *newArray_DecodercLcLScaler3800(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Scaler3800[nElements] : new ::Decoder::Scaler3800[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLScaler3800(void *p) {
      delete (static_cast<::Decoder::Scaler3800*>(p));
   }
   static void deleteArray_DecodercLcLScaler3800(void *p) {
      delete [] (static_cast<::Decoder::Scaler3800*>(p));
   }
   static void destruct_DecodercLcLScaler3800(void *p) {
      typedef ::Decoder::Scaler3800 current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Scaler3800

namespace Decoder {
//______________________________________________________________________________
void Scaler3801::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Scaler3801.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Scaler3801::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Scaler3801::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLScaler3801(void *p) {
      return  p ? new(p) ::Decoder::Scaler3801 : new ::Decoder::Scaler3801;
   }
   static void *newArray_DecodercLcLScaler3801(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Scaler3801[nElements] : new ::Decoder::Scaler3801[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLScaler3801(void *p) {
      delete (static_cast<::Decoder::Scaler3801*>(p));
   }
   static void deleteArray_DecodercLcLScaler3801(void *p) {
      delete [] (static_cast<::Decoder::Scaler3801*>(p));
   }
   static void destruct_DecodercLcLScaler3801(void *p) {
      typedef ::Decoder::Scaler3801 current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Scaler3801

namespace Decoder {
//______________________________________________________________________________
void F1TDCModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::F1TDCModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::F1TDCModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::F1TDCModule::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLF1TDCModule(void *p) {
      return  p ? new(p) ::Decoder::F1TDCModule : new ::Decoder::F1TDCModule;
   }
   static void *newArray_DecodercLcLF1TDCModule(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::F1TDCModule[nElements] : new ::Decoder::F1TDCModule[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLF1TDCModule(void *p) {
      delete (static_cast<::Decoder::F1TDCModule*>(p));
   }
   static void deleteArray_DecodercLcLF1TDCModule(void *p) {
      delete [] (static_cast<::Decoder::F1TDCModule*>(p));
   }
   static void destruct_DecodercLcLF1TDCModule(void *p) {
      typedef ::Decoder::F1TDCModule current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::F1TDCModule

namespace Decoder {
//______________________________________________________________________________
void Caen1190Module::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Caen1190Module.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Caen1190Module::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Caen1190Module::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLCaen1190Module(void *p) {
      return  p ? new(p) ::Decoder::Caen1190Module : new ::Decoder::Caen1190Module;
   }
   static void *newArray_DecodercLcLCaen1190Module(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Caen1190Module[nElements] : new ::Decoder::Caen1190Module[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLCaen1190Module(void *p) {
      delete (static_cast<::Decoder::Caen1190Module*>(p));
   }
   static void deleteArray_DecodercLcLCaen1190Module(void *p) {
      delete [] (static_cast<::Decoder::Caen1190Module*>(p));
   }
   static void destruct_DecodercLcLCaen1190Module(void *p) {
      typedef ::Decoder::Caen1190Module current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Caen1190Module

namespace Decoder {
//______________________________________________________________________________
void Caen775Module::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Caen775Module.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Caen775Module::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Caen775Module::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLCaen775Module(void *p) {
      return  p ? new(p) ::Decoder::Caen775Module : new ::Decoder::Caen775Module;
   }
   static void *newArray_DecodercLcLCaen775Module(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Caen775Module[nElements] : new ::Decoder::Caen775Module[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLCaen775Module(void *p) {
      delete (static_cast<::Decoder::Caen775Module*>(p));
   }
   static void deleteArray_DecodercLcLCaen775Module(void *p) {
      delete [] (static_cast<::Decoder::Caen775Module*>(p));
   }
   static void destruct_DecodercLcLCaen775Module(void *p) {
      typedef ::Decoder::Caen775Module current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Caen775Module

namespace Decoder {
//______________________________________________________________________________
void Caen792Module::Streamer(TBuffer &R__b)
{
   // Stream an object of class Decoder::Caen792Module.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Decoder::Caen792Module::Class(),this);
   } else {
      R__b.WriteClassBuffer(Decoder::Caen792Module::Class(),this);
   }
}

} // namespace Decoder
namespace ROOT {
   // Wrappers around operator new
   static void *new_DecodercLcLCaen792Module(void *p) {
      return  p ? new(p) ::Decoder::Caen792Module : new ::Decoder::Caen792Module;
   }
   static void *newArray_DecodercLcLCaen792Module(Long_t nElements, void *p) {
      return p ? new(p) ::Decoder::Caen792Module[nElements] : new ::Decoder::Caen792Module[nElements];
   }
   // Wrapper around operator delete
   static void delete_DecodercLcLCaen792Module(void *p) {
      delete (static_cast<::Decoder::Caen792Module*>(p));
   }
   static void deleteArray_DecodercLcLCaen792Module(void *p) {
      delete [] (static_cast<::Decoder::Caen792Module*>(p));
   }
   static void destruct_DecodercLcLCaen792Module(void *p) {
      typedef ::Decoder::Caen792Module current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Decoder::Caen792Module

//______________________________________________________________________________
void THaBenchmark::Streamer(TBuffer &R__b)
{
   // Stream an object of class THaBenchmark.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(THaBenchmark::Class(),this);
   } else {
      R__b.WriteClassBuffer(THaBenchmark::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_THaBenchmark(void *p) {
      return  p ? new(p) ::THaBenchmark : new ::THaBenchmark;
   }
   static void *newArray_THaBenchmark(Long_t nElements, void *p) {
      return p ? new(p) ::THaBenchmark[nElements] : new ::THaBenchmark[nElements];
   }
   // Wrapper around operator delete
   static void delete_THaBenchmark(void *p) {
      delete (static_cast<::THaBenchmark*>(p));
   }
   static void deleteArray_THaBenchmark(void *p) {
      delete [] (static_cast<::THaBenchmark*>(p));
   }
   static void destruct_THaBenchmark(void *p) {
      typedef ::THaBenchmark current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::THaBenchmark

namespace ROOT {
   static TClass *setlEDecodercLcLModulecLcLModuleTypegR_Dictionary();
   static void setlEDecodercLcLModulecLcLModuleTypegR_TClassManip(TClass*);
   static void *new_setlEDecodercLcLModulecLcLModuleTypegR(void *p = nullptr);
   static void *newArray_setlEDecodercLcLModulecLcLModuleTypegR(Long_t size, void *p);
   static void delete_setlEDecodercLcLModulecLcLModuleTypegR(void *p);
   static void deleteArray_setlEDecodercLcLModulecLcLModuleTypegR(void *p);
   static void destruct_setlEDecodercLcLModulecLcLModuleTypegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const set<Decoder::Module::ModuleType>*)
   {
      set<Decoder::Module::ModuleType> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(set<Decoder::Module::ModuleType>));
      static ::ROOT::TGenericClassInfo 
         instance("set<Decoder::Module::ModuleType>", -2, "set", 94,
                  typeid(set<Decoder::Module::ModuleType>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &setlEDecodercLcLModulecLcLModuleTypegR_Dictionary, isa_proxy, 4,
                  sizeof(set<Decoder::Module::ModuleType>) );
      instance.SetNew(&new_setlEDecodercLcLModulecLcLModuleTypegR);
      instance.SetNewArray(&newArray_setlEDecodercLcLModulecLcLModuleTypegR);
      instance.SetDelete(&delete_setlEDecodercLcLModulecLcLModuleTypegR);
      instance.SetDeleteArray(&deleteArray_setlEDecodercLcLModulecLcLModuleTypegR);
      instance.SetDestructor(&destruct_setlEDecodercLcLModulecLcLModuleTypegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Insert< set<Decoder::Module::ModuleType> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("set<Decoder::Module::ModuleType>","std::set<Decoder::Module::ModuleType, std::less<Decoder::Module::ModuleType>, std::allocator<Decoder::Module::ModuleType> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const set<Decoder::Module::ModuleType>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *setlEDecodercLcLModulecLcLModuleTypegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const set<Decoder::Module::ModuleType>*>(nullptr))->GetClass();
      setlEDecodercLcLModulecLcLModuleTypegR_TClassManip(theClass);
   return theClass;
   }

   static void setlEDecodercLcLModulecLcLModuleTypegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_setlEDecodercLcLModulecLcLModuleTypegR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) set<Decoder::Module::ModuleType> : new set<Decoder::Module::ModuleType>;
   }
   static void *newArray_setlEDecodercLcLModulecLcLModuleTypegR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) set<Decoder::Module::ModuleType>[nElements] : new set<Decoder::Module::ModuleType>[nElements];
   }
   // Wrapper around operator delete
   static void delete_setlEDecodercLcLModulecLcLModuleTypegR(void *p) {
      delete (static_cast<set<Decoder::Module::ModuleType>*>(p));
   }
   static void deleteArray_setlEDecodercLcLModulecLcLModuleTypegR(void *p) {
      delete [] (static_cast<set<Decoder::Module::ModuleType>*>(p));
   }
   static void destruct_setlEDecodercLcLModulecLcLModuleTypegR(void *p) {
      typedef set<Decoder::Module::ModuleType> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class set<Decoder::Module::ModuleType>

namespace {
  void TriggerDictionaryInitialization_THaDecDict_Impl() {
    static const char* headers[] = {
"hana_decode/THaUsrstrutils.h",
"hana_decode/THaCrateMap.h",
"hana_decode/THaCodaData.h",
"hana_decode/THaEpics.h",
"hana_decode/THaCodaFile.h",
"hana_decode/SimDecoder.h",
"hana_decode/THaSlotData.h",
"hana_decode/THaEvData.h",
"hana_decode/CodaDecoder.h",
"hana_decode/Module.h",
"hana_decode/VmeModule.h",
"hana_decode/FastbusModule.h",
"hana_decode/Lecroy1877Module.h",
"hana_decode/Lecroy1881Module.h",
"hana_decode/Lecroy1875Module.h",
"hana_decode/Fadc250Module.h",
"hana_decode/GenScaler.h",
"hana_decode/Scaler560.h",
"hana_decode/Scaler1151.h",
"hana_decode/Scaler3800.h",
"hana_decode/Scaler3801.h",
"hana_decode/F1TDCModule.h",
"hana_decode/Caen1190Module.h",
"hana_decode/Caen775Module.h",
"hana_decode/Caen792Module.h",
"hana_decode/THaBenchmark.h",
nullptr
    };
    static const char* includePaths[] = {
"/w/halla-scshelf2102/sbs/ralphmm/adaq/analyzer/src",
"/w/halla-scshelf2102/sbs/ralphmm/adaq/analyzer/hana_decode",
"/u/group/halla/apps/ROOT/6.30-04/el9/RelWithDebInfo/include",
"/w/halla-scshelf2102/sbs/ralphmm/adaq/analyzer/evio/evio-4.4.6/src/libsrc",
"/u/group/halla/apps/ROOT/6.30-04/el9/RelWithDebInfo/include/",
"/w/halla-scshelf2102/sbs/ralphmm/adaq/analyzer/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "THaDecDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/THaUsrstrutils.h")))  THaUsrstrutils;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/THaCrateMap.h")))  THaCrateMap;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/THaCodaData.h")))  THaCodaData;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/THaEpics.h")))  THaEpics;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/THaCodaFile.h")))  THaCodaFile;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$Module.h")))  __attribute__((annotate("$clingAutoload$hana_decode/SimDecoder.h")))  Module;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$THaSlotData.h")))  __attribute__((annotate("$clingAutoload$hana_decode/SimDecoder.h")))  THaSlotData;}
class __attribute__((annotate("$clingAutoload$THaEvData.h")))  __attribute__((annotate("$clingAutoload$hana_decode/SimDecoder.h")))  THaEvData;
namespace Podd{class __attribute__((annotate("$clingAutoload$hana_decode/SimDecoder.h")))  MCTrack;}
namespace Podd{class __attribute__((annotate("$clingAutoload$hana_decode/SimDecoder.h")))  MCHitInfo;}
namespace Podd{class __attribute__((annotate("$clingAutoload$hana_decode/SimDecoder.h")))  MCTrackPoint;}
namespace Podd{class __attribute__((annotate("$clingAutoload$hana_decode/SimDecoder.h")))  SimDecoder;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/CodaDecoder.h")))  CodaDecoder;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/VmeModule.h")))  VmeModule;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/FastbusModule.h")))  FastbusModule;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/Lecroy1877Module.h")))  Lecroy1877Module;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/Lecroy1881Module.h")))  Lecroy1881Module;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/Lecroy1875Module.h")))  Lecroy1875Module;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$PipeliningModule.h")))  __attribute__((annotate("$clingAutoload$hana_decode/Fadc250Module.h")))  PipeliningModule;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/Fadc250Module.h")))  Fadc250Module;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/GenScaler.h")))  GenScaler;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/Scaler560.h")))  Scaler560;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/Scaler1151.h")))  Scaler1151;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/Scaler3800.h")))  Scaler3800;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/Scaler3801.h")))  Scaler3801;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/F1TDCModule.h")))  F1TDCModule;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/Caen1190Module.h")))  Caen1190Module;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/Caen775Module.h")))  Caen775Module;}
namespace Decoder{class __attribute__((annotate("$clingAutoload$hana_decode/Caen792Module.h")))  Caen792Module;}
class __attribute__((annotate("$clingAutoload$hana_decode/THaBenchmark.h")))  THaBenchmark;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "THaDecDict dictionary payload"

#ifndef NDEBUG
  #define NDEBUG 1
#endif
#ifndef WITH_DEBUG
  #define WITH_DEBUG 1
#endif
#ifndef LINUXVERS
  #define LINUXVERS 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "hana_decode/THaUsrstrutils.h"
#include "hana_decode/THaCrateMap.h"
#include "hana_decode/THaCodaData.h"
#include "hana_decode/THaEpics.h"
#include "hana_decode/THaCodaFile.h"
#include "hana_decode/SimDecoder.h"
#include "hana_decode/THaSlotData.h"
#include "hana_decode/THaEvData.h"
#include "hana_decode/CodaDecoder.h"
#include "hana_decode/Module.h"
#include "hana_decode/VmeModule.h"
#include "hana_decode/FastbusModule.h"
#include "hana_decode/Lecroy1877Module.h"
#include "hana_decode/Lecroy1881Module.h"
#include "hana_decode/Lecroy1875Module.h"
#include "hana_decode/Fadc250Module.h"
#include "hana_decode/GenScaler.h"
#include "hana_decode/Scaler560.h"
#include "hana_decode/Scaler1151.h"
#include "hana_decode/Scaler3800.h"
#include "hana_decode/Scaler3801.h"
#include "hana_decode/F1TDCModule.h"
#include "hana_decode/Caen1190Module.h"
#include "hana_decode/Caen775Module.h"
#include "hana_decode/Caen792Module.h"
#include "hana_decode/THaBenchmark.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Decoder::Caen1190Module", payloadCode, "@",
"Decoder::Caen775Module", payloadCode, "@",
"Decoder::Caen792Module", payloadCode, "@",
"Decoder::CodaDecoder", payloadCode, "@",
"Decoder::F1TDCModule", payloadCode, "@",
"Decoder::Fadc250Module", payloadCode, "@",
"Decoder::FastbusModule", payloadCode, "@",
"Decoder::GenScaler", payloadCode, "@",
"Decoder::Lecroy1875Module", payloadCode, "@",
"Decoder::Lecroy1877Module", payloadCode, "@",
"Decoder::Lecroy1881Module", payloadCode, "@",
"Decoder::Module", payloadCode, "@",
"Decoder::Module::ModuleType", payloadCode, "@",
"Decoder::PipeliningModule", payloadCode, "@",
"Decoder::Scaler1151", payloadCode, "@",
"Decoder::Scaler3800", payloadCode, "@",
"Decoder::Scaler3801", payloadCode, "@",
"Decoder::Scaler560", payloadCode, "@",
"Decoder::THaCodaData", payloadCode, "@",
"Decoder::THaCodaFile", payloadCode, "@",
"Decoder::THaCrateMap", payloadCode, "@",
"Decoder::THaEpics", payloadCode, "@",
"Decoder::THaSlotData", payloadCode, "@",
"Decoder::THaUsrstrutils", payloadCode, "@",
"Decoder::VmeModule", payloadCode, "@",
"Podd::MCHitInfo", payloadCode, "@",
"Podd::MCTrack", payloadCode, "@",
"Podd::MCTrackPoint", payloadCode, "@",
"Podd::SimDecoder", payloadCode, "@",
"THaBenchmark", payloadCode, "@",
"THaEvData", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("THaDecDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_THaDecDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_THaDecDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_THaDecDict() {
  TriggerDictionaryInitialization_THaDecDict_Impl();
}
