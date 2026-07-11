#include <stdio.h>

// int show_wave(const TString filename)
int time_wave_scan(const int runnum, const int Mid1,const int Mid2, const int channel1,const int channel2,const int risetime, const int pulsewidth, const double fraction)
{
  //int channel;
  int ch_to_plot1;
  int ch_to_plot2;
  FILE *fp1;
  FILE *fp2;
  long long file_size;
  int nevt;
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
  int cont;
  char filename1[200];
  char filename2[200];
  char pngname[100];
  int min, min_index;
  int timing_fast;
  int fraction_index;
  int remain;
  int timing1, timing2;
  // get channel to plot, channel = 1 ~ 32
  //printf("Channel to plot(1~32) : ");
  //scanf("%d", &channel);
  if (channel1 < 1)
    ch_to_plot1 = 0;
  else if (channel1 > 32)
    ch_to_plot1 = 31;
  else
    ch_to_plot1 = channel1 - 1;
  if (channel2 < 1)
    ch_to_plot2 = 0;
  else if (channel2 > 32)
    ch_to_plot2 = 31;
  else
    ch_to_plot2 = channel2 - 1;
    
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 500);
  TH1F *plot1 = new TH1F("wave_plot1", ";1 bin = 12.5 ps; entry", 1023,0,1023); 
  TH1F *plot2 = new TH1F("wave_plot2", ";1 bin = 12.5 ps; entry", 1000,0,16000); 
  TH1F *plot_diff = new TH1F("diff", "difference;1 bin = 12.5 ps; entry", 1000,0, 16000); 
  plot1->SetStats(0);
  plot2->SetStats(0);
  plot_diff->SetStats(0);
  // get # of events in file
 // sprintf(filename,"cal_wave_1.dat");
//  TFile *fp_root = new TFile(Form("./pngs/timing_wave_%d.root",runnum),"recreate");
  sprintf(filename1,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid1,runnum,Mid1);
  sprintf(filename2,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid2,runnum,Mid2);
  //sprintf(filename,"./Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid,runnum,Mid);
  //sprintf(filename_fast,"./Run_%d/Run_%d_Fast/Run_%d_Fast_MID_%d/Run_%d_Fast_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid,runnum,Mid);
  printf("%s\n",filename1);
  printf("%s\n",filename2);

  fp1 = fopen(filename1, "rb");
  fseek(fp1, 0L, SEEK_END);
  file_size = ftell(fp1);
  fclose(fp1);
  nevt = file_size / 65536;
  
  printf("hello\n");
  fp1 = fopen(filename1, "rb");
  fp2 = fopen(filename2, "rb");
