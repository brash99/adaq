// A simple decoder and histogrammer for fastbus data.
// R. Michaels, Oct 24, 2014.
// developed later by Dasuni, et al.

#define MAXROC     32
#define NUMCHANT   96   // TDC
#define NUMCHANA   64   // ADC
#define NUMADCSLOTS   8   // ADC
#define MAXHITS    100
#define MAXEVENTS  1000000

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
Int_t myroc, myslot, mychan;
Int_t *nthits;
Int_t *tdcdat,*numslothits;
Int_t adcdat[NUMCHANA][NUMADCSLOTS];
Int_t adcchan[NUMCHANA][NUMADCSLOTS];
TH1F *hadc1, *hadc2;
TH2F  *hadc3[NUMADCSLOTS];
Int_t fHasHeader=1;   // =1 for 1877 and 1881, but =0 for 1875
Int_t fWordSeen;
Int_t slotind[NUMADCSLOTS]={17,18,19,20,21,22,23,24};
//Int_t slotind[NUMADCSLOTS]={6, 7, 8, 9, 10, 11, 12, 13};
Int_t debug=1;

int main(int argc, char* argv[])
{

  THaCodaData *coda;      
  char ctitle[100];
  char htitle[100];
  Int_t maxevent = 10000;
  Int_t ievent = 0;
  Int_t istatus;

  if (argc < 2) {
     maxevent = MAXEVENTS;
  }
  if (argc > 2) {
     usage();
     return 1;
  }  

  int choice1 = 1;
  myroc=2;  myslot=20;  mychan = 16;   // defaults

  irn = new Int_t[MAXROC];
  rocpos = new Int_t[MAXROC];
  roclen = new Int_t[MAXROC];
 
  if (argc > 1) maxevent = atoi(argv[1]);
 
  cout << "Fastbus analysis "<<endl;
  cout << "Events to process "<<maxevent<<endl;

  nthits  = new Int_t[NUMCHANA];
  tdcdat  = new Int_t[NUMCHANT*MAXHITS];
  numslothits  = new Int_t[NUMADCSLOTS];

// Initialize root and output
  TROOT fbana("fbroot","Hall A SBS analysis for Fastbus");
  TFile hfile("sbs.root","RECREATE","SBS data");

  sprintf(ctitle,"ADC hits per slot ");
  hadc1 = new TH1F("hadc1",ctitle,30,-1,30.);
  sprintf(ctitle,"ADC data on slot %d channel %d",myslot,mychan);
  hadc2 = new TH1F("hadc2",ctitle,200,0,4000.);
  for (int i = 0; i < NUMADCSLOTS;i++) {
     sprintf(ctitle,"ADC data on all channels of slot %d",slotind[i]);
     sprintf(htitle,"ADC_chan_slot_%d",slotind[i]);
     hadc3[i] = new TH2F(htitle,ctitle,200,0,2000.,64,0,64.);
  }
  if (choice1 == 1) {  // CODA File
    
    // CODA file "run.dat" may be a link to CODA datafile on disk
    TString filename("run.dat");

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
  cout << "Usage:  'fbana [maxevents] ' " << endl;
  cout << "Can have most one optional argument "<<endl;
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
  static int dodump = 1;  // dump the raw data

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
   while( (data[pos++]&0xda000011) !=0) {
   if (debug) cout << " pos = " << pos << " data = " << data[pos] << endl; 
 }
   //       rocpos[myroc]  = pos;
 if (debug)  cout << " roc pos = " << rocpos[myroc] <<  " roc len = " << roclen[myroc] << endl; 
  Int_t found = 0;
  for (int j=0; j<nroc; j++) {
     if(myroc == irn[j]) found=1;
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

 // Go over the data in myroc

  // loop to find header of ADC block read
  int index=0;
  while (index<evlen && ((data[index]&0xffffffff)!=0x0da000011)) {
    index++;
  }
  int indexlast=0;
  while (indexlast<evlen && ((data[indexlast]&0xffffffff)!=0x0da000022)) {
    indexlast++;
  }
  int size;
  int slotnew=0;
  if (debug) cout << " ADC header at index = " << index << " last = "<< indexlast << endl;
  for (int j = index+1; j < indexlast; j++) {

    if (debug) printf("data[%d] = 0x%x = (dec)  \n",j,data[j]);

    int slot = (data[j]&0xf8000000)>>27; 
    int slot_ndata;
    if (slot!=slotnew) {
      slotnew=slot;
      slot_ndata= (data[j]&0x7f) - 1;
    }
    int slotindex=0;
    for (int jj=0;jj<NUMADCSLOTS;jj++) {
      if (slot==slotind[jj]) slotindex=jj;
    }
    if (debug) cout << "slot "<<slot<< " slot index = " << slotindex << " slot indata = " << slot_ndata << endl;
    for (int ii=0;ii<slot_ndata;ii++){
      j++;
    if (debug) printf("data[%d] = 0x%x = (dec) %d %d  \n",j,data[j],(data[j]&0xfe0000)>>17,data[j]&0x3fff);
	   ichan = (data[j]&0xfe0000)>>17; // 1881
	   rdata = data[j]&0x3fff;  // 1881
           if (ichan >= 0 && ichan < NUMCHANA && rdata > 0) {
	        adcdat[ii][slotindex] = rdata; 
	        adcchan[ii][slotindex] = ichan; 
	   }
    }
    numslothits[slotindex]=slot_ndata;
  }
}



void analysis() {

  Int_t islot, ichan,ihit, rdata;
  if (debug) cout << " analysis " << endl;
  for (islot = 0; islot < NUMADCSLOTS; islot++) {
    hadc1->Fill(slotind[islot],numslothits[islot]);
    if (debug) cout << " slot " << slotind[islot] << " " << numslothits[islot] << endl;
     for (ihit = 0; ihit < numslothits[islot] ; ihit++) {
          rdata = adcdat[ihit][islot];
          ichan = adcchan[ihit][islot];
          if (ichan == mychan &&  slotind[islot]== myslot) hadc2->Fill(rdata);
 	  hadc3[islot]->Fill(rdata,float(ichan)); // all channels of this slot
      }

  }
}




