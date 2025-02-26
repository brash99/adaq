// A simple decoder and histogrammer for fastbus data.
// R. Michaels, Oct 24, 2014.
// developed later by Dasuni, et al.

#define MAXROC     32
#define NUMCHANT   96   // TDC
#define NUMTDCSLOTS 3   // TDC
#define NUMCHANA   64   // ADC
#define NUMADCSLOTS 4   // ADC
#define MAXHITS    100
#define MAXEVENTS  1000000
// constants used to easily distinguish which case the bit 16 of TDC data word is referring to.
#define TRAILING_EDGE 0
#define LEADING_EDGE  1


#include <iostream>
#include <string>
#include <vector>
#include "THaCodaFile.h"
#include "THaEtClient.h"
#include "TString.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TNtuple.h"
#include "TRandom.h"
#include <vector>
#include "Riostream.h"

using namespace std;

void usage();
void decode(int* data);
void analysis();
void clear();
void pedsup();
void readpedsup();

// Global data 
Int_t evlen, evtype, evnum;
Int_t *irn, *rocpos, *roclen;
Int_t myroc, myslot[2], mychan[2];
Int_t ievent = 0;
//Int_t *nthits;
Int_t numslothitsADC[NUMADCSLOTS];
Int_t adcdat[NUMCHANA][NUMADCSLOTS];
Int_t adcchan[NUMCHANA][NUMADCSLOTS];
Int_t nbad_adc[NUMADCSLOTS]={0,0,0,0};

TH1F  *hadc1;
TH1F  *hadc2[2];
TH2F  *hadc3[NUMADCSLOTS];
TH2F  *h2adc;
TH1F  *hadcped[NUMADCSLOTS][NUMCHANA];
TH1F  *hbranch;

Int_t fHasHeader=1;   // =1 for 1877 and 1881, but =0 for 1875
Int_t fWordSeen;
Int_t slotindADC[NUMADCSLOTS];
Int_t debug=1;
Int_t numLocal = 0;
Int_t branchnum = 999;
Int_t PEDSUP; // 0 = read pedestal file, 1 = calculate and write file. Read from argv
Int_t pedestal_threshold[NUMCHANA][NUMADCSLOTS];

//TDC
Int_t numslothitsTDC[NUMTDCSLOTS];
Int_t tdcdat[2][NUMCHANT][NUMTDCSLOTS]; // the first index discriminate between leading and trainling edge
Int_t tdcchan[NUMCHANT][NUMTDCSLOTS];
Int_t nbad_tdc[NUMTDCSLOTS]={0,0,0};
Int_t slotindTDC[NUMTDCSLOTS]={16,15,14};
//Int_t ngoodpulsestotal[NUMTDCSLOTS];

TH1F  *htdc1;
TH1F  *hleadingtime;
TH1F  *htdcpulsewidth;

