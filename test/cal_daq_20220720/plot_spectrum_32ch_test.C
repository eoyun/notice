#include <stdio.h>

int plot_spectrum_32ch_test()
//int plot_spectrum_32ch(const TString filename)
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

  std::vector<double> ch;
  std::vector<double> mean;
  std::vector<double> stddev;
   ch.clear();
   mean.clear();
   stddev.clear();

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
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);
  gStyle->SetStatW(0.4);
  gStyle->SetStatH(0.2);
  gStyle->SetOptStat(111111);
  gStyle->SetFillStyle(0);

  TH1F *plot1 = new TH1F("plot", "Spectrum ch1", 1023, 0, 4096); 
  TH1F *plot2 = new TH1F("plot", "Spectrum ch2", 1023, 0, 4096);
  TH1F *plot3 = new TH1F("plot", "Spectrum ch3", 1023, 0, 4096);
  TH1F *plot4 = new TH1F("plot", "Spectrum ch4", 1023, 0, 4096);
  TH1F *plot5 = new TH1F("plot", "Spectrum ch5", 1023, 0, 4096);
  TH1F *plot6 = new TH1F("plot", "Spectrum ch6", 1023, 0, 4096);
  TH1F *plot7 = new TH1F("plot", "Spectrum ch7", 1023, 0, 4096);
  TH1F *plot8 = new TH1F("plot", "Spectrum ch8", 1023, 0, 4096);
  TH1F *plot9 = new TH1F("plot", "Spectrum ch9", 1023, 0, 4096);
  TH1F *plot10 = new TH1F("plot", "Spectrum ch10", 1023, 0, 4096);
  TH1F *plot11 = new TH1F("plot", "Spectrum ch11", 1023, 0, 4096);
  TH1F *plot12 = new TH1F("plot", "Spectrum ch12", 1023, 0, 4096);
  TH1F *plot13 = new TH1F("plot", "Spectrum ch13", 1023, 0, 4096);
  TH1F *plot14 = new TH1F("plot", "Spectrum ch14", 1023, 0, 4096);
  TH1F *plot15 = new TH1F("plot", "Spectrum ch15", 1023, 0, 4096);
  TH1F *plot16 = new TH1F("plot", "Spectrum ch16", 1023, 0, 4096);
  TH1F *plot17 = new TH1F("plot", "Spectrum ch17", 1023, 0, 4096);
  TH1F *plot18 = new TH1F("plot", "Spectrum ch18", 1023, 0, 4096);
  TH1F *plot19 = new TH1F("plot", "Spectrum ch19", 1023, 0, 4096);
  TH1F *plot20 = new TH1F("plot", "Spectrum ch20", 1023, 0, 4096);
  TH1F *plot21 = new TH1F("plot", "Spectrum ch21", 1023, 0, 4096);
  TH1F *plot22 = new TH1F("plot", "Spectrum ch22", 1023, 0, 4096);
  TH1F *plot23 = new TH1F("plot", "Spectrum ch23", 1023, 0, 4096);
  TH1F *plot24 = new TH1F("plot", "Spectrum ch24", 1023, 0, 4096);
  TH1F *plot25 = new TH1F("plot", "Spectrum ch25", 1023, 0, 4096);
  TH1F *plot26 = new TH1F("plot", "Spectrum ch26", 1023, 0, 4096);
  TH1F *plot27 = new TH1F("plot", "Spectrum ch27", 1023, 0, 4096);
  TH1F *plot28 = new TH1F("plot", "Spectrum ch28", 1023, 0, 4096);
  TH1F *plot29 = new TH1F("plot", "Spectrum ch29", 1023, 0, 4096);
  TH1F *plot30 = new TH1F("plot", "Spectrum ch30", 1023, 0, 4096);
  TH1F *plot31 = new TH1F("plot", "Spectrum ch31", 1023, 0, 4096);
  TH1F *plot32 = new TH1F("plot", "Spectrum ch32", 1023, 0, 4096);
  int rmin = 3500 ;
  int rmax = 4096 ;
  plot1->GetXaxis()->SetRangeUser(rmin,rmax);
  plot1->GetXaxis()->SetRangeUser(rmin,rmax);
  plot2->GetXaxis()->SetRangeUser(rmin,rmax);
  plot3->GetXaxis()->SetRangeUser(rmin,rmax);
  plot4->GetXaxis()->SetRangeUser(rmin,rmax);
  plot5->GetXaxis()->SetRangeUser(rmin,rmax);
  plot6->GetXaxis()->SetRangeUser(rmin,rmax);
  plot7->GetXaxis()->SetRangeUser(rmin,rmax);
  plot8->GetXaxis()->SetRangeUser(rmin,rmax);
  plot9->GetXaxis()->SetRangeUser(rmin,rmax);
  plot10->GetXaxis()->SetRangeUser(rmin,rmax);
  plot11->GetXaxis()->SetRangeUser(rmin,rmax);
  plot12->GetXaxis()->SetRangeUser(rmin,rmax);
  plot13->GetXaxis()->SetRangeUser(rmin,rmax);
  plot14->GetXaxis()->SetRangeUser(rmin,rmax);
  plot15->GetXaxis()->SetRangeUser(rmin,rmax);
  plot16->GetXaxis()->SetRangeUser(rmin,rmax);
  plot17->GetXaxis()->SetRangeUser(rmin,rmax);
  plot18->GetXaxis()->SetRangeUser(rmin,rmax);
  plot19->GetXaxis()->SetRangeUser(rmin,rmax); // for mid11 3700 -> 3500 ADC
  plot20->GetXaxis()->SetRangeUser(rmin,rmax);
  plot21->GetXaxis()->SetRangeUser(rmin,rmax);
  plot22->GetXaxis()->SetRangeUser(rmin,rmax);
  plot23->GetXaxis()->SetRangeUser(rmin,rmax);
  plot24->GetXaxis()->SetRangeUser(rmin,rmax);
  plot25->GetXaxis()->SetRangeUser(rmin,rmax);
  plot26->GetXaxis()->SetRangeUser(rmin,rmax);
  plot27->GetXaxis()->SetRangeUser(rmin,rmax);
  plot28->GetXaxis()->SetRangeUser(rmin,rmax);
  plot29->GetXaxis()->SetRangeUser(rmin,rmax);
  plot30->GetXaxis()->SetRangeUser(rmin,rmax);
  plot31->GetXaxis()->SetRangeUser(rmin,rmax);
  plot32->GetXaxis()->SetRangeUser(rmin,rmax);
