// MAPPING

static const Int_t NLAYERS=2;
static const Int_t NMODULES=6;
static const Int_t NLR=2;
static const Int_t NPMT=14;
static const Int_t NPIXEL=16;
static const Int_t NBARS=NLAYERS*NMODULES*NLR*NPMT*NPIXEL;

Int_t baread[NBARS]; // ADC of bar as read from database
Int_t ba[NBARS], bt[NBARS]; // ADC of bar corrected for gain, TDC of bar
Double_t gainfactor[NBARS];
Int_t aslot[NLAYERS][NMODULES][NLR][NPMT], achan[NLAYERS][NMODULES][NLR][NPMT], tslot[NLAYERS][NMODULES][NLR][NPMT], tchan[NLAYERS][NMODULES][NLR][NPMT], pixel1_miss[NLAYERS][NMODULES][NLR][NPMT], pixel2_miss[NLAYERS][NMODULES][NLR][NPMT];

// read-from-database mapping (work in progress):
TString dbname = "database.txt";
void readdb(TString fname = dbname){
  ifstream in;
  in.open(fname);
  TString header;
  Int_t i;
  Int_t layer, module, lr, pmt;
  Int_t as,ac,ts,tc,p1,p2;

  // Initialize to -1 for check of element existence
  for (layer=0; layer<NLAYERS;layer++){
   for (module=0; module<NMODULES;module++){
    for (lr=0; lr<NLR;lr++){
     for (pmt=0; pmt<NPMT;pmt++){
	aslot[layer][module][lr][pmt]=-1;
	tslot[layer][module][lr][pmt]=-1;
	achan[layer][module][lr][pmt]=-1;
	tchan[layer][module][lr][pmt]=-1;
	pixel1_miss[layer][module][lr][pmt]=-1;
	pixel2_miss[layer][module][lr][pmt]=-1;
     }
    }
   }
  }

  // Read database from file
  //cout << "Map Read:" << endl;
  in >> header;
  //cout << header << endl;
  for (i=0; i<NPMT; i++){
    in >> layer >> module >> lr >> pmt 
       >> as >> ac
       >> ts >> tc
       >> p1 >> p2;
    //cout << layer <<" "<< module <<" "<< lr <<" "<< pmt <<" "<< as <<" "<< ac <<" "<< ts <<" "<< tc <<" "<< p1 <<" "<< p2 << endl;
    aslot[layer-1][module-1][lr-1][pmt-1] = as;
    achan[layer-1][module-1][lr-1][pmt-1] = ac;
    tslot[layer-1][module-1][lr-1][pmt-1] = ts;
    tchan[layer-1][module-1][lr-1][pmt-1] = tc;
    pixel1_miss[layer-1][module-1][lr-1][pmt-1] = p1;
    pixel2_miss[layer-1][module-1][lr-1][pmt-1] = p2;
    //cout << "Layer: " << layer << " Module: "<< module <<" LR: " << lr << " ADC slot/chan,TDC slot/chan,P1,P2: " << aslot[layer-1][module-1][lr-1][pmt-1] <<":"<< achan[layer-1][module-1][lr-1][pmt-1] <<":"<< tslot[layer-1][module-1][lr-1][pmt-1]  <<":"<< tchan[layer-1][module-1][lr-1][pmt-1]  <<":"<< pixel1_miss[layer-1][module-1][lr-1][pmt-1]  <<":"<< pixel2_miss[layer-1][module-1][lr-1][pmt-1] << endl;
  }
  in.close();
}

// to do: create a tree friend of t, then try and fill it with ba[],
// baread[], bt[], calculated from the arrays read from database via
// TString.Form(); this should trick root, whose TFormula class, used
// in t->Draw(), cannot call a function which return a string, it
// accept just integers, (thanks, root dev team, mapping would be too
// simple otherwise)


// global pixel number 0..NBAR
Int_t globalpixel(Int_t pmt, Int_t pixel){
  return (pmt-1)*NPIXEL + (pixel-1);
}

