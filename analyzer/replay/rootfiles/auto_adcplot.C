  // 2th November 2020
  //
  // Script to make a comparison plot of the mean adc values
  // obtained from a fit to ADCs where a cut on the two adjacent
  // paddles was used. Comparing a single pmt, before and after
  // a set of resistors was installed to do the charge equalisation
  // Angelo Rosso -created from "adcplot.C" to automate


#ifndef __CINT__
#include <iostream>
#include <fstream>
#endif
#include <cstdio>
#include <cstring>
#include <string>
#include <math>
#include <cstdlib>
#include <vector>
#include "TMath.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TGraphErrors"
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

void auto_adcplot()
{

  // Set some of the styles first
  
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetTitleSize(0.05,"a");
  gStyle->SetTitleOffset(1.5,"a");
  gStyle->SetTitleOffset(0.9,"x");
  gStyle->SetTitleOffset(1.7,"y");
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

  // for now, take the adc values hardcoded in arrays
  // can update to read in from a file later

//   Float_t abef[14] = {417.0,358.0,364.0,346.0,400.0,436.0,384.0,442.0,430.0,339.0,304.0,346.0,361.0,363.0};  // ADC before 
  
//   Float_t aaft[14] = {294.0,295.0,306.5,292.0,269.0,281.0,297.0,276.0,287.0,327.0,319.0,284.0,300.0,342.0};  // ADC after

  // Float_t abef[14] = {291.7, 294.5, 306.3, 300.5, 299.5, 303.0, 297.7, 275.9, 283.8, 327.5, 320.2, 300.0, 297.1, 333.7};
  // Float_t aaft[14] = {309.7, 309.9, 318.9, 301.9, 340.1, 332.1, 308.5, 305.5, 311.7, 323.8, 323.5, 317.3, 308.7, 358.3};

  Double_t dummyb, dummyc, dummyd, dummye, dummyf, dummyg, dummyh, dummyk;
  Double_t pixel[14], mean1[14], mean2[14], sigma1[14], sigma2[14];


  Float_t abef[14];
  Float_t aaft[14];

  TString filename;

  cout << "Input file name: " << endl;
  cin >> filename;
  //filename = "M4-L/PMT8_adcmean_comparison.txt";

  // read from file
  ifstream file(filename);
  if(file.is_open())
  {
      string textLine;
      string pmtNum;
      string beforeRunNum;
      string afterRunNum;
      string temp;
      getline(file, textLine);
      pmtNum = textLine[30];      // 31 and 32 for M4-L
      if (isdigit(textLine[31])) {
        pmtNum = pmtNum + textLine[32];
      }
      file >> temp >> beforeRunNum >> temp >> afterRunNum;
      getline(file, textLine);
      getline(file, textLine);
      // Begin reading your stream here
      for (int i = 0; i<14; i++)
      {
         file >> dummyb >> dummyc >> dummyd >> dummye >> dummyf;
         pixel[i] = dummyb;
         abef[i] = dummyc;
         sigma1[i] = dummyd;
         aaft[i] = dummye;
         sigma2[i] = dummyf;
      }
  }
  file.close();

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

  Double_t maxim = 0.0;
  for(Int_t i=0; i<14; i++){
    
    g1->SetPoint(i,i+1,abef[i]);
    g1->SetPointError(i,0.0,0.0);

    g2->SetPoint(i,i+1,aaft[i]);
    g2->SetPointError(i,0.0,0.0);

    // calculate the means
    //cout << abef[i] << aaft[i] << i << endl;
    if (abef[i] > maxim) {
      maxim = abef[i];
    }
    if (aaft[i] > maxim) {
      maxim = aaft[i];
    }
    bfm += abef[i];
    afm += aaft[i];
  }

  // ------------------------------------------------

  // Create the canvas and histogram and draw the graphs


  TCanvas *s1 = new TCanvas("s1","adc-compare",900,700);
  s1->cd();
  string title = "Comparison of Mean ADCs - PMT" + pmtNum;
  TH1F *mdf = new TH1F("mdf",title.c_str() ,15,0,15);
  mdf->SetMaximum(maxim + 100);
  mdf->SetMinimum(0.0);
  mdf->SetStats(0);

  mdf->GetYaxis()->SetTitle("ADC Mean Value");
  mdf->GetXaxis()->SetTitle("Paddle Number");
  mdf->SetTitleOffset(1.55,"y");
  mdf->Draw();

  // now draw the two graphs on the same plot

  g1->Draw("same P");

  g2->Draw("same P");

  // add in a legend for this plot

  lgto = new TLegend(0.52,0.67,0.86,0.87,"");
  // lgto->AddEntry(g1, "WITH CAPACITORS (1756)", "P");
  // lgto->AddEntry(g2, "WITHOUT CAPACITORS (1764)", "P");
  string beforeLegendEntry = "run " + beforeRunNum + ": without resistors";
  string afterLegendEntry = "run " + afterRunNum + ": optimized resistors";
  lgto->AddEntry(g1, beforeLegendEntry.c_str(), "P");
  lgto->AddEntry(g2, afterLegendEntry.c_str(), "P");
  //Use this line if you want to exclude outliers. Comment out if not needed
  //mdf->SetMaximum(700);  // use this to overide maximum
  //lgto->AddEntry("","pixel 16 at 930 (out of range)","P"); // add legend note
  //lgto->AddEntry("","pixel 14 at 4710 (out of range)","P");


  lgto->SetTextFont(42);
  lgto->SetTextSize(0.025);
  lgto->SetFillStyle(0);
  lgto->SetBorderSize(1);
  lgto->Draw();

  // add in two lines as eye-guides
  // use the mean of each distribution for numbers

  
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

  //s1->Print(filename.substr(0, 7) + pmtNum + "_adccomparison_plot.pdf");

/*
  // Save plot as PDF
  string choice;
  cout << "save plot? (y/n): ";
  cin >> choice;
  if (choice == "y"){
  string saveAs = "M4-L/PMT" + pmtNum + "_adccomparison_plot.pdf"
  s1->Print(saveAs);
  cout << endl << "plot saved as" + saveAs << endl;
  }

*/
  // end of script here

}
