#include <unistd.h>
#include <stdio.h>
#include "NoticeCALTCB.h"

int main(void)
{
  // setting here
  unsigned long cw_tcb = 10;
  unsigned long pedestal_trigger_interval = 0;
  unsigned long trigger_enable = 0x8; //1 self 2 pede 4 soft 8 external
  unsigned long multiplicity_thr_tcb = 1;
  unsigned long cw_daq = 3;
  int univ = 1;   // 0 = knu, 1 = ysu
  //float hv = 38.0;
  float hv=0;
  unsigned long thr = 4095;
  unsigned long thr_tri = 400;
  unsigned long multiplicity_thr_daq = 1;
  unsigned long trigger_delay = 4;// self 2 external 4
  unsigned long trigger_latency = 250;
  unsigned long run_number;
  unsigned long down_sampling = 0;    // 0 = 5 Ghz, 1 = 2.5 GHz
  int sid = 0;
  int mid[40];
  int num_of_daq = 0;
  unsigned long link_data[2];
  int linked[40];
  unsigned long mid_data[40];
  unsigned long ch;
  int daq;
  FILE *fp;
  char filename[100];
  int pmtch;
  int pmt;
  int pmtchnum[64];
  int pmtHV[64];
 
  // init LIBUSB
  USB3Init();
  printf("number of channels : ");
  scanf("%d",&pmtch);
  for (pmt=0;pmt<pmtch;pmt++){
    printf("%d\n",pmt);
    printf("ch number : ");
    scanf("%d",&pmtchnum[pmt]);
    printf("HV(V) : ");
    scanf("%d",&pmtHV[pmt]);
  }  
  // open TCB
  CALTCBopen(sid);

  // get link status
  CALTCBread_LINK(0, link_data);
  CALTCBread_LINK(sid, link_data);
  
  for (ch = 0; ch < 32; ch++)
    linked[ch] = (link_data[0] >> ch) & 0x1;
  for (ch = 32; ch < 40; ch++)
    linked[ch] = (link_data[1] >> (ch - 32)) & 0x1;
  
  // read mid of linked DAQ modules
  CALTCBread_MID(sid, mid_data);
 
  // read connected DAQ machines
  for (ch = 0; ch < 40; ch++) {
    if (linked[ch]) {
      mid[num_of_daq] = mid_data[ch];
      //printf("%lu\n",mid_data[ch]);
      printf("mid %d is found at ch%ld\n", mid[num_of_daq], ch + 1);
      // first come, first served
      //ch = 40;
      num_of_daq = num_of_daq + 1;
    }
  }

  // reset DAQ
  //CALTCBresetTIMER(sid);   // optional timer reset
  CALTCBreset(sid);

  // initialize DAQ
  for (int i=0;i<num_of_daq;i++) CALTCB_DRSinit(sid, mid[i]);

  // write setting
  CALTCBwrite_CW(sid, 0, cw_tcb);
  CALTCBwrite_PEDESTAL_TRIGGER_INTERVAL(sid, pedestal_trigger_interval);
  CALTCBwrite_TRIGGER_ENABLE(sid, trigger_enable);
  CALTCBwrite_MULTIPLICITY_THR(sid, 0, multiplicity_thr_tcb);
  CALTCBwrite_TRIGGER_DELAY(sid, trigger_delay);
  for (daq = 0; daq < num_of_daq; daq++) {
    CALTCBwrite_CW(sid, mid[daq], cw_daq);
    CALTCBwrite_MULTIPLICITY_THR(sid, mid[daq], multiplicity_thr_daq);
    CALTCBwrite_TRIGGER_LATENCY(sid, mid[daq], trigger_latency);
    
    //set run number
    run_number=1+CALTCBread_RUN_NUMBER(sid,mid[daq]);
    CALTCBwrite_RUN_NUMBER(sid, mid[daq], run_number);
    CALTCBwrite_DOWN_SAMPLING(sid, mid[daq], down_sampling);
    for (ch = 1; ch <= 4; ch++)
      CALTCBwrite_HV(sid, mid[daq], ch, univ, hv);
    for (ch = 1; ch <= 32; ch++){
      if (ch ==17) CALTCBwrite_THR(sid, mid[daq], ch, thr);	    
      else CALTCBwrite_THR(sid, mid[daq], ch, thr);
    }
    
    //CALTCBwrite_THR(sid, mid[daq], 1, 40);
   // CALTCBwrite_THR(sid, mid[daq], 3, 25);
    //CALTCBwrite_THR(sid, mid[daq], 15, 40);
  }
  //write setting log
  sprintf(filename,"/media/yu/Expansion/DAQ_data/220604/settinglog/set_%lu.log",run_number);
  fp = fopen(filename,"wt");
  
  fprintf(fp,"TCB Coincidence width = %ld\n", CALTCBread_CW(sid, 0));
  fprintf(fp,"Pedestal trigger interval = %ld\n", CALTCBread_PEDESTAL_TRIGGER_INTERVAL(sid));
  fprintf(fp,"Trigger enable = %lX\n", CALTCBread_TRIGGER_ENABLE(sid));
  fprintf(fp,"TCB Multiplicity threshold = %ld\n", CALTCBread_MULTIPLICITY_THR(sid, 0));
  fprintf(fp,"DAQ Coincidence width = %ld\n", CALTCBread_CW(sid, mid[0]));
  fprintf(fp,"DAQ Multiplicity threshold = %ld\n", CALTCBread_MULTIPLICITY_THR(sid, mid[0]));
  fprintf(fp,"Trigger delay = %ld\n", CALTCBread_TRIGGER_DELAY(sid));
  fprintf(fp,"Trigger latency = %ld\n", CALTCBread_TRIGGER_LATENCY(sid, mid[0]));
  fprintf(fp,"Run number = %ld\n", CALTCBread_RUN_NUMBER(sid, mid[0]));
  fprintf(fp,"Down sampling = %ld\n", CALTCBread_DOWN_SAMPLING(sid, mid[0]));
  fprintf(fp,"Temperature = %f\n", CALTCBread_TEMP(sid, mid[0]));
  /*for (ch = 1; ch <= 4; ch++)
    fprintf(fp,"HV[%ld] = %f\n", ch, CALTCBread_HV(sid, mid[0], ch, univ));*/
  for (ch = 1; ch <pmtch+1;ch++){
    fprintf(fp,"HV[%d]=%d\n",pmtchnum[ch-1],pmtHV[ch-1]);
  }
  for (ch = 1; ch <= 32; ch++)
    if(ch==1||ch==5||ch==11||ch==15) fprintf(fp,"Threshold[%ld] = %ld\n", ch, CALTCBread_THR(sid, mid[0], ch));

  //CALTCBwrite_CW(sid, mid, cw_daq);
  //CALTCBwrite_MULTIPLICITY_THR(sid, mid, multiplicity_thr_daq);
  //CALTCBwrite_TRIGGER_DELAY(sid, mid, trigger_delay);
  //CALTCBwrite_TRIGGER_LATENCY(sid, mid, trigger_latency);
  //CALTCBwrite_RUN_NUMBER(sid, mid, run_number);
  //for (ch = 1; ch <= 4; ch++)
  //  CALTCBwrite_HV(sid, mid, ch, hv);
  //for (ch = 1; ch <= 32; ch++)
  //  CALTCBwrite_THR(sid, mid, ch, thr);

  // readback setting
  

  printf("TCB Coincidence width = %ld\n", CALTCBread_CW(sid, 0));
  printf("Pedestal trigger interval = %ld\n", CALTCBread_PEDESTAL_TRIGGER_INTERVAL(sid));
  printf("Trigger enable = %lX\n", CALTCBread_TRIGGER_ENABLE(sid));
  printf("TCB Multiplicity threshold = %ld\n", CALTCBread_MULTIPLICITY_THR(sid, 0));
  printf("DAQ Coincidence width = %ld\n", CALTCBread_CW(sid, mid[0]));
  printf("DAQ Multiplicity threshold = %ld\n", CALTCBread_MULTIPLICITY_THR(sid, mid[0]));
  printf("Trigger delay = %ld\n", CALTCBread_TRIGGER_DELAY(sid));
  printf("Trigger latency = %ld\n", CALTCBread_TRIGGER_LATENCY(sid, mid[0]));
  printf("Run number = %ld\n", CALTCBread_RUN_NUMBER(sid, mid[0]));
  printf("Down sampling = %ld\n", CALTCBread_DOWN_SAMPLING(sid, mid[0]));
  printf("Temperature = %f\n", CALTCBread_TEMP(sid, mid[0]));
 // for (ch = 1; ch <= 4; ch++)
   // printf("HV[%ld] = %f\n", ch, CALTCBread_HV(sid, mid[0], ch, univ));
  for (ch = 1; ch <pmtch+1;ch++){
    printf("HV[%d]=%d\n",pmtchnum[ch-1],pmtHV[ch-1]);
  }

  for (ch = 1; ch <= 32; ch++)
    if(ch==1||ch==5||ch==11||ch==15)printf("Threshold[%ld] = %ld\n", ch, CALTCBread_THR(sid, mid[0], ch));

//CALTCBwrite_DRS_CALIB(sid, mid, 1);
  
  
  // close TCB
  CALTCBclose(sid);

  // exit LIBUSB
  USB3Exit();

  return 0;
}



