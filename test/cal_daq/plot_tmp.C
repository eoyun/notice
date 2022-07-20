#include <stdio.h>

int plot_tmp()
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
  int cont;

  // get channel to plot, channel = 1 ~ 32
  printf("Channel to plot(1~32) : ");
  scanf("%d", &channel);
  if (channel < 1)
    ch_to_plot = 0;
  else if (channel > 32)
    ch_to_plot = 31;
  else
    ch_to_plot = channel - 1;
    
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 1600, 1000);
  c1->Divide(8,4,0.01,0.01);
  TH1F *plot1 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot2 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot3 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot4 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot5 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot6 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot7 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot8 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot9 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot10 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot11 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot12 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot13 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot14 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot15 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot16 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot17 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot18 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot19 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot20 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot21 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot22 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot23 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot24 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot25 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot26 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot27 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot28 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot29 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot30 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot31 = new TH1F("plot", "Waveform", 1000, 0, 1000);
  TH1F *plot32 = new TH1F("plot", "Waveform", 1000, 0, 1000);

  plot1->SetStats(0);
  plot2->SetStats(0);
  plot3->SetStats(0);
  plot4->SetStats(0);
  plot5->SetStats(0);
  plot6->SetStats(0);
  plot7->SetStats(0);
  plot8->SetStats(0);
  plot9->SetStats(0);
  plot10->SetStats(0);
  plot11->SetStats(0);
  plot12->SetStats(0);
  plot13->SetStats(0);
  plot14->SetStats(0);
  plot15->SetStats(0);
  plot16->SetStats(0);
  plot17->SetStats(0);
  plot18->SetStats(0);
  plot19->SetStats(0);
  plot20->SetStats(0);
  plot21->SetStats(0);
  plot22->SetStats(0);
  plot23->SetStats(0);
  plot24->SetStats(0);
  plot25->SetStats(0);
  plot26->SetStats(0);
  plot27->SetStats(0);
  plot28->SetStats(0);
  plot29->SetStats(0);
  plot30->SetStats(0);
  plot31->SetStats(0);
  plot32->SetStats(0);

  // get # of events in file
  fp = fopen("cal.dat", "rb");
  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  fclose(fp);
  nevt = file_size / 65536;
  
  printf("-----------------------------------------------------------------------\n");
  fp = fopen("cal.dat", "rb");

  for (evt = 0; evt < nevt; evt++) {
    // read header
    fread(data, 1, 64, fp);
    
    // data length
    data_length = data[0] & 0xFF;
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
    
    // read waveform
    fread(adc, 2, 32736, fp);
    
    // fill waveform for channel to plotgecit 
    plot1->Reset();
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
    
    for (i = 0; i < 1000; i++) {
      plot1->Fill(i, adc[i * 32 + 32 + 0]);
      plot2->Fill(i, adc[i * 32 + 32 + 1]);
      plot3->Fill(i, adc[i * 32 + 32 + 2]);
      plot4->Fill(i, adc[i * 32 + 32 + 3]);
      plot5->Fill(i, adc[i * 32 + 32 + 4]);
      plot6->Fill(i, adc[i * 32 + 32 + 5]);
      plot7->Fill(i, adc[i * 32 + 32 + 6]);
      plot8->Fill(i, adc[i * 32 + 32 + 7]);
      plot9->Fill(i, adc[i * 32 + 32 + 8]);
      plot10->Fill(i, adc[i * 32 + 32 + 9]);
      plot11->Fill(i, adc[i * 32 + 32 + 10]);
      plot12->Fill(i, adc[i * 32 + 32 + 11]);
      plot13->Fill(i, adc[i * 32 + 32 + 12]);
      plot14->Fill(i, adc[i * 32 + 32 + 13]);
      plot15->Fill(i, adc[i * 32 + 32 + 14]);
      plot16->Fill(i, adc[i * 32 + 32 + 15]);
      plot17->Fill(i, adc[i * 32 + 32 + 16]);
      plot18->Fill(i, adc[i * 32 + 32 + 17]);
      plot19->Fill(i, adc[i * 32 + 32 + 18]);
      plot20->Fill(i, adc[i * 32 + 32 + 19]);
      plot21->Fill(i, adc[i * 32 + 32 + 20]);
      plot22->Fill(i, adc[i * 32 + 32 + 21]);
      plot23->Fill(i, adc[i * 32 + 32 + 22]);
      plot24->Fill(i, adc[i * 32 + 32 + 23]);
      plot25->Fill(i, adc[i * 32 + 32 + 24]);
      plot26->Fill(i, adc[i * 32 + 32 + 25]);
      plot27->Fill(i, adc[i * 32 + 32 + 26]);
      plot28->Fill(i, adc[i * 32 + 32 + 27]);
      plot29->Fill(i, adc[i * 32 + 32 + 28]);
      plot30->Fill(i, adc[i * 32 + 32 + 29]);
      plot31->Fill(i, adc[i * 32 + 32 + 30]);
      plot32->Fill(i, adc[i * 32 + 32 + 31]);

      //if (adc[i * 32 + ch_to_plot] < 3000) printf("abnormal value = %d @ %d\n", adc[i * 32 + ch_to_plot], evt);
    }
    //plot->GetXaxis()->SetRangeUser(0,1000);
    //TH1F* plot_copy= (TH1F*)plot->Clone();
    int min =3400;
    int max = 4096;
    c1->cd(1);
//    plot1->GetYaxis()->SetRangeUser(min,max);
    plot1->Draw("hist");

    c1->cd(2);
//    plot2->GetYaxis()->SetRangeUser(min,max);
    plot2->Draw("hist");    
    c1->cd(3);
//    plot3->GetYaxis()->SetRangeUser(min,max);
    plot3->Draw("hist");    
    c1->cd(4);
//    plot4->GetYaxis()->SetRangeUser(min,max);
    plot4->Draw("hist");    
    c1->cd(5);
//    plot5->GetYaxis()->SetRangeUser(min,max);
    plot5->Draw("hist");    
    c1->cd(6);
//    plot6->GetYaxis()->SetRangeUser(min,max);
    plot6->Draw("hist");    
    c1->cd(7);
//    plot7->GetYaxis()->SetRangeUser(min,max);
    plot7->Draw("hist");    
    c1->cd(8);
//    plot8->GetYaxis()->SetRangeUser(min,max);
    plot8->Draw("hist");    
    c1->cd(9);
//    plot9->GetYaxis()->SetRangeUser(min,max);
    plot9->Draw("hist");    
    c1->cd(10);
//    plot10->GetYaxis()->SetRangeUser(min,max);
    plot10->Draw("hist");    
    c1->cd(11);
//    plot11->GetYaxis()->SetRangeUser(min,max);
    plot11->Draw("hist");    
    c1->cd(12);
//    plot12->GetYaxis()->SetRangeUser(min,max);
    plot12->Draw("hist");    
    c1->cd(13);
//    plot13->GetYaxis()->SetRangeUser(min,max);
    plot13->Draw("hist");    
    c1->cd(14);
//    plot14->GetYaxis()->SetRangeUser(min,max);
    plot14->Draw("hist");    
    c1->cd(15);
//    plot15->GetYaxis()->SetRangeUser(min,max);
    plot15->Draw("hist");    
    c1->cd(16);
 //   plot16->GetYaxis()->SetRangeUser(min,max);
    plot16->Draw("hist");    
    c1->cd(17);
//    plot17->GetYaxis()->SetRangeUser(min,max);
    plot17->Draw("hist");    
    c1->cd(18);
//    plot18->GetYaxis()->SetRangeUser(min,max);
    plot18->Draw("hist");    
    c1->cd(19);
//    plot19->GetYaxis()->SetRangeUser(min,max);
    plot19->Draw("hist");    
    c1->cd(20);
//    plot20->GetYaxis()->SetRangeUser(min,max);
    plot20->Draw("hist");    
    c1->cd(21);
//    plot21->GetYaxis()->SetRangeUser(min,max);
    plot21->Draw("hist");    
    c1->cd(22);
//    plot22->GetYaxis()->SetRangeUser(min,max);
    plot22->Draw("hist");    
    c1->cd(23);
 //   plot23->GetYaxis()->SetRangeUser(min,max);
    plot23->Draw("hist");    
    c1->cd(24);
 //   plot24->GetYaxis()->SetRangeUser(min,max);
    plot24->Draw("hist");    
    c1->cd(25);
 //   plot25->GetYaxis()->SetRangeUser(min,max);
    plot25->Draw("hist");    
    c1->cd(26);
 //   plot26->GetYaxis()->SetRangeUser(min,max);
    plot26->Draw("hist");    
    c1->cd(27);
 //   plot27->GetYaxis()->SetRangeUser(min,max);
    plot27->Draw("hist");    
    c1->cd(28);
 //   plot28->GetYaxis()->SetRangeUser(min,max);
    plot28->Draw("hist");    
    c1->cd(29);
 //   plot29->GetYaxis()->SetRangeUser(min,max);
    plot29->Draw("hist");    
    c1->cd(30);
 //   plot30->GetYaxis()->SetRangeUser(min,max);
    plot30->Draw("hist");    
    c1->cd(31);
 //   plot31->GetYaxis()->SetRangeUser(min,max);
    plot31->Draw("hist");    
    c1->cd(32);
 //   plot32->GetYaxis()->SetRangeUser(min,max);
    plot32->Draw("hist");    
    //plot_copy->Rebin(10);
    //plot_copy->GetYaxis()->SetLabelOffset(0.25);
//    plot_copy->Draw("hist");
    c1->Modified();
    c1->Update();
      
    printf("Continue? ");
    scanf("%d", &cont);
    
    if (cont == 0)
      evt = nevt;
  }

  fclose(fp);

  return 0;
}

