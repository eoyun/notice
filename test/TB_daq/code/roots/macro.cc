void macro(){
  gStyle->SetOptStat(0);
  TFile *a1 = new TFile("Run_2753_RE_40_int_60.root","read");
  TFile *a2 = new TFile("Run_2752_RE_40_int_60.root","read");

  TH1D *f1 = (TH1D*) a1->Get("ADC");
  TH1D *f2 = (TH1D*) a2->Get("ADC");

  f1->SetLineColor(kRed);
  f2->SetLineColor(kBlack);
  f1->SetLineWidth(2);
  f2->SetLineWidth(2);
  f1->Rebin(10);
  f2->Rebin(10);
  f1->GetXaxis()->SetRangeUser(0,50000);
  f1->Draw("hist");
  f2->Draw("hist&same");


  TLegend* legend = new TLegend(0.7,0.7,0.9,0.9);
  legend ->AddEntry(f1,"No source","l");
  legend ->AddEntry(f2,"Co 60","l");
  legend->Draw();
}
