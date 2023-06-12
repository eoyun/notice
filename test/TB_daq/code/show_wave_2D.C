#include <stdio.h>

// int show_wave(const TString filename)
int show_wave_2D(const int runnum, const int Mid, const int channel)
{
  //int channel;
  int ch_to_plot;
  FILE *fp;
  long long file_size;
  int nevt;
  char data[64];
  short adc[32736];
  int evt;
  int data_length;
  int run_number;
  int tcb_trig_type;
  int tcb_trig_number;
  long long tcb_trig_time;
  int mid;
  int local_trig_number;
  int local_trigger_pattern;
  long long local_trig_time;
  long long diff_time;
  long long fine_time;
  long long coarse_time;
  int itmp;
  long long ltmp;
  int i,j;
  int cont;
  char filename[100];
  char pngname[100];
  
  // get channel to plot, channel = 1 ~ 32
  //printf("Channel to plot(1~32) : ");
  //scanf("%d", &channel);
  if (channel < 1)
    ch_to_plot = 0;
  else if (channel > 32)
    ch_to_plot = 31;
  else
    ch_to_plot = channel - 1;
    
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 500);
  TH2F *plot = new TH2F("plot", "Waveform", 1023, 0, 1023,4096,0,4096); 
  plot->SetStats(0);

  // get # of events in file
 // sprintf(filename,"cal_wave_1.dat");
  
  for (j=0;j<200;j++){
    filename[0]='\0';
    sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_%d.dat",runnum,runnum,runnum,Mid,runnum,Mid,j);
    printf("%s\n",filename);
    fflush(stdout);
    if(access(filename,0)!=0){printf("im hereee\n"); break;}
     fp = fopen(filename, "rb");
     fseek(fp, 0L, SEEK_END);
     file_size = ftell(fp);
     fclose(fp);
     nevt = file_size / 65536;
     
     printf("-----------------------------------------------------------------------\n");
     fp = fopen(filename, "rb");
    //  fp = fopen("cal_sipm_pmt_10k.dat", "rb");
    
     //btcb_trig_time = 0;
    
     for (evt = 0; evt < nevt; evt++) {
       // read header
       //if (evt!=106||evt!=404||evt!=1339||evt!=1453||evt!=1855) continue;
       //if (evt!=evtnum) continue;
       fread(data, 1, 64, fp);
       
       // data length
       
       // read waveform
       fread(adc, 2, 32736, fp);
       //printf("evt num is %d\n",evt);
       //if(evt<10000) continue;
       // fill waveform for channel to plotgecit 
       for (i = 0; i < 1023; i++) {
         plot->Fill(i, adc[i * 32 + ch_to_plot]);
       }
       
       //sprintf(pngname,"wave_%d.png",evt);
       //c1->SaveAs(filename+Form(("_%d.png"),evt));
     }    
  }

  plot->Draw("COLZ");

  fclose(fp);

  return 0;
}

