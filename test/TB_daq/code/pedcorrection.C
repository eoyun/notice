#include <stdio.h>

int pedcorrection(const TString outputname, const int runnum, const int Mid, const int channel)
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
  int j;
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


  TFile *file = new TFile(Form("Run_%d_%s.root", runnum, outputname.Data()), "recreate");
  std::vector<TH1F*> plot;

  sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid,runnum,Mid);
  fp = fopen(filename, "rb");
  fseek(fp, 0L, SEEK_END); 
  file_size = ftell(fp); 
  fclose(fp); 
  nevt = file_size / 65536;
  fp = fopen(filename, "rb"); 

  for (evt = 0; evt < nevt; evt++)
  {                    
    plot.push_back(new TH1F("waveform_" + (TString)(std::to_string(evt)), "Waveform;ADC;ADC", 1024, 0., 1024.));

    // read header
    fread(data, 1, 64, fp);

    // read waveform
    fread(adc, 2, 32736, fp);

    // get pedestal 
    float ped = 0;
    for ( j = 1; j < 101; j++ ){
    ped += (float)adc[j * 32 + ch_to_plot]/100.; 
  	}

    for (i = 0; i < 1023; i++){
      plot.at(evt)->Fill(i, abs((int)(ped - adc[i * 32 + ch_to_plot])));
      // plot.at(evt)->Fill(i, ((int)(ped - adc[i * 32 + ch_to_plot])));
    }

    plot.at(evt)->SetLineColor(kBlue);
    plot.at(evt)->SetOption("Hist");
    // plot.at(evt)->GetXaxis()->SetRangeUser(50,300);
    plot.at(evt)->GetYaxis()->SetRangeUser(-100,1500);
    plot.at(evt)->Write();
  }

    printf("# of events = %d\n", nevt);

    file->Close();
    fclose(fp);

    return 0;
}
  
