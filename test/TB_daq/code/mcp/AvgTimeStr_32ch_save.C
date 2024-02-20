#include <stdio.h>

int AvgTimeStr_32ch_save(const int runnum, const int Mid)
{
  int ch_to_plot;
  int channel;
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
  int j;
  int cont;
  char filename[100];
  char pngname[100];


  TFile *afile = new TFile("Avg_32ch_Run_" + (TString)(std::to_string(runnum)) + "_" + (TString)(std::to_string(Mid)) + ".root", "RECREATE");

  std::vector<TH1F*> plot;

  for ( i = 0; i < 32; i++ ) {

    plot.push_back(new TH1F((TString)("Avg_mid" + std::to_string(Mid) + "_ch" + std::to_string(i+1)), (TString)("ch" + std::to_string(i+1)), 1024, 0., 1024.));
    plot.at(i)->SetStats(0);
    plot.at(i)->Sumw2();
  }

  sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid,runnum,Mid);
  fp = fopen(filename, "rb");
  fseek(fp, 0L, SEEK_END); 
  file_size = ftell(fp); 
  fclose(fp); 
  nevt = file_size / 65536;
  fp = fopen(filename, "rb"); 

  for (evt = 0; evt < nevt; evt++) {

    fread(data, 1, 64, fp);
    fread(adc, 2, 32736, fp);

    for ( i = 0; i < 1023; i++) {
      for ( j = 0; j < 32; j++ ) {
        plot.at(j)->Fill(i, adc[i * 32 + j]);
      }
    }
  }

    for ( i = 0; i < 32; i++ ) {
      plot.at(i)->Scale(1./(float)nevt);
      plot.at(i)->GetYaxis()->SetRangeUser(plot.at(i)->GetBinContent(100) - 800, plot.at(i)->GetBinContent(100) + 200);
      plot.at(i)->SetOption("HIST");
      plot.at(i)->Write();
    }

  afile->Close();

  fclose(fp);
  return 0;
  
}
