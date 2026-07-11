#include <unistd.h>
#include <stdio.h>
#include "NoticeJBNU_TCB_FOR_YS_SIPM.h"

int main(int argc, char *argv[])
{
  int sid=0;
  int link_status[2];
  int daq_mid[40];
  int num_of_daq;
  int daq;
  int daq_exist;
  int mid[40];
  FILE *fp;
  float hv[2];
  int coin_width[2];
  int multi_thr_daq[2];
  int disc_thr[2][32];
  int gate_width[2];
  int idelay[2];
  int peak_sum_width[2];
  int zero_sup[2];
  int prescale[2];
  float temp[2][2];
  int ch;
  char config_folder_name[256];
  char daq_name[256];
  char var_name[50];
  int lval_i;
  float lval_f;
  unsigned long idaq_setup; 
  FILE *fp_mid;

  if (argc<2){
    //printf("Enter the config name : ");
	//scanf("%s",&config_folder_name);
	  return 0;
  } else {
	  sprintf(config_folder_name,"/Users/yhep/scratch/notice/test/TB_daq/config/%s", argv[1]);
  }

  // init USB
  USB3Init();
  
  // open TCB
  TCBopen(sid);

  // reset TCB
  TCBreset(sid);

  // get link status and ADC mid
  TCBread_LINK_STATUS(sid, link_status);
  TCBread_MID(sid, daq_mid);
  printf("link status = %X %X\n", link_status[1], link_status[0]);
  num_of_daq = 0;
//  for (daq = 0; daq < 32; daq++) {
//    daq_exist = link_status[0] & (1 << daq);
//    if (daq_exist) {
//      mid[num_of_daq] = daq_mid[daq]; 
//      printf("DAQ[%d] is found @%d\n", mid[num_of_daq], daq + 1);
//      num_of_daq = num_of_daq + 1;
//    }
//  }
  for (daq = 0; daq < 8; daq++) {
    daq_exist = link_status[1] & (1 << daq);
    if (daq_exist) {
      mid[num_of_daq] = daq_mid[daq + 32]; 
      printf("JBNU DAQ[%d] is found @%d\n", mid[num_of_daq], daq + 33);
      num_of_daq = num_of_daq + 1;
    }
  }

  // read settings
  if ((access(config_folder_name, 0)) == 0) {
    for(idaq_setup=0;idaq_setup<num_of_daq;idaq_setup++){
	    daq_name[0] ='\0';
	    sprintf(daq_name,"%s/set_JBNU_DAQ_%d.txt",config_folder_name,mid[idaq_setup]);
	    fp = fopen(daq_name,"rt");
      fscanf(fp,"%s",var_name);
      fscanf(fp,"%d",&lval_i);
      coin_width[idaq_setup]=lval_i;
      fscanf(fp,"%s",var_name);
      fscanf(fp,"%f",&lval_f);
      hv[idaq_setup]=lval_f;
      fscanf(fp,"%s",var_name);
      fscanf(fp,"%d",&lval_i);
      multi_thr_daq[idaq_setup]=lval_i;
      fscanf(fp,"%s",var_name);
      fscanf(fp,"%d",&lval_i);
      idelay[idaq_setup]=lval_i;
      fscanf(fp,"%s",var_name);
      fscanf(fp,"%d",&lval_i);
      gate_width[idaq_setup]=lval_i;
      fscanf(fp,"%s",var_name);
      fscanf(fp,"%d",&lval_i);
      peak_sum_width[idaq_setup]=lval_i;
      fscanf(fp,"%s",var_name);
      fscanf(fp,"%d",&lval_i);
      zero_sup[idaq_setup]=lval_i;
      fscanf(fp,"%s",var_name);
      fscanf(fp,"%d",&lval_i);
      prescale[idaq_setup]=lval_i;
      fscanf(fp,"%s",var_name);
      for (ch=0;ch<32;ch++){
        fscanf(fp,"%d",&lval_i);
        disc_thr[idaq_setup][ch]=lval_i;
      }

    } 
  }
  else{
	  return 0;
  }
  fp_mid = fopen("/Users/yhep/scratch/notice/test/TB_daq/turn_on_mid_anc.txt","wt");
  for (idaq_setup=0;idaq_setup<num_of_daq;idaq_setup++){
    fprintf(fp_mid,"%d ",mid[idaq_setup]);
  }
  fclose(fp_mid);
  // write setting
  //TCBwrite_COIN_WIDTH(sid, 0, coin_width);
  //TCBwrite_MULTI_THR(sid, 0, multi_thr_tcb);
  //TCBwrite_PEDESTAL_TRIGGER_INTERVAL(sid, ptrig_interval);
  //TCBwrite_TRIGGER_ENABLE(sid, trig_enable);
  for (daq = 0; daq < num_of_daq; daq++) {
    TCBinit_ADC(sid, mid[daq]);
    TCBinit_DRAM(sid, mid[daq]);
    TCBwrite_HV(sid, mid[daq], 1, hv[daq]);
    TCBwrite_HV(sid, mid[daq], 2, hv[daq]);
    TCBwrite_COIN_WIDTH(sid, mid[daq], coin_width[daq]);
    TCBwrite_MULTI_THR(sid, mid[daq], multi_thr_daq[daq]);
//    for (ch = 1; ch <= 32; ch++)
//      TCBwrite_DISC_THR(sid, mid[daq], ch, disc_thr);
    for (ch = 1; ch <= 32; ch++)
      TCBwrite_DISC_THR(sid, mid[daq], ch, disc_thr[daq][ch]);
    TCBwrite_GATE_WIDTH(sid, mid[daq], gate_width[daq]);
    TCBwrite_DELAY(sid, mid[daq], idelay[daq]);
    TCBwrite_PEAK_SUM_WIDTH(sid, mid[daq], peak_sum_width[daq]);
    TCBwrite_ZERO_SUP(sid, mid[daq], zero_sup[daq]);
    TCBwrite_PRESCALE(sid, mid[daq], prescale[daq]);
    TCBmeasure_PED(sid, mid[daq]);
  }
  
  // read back settings
  //printf("TCB coincidence width = %d ns\n", TCBread_COIN_WIDTH(sid, 0));
  //printf("TCB multiplicity threshold = %d\n", TCBread_MULTI_THR(sid, 0));
  //printf("TCB pedestal trigger interval = %d ms\n", TCBread_PEDESTAL_TRIGGER_INTERVAL(sid));
  //printf("TCB trigger enable = %d\n", TCBread_TRIGGER_ENABLE(sid));
  for (daq = 0; daq < num_of_daq; daq++) {
    printf("DAQ[%d] high voltage ch1 = %f V\n", mid[daq], TCBread_HV(sid, mid[daq], 1));
    printf("DAQ[%d] high voltage ch2 = %f V\n", mid[daq], TCBread_HV(sid, mid[daq], 2));
    printf("DAQ[%d] coincidence width = %d ns\n", mid[daq], TCBread_COIN_WIDTH(sid, mid[daq]));
    printf("DAQ[%d] multiplicity threshold = %d\n", mid[daq], TCBread_MULTI_THR(sid, mid[daq]));
    printf("DAQ[%d] discriminator threshold = %d\n", mid[daq], TCBread_DISC_THR(sid, mid[daq], 1));
    printf("DAQ[%d] gate width = %d us\n", mid[daq], TCBread_GATE_WIDTH(sid, mid[daq]));
    printf("DAQ[%d] input delay = %d ns\n", mid[daq], TCBread_DELAY(sid, mid[daq]));
    printf("DAQ[%d] peak sum width = %d ns\n", mid[daq], TCBread_PEAK_SUM_WIDTH(sid, mid[daq]));
    printf("DAQ[%d] zero suppression = %d\n", mid[daq], TCBread_ZERO_SUP(sid, mid[daq]));
    printf("DAQ[%d] waveform prescale = %d\n", mid[daq], TCBread_PRESCALE(sid, mid[daq]));
    TCBread_TEMP(sid, mid[daq], temp[daq]);
    printf("DAQ[%d] temperature ch1 = %f\n", mid[daq], temp[daq][0]);
    printf("DAQ[%d] temperature ch2 = %f\n", mid[daq], temp[daq][1]);
    for (ch = 1; ch <= 32; ch++)
      printf("DAQ[%d] ch%d pedestal = %d\n", mid[daq], ch, TCBread_PED(sid, mid[daq], ch));
  }
    
  // only if necessary, reset timer
  TCBreset_TIMER(sid);
  
  // reset DAQ
  TCBreset(sid);

  // close TCB
  TCBclose(sid);
  
  // close USB
  USB3Exit();
  
  return 0;
}


