//ww.youtube.com/watch?v=4dqNf7liubs&list=PLLybgCU6QCGWLdDO4ZDaB0kLrO3maeYAe&index=24

#include "TROOT.h"
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLatex.h"
#include "TList.h"
#include "TPad.h"
#include "TPaveStats.h"
#include "TPaveText.h"
#include "TProfile.h"
#include "TString.h"
#include "TStyle.h"
#include "TText.h"
#include "TTree.h"
#include "TGraph.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

static const Int_t NUMPMT = 14;
static const Int_t NUMPIXEL = 16;
static const Int_t NUMPADDLE = 14;
static const Int_t NUMPADDLES = NUMPMT*NUMPIXEL;

static const float adc_charge = 50*1e-15; // 50 fC, corresponding to an ADC chan
static const float e = 1.6e-19; // C, electron charge
static const Int_t xcanvas = 800; // width of canvases
static const Int_t ycanvas = 800; // height of canvases

TTree *T; // tree from root file, raw adc/tdc/hit data
// local arrays to hold data per event from tree
Double_t adc[NUMPADDLES];
Double_t adc_c[NUMPADDLES];
Double_t tdcl[NUMPADDLES];
Double_t tdcl_c[NUMPADDLES];
Double_t tdct[NUMPADDLES];
Double_t tdct_c[NUMPADDLES];
Double_t nahit;
Double_t nthit;
Double_t nhit;

//Latest map set for M1-L
//Int_t pixel1[NUMPMT]={1, 2, 13,4, 6, 12, 1,1, 1,5,13,1, 1,8};
//Int_t pixel2[NUMPMT]={14, 16,16,6,13,13,13,3,6,9,15,4,16,12};
//Map for M2-L
//Int_t pixel1[NUMPMT]={11,12, 4,1, 4, 4, 9,4, 2, 4, 7,11,1,13 };
//Int_t pixel2[NUMPMT]={12,13,14,4,14,16,16,7,16,14,11,13,8,14 };
//Map for M3-L
//Int_t pixel1[NUMPMT]={3, 7, 2,11, 6,12, 1,1, 1,14,2,2,1, 1 };
//Int_t pixel2[NUMPMT]={4,11,13,14,13,15,13,4,13,16,6,6,4,15 };
//Map for M5-L
Int_t pixel1[NUMPMT]={1, 3, 1, 4,13,13, 1, 6, 8,11,13,13,1, 1};
Int_t pixel2[NUMPMT]={13,16,6,15,16,16,13,16,13,16,16,16,14,4};

Int_t paddleindex[NUMPADDLES];

Int_t run; // run number used in titles of plots
Int_t n_events_to_analyze; // number of events to analyze ... -1 = all.
TStyle *MyStyle = new TStyle("MyStyle","MyStyle");

void Chain(Int_t runno, Int_t n_events=-1){
 
  TString filename;
  filename.Form("scint_%d.root",runno);
  TFile *_file0 = TFile::Open(filename);

  run=runno;
// Set Canvas preferences
  MyStyle->SetTitleFontSize(0.08);
  MyStyle->SetTitleX(0.15);
  MyStyle->SetTitleY(0.99);
  MyStyle->SetStatW(0.9);
  MyStyle->SetMarkerStyle(6);
  gStyle->SetCanvasDefH(xcanvas);
  gStyle->SetCanvasDefW(ycanvas);
  gStyle->SetPalette(1);
  gROOT->SetStyle("MyStyle");

  T = (TTree *)_file0->Get("T"); 
// Setting addresses so we can call them later
  T->SetBranchAddress("C.cdetm1r.adc",&adc);
  T->SetBranchAddress("C.cdetm1r.adc_c",&adc_c);
  T->SetBranchAddress("C.cdetm1r.tdcl",&tdcl);
  T->SetBranchAddress("C.cdetm1r.tdct",&tdct);
  T->SetBranchAddress("C.cdetm1r.tdcl_c",&tdcl_c);
  T->SetBranchAddress("C.cdetm1r.tdct_c",&tdct_c);
  T->SetBranchAddress("C.cdetm1r.nhit",&nhit);
  T->SetBranchAddress("C.cdetm1r.nahit",&nahit);
  T->SetBranchAddress("C.cdetm1r.nthit",&nthit);

  Int_t n_entries = T->GetEntries(); // Checks how many total entries in T
  cout << "Found " << n_entries << " events"<<endl;
  if (n_events==-1){
	n_events_to_analyze = n_entries;
	cout << "Analyzing all events." << endl;
  } else {
	if (n_events < n_entries) {
		n_events_to_analyze = n_events;
		cout << "Analyzing " << n_events << " events." << endl;
  	}else{
		n_events_to_analyze = n_entries;
		cout << "Analyzing " << n_entries << " events." << endl;
	}
  }

 }


void setPaddleIndices(){
	// Setting the location of paddles, taking into account the missing pixels in each PMT
          for(Int_t pmt=0; pmt<NUMPMT;pmt++){
	    Int_t ipaddle = (pmt)*NUMPADDLE;
                for(Int_t pixel=0; pixel<NUMPIXEL;pixel++){
		  Int_t index = (pmt*NUMPIXEL)+pixel;
                        if (pixel!=pixel1[pmt]-1&&pixel!=pixel2[pmt]-1){
                                ipaddle++;
				paddleindex[ipaddle]=index;
                        }
                }
           }
	   return;
}



