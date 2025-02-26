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

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "mapping.C"
using namespace std;
// this  script fits a gaussian to adc data after being centered about 0
// signma is found and used later
// This is for all ADC from [0][0] to [3][63].  
// compiled into a single array, from +2 sigma to 40 channels an exponential fit is done
// the exponential term is then placed into a text document
// note that there are 4 loops instead of one nested loop for each fitting. This is because root hates strings (I'm new to programming root)
// Parker Reid - Feb 02
void initan() {

 // open file
  Int_t i;
  Int_t n; 
 ofstream myfile;
  myfile.open ("slopean.txt");
    Double_t q[100][100];
Double_t mit[100][100];
    double result[100][100];
    
    Int_t useint[100][100];
    
    
    
	// start the loop of ADC channels
	// for i = 0 - 3  {
	// for n = 0 - 63 {
	//   LOGIC HERE 
		
	 // fit a gaussian to the adc
	
	    
    
    int i = 0;

    for ( n = 0; n<64; n++){

 TF1 *myfit = new TF1("myfit", "gaus"); // line fit
    myfit->SetParName(0,"height of Gaussian");
    myfit->SetParName(1,"should be 0");
    myfit->SetParName(2,"sig"); // this is the slope parameter which we need
    
   TString adcname;
   adcname.Form("adc[0][%i]",n);
   TString adclim;
   adclim.Form("adc[0][%i]>-20&&adc[0][%i]<20",n,n); // changed from 0 to 20
    cout << adclim;
    t->Fit("myfit",adcname, adclim);
    
    mit[i][n] = myfit -> GetParameter(2);
//    useint[i][n] = int(mit[i][n]);
   
}
//loop i
i++;

    
    for ( n = 0; n<64; n++){
 TF1 *myfit = new TF1("myfit", "gaus"); // line fit
    myfit->SetParName(0,"Height of Gauss");
    myfit->SetParName(1,"Not used");
    myfit->SetParName(2,"Sig"); // this is the slope parameter which we need

    
   TString adcname;
   adcname.Form("adc[1][%i]",n);
   TString adclim;
   adclim.Form("adc[1][%d]>-20&&adc[1][%d]<20",n,n); // changed from 0 to 20
  
    t->Fit("myfit",adcname, adclim);
    
    mit[i][n] = myfit -> GetParameter(2);
  //  useint[i][n] = int (mit[i][n]);
   
}
//loop i
i++;

    
    for ( n = 0; n<64; n++){
 TF1 *myfit = new TF1("myfit", "gaus"); // line fit
    myfit->SetParName(0,"Height of Gaus");
    myfit->SetParName(1,"not used");
    myfit->SetParName(2,"sig"); // this is the slope parameter which we need

    
   TString adcname;
   adcname.Form("adc[2][%i]",n);
   TString adclim;
   adclim.Form("adc[2][%i]>-20&&adc[2][%i]<20",n,n); // changed from 0 to 20
   
    t->Fit("myfit",adcname, adclim);
    
    mit[i][n] = myfit -> GetParameter(2);
  //  useint[i][n] = int (mit[i][n]);
   
}
//loop i
i++;


    for ( n = 0; n<64; n++){
 TF1 *myfit = new TF1("myfit", "gaus"); // line fit
    myfit->SetParName(0,"Height of Gauss");
    myfit->SetParName(1,"exp fit coming soon");
    myfit->SetParName(2,"sig"); // ^^this is the slope parameter which we need

    
   TString adcname;
   adcname.Form("adc[3][%i]",n);
   TString adclim;
   adclim.Form("adc[3][%i]>-20&&adc[3][%i]<20",n,n); // changed from 0 to 20
   
    t->Fit("myfit",adcname, adclim);
    
    mit[i][n] = myfit -> GetParameter(2);
 //   useint[i][n] = int (mit[i][n]);
   
}
//loop i
i++;


//  Manipulate the gaussian here (slightly inefficient but its all in one place)

for ( i = 0; i<4; i++){
for ( n = 0; n<64; n++){
     mit[i][n] = 2*mit[i][n];
     useint[i][n] = (int)mit[i][n];
     cout << useint[i][n] << endl;
      
}
}
//
//
//
//
//   Now we fit an exponential from +2sigma to the end
//
//
//
//
	 //
      
   TF1 *myfit = new TF1("myfit", "expo"); // exponential fit
    myfit->SetParName(0,"po");
    myfit->SetParName(1,"m"); // this is the exponential parameter which we need 
    
    int i = 0;
    for ( n = 0; n<64; n++){
// TF1 *myfit = new TF1("myfit", "[0]*2.71828183^(-[1]*x)"); // line fit
 //   myfit->SetParName(0,"po");
 //   myfit->SetParName(1,"m"); // this is the exponential parameter which we need

const Int_t test = useint[i][n]; 
    
   TString adcname;
   adcname.Form("adc[0][ %d ]",n);
   TString adclim;
   adclim.Form("adc[0][ %d ]> %d &&adc[0][ %d ]<40", n, test, n); // changed from 0 to 20
 cout << adclim;  
    t->Fit("myfit",adcname, adclim);
    
    q[i][n] = myfit -> GetParameter(1);
    result[i][n] = (q[i][n]); // redundant unless the result needs changed
    myfile <<  i<<" "<< n <<" " << result[i][n] << endl;
}
//loop 2
i++;
 for ( n = 0; n<64; n++){
 // TF1 *myfit = new TF1("myfit", "[0]*2.71828183^(-[1]*x)"); // line fit
   // myfit->SetParName(0,"po");
   // myfit->SetParName(1,"m"); // this is the exponential parameter which we need

    const Int_t test = useint[i][n]; 
   TString adcname;
   adcname.Form("adc[1][%i]",n);
   TString adclim;
   adclim.Form("adc[1][%i]>%d&&adc[1][%i]<40",n,test,n); // changed from 0 to 20
   
    t->Fit("myfit",adcname, adclim);
    
    q[i][n] = myfit -> GetParameter(1);
    result[i][n] = (q[i][n]);
    myfile <<  i<<" "<< n <<" " << result[i][n] << endl;
}
// loop 3
i++;
 for ( n = 0; n<64; n++){
 //TF1 *myfit = new TF1("myfit", "[0]*2.71828183^(-[1]*x)"); // line fit
  //  myfit->SetParName(0,"po");
 //   myfit->SetParName(1,"m"); // this is the exponential parameter which we need

const Int_t test = useint[i][n]; 
    
   TString adcname;
   adcname.Form("adc[2][%i]",n);
   TString adclim;
   adclim.Form("adc[0][%i]>%d&&adc[0][%i]<40",n,test,n); // changed from 0 to 20
   
    t->Fit("myfit",adcname, adclim);
    
    q[i][n] = myfit -> GetParameter(1);
    result[i][n] = (q[i][n]);
    myfile <<  i<<" "<< n <<" " << result[i][n] << endl;
} 
// loop 4
i++;
for ( n = 0; n<64; n++){
// TF1 *myfit = new TF1("myfit", "[0]*2.71828183^(-[1]*x)"); // line fit
//    myfit->SetParName(0,"po");
//    myfit->SetParName(1,"m"); // this is the exponential parameter which we need
    
const Int_t test = useint[i][n]; 
    
   TString adcname;
   adcname.Form("adc[3][%i]",n);
   TString adclim;
  adclim.Form("adc[0][%i]>%d&&adc[0][%i]<40",n,test,n); // changed from 0 to 20
   
    t->Fit("myfit",adcname, adclim);
    
    q[i][n] = myfit -> GetParameter(1);
    result[i][n] = (q[i][n]);
    myfile <<  i<<" "<< n <<" " << result[i][n] << endl;
}

myfile.close();
}
