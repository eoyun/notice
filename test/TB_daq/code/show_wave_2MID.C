#include <stdio.h>

// int show_wave(const TString filename)
int show_wave_2MID(const int runnum, const int Mid1, const int Mid2, const int channel, const int min, const int max)
{
  //int channel;
  int ch_to_plot;
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
  
  // get channel to plot, channel = 1 ~ 32
  //printf("Channel to plot(1~32) : ");
  //scanf("%d", &channel);
  if (channel < 1)
    ch_to_plot = 0;
  else if (channel > 32)
    ch_to_plot = 31;
  else
    ch_to_plot = channel - 1;
    
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 800);
  c1->Divide(1,2);
  TH1F *plot1 = new TH1F("plot1", "Waveform", 1023, 0, 1023); 
  TH1F *plot2 = new TH1F("plot2", "Waveform", 1023, 0, 1023); 
  plot1->SetStats(0);
  plot2->SetStats(0);
  // get # of events in file
  //sprintf(filename,"cal_wave_1.dat");
  //sprintf(filename,"/Volumes/HDD_16TB_3/HDD/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid,runnum,Mid);
  sprintf(filename1,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid1,runnum,Mid1);
  sprintf(filename2,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,Mid2,runnum,Mid2);
  printf("%s\n",filename1);
  fp1 = fopen(filename1, "rb");
  fseek(fp1, 0L, SEEK_END);
  printf("test\n");
  file_size = ftell(fp1);
  fclose(fp1);
  nevt = file_size / 65536;
  //nevt=10000;
  printf("-----------------------------------------------------------------------\n");
  fp1 = fopen(filename1, "rb");
  fp2 = fopen(filename2, "rb");
//  fp = fopen("cal_sipm_pmt_10k.dat", "rb");

  //btcb_trig_time = 0;

  for (evt = 0; evt < nevt; evt++) {
    
    // read header
    //if (evt!=106||evt!=404||evt!=1339||evt!=1453||evt!=1855) continue;
    //if (evt!=evtnum) continue;
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
    // if (evt%10!=0) continue;
    /*if (evt<minevt) {
      printf("%d evt is skipped",evt);
      continue;}*/
    //printf("evt num is %d\n",evt);
    //if(evt<1200) continue;
    // fill waveform for channel to plotgecit 
    plot1->Reset();
    plot2->Reset();
    for (i = 0; i < 1023; i++) {
      plot1->Fill(i, adc1[i * 32 + ch_to_plot]);
      plot2->Fill(i, adc2[i * 32 + ch_to_plot]);
    }
    plot1->GetYaxis()->SetRangeUser(min,max);
    plot2->GetYaxis()->SetRangeUser(min,max);
    c1->cd(1);
    plot1->Draw("hist");
    c1->cd(2);
    plot2->Draw("hist");
    
    //sprintf(pngname,"wave_%d.png",evt);
    //c1->SaveAs(filename+Form(("_%d.png"),evt));
    c1->Modified();
    c1->Update();
    
    printf("Continue?");
    scanf("%d", &cont);

    if ( cont == 0 )
      evt = nevt;
    // usleep(100000); 
    //if (evt== maxevt) break;
  }

  fclose(fp1);
  fclose(fp2);

  return 0;
}

