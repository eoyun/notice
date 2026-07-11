#include <stdio.h>

//int plot_waveform_32ch(const TString filename, const int min, const int max, const TString condition)
int peakToPeak(const int runnum, const int Mid)
{
  char filename[100];
  int channel;
  int ch_to_plot;
  FILE *fp;
  int file_size;
  int nevt;
  int ndraw;
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
  int max_index[32];
  int min_index[32];
  int max[32];
  int min[32];
  ndraw = 10;
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
    
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 900, 500);
  c1->Divide(8,4,0.001,0.001);
  gPad->SetLeftMargin(0);
  gPad->SetRightMargin(0);
  gPad->SetBottomMargin(0);
  gPad->SetTopMargin(0);

  TH1F *plot[32];
  for( i = 0 ; i < 32 ; i ++)
  {
     plot[i] = new TH1F(Form("plot%d",i+1), Form("peaktopeak ch%d", i+1), 4096,0,4096);
     plot[i]->SetStats(0);  
  }

  // get # of events in file
  sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid,runnum,Mid);
  fp = fopen(filename, "rb");
  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  fclose(fp);
  nevt = file_size / 65536;
  
  printf("-----------------------------------------------------------------------\n");
  fp = fopen(filename, "rb");

  for (evt = 0; evt < nevt; evt++) {
    // read header
    fread(data, 1, 64, fp);
    // read waveform
    fread(adc, 2, 32736, fp);
    //if (evt<1200) continue; 
    // fill waveform for channel to plotgecit 
    for( i = 0 ; i < 32 ; i ++)
    {
      max_index[i]=0;
      min_index[i]=0;
      max[i]=0;
      min[i]=4096;
      // plot[i]->Reset();
    }
    
    for (i = 1; i < 1000; i++) {
      for( j = 0; j < 32 ; j ++) {
//		if(j==21 || j == 22 || j ==23) cout << j << " th channel : " << adc[i * 32 + j]<< endl;
        if(adc[i*32+j]>max[j]){
          max_index[j]=i;
          max[j]=adc[i*32+j];
        } 
        if(adc[i*32+j]<min[j]){
          min_index[j]=i;
          min[j]=adc[i*32+j];
        } 
        //plot[j]->Fill(i, adc[i * 32 + j]);
      }
    }
   for (i=0;i<32;i++){
     plot[i]->Fill(max[i]-min[i]);
   } 
    //c1->SaveAs(filename+"_"+condition+Form("_AllchWave_evtNum%d.png", evt));      
    //c1->SaveAs(filename+Form("_AllchWave_evtNum%d.png", evt));      
    // c1->SaveAs("test.png");
    /*printf("Continue? ");
    scanf("%d", &cont);
    if (evt==0) c1->SaveAs(Form("evt_range_%d_%d.png",evt,Mid));
    if (cont == 0)
      evt = nevt;*/
//      if(evt == ndraw) evt = nevt;
  }
  double PTP_std[32];
  double PTP_mean[32];
  double PTP_ch[32];

  for(i =0; i<32;i++){   
     c1->cd(i+1);
     //plot[i]->GetYaxis()->SetRangeUser(min,max);
     plot[i]->GetYaxis()->SetLabelSize(0.05);
     plot[i]->Draw("hist");
     PTP_ch[i]=i+1;
     PTP_mean[i]=plot[i]->GetMean();
     PTP_std[i]=plot[i]->GetStdDev();
     
  }
    //c1->Modified();
    //c1->Update();
// TH1D* ch = new TH1D("test",";ch;peak to peak(ADC)",32,0,32);
// for(i=0;i<32;i++){
//   plot
// }

  c1->SaveAs(Form("ptp_bych_%d.png",runnum));

  TCanvas *c2 = new TCanvas();
  c2->cd();
  TGraphErrors* ptp = new TGraphErrors(32,PTP_ch,PTP_mean,0,PTP_std);
  ptp->SetTitle(";ch;peak to peak (ADC)");
  ptp->SetMarkerStyle(21);
  ptp->Draw("ap");
  c2->SetGrid();
  c2->SaveAs(Form("ptp_%d.png",runnum));
  fclose(fp);

  return 0;
}

