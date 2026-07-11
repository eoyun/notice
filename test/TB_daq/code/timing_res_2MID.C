#include <stdio.h>

int timing_res_2MID(const int runnum,const int Mid1, const int Mid2, const int ch)
{
  int channel;
  int ch_to_plot;
  //int ch_to_plot2;
  FILE *fp1;
  FILE *fp2;
  int file_size;
  int nevt;
  char header1[64];
  char header2[64];
  short data1[96];
  short data2[96];
  int evt;
  int i;
  int energy;
  int timing1;
  int timing2;
  char filename1[200];
  char filename2[200];

  // get channel to plot, channel = 1 ~ 32
  //printf("Channel to plot(1~32) : ");
  //scanf("%d", &channel);
  if (ch < 1)
    ch_to_plot = 0;
  else if (ch > 32)
    ch_to_plot = 31;
  else
    ch_to_plot = ch - 1;
  //if (ch2 < 1)
  //  ch_to_plot2 = 0;
  //else if (ch2 > 32)
  //  ch_to_plot2 = 31;
  //else
  //  ch_to_plot2 = ch2 - 1;
  //TFile *fp_root = new TFile("531.root","recreate");  
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 800);
  c1->Divide(1, 3);
  //TH1F *plot_e = new TH1F("plot_e", "Energy", 1000, 0, 100000); 
  TH1F *plot_t1 = new TH1F("plot_t1", "Timing1", 1000, 0, 16000); 
  TH1F *plot_t2 = new TH1F("plot_t1", "Timing2", 1000, 0, 16000); 
  TH1F *plot_t_diff = new TH1F("plot_t_diff", "diff", 2000, -1000, 1000); 
  //plot_e->Reset();
  plot_t1->Reset();
  plot_t2->Reset();
  plot_t_diff->Reset();

  // get # of events in file
  sprintf(filename1,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Fast/Run_%d_Fast_MID_%d/Run_%d_Fast_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid1,runnum,Mid1);
  sprintf(filename2,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Fast/Run_%d_Fast_MID_%d/Run_%d_Fast_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid2,runnum,Mid2);
  //sprintf(filename,"cal_fast_7_10.dat");
  fp1 = fopen(filename1, "rb");
  fseek(fp1, 0L, SEEK_END);
  file_size = ftell(fp1);
  fclose(fp1);
  nevt = file_size / 256;
  
  fp1 = fopen(filename1, "rb");
  fp2 = fopen(filename2, "rb");

  for (evt = 0; evt < nevt; evt++) {
    // read header
    fread(header1, 1, 64, fp1);
    fread(header2, 1, 64, fp2);
    
    // read fast data
    fread(data1, 2, 96, fp1);
    fread(data2, 2, 96, fp2);
    
    // fill waveform for channel to plot
    //energy = data[ch_to_plot * 3 + 1] & 0xFFFF;
    //energy = energy * 65536;
    //energy = energy + (data[ch_to_plot * 3] & 0xFFFF);

    timing1 = data1[ch_to_plot * 3 + 2] & 0xFFFF;
    timing2 = data2[ch_to_plot * 3 + 2] & 0xFFFF;
    //if (timing>10000){
    //printf("energy : %d evt : %d\n",energy,evt);
    //plot_e->Fill(energy);
    plot_t1->Fill(timing1);
    plot_t2->Fill(timing2);
    plot_t_diff->Fill(timing2-timing1);
    //}
  }

  c1->cd(1);
  plot_t1->Draw();
  c1->cd(2);
  plot_t2->Draw();
  c1->cd(3);
  plot_t_diff->Draw();

  c1->Write();
  c1->Modified();
  c1->Update();
      
  fclose(fp1);
  fclose(fp2);
  //fp_root->Close();
  return 0;
}

