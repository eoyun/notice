#include <unistd.h>
#include <stdio.h>
#include "NoticeCALTCB.h"

int main(void)
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
  unsigned long trig_dly;
  unsigned long cw_daq;
  float hv;
  unsigned long mthr_daq;
  unsigned long prescale;
  unsigned long trig_latency;
  unsigned long run_num;
  unsigned long down_sampling;
  unsigned long pulse_width;
  unsigned long risetime;
  float fraction;
  unsigned long thr[32];
  unsigned long lval;
  FILE *fp;
  int ch;

  if ((access("setup.txt", 0)) == 0) {
    fp = fopen("setup.txt", "rt");
    fscanf(fp, "%ld", &cw_tcb);
    fscanf(fp, "%ld", &ptrig_interval);
    fscanf(fp, "%ld", &trig_enable);
    fscanf(fp, "%ld", &mthr_tcb);
    fscanf(fp, "%ld", &trig_dly);
    fscanf(fp, "%ld", &cw_daq);
    fscanf(fp, "%f", &hv);
    fscanf(fp, "%ld", &mthr_daq);
    fscanf(fp, "%ld", &prescale);
    fscanf(fp, "%ld", &trig_latency);
    //fscanf(fp, "%ld", &run_num);
    fscanf(fp, "%ld", &down_sampling);
    fscanf(fp, "%ld", &pulse_width);
    fscanf(fp, "%ld", &risetime);
    fscanf(fp, "%f", &fraction);
    for (ch = 0; ch < 32; ch++) {
      fscanf(fp, "%ld", &lval);
      thr[ch] = lval;
    }
    fclose(fp);
  }
  else {                                    // default setting here
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
  }

  // init LIBUSB
  USB3Init();
    
  // open TCB
  CALTCBopen(sid);
  CALTCBdisable_LINK(sid,17,1);
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
      if(mid_data[daq]==0||mid_data[daq]>15) continue;
      mid[num_of_daq] = mid_data[daq];
      printf("mid %ld is found at ch%d\n", mid[num_of_daq], daq + 1);
      num_of_daq = num_of_daq + 1;
    }
  }
  CALTCBstop_DAQ(sid);
  // reset DAQ
  //CALTCBresetTIMER(sid);   // optional timer reset
  CALTCBreset(sid);
 
  // setting TCB
  CALTCBwrite_CW(sid, 0, cw_tcb);
  CALTCBwrite_PEDESTAL_TRIGGER_INTERVAL(sid, ptrig_interval);
  CALTCBwrite_TRIGGER_ENABLE(sid, trig_enable);
  CALTCBwrite_MULTIPLICITY_THR(sid, 0, mthr_tcb);
  CALTCBwrite_TRIGGER_DELAY(sid, trig_dly);

  // setting DAQ
  for (daq = 0; daq < num_of_daq; daq++) {
    CALTCB_DRSinit(sid, mid[daq]);
    CALTCBwrite_CW(sid, mid[daq], cw_daq);
    for (ch = 1; ch <= 4; ch++)
      CALTCBwrite_HV(sid, mid[daq], ch, 1, hv);
    CALTCBwrite_MULTIPLICITY_THR(sid, mid[daq], mthr_daq);
    CALTCBwrite_PRESCALE(sid, mid[daq], prescale);
    CALTCBwrite_TRIGGER_LATENCY(sid, mid[daq], trig_latency);
    //CALTCBwrite_RUN_NUMBER(sid, mid[daq], run_num);
    CALTCBwrite_DOWN_SAMPLING(sid, mid[daq], down_sampling);
    CALTCBwrite_PULSE_WIDTH(sid, mid[daq], pulse_width);
    CALTCBwrite_RISETIME(sid, mid[daq], risetime);
    CALTCBwrite_CF_FRACTION(sid, mid[daq], fraction);
    for (ch = 1; ch <= 32; ch++)
      CALTCBwrite_THR(sid, mid[daq], ch, thr[ch - 1]);
  }

  // readback TCB setting
  printf("TCB coincidence = %ld\n", CALTCBread_CW(sid, 0));
  printf("TCB pedestal trigger interval = %ld\n", CALTCBread_PEDESTAL_TRIGGER_INTERVAL(sid));
  printf("TCB trigger enable = %ld\n", CALTCBread_TRIGGER_ENABLE(sid));
  printf("TCB multiplicity threshold = %ld\n", CALTCBread_MULTIPLICITY_THR(sid, 0));
  printf("TCB trigger delay = %ld\n", CALTCBread_TRIGGER_DELAY(sid));
  
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



