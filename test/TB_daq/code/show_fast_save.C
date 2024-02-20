#include <stdio.h>

int show_fast_save(const int RunNum, const int Mid, const int ch)
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
  int j;
  char filename[100];
//  char filename[100];

  // get channel to plot, channel = 1 ~ 32
  if (ch < 1)
    ch_to_plot = 0;
  else if (ch > 32)
    ch_to_plot = 31;
  else
    ch_to_plot = ch - 1;
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 800);
  c1->Divide(1, 2);
  TH1F *plot_e = new TH1F("plot_e", "Energy", 1000, -5000, 300000); 
  TH1F *plot_t = new TH1F("plot_t", "Timing", 1000, 0, 16000); 
  plot_e->Reset();
  plot_t->Reset();

  // get # of events in file
  //sprintf(filename,"cal_fast_7_10.dat");
  for (j=0;j<200;j++){
    filename[0]='\0';
    sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Fast/Run_%d_Fast_MID_%d/Run_%d_Fast_MID_%d_FILE_%d.dat",RunNum,RunNum,RunNum,Mid,RunNum,Mid,j);
    //sprintf(filename,"/Volumes/HDD_16TB_3/Run_%d/Run_%d_Fast/Run_%d_Fast_MID_%d/Run_%d_Fast_MID_%d_FILE_%d.dat",RunNum,RunNum,RunNum,Mid,RunNum,Mid,j);
	  if(access(filename,0)!=0){printf("im hereee\n"); break;}
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
      printf("energy : %d | timing : %d | evt : %d\n",energy,timing,evt);
      plot_e->Fill(energy);
      plot_t->Fill(timing);
      //}
    }
    fclose(fp);
  }

  c1->cd(1);
  plot_e->Draw();
  c1->cd(2);
  plot_t->Draw();

      
  return 0;
}

