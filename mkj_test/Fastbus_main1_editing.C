// A simple decoder and histogrammer for fastbus data.
// R. Michaels, Oct 24, 2014.
// developed later by Dasuni, et al.

#define MAXROC     32
#define NUMCHANT   96   // TDC
#define NUMCHANA   64   // ADC
#define NUMADCSLOTS   8   // ADC
#define MAXHITS    100
#define MAXEVENTS  1000000
#define NUMROC 2

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
#include "TProfile.h"
#include "TNtuple.h"
#include "TRandom.h"
#include <vector>

using namespace std;

void usage();
void decode(int* data);
void analysis();
void clear();

// Global data 
Int_t evlen, evtype, evnum;
Int_t *irn, *rocpos, *roclen;
Int_t myroc[NUMROC] = {1,2};
Int_t myslot[2][2] = {{6,8},{17,18}};
Int_t mychan[2][2] = {{16,16},{16,16}};

Int_t *nthits;
Int_t *tdcdat,*numslothits;
Int_t adcdat[NUMCHANA][NUMADCSLOTS][NUMROC];
Int_t adcchan[NUMCHANA][NUMADCSLOTS][NUMROC];
TH1F *hadc1[2], *hadc2[2][2];
TH2F *hadc3[NUMROC][NUMADCSLOTS];
Int_t fHasHeader=1;   // =1 for 1877 and 1881, but =0 for 1875
Int_t fWordSeen;
Int_t slotind[NUMADCSLOTS][NUMROC];
Int_t debug=0;
Int_t numLocal = 0;

int main(int argc, char* argv[])
{

  THaCodaData *coda;      
  char ctitle[100];
  char dtitle[100];
  char rtitle[100];
  char htitle[100];
  Int_t maxevent = 10000;
  Int_t runno = 1000;
  Int_t ievent = 0;
  Int_t istatus;
  Int_t lslot;

  if (argc < 2) {
     maxevent = MAXEVENTS;
  }
  if (argc > 3 || argc < 2) {
     usage();
     return 1;
  }  

  int choice1 = 1;
  irn = new Int_t[MAXROC];
  rocpos = new Int_t[MAXROC];
  roclen = new Int_t[MAXROC];
 
  if (argc == 4) 
    maxevent = atoi(argv[3]);
  
  if (argc >1){
    runno = atoi(argv[1]);
  }
  
  for (int j = 0; j < NUMROC; j++){
    for (int i = 0; i < NUMADCSLOTS; i++){
      slotind[i][j] = myslot[i][j];
      myslot[i][j]++;
    }
  }

   

  cout << "Fastbus analysis "<<endl;
  cout << "Events to process "<<maxevent<<endl;

  nthits  = new Int_t[NUMCHANA];
  tdcdat  = new Int_t[NUMCHANT*MAXHITS];
  numslothits  = new Int_t[NUMADCSLOTS];

// Initialize root and output
  sprintf(rtitle,"sbs_%4d.root",runno);
  sprintf(dtitle,"../data/sbs_%4d.dat",runno);
  TROOT fbana("fbroot","Hall A SBS analysis for Fastbus");
  TFile hfile(rtitle,"RECREATE","SBS data");

  sprintf(ctitle,"ADC hits per slot %d", myroc[0]);
  hadc1[0] = new TH1F("hadc11",ctitle,31,-1,30.);
  sprintf(ctitle,"ADC data on slot %d channel %d roc %d",myslot[0][0],mychan[0][0], myroc[0]);
  hadc2[0][0] = new TH1F("hadc21_0",ctitle,200,0,4000.);
  sprintf(ctitle,"ADC data on slot %d channel %d roc %d",myslot[0][1],mychan[0][1], myroc[0]);
  hadc2[0][1] = new TH1F("hadc21_1",ctitle,200,0,4000.);

  sprintf(ctitle,"ADC hits per slot %d", myroc[1]);
  hadc1[1] = new TH1F("hadc12",ctitle,31,-1,30.);
  sprintf(ctitle,"ADC data on slot %d channel %d roc %d",myslot[1][0],mychan[1][0], myroc[1]);
  hadc2[1][0] = new TH1F("hadc22_0",ctitle,200,0,4000.);
  sprintf(ctitle,"ADC data on slot %d channel %d roc %d",myslot[1][1],mychan[1][1], myroc[1]);
  hadc2[1][1] = new TH1F("hadc22_1",ctitle,200,0,4000.);
 
  
  for (int j = 0; j < NUMROC; j++){
    for (int i = 0; i < NUMADCSLOTS;i++) {
      sprintf(ctitle,"ADC data on all channels of slot %d roc %d",slotind[i][j], myroc[0]);
      sprintf(htitle,"ADC_chan_slot_%d_roc_%d",slotind[i][j], myroc[0]);
      hadc3[j][i] = new TH2F(htitle,ctitle,200,0,2000.,64,0,64.);
    }
  }


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
      if (evtype < 10) analysis();

    }
  }

end1:
  
  printf("\n");
  printf("Number of events analyzed = %d \n",ievent);

  coda->codaClose();

  hfile.Write();
  hfile.Close();

  return 0;

}; //end of main function


void usage() {  
  cout << "Usage:  'fbana [runno] [maxevents] ' " << endl;
  cout << "Need [runno] [roc#] arguments "<<endl;
  cout << "[maxev] optional argument "<<endl;
  cout << "Can have most two arguments "<<endl;
}; 

