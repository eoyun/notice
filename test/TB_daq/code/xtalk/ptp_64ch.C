#include <stdio.h>

//int plot_waveform_32ch(const TString filename, const int min, const int max, const TString condition)
int ptp_64ch(const int runnum, const int Mid1, const int Mid2)
{
  char filename1[100];
  char filename2[100];

  int channel;
  int ch_to_plot;
  FILE *fp1;
  FILE *fp2;
  int file_size;
  int nevt;
  int ndraw;
  char data1[64];
  char data2[64];
  short adc1[32736];
  short adc2[32736];
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
  int max[64];
  int min[64];
  //ndraw = 10;
//  ndraw = 10;
  // get channel to plot, channel = 1 ~ 32
//  printf("Channel to plot(1~32) : ");
//  scanf("%d", &channel);
//  if (channel < 1)
//    ch_to_plot = 0;
//  else if (channel > 32)
//    ch_to_plot = 31;
//  else
//    ch_to_plot = channel - 1;
  TFile* test = new TFile(Form("test_%d.root",runnum),"recreate");
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 3600, 3600);
  c1->Divide(8,8,0.001,0.001);
  gPad->SetLeftMargin(0);
  gPad->SetRightMargin(0);
  gPad->SetBottomMargin(0);
  gPad->SetTopMargin(0);

  TH1F *plot[64];
  for( i = 0 ; i < 64 ; i ++)
  {
     plot[i] = new TH1F(Form("plot%d",i+1), Form("%d", i+1), 4096,0,4096);
     plot[i]->SetStats(0);  
  }

  // get # of events in file
  sprintf(filename1,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid1,runnum,Mid1);
  sprintf(filename2,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid2,runnum,Mid2);
  fp1 = fopen(filename1, "rb");
  fseek(fp1, 0L, SEEK_END);
  file_size = ftell(fp1);
  fclose(fp1);
  nevt = file_size / 65536;
  
  printf("-----------------------------------------------------------------------\n");
  fp1 = fopen(filename1, "rb");
  fp2 = fopen(filename2, "rb");

  for (evt = 0; evt < nevt; evt++) {
    // read header
    fread(data1, 1, 64, fp1);
    fread(data2, 1, 64, fp2);
    
    // data length
    /*data_length = data[0] & 0xFF;
    itmp = data[1] & 0xFF;
    itmp = itmp << 8;
    data_length = data_length + itmp;
    itmp = data[2] & 0xFF;
    itmp = itmp << 16;
    data_length = data_length + itmp;
    itmp = data[3] & 0xFF;
    itmp = itmp << 24;
    data_length = data_length + itmp;

    // run number
    run_number = data[4] & 0xFF;
    itmp = data[5] & 0xFF;
    itmp = itmp << 8;
    run_number = run_number + itmp;
    
    // trigger type
    tcb_trig_type = data[6] & 0xFF;
    
    // TCB trigger #
    tcb_trig_number = data[7] & 0xFF;
    itmp = data[8] & 0xFF;
    itmp = itmp << 8;
    tcb_trig_number = tcb_trig_number + itmp;
    itmp = data[9] & 0xFF;
    itmp = itmp << 16;
    tcb_trig_number = tcb_trig_number + itmp;
    itmp = data[10] & 0xFF;
    itmp = itmp << 24;
    tcb_trig_number = tcb_trig_number + itmp;

    // TCB trigger time
    fine_time = data[11] & 0xFF;
    fine_time = fine_time * 11;     // actually * (1000 / 90)
    coarse_time = data[12] & 0xFF;
    ltmp = data[13] & 0xFF;
    ltmp = ltmp << 8;
    coarse_time = coarse_time + ltmp;
    ltmp = data[14] & 0xFF;
    ltmp = ltmp << 16;
    coarse_time = coarse_time + ltmp;
    ltmp = data[15] & 0xFF;
    ltmp = ltmp << 24;
    coarse_time = coarse_time + ltmp;
    ltmp = data[16] & 0xFF;
    ltmp = ltmp << 32;
    coarse_time = coarse_time + ltmp;
    ltmp = data[17] & 0xFF;
    ltmp = ltmp << 40;
    coarse_time = coarse_time + ltmp;
    coarse_time = coarse_time * 1000;   // get ns
    tcb_trig_time = fine_time + coarse_time;
    
    // mid
    mid = data[18] & 0xFF;

    // local trigger #
    local_trig_number = data[19] & 0xFF;
    itmp = data[20] & 0xFF;
    itmp = itmp << 8;
    local_trig_number = local_trig_number + itmp;
    itmp = data[21] & 0xFF;
    itmp = itmp << 16;
    local_trig_number = local_trig_number + itmp;
    itmp = data[22] & 0xFF;
    itmp = itmp << 24;
    local_trig_number = local_trig_number + itmp;

    // local trigger #
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

    // local trigger time
    fine_time = data[27] & 0xFF;
    fine_time = fine_time * 11;     // actually * (1000 / 90)
    coarse_time = data[28] & 0xFF;
    ltmp = data[29] & 0xFF;
    ltmp = ltmp << 8;
    coarse_time = coarse_time + ltmp;
    ltmp = data[30] & 0xFF;
    ltmp = ltmp << 16;
    coarse_time = coarse_time + ltmp;
    ltmp = data[31] & 0xFF;
    ltmp = ltmp << 24;
    coarse_time = coarse_time + ltmp;
    ltmp = data[32] & 0xFF;
    ltmp = ltmp << 32;
    coarse_time = coarse_time + ltmp;
    ltmp = data[33] & 0xFF;
    ltmp = ltmp << 40;
    coarse_time = coarse_time + ltmp;
    coarse_time = coarse_time * 1000;   // get ns
    local_trig_time = fine_time + coarse_time;

    diff_time = local_trig_time - tcb_trig_time;
    printf("evt = %d, data length = %d, run # = %d, mid = %d\n", evt, data_length, run_number, mid);
    printf("trigger type = %X, local trigger pattern = %X\n", tcb_trig_type, local_trigger_pattern);
    printf("TCB trigger # = %d, local trigger # = %d\n", tcb_trig_number, local_trig_number);
    printf("TCB trigger time = %lld, local trigger time = %lld, difference = %lld\n", tcb_trig_time, local_trig_time, diff_time);
    printf("-----------------------------------------------------------------------\n");
    */
    // read waveform
    fread(adc1, 2, 32736, fp1);
    fread(adc2, 2, 32736, fp2);
    //if (evt<1200) continue; 
    // fill waveform for channel to plotgecit 
    for( i = 0 ; i < 64 ; i ++)
    {
      max[i]=0;
      min[i]=4096;
       //plot[i]->Reset();
    }
    
    for (i = 1; i < 1000; i++) {
      for( j = 0; j < 64 ; j ++) {
//		if(j==21 || j == 22 || j ==23) cout << j << " th channel : " << adc[i * 32 + j]<< endl;
         if (j<32){ 
           if(max[j]<adc1[i*32+j]) max[j]=adc1[i*32+j];
           if(min[j]>adc1[i*32+j]) min[j]=adc1[i*32+j];
           //plot[j]->Fill(i,(double) adc1[i * 32 + j]/nevt);
         }
         else {
           if(max[j]<adc2[i*32+j-32]) max[j]=adc2[i*32+j-32];
           if(min[j]>adc2[i*32+j-32]) min[j]=adc2[i*32+j-32];

         }//plot[j]->Fill(i,(double) adc2[i * 32 + j-32]/nevt);
      }
    }
    for (i=0;i<64;i++){
      plot[i]->Fill(max[i]-min[i]);
    }
    //c1->Modified();
    //c1->Update();
    //c1->SaveAs(filename+"_"+condition+Form("_AllchWave_evtNum%d.png", evt));      
    //c1->SaveAs(filename+Form("_AllchWave_evtNum%d.png", evt));      
    // c1->SaveAs("test.png");
    //printf("Continue? ");
    //scanf("%d", &cont);
    
   // if (cont == 0)
     // evt = nevt;
//      if(evt == ndraw) evt = nevt;
  }
  int plot_index;
  double PTP_ch[64];
  double PTP_std[64];
  double PTP_mean[64];
  for(i =0; i<64;i++){   
     if ((i+1)%2!=0) plot_index = (i+1)/2 + 1 + 8 * ((i+1)/16)-1;
     else plot_index = (i+1)/2 + 8 * (i/16+1)-1;
     c1->cd(plot_index+1);
     printf("i is %d | plot index is %d\n",i+1,plot_index);
     //plot[i]->GetYaxis()->SetRangeUser(min,max);
     plot[i]->GetYaxis()->SetLabelSize(0.05);
     plot[i]->Draw("hist");
     PTP_ch[i]=i+1;
     PTP_mean[i]=plot[i]->GetMean();
     PTP_std[i]=plot[i]->GetStdDev();

  }
 // c1->SaveAs("test.png");
 //
  TCanvas *c2 = new TCanvas("c2","test");
  TGraphErrors *ptp_g = new TGraphErrors(64,PTP_ch,PTP_mean,0,PTP_std);
  ptp_g->SetTitle(";ch;peak to peak (ADC)");
  ptp_g->SetMarkerStyle(21);
  ptp_g->Draw("ap");
  c2->SetGrid();
  
  fclose(fp1);
  fclose(fp2);
  c1->Write();
  c2->Write();
  test->Close();
  return 0;
}