// adc slot of NINO ID
int aslotNINO(int NINOID){
  int value=-1;
  switch (NINOID){
  case 1:
  case 2:
  case 3: 
  case 4:
    value=0; break;
  case 5:
  case 8:
    value=1; break;
  case 9:
  case 10:
  case 11:
  case 12:
    value=2; break;
  case 13:
  case 14:
    value=3; break;
  default:
    cout <<" Unknown NINO ID"<<endl;
  }
  return value;
} 
// first ADC chan of NINO ID (the 15 channels which follow are
// supposed to be in the same order in both NINO card and ADC module)
int achanNINO(int NINOID){
  int value=-1;
  switch(NINOID){
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
    value=0; break;
  default:
    cout<<" Unknown NINO ID"<<endl;
  }
  return value; 
}

// used by Fastbus_main1.C:
//-------------------------
// pmt (>=1, <=14) -> ADC slot
Int_t handmapping_adc_slot(Int_t pmt){
  switch (pmt){
  case 1:
  case 2:
  case 3:
  case 4:
    return 0;
    break;
  case 5:
  case 8:
    return 1;
    break;
  case 9:
  case 10:
  case 11:
  case 12:
    return 2;
    break;
  case 6:
  case 7:
  case 13:
  case 14:
    return 3;
    break;
  default:
    return -1;
  }
}

// pmt (>=1,<=14), pixel (>=1, <=16) -> ADC channel
Int_t handmapping_adc_chan(Int_t pmt, Int_t pixel){
  pixel--;
  switch(pmt){
  case 1:
  case 5:
  case 9:
  case 13:
    return pixel;
    break;
  case 2:
  case 10:
  case 14:
    return 16+pixel;
    break;
  case 3:
  case 6:
  case 11:
    return 32+pixel;
    break;
  case 4:
  case 7:
  case 8:
  case 12:
    return 48+pixel;
    break;
  default:
    return -1;
  }
}

// pmt (>=1, <=14) -> TDC slot
Int_t handmapping_tdc_slot(Int_t pmt){
  switch (pmt){
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
    return 0;
    break;
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
    return 1;
    break;
  case 13:
  case 14:
    return 2;
    break;
  default:
    return -1;
  }
}

// pmt (>=1,<=14), pixel (>=1, <=16) -> TDC channel
Int_t handmapping_tdc_chan(Int_t pmt, Int_t pixel){
  pixel--;
  switch(pmt){
  case 1:
  case 7:
  case 13:
    return pixel;
    break;
  case 2:
  case 8:
  case 14:
    return 16+pixel;
    break;
  case 3:
  case 9:
    return 32+pixel;
    break;
  case 4:
  case 10:
    return 48+pixel;
    break;
  case 5:
  case 11:
    return 64+pixel;
    break;
  case 6:
  case 12:
    return 80+pixel;
  default:
    return -1;
  }
}
// pmt (>=1,<=14) first disconnected PMT channel
Int_t handmapping_pmt_pixel1(Int_t pmt){
	switch(pmt){
	case 1:
	case 2:
	case 5:
	case 7:
	case 9:
		return 4;
		break;
	case 6:
		return 3;
		break;
	case 3:
		return 2;
		break;
	case 4:
		return 1;
		break;
	case 8:
	case 10:
	case 11:
	case 12:
	case 14:
		return 13;
		break;
	case 13:
		return 5;
		break;
	default:
		return -1;
	}
}
//-------------------------
// pmt (>=1,<=14) first disconnected PMT channel
Int_t handmapping_pmt_pixel2(Int_t pmt){
	switch(pmt){
	case 1:
		return 5;
		break;
	case 2:
		return 8;
		break;
	case 3:
		return 11;
		break;
	case 4:
	case 6:
		return 13;
		break;
	case 5:
		return 14;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
		return 16;
		break;
	default:
		return -1;
	}
}
