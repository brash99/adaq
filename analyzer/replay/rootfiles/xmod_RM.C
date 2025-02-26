// 11th July 2024
//
// Script to make various crosstalk plots per module.
//


#ifndef __CINT__
#include <iostream>
#include <fstream>
#endif
#include <cstdio>
#include <cstring>
#include <string>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "TMath.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TROOT.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TCut.h"
#include "TCutG.h"
#include "TCanvas.h"
#include "TFormula.h"
#include "TTree.h"
#include "TPad.h"
#include "TArrow.h"
#include "TLine.h"
#include "TString.h"
#include "TLatex.h"
#include "TPostScript.h"
#include "TText.h"
#include "TColor.h"
#include "TColorGradient.h"
#include "TPaveStats.h"
#include "TPaveText.h"
#include "TProfile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TList.h"

using namespace std;

static const Int_t NUMPMT = 14;
static const Int_t NUMPIXEL = 16;
static const Int_t NUMPADDLE = 14;
static const Int_t NUMMOD = 12;
static const Int_t NUMSIDE = 2;
static const Int_t NUMPADDLES = NUMPMT*NUMPIXEL;
static const Int_t NUMPIXELS = NUMPMT*NUMPIXEL*NUMMOD;
static const Int_t NUMPMTS = NUMPMT*NUMMOD;
static const Int_t NUMRUNS = 5;
static const Int_t NUMBINS = NUMRUNS*NUMPADDLE;

Double_t adc[NUMPADDLES];
Double_t adc_c[NUMPADDLES];
Double_t tdct[NUMPADDLES];
Double_t tdcl[NUMPADDLES];
Double_t tdct[NUMPADDLES];
Double_t tdct_c[NUMPADDLES];
Double_t nahit;
Double_t nthit;
Double_t nhit;

//Latest map set for M1-L
Int_t pixel1[NUMPMT]={ 1, 2,13,4, 6,12, 4,1,1,5,13,1, 1, 8};
Int_t pixel2[NUMPMT]={14,16,16,6,13,13,16,3,6,9,15,4,16,12};
//Map set for M1-R
//Int_t pixel1[NUMPMT]={4, 4, 2, 1, 4, 3, 4,13, 4,13,13,13, 5,13};
//Int_t pixel2[NUMPMT]={5, 8,11,13,14,13,16,16,16,16,16,16,16,16};
//Map set for M2-R
//Int_t pixel1[NUMPMT]={6, 4, 1, 13, 2, 13, 4,14, 4,2,10,10, 13,4};
//Int_t pixel2[NUMPMT]={13, 8,13,16,13,16,16,16,16,16,12,11,16,14};
//Map set for M2-L
//Int_t pixel1[NUMPMT]={11,12, 4,1, 4, 4, 9,4, 2, 4, 7,11,1,13 };
//Int_t pixel2[NUMPMT]={12,13,14,4,14,16,16,7,16,14,11,13,8,14 };
//Map set for M3-R
//Int_t pixel1[NUMPMT]={ 1, 4, 4,2, 5, 1, 4, 3,13,1,13,1, 2, 4};
//Int_t pixel2[NUMPMT]={10,16,15,3,14,14,14,16,16,4,16,4,16,14};
//Map for M3-L
//Int_t pixel1[NUMPMT]={3, 7, 2,11, 6,12, 1,1, 1,14,2,2,1, 1 };
//Int_t pixel2[NUMPMT]={4,11,13,14,13,15,13,4,13,16,6,6,4,15 };
//Map for M4-R
//Int_t pixel1[NUMPMT]={ 3, 2, 1, 5, 4,13,13,13,1,15, 2, 4, 1, 1};
//Int_t pixel2[NUMPMT]={11,11,14,16,14,16,16,16,7,16,14,16,14,16};
//Map for M4-L
//Int_t pixel1[NUMPMT]={13,13,10, 7, 5, 1, 1, 4, 2, 4,1,5, 1, 2};
//Int_t pixel2[NUMPMT]={16,16,16,11,16,15,13,13,16,13,4,9,13,13};
//Map set for M5-R
//Int_t pixel1[NUMPMT]={ 6,  1, 1, 13, 13, 15, 13,  4,  5,  2,  1,  1, 13,  6};
//Int_t pixel2[NUMPMT]={16, 16, 4, 16, 16, 16, 16, 16, 16, 13, 14, 16, 16, 16};
//Map set for M5-L
//Int_t pixel1[NUMPMT]={ 1, 3,1, 4,13,13, 1, 6, 8,11,13,13, 1,1};
//Int_t pixel2[NUMPMT]={13,16,6,15,16,16,13,16,13,16,16,16,14,4};
//Map set for M6-R
//Int_t pixel1[NUMPMT]={ 1, 5, 6,13, 7,15,13,15, 6, 1,15,13,13,13};
//Int_t pixel2[NUMPMT]={14,16,16,16,16,16,16,16,16,13,16,16,16,16};
//Map for M6-L
//Int_t pixel1[NUMPMT]={9,15,6, 4, 3, 1, 1, 6, 13, 7, 15, 4, 3, 1};
//Int_t pixel2[NUMPMT]={13,16,13,7,13,16,16,13,16,13,16,13,13,16};