int main(int argc, char* argv[])
{

  THaCodaData *coda;      
  char ctitle[100];
  char dtitle[100];
  char rtitle[100];
  char htitle[100];
  char cpedtitle[100];
  char hpedtitle[100];
  Int_t maxevent = 10000;
  Int_t runno = 1000;
  Int_t istatus;
  Int_t lslot;

  int choice1 = 1;
  irn = new Int_t[MAXROC];
  rocpos = new Int_t[MAXROC];
  roclen = new Int_t[MAXROC];

  //default args
  PEDSUP = 0;
  maxevent = MAXEVENTS;
 
   if (argc >= 3) {
    runno = atoi(argv[1]); // required arg: run#
    myroc = atoi(argv[2]); // required arg: roc#
  }

  if (argc > 3) // optional arg: maxevent
    maxevent = atoi(argv[3]);

  if (argc > 4)  // optional arg: pedsup
    PEDSUP = atoi(argv[4]);

  if (argc > 5 || argc < 3) { // wrong # of args
    usage();
    return 1;
  }  

  cout << "args elaborated" << endl;

  if (myroc == 14){
    lslot = 20;
  }
  else if (myroc == 5){
    lslot = 17;
  }

  else if (myroc == 6){
    lslot = 11;
  }
  else if (myroc == 7){
    lslot = 17;
  }

  else if (myroc == 15){
    lslot = 5;
  }

  else if (myroc == 16){
    lslot = 14;
  }

  else if (myroc == 17){
    lslot = 8;
  }
  
  else{
    lslot = 11;
  }
  
  myslot[0]= lslot;
  myslot[1]= lslot+1;

  cout << "my roc is " << myroc << endl;

  for (int i = 0; i < NUMADCSLOTS; i++){
    slotindADC[i] = lslot;
    lslot++;
  }

  mychan[0] = 0;  mychan[1] = 0;  // defaults
  

  cout << "Fastbus analysis "<<endl;
  cout << "Events to process "<<maxevent<<endl;

  //nthits  = new Int_t[NUMCHANA];
  //numslothitsADC  = new Int_t[NUMADCSLOTS];

  // Initialize root and output
  sprintf(rtitle,"sbs_%d_%i.root",runno,myroc);
  sprintf(dtitle,"../data/scint_%d.dat",runno);
  TROOT fbana("fbroot","Hall A SCINT analysis for Fastbus");
  TFile hfile(rtitle,"RECREATE","SBS data");

  sprintf(ctitle,"ADC hits per slot ");
  hadc1 = new TH1F("hadc1",ctitle,31,-1,30.);
  sprintf(ctitle,"ADC data on slot %d channel %d",myslot[0],mychan[0]);
  hadc2[0] = new TH1F("hadc2_0",ctitle,200,0,4000.);
  sprintf(ctitle,"ADC data on slot %d channel %d",myslot[1],mychan[1]);
  hadc2[1] = new TH1F("hadc2_1",ctitle,200,0,4000.);

  sprintf(ctitle,"ADC data on 2 channnels");
  h2adc = new TH2F("h2adc",ctitle,200,0,4000.,200,0,4000.);
  sprintf(ctitle,"Branch #");
  hbranch = new TH1F("hbranch",ctitle,10,0,10.);

  for (int i = 0; i < NUMADCSLOTS;i++) {
    sprintf(ctitle,"ADC data on all channels of slot %d",slotindADC[i]);
    sprintf(htitle,"ADC_chan_slot_%d",slotindADC[i]);
    hadc3[i] = new TH2F(htitle,ctitle,200,0,2000.,64,0,64.);
  }
  
  for (int i = 0; i < NUMADCSLOTS;i++) {
    for (int j = 0; j < NUMCHANA;j++) {
      sprintf(cpedtitle,"myADC data on chan %d of slot %d", j, slotindADC[i]);
      sprintf(hpedtitle,"myADC_chan_%d_slot_%d",j,slotindADC[i]);
      hadcped[i][j] = new TH1F(hpedtitle,cpedtitle,2000,0,2000.);
    }

  }

  // TDC
  sprintf(ctitle,"TDC hits per slot ");
  htdc1 = new TH1F("htdc1",ctitle,31,-1,30.);
  htdc1->GetXaxis()->SetTitle("slot");
  htdc1->GetYaxis()->SetTitle("counts");
  sprintf(ctitle,"TDC leading edge times ");
  hleadingtime = new TH1F("hleadingtime",ctitle,1000,0,1000.);
  hleadingtime->GetXaxis()->SetTitle("channel (0.5 ns each)");
  hleadingtime->GetYaxis()->SetTitle("counts");
  sprintf(ctitle,"TDC pulse width ");
  htdcpulsewidth = new TH1F("htdcpulsewidth",ctitle,1000,0,1000.);
  htdcpulsewidth->GetXaxis()->SetTitle("channel (0.5 ns each)");
  htdcpulsewidth->GetYaxis()->SetTitle("counts");

  if (choice1 == 1) {  // CODA File
    
    // CODA file "run.dat" may be a link to CODA datafile on disk
    TString filename(dtitle);

    coda = new THaCodaFile();
    if (coda->codaOpen(filename) != 0) {
      cout << "ERROR: Cannot open CODA data" << endl;
      goto end1;
    }

    
  } else {         // Online ET connection
    
    int mymode = 1;
    TString mycomputer("sbs1");
    TString mysession("sbsfb1");
    
    coda = new THaEtClient();
    if (coda->codaOpen(mycomputer, mysession, mymode) != 0) {
      cout << "ERROR:  Cannot open ET connection" << endl;
      goto end1;
    }
    
  }

  if (PEDSUP == 0) {
    cout << "Reading pedestal file because PEDSUP = 0 " <<endl;
    readpedsup();
  };

  // Loop over events

  for (int iev = 0; iev < maxevent; iev++)  {//the loop over the events
    
    if (iev > 0 && ((iev%1000)==0) ) printf("%d events\n",iev);

    clear();

    istatus = coda->codaRead();  

    if (istatus != 0) {  // EOF or no data

      if ( istatus == -1) {
	if (choice1 == 1) {
	  cout << "End of CODA file. Bye bye." << endl;
	}
	if (choice1 == 2) cout << "CODA/ET not running. Bye bye." << endl;
      } else {
	cout << "ERROR: codaRread istatus = " << hex << istatus << endl;
      }
      goto end1;
  

    } else {   // we have data ...

      ievent++;
      decode( coda->getEvBuffer() );
      if (evtype < 10) 
	analysis();

    }
  }

 end1:

  cout << endl;
  if (PEDSUP==1) {
    cout << "Calculating pedestal because PEDSUP = 1" << endl;
    pedsup();
  }

  // end1:
  
  printf("\n");
  printf("Number of events analyzed = %d \n",ievent);
  for (int i=0;i<NUMADCSLOTS;i++){
    printf("In Slot %d, Number of events with bad adc = %d \n",slotindADC[i],nbad_adc[i]);
  }

  // TDC
  for (int i=0;i<NUMTDCSLOTS;i++){
    printf("In Slot %d, Number of events with bad tdc = %d \n",slotindTDC[i],nbad_tdc[i]);
  }
  cout << endl;
  // for (int i=0;i<NUMTDCSLOTS;i++){
  //   cout << "Total number of good pulses in TDC in slot " << i << " = " << ngoodpulsestotal[i] << endl;
  // }
 
  coda->codaClose();

  hfile.Write();
  hfile.Close();

  return 0;

}; //end of main function


