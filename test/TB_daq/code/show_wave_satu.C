#include <stdio.h>

//int plot_waveform_32ch_SaveAll(const TString filename, const int min, const int max, const TString condition)
int show_wave_satu(const int RunNum,const int runend, const int Mid, const int ch, const int cut_value)
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
  int ped;
  int sum;
  int peak;
  int i_run;
  char filename[256];
  char cutfilename[256];
  int flag =0;
  int count =0;// get # of events in file
  //fp = fopen( Form("Run_%d_Wave_MID_%d_FILE_0.dat",RunNum,Mid), "rb");

  if (ch < 1)
	ch_to_plot = 0;
  else if (ch > 32)
	ch_to_plot = 31;
  else
	ch_to_plot = ch - 1;

  //TFile *tfile = new TFile(Form("PeakHistSave_Wave_Run%d_%d_Mid%d_%d.root", RunNum,runend,Mid,ch),"RECREATE");
  //TFile *tfile = new TFile(Form("PeakHistSave_Wave_Run%d_%d_Mid%d_%d_800cut.root", RunNum,runend,Mid,ch),"RECREATE");
  //sprintf(cutfilename,"select_evt_run_%d_%d_ch_%d.txt",RunNum,runend,ch);
  //fp_cut = fopen(cutfilename,"wt");
  //TFile *tfile = new TFile(Form("PeakHistSave_Wave_Run%d_%d_Mid%d_%d.root", RunNum,runend,Mid,ch),"RECREATE");
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 500);
  TH1F *plot = new TH1F("plot", "Waveform", 1023, 0, 1023); 
  plot->SetStats(0);

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
        
        // for peak histogram`
        sum=0;  ped=0;  peak=4096;    
        for (i = 0; i < 101; i++) {
          sum+=adc[i * 32 + ch_to_plot];
        }
        ped=sum/100;
        //for (i = 101; i < 1020; i++) 
        for (i = 20; i < 1020; i++) 
        {
          if( peak > adc[i * 32 + ch_to_plot]) peak=adc[i * 32 + ch_to_plot]; 
        }
		    if (ped-peak>cut_value) {
          plot->Reset();
          for (i = 0; i < 1023; i++) {
            plot->Fill(i, adc[i * 32 + ch_to_plot]);
          }
          plot->GetYaxis()->SetRangeUser(0,4096);
          plot->Draw("hist");
          if (evt < 1000)c1->SaveAs(Form("./pngs/run%d_waveform%d.png",i_run,evt));
          c1->Modified();
          c1->Update();
          printf("peak value is %d\n",ped-peak);
          printf("Continue? ");
          scanf("%d", &cont);
          count+=1;
          if (cont == 0){
            flag=1;
            break;
          }
        }
    //((ch==16&&ped-peak<66)) {fprintf(fp_cut,"%d\n",evt);printf("peak value is %d\n",ped-peak);}
		//if ((ch==12&&ped-peak>1200)) {fprintf(fp_cut,"%d\n",evt);printf("peak value is %d\n",ped-peak);}
        //if(ped-peak<150) printf("evt number : %d | peak : %d\n",evt,ped-peak);  
      }//loop read .dat !
  //tfile->Close();
  //tfile->Close();
  //tfile->Close();
      fclose(fp);
      if (flag==1) break;
    }
    if (flag==1) break;
  }
  printf("total number of over %d ADC evt is %d\n",cut_value,count);
  //tfile->Close();
  //fclose(fp_cut);
  return 0;
}