void xmod_RM(Int_t mod=1, Int_t rn=4, Int_t div = 10){

  // Set some of the styles first
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetTitleSize(0.05,"y");
  gStyle->SetTitleOffset(1.4,"a");
  gStyle->SetTitleOffset(1.0,"x");
  // gStyle->SetTitleOffset(1.7,"y");
  gStyle->SetLabelSize(0.03,"x");
  gStyle->SetLabelSize(0.03,"y");
  gStyle->SetPadColor(0);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetOptTitle(1);
  gStyle->SetStatColor(0); 
  gStyle->SetStatH(0.16);
  gStyle->SetStatW(0.16);
  gStyle->UseCurrentStyle();
  gStyle->SetMarkerStyle(22);
  gStyle->SetMarkerColor(kBlack);
  gStyle->SetLineColor(kBlack);
  gStyle->SetMarkerSize(1.2);

  Int_t paddleindex[NUMPADDLES];

  for(Int_t pmt=0; pmt<NUMPMT;pmt++){
    Int_t ipaddle = (pmt+1)*NUMPADDLE+1;
    for(Int_t pixel=0; pixel<NUMPIXEL;pixel++){
      Int_t index = pmt*NUMPIXEL+pixel;
      if (pixel!=pixel1[pmt]-1&&pixel!=pixel2[pmt]-1){
	ipaddle--;
	paddleindex[ipaddle]=index;
      }
    }
  }

        TString cut, draw, draw1, title;
	string tx[NUMSIDE][NUMPMT],px[NUMSIDE][NUMPMT];
	Int_t hv[NUMSIDE][NUMPMT][NUMRUNS];
	Int_t runs[NUMSIDE][NUMPMT][NUMRUNS];
	Int_t pmtn[NUMSIDE][NUMPMT];
	Int_t mp1[NUMSIDE][NUMPMT] = {0,1}{0,0,0,0,0,0,0,0,0,0,0,0,0,0};   // missing pixel numbers
	Int_t mp2[NUMSIDE][NUMPMT] = {0,1}{0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
	Int_t* pixels = new Int_t[NUMSIDE][NUMPMT][NUMPIXEL];
        title.Form("XTalk_MOD_%d",mod);
        TCanvas *cXMOD = new TCanvas("cXMOD","cXMOD",2000,1000);
	cXMOD->SetGrid();

	Double_t xhits[NUMSIDE][NUMPMT][NUMPIXEL] = new Double_t[NUMSIDE][NUMPMT][NUMPIXEL];
	Double_t hits[NUMSIDE][NUMPMT][NUMPIXEL] = new Double_t[NUMSIDE][NUMPMT][NUMPIXEL];
	Double_t dhits[NUMSIDE][NUMPMT][NUMPIXEL] = new Double_t[NUMSIDE][NUMPMT][NUMPIXEL];

	Double_t thr[NUMSIDE][NUMPMT][NUMRUNS][NUMPIXEL] = new Double_t[NUMSIDE][NUMPMT][NUMRUNS][NUMPIXEL];
	Double_t threshs[NUMSIDE][NUMPMT][NUMPIXEL] = new Double_t[NUMSIDE][NUMPMT][NUMPIXEL];
	Int_t tdcw[NUMSIDE][NUMPMT][NUMRUNS][NUMPIXEL] = new Int_t[NUMSIDE][NUMPMT][NUMRUNS][NUMPIXEL];   // 2D array TDC width cuts 
	Int_t twpx[NUMSIDE][NUMPMT][NUMPIXEL] = new Int_t[NUMSIDE][NUMPMT][NUMPIXEL]; 
	Int_t twidths[NUMSIDE][NUMPMT][NUMPIXEL] = new Int_t[NUMSIDE][NUMPMT][NUMPIXEL];

	TString inthres[NUMSIDE][NUMPMT];
	TString intwidth[NUMSIDE][NUMPMT];

	for(Int_t side=0; side<NUMSIDE;side++){
	  for(Int_t pmt=1; pmt<=NUMPMT; pmt++){
	    if(side==0){
	      inthres[side][pmt-1] = Form("M%dL_threshold_pmt%d.dat",mod,pmt);  // threshold data
	      cout << inthres[side][pmt-1] << endl;
	      intwidth[side][pmt-1] = Form("M%dL_tdcwidth_pmt%d.dat",mod,pmt);  // tdc width cut data
	      cout << intwidth[side][pmt-1] << endl;
	    }
	    else if(side==1){
	      inthres[side][pmt-1] = Form("M%dR_threshold_pmt%d.dat",mod,pmt);  // threshold data
	      cout << inthres[side][pmt-1] << endl;
	      intwidth[side][pmt-1] = Form("M%dR_tdcwidth_pmt%d.dat",mod,pmt);  // tdc width cut data
	      cout << intwidth[side][pmt-1] << endl;
	    }
	  }	
	}
	
        TString tmpentry;
	tmpentry.Form("hxheat");
        gStyle->SetStatX(0.9);
        gStyle->SetStatY(0.6);
        gStyle->SetStatW(0.4);

	TH2D *hxheat = new TH2D("hxheat","hxheat",4,-2,2,NUMPADDLE*NUMPMT,0,NUMPADDLE*NUMPMT);
        title.Form("XTalk: Module %d",mod);
        hxheat->SetTitle(title);
	hxheat->GetXaxis()->SetNdivisions(4);
	hxheat->GetYaxis()->SetNdivisions(14,14,0,0);

	TH2D *hxhits = new TH2D("hxhits", "hxhits", 4,-2,2,NUMPADDLE*NUMPMT,0,NUMPADDLE*NUMPMT);
	title.Form("Total Hits: Module %d", mod);
	hxhits->SetTitle(title);
	hxhits->GetXaxis()->SetNdivisions(4);
	hxhits->GetYaxis()->SetNdivisions(14,14,0,0);
	
	TH2D *hdb_hits = new TH2D("hdhits", "hdhits", 4,-2,2,NUMPADDLE*NUMPMT,0,NUMPADDLE*NUMPMT);
	title.Form("Double Hits: Module %d", mod);
	hdb_hits->SetTitle(title);
	hdb_hits->GetXaxis()->SetNdivisions(4);
	hdb_hits->GetYaxis()->SetNdivisions(14,14,0,0);

        TH2D *hxper = new TH2D("hxper","hxper",4,-2,2,NUMPADDLE*NUMPMT,0,NUMPADDLE*NUMPMT);
        title.Form("XTalk (%): Module %d",mod);
        hxper->SetTitle(title);
	hxper->GetXaxis()->SetNdivisions(4);
	hxper->GetYaxis()->SetNdivisions(14,14,0,0);

	TH2D *hdper = new TH2D("hdper", "hdper", 4,-2,2,NUMPADDLE*NUMPMT,0,NUMPADDLE*NUMPMT);
	title.Form("Double Hits (%): Module %d", mod);
	hdper->SetTitle(title);
	hdper->GetXaxis()->SetNdivisions(4);
	hdper->GetYaxis()->SetNdivisions(14,14,0,0);

	TH2D *hxdper = new TH2D("hxdper", "hxdper", 4,-2,2,NUMPADDLE*NUMPMT,0,NUMPADDLE*NUMPMT);
	title.Form("XTalk Double Hits (%): Module %d", mod);
	hxdper->SetTitle(title);
	hxdper->GetXaxis()->SetNdivisions(4);
	hxdper->GetYaxis()->SetNdivisions(14,14,0,0);

	hxper->SetMinimum(0);
	hxper->SetMaximum(100);
	hdper->SetMinimum(0);
	hdper->SetMaximum(100);
   	hxdper->SetMinimum(0);
	hxdper->SetMaximum(100);

	for(Int_t side=0;side<NUMSIDE;side++){
	  for(Int_t pmt=0;pmt<NUMPMT;pmt++){
	    ifstream tt; 
	    pmtn[side][pmt]=pmt;
	    tt.open(inthres[side][pmt],ios::in);  
	    if(tt.is_open()){
	      tt >> tx[side][pmt] >> runs[side][pmt][0] >> runs[side][pmt][1] >> runs[side][pmt][2] >> runs[side][pmt][3] >> runs[side][pmt][4];
	      tt >> px[side][pmt] >> pmtn[side][pmt] >> hv[side][pmt][0] >> hv[side][pmt][1] >> hv[side][pmt][2] >> hv[side][pmt][3] >> hv[side][pmt][4];
	      cout << px[side][pmt] << "\t" << pmtn[side][pmt] << endl;
	      cout << tx[side][pmt] << "\t" << runs[side][pmt][0] << "\t" << runs[side][pmt][1] << "\t" << runs[side][pmt][2] << "\t" << 
		                               runs[side][pmt][3] << "\t" << runs[side][pmt][4] << endl;
	      cout << "HV:" << "\t" << hv[side][pmt][0] << "\t" << hv[side][pmt][1] << "\t" << hv[side][pmt][2] << "\t" << 
		                       hv[side][pmt][3] << "\t" << hv[side][pmt][4] << endl;
	      for(Int_t i=0;i<NUMPIXEL;i++){
		if(i==16){break;}
		tt >> pixels[side][pmt][i] >> thr[side][pmt][0][i] >> thr[side][pmt][1][i] >> thr[side][pmt][2][i] >> thr[side][pmt][3][i] >> 
		                              thr[side][pmt][4][i];
		cout <<pixels[side][pmt][i]<<"\t"<<thr[side][pmt][0][i]<<"\t"<<thr[side][pmt][1][i]<<"\t"<<thr[side][pmt][2][i]<<"\t"<<
		                             thr[side][pmt][3][i]<<"\t"<<thr[side][pmt][4][i]<< endl;
		threshs[side][pmt][i] = thr[side][pmt][rn][i];
		if(i==15){break;}
	      }
	    }
	    tt.close();
	  }
	}

	cout << "Thresholds read." << endl;

	for(Int_t side=0;side<NUMSIDE;side++){
	  for(Int_t pmt=0;pmt<NUMPMT;pmt++){
	    if(pmt==14){break;}
	    ifstream tw;
	    tw.open(intwidth[side][pmt],ios::in);
	    string tln;   // using to check first line of input file
	    if(tw.is_open()){ getline(tw,tln); // get the first line of the file
	      if(!isdigit(tln[0]) ){
		cout << tln << endl; 
	      }
	      for(Int_t j=0;j<NUMPIXEL;j++){ 
		if(j==16){break;}
		tw >> twpx[side][pmt][j] >> tdcw[side][pmt][0][j] >> tdcw[side][pmt][1][j] >> tdcw[side][pmt][2][j] >> 
		                            tdcw[side][pmt][3][j] >> tdcw[side][pmt][4][j];
		cout<<twpx[side][pmt][j]<< "\t" <<tdcw[side][pmt][0][j]<< "\t" <<tdcw[side][pmt][1][j]<< "\t" << tdcw[side][pmt][2][j]<< "\t" <<
		                                  tdcw[side][pmt][3][j]<< "\t" <<tdcw[side][pmt][4][j]<< endl;
		twidths[side][pmt][j] = tdcw[side][pmt][rn][j];
		if(j==15){break;} 
	      }
	    } 
	    tw.close();
	    
	    if(pmt==13){
	      break;
	    }
	  }
	}

	cout<< "TDC Widths read." <<endl;


	// REMOVE or change
	cout << "Missing Pixels:" << endl;
	for(Int_t side=0;side<NUMSIDE;side++){
	  for(Int_t pmt=0; pmt< NUMPMT;pmt++){
	    for(Int_t i=0; i<NUMPIXEL; i++){
	      if(thr[side][pmt][4][i] == 0 && mp1[side][pmt] == 0){
		mp1[side][pmt] = i+1;
	      }
	      if(thr[side][pmt][4][i] == 0 && mp1[side][pmt]>0){
		mp2[side][pmt] = i+1;
	      }
	    }
	    cout << "PMT " << pmt+1 << "\t"  << mp1[side][pmt] << "\t"  << mp2[side][pmt] << endl;
	  }
	}

	// create the string for the root filenames
  
	TString fname[NUMSIDE][NUMPMT][NUMRUNS];
	TFile *f[NUMSIDE][NUMPMT][NUMRUNS];
	TTree *t[NUMSIDE][NUMPMT][NUMRUNS];
	Int_t numentries[NUMSIDE][NUMPMT][NUMRUNS];
	
	for(Int_t side=0;side<NUMSIDE;side++){
	  for (Int_t pmt = 0; pmt<NUMPMT; pmt++){
	    //cout << runs[pmt][rn] << endl;
	    fname[side][pmt][rn] = Form("../rootfiles/scint_%d.root",runs[side][pmt][rn]);
	    if(pmt==0 || fname[side][pmt][rn]!=fname[side][pmt-1][rn]){
	      f[side][pmt][rn] = TFile::Open(fname[side][pmt][rn],"OPEN");
	      cout << "Opening file " << fname[side][pmt][rn] << endl;
	      f[side][pmt][rn]->cd();
	      t[side][pmt][rn] = (TTree*)f[side][pmt][rn]->Get("T");
	      t[side][pmt][rn]->SetBranchAddress("C.cdetm1r.adc_c",&adc_c);
	      t[side][pmt][rn]->SetBranchAddress("C.cdetm1r.adc",&adc);
	      t[side][pmt][rn]->SetBranchAddress("C.cdetm1r.tdcl",&tdcl);
	      t[side][pmt][rn]->SetBranchAddress("C.cdetm1r.tdct",&tdct);
	      numentries[side][pmt][rn] = t[side][pmt][rn]->GetEntries();
	    }
	    else{
	      f[side][pmt][rn] = f[side][pmt-1][rn];
	      cout << "File " << fname[side][pmt][rn] << endl;
	      f[side][pmt][rn]->cd();
	      t[side][pmt][rn] = t[side][pmt-1][rn];
	      t[side][pmt][rn]->SetBranchAddress("C.cdetm1r.adc_c",&adc_c);
	      t[side][pmt][rn]->SetBranchAddress("C.cdetm1r.adc",&adc);
	      t[side][pmt][rn]->SetBranchAddress("C.cdetm1r.tdcl",&tdcl);
	      t[side][pmt][rn]->SetBranchAddress("C.cdetm1r.tdct",&tdct);
	      numentries[side][pmt][rn] = t[side][pmt][rn]->GetEntries();
	    }
	  }
	}
	
	cout << "Start tree looping." << endl;

	for(Int_t side=0;side<NUMSIDE;side++){
	  for(Int_t pmt=0; pmt < NUMPMT; pmt++){
	    for(Int_t id=1;id<=(numentries[side][pmt][rn]/div);id++){
	      t[side][pmt][rn]->GetEntry(id);
	      if(id % 100000 == 0){cout << id << endl;}
	      for(Int_t index = 1; index<=NUMPIXEL; index++){
		Int_t ipaddle = (pmt)*NUMPIXEL+index-1;
		Int_t rnum = rn;
		Int_t pxl = index-1;
		bool isdouble = false;
		Double_t tdiff = tdcl[ipaddle]-tdct[ipaddle];
		Double_t adc = adc_c[ipaddle];
		Double_t tdc = tdcl[ipaddle];
		if(tdc>500){hits[side][pmt][pxl]+=1;}
		for(Int_t j=1; j<=NUMPIXEL;j++){
		  Int_t ipad = (pmt)*NUMPIXEL+j-1;
		  if(tdc>500 && tdcl[ipad]>500 && ipad!=ipaddle){
		    isdouble = true;
		  }
		}
		if(isdouble){
		  dhits[side][pmt][pxl]+=1;
		}
		if(tdc>500 && adc>threshs[side][pmt][pxl] && tdiff<=twidths[side][pmt][pxl])
		  {
		    xhits[side][pmt][pxl]+=1;
		  }
	      }
	    }
	    cout << "Side " << side + 1 << " PMT " << pmt + 1 << " complete." << endl;
	  }
	}
	
	cout << "End tree looping." << endl;

	// Print Crosstalk Data
	cout << "CROSSTALK DATA" << endl;
	cout << "Side" << "\t" << "PMT" << "\t" << "PXL" << "\t" << "xHits" << endl;
	for(Int_t side=0;side<NUMSIDE;side++){
	  for(Int_t pmt=0; pmt < NUMPMT; pmt++){
	    for(Int_t pix=0; pix<NUMPIXEL; pix++){
	      if(pix != mp1[side][pmt]-1 && pix != mp2[side][pmt]-1){
	      cout << side+1 << "\t" << pmt+1 << "\t" << pix+1 << "\t" << xhits[side][pmt][pix] << endl;
	      }
	    }
	    cout << "------------------------------" << endl;
	  }
	}

	hxheat->SetStats(0);
	hxhits->SetStats(0);
	hdb_hits->SetStats(0);
	hxper->SetStats(0);
	hdper->SetStats(0);
	hxdper->SetStats(0);

	Double_t xhitadd, xperadd, xhitavg, xperavg, xhitmin, xhitmax, xpermin, xpermax, xhitdelta, xperdelta;
	xhitmin = 100000000;
	xhitmax = 0;
	xpermin = 100000000;
	xpermax = 0;
	xperadd = 0;

	//plot 3D histo
        Int_t icount=0;
	for(Int_t side=0;side<NUMSIDE;side++){
	  for (Int_t pmt=0; pmt<NUMPMT; pmt++){
	    if(side==0){Int_t paddle = pmt*NUMPADDLE;Int_t lr = -1;}
	    else if(side==1){Int_t paddle = (pmt+1)*NUMPADDLE;Int_t lr = 0;}
	    for (Int_t pix=0; pix<NUMPIXEL; pix++){
	      if(pix != mp1[side][pmt]-1 && pix != mp2[side][pmt]-1 && side==0){
		hxhits->Fill(lr,paddle,hits[side][pmt][pix]);
		hxheat->Fill(lr,paddle,xhits[side][pmt][pix]);
		hdb_hits->Fill(lr,paddle,dhits[side][pmt][pix]);
		hxper->Fill(lr,paddle,(xhits[side][pmt][pix]/hits[side][pmt][pix])*100);
		hdper->Fill(lr,paddle,(dhits[side][pmt][pix]/hits[side][pmt][pix])*100);
		hxdper->Fill(lr,paddle,(xhits[side][pmt][pix]/dhits[side][pmt][pix])*100);
		paddle++;
		xhitadd += xhits[side][pmt][pix];
		//xperadd += xhits[side][pmt][pix]/dhits[side][pmt][pix])*100
		Int_t sum = (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100;
		if(sum>0){xperadd += sum;}
	        if(xhitmin > xhits[side][pmt][pix] && xhits[side][pmt][pix] > 0.001){
		  xhitmin = xhits[side][pmt][pix];
		}
		if(xhitmax < xhits[side][pmt][pix]){
		  xhitmax = xhits[side][pmt][pix];
		}
		if(xpermin > (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100 
		   && (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100 > 0.001){
		  xpermin = (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100;
		}
		if(xpermax < (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100){
		  xpermax = (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100;
		}
	      }
	      else if(pix != mp1[side][pmt]-1 && pix != mp2[side][pmt]-1 && side==1){
		paddle--;
		hxhits->Fill(lr,paddle,hits[side][pmt][pix]);
		hxheat->Fill(lr,paddle,xhits[side][pmt][pix]);
		hdb_hits->Fill(lr,paddle,dhits[side][pmt][pix]);
		hxper->Fill(lr,paddle,(xhits[side][pmt][pix]/hits[side][pmt][pix])*100);
		hdper->Fill(lr,paddle,(dhits[side][pmt][pix]/hits[side][pmt][pix])*100);
		hxdper->Fill(lr,paddle,(xhits[side][pmt][pix]/dhits[side][pmt][pix])*100);
		xhitadd += xhits[side][pmt][pix];
		xperadd += (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100;
	        if(xhitmin > xhits[side][pmt][pix] && xhits[side][pmt][pix] > 0.001){
		  xhitmin = xhits[side][pmt][pix];
		}
		if(xhitmax < xhits[side][pmt][pix]){
		  xhitmax = xhits[side][pmt][pix];
		}
		if(xpermin > (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100 
		   && (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100 > 0.001 ){
		  xpermin = (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100;
		}
		if(xpermax < (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100){
		  xpermax = (xhits[side][pmt][pix]/dhits[side][pmt][pix])*100;
		}
	      }
	    }
	  }
	}

        xhitavg = xhitadd/392;
        xperavg = xperadd/392;
        xhitdelta = xhitmax-xhitmin;
        xperdelta = xpermax-xpermin;
	
	cout << "Mean" << "\t" << "Max" << "\t" << "Min" << "\t" << "Delta" << "\t" << "% Mean" << "\t" << "% Max" << "\t" << "% Min" << "\t" << "% Delta" << endl;
	cout << xhitavg << "\t"<<  xhitmax<< "\t"  << xhitmin<< "\t"  << xhitdelta<< "\t" << xperavg<< "\t" << xpermax << "\t" << xpermin<< "\t" << xperdelta << endl;

	cXMOD->SetName("cXMOD");
	cXMOD->Divide(3,2);
	cXMOD->cd(1);
	gPad->SetGrid();
	hxheat->Draw("COLZ");
	hxheat->GetXaxis()->SetTitle("");
	hxheat->GetYaxis()->SetTitle("PMT NUMBER x PADDLE NUMBER");
	cXMOD->cd(2);
	gPad->SetGrid();
	hxhits->Draw("COLZ");
	hxhits->GetXaxis()->SetTitle("");
	hxhits->GetYaxis()->SetTitle("PMT NUMBER x PADDLE NUMBER");
	cXMOD->cd(3);
	gPad->SetGrid();
	hdb_hits->Draw("COLZ");
	hdb_hits->GetXaxis()->SetTitle("");
	hdb_hits->GetYaxis()->SetTitle("PMT NUMBER x PADDLE NUMBER");
	cXMOD->cd(4);
	gPad->SetGrid();
	hxper->Draw("COLZ");
	hxper->GetXaxis()->SetTitle("");
	hxper->GetYaxis()->SetTitle("PMT NUMBER x PADDLE NUMBER");
	cXMOD->cd(5);
	gPad->SetGrid();
	hdper->Draw("COLZ");
	hdper->GetXaxis()->SetTitle("");
	hdper->GetYaxis()->SetTitle("PMT NUMBER x PADDLE NUMBER");
	cXMOD->cd(6);
	gPad->SetGrid();
	hxdper->Draw("COLZ");
	hxdper->GetXaxis()->SetTitle("");
	hxdper->GetYaxis()->SetTitle("PMT NUMBER x PADDLE NUMBER");

	title.Form("module_%d_xmod.pdf",mod);
	cXMOD->Print(title);

}

