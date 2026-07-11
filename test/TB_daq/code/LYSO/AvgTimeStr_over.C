#include <stdio.h>

int AvgTimeStr_over(const int runnum, const int Mid, const int channel1, const int channel2)
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
  // if (channel < 1)
  //   ch_to_plot = 0;
  // else if (channel > 32)
  //   ch_to_plot = 31;
  // else
  //   ch_to_plot = channel - 1;
  

  //TFile *file = new TFile(Form("AvgTimeStr_Run_%d_%s.root", runnum, outputname.Data()), "RECREATE");
  TH1F *plot1 = new TH1F("Avg.Time.Str1", "Avg.Time.Str;ADC;ADC", 1000, 0., 1000.);
  TH1F *plot2 = new TH1F("Avg.Time.Str2", "Avg.Time.Str;ADC;ADC", 1000, 0., 1000.);
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 500);

  plot1->Sumw2();
  plot2->Sumw2();

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

    plot1->Reset();
    plot2->Reset();

    for ( i = 1; i < 1000; i++ ) {
      plot1->Fill(i, adc[i * 32 + channel1 - 1]);
      plot2->Fill(i, adc[i * 32 + channel2 - 1]);
    }

  }

  plot1->SetLineColor(kRed);
  plot2->SetLineColor(kBlue);

  plot1->Scale(1./nevt);
  plot2->Scale(1./nevt);
  // plot1->GetYaxis()->SetRangeUser(1500.,3800.);
  // plot2->GetYaxis()->SetRangeUser(1500.,3800.);
  //plot->SetOption("HIST");
  //plot->Write();

  // printf("total events = %d\n", nevt);

  //file->Close();


  c1->cd();
  plot1->Draw("HIST");
  plot2->Draw("HIST SAME");
  
  fclose(fp);

  return 0;

}