Int_t getPaddleIndex(Int_t pmt, Int_t ipaddle){
  //pmt numbers are from 0 to 13 as are ipaddle numbers and the code accounts for removed pixels
	setPaddleIndices();

	Int_t localpaddle = (pmt+1)*NUMPADDLE-ipaddle;

	return paddleindex[localpaddle];
}

Int_t getPaddlePixel(Int_t pmt, Int_t ipaddle){
  //gives the pixel number for the given paddle number of the pmt, again paddle and pmt numbers are from 0 to 13
	setPaddleIndices();

	Int_t localpaddle = (pmt+1)*NUMPADDLE-ipaddle;

	return paddleindex[localpaddle]-(pmt)*NUMPIXEL;
}  

void print_event(Int_t adc_cut=50, Int_t start_event=1, Int_t num_events=1, Int_t tdc_min=750, Int_t tdc_width=300){

        for (Int_t id=start_event;id<start_event+num_events;id++){
          T->GetEntry(id);
	  cout << "Event " << id << endl;
	  for (Int_t pmt=1; pmt<=NUMPMT;pmt++){
	    for (Int_t index=1; index<=NUMPIXEL; index++){
	      Int_t ipixel = (pmt-1)*NUMPIXEL+index-1;
	      if(tdcl[ipixel]>tdc_min&&tdcl[ipixel]<tdc_min+tdc_width){
		cout << "Leading Edge TDC Hit on global pixel " << ipixel << " = " << tdcl[ipixel] << endl;
	      }
	      if(tdct[ipixel]>tdc_min&&tdct[ipixel]<tdc_min+tdc_width){
		cout << "Trailing Edge TDC Hit on global pixel " << ipixel << " = " << tdct[ipixel] << endl;
	      }
	      if(adc_c[ipixel]>adc_cut){
		cout << "ADC Hit on global pixel " << ipixel << " = " << adc_c[ipixel] << endl;
	      }	
	    }
	  }
	}
	
	return;
}

TCanvas *plot_adc(Int_t pmt=1, Int_t tdc_min=750, Int_t tdc_width=300){

        TString cut, draw, draw1, title;
        title.Form("run_%d_ADC",run);
        TCanvas *cADC= new TCanvas("cADC",title,xcanvas,ycanvas);

        TH1D *htmpa[NUMPIXEL];//=new TH1D("htmp","htmp",nbin,min,max);
        TH1D *htmpb[NUMPIXEL];//=new TH1D("htmp1","htmp1",nbin,min,max);

        TString tmpentry;
        MyStyle->SetStatX(0.9);
        MyStyle->SetStatY(0.6);
        MyStyle->SetStatW(0.4);

        Int_t nbin=600;
        Int_t min=-100, max=500;
        for(Int_t icounter=1;icounter<=NUMPIXEL;icounter++){
                tmpentry.Form("htmpa%d", icounter);
                htmpa[icounter - 1] = new TH1D(tmpentry,tmpentry,nbin,min,max);
                tmpentry.Form("htmpb%d", icounter);
                htmpb[icounter - 1] = new TH1D(tmpentry,tmpentry,nbin,min,max);
                htmpa[icounter - 1]->SetLineColor(kBlue);
                htmpb[icounter - 1]->SetLineColor(kRed);
                title.Form("Run %d ADC pmt %d, paddle %d: %d < tdc < %d",run,pmt,icounter,tdc_min,tdc_min+tdc_width);
                htmpa[icounter - 1]->SetTitle(title);
                htmpb[icounter - 1]->SetTitle(title);
        }

        Int_t nentries=n_events_to_analyze;

        for (Int_t id=1;id<=nentries;id++){
          T->GetEntry(id);
	  for (Int_t index=1; index<=NUMPIXEL; index++){
	    Int_t ipaddle = (pmt-1)*NUMPIXEL+index-1;
	    htmpa[index-1]->Fill(adc_c[ipaddle]);
	    if(tdcl[ipaddle]>tdc_min&&tdcl[ipaddle]<tdc_min+tdc_width){
		htmpb[index-1]->Fill(adc_c[ipaddle]);
	    }
	  }
	}

	cADC->Clear();
        cADC->Divide(4,4);

        //plot histos
        Int_t icount=0;
        for (Int_t i=0; i<NUMPIXEL; i++){

          if(i != pixel1[pmt-1]-1 && i != pixel2[pmt-1]-1) {

            //cout<<"into loop 2, i = " << i << endl;

            cADC->cd( icount + 1 );
            gPad->SetLogy();

            cADC->Update();
                
            Int_t entries = htmpa[i]->GetEntries();
            float mean = htmpa[i]->GetMean(1);
            float RMS = htmpa[i]->GetRMS(1);
            //cout << entries <<" "<< mean <<" "<< RMS <<endl;
                
            htmpa[i]->SetStats(0);
            //current->Modified();
            htmpa[i]->Draw();
            htmpb[i]->Draw("same");

            icount++;
          
	  }
        }

        title.Form("run_%d_ADC_pmt_%d_tdc_min_%d_max_%d.png",
                   run,pmt,tdc_min,tdc_min+tdc_width);
        cADC->Print(title);
        cADC->cd(0);
        return cADC;
}