void usage() {  
  cout << "Usage:  'fbana [runno] [roc#] [maxevents] [PEDSUP] ' " << endl;
  cout << "Need [runno] [roc#] arguments "<<endl;
  cout << "[maxev] optional argument "<<endl;
  cout << "[PEDSUPl] optional argument. 0 means use pedestal saved in file ped_test.dat."<<endl;
  cout << "                             1 means calculate it and save it." <<endl;
  cout << "                             Default = 0" <<endl;
  cout << "Can have at most four arguments "<<endl;
}; 

void clear() {
  fWordSeen = 0;
  //  memset (nthits, 0, NUMCHANA*sizeof(Int_t));
  memset (adcdat, 0, NUMADCSLOTS*NUMCHANA*sizeof(Int_t));
  memset (adcchan, 0, NUMADCSLOTS*NUMCHANA*sizeof(Int_t));
  memset (numslothitsADC, 0, NUMADCSLOTS*sizeof(Int_t));
  
  //TDC
  memset (numslothitsTDC, 0, NUMTDCSLOTS*sizeof(Int_t));
  memset (tdcdat, 0, 2*NUMTDCSLOTS*NUMCHANT*sizeof(Int_t));
  // memset (ngoodpulsestotal, 0, NUMTDCSLOTS*sizeof(Int_t));
}

