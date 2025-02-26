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
// Int_t pixel1[NUMPMT]={1, 3, 1, 4,13,13, 1, 6, 8,11,13,13,1, 1};
// Int_t pixel2[NUMPMT]={13,16,6,15,16,16,13,16,13,16,16,16,14,4};
//Map for M4-L
Int_t pixel1[NUMPMT]={13,13,10, 7, 5, 1, 1, 4, 2, 4,1,5, 1, 2};
Int_t pixel2[NUMPMT]={16,16,16,11,16,15,13,13,16,13,4,9,13,13};

Int_t paddleindex[NUMPADDLES];

Int_t run; // run number used in titles of plots
Int_t n_events_to_analyze; // number of events to analyze ... -1 = all.
TStyle *MyStyle = new TStyle("MyStyle","MyStyle");

TCanvas *threshold(Int_t pmt=1, Int_t tdc_min=750, Int_t tdc_width=200){

        TString cut, draw, draw1, title;
        title.Form("run_%d_ADCMEANRATIO",run);
        TCanvas *cADCMEANRATIO= new TCanvas("cADCMEANRATIO",title,xcanvas,ycanvas);

        TH1D *htmpa[NUMPIXEL];//=new TH1D("htmp","htmp",nbin,min,max);
        TH1D *htmpb[NUMPIXEL];//=new TH1D("htmp1","htmp1",nbin,min,max);
        TH1D *htmpc[NUMPIXEL];//=new TH1D("htmp1","htmp1",nbin,min,max);

        TString tmpentry;
        MyStyle->SetStatX(0.9);
        MyStyle->SetStatY(0.6);
        MyStyle->SetStatW(0.4);

        Int_t nbin=25;
        Int_t min=0, max=200;
        for(Int_t icounter=1;icounter<=NUMPIXEL;icounter++){
                tmpentry.Form("htmpa%d", icounter);
                htmpa[icounter - 1] = new TH1D(tmpentry,tmpentry,nbin,min,max);
                tmpentry.Form("htmpb%d", icounter);
                htmpb[icounter - 1] = new TH1D(tmpentry,tmpentry,nbin,min,max);
                tmpentry.Form("htmpc%d", icounter);
                htmpc[icounter - 1] = new TH1D(tmpentry,tmpentry,nbin,min,max);
                htmpa[icounter - 1]->SetLineColor(kBlue);
                htmpb[icounter - 1]->SetLineColor(kRed);
                htmpc[icounter - 1]->SetLineColor(kRed);
                title.Form("Run %d ADC pmt %d, paddle %d: %d < tdc < %d",run,pmt,icounter,tdc_min,tdc_min+tdc_width);
                htmpa[icounter - 1]->SetTitle(title);
                htmpb[icounter - 1]->SetTitle(title);
                htmpc[icounter - 1]->SetTitle(title);
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

	cADCMEANRATIO->Clear();

        //plot histos
        Int_t icount=0;
	cADCMEANRATIO->cd();

        Double_t mean[NUMPADDLE]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        Double_t sigma[NUMPADDLE]={1,1,1,1,1,1,1,1,1,1,1,1,1,1};
        Double_t paddle[NUMPADDLE]={1,2,3,4,5,6,7,8,9,10,11,12,13,14};
        Double_t epaddle[NUMPADDLE]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        TF1 *myfit = new TF1("myfit","1.0-0.5*ROOT::Math::erfc((x-[0])/[1])",0,1);
        myfit->SetParName(0,"Mean");
        myfit->SetParName(1,"Width");

        for (Int_t i=0; i<NUMPIXEL; i++){

          if(i != pixel1[pmt-1]-1 && i != pixel2[pmt-1]-1) {

            htmpb[i]->SetStats(0);

            myfit->SetParameter(0,40.0);
            myfit->SetParameter(1,10.0);
            htmpc[i] = (TH1D*)htmpb[i]->Clone();
            htmpc[i]->Divide(htmpa[i]);
            htmpc[i]->Fit("myfit");

	    Int_t numentries = htmpc[i]->GetEntries();
	    mean[icount]=myfit->GetParameter(0);
	    //sigma[icount]=myfit->GetParameter(1)/sqrt(numentries);
	    sigma[icount]=myfit->GetParameter(1);

            icount++;
          
	  }
        }

        TGraphErrors *gr = new TGraphErrors(NUMPADDLE,paddle,mean,epaddle,sigma);
        gr->SetMarkerStyle(21);
        gr->GetXaxis()->SetTitle("Paddle Number");
        gr->GetYaxis()->SetTitle("50% Threshold (Good TDC)");
	gr->GetHistogram()->SetMaximum(100);
	gr->GetHistogram()->SetMinimum(0);

        gr->Draw("AP");

        cADCMEANRATIO->Update();

        title.Form("run_%d_ADCMEANRATIO_pmt_%d_tdc_min_%d_max_%d.png",
                   run,pmt,tdc_min,tdc_min+tdc_width);
        //cADCMEANRATIO->Print(title);
        cADCMEANRATIO->cd();
        return cADCMEANRATIO;

}

