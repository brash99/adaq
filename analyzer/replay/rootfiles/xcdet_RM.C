// 17th July 2024
//
// Script to make various crosstalk plots for all CDet.
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
static const Int_t NUMHMOD = 12;
static const Int_t NUMMOD = 6;
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
//Int_t pixel1[NUMPMT]={ 1, 2,13,4, 6,12, 4,1,1,5,13,1, 1, 8};
//Int_t pixel2[NUMPMT]={14,16,16,6,13,13,16,3,6,9,15,4,16,12};
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
Int_t pixel1[NUMPMT]={9,15,6, 4, 3, 1, 1, 6, 13, 7, 15, 4, 3, 1};
Int_t pixel2[NUMPMT]={13,16,13,7,13,16,16,13,16,13,16,13,13,16};

void xcdet_RM(Int_t layer=2,Int_t rn=4, Int_t div = 10){

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
	string tx[NUMMOD][NUMSIDE][NUMPMT],px[NUMMOD][NUMSIDE][NUMPMT];
	Int_t hv[NUMMOD][NUMSIDE][NUMPMT][NUMRUNS];
	Int_t runs[NUMMOD][NUMSIDE][NUMPMT][NUMRUNS];
	Int_t pmtn[NUMMOD][NUMSIDE][NUMPMT];
	Int_t mp1[NUMMOD][NUMSIDE][NUMPMT] = {0,1,2,3,4,5}{0,1}{0,0,0,0,0,0,0,0,0,0,0,0,0,0};   // missing pixel numbers
	Int_t mp2[NUMMOD][NUMSIDE][NUMPMT] = {0,1,2,3,4,5}{0,1}{0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
	Int_t* pixels = new Int_t[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL];
        title.Form("XTalk_CDET");
        TCanvas *cXMOD = new TCanvas("cXMOD","cXMOD",2000,1000);
	cXMOD->SetGrid();

	Double_t xhits[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL] = new Double_t[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL];
	Double_t hits[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL] = new Double_t[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL];
	Double_t dhits[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL] = new Double_t[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL];

	Double_t thr[NUMMOD][NUMSIDE][NUMPMT][NUMRUNS][NUMPIXEL] = new Double_t[NUMMOD][NUMSIDE][NUMPMT][NUMRUNS][NUMPIXEL];
	Double_t threshs[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL] = new Double_t[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL];
	Int_t tdcw[NUMMOD][NUMSIDE][NUMPMT][NUMRUNS][NUMPIXEL] = new Int_t[NUMMOD][NUMSIDE][NUMPMT][NUMRUNS][NUMPIXEL];
	Int_t twpx[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL] = new Int_t[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL]; 
	Int_t twidths[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL] = new Int_t[NUMMOD][NUMSIDE][NUMPMT][NUMPIXEL];

	TString inthres[NUMMOD][NUMSIDE][NUMPMT];
	TString intwidth[NUMMOD][NUMSIDE][NUMPMT];

	Int_t startmod, endmod;

	if(layer==1){startmod=0;endmod=3;}
	else if(layer==2){startmod=3;endmod=0;}
	else{startmod=0;endmod=0;}

	for(Int_t mod=0+startmod; mod<NUMMOD-endmod;mod++){
	  for(Int_t side=0; side<NUMSIDE;side++){
	    for(Int_t pmt=1; pmt<=NUMPMT; pmt++){
	      if(side==0){
		inthres[mod][side][pmt-1] = Form("M%dL_threshold_pmt%d.dat",mod+1,pmt);  // threshold data
		cout << inthres[mod][side][pmt-1] << endl;
		intwidth[mod][side][pmt-1] = Form("M%dL_tdcwidth_pmt%d.dat",mod+1,pmt);  // tdc width cut data
		cout << intwidth[mod][side][pmt-1] << endl;
	      }
	      else if(side==1){
		inthres[mod][side][pmt-1] = Form("M%dR_threshold_pmt%d.dat",mod+1,pmt);  // threshold data
		cout << inthres[mod][side][pmt-1] << endl;
		intwidth[mod][side][pmt-1] = Form("M%dR_tdcwidth_pmt%d.dat",mod+1,pmt);  // tdc width cut data
		cout << intwidth[mod][side][pmt-1] << endl;
	      }
	    }	
	  }
	}
	
        TString tmpentry;
	tmpentry.Form("hxheat");
        gStyle->SetStatX(0.9);
        gStyle->SetStatY(0.6);
        gStyle->SetStatW(0.4);

	TH2D *hxheat = new TH2D("hxheat","hxheat",10,-5,5,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE),0,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE));
        title.Form("XTalk: CDet Layer %d",layer);
        hxheat->SetTitle(title);
	hxheat->GetXaxis()->SetNdivisions(2,5);
	hxheat->GetYaxis()->SetNdivisions(3,14,0,0);

	TH2D *hxhits = new TH2D("hxhits", "hxhits", 10,-5,5,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE),0,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE));
	title.Form("Total Hits: CDet Layer %d",layer);
	hxhits->SetTitle(title);
	hxhits->GetXaxis()->SetNdivisions(2,5);
	hxhits->GetYaxis()->SetNdivisions(3,14,0,0);
	
	TH2D *hdb_hits = new TH2D("hdhits", "hdhits", 10,-5,5,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE),0,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE));
	title.Form("Double Hits: CDet Layer %d", layer);
	hdb_hits->SetTitle(title);
	hdb_hits->GetXaxis()->SetNdivisions(2,5);
	hdb_hits->GetYaxis()->SetNdivisions(3,14,0,0);

        TH2D *hxper = new TH2D("hxper","hxper",10,-5,5,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE),0,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE));
        title.Form("XTalk (%): CDet Layer %d", layer);
        hxper->SetTitle(title);
	hxper->GetXaxis()->SetNdivisions(2,5);
	hxper->GetYaxis()->SetNdivisions(3,14,0,0);

	TH2D *hdper = new TH2D("hdper", "hdper", 10,-5,5,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE),0,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE));
	title.Form("Double Hits (%): CDet Layer %d", layer);
	hdper->SetTitle(title);
	hdper->GetXaxis()->SetNdivisions(2,5);
	hdper->GetYaxis()->SetNdivisions(3,14,0,0);

	TH2D *hxdper = new TH2D("hxdper", "hxdper", 10,-5,5,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE),0,NUMPADDLE*NUMPMT*(NUMMOD/NUMSIDE));
	title.Form("XTalk Double Hits (%): CDet Layer %d", layer);
	hxdper->SetTitle(title);
	hxdper->GetXaxis()->SetNdivisions(2,5);
	hxdper->GetYaxis()->SetNdivisions(3,14,0,0);

	hxper->SetMinimum(0);
	hxper->SetMaximum(100);
	hdper->SetMinimum(0);
	hdper->SetMaximum(100);
   	hxdper->SetMinimum(0);
	hxdper->SetMaximum(100);

	for(Int_t mod=0+startmod; mod<NUMMOD-endmod;mod++){
	  for(Int_t side=0;side<NUMSIDE;side++){
	    for(Int_t pmt=0;pmt<NUMPMT;pmt++){
	      ifstream tt; 
	      pmtn[mod][side][pmt]=pmt;
	      tt.open(inthres[mod][side][pmt],ios::in);  
	      if(tt.is_open()){
		tt >> tx[mod][side][pmt] >> runs[mod][side][pmt][0] >> runs[mod][side][pmt][1] >> runs[mod][side][pmt][2] >> runs[mod][side][pmt][3] >> 
		                            runs[mod][side][pmt][4];
		tt >> px[mod][side][pmt] >> pmtn[mod][side][pmt] >> hv[mod][side][pmt][0] >> hv[mod][side][pmt][1] >> hv[mod][side][pmt][2] >> 
		                                                    hv[mod][side][pmt][3] >> hv[mod][side][pmt][4];
		//cout << px[mod][side][pmt] << "\t" << pmtn[mod][side][pmt] << endl;
		//cout << tx[mod][side][pmt] << "\t" << runs[mod][side][pmt][0] << "\t" << runs[mod][side][pmt][1] << "\t" << runs[mod][side][pmt][2] << "\t" << 
		//runs[mod][side][pmt][3] << "\t" << runs[mod][side][pmt][4] << endl;
		//cout << "HV:" << "\t" << hv[mod][side][pmt][0] << "\t" << hv[mod][side][pmt][1] << "\t" << hv[mod][side][pmt][2] << "\t" << 
		//hv[mod][side][pmt][3] << "\t" << hv[mod][side][pmt][4] << endl;
		for(Int_t i=0;i<NUMPIXEL;i++){
		  if(i==16){break;}
		  tt >> pixels[mod][side][pmt][i] >> thr[mod][side][pmt][0][i] >> thr[mod][side][pmt][1][i] >> thr[mod][side][pmt][2][i] >> 
		                                     thr[mod][side][pmt][3][i] >> thr[mod][side][pmt][4][i];
		  //cout <<pixels[mod][side][pmt][i]<<"\t"<<thr[mod][side][pmt][0][i]<<"\t"<<thr[mod][side][pmt][1][i]<<"\t"<<thr[mod][side][pmt][2][i]<<"\t"<<
		  //thr[mod][side][pmt][3][i]<<"\t"<<thr[mod][side][pmt][4][i]<< endl;
		  threshs[mod][side][pmt][i] = thr[mod][side][pmt][rn][i];
		  if(i==15){break;}
		}
	      }
	      tt.close();
	    }
	  }
	}

	cout << "Thresholds read." << endl;

	for(Int_t mod=0+startmod; mod<NUMMOD-endmod;mod++){
	  for(Int_t side=0;side<NUMSIDE;side++){
	    for(Int_t pmt=0;pmt<NUMPMT;pmt++){
	      if(pmt==14){break;}
	      ifstream tw;
	      tw.open(intwidth[mod][side][pmt],ios::in);
	      string tln;   // using to check first line of input file
	      if(tw.is_open()){ 
		getline(tw,tln); // get the first line of the file
		//if(!isdigit(tln[0]) ){
		  //cout << tln << endl; 
		//}
		for(Int_t j=0;j<NUMPIXEL;j++){ 
		  if(j==16){break;}
		  tw >> twpx[mod][side][pmt][j] >> tdcw[mod][side][pmt][0][j] >> tdcw[mod][side][pmt][1][j] >> tdcw[mod][side][pmt][2][j] >> 
		    tdcw[mod][side][pmt][3][j] >> tdcw[mod][side][pmt][4][j];
		  //cout<<twpx[mod][side][pmt][j]<< "\t" <<tdcw[mod][side][pmt][0][j]<< "\t" <<tdcw[mod][side][pmt][1][j]<< "\t" << tdcw[mod][side][pmt][2][j]<< "\t" <<
		  //tdcw[mod][side][pmt][3][j]<< "\t" <<tdcw[mod][side][pmt][4][j]<< endl;
		  twidths[mod][side][pmt][j] = tdcw[mod][side][pmt][rn][j];
		  if(j==15){break;} 
		}
	      } 
	      tw.close();
	      if(pmt==13){
		break;
	      }
	    }
	  }
	}

	cout<< "TDC Widths read." <<endl;

	// Get missing pixels.
	for(Int_t mod=0+startmod; mod<NUMMOD-endmod;mod++){
	  for(Int_t side=0;side<NUMSIDE;side++){
	    for(Int_t pmt=0; pmt< NUMPMT;pmt++){
	      for(Int_t i=0; i<NUMPIXEL; i++){
		if(thr[mod][side][pmt][4][i] == 0 && mp1[mod][side][pmt] == 0){
		  mp1[mod][side][pmt] = i+1;
		}
		if(thr[mod][side][pmt][4][i] == 0 && mp1[mod][side][pmt]>0){
		  mp2[mod][side][pmt] = i+1;
		}
	      }
	      //cout << "PMT " << pmt+1 << "\t"  << mp1[mod][side][pmt] << "\t"  << mp2[mod][side][pmt] << endl;
	    }
	  }
	}

	cout << "Missing pixels read." << endl;

	// create the string for the root filenames
  
	TString fname[NUMMOD][NUMSIDE][NUMPMT][NUMRUNS];
	TFile *f[NUMMOD][NUMSIDE][NUMPMT][NUMRUNS];
	TTree *t[NUMMOD][NUMSIDE][NUMPMT][NUMRUNS];
	Int_t numentries[NUMMOD][NUMSIDE][NUMPMT][NUMRUNS];
	
	for(Int_t mod=0+startmod; mod<NUMMOD-endmod;mod++){
	  for(Int_t side=0;side<NUMSIDE;side++){
	    for (Int_t pmt = 0; pmt<NUMPMT; pmt++){
	      //cout << runs[pmt][rn] << endl;
	      fname[mod][side][pmt][rn] = Form("../rootfiles/scint_%d.root",runs[mod][side][pmt][rn]);
	      if(pmt==0 || fname[mod][side][pmt][rn]!=fname[mod][side][pmt-1][rn]){
		f[mod][side][pmt][rn] = TFile::Open(fname[mod][side][pmt][rn],"OPEN");
		cout << "Opening file " << fname[mod][side][pmt][rn] << endl;
		f[mod][side][pmt][rn]->cd();
		t[mod][side][pmt][rn] = (TTree*)f[mod][side][pmt][rn]->Get("T");
		t[mod][side][pmt][rn]->SetBranchAddress("C.cdetm1r.adc_c",&adc_c);
		t[mod][side][pmt][rn]->SetBranchAddress("C.cdetm1r.adc",&adc);
		t[mod][side][pmt][rn]->SetBranchAddress("C.cdetm1r.tdcl",&tdcl);
		t[mod][side][pmt][rn]->SetBranchAddress("C.cdetm1r.tdct",&tdct);
		numentries[mod][side][pmt][rn] = t[mod][side][pmt][rn]->GetEntries();
	      }
	      else{
		f[mod][side][pmt][rn] = f[mod][side][pmt-1][rn];
		//cout << "File " << fname[mod][side][pmt][rn] << endl;
		f[mod][side][pmt][rn]->cd();
		t[mod][side][pmt][rn] = t[mod][side][pmt-1][rn];
		t[mod][side][pmt][rn]->SetBranchAddress("C.cdetm1r.adc_c",&adc_c);
		t[mod][side][pmt][rn]->SetBranchAddress("C.cdetm1r.adc",&adc);
		t[mod][side][pmt][rn]->SetBranchAddress("C.cdetm1r.tdcl",&tdcl);
		t[mod][side][pmt][rn]->SetBranchAddress("C.cdetm1r.tdct",&tdct);
		numentries[mod][side][pmt][rn] = t[mod][side][pmt][rn]->GetEntries();
	      }
	      cout << "MOD " << mod+1 << " Side " << side << " PMT " << pmt << endl;
	    }
	  }
	}

	cout << "Start tree looping." << endl;

	for(Int_t mod=0+startmod; mod<NUMMOD-endmod;mod++){
	  for(Int_t side=0;side<NUMSIDE;side++){
	    for(Int_t pmt=0; pmt < NUMPMT; pmt++){
	      for(Int_t id=1;id<=(numentries[mod][side][pmt][rn]/div);id++){
		t[mod][side][pmt][rn]->GetEntry(id);
		if(id % (100000/div) == 0){cout << id << endl;}
		for(Int_t index = 1; index<=NUMPIXEL; index++){
		  Int_t ipaddle = (pmt)*NUMPIXEL+index-1;
		  Int_t rnum = rn;
		  Int_t pxl = index-1;
		  bool isdouble = false;
		  Double_t tdiff = tdcl[ipaddle]-tdct[ipaddle];
		  Double_t adc = adc_c[ipaddle];
		  Double_t tdc = tdcl[ipaddle];
		  if(tdc>500){hits[mod][side][pmt][pxl]+=1;}
		  for(Int_t j=1; j<=NUMPIXEL;j++){
		    Int_t ipad = (pmt)*NUMPIXEL+j-1;
		    if(tdc>500 && tdcl[ipad]>500 && ipad!=ipaddle){
		      isdouble = true;
		    }
		  }
		  if(isdouble){
		    dhits[mod][side][pmt][pxl]+=1;
		  }
		  if(tdc>500 && adc>threshs[mod][side][pmt][pxl] && tdiff<=twidths[mod][side][pmt][pxl])
		    {
		      xhits[mod][side][pmt][pxl]+=1;
		    }
		}
	      }
	    }
	  }
	}
	
	cout << "End tree looping." << endl;

	hxheat->SetStats(0);
	hxhits->SetStats(0);
	hdb_hits->SetStats(0);
	hxper->SetStats(0);
	hdper->SetStats(0);
	hxdper->SetStats(0);
	
	Int_t startpaddle[NUMMOD];
	Int_t bar_pos[NUMMOD][NUMSIDE][NUMPMT];

	for(Int_t mod=1+startmod;mod<=NUMMOD-endmod;mod++){
	  if(mod==2 || mod==5){startpaddle[mod-1]=NUMPMT*NUMPADDLE-1;}
	  if(mod==1 || mod==4){startpaddle[mod-1]=NUMPMT*NUMPADDLE*2-1;}
	  if(mod==3 || mod==6){startpaddle[mod-1]=NUMPMT*NUMPADDLE*3-1;}
	  for (Int_t pmt=0; pmt<NUMPMT; pmt++){
	    if(mod==2){
	      if(pmt<NUMPMT/2){bar_pos[mod-1][0][pmt]=-2;bar_pos[mod-1][1][pmt]=-3;}
	      if(pmt>=NUMPMT/2){bar_pos[mod-1][0][pmt]=-1;bar_pos[mod-1][1][pmt]=-2;}
	    }
	    if(mod==5){
	      if(pmt<NUMPMT/2){bar_pos[mod-1][0][pmt]=2;bar_pos[mod-1][1][pmt]=1;}
	      if(pmt>=NUMPMT/2){bar_pos[mod-1][0][pmt]=3;bar_pos[mod-1][1][pmt]=2;}
	    }
	    if(mod==1){
	      bar_pos[mod-1][0][pmt]=-3;bar_pos[mod-1][1][pmt]=-4;
	    }
	    if(mod==4){
	      bar_pos[mod-1][0][pmt]=1;bar_pos[mod-1][1][pmt]=0;
	    }
	    if(mod==3){
	      if(pmt<NUMPMT/2){bar_pos[mod-1][0][pmt]=-1;bar_pos[mod-1][1][pmt]=-2;}
	      if(pmt>=NUMPMT/2){bar_pos[mod-1][0][pmt]=-2;bar_pos[mod-1][1][pmt]=-3;}
	    }
	    if(mod==6){
	      if(pmt<NUMPMT/2){bar_pos[mod-1][0][pmt]=3;bar_pos[mod-1][1][pmt]=2;}
	      if(pmt>=NUMPMT/2){bar_pos[mod-1][0][pmt]=2;bar_pos[mod-1][1][pmt]=1;}
	    }
	  }
	}

	//plot 3D histos
	for(Int_t mod=1+startmod; mod<=NUMMOD-endmod;mod++){
	  for(Int_t side=0;side<NUMSIDE;side++){
	    for (Int_t pmt=0; pmt<NUMPMT; pmt++){
	      if(side==0){Int_t paddle = startpaddle[mod-1]-(pmt*NUMPADDLE);}
	      else if(side==1){Int_t paddle = startpaddle[mod-1]-((pmt+1)*NUMPADDLE);}
	      for (Int_t pix=0; pix<NUMPIXEL; pix++){
		//cout<< paddle << endl;
		if(pix != mp1[mod-1][side][pmt]-1 && pix != mp2[mod-1][side][pmt]-1 && side==0){
		  hxhits->Fill(bar_pos[mod-1][side][pmt],paddle,hits[mod-1][side][pmt][pix]);
		  hxheat->Fill(bar_pos[mod-1][side][pmt],paddle,xhits[mod-1][side][pmt][pix]);
		  hdb_hits->Fill(bar_pos[mod-1][side][pmt],paddle,dhits[mod-1][side][pmt][pix]);
		  hxper->Fill(bar_pos[mod-1][side][pmt],paddle,(xhits[mod-1][side][pmt][pix]/hits[mod-1][side][pmt][pix])*100);
		  hdper->Fill(bar_pos[mod-1][side][pmt],paddle,(dhits[mod-1][side][pmt][pix]/hits[mod-1][side][pmt][pix])*100);
		  hxdper->Fill(bar_pos[mod-1][side][pmt],paddle,(xhits[mod-1][side][pmt][pix]/dhits[mod-1][side][pmt][pix])*100);
		  paddle--;
		}
		else if(pix != mp1[mod-1][side][pmt]-1 && pix != mp2[mod-1][side][pmt]-1 && side==1){
		  paddle++;
		  hxhits->Fill(bar_pos[mod-1][side][pmt],paddle,hits[mod-1][side][pmt][pix]);
		  hxheat->Fill(bar_pos[mod-1][side][pmt],paddle,xhits[mod-1][side][pmt][pix]);
		  hdb_hits->Fill(bar_pos[mod-1][side][pmt],paddle,dhits[mod-1][side][pmt][pix]);
		  hxper->Fill(bar_pos[mod-1][side][pmt],paddle,(xhits[mod-1][side][pmt][pix]/hits[mod-1][side][pmt][pix])*100);
		  hdper->Fill(bar_pos[mod-1][side][pmt],paddle,(dhits[mod-1][side][pmt][pix]/hits[mod-1][side][pmt][pix])*100);
		  hxdper->Fill(bar_pos[mod-1][side][pmt],paddle,(xhits[mod-1][side][pmt][pix]/dhits[mod-1][side][pmt][pix])*100);
		}
	      }
	    }
	  }
	}

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

	title.Form("layer_%d_xcdet.pdf",layer);
	cXMOD->Print(title);

}
