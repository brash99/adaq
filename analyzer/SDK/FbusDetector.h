#ifndef ROOT_FbusDetector
#define ROOT_FbusDetector

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FbusDetector                                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "THaNonTrackingDetector.h"
#include <vector>

class FbusDetector : public THaNonTrackingDetector {

public:
  FbusDetector( const char* name, const char* description = "",
		THaApparatus* a = NULL );
  FbusDetector() : fRawADC(0), fCorADC(0) {}
  virtual ~FbusDetector();

  virtual void       Clear( Option_t* opt="" );
  virtual Int_t      Decode( const THaEvData& );
  virtual Int_t      CoarseProcess( TClonesArray& tracks );
  virtual Int_t      FineProcess( TClonesArray& tracks );
  virtual void       Print( Option_t* opt="" ) const;

  Int_t GetNhits() const { return fNhit; }

protected:
  // Typical experimental data are far less certain than even single precision,
  // so floats are usually just fine for storing them, especially if there
  // are arrays of values.
  typedef std::vector<Float_t> Vflt_t;

  // Calibration data from database
  Vflt_t     fPed;       // ADC pedestals
  Vflt_t     fGain;      // ADC gains
  
  // Per-event data
  Int_t      fNhit;      // Number of hits
  Float_t*   fRawADC;    // [fNelem] Raw ADC data
  Float_t*   fCorADC;    // [fNelem] Corrected ADC data
 
  virtual Int_t  ReadDatabase( const TDatime& date );
  virtual Int_t  DefineVariables( EMode mode );

  ClassDef(FbusDetector,0)   // Fastbus detector 1
};

////////////////////////////////////////////////////////////////////////////////

#endif