void clear() {
  fWordSeen = 0;
  memset (nthits, 0, NUMCHANA*sizeof(Int_t));
  memset (adcdat, 0, NUMADCSLOTS*NUMCHANA*sizeof(Int_t));
  memset (adcchan, 0, NUMADCSLOTS*NUMCHANA*sizeof(Int_t));
  memset (numslothits, 0, NUMADCSLOTS*sizeof(Int_t));
  memset (tdcdat, 0, NUMCHANT*MAXHITS*sizeof(Int_t));
}

void decode (int* data) {

  Int_t found_dat;
  Int_t ichan, rdata;
  evlen = data[0] + 1;
  evtype = data[1]>>16;
  evnum = data[4];
  static int dodump = 0;  // dump the raw data

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
  // n1 = pointer to first word of ROC
  int pos = data[2]+3;
  int nroc = 0;
  while( pos+1 < evlen && nroc < MAXROC ) {
     int len  = data[pos];
     int iroc = (data[pos+1]&0xff0000)>>16;
     if(iroc>=MAXROC || nroc >= MAXROC) {
  	 cout << "Decoder error:  ";
	 cout << "  ROC num " <<dec<<iroc;
         cout << "  nroc "<<nroc<<endl;
         return;
       }
// Save position and length of each found ROC data block
       rocpos[iroc]  = pos;
       roclen[iroc]  = len;
       irn[nroc++] = iroc;
       pos += len+1;
  }
 
  for (int i=0; i < NUMROC; i++){
    if (debug)  cout << " roc pos = " << rocpos[myroc[i]] <<  " roc len = " << roclen[myroc[i]] << endl; 
    Int_t found = 0;
    for (int j=0; j<nroc; j++) {
      if(myroc[i] == irn[j]) found=1;
    }
    if (!found && debug) {
      cout << "ROC "<<dec<<myroc<<" not in datastream ";
      cout << "  for event "<<evnum<<"  evtype "<<evtype<<endl;
      return;
    }
    
    if (debug) {
      cout << "Roc info "<<nroc<<endl;
      for (int j=0; j<nroc; j++) {
	Int_t iroc = irn[j];
	cout << "irn "<<dec<<iroc<<"   pos "<<rocpos[iroc];
	cout <<"   len "<<roclen[iroc]<<endl;
      }
    }   
  } 
 

  // loop to find header of ADC block read
  
  for (int i = 0; i < NUMROC; i++){

    int index=rocpos[myroc[i]];
    int indexlast=rocpos[myroc[i]];
    if (debug) cout << " START LOOKING AT  at index = " << index << " last = "<< indexlast << " " << roclen[myroc[i]]<< endl;
    while (index<(rocpos[myroc[i]]+roclen[myroc[i]]) && ((data[index]&0xffffffff)!=0x0da000011)) {
      index++;
    }
    while (indexlast<(rocpos[myroc[i]]+roclen[myroc[i]]) && ((data[indexlast]&0xffffffff)!=0x0da000022)) {
      indexlast++;
    }
   
    int slotnew=0;
    if (debug) cout << " ADC header at index = " << index << " last = "<< indexlast << endl;
    
    for (int j = index+1; j < indexlast; j++) {
      
      if (debug) printf("data[%d] = 0x%x = (dec)  \n",j,data[j]);
      
      int slot = (data[j]&0xf8000000)>>27; 
      int slot_ndata = 0;

      if (slot!=slotnew) {
	slotnew=slot;
	slot_ndata= (data[j]&0x7f) - 1;
      }
      int slotindex=0;
      for (int jj=0;jj<NUMADCSLOTS;jj++) {
	if (slot==slotind[jj][i]) slotindex=jj;
      }
      if (debug) cout << "slot "<<slot<< " slot index = " << slotindex << " slot indata = " << slot_ndata << endl;
      for (int ii=0;ii<slot_ndata;ii++){
	j++;
	if (debug==2) printf("data[%d] = 0x%x = (dec) %d %d  \n",j,data[j],(data[j]&0xfe0000)>>17,data[j]&0x3fff);
	ichan = (data[j]&0xfe0000)>>17; // 1881
	rdata = data[j]&0x3fff;  // 1881
	if (ichan >= 0 && ichan < NUMCHANA && rdata > 0) {
	  adcdat[ii][slotindex][i] = rdata; 
	  adcchan[ii][slotindex][i] = ichan; 
	}
      }
      numslothits[slotindex]=slot_ndata;
    }

  }
}



void analysis() {

  Int_t islot, ichan,ihit, rdata;
  if (debug) cout << " analysis " << endl;

  //if (numLocal >= 1){
  for (int i = 0 ; i < NUMROC; i++){
    for (islot = 0; islot < NUMADCSLOTS; islot++) {
     hadc1[i]->Fill(slotind[islot][i],numslothits[islot]);
      if (debug) cout << " slot " << slotind[islot] << " " << numslothits[islot] << endl;
      for (ihit = 0; ihit < numslothits[islot] ; ihit++) {
          rdata = adcdat[ihit][islot][i];
          ichan = adcchan[ihit][islot][i];
	  
	  if (ichan == mychan[i][0] &&  slotind[islot][i]== myslot[i][0]) hadc2[i][0]->Fill(rdata);
	  if (ichan == mychan[i][1] &&  slotind[islot][i]== myslot[i][1]) hadc2[i][1]->Fill(rdata);
	  hadc3[i][islot]->Fill(rdata,float(ichan)); // all channels of this slot
	 
      }

    }
  }
    //}
}




