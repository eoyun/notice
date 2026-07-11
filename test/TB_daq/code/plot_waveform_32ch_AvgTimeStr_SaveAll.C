#include <stdio.h>

//int plot_waveform_32ch_AvgTimeStr(const TString filename, const int min, const int max, const TString condition)
int plot_waveform_32ch_AvgTimeStr_SaveAll(const TString filename, const int min, const int max)
//int plot_waveform_32ch_AvgTimeStr(const TString filename)
{
  int channel;
  int ch_to_plot;
  FILE *fp;
  int file_size;
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

  // get channel to plot, channel = 1 ~ 32
//  printf("Channel to plot(1~32) : ");
//  scanf("%d", &channel);
//  if (channel < 1)
//    ch_to_plot = 0;
//  else if (channel > 32)
//    ch_to_plot = 31;
//  else
//    ch_to_plot = channel - 1;
    
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 1800, 1000);
  c1->Divide(8,4,0.001,0.001);
  gPad->SetLeftMargin(0);
  gPad->SetRightMargin(0);
  gPad->SetBottomMargin(0);
  gPad->SetTopMargin(0);

  TH1F *plot[32];
  for( i = 0 ; i < 32 ; i ++)
  {
     plot[i] = new TH1F(Form("plot%d",i+1), Form("Avg.Time.Str. ch%d", i+1), 1023,0,1023);
     plot[i]->GetXaxis()->SetRangeUser(min,max);  
     plot[i]->SetStats(0);
  }
  TFile *tfile = new TFile(filename+"_AvgTimeStrAllCh_.root","RECREATE");

  // get # of events in file
  fp = fopen(filename, "rb");
  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  fclose(fp);
  nevt = file_size / 65536;
//  nevt = 70000;  
  printf("-----------------------------------------------------------------------\n");
  fp = fopen(filename, "rb");

  for (evt = 0; evt < nevt; evt++) {
    // read header
    fread(data, 1, 64, fp);
    
    // data length
 /*   data_length = data[0] & 0xFF;
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
    fread(adc, 2, 32736, fp);
    
    // fill waveform for channel to plotgecit 
/*    plot1->Reset();
    plot2->Reset();
    plot3->Reset();
    plot4->Reset();
    plot5->Reset();
    plot6->Reset();
    plot7->Reset();
    plot8->Reset();
    plot9->Reset();
    plot10->Reset();
    plot11->Reset();
    plot12->Reset();
    plot13->Reset();
    plot14->Reset();
    plot15->Reset();
    plot16->Reset();
    plot17->Reset();
    plot18->Reset();
    plot19->Reset();
    plot20->Reset();
    plot21->Reset();
    plot22->Reset();
    plot23->Reset();
    plot24->Reset();
    plot25->Reset();
    plot26->Reset();
    plot27->Reset();
    plot28->Reset();
    plot29->Reset();
    plot30->Reset();
    plot31->Reset();
    plot32->Reset();
  */  
    for (i = 0; i < 1023; i++) {
      for(j = 0 ; j < 32 ; j++){
	  plot[j]->Fill(i, adc[i * 32 + j]);
      }
      //if (adc[i * 32 + ch_to_plot] < 3000) printf("abnormal value = %d @ %d\n", adc[i * 32 + ch_to_plot], evt);
    }
    //plot->GetXaxis()->SetRangeUser(0,1000);
    //TH1F* plot_copy= (TH1F*)plot->Clone();
//    int min =3400;
//    int max = 4096;
  for(i =0; i<32;i++){   
     //rmin = plot[i]->GetMean() - 5*plot[i]->GetStdDev();
     //rmax = plot[i]->GetMean() + 5*plot[i]->GetStdDev();

     c1->cd(i+1);
     plot[i]->GetYaxis()->SetLabelSize(0.05);
     plot[i]->Draw("hist");
  }
    
   // if (cont == 0)
     // evt = nevt;
  }
    c1->Modified();
    c1->Update();
    //c1->SaveAs(filename+"_"+condition+"_AllchAvgTimeStr.png");      
    c1->SaveAs(filename+"_AllchAvgTimeStr.png");      
    c1->Write();
	tfile->Close();

  fclose(fp);
  return 0;
}