//
//  plot1->SetStats(0);
//  plot2->SetStats(0);
//  plot3->SetStats(0);
//  plot4->SetStats(0);
//  plot5->SetStats(0);
//  plot6->SetStats(0);
//  plot7->SetStats(0);
//  plot8->SetStats(0);
//  plot9->SetStats(0);
//  plot10->SetStats(0);
//  plot11->SetStats(0);
//  plot12->SetStats(0);
//  plot13->SetStats(0);
//  plot14->SetStats(0);
//  plot15->SetStats(0);
//  plot16->SetStats(0);
//  plot17->SetStats(0);
//  plot18->SetStats(0);
//  plot19->SetStats(0);
//  plot20->SetStats(0);
//  plot21->SetStats(0);
//  plot22->SetStats(0);
//  plot23->SetStats(0);
//  plot24->SetStats(0);
//  plot25->SetStats(0);
//  plot26->SetStats(0);
//  plot27->SetStats(0);
//  plot28->SetStats(0);
//  plot29->SetStats(0);
//  plot30->SetStats(0);
//  plot31->SetStats(0);
//  plot32->SetStats(0);
//
  // get # of events in file
  fp = fopen("./data/cal_wave_8_130.dat", "rb");
  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  fclose(fp);
  //nevt = file_size / 65536;
  nevt = 1000;
  
  printf("-----------------------------------------------------------------------\n");
  //fp = fopen(filename, "rb");
  fp = fopen("./data/cal_wave_8_130.dat", "rb");

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
    
