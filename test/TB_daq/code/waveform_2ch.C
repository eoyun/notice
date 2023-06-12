#include <stdio.h>

int waveform_2ch(const int runnum, const int Mid, const int ch_1, const int ch_2, const int min, const int max)
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
  int ifile;
  char filename[100];
  char pngname[100];

  // // get channel to plot, channel = 1 ~ 32
  // if (channel < 1)
  //   ch_to_plot = 0;
  // else if (channel > 32)
  //   ch_to_plot = 31;
  // else
  //   ch_to_plot = channel - 1;

  //TFile *afile = new TFile("waveform.root", "RECREATE");

  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 1000, 500);
  c1->Divide(2,1,0.0001,0.0001);
  gPad->SetLeftMargin(0);
  gPad->SetRightMargin(0);
  gPad->SetBottomMargin(0);
  gPad->SetTopMargin(0);

  std::vector<TH1F*> plot;

  // for (i=0; i<2; i++){

    plot.push_back(new TH1F((TString)("ch_" + std::to_string(ch_1)), (TString)("ch_" + std::to_string(ch_1)), 1024, 0., 1024.));
    // plot.at(ch_1)->SetStats(0);
     plot.push_back(new TH1F((TString)("ch_" + std::to_string(ch_2)), (TString)("ch_" + std::to_string(ch_2)), 1024, 0., 1024.));
    // plot.at(ch_2)->SetStats(0);
  // }

  for ( ifile = 0; ifile < 200; ifile++ ) {  
  sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_%d.dat",runnum,runnum,runnum,Mid,runnum,Mid,ifile);
  fp = fopen(filename, "rb");
  fseek(fp, 0L, SEEK_END); 
  file_size = ftell(fp); 
  fclose(fp); 
  nevt = file_size / 65536;
  fp = fopen(filename, "rb"); 

  for (evt = 0; evt < nevt; evt++){

    fread(data, 1, 64, fp);
    fread(adc, 2, 32736, fp);

    for (i=0; i<2; i++){
    plot.at(i)->Reset();
    }

    for (i=0; i<1023; i++) {
      for (j=0; j<1; j++) {
        plot.at(j)->Fill(i, adc[i * 32 + (ch_1-1)]);
      }
      for (j=1; j<2; j++) {
        plot.at(j)->Fill(i, adc[i * 32 + (ch_2-1)]);
      }
    }

    for(i=0; i<2; i++){
    c1->cd(i+1);
    plot.at(i)->GetYaxis()->SetRangeUser(min,max);
    plot.at(i)->Draw("HIST");
//    plot.at(i)->write();
    }

    c1->Modified();
    c1->Update();

    printf("Continue? ");
    scanf("%d", &cont);
  
    if (cont == 0){
      evt = nevt;
    }
  }

//  afile->Close();
  fclose(fp);
}
  return 0;
  
}
