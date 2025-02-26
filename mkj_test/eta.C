
Double_t _eta_func(Double_t *x, Double_t *par){
  Double_t A = x[0];
  Double_t Ath = par[0];
  Double_t Aw = par[1];
  Double_t pi = 4*atan(1);

  return ( atan( (A - Ath) / Aw ) + pi/2 ) / pi;
};

// plot eta (calculated from histos) and its fit for a given slot and
// channel. N is the number of bins to consider, drawthings=1 actually
// do the plots (if 0 don't. Useful for just fit everything and save results)
// The fit results are stored in global arrays Athresholds and Awidths
void plot_eta(int slot, int chan, int tdc_cut, const int n=200){
  TH1D eta_raw_adctot = TH1D("eta_raw_adctot","eta_raw_adctot",n, 0, n);
  TH1D eta_raw_adccut = TH1D("eta_raw_adccut","eta_raw_adccut",n, 0, n);
  float eta_raw_out[n];
  TString draw, cut, title;
  float ratio;
  float bin[n];

  TCanvas *ceta_raw = new TCanvas("ceta_raw","ceta_raw");

  TGraph *geta_raw;

  draw.Form("adc[%d][%d]>>eta_raw_adctot",slot,chan);
  t->Draw(draw,"");

  draw.Form("adc[%d][%d]>>eta_raw_adccut", slot,chan);
  cut.Form("tdct[%d][%d]>%d",slot, chan, tdc_cut);
  t->Draw(draw,cut);

  for (int i=1; i<n; i++){
    bin[i] = i;
    float num = (float)eta_raw_adccut.GetBinContent(i);
    float den = (float)eta_raw_adctot.GetBinContent(i);
    
    if (den == 0){ // no counts to begin with, so we also have 0 counts after the cut. Same number of counts (0) implies ratio = 1 (if we would blindly divide the counts, we would have ratio=0/0, so we need a special case)
      ratio = 1.;
    } else { // at least one count in the channel before the cut
      ratio = num/den;
    };
    eta_raw_out[i]=ratio;

    // cout<<"i "<< i<<" num "<< num<<" den "<< den<<" ratio "<<ratio<<endl;
  };
  
  title.Form("run %d slot %d channel %d eta",run,slot,chan);
  geta_raw = new TGraph(n,bin,eta_raw_out);
  geta_raw->SetTitle(title);
  geta_raw->SetMarkerStyle(6);
  geta_raw->SetMarkerColor(kRed);
  geta_raw->GetXaxis()->SetRangeUser(0,n);
  
  TF1 eta = TF1("eta",_eta_func,(Double_t)0.,(Double_t)n, 2);
  eta.SetLineColor(kBlue);
  eta.SetParNames("Ath","Aw");
  eta.SetParameters(40,20);
  
  geta_raw->Draw("AP");
  geta_raw->Fit("eta","R");
  
  double Ath = eta.GetParameter(0);
  double Aw = eta.GetParameter(1);
  
  // write fit results
  TPaveText *pt = new TPaveText(10,.8,60,.99);
  title.Form("Threshold Ath = %f",Ath);
  pt->AddText(title);
  title.Form("Width Aw = %f",Aw);
  pt->AddText(title);
  pt->Draw();
    
  Athresholds[slot][chan] = Ath;
  Awidths[slot][chan] = Aw;
  cout << "Saved Ath = " << Ath << " Aw = " << Aw <<" in arrays Athresholds and Awidths"<<endl;

  // return ceta_raw;
}

// calculate all eta fits, in order to populate the arrays
void fit_all_eta(int tdc_cut, int maxbin=200){
  for (int slot = 0; slot <NUMADCSLOTS; slot++){
    for (int chan = 0; chan < NUMCHANA; chan++){
      plot_eta(slot, chan, tdc_cut, maxbin);
    }
  }
}