void decode (int* data) {

  Int_t ichan, rdata;
  evlen = data[0] + 1;
  evtype = data[1]>>16;
  evnum = data[4];
  static int dodump = 0;  // dump the raw data

  Int_t edgetype;

  if (dodump) { // Event dump
    cout << "\n\n Event number " << dec << evnum;
    cout << " length " << evlen << " type " << evtype << endl;
    int ipt = 0;
    for (int j = 0; j < (evlen/5); j++) {
      cout << dec << "\n evbuffer[" << ipt << "] = ";
      for (int k=j; k<j+5; k++) {
	cout << hex << data[ipt++] << " ";
      }
      cout << endl;
    }
    if (ipt < evlen) {
      cout << dec << "\n evbuffer[" << ipt << "] = ";
      for (int k=ipt; k<evlen; k++) {
	cout << hex << data[ipt++] << " ";
      }
      cout << endl;
    }
  }
  if (evtype > 10) return;
  
  //printf("#localTrig = %d \n",numLocal);

  // loop to find header of ADC block read
  int index=0;
  int indexlast=0;
  if (debug) cout << " START LOOKING AT  at index = " << index << " last = "<< indexlast << " " << roclen[myroc]<< endl;
  while (((data[index]&0xffffffff)!=0x0da000011)) {
    index++;
  }
  while (((data[indexlast]&0xffffffff)!=0x0da000022)) {
    indexlast++;
  }

  //loop to find header of TDC block read. It starts at indexlast+1 and end at indexend
  int indexend=indexlast;
  while (((data[indexend]&0xffffffff)!=0x0da000033)) {
    indexend++;
  }

  //  indexlast=indexlast-1;
  int slotnew=0;
  if (debug) cout << " ADC header at index = " << index << " last = "<< indexlast << endl;
  
  if (debug) cout << " TDC header at index = " << indexlast+1 << " last = "<< indexend << endl;

  // ADC loop
  for (int j = index+1; j < indexlast; j++) {

    if (debug) printf("data[%d] = 0x%x = (dec)  \n",j,data[j]);

    int slot = (data[j]&0xf8000000)>>27; 
    int slot_ndata = 0;
    Int_t slotindex=0;
    if (slot!=slotnew) {
      slotindex=0;
      slotnew=slot;
      slot_ndata= (data[j]&0x7f) - 1;
      cout << slot_ndata << " " << slot << endl;
      for (int jj=0;jj<NUMADCSLOTS;jj++) {
	if (slot==slotindADC[jj]) slotindex=jj;
      }
      if (slot_ndata > NUMCHANA) {
        printf("evnum= %d,slot_ndata = %d, data[%d] = 0x%x = (dec)  \n",evnum,slot_ndata,j,data[j]);
        nbad_adc[slotindex]++;
	return;
      }
    }
    if (debug) cout << "slot "<<slot<< " slot index = " << slotindex << " slot indata = " << slot_ndata << endl;
    for (int ii=0;ii<slot_ndata;ii++){
      j++;
      if (debug==2) printf("data[%d] = 0x%x = (dec) %d %d  \n",j,data[j],(data[j]&0xfe0000)>>17,data[j]&0x3fff);
      ichan = (data[j]&0xfe0000)>>17; // 1881
      rdata = data[j]&0x3fff;  // 1881
      //	   ichan = (data[j]&0xfe0000)>>17; // 1877
      //rdata = data[j]&0xffff;  // 1877
      if (ichan >= 0 && ichan < NUMCHANA && rdata >= 0) {
	  adcdat[ii][slotindex] = rdata; 
	  adcchan[ii][slotindex] = ichan; 
	}
	}
      numslothitsADC[slotindex]=slot_ndata;
    } // end of ADC loop
    
    // TDC loop
    slotnew=0;

    for (int j = indexlast+1; j < indexend; j++) {

      if (debug) printf("TDC data[%d] = 0x%x = (dec)  \n",j,data[j]);

      int slot = (data[j]&0xf8000000)>>27; 
      int slot_ndata = 0;
      Int_t slotindex=0;
      if (slot!=slotnew) {
    	slotindex=0;
    	slotnew=slot;
    	slot_ndata= (data[j]&0xff) - 1;
	
    	cout << slot_ndata << " " << slot << endl;
    	for (int jj=0;jj<NUMTDCSLOTS;jj++) {
    	  if (slot==slotindTDC[jj]) slotindex=jj;
    	}
    	if (slot_ndata > NUMCHANT) {
    	  printf("TDC evnum= %d,slot_ndata = %d, data[%d] = 0x%x = (dec)  \n",evnum,slot_ndata,j,data[j]);
    	  nbad_tdc[slotindex]++;
    	  return;
    	}
      }
      if (debug) cout << "TDC slot "<<slot<< " slot index = " << slotindex << " slot indata = " << slot_ndata << endl;
   
      for (int ii=0;ii<slot_ndata;ii++){
    	j++;
    	ichan = (data[j]&0xfe0000)>>17; // 1881
    	rdata = data[j]&0xffff;  // 1881
    	//	   ichan = (data[j]&0xfe0000)>>17; // 1877
    	//rdata = data[j]&0xffff;  // 1877
    	edgetype = (data[j]&0x10000)>>16; // the type of the edge is in bit 16
    	if (debug==2) printf("TDC data[%d] = 0x%x = (dec) %d %d %d \n",j,data[j],(data[j]&0xfe0000)>>17,data[j]&0x3fff,edgetype);



	  if (ichan >= 0 && ichan < NUMCHANT && rdata >= 0) {
    	    tdcdat[edgetype][ii][slotindex] = rdata; 
    	    tdcchan[ii][slotindex] = ichan; 
    	  }
      } // for 
    	numslothitsTDC[slotindex]=slot_ndata;
      } // end of TDC loop

    } // end of decode()



    void analysis() {

      Int_t islot, ichan,ihit, rdata;
      Int_t rawtimes[2];
      if (debug) cout << " analysis " << endl;

      // ADC
      //if (numLocal >= 1){
      for (islot = 0; islot < NUMADCSLOTS; islot++) {
	hadc1->Fill(slotindADC[islot] ,numslothitsADC[islot]);
     
	if (debug) cout << " slot " << slotindADC[islot] << " " << numslothitsADC[islot] << endl;
	for (ihit = 0; ihit < numslothitsADC[islot] ; ihit++) {
	  rdata = adcdat[ihit][islot];
	  ichan = adcchan[ihit][islot];
	  hbranch->Fill(branchnum);

	  if (ichan == mychan[0] &&  slotindADC[islot]== myslot[0]) hadc2[0]->Fill(rdata);
	  if (ichan == mychan[1] &&  slotindADC[islot]== myslot[1]) hadc2[1]->Fill(rdata);
 
  
	  hadc3[islot]->Fill(rdata,float(ichan)); // all channels of this slot
	  hadcped[islot][ichan]->Fill(rdata); // for reading peadestals
	}

      } // end of ADC

      // TDC
      //   if (numLocal >= 1){
      Int_t ngoodpulses = 0; // stores the number of hit couples, that is: a leading edge and a trailing edge in successive hits
      for (islot = 0; islot < NUMTDCSLOTS; islot++) {
	cout <<"TDC analysis: islot " << islot << " slotindTDC[islot] " << slotindTDC[islot] << " numslothitsTDC[islot] " << numslothitsTDC[islot] << endl;
	htdc1->Fill(slotindTDC[islot] ,numslothitsTDC[islot]);
	ihit=0;
	while (ihit < numslothitsTDC[islot] ) { 
	  if (tdcdat[LEADING_EDGE][ihit][islot] > 0 && tdcdat[TRAILING_EDGE][ihit+1][islot] > 0) {
	    ngoodpulses++;
      	    rawtimes[TRAILING_EDGE] = tdcdat[TRAILING_EDGE][ihit+1][islot];
      	    rawtimes[LEADING_EDGE]  = tdcdat[LEADING_EDGE] [ihit][islot];
      	    ichan = tdcchan[ihit][islot];
	    if (debug) cout << " TDC (before filling) slot " << slotindTDC[islot] << " " << numslothitsTDC[islot] << " " << ichan << " " << rawtimes[0]<< " " << rawtimes[1]<< " " << rawtimes[TRAILING_EDGE] - rawtimes[LEADING_EDGE] << endl;
      	    hleadingtime->Fill(float(rawtimes[LEADING_EDGE]));
      	    htdcpulsewidth->Fill(float(rawtimes[TRAILING_EDGE] - rawtimes[LEADING_EDGE]));
	    ihit=ihit+1;
	  } else {
	    ihit++;
	  }
	}

	cout << "    TDC analysis: event = "<< ievent <<" slot = " << islot << " numslothitsTDC = " << numslothitsTDC[islot] <<", found " << ngoodpulses << " good pulses " << endl;

	//	ngoodpulsestotal[islot] += ngoodpulses;
	//cout<< "    TDC: up to now, in slot " << islot << " there are " << ngoodpulsestotal[islot] << " (total)" <<endl; 

      } // end of TDC
	//	}
      
    }

      void pedsup(){
  
	ofstream outfile("ped_test.dat");
	Double_t maxbin, pedx, maxx, minx, mean = 0, sigma=0; 
	Int_t thres = 0;
	TF1* gausfit=new TF1("gausfit","gaus",0,2000);
	for (int i = 0; i < NUMADCSLOTS;i++) {
	  outfile << "slot=" << slotindADC[i] << endl;
	  for (int j = 0; j < NUMCHANA;j++) {
	    maxbin =  hadcped[i][j]->GetMaximumBin();
	    pedx =  hadcped[i][j]->GetBinCenter(maxbin);
	    maxx = pedx + 50;
	    minx = pedx - 50; 
	    hadcped[i][j]->Fit("gausfit","","",minx,maxx);
	    mean  = gausfit->GetParameter(1);
	    sigma = gausfit->GetParameter(2);
	    thres = int(mean + 5*sigma);
	    outfile <<  thres << endl;
	  }
	}
	outfile.close();
      }


void readpedsup(){
  
  ifstream infile("ped_test.dat");
  Int_t thres = 0;
  Int_t slot = 0;
  char header[8]; // will contain "slot=.." where .. are the slot digits
  char slotstr[2]; // helps parsing header
  
  for (int i = 0; i < NUMADCSLOTS;i++) {
    infile >> header; // slot=..
    //cout << "readpedsup(): read line: " << header << endl;
    slotstr[0] = header[5]; // first slot digit
    slotstr[1] = header[6]; // second slot digit
    slot = atoi(slotstr);   // convert header substring (saved in slotstr) to slot#
    cout << "readpedsup(): reading pedestals of ADC slot " <<slot << "..." << endl;
    for (int j = 0; j < NUMCHANA;j++) {
      infile >>  thres;
      pedestal_threshold[j][slot] = thres;
      //cout << "readpedsup(): value = " << pedestal_threshold[slot][j] <<endl;
    }
  }
  
  infile.close();
  cout << "readpedsup(): done! "<<endl;
}