//    // trigger type
//    tcb_trig_type = data[6] & 0xFF;
//    
//    // TCB trigger #
//    tcb_trig_number = data[7] & 0xFF;
//    itmp = data[8] & 0xFF;
//    itmp = itmp << 8;
//    tcb_trig_number = tcb_trig_number + itmp;
//    itmp = data[9] & 0xFF;
//    itmp = itmp << 16;
//    tcb_trig_number = tcb_trig_number + itmp;
//    itmp = data[10] & 0xFF;
//    itmp = itmp << 24;
//    tcb_trig_number = tcb_trig_number + itmp;
//
//    // TCB trigger time
//    fine_time = data[11] & 0xFF;
//    fine_time = fine_time * 11;     // actually * (1000 / 90)
//    coarse_time = data[12] & 0xFF;
//    ltmp = data[13] & 0xFF;
//    ltmp = ltmp << 8;
//    coarse_time = coarse_time + ltmp;
//    ltmp = data[14] & 0xFF;
//    ltmp = ltmp << 16;
//    coarse_time = coarse_time + ltmp;
//    ltmp = data[15] & 0xFF;
//    ltmp = ltmp << 24;
//    coarse_time = coarse_time + ltmp;
//    ltmp = data[16] & 0xFF;
//    ltmp = ltmp << 32;
//    coarse_time = coarse_time + ltmp;
//    ltmp = data[17] & 0xFF;
//    ltmp = ltmp << 40;
//    coarse_time = coarse_time + ltmp;
//    coarse_time = coarse_time * 1000;   // get ns
//    tcb_trig_time = fine_time + coarse_time;
//    
//    // mid
//    mid = data[18] & 0xFF;
//
//    // local trigger #
//    local_trig_number = data[19] & 0xFF;
//    itmp = data[20] & 0xFF;
//    itmp = itmp << 8;
//    local_trig_number = local_trig_number + itmp;
//    itmp = data[21] & 0xFF;
//    itmp = itmp << 16;
//    local_trig_number = local_trig_number + itmp;
//    itmp = data[22] & 0xFF;
//    itmp = itmp << 24;
//    local_trig_number = local_trig_number + itmp;
//
//    // local trigger #
//    local_trigger_pattern = data[23] & 0xFF;
//    itmp = data[24] & 0xFF;
//    itmp = itmp << 8;
//    local_trigger_pattern = local_trigger_pattern + itmp;
//    itmp = data[25] & 0xFF;
//    itmp = itmp << 16;
//    local_trigger_pattern = local_trigger_pattern + itmp;
//    itmp = data[26] & 0xFF;
//    itmp = itmp << 24;
//    local_trigger_pattern = local_trigger_pattern + itmp;
//
//    // local trigger time
//    fine_time = data[27] & 0xFF;
//    fine_time = fine_time * 11;     // actually * (1000 / 90)
//    coarse_time = data[28] & 0xFF;
//    ltmp = data[29] & 0xFF;
//    ltmp = ltmp << 8;
//    coarse_time = coarse_time + ltmp;
//    ltmp = data[30] & 0xFF;
//    ltmp = ltmp << 16;
//    coarse_time = coarse_time + ltmp;
//    ltmp = data[31] & 0xFF;
//    ltmp = ltmp << 24;
//    coarse_time = coarse_time + ltmp;
//    ltmp = data[32] & 0xFF;
//    ltmp = ltmp << 32;
//    coarse_time = coarse_time + ltmp;
//    ltmp = data[33] & 0xFF;
//    ltmp = ltmp << 40;
//    coarse_time = coarse_time + ltmp;
//    coarse_time = coarse_time * 1000;   // get ns
//    local_trig_time = fine_time + coarse_time;
//
//    diff_time = local_trig_time - tcb_trig_time;
//    printf("evt = %d, data length = %d, run # = %d, mid = %d\n", evt, data_length, run_number, mid);
//    printf("trigger type = %X, local trigger pattern = %X\n", tcb_trig_type, local_trigger_pattern);
//    printf("TCB trigger # = %d, local trigger # = %d\n", tcb_trig_number, local_trig_number);
//    printf("TCB trigger time = %lld, local trigger time = %lld, difference = %lld\n", tcb_trig_time, local_trig_time, diff_time);
//    printf("-----------------------------------------------------------------------\n");

      if(evt%100==0) cout<< evt << " is done ! " << endl;

    // read waveform
    fread(adc, 2, 32736, fp);
    
    // fill waveform for channel to plotgecit 
    
    for (i = 0; i < 1023; i++) {
      plot1->Fill(adc[i * 32 + 0]);
      plot2->Fill(adc[i * 32 + 1]);
      plot3->Fill(adc[i * 32 + 2]);
      plot4->Fill(adc[i * 32 + 3]);
      plot5->Fill(adc[i * 32 + 4]);
      plot6->Fill(adc[i * 32 + 5]);
      plot7->Fill(adc[i * 32 + 6]);
      plot8->Fill(adc[i * 32 + 7]);
      plot9->Fill(adc[i * 32 + 8]);
      plot10->Fill( adc[i * 32 + 9]);
      plot11->Fill( adc[i * 32 + 10]);
      plot12->Fill( adc[i * 32 + 11]);
      plot13->Fill( adc[i * 32 + 12]);
      plot14->Fill( adc[i * 32 + 13]);
      plot15->Fill( adc[i * 32 + 14]);
      plot16->Fill( adc[i * 32 + 15]);
      plot17->Fill( adc[i * 32 + 16]);
      plot18->Fill( adc[i * 32 + 17]);
      plot19->Fill( adc[i * 32 + 18]);
      plot20->Fill( adc[i * 32 + 19]);
      plot21->Fill( adc[i * 32 + 20]);
      plot22->Fill( adc[i * 32 + 21]);
      plot23->Fill( adc[i * 32 + 22]);
      plot24->Fill( adc[i * 32 + 23]);
      plot25->Fill( adc[i * 32 + 24]);
      plot26->Fill( adc[i * 32 + 25]);
      plot27->Fill( adc[i * 32 + 26]);
      plot28->Fill( adc[i * 32 + 27]);
      plot29->Fill( adc[i * 32 + 28]);
      plot30->Fill( adc[i * 32 + 29]);
      plot31->Fill( adc[i * 32 + 30]);
      plot32->Fill( adc[i * 32 + 31]);
      //if (adc[i * 32 + ch_to_plot] < 3000) printf("abnormal value = %d @ %d\n", adc[i * 32 + ch_to_plot], evt);
    }
    if (cont == 0)
      evt = nevt;
  }
    int min1 =plot1->GetMean()-plot1->GetStdDev()*5;
    int max1 = plot1->GetMean()+plot1->GetStdDev()*5;
    plot1->GetXaxis()->SetRangeUser(min1,max1);
    c1->cd(1);
    plot1->Draw("hist");
    c1->cd(2);
    plot2->Draw("hist");    
    c1->cd(3);
    plot3->Draw("hist");    
    c1->cd(4);
    plot4->Draw("hist");    
    c1->cd(5);
    plot5->Draw("hist");    
    c1->cd(6);
    plot6->Draw("hist");    
    c1->cd(7);
    plot7->Draw("hist");    
    c1->cd(8);
    plot8->Draw("hist");    
    c1->cd(9);
    plot9->Draw("hist");    
    c1->cd(10);
    plot10->Draw("hist");    
    c1->cd(11);
    plot11->Draw("hist");    
    c1->cd(12);
    plot12->Draw("hist");    
    c1->cd(13);
    plot13->Draw("hist");    
    c1->cd(14);
    plot14->Draw("hist");    
    c1->cd(15);
    plot15->Draw("hist");    
    c1->cd(16);
    plot16->Draw("hist");    
    c1->cd(17);
    plot17->Draw("hist");    
    c1->cd(18);
    plot18->Draw("hist");    
    c1->cd(19);
    plot19->Draw("hist");    
    c1->cd(20);
    plot20->Draw("hist");    
    c1->cd(21);
    plot21->Draw("hist");    
    c1->cd(22);
    plot22->Draw("hist");    
    c1->cd(23);
    plot23->Draw("hist");    
    c1->cd(24);
    plot24->Draw("hist");    
    c1->cd(25);
    plot25->Draw("hist");    
    c1->cd(26);
    plot26->Draw("hist");    
    c1->cd(27);
    plot27->Draw("hist");    
    c1->cd(28);
    plot28->Draw("hist");    
    c1->cd(29);
    plot29->Draw("hist");    
    c1->cd(30);
    plot30->Draw("hist");    
    c1->cd(31);
    plot31->Draw("hist");    
    c1->cd(32);
    plot32->Draw("hist");    
    //plot_copy->Rebin(10);
    //plot_copy->GetYaxis()->SetLabelOffset(0.25);
