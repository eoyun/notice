#include <stdio.h>

float getPed(std::vector<float> wave) {

  float ped = 0;

  for ( int i = 0; i < 100; i++ )
    ped += wave.at(i) / 100.;

  return ped;
} 

// int show_wave(const TString filename)
int getFastEmulIntADC(const int runnum, const int Mid, const int channel, const int RE, const int Interval,const double time)
{

  gStyle->SetOptFit(1);
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
  int i;
  int cont;
  char filename[100];
  char rootfilename[100];
  int min_idx;
  float intADC;
  float intpedADC;
  float ped;
  int ifile;
  // get channel to plot, channel = 1 ~ 32
  //printf("Channel to plot(1~32) : ");
  //scanf("%d", &channel);
  if (channel < 1)
    ch_to_plot = 0;
  else if (channel > 32)
    ch_to_plot = 31;
  else
    ch_to_plot = channel - 1;
  sprintf(rootfilename,"./roots/Run_%d_RE_%d_int_%d.root",runnum,RE,Interval);
  //TFile* file_root = new TFile(rootfilename,"recreate");
  //TCanvas* c = new TCanvas("", "", 500, 500);
  TH1F* intADCdis = new TH1F("ADC", "Int. ADC;Int. ADC;# evts", 21000, -1000, 20000);
  std::vector<float> tmpWave;
  intADCdis->Sumw2();
  intADCdis->SetLineColor(kBlack);
  for (ifile=0;ifile<200;ifile++){  
    std::cout<<"hello"<<std::endl;
    sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_%d.dat",runnum,runnum,runnum,Mid,runnum,Mid,ifile);
    //sprintf(filename,"/Volumes/HDD_16TB_3/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_%d.dat",runnum,runnum,runnum,Mid,runnum,Mid,ifile);
    if (access(filename,0)!=0)break;
    printf("%s\n",filename);
    fp = fopen(filename, "rb");
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fclose(fp);
    nevt = file_size / 65536;
    
    printf("-----------------------------------------------------------------------\n");
    fp = fopen(filename, "rb");
    //intADCdis->SetLineWidth(2);
   
    //std::vector<float> tmpWave;
   
   
    for (evt = 0; evt < nevt; evt++) {
     
      fread(data, 1, 64, fp);
      fread(adc, 2, 32736, fp);
   
      tmpWave.clear();
      //tmpWave.clear();
   
      for (i = 0; i < 1023; i++)
        tmpWave.push_back(adc[i * 32 + ch_to_plot]);
   
      //ped = getPed(tmpWave);
   
      //for (i = 1; i <= 1000; i++ ) 

        //tmpWave.push_back(ped - tmpWave.at(i));
   
    
      min_idx = std::min_element(tmpWave.begin()+1, tmpWave.end()) - tmpWave.begin();
      intADC = std::accumulate(tmpWave.begin() + min_idx - RE, tmpWave.begin() + min_idx - RE + Interval, 0.0);
      intpedADC = std::accumulate(tmpWave.begin() + min_idx - RE - Interval, tmpWave.begin() + min_idx - RE, 0.0);
      intADCdis->Fill(intpedADC-intADC);
      tmpWave.clear();
    }
    fclose(fp);
  }
  //c->cd();
  //intADCdis->Scale(intADCdis->Integral()/time);
  intADCdis->Scale(1/time);
  intADCdis->Draw("Hist");
 // intADCdis->SetOption("Hist");
 // intADCdis->Write();
 // file_root->Close();
  //c->SaveAs((TString)("./pngs/Run"+std::to_string(runnum)+"_FastEmulIntADC_RE"+std::to_string(RE)+"_INT"+std::to_string(Interval)+".png"));


  return 0;
}

