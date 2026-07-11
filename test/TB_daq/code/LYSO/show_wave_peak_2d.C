#include <stdio.h>

//int plot_waveform_32ch_SaveAll(const TString filename, const int min, const int max, const TString condition)
int show_wave_peak_2d(const int RunNum, const int Mid, const int ch1, const int ch2, const int rangeFirst, const int rangeSecond, const int trigPatter)
{
  int channel;
  int ch_to_plot;
  FILE *fp;
  FILE *fp_cut;
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
  int k;
  int cont;
  int totN;
  double ped1;
  double ped2;
  int sum;
  double peak1;
  double peak2;
  int i_run;
  char filename[256];
  char cutfilename[256];
  // get # of events in file
  //fp = fopen( Form("Run_%d_Wave_MID_%d_FILE_0.dat",RunNum,Mid), "rb");

  int runend = RunNum;
 //  if (ch < 1)
	// ch_to_plot = 0;
 //  else if (ch > 32)
	// ch_to_plot = 31;
 //  else
	// ch_to_plot = ch - 1;

  //TFile *tfile = new TFile(Form("PeakHistSave_Wave_Run%d_%d_Mid%d_%d.root", RunNum,runend,Mid,ch),"RECREATE");
  //TFile *tfile = new TFile(Form("PeakHistSave_Wave_Run%d_%d_Mid%d_%d_800cut.root", RunNum,runend,Mid,ch),"RECREATE");
  // TH1F *hpeak = new TH1F("peak",Form("peak spectrum ch%d",ch),4096,0,4096);
  TH2D *peak2D = new TH2D("Peak2D", ";ch3 (old PMT);ch11 (new PMT)", 150, 100., 3000., 150, 100., 1800.);
  //sprintf(cutfilename,"select_evt_run_%d_%d_ch_%d.txt",RunNum,runend,ch);
  //fp_cut = fopen(cutfilename,"wt");
  TFile *tfile = new TFile(Form("Peak_2D_Run_%d.root", RunNum), "RECREATE");

//peak historgram each ch.
  //TH1F *hpeak[32];
  
  printf("-----------------------------------------------------------------------\n");
///Users/drc_daq/scratch/Aug2022TB/SSD/SSD_Run_340/Run_340_Wave/Run_340_Wave_MID_5
  for (i_run=RunNum;i_run<runend+1;i_run++){
    for (j=0;j<200;j++){
      filename[0]='\0';
      sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_%d.dat",i_run,i_run,i_run,Mid,i_run,Mid,j);
      //sprintf(filename,"/Volumes/HDD_16TB_1/HDD_Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_%d.dat",i_run,i_run,i_run,Mid,i_run,Mid,i);
      printf("%s\n",filename);
	    fflush(stdout);
	    if(access(filename,0)!=0){printf("im hereee\n"); break;}
      fp = fopen( filename, "rb");
      fseek(fp, 0L, SEEK_END);
      file_size = ftell(fp);
      fclose(fp);
      nevt = file_size / 65536;
      //nevt = 9700;
      fp = fopen(filename , "rb");
      //fp = fopen( Form("Run_%d_Wave_MID_%d_FILE_0.dat",RunNum,Mid), "rb");
     
      for (evt = 0; evt < nevt; evt++) {
        // read header
        fread(data, 1, 64, fp);
     
     
     
     
        // read waveform
        fread(adc, 2, 32736, fp);

        local_trigger_pattern = data[23] & 0xFF;
        itmp = data[24] & 0xFF;
        itmp = itmp << 8;
        local_trigger_pattern = local_trigger_pattern + itmp;
        itmp = data[25] & 0xFF;
        itmp = itmp << 16;
        local_trigger_pattern = local_trigger_pattern + itmp;
        itmp = data[26] & 0xFF;
        itmp = itmp << 24;
        local_trigger_pattern = local_trigger_pattern + itmp;
        
        // for peak histogram`
        // sum=0;  ped=0;  peak=4096;

        if ( trigPatter != -1 ) 
          if ( local_trigger_pattern != trigPatter )
            continue;
        

        /*for (i = 0; i < 51; i++) {
          sum+=adc[i * 32 + ch_to_plot];
        }
        ped=sum/50;*/
        
        //for (i = 101; i < 1020; i++) 
        std::vector<short> aWaveCH1;
        std::vector<short> aWaveCH2;
        for (i = 1; i < 1000; i++) 
        {
          aWaveCH1.push_back(adc[i * 32 + ch1 - 1]);
          aWaveCH2.push_back(adc[i * 32 + ch2 - 1]);
          // if( peak > adc[i * 32 + ch_to_plot]) peak=adc[i * 32 + ch_to_plot]; 
        }
		//if ((ch==16&&ped-peak<66)) {fprintf(fp_cut,"%d\n",evt);printf("peak value is %d\n",ped-peak);}
		//if ((ch==12&&ped-peak>1200)) {fprintf(fp_cut,"%d\n",evt);printf("peak value is %d\n",ped-peak);}
      	ped1 = std::accumulate(aWaveCH1.begin(), aWaveCH1.begin() + 100, 0.) / 100.;
        peak1 = *std::min_element(aWaveCH1.begin() + rangeFirst, aWaveCH1.begin() + rangeSecond);
        ped2 = std::accumulate(aWaveCH2.begin(), aWaveCH2.begin() + 100, 0.) / 100.;
        peak2 = *std::min_element(aWaveCH2.begin() + rangeFirst, aWaveCH2.begin() + rangeSecond);

        peak2D->Fill(ped1-peak1, ped2-peak2);


        //if(ped-peak<150) printf("evt number : %d | peak : %d\n",evt,ped-peak);  
      }//loop read .dat !
  //tfile->Close();
  //tfile->Close();
  //tfile->Close();
      fclose(fp);
    }
  }
  peak2D->Draw("colz");
  peak2D->Write();
  tfile->Close();
  //fclose(fp_cut);
  return 0;
}

