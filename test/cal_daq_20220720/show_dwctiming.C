#include <stdio.h>

int show_dwctiming(const TString filename,const int dwcNum)
{
  int ch_to_plot;
  FILE *fp;
  int file_size;
  int nevt;
  char header[64];
  short data[96];
  int evt;
  int i;
  int timing_r;
  int timing_l;
  int diff_lr;
  int timing_u;
  int timing_d;
  int diff_du;
  //char filename[100];
  int channel[4];

  TFile *tfile = new TFile(filename+"_DWC_.root","RECREATE");
  if(dwcNum == 1)
  {
  channel[0]=16;
  channel[1]=18;
  channel[2]=20;
  channel[3]=22;// 17right, 19 left, 21up, 23down
 }
// DWC 2
  else if(dwcNum == 2) 
  {
  channel[0]=24;// 25, 27, 29, 31
  channel[1]=26;
  channel[2]=28;
  channel[3]=30;// 17right, 19 left, 21up, 23down
  }
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 800);
  c1->Divide(1, 3);
  TH1F *plot_r = new TH1F("plot_r", "rigth", 1000, 0,1000); 
  TH1F *plot_l = new TH1F("plot_l", "left", 1000, 0, 1000); 
  TH1F *plot_dlr = new TH1F("plot_dlr", "Difference LR", 1000, -500, 500); 
  TH1F *plot_u = new TH1F("plot_u", "up", 1000, 0, 1000); 
  TH1F *plot_d = new TH1F("plot_d", "down", 1000, 0, 1000); 
  TH1F *plot_ddu = new TH1F("plot_ddu", "Difference DU", 1000, -500, 500); 
  plot_r->Reset();
  plot_l->Reset();
  plot_dlr->Reset();

  plot_u->Reset();
  plot_d->Reset();
  plot_ddu->Reset();
  // get # of events in file
  //sprintf(filename,"cal_fast_7_10.dat");
  fp = fopen(filename, "rb");
  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  fclose(fp);
  nevt = file_size / 256;
  
  fp = fopen(filename, "rb");

  for (evt = 0; evt < nevt; evt++) {
    // read header
    fread(header, 1, 64, fp);
    
    // read fast data
    fread(data, 2, 96, fp);
    
    // fill waveform for channel to plot
    timing_r = data[channel[0]*3+2] & 0xFFFF;
    timing_l = data[channel[1]*3+2] & 0xFFFF;

    diff_lr = timing_l - timing_r;
    timing_u = data[channel[2]*3+2] & 0xFFFF;
    timing_d = data[channel[3]*3+2] & 0xFFFF;
    diff_du = timing_d - timing_u;

    plot_r->Fill(timing_r/16);
    plot_l->Fill(timing_l/16);
    plot_dlr->Fill(diff_lr/16);
    plot_u->Fill(timing_u/16);
    plot_d->Fill(timing_d/16);
    plot_ddu->Fill(diff_du/16);
  }

  plot_dlr->GetXaxis()->SetRangeUser( plot_dlr->GetMean()-5*plot_dlr->GetStdDev(), plot_dlr->GetMean()+5*plot_dlr->GetStdDev());
  plot_ddu->GetXaxis()->SetRangeUser( plot_ddu->GetMean()-5*plot_ddu->GetStdDev(), plot_ddu->GetMean()+5*plot_ddu->GetStdDev());
  /*c1->cd(1);
  plot_a->Draw();
  c1->cd(2);
  plot_b->Draw();
  c1->cd(3);
  plot_d->Draw();
  c1->Modified();
  c1->Update();
  */    
  fclose(fp);
  plot_r->Write();
  plot_l->Write();
  plot_dlr->Write();
  plot_u->Write();
  plot_d->Write();
  plot_ddu->Write();
  tfile->Close();
  return 0;
}

