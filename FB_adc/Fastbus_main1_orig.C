// A simple decoder and histogrammer for fastbus data.
// R. Michaels, Oct 24, 2014.
// developed later by Dasuni, et al.

#define MAXROC     32
#define NUMCHANT   96   // TDC
#define NUMCHANA   64   // ADC
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
Int_t *tdcdat, *adcdat;
TH1F *htdc1, *htdc2, *htdc3;
Int_t fHasHeader=1;   // =1 for 1877 and 1881, but =0 for 1875
Int_t fWordSeen;

Int_t debug=1;

int main(int argc, char* argv[])
{

  THaCodaData *coda;      
  char ctitle[100];
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
  myroc=1;  myslot=1;  mychan = 6;   // defaults

  irn = new Int_t[MAXROC];
  rocpos = new Int_t[MAXROC];
  roclen = new Int_t[MAXROC];
 
  if (argc > 1) maxevent = atoi(argv[1]);
 
  cout << "Fastbus analysis "<<endl;
  cout << "Events to process "<<maxevent<<endl;

  nthits  = new Int_t[NUMCHANT];
  tdcdat  = new Int_t[NUMCHANT*MAXHITS];
  adcdat  = new Int_t[NUMCHANA];

// Initialize root and output
  TROOT fbana("fbroot","Hall A SBS analysis for Fastbus");
  TFile hfile("sbs.root","RECREATE","SBS data");

  sprintf(ctitle,"TDC hits per channel on slot %d",myslot);
  htdc1 = new TH1F("htdc1",ctitle,20,-1,19.);
  sprintf(ctitle,"TDC data on slot %d channel %d",myslot,mychan);
  htdc2 = new TH1F("htdc2",ctitle,200,0,4100.);
  sprintf(ctitle,"TDC data on all channels of slot %d",myslot);
  htdc3 = new TH1F("htdc3",ctitle,200,0,4100.);

  if (choice1 == 1) {  // CODA File
    
    // CODA file "run.dat" may be a link to CODA datafile on disk
    TString filename("run.dat");

    coda = new THaCodaFile();
    if (coda->codaOpen(filename) != 0) {
      cout << "ERROR:  Cannot open CODA data" << endl;
      goto end1;
    }

    
  } else {         // Online ET connection
    
    int mymode = 1;
    TString mycomputer("sbs1");
    TString mysession("whatever");
    
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
      analysis();

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
  memset (nthits, 0, NUMCHANT*sizeof(Int_t));
  memset (adcdat, 0, NUMCHANA*sizeof(Int_t));
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

  found_dat = 0;
 
  for (int j = rocpos[myroc]; j < rocpos[myroc]+roclen[myroc]; j++) {

    if (debug) printf("data[%d] = 0x%x = (dec) %d \n",j,data[j],data[j]);

    Int_t slot = data[j] >> 27;
    if (debug) cout << "slot "<<slot<<endl;

    if (slot == myslot) {

      if (fWordSeen > 0 || !fHasHeader) {  // skip header if there is one

         ichan = (data[j]&0xfe0000)>>17; // 1877 
     //  chan = (data[j]&0x7e0000)>>17;    // 1881

         rdata = data[j]&0xffff;  // 1877 
       // rdata = data[j]&0x3fff;  // 1881

           if (ichan >= 0 && ichan < NUMCHANT && rdata > 0) {
	     if (nthits[ichan] < MAXHITS-1) {

	        tdcdat[nthits[ichan]*NUMCHANT+ichan] = rdata;
                nthits[ichan]++;
                if (nthits[ichan] > 100) cout << "HUH ?"<<endl;

	     }
	   }
	}
      fWordSeen++;
 
    }
  }
}



void analysis() {

  Int_t ichan, ihit, rdata;

  for (ichan = 0; ichan < NUMCHANT; ichan++) {

    if (debug && (ichan == mychan)) cout << "Hits on mychan "<<myslot<<"  "<<mychan<<"  "<<nthits[ichan]<<endl;

    if (debug && nthits[ichan]>0) cout << "chan "<<ichan<<"  nthits "<<nthits[ichan]<<endl;

     htdc1->Fill(nthits[ichan]);

     for (ihit = 0; ihit < nthits[ichan]; ihit++) {
 
          rdata = tdcdat[ihit*NUMCHANT+ichan];

          if (debug) cout << "data on myslot "<<rdata<<endl;

          if (ichan == mychan) htdc2->Fill(rdata);
	  htdc3->Fill(rdata); // all channels of this slot

     }
  }
}




