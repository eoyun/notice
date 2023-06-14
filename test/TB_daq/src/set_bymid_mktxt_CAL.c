#include <unistd.h>
#include <stdio.h>
#include "NoticeCALTCB.h"

int main(int argc, char *argv[])
{
  int sid = 0;
  unsigned long link_data[2];
  int linked[40];
  unsigned long mid_data[40];
  unsigned long mid[40];
  int num_of_daq;
  int daq;
  unsigned long cw_tcb;
  unsigned long ptrig_interval;
  unsigned long trig_enable;
  unsigned long mthr_tcb;
  unsigned long trig_dly[15];
  unsigned long cw_daq[15];
  float hv[15][4];
  unsigned long mthr_daq[15];
  unsigned long prescale;
  unsigned long trig_latency[15];
  unsigned long run_num;
  unsigned long down_sampling;
  unsigned long pulse_width;
  unsigned long risetime;
  float fraction;
  unsigned long thr[15][32];
  unsigned long lval_ld;
  unsigned long daq_mid;
  float lval_f;
  FILE *fp;
  int ch;
  char var_name[50];
  char config_folder_name[256];
  char tcb_name[256];
  char daq_name[256];
  int idaq_setup;
  int daq_setup=15;
  FILE *fp_mid;

  if (argc<2){
    //printf("Enter the config name : ");
	//scanf("%s",&config_folder_name);
	system("touch /Users/yhep/scratch/notice/test/TB_daq/FAILSET");  
    printf("please check the input name \n");
	return 0;
  } else {
	sprintf(config_folder_name,"/Users/yhep/scratch/notice/test/TB_daq/config/%s", argv[1]);
    printf("config file directory is %s\n",config_folder_name);
  }
  
  //printf("dbgdbgdbg : %s\n",config_folder_name);
//  if ((access("setup.txt", 0)) == 0) {
//    fp = fopen("setup.txt", "rt");
/*  if ((access(config_folder_name, 0)) == 0) {
	printf("hello2\n");
	sprintf(tcb_name,"%s/set_TCB.txt",config_folder_name);
	printf("%s\n",tcb_name);
    fp = fopen(tcb_name, "rt");
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &cw_tcb);
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &ptrig_interval);
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &trig_enable);
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &mthr_tcb);
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &trig_dly);
	fclose(fp);
    for(idaq_setup=0;idaq_setup<daq_setup;idaq_setup++){
	  daq_name[0] ='\0';
	  sprintf(daq_name,"%s/set_DAQ_%d.txt",config_folder_name,idaq_setup+1);
	  fp = fopen(daq_name,"rt");
	  fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
	  cw_daq[idaq_setup] = lval_ld;
	  fscanf(fp,"%s",var_name);
	  for (ch=0;ch<4;ch++){
        fscanf(fp, "%f", &lval_f);
		hv[idaq_setup][ch] = lval_f;
	  }

      fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
	  mthr_daq[idaq_setup] = lval_ld;
      fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
	  prescale[idaq_setup] = lval_ld;
      fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
	  trig_latency[idaq_setup] = lval_ld;
    //fscanf(fp, "%ld", &run_num);
      fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
	  down_sampling[idaq_setup] = lval_ld;
      fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
	  pulse_width[idaq_setup] = lval_ld;
      fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
      risetime[idaq_setup] = lval_ld;
      fscanf(fp,"%s",var_name);
      fscanf(fp, "%f", &lval_f);
	  fraction[idaq_setup] = lval_f;
      fscanf(fp,"%s",var_name);
      for (ch = 0; ch < 32; ch++) {
        fscanf(fp, "%ld", &lval_ld);
        thr[idaq_setup][ch] = lval_ld;
      }
	}
    fclose(fp);
  }
  else{
	printf("please check file name\n");
	return 0;
  }
  
  printf("TCB coincidence = %ld\n", cw_tcb);
  printf("TCB pedestal trigger interval = %ld\n", ptrig_interval);
  printf("TCB trigger enable = %ld\n", trig_enable);
  printf("TCB multiplicity threshold = %ld\n", mthr_tcb);
  printf("TCB trigger delay = %ld\n", trig_dly);

  // readbcak DAQ setting
  for (daq = 0; daq < 15; daq++) {
    printf("---------------- DAQ %d --------------------\n", daq+1);
    printf("Coincidence width = %ld\n", cw_daq[daq]);
    for (ch = 0;ch<4;ch++){
	  printf("High voltage[%d] = %f\n", ch+1 , hv[daq][ch]);}
    printf("Multiplicity threshold = %ld\n", mthr_daq[daq]);
    printf("Prescale = %ld\n", prescale[daq]);
    printf("Trigger latency = %ld\n", trig_latency[daq] );
    printf("Down-sampling = %ld\n", down_sampling[daq]);
    printf("Pulse width = %ld\n", pulse_width[daq]);
    printf("Rise time = %ld\n", risetime[daq]);
    printf("Constant fraction = %f\n", fraction[daq]);
    for (ch = 0; ch < 32; ch++)
      printf("Threshold[%d] = %ld\n", ch + 1, thr[daq][ch] );
   }*/
   /*else {                                    // default setting here
    cw_tcb = 10;
    ptrig_interval = 0;
    trig_enable = 15;
    mthr_tcb = 1;
    trig_dly = 5;
    cw_daq = 5;
    hv = 0.0;
    mthr_daq = 1;
    prescale = 1;
    trig_latency = 250;
    //run_num = 1;
    down_sampling = 0;
    pulse_width = 300;
    risetime = 100;
    fraction = 0.5;
    for (ch = 0; ch < 32; ch++) 
      thr[ch] = 100;
  }*/


  // init LIBUSB
  USB3Init();
  CALTCBreset(sid);
  // open TCB
  CALTCBopen(sid);
  //CALTCBdisable_LINK(sid,17,1);
  // get link status
  CALTCBread_LINK(sid, link_data);

  for (daq = 0; daq < 32; daq++)
    linked[daq] = (link_data[0] >> daq) & 0x1;
  for (daq = 32; daq < 40; daq++)
    linked[daq] = (link_data[1] >> (daq - 32)) & 0x1;
    
  // read mid of linked DAQ modules
  CALTCBread_MID(sid, mid_data);

  // assign DAQ index
  num_of_daq = 0;

  for (daq = 0; daq < 40; daq++) {
    if (linked[daq]) {
      //if(mid_data[daq]==0||mid_data[daq]>15) continue;
      daq_mid = CALTCBread_DAQ_MID(sid,mid_data[daq]);
      if(daq_mid!=mid_data[daq]) continue;
      mid[num_of_daq] = mid_data[daq];
      printf("mid %ld is found at ch%d\n", mid[num_of_daq], daq + 1);
      num_of_daq = num_of_daq + 1;
    }
  }
  CALTCBstop_DAQ(sid);
  // reset DAQ
  //CALTCBresetTIMER(sid);   // optional timer reset

  if ((access(config_folder_name, 0)) == 0) {
	//printf("hello2\n");
	sprintf(tcb_name,"%s/set_TCB.txt",config_folder_name);
	//printf("%s\n",tcb_name);
    fp = fopen(tcb_name, "rt");
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &cw_tcb);
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &ptrig_interval);
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &trig_enable);
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &mthr_tcb);
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &prescale);
	
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &down_sampling);
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &pulse_width);
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%ld", &risetime);
    fscanf(fp,"%s",var_name);
    fscanf(fp, "%f", &fraction);
	fclose(fp);
    for(idaq_setup=0;idaq_setup<num_of_daq;idaq_setup++){
	  daq_name[0] ='\0';
	  sprintf(daq_name,"%s/set_DAQ_%lu.txt",config_folder_name,mid[idaq_setup]);
	  fp = fopen(daq_name,"rt");
	  fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
	  cw_daq[idaq_setup] = lval_ld;
	  fscanf(fp,"%s",var_name);
	  for (ch=0;ch<4;ch++){
        fscanf(fp, "%f", &lval_f);
		hv[idaq_setup][ch] = lval_f;
	  }

      fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
	    mthr_daq[idaq_setup] = lval_ld;
     // fscanf(fp,"%s",var_name);
     // fscanf(fp, "%ld", &lval_ld);
	 // prescale[idaq_setup] = lval_ld;
      fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
	    trig_latency[idaq_setup] = lval_ld;
      fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
	    trig_dly[idaq_setup] = lval_ld;
    //fscanf(fp, "%ld", &run_num);
   //   fscanf(fp,"%s",var_name);
   //   fscanf(fp, "%ld", &lval_ld);
//	  down_sampling[idaq_setup] = lval_ld;
  /*    fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
	  pulse_width[idaq_setup] = lval_ld;
      fscanf(fp,"%s",var_name);
      fscanf(fp, "%ld", &lval_ld);
      risetime[idaq_setup] = lval_ld;
      fscanf(fp,"%s",var_name);
      fscanf(fp, "%f", &lval_f);
	  fraction[idaq_setup] = lval_f;*/
      fscanf(fp,"%s",var_name);
      for (ch = 0; ch < 32; ch++) {
        fscanf(fp, "%ld", &lval_ld);
        thr[idaq_setup][ch] = lval_ld;
      }
	}
    fclose(fp);
  }
  else{
	system("touch /Users/yhep/scratch/notice/test/TB_daq/FAILSET");
	printf("please check file name\n");
	return 0;
  }

  fp_mid = fopen("/Users/yhep/scratch/notice/test/TB_daq/turn_on_mid.txt","wt");
  for (idaq_setup=0;idaq_setup<num_of_daq;idaq_setup++){
    fprintf(fp_mid,"%lu ",mid[idaq_setup]);
  }
  fclose(fp_mid);

  CALTCBreset(sid);
 
  // setting TCB
  CALTCBwrite_CW(sid, 0, cw_tcb);
  CALTCBwrite_PEDESTAL_TRIGGER_INTERVAL(sid, ptrig_interval);
  CALTCBwrite_TRIGGER_ENABLE(sid, trig_enable);
  CALTCBwrite_MULTIPLICITY_THR(sid, 0, mthr_tcb);

  // setting DAQ
  for (daq = 0; daq < num_of_daq; daq++) {
    CALTCB_DRSinit(sid, mid[daq]);
    CALTCBwrite_CW(sid, mid[daq], cw_daq[daq]);
    for (ch = 1; ch <= 4; ch++)
      CALTCBwrite_HV(sid, mid[daq], ch, 1, hv[daq][ch-1]);
    CALTCBwrite_MULTIPLICITY_THR(sid, mid[daq], mthr_daq[daq]);
    CALTCBwrite_PRESCALE(sid, mid[daq], prescale);
    CALTCBwrite_TRIGGER_LATENCY(sid, mid[daq], trig_latency[daq]);
    CALTCBwrite_TRIGGER_DELAY(sid, mid[daq], trig_dly[daq]);
    //CALTCBwrite_RUN_NUMBER(sid, mid[daq], run_num);
    CALTCBwrite_DOWN_SAMPLING(sid, mid[daq], down_sampling);
    CALTCBwrite_PULSE_WIDTH(sid, mid[daq], pulse_width);
    CALTCBwrite_RISETIME(sid, mid[daq], risetime);
    CALTCBwrite_CF_FRACTION(sid, mid[daq], fraction);
    for (ch = 1; ch <= 32; ch++)
      CALTCBwrite_THR(sid, mid[daq], ch, thr[daq][ch - 1]);
  }

  // readback TCB setting
  printf("TCB coincidence = %ld\n", CALTCBread_CW(sid, 0));
  printf("TCB pedestal trigger interval = %ld\n", CALTCBread_PEDESTAL_TRIGGER_INTERVAL(sid));
  printf("TCB trigger enable = %ld\n", CALTCBread_TRIGGER_ENABLE(sid));
  printf("TCB multiplicity threshold = %ld\n", CALTCBread_MULTIPLICITY_THR(sid, 0));
  
  // readbcak DAQ setting
  for (daq = 0; daq < num_of_daq; daq++) {
    printf("---------------- DAQ %ld --------------------\n", mid[daq]);
    printf("Temperature = %f\n", CALTCBread_TEMP(sid, mid[daq]));
    printf("Coincidence width = %ld\n", CALTCBread_CW(sid, mid[daq]));
    printf("High voltage = %f\n", CALTCBread_HV(sid, mid[daq], 1, 1));
    printf("Multiplicity threshold = %ld\n", CALTCBread_MULTIPLICITY_THR(sid, mid[daq]));
    printf("Prescale = %ld\n", CALTCBread_PRESCALE(sid, mid[daq]));
    printf("Trigger latency = %ld\n", CALTCBread_TRIGGER_LATENCY(sid, mid[daq]));
    printf("Run # = %ld\n", CALTCBread_RUN_NUMBER(sid, mid[daq]));
    printf("TCB trigger delay = %ld\n", CALTCBread_TRIGGER_DELAY(sid,mid[daq]));
    printf("Down-sampling = %ld\n", CALTCBread_DOWN_SAMPLING(sid, mid[daq]));
    printf("Pulse width = %ld\n", CALTCBread_PULSE_WIDTH(sid, mid[daq]));
    printf("Rise time = %ld\n", CALTCBread_RISETIME(sid, mid[daq]));
    printf("Constant fraction = %f\n", CALTCBread_CF_FRACTION(sid, mid[daq]));
    for (ch = 1; ch <= 32; ch++)
      printf("Threshold[%d] = %ld\n", ch, CALTCBread_THR(sid, mid[daq], ch));
  }
  
  // close TCB
  CALTCBclose(sid);

  // exit LIBUSB
  USB3Exit();

  return 0;
}



