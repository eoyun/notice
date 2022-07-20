#include <stdio.h>

int show_timing(const int filenum)
{
  int channel;
  int ch_to_plot;
  FILE *fp;
  int file_size;
  int nevt;
  char header[64];
  short data[96];
  int evt;
  int i;
  int timing_a;
  int timing_b;
  int diff;
  char filename[100];

  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 800);
  c1->Divide(1, 3);
  TH1F *plot_a = new TH1F("plot_a", "channel A", 1000, 0, 16000); 
  TH1F *plot_b = new TH1F("plot_b", "channel B", 1000, 0, 16000); 
  TH1F *plot_d = new TH1F("plot_d", "Difference", 32, -512, 0); 
  plot_a->Reset();
  plot_b->Reset();
  plot_d->Reset();

  // get # of events in file
  sprintf(filename,"cal_fast_%d.dat",filenum);
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
    timing_a = data[2] & 0xFFFF;
    timing_b = data[26] & 0xFFFF;
    diff = timing_a - timing_b;

    plot_a->Fill(timing_a);
    plot_b->Fill(timing_b);
    plot_d->Fill(diff);
  }

  c1->cd(1);
  plot_a->Draw();
  c1->cd(2);
  plot_b->Draw();
  c1->cd(3);
  plot_d->Draw();
  c1->Modified();
  c1->Update();
      
  fclose(fp);

  return 0;
}

