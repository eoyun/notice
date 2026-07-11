#include <stdio.h>
#include <vector>

std::vector<double> pedcorwave (std::vector<short> waveform, int range){
        double ped = (double) std::accumulate(waveform.begin() + 1, waveform.begin() + 1 + range, 0.)/range;
        std::vector<double> pedcor;
        //std::cout<<ped<<std::endl;
        for (int i = 0; i < (int)waveform.size(); i++ )
                pedcor.push_back((double)(ped - waveform.at(i)));
        return pedcor;
}

double getTime (std::vector<double> pedcorwave, double fraction){
        int max_idx = max_element(pedcorwave.begin()+1,pedcorwave.end())-pedcorwave.begin();
        double max = *max_element(pedcorwave.begin()+1,pedcorwave.end());
        double time = std::numeric_limits<double>::max();
        for (int i = 0; i<1000; i++){
                int idx = max_idx - i;
                if (idx < 0) break;
                if (pedcorwave.at(idx) < max * fraction){
                        time = idx + (max * fraction - pedcorwave.at(idx))/( pedcorwave.at(idx + 1) - pedcorwave.at(idx));
                        //std::cout<<idx<<" | "<<(max * fraction - pedcorwave.at(idx))/( pedcorwave.at(idx + 1) - pedcorwave.at(idx))<<std::endl;
                        break;
                }
        }
        return time * 0.2;
}


// int show_wave(const TString filename)
int printDRSStop(const int runnum, const int Mid,const int Mid2, const int channel, const int channel2)
{

  gStyle->SetOptFit(1);
  //int channel;
  int ch_to_plot, ch_to_plot2;
  FILE *fp;
  FILE *fp2;
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
  double time_res;
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
  if (channel2 < 1)
    ch_to_plot2 = 0;
  else if (channel2 > 32)
    ch_to_plot2 = 31;
  else
    ch_to_plot2 = channel2 - 1;
  sprintf(rootfilename,"./roots/Run_%d.root",runnum);
  //TFile* file_root = new TFile(rootfilename,"recreate");
  //TCanvas* c = new TCanvas("", "", 500, 500);
  TH1F* timedis = new TH1F("ADC", ";stop Bin;# evts", 1000, -1000, 1000);
  TH1F* stopdis = new TH1F("ADC", ";time;# evts", 1000, 0, 1000);
  TH2F* vs = new TH2F("","",1000,0,1000,1000,-5,5);
  std::vector<short> tmpWave_ch1;
  std::vector<short> tmpWave_ch2;
  std::vector<double> Wave_ch1;
  std::vector<double> Wave_ch2;

  timedis->Sumw2();
  timedis->SetLineColor(kBlack);
  for (ifile=0;ifile<200;ifile++){  
    sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_%d.dat",runnum,runnum,runnum,Mid,runnum,Mid,ifile);
    if (access(filename,0)!=0)break;
    printf("%s\n",filename);
    fp = fopen(filename, "rb");
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fclose(fp);
    nevt = file_size / 65536;
    
    printf("-----------------------------------------------------------------------\n");
    fp = fopen(filename, "rb");
    sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_%d.dat",runnum,runnum,runnum,Mid2,runnum,Mid2,ifile);
    fp2 = fopen(filename, "rb");
    //intADCdis->SetLineWidth(2);
   
    //std::vector<float> tmpWave;
   
   
    for (evt = 0; evt < 1000; evt++) {
      int drs_stop1, drs_stop2, drs_stop3, drs_stop4; 
      int drs_stop1_mid2, drs_stop2_mid2, drs_stop3_mid2, drs_stop4_mid2; 
      fread(data, 1, 64, fp);
      drs_stop1 = data[34] & 0xFF;
      itmp = data[35] &0xFF;
      itmp = itmp << 8;
      drs_stop1 += itmp;
      drs_stop2 = data[36] & 0xFF;
      itmp = data[37] &0xFF;
      itmp = itmp << 8;
      drs_stop2 += itmp;
      drs_stop3 = data[38] & 0xFF;
      itmp = data[39] &0xFF;
      itmp = itmp << 8;
      drs_stop3 += itmp;
      drs_stop4 = data[40] & 0xFF;
      itmp = data[41] &0xFF;
      itmp = itmp << 8;
      drs_stop4 += itmp;
      int pll = data[42] & 0xFF;
      fread(adc, 2, 32736, fp);
      fread(data, 1, 64, fp2);
      drs_stop1_mid2 = data[34] & 0xFF;
      itmp = data[35] &0xFF;
      itmp = itmp << 8;
      drs_stop1_mid2 += itmp;
      drs_stop2_mid2 = data[36] & 0xFF;
      itmp = data[37] &0xFF;
      itmp = itmp << 8;
      drs_stop2_mid2 += itmp;
      drs_stop3_mid2 = data[38] & 0xFF;
      itmp = data[39] &0xFF;
      itmp = itmp << 8;
      drs_stop3_mid2 += itmp;
      drs_stop4_mid2 = data[40] & 0xFF;
      itmp = data[41] &0xFF;
      itmp = itmp << 8;
      drs_stop4_mid2 += itmp;
      int pll_mid2 = data[42] & 0xFF;
      fread(adc, 2, 32736, fp2);

      printf("MID 1 (DRS1) : %d | MID 2 (DRS1) : %d \n",drs_stop1,drs_stop1_mid2);
   
      timedis->Fill(drs_stop1 - drs_stop1_mid2);
    
      //for (i = 1; i <= 1000; i++ ) 

        //tmpWave.push_back(ped - tmpWave.at(i));
   
    
      //min_idx = std::min_element(tmpWave.begin(), tmpWave.end()) - tmpWave.begin();
      //intpedADC = std::accumulate(tmpWave.begin() + min_idx - RE - Interval, tmpWave.begin() + min_idx - RE, 0.0);
    }
    fclose(fp);
    fclose(fp2);
  }
  timedis->Draw("hist");
  //c->cd();
  //vs->Draw("COLZ");
  //intADCdis->SetOption("Hist");
  //intADCdis->Write();
  //file_root->Close();
  //c->SaveAs((TString)("./pngs/Run"+std::to_string(runnum)+"_FastEmulIntADC_RE"+std::to_string(RE)+"_INT"+std::to_string(Interval)+".png"));


  return 0;
}

