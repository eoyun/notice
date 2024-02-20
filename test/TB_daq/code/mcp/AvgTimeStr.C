#include <stdio.h>

int AvgTimeStr(const int runnum, const int Mid, const int channel, const TString name)
{
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
  int i;
  int cont;
  char filename[100];
  char pngname[100];


  // get channel to plot, channel = 1 ~ 32
  if (channel < 1)
    ch_to_plot = 0;
  else if (channel > 32)
    ch_to_plot = 31;
  else
    ch_to_plot = channel - 1;
  

  TFile *file = new TFile("./240129/" + name + ".root", "RECREATE");
  TH1F *plot = new TH1F("Avg.Time.Str", "Avg.Time.Str;ADC;ADC", 1024, 0., 1024.);
  // TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 500);


  sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid,runnum,Mid);
  fp = fopen(filename, "rb");
  fseek(fp, 0L, SEEK_END); 
  file_size = ftell(fp); 
  fclose(fp); 
  nevt = file_size / 65536;
  fp = fopen(filename, "rb"); 


  for ( evt = 0; evt < nevt; evt++ ) {

    fread(data, 1, 64, fp);
    fread(adc, 2, 32736, fp);

    for ( i = 0; i < 1023; i++ ) {

      plot->Fill(i, adc[i * 32 + ch_to_plot]);
    }

  }

  plot->Sumw2();
  plot->Scale(1./nevt);
  plot->GetYaxis()->SetRangeUser(1000,4000);
  plot->SetOption("HIST");
  // plot->Draw("HIST");
  plot->Write();
  
  file->Close();
  
  fclose(fp);

  return 0;

}
