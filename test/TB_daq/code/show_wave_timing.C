#include <stdio.h>

// int show_wave(const TString filename)
int show_wave_timing(const int runnum, const int Mid, const int channel,const int risetime, const int pulsewidth, const double fraction)
{
  //int channel;
  int ch_to_plot;
  FILE *fp;
  FILE *fp_fast;
  long long file_size;
  int nevt;
  char data[64];
  char data_fast[64];
  short adc[32736];
  short timing[96];
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
  char filename[200];
  char filename_fast[200];
  char pngname[100];
  int min, min_index;
  int timing_fast;
  int fraction_index;
  int remain;
  // get channel to plot, channel = 1 ~ 32
  //printf("Channel to plot(1~32) : ");
  //scanf("%d", &channel);
  if (channel < 1)
    ch_to_plot = 0;
  else if (channel > 32)
    ch_to_plot = 31;
  else
    ch_to_plot = channel - 1;
    
  //TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 500);
  TH1F *plot = new TH1F("wave", ";1 bin = 12.5 ps; entry", 100, 8100, 9200); 
  TH1F *plot_fast = new TH1F("fast", "fast timing;1 bin = 12.5 ps; entry", 100, 8100, 9200); 
  TH1F *plot_diff = new TH1F("diff", "difference;1 bin = 12.5 ps; entry", 4000, -2000, 2000); 
  plot->SetStats(0);
  plot_fast->SetStats(0);
  plot_diff->SetStats(0);
  // get # of events in file
 // sprintf(filename,"cal_wave_1.dat");
//  TFile *fp_root = new TFile(Form("./pngs/timing_wave_%d.root",runnum),"recreate");
  sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid,runnum,Mid);
  sprintf(filename_fast,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Fast/Run_%d_Fast_MID_%d/Run_%d_Fast_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid,runnum,Mid);
  //sprintf(filename,"./Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid,runnum,Mid);
  //sprintf(filename_fast,"./Run_%d/Run_%d_Fast/Run_%d_Fast_MID_%d/Run_%d_Fast_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid,runnum,Mid);
  printf("%s\n",filename);

  fp_fast = fopen(filename_fast, "rb");
  fseek(fp_fast, 0L, SEEK_END);
  file_size = ftell(fp_fast);
  fclose(fp_fast);
  nevt = file_size / 256;
  
  fp = fopen(filename, "rb");
  fp_fast = fopen(filename_fast, "rb");
//  fp = fopen("cal_sipm_pmt_10k.dat", "rb");

  //btcb_trig_time = 0;

  for (evt = 0; evt < nevt; evt++) {
    // read header
    //if (evt!=106||evt!=404||evt!=1339||evt!=1453||evt!=1855) continue;
    //if (evt!=evtnum) continue;
    fread(data, 1, 64, fp);
  printf("-----------------------------------------------------------------------\n");
    fread(data_fast, 1, 64, fp_fast);
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
    fread(adc, 2, 32736, fp);
    //printf("evt num is %d\n",evt);
    //if(evt<10000) continue;
    // fill waveform for channel to plotgecit 
    min =4096;
    for (i = 0; i < 1023; i++) {
      if (min>adc[i*32+ch_to_plot]) {
        min = adc[i*32+ch_to_plot];
	      min_index = i;
      }
    }
    double ped = 0.;
    for (i=min_index-risetime-pulsewidth;i<min_index;i++){
      if(i<min_index-risetime){
        ped+=(double)adc[i*32+ch_to_plot]/pulsewidth;
      } else {
        if ((ped-adc[min_index*32+ch_to_plot])*fraction<(ped-adc[i*32+ch_to_plot])) {
          fraction_index = i;
          break;
        }
      }

    }
    remain = 16*((ped-adc[min_index*32+ch_to_plot])*fraction-ped+adc[fraction_index*32+ch_to_plot])/(adc[(fraction_index-1)*32+ch_to_plot]-adc[fraction_index*32+ch_to_plot]);
    fread(timing,2,96,fp_fast);
    timing_fast = timing[ch_to_plot*3+2] & 0xFFFF;
    plot_fast->Fill(timing_fast);
    plot->Fill(fraction_index*16-remain);
    plot_diff->Fill(fraction_index*16-remain-timing_fast);
    //sprintf(pngname,"wave_%d.png",evt);
    //c1->SaveAs(filename+Form(("_%d.png"),evt));
      
    
  }
  TCanvas *c = new TCanvas();
  TPad *p1 = new TPad("p1","",0,0.3,1,1);
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

  //plot->Write("hist");
  //plot_fast->Write("hist");
  //plot_diff->Write("hist");
  std::cout<<"1111"<<std::endl;
  fclose(fp);
  fclose(fp_fast);
  //fp_root->Close();

  return 0;
}