//  fp = fopen("cal_sipm_pmt_10k.dat", "rb");

  //btcb_trig_time = 0;

  for (evt = 0; evt < nevt; evt++) {
    // read header
    //if (evt!=106||evt!=404||evt!=1339||evt!=1453||evt!=1855) continue;
    //if (evt!=evtnum) continue;
    fread(data1, 1, 64, fp1);
  //printf("-----------------------------------------------------------------------\n");
    fread(data2, 1, 64, fp2);
    /*
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
*/    
    // read waveform
    fread(adc1, 2, 32736, fp1);
    fread(adc2, 2, 32736, fp2);
    //printf("evt num is %d\n",evt);
    //if(evt<10000) continue;
    // fill waveform for channel to plotgecit 
    min =4096;
    for (i = 0; i < 1023; i++) {
      if (min>adc1[i*32+ch_to_plot1]) {
        min = adc1[i*32+ch_to_plot1];
	      min_index = i;
      }
    }
    double ped = 0.;
    for (i=min_index-risetime-pulsewidth;i<min_index;i++){
      if(i<min_index-risetime){
        ped+=(double)adc1[i*32+ch_to_plot1]/pulsewidth;
      } else {
        if ((ped-adc1[min_index*32+ch_to_plot1])*fraction<(ped-adc1[i*32+ch_to_plot1])) {
          fraction_index = i;
          break;
        }
      }

    }
    remain = 16*((ped-adc1[min_index*32+ch_to_plot1])*fraction-ped+adc1[fraction_index*32+ch_to_plot1])/(adc1[(fraction_index-1)*32+ch_to_plot1]-adc1[fraction_index*32+ch_to_plot1]);
    //fread(timing,2,96,fp_fast);
    //timing_fast = timing[ch_to_plot*3+2] & 0xFFFF;
    //plot_fast->Fill(timing_fast);
    timing1 = fraction_index*16-remain;
    //if(evt<10000) continue;
    // fill waveform for channel to plotgecit 
    min =4096;
    for (i = 0; i < 1023; i++) {
      if (min>adc2[i*32+ch_to_plot2]) {
        min = adc2[i*32+ch_to_plot2];
	      min_index = i;
      }
    }
    ped = 0.;
    for (i=min_index-risetime-pulsewidth;i<min_index;i++){
      if(i<min_index-risetime){
        ped+=(double)adc2[i*32+ch_to_plot2]/pulsewidth;
      } else {
        if ((ped-adc2[min_index*32+ch_to_plot2])*fraction<(ped-adc2[i*32+ch_to_plot2])) {
          fraction_index = i;
          break;
        }
      }

    }

    remain = 16*((ped-adc2[min_index*32+ch_to_plot2])*fraction-ped+adc2[fraction_index*32+ch_to_plot2])/(adc2[(fraction_index-1)*32+ch_to_plot2]-adc2[fraction_index*32+ch_to_plot2]);

    timing2 = fraction_index*16-remain;

    if(timing2-timing1<107) continue;
    plot1->Reset();
    for (i=0;i<1023;i++){
      plot1->Fill(i,adc1[i*32+ch_to_plot1]);
    }
    plot1->Draw("hist");
    scanf("%d",&cont);
    //printf("%d %d %d %d\n",timing1,timing2,evt,nevt);
    //plot1->Fill(timing1);
    //plot2->Fill(timing2);
    //plot_diff->Fill(timing2-timing1);
    //sprintf(pngname,"wave_%d.png",evt);
    //c1->SaveAs(filename+Form(("_%d.png"),evt));
    c1->Modified();
    c1->Update();
      
    if(cont==0)break;
  }
  //TCanvas *c = new TCanvas();
  //c->Divide(1,3);
  /*TPad *p1 = new TPad("p1","",0,0.3,1,1);
  p1->SetBottomMargin(0.01);
  p1->Draw();
  TPad *p2 = new TPad("p2","",0,0,1,0.3);
  p2->SetTopMargin(0.03);
  p2->SetBottomMargin(0.3);
  p2->Draw();
  p1->cd();
  plot->SetLineColor(kRed);
  plot_fast->SetMarkerColor(kBlack);
  plot_fast->SetMarkerStyle(21);
  plot->Draw("hist");
  plot_fast->Draw("same&p");
  auto legend = new TLegend(0.4,0.3,0.6,0.5);
  legend->AddEntry(plot,"waveform","l");
  legend->AddEntry(plot_fast,"fast","p");
  legend->SetFillStyle(0);
  legend->Draw();
  TH1D* r_wave = (TH1D*) plot->Clone("ratio");
  r_wave->Divide(plot_fast);
  r_wave->SetStats(0);
  r_wave->SetTitle("");
	r_wave->GetYaxis()->SetTitle("wave/fast");
 	r_wave->GetYaxis()->SetTitleSize(0.12);
	r_wave->GetYaxis()->SetTitleOffset(0.4);
	r_wave->GetXaxis()->SetLabelSize(0.1);
	r_wave->GetYaxis()->SetLabelSize(0.1);
	r_wave->GetXaxis()->SetLabelOffset(0.01);
	r_wave->GetYaxis()->SetLabelOffset(0.01);
	r_wave->GetXaxis()->SetTitleSize(0.12);
	r_wave->GetXaxis()->SetTitleOffset(0.75);
	r_wave->SetMarkerStyle(21);
	r_wave->SetMarkerSize(1);
	r_wave->SetLineColor(kBlack);

	r_wave->GetYaxis()->SetRangeUser(0.5,1.5);
	p2->SetGrid();
	p2->cd();
	r_wave->Draw("p");
*/
  //c->cd(1);
  //plot1->Draw("hist");
  //c->cd(2);
  //plot2->Draw("hist");
  //c->cd(3);
  //plot_diff->Draw("hist");
  //plot->Write("hist");
  //plot_fast->Write("hist");
  //plot_diff->Write("hist");
  std::cout<<"1111"<<std::endl;
  fclose(fp1);
  fclose(fp2);
  //fp_root->Close();

  return 0;
}

