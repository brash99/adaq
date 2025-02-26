  // 25th May 2018
  //
  // Script to make a comparison plot of the efficiency ratio
  // values as a function of high voltage settings for PMT 7 
  // between runs 1317 - 1322 with resistors installed.
  // The threshold numbers for each setting have to be read in.


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

using namespace std;

void effplot()
{

  // Set some of the styles first
  
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetTitleSize(0.06,"y");
  gStyle->SetTitleOffset(1.5,"a");
  gStyle->SetTitleOffset(0.9,"x");
  //  gStyle->SetTitleOffset(1.7,"y");
  gStyle->SetPadColor(10);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(10);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetOptTitle(1);
  gStyle->SetStatColor(10); 
  gStyle->SetStatH(0.16);
  gStyle->SetStatW(0.16);
  gStyle->UseCurrentStyle();

  // ------------------------------------------------

  // Read in the data from a text file.
  // First line are run numbers corresponding to runs at 
  // HV settings of 700, 725, 750, 775 and 800 V respectively

  // define arrays for threshold values from each run.
  Float_t* thr1 = new Float_t[16];     // 700 V
  Float_t* thr2 = new Float_t[16];     // 725 V
  Float_t* thr3 = new Float_t[16];     // 750 V
  Float_t* thr4 = new Float_t[16];     // 775 V
  Float_t* thr5 = new Float_t[16];     // 800 V

  Int_t* pixel = new Int_t[16];   // pixel number in PMT

  Int_t pmt = 0;   // tag for PMT number
  Int_t rn1, rn2, rn3, rn4, rn5;  // run numbers for root files
  Int_t hv1, hv2, hv3, hv4, hv5;  // HV settings

  Int_t mp1 = 0;   // missing pixel numbers
  Int_t mp2 = 0; 

  Float_t* bf = new Float_t[16];   // counts before threshold cut
  Float_t* af = new Float_t[16];   // counts after threshold cut

  Float_t* eff = new Float_t[16];  // efficiency numbers
  
  string tx,px;

  // array of TDC width cuts for 800 V only
  Float_t tdcw[16] = {31, 32, 34, 0, 34, 32, 32, 35, 0, 32, 34, 34, 30, 31, 33, 0};

  ifstream tt; 
  tt.open("testtext.txt",ios::in);  // read in the input file of values


  // if(!tt.good()){
  //   cout << "Problem with input file" << endl;
  //   break;
  // }

  tt >> tx >> rn1 >> rn2 >> rn3 >> rn4 >> rn5;

  tt >> px >> pmt >> hv1 >> hv2 >> hv3 >> hv4 >> hv5;

  cout << tx << rn1 << rn2 << rn3 << rn4 << rn5 << endl;
  cout << px << "\t" << pmt << endl;
  cout << hv1 << hv2 << hv3 << hv4 << hv5 << endl;

  // now read in the threshold values into the arrays

   //  while(tt.good()){
    
  for(Int_t i=0; i<16; i++){
    tt >> pixel[i] >> thr1[i] >> thr2[i] >> thr3[i] >> thr4[i] >> thr5[i];
    
    if(thr5[i] == 0 && mp1 == 0){
      mp1 = i+1;
      cout << i << "\t" << mp1 << "\t" << thr5[i] << endl;
    }

    if(thr5[i] == 0 && mp1>0){
      mp2 = i+1;
      cout << i << "\t" << mp2 << "\t" << thr5[i] << endl;
    }
 
    cout << i << "\t" << pixel[i] << "\t" << thr5[i] << "\t" << hv5 << endl;
    
  }

  cout << "missing pixels are: " << mp1 << "\t" << mp2 << endl;
    
    //  }

  // create the string for the root filenames
  
  TString fname1 = Form("../rootfiles/scint_%d.root",rn1);
  TString fname2 = Form("../rootfiles/scint_%d.root",rn2);
  TString fname3 = Form("../rootfiles/scint_%d.root",rn3);
  TString fname4 = Form("../rootfiles/scint_%d.root",rn4);
  TString fname5 = Form("../rootfiles/scint_%d.root",rn5);

  cout << fname1 << "\t" << fname4 << endl;

  // Now open the root files

  TFile *f1 = TFile::Open(fname1,"OPEN");
  TFile *f2 = TFile::Open(fname2,"OPEN");
  TFile *f3 = TFile::Open(fname3,"OPEN");
  TFile *f4 = TFile::Open(fname4,"OPEN");
  TFile *f5 = TFile::Open(fname5,"OPEN");

  // Figure out the root indices for all the pixels

  Int_t pc = ((pmt-1)*16);

  cout << "pixel 1 has index = " << pc << endl;

  // Get access to the tree in each root file

  f1->cd();
  TTree *t1 = (TTree*)f1->Get("T");

  f2->cd();
  TTree *t2 = (TTree*)f2->Get("T");

  f3->cd();
  TTree *t3 = (TTree*)f3->Get("T");

  f4->cd();
  TTree *t4 = (TTree*)f4->Get("T");

  f5->cd();
  TTree *t5 = (TTree*)f5->Get("T");

  // Create all fourteen canvases and all the histograms now

  TCanvas *sc[15];

  TH1F *hh[15];   // adc spectrum with TDC & ADC neighbors cut
  TH1F *ha[15];   // adc with TDC crosstalk cut added to previous cuts
  TH1F *hb[15];   // adc with TDC width cut included also
  TH1F *hc[15];   // adc with threshold cut added to other cuts

  ifstream outf;  // output file
  outf.open("efficiency.txt",ios::out);

  outf << "Pixel \t Efficiency" << endl;

  for(Int_t i=0; i<6; i++){

    if(i != mp1-1 && i != mp2-1){

      //  Int_t paddle = i+1


    TCut tdccutpx = Form("C.cdetm1r.tdcl[%d]>500",pc+i);
    TCut tdccutlw = Form("C.cdetm1r.tdcl[%d]<500",pc+i-1);
    TCut tdccutup = Form("C.cdetm1r.tdcl[%d]<500",pc+i+1);

    TCut adccutlw = Form("C.cdetm1r.adc_c[%d]<20",pc+i-1);
    TCut adccutup = Form("C.cdetm1r.adc_c[%d]<20",pc+i+1);

    cout << i <<  "\t pixel = " << pc+i << "\t cut indices: " << pc+i-1 << "  " << pc+i+1 << endl;

    cout << "lower ADC cut: " << adccutlw << endl;

    if(i+1 == mp1-1 || i+1 == mp2-1){
      TCut tdccutup = Form("C.cdetm1r.tdcl[%d]<500",pc+i+2);
      TCut adccutup = Form("C.cdetm1r.adc_c[%d]<20",pc+i+2);
      
      cout << "upper: " << adccutup << endl;
    }

    if(i-1 == mp1-1 || i-1 == mp2-1){
      TCut tdccutlw = Form("C.cdetm1r.tdcl[%d]<500",pc+i-2);
      TCut adccutlw = Form("C.cdetm1r.adc_c[%d]<20",pc+i-2);

      cout << "lower: " << adccutlw << endl;
    }

    TCut tcxcut, ttcut;

    for(Int_t j=0; j<16; j++){

      if(j != i ){
	ttcut = Form("C.cdetm1r.tdcl[%d]<500",pc+j);
	tcxcut += ttcut;

	//	cout << j << "\t" << tcxcut << endl;
      }

      //      cout << tcxcut << endl;
    }

    TCut wdtdc = Form("(C.cdetm1r.tdcl[%d]-C.cdetm1r.tdct[%d]) > %f",pc+i,pc+i,tdcw[i]);

    cout << wdtdc << endl;

    TCut cutthrs = Form("C.cdetm1r.adc_c[%d]>%f",pc+i,thr5[i]);

    cout << cutthrs << endl;

    TString cname = Form("sc%d",i+1);
    TString ctitle = Form("pixel %d",i+1);
    sc[i+1] = new TCanvas(cname,ctitle,1200,700);
    sc[i+1]->Divide(5,2);
    sc[i+1]->Draw();

    TString hhname = Form("hh[%d]",i);
    TString hhtitle = Form("Pixel %d at 700 V",i+1);
    hh[i] = new TH1F(hhname,hhtitle,100,-100,400);

    TString haname = Form("ha[%d]",i);
    ha[i] = new TH1F(haname,"",100,-100,400);
    ha[i]->SetLineColor(2);

    TString hbname = Form("hb[%d]",i);
    hb[i] = new TH1F(hbname,"",100,-100,400);
    hb[i]->SetLineColor(3);

    TString hcname = Form("hc[%d]",i);
    hc[i] = new TH1F(hcname,"",100,-100,400);
    hc[i]->SetLineColor(3);
    hc[i]->SetFillColor(3);

    TString adc = Form("C.cdetm1r.adc_c[%d] >> hh[%d]",pc+i,i);
    TString adccx = Form("C.cdetm1r.adc_c[%d] >> ha[%d]",pc+i,i);
    TString adcwd = Form("C.cdetm1r.adc_c[%d] >> hb[%d]",pc+i,i);
    TString adcthr = Form("C.cdetm1r.adc_c[%d] >> hc[%d]",pc+i,i);
    
    sc[i+1]->cd(5);

    t5->Draw(adc,tdccutpx && tdccutlw && tdccutup && adccutlw && adccutup);

    t5->Draw(adccx,tdccutpx && tdccutlw && tdccutup && adccutlw && adccutup && tcxcut,"same");

    t5->Draw(adcwd,tdccutpx && tdccutlw && tdccutup && adccutlw && adccutup && wdtdc && tcxcut,"same");

    t5->Draw(adcthr,tdccutpx && tdccutlw && tdccutup && adccutlw && adccutup && wdtdc && tcxcut && cutthrs,"same");
    
    bf[i] = hb[i]->GetEntries();
    af[i] = hc[i]->GetEntries();

    eff[i] = af[i]/bf[i];

    cout << "# events before = " << bf[i] << endl;
    cout << "# events after = " << af[i] << endl;
    cout << " efficiency = " << eff[i] << endl;

    // write the efficiency values to an output file

    outf << i+1 << "\t" << eff[i] << endl;

    }
  }  // end of 'i' for loop

  // Create all the cuts for each pixel and make the adc plots

  cout << "end of script here" << endl;
  /*
  // going to create the plots for each pixel in turn

  TCanvas *s1 = new TCanvas("s1","pixel 1 - 700 V",700,900);
  s1->cd();
  s1->Divide(1,2);

  s1->cd(1);

  // t1->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96]","C.cdetm1r.tdcl[96]>500");
  // tg->SetMarkerStyle(20);
  // tg->SetMarkerSize(0.7);

  TCut tdccut = Form("C.cdetm1r.tdcl[%d]>500 && C.cdetm1r.tdcl[%d]<500",pc,pc-1);

  TCut adccut = Form("C.cdetm1r.adc_c[%d]<20 && C.cdetm1r.adc_c[%d]<20",pc-1, pc+1);

  // cout << adccut << "\n" << endl;
  // cout << tdccut << "\n" << endl;
  
  TCut totalcut = adccut && tdccut;

  // cout << totalcut << endl;

  
  //  cout << thrscut << endl;

  // // s1->Update();

  // s1->cd(2);

  //  t1->Draw("C.cdetm1r.adc_c[96]",adccut);
  TH1F *hh = new TH1F("hh","ADC PMT 7, pixel 1;ADC channels; Counts",100,-100,400);
  TH1F *ha = new TH1F("ha","",100,-100,400);
  ha->SetLineColor(2);
  TH1F *hb = new TH1F("hb","",100,-100,400);
  hb->SetLineColor(3);

  t1->Draw("C.cdetm1r.adc_c[96] >> hh",totalcut);
  //  hh->SetName("aa");
  // apply the tdc width cut

  t1->SetLineColor(2);
  t1->Draw("C.cdetm1r.adc_c[96] >> ha",totalcut*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");
  cout << ha->GetEntries() << endl;

  t1->SetLineColor(3);
  t1->Draw("C.cdetm1r.adc_c[96] >> hb",(totalcut && thrscut1)*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");  // width, crosstalk and threshold cut
  cout << hb->GetEntries() << endl;
  //  htemp->SetName("bb"):
  
  // Float_t eff = 0.0;
  // cout << eff = hb->GetEntries()/ha->GetEntries() << endl;

  // Now plot the leading versus trailing tdc
  s1->cd(2);
  t1->SetMarkerStyle(21);
  t1->SetMarkerSize(0.7);
  t1->SetMarkerColor(1);

  t1->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96] >> h1",totalcut);

  t1->SetMarkerColor(2);
  t1->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96]",totalcut*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");
  

  t1->SetMarkerColor(3);
  t1->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96]",(totalcut && thrscut1)*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");


  //------------------------------------------------------------------

  // Now repeat only for this pixel but for the other HV settings
  // HV = 725 V

  TCanvas *s2 = new TCanvas("s2","pixel 1 - 725 V",700,900);
  s2->cd();
  s2->Divide(1,2);

  s2->cd(1);



  //  t1->Draw("C.cdetm1r.adc_c[96]",adccut);
  TH1F *jh = new TH1F("jh","ADC PMT 7, pixel 1;ADC channels; Counts",100,-100,400);
  TH1F *ja = new TH1F("ja","",100,-100,400);
  ja->SetLineColor(2);
  TH1F *jb = new TH1F("jb","",100,-100,400);
  jb->SetLineColor(3);

  t2->Draw("C.cdetm1r.adc_c[96] >> jh",totalcut);
  //  hh->SetName("aa");
  // apply the tdc width cut

  t2->SetLineColor(2);
  t2->Draw("C.cdetm1r.adc_c[96] >> ja",totalcut*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");
  cout << ja->GetEntries() << endl;

  t2->SetLineColor(3);
  t2->Draw("C.cdetm1r.adc_c[96] >> jb",(totalcut && thrscut2)*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");  // width, crosstalk and threshold cut
  cout << jb->GetEntries() << endl;

  // Now plot the leading versus trailing tdc
  s2->cd(2);
  t2->SetMarkerStyle(21);
  t2->SetMarkerSize(0.7);
  t2->SetMarkerColor(1);

  t2->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96] >> h1",totalcut);

  t2->SetMarkerColor(2);
  t2->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96]",totalcut*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");
  

  t2->SetMarkerColor(3);
  t2->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96]",(totalcut && thrscut2)*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");

  
  //--------------------------------------------------------------
  // HV = 750 V

  TCanvas *s3 = new TCanvas("s3","pixel 1 - 750 V",700,900);
  s3->cd();
  s3->Divide(1,2);

  s3->cd(1);


  TH1F *kh = new TH1F("kh","ADC PMT 7, pixel 1;ADC channels; Counts",100,-100,400);
  TH1F *ka = new TH1F("ka","",100,-100,400);
  ka->SetLineColor(2);
  TH1F *kb = new TH1F("kb","",100,-100,400);
  kb->SetLineColor(3);

  t3->Draw("C.cdetm1r.adc_c[96] >> kh",totalcut);
  //  hh->SetName("aa");
  // apply the tdc width cut

  t3->SetLineColor(2);
  t3->Draw("C.cdetm1r.adc_c[96] >> ka",totalcut*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");
  cout << ka->GetEntries() << endl;

  t3->SetLineColor(3);
  t3->Draw("C.cdetm1r.adc_c[96] >> kb",(totalcut && thrscut3)*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");  // width, crosstalk and threshold cut
  cout << kb->GetEntries() << endl;

  s3->cd(2);
  t3->SetMarkerStyle(21);
  t3->SetMarkerSize(0.7);
  t3->SetMarkerColor(1);

  t3->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96] >> h1",totalcut);

  t3->SetMarkerColor(2);
  t3->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96]",totalcut*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");
  

  t3->SetMarkerColor(3);
  t3->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96]",(totalcut && thrscut3)*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");


  //---------------------------------------------------
  // HV = 775 V

  TCanvas *s4 = new TCanvas("s4","pixel 1 - 775 V",700,900);
  s4->cd();
  s4->Divide(1,2);

  s4->cd(1);



  //  t1->Draw("C.cdetm1r.adc_c[96]",adccut);
  TH1F *lh = new TH1F("lh","ADC PMT 7, pixel 1;ADC channels; Counts",100,-100,400);
  TH1F *la = new TH1F("la","",100,-100,400);
  la->SetLineColor(2);
  TH1F *lb = new TH1F("lb","",100,-100,400);
  lb->SetLineColor(3);

  t4->Draw("C.cdetm1r.adc_c[96] >> lh",totalcut);
  //  hh->SetName("aa");
  // apply the tdc width cut

  t4->SetLineColor(2);
  t4->Draw("C.cdetm1r.adc_c[96] >> la",totalcut*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");
  cout << la->GetEntries() << endl;

  t4->SetLineColor(3);
  t4->Draw("C.cdetm1r.adc_c[96] >> lb",(totalcut && thrscut4)*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");  // width, crosstalk and threshold cut
  cout << lb->GetEntries() << endl;

  s4->cd(2);
  t4->SetMarkerStyle(21);
  t4->SetMarkerSize(0.7);
  t4->SetMarkerColor(1);

  t4->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96] >> h1",totalcut);

  t4->SetMarkerColor(2);
  t4->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96]",totalcut*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");
  

  t4->SetMarkerColor(3);
  t4->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96]",(totalcut && thrscut4)*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");

  //---------------------------------------------------------
  // HV = 800 V

  TCanvas *s5 = new TCanvas("s5","pixel 1 - 800 V",700,900);
  s5->cd();
  s5->Divide(1,2);

  s5->cd(1);



  //  t1->Draw("C.cdetm1r.adc_c[96]",adccut);
  TH1F *mh = new TH1F("mh","ADC PMT 7, pixel 1;ADC channels; Counts",100,-100,400);
  TH1F *ma = new TH1F("ma","",100,-100,400);
  ma->SetLineColor(2);
  TH1F *mb = new TH1F("mb","",100,-100,400);
  mb->SetLineColor(3);

  t5->Draw("C.cdetm1r.adc_c[96] >> mh",totalcut);
  //  hh->SetName("aa");
  // apply the tdc width cut

  t5->SetLineColor(2);
  t5->Draw("C.cdetm1r.adc_c[96] >> ma",totalcut*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>50","same");
  cout << ma->GetEntries() << endl;

  t5->SetLineColor(3);
  t5->Draw("C.cdetm1r.adc_c[96] >> mb",(totalcut && thrscut5)*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>50","same");  // width, crosstalk and threshold cut
  cout << mb->GetEntries() << endl;
  
  s5->cd(2);
  t5->SetMarkerStyle(21);
  t5->SetMarkerSize(0.7);
  t5->SetMarkerColor(1);

  t5->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96] >> h1",totalcut);

  t5->SetMarkerColor(2);
  t5->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96]",totalcut*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");
  

  t5->SetMarkerColor(3);
  t5->Draw("C.cdetm1r.tdcl[96]:C.cdetm1r.tdct[96]",(totalcut && thrscut5)*"(C.cdetm1r.tdcl[96]-C.cdetm1r.tdct[96])>40","same");


  //-------------------------------------------------------

  // Now create the efficiency plot and check

  TCanvas *s6 = new TCanvas("s6","efficiency",800,600);
  s6->cd();

  TH2F *ef1 = new TH2F("ef2","; High Voltage [V]; Efficiency [%]",100,690,810,100,90.0,100.0);
  // ef1->SetMarkerStyle(22);
  // ef1->SetMarkerColor(2);
  // ef1->SetMarkerSize(0.9);
  ef1->Draw();

  TGraph *gr1 = new TGraph();
  gr1->SetName("gr1");
  gr1->SetMarkerStyle(21);
  gr1->SetMarkerColor(7);
  //  gr1->SetMarkerSize(0.9);

  // fill the arrays with the numbers of events

  bf[0] = ha->GetEntries();
  bf[1] = ja->GetEntries();
  bf[2] = ka->GetEntries();
  bf[3] = la->GetEntries();
  bf[4] = ma->GetEntries();

  af[0] = hb->GetEntries();
  af[1] = jb->GetEntries();
  af[2] = kb->GetEntries();
  af[3] = lb->GetEntries();
  af[4] = mb->GetEntries();

  for(Int_t i=0; i<5; i++){
    gr1->SetPoint(i,700+25*i,100*af[i]/bf[i]);

    cout << i << "\t" << af[i] << "\t" << 100*af[i]/bf[i] << endl;
  } 
  
  gr1->Draw("same P");

  s6->Print("eff-test.pdf");
























 /*
 
  TH2F *h1 = new TH2F("h1","TDC comparison;Leading Edge; Trailing Edge",100,800,950,100,800,950);
  h1->SetMarkerStyle(21);
  h1->SetMarkerColor(1);
  h1->SetMarkerSize(0.7);


  //  cout << ff->GetEntries() << endl;




  /*
  Float_t bfm = 0.0;
  Float_t afm = 0.0;  // to calculate the mean of each set

  // Define a graph for each set of values
  
  TGraphErrors *g1 = new TGraphErrors();  // before
  g1->SetName("g1");
  g1->SetMarkerStyle(22);       // up triangles
  g1->SetMarkerColor(kBlack);   // color = black
  g1->SetLineColor(kBlack);
  g1->SetMarkerSize(1.2);
  
  TGraphErrors *g2 = new TGraphErrors();  // after
  g2->SetName("g2");
  g2->SetMarkerStyle(20);       // circles
  g2->SetMarkerColor(kGreen+2);   // color = green
  g2->SetLineColor(kGreen+2);
  g2->SetMarkerSize(1.2);

  // Use a loop to set the values in each graph

  for(Int_t i=0; i<14; i++){
    
    g1->SetPoint(i,i+1,abef[i]);
    g1->SetPointError(i,0.0,0.0);

    g2->SetPoint(i,i+1,aaft[i]);
    g2->SetPointError(i,0.0,erraft[i]);

    // calculate the means

    bfm += abef[i];
    afm += aaft[i];

  }

  // ------------------------------------------------

  // Create the canvas and histogram and draw the graphs

  TCanvas *s1 = new TCanvas("s1","adc-compare",900,700);
  s1->cd();

  TH1F *mdf = new TH1F("mdf","Comparison of Efficiency Ratio - PMT 7",15,0,15);
  mdf->SetMaximum(150.0);
  mdf->SetMinimum(0.0);
  mdf->SetStats(0);

  mdf->GetYaxis()->SetTitle("50% ADC Value");
  mdf->GetXaxis()->SetTitle("Paddle Number");
  mdf->SetTitleOffset(1.55,"y");
  mdf->Draw();

  // now draw the two graphs on the same plot

  g1->Draw("same P");

  g2->Draw("same P");

  // add in a legend for this plot

  lgto = new TLegend(0.63,0.7,0.88,0.85,"");
  lgto->AddEntry(g1, "CAPACITORS", "P");
  lgto->AddEntry(g2, "NO CAPACITORS", "P");
  lgto->SetTextFont(42);
  lgto->SetTextSize(0.025);
  lgto->SetFillStyle(0);
  lgto->SetBorderSize(1);
  lgto->Draw();

  // add in two lines as eye-guides
  // use the mean of each distribution for numbers

  /*
  TLine *lg1 = new TLine(0.0,bfm/14 ,15.0, bfm/14);
  lg1->SetLineColor(kRed+2);
  lg1->SetLineStyle(2);
  lg1->SetLineWidth(3);
  lg1->Draw();

  TLine *lg2 = new TLine(0.0,afm/14 ,15.0, afm/14);
  lg2->SetLineColor(kGreen+1);
  lg2->SetLineStyle(2);
  lg2->SetLineWidth(3);
  lg2->Draw();
  */



  // end of script here

}
