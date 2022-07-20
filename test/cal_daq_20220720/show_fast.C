#include <stdio.h>

int show_fast(const int filenum)
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
  int energy;
  int timing;
  char filename[100];

  // get channel to plot, channel = 1 ~ 32
  printf("Channel to plot(1~32) : ");
  scanf("%d", &channel);
  if (channel < 1)
    ch_to_plot = 0;
  else if (channel > 32)
    ch_to_plot = 31;
  else
    ch_to_plot = channel - 1;
    
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 800);
  c1->Divide(1, 2);
  TH1F *plot_e = new TH1F("plot_e", "Energy", 1000, 0, 100000); 
  TH1F *plot_t = new TH1F("plot_t", "Timing", 1000, 0, 16000); 
  plot_e->Reset();
  plot_t->Reset();

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
    energy = data[ch_to_plot * 3 + 1] & 0xFFFF;
    energy = energy * 65536;
    energy = energy + (data[ch_to_plot * 3] & 0xFFFF);

    timing = data[ch_to_plot * 3 + 2] & 0xFFFF;
    //if (timing>10000){
    printf("energy : %d evt : %d\n",energy,evt);
    plot_e->Fill(energy);
    plot_t->Fill(timing);
    //}
  }

  c1->cd(1);
  plot_e->Draw();
  c1->cd(2);
  plot_t->Draw();
  c1->Modified();
  c1->Update();
      
  fclose(fp);

  return 0;
}