//    plot_copy->Draw("hist");

//    c1->Modified();
//    c1->Update();
    //c1->SaveAs(filename+"_AllchSpectrum.png");      
    //c1->SaveAs(filename+"_AllchSpectrum_SiPMmidConnectCable.png");      
    c1->SaveAs("test_AllchSpectrum_noCable_expsite.png");      

  fclose(fp);

   for( i = 1; i < 33 ; i ++)  ch.push_back(i); //1~32 channel
   mean.push_back(plot1->GetMean());
   mean.push_back(plot2->GetMean());
   mean.push_back(plot3->GetMean());
   mean.push_back(plot4->GetMean());
   mean.push_back(plot5->GetMean());
   mean.push_back(plot6->GetMean());
   mean.push_back(plot7->GetMean());
   mean.push_back(plot8->GetMean());
   mean.push_back(plot9->GetMean());
   mean.push_back(plot10->GetMean());
   mean.push_back(plot11->GetMean());
   mean.push_back(plot12->GetMean());
   mean.push_back(plot13->GetMean());
   mean.push_back(plot14->GetMean());
   mean.push_back(plot15->GetMean());
   mean.push_back(plot16->GetMean());
   mean.push_back(plot17->GetMean());
   mean.push_back(plot18->GetMean());
   mean.push_back(plot19->GetMean());
   mean.push_back(plot20->GetMean());
   mean.push_back(plot21->GetMean());
   mean.push_back(plot22->GetMean());
   mean.push_back(plot23->GetMean());
   mean.push_back(plot24->GetMean());
   mean.push_back(plot25->GetMean());
   mean.push_back(plot26->GetMean());
   mean.push_back(plot27->GetMean());
   mean.push_back(plot28->GetMean());
   mean.push_back(plot29->GetMean());
   mean.push_back(plot30->GetMean());
   mean.push_back(plot31->GetMean());
   mean.push_back(plot32->GetMean());
   stddev.push_back(plot1->GetStdDev());
   stddev.push_back(plot2->GetStdDev());
   stddev.push_back(plot3->GetStdDev());
   stddev.push_back(plot4->GetStdDev());
   stddev.push_back(plot5->GetStdDev());
   stddev.push_back(plot6->GetStdDev());
   stddev.push_back(plot7->GetStdDev());
   stddev.push_back(plot8->GetStdDev());
   stddev.push_back(plot9->GetStdDev());
   stddev.push_back(plot10->GetStdDev());
   stddev.push_back(plot11->GetStdDev());
   stddev.push_back(plot12->GetStdDev());
   stddev.push_back(plot13->GetStdDev());
   stddev.push_back(plot14->GetStdDev());
   stddev.push_back(plot15->GetStdDev());
   stddev.push_back(plot16->GetStdDev());
   stddev.push_back(plot17->GetStdDev());
   stddev.push_back(plot18->GetStdDev());
   stddev.push_back(plot19->GetStdDev());
   stddev.push_back(plot20->GetStdDev());
   stddev.push_back(plot21->GetStdDev());
   stddev.push_back(plot22->GetStdDev());
   stddev.push_back(plot23->GetStdDev());
   stddev.push_back(plot24->GetStdDev());
   stddev.push_back(plot25->GetStdDev());
   stddev.push_back(plot26->GetStdDev());
   stddev.push_back(plot27->GetStdDev());
   stddev.push_back(plot28->GetStdDev());
   stddev.push_back(plot29->GetStdDev());
   stddev.push_back(plot30->GetStdDev());
   stddev.push_back(plot31->GetStdDev());
   stddev.push_back(plot32->GetStdDev());

  TCanvas *c2 = new TCanvas("c2", "c2", 1000, 800);
  TGraphErrors* gr = new TGraphErrors(ch.size(), &(ch[0]), &(mean[0]), 0, &(stddev[0]));
//  TString id(filename(9,10));
//  gr->SetTitle("noise level as a function of channels @Mid "+id);
  gr->GetYaxis()->SetTitle("ADC");
  gr->GetYaxis()->SetTitleOffset(1.5);
  gr->GetXaxis()->SetTitle("Channels");
  gr->SetMarkerStyle(8);
  gr->Draw("AP");

  c2->SaveAs("test_Allch_noiseLevel_noCable_expSite.png");      
  c2->Modified();
  c2->Update();

  return 0;
}

