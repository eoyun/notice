#include <stdio.h>

int waveform_8ch_avg(const int runnum, const int Mid)
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

  // // get channel to plot, channel = 1 ~ 32
  // if (channel < 1)
  //   ch_to_plot = 0;
  // else if (channel > 32)
  //   ch_to_plot = 31;
  // else
  //   ch_to_plot = channel - 1;

  // TFile *afile = new TFile("waveform.root", "RECREATE");
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 600, 1000);
  c1->Divide(2,4,0.001,0.001);
  gPad->SetLeftMargin(0);
  gPad->SetRightMargin(0);
  gPad->SetBottomMargin(0);
  gPad->SetTopMargin(0);

  std::vector<TH1F*> plot;

  for ( i = 0; i < 8; i++ ) {
    plot.push_back(new TH1F((TString)("Waveform_ch_" + std::to_string(i+1)), (TString)("ch_" + std::to_string(i+1)), 1024, 0., 1024.));
    //plot.push_back(new TH1F((TString)("Waveform_ch_" + std::to_string(i+17)), (TString)("ch_" + std::to_string(i+17)), 1024, 0., 1024.));
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

  for (evt = 0; evt < nevt; evt++){

    fread(data, 1, 64, fp);
    fread(adc, 2, 32736, fp);


    for ( i = 0; i < 1023; i++ ) {
      for ( j = 0; j < 8; j++ ) {
      plot.at(j)->Fill(i, adc[i * 32 + j]);
      }
    }
  }


  for( i = 0; i < 8; i++ ) {
    plot.at(i)->Scale(1./nevt);
    // plot.at(i)->GetYaxis()->SetRangeUser(plot.at(i)->GetBinContent(100) - 60plot.at(i)->GetBinContent(100) + 40
    plot.at(i)->GetYaxis()->SetRangeUser(3200,4000);
    // c1->cd(i+1)
    // plot.at(i)->Draw("HIST");
  }

  c1->cd(1); plot.at(1)->Draw("HIST");
  c1->cd(2); plot.at(0)->Draw("HIST");
  c1->cd(3); plot.at(3)->Draw("HIST");
  c1->cd(4); plot.at(2)->Draw("HIST");
  c1->cd(5); plot.at(5)->Draw("HIST");
  c1->cd(6); plot.at(4)->Draw("HIST");
  c1->cd(7); plot.at(7)->Draw("HIST");
  c1->cd(8); plot.at(6)->Draw("HIST");

  c1->SaveAs("./plots/Avg_Run_" + (TString)(std::to_string(runnum)) + "_8ch.png");
  // afile->Close();

  fclose(fp);
  return 0;
  
}
