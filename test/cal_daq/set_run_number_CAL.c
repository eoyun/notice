#include <unistd.h>
#include <stdio.h>
#include "NoticeCALTCB.h"

int main(void)
{
  // setting here
  //unsigned long run_number;
  unsigned long run_number=16;
  int sid = 0;
  int mid[40];
  int num_of_daq = 0;
  unsigned long link_data[2];
  int linked[40];
  unsigned long mid_data[40];
  unsigned long ch;
  int daq;

  // init LIBUSB
  USB3Init();
    
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
      printf("mid %d is found at ch%ld\n", mid[num_of_daq], ch + 1);
      // first come, first served
      // ch = 40;
      num_of_daq = num_of_daq + 1;
    }
  }

  // reset DAQ
  //CALTCBresetTIMER(sid);   // optional timer reset
  CALTCBreset(sid);

  // initialize DAQ
  for (int i=0;i<num_of_daq;i++) CALTCB_DRSinit(sid, mid[i]);

  // write setting
  for (daq = 0; daq < num_of_daq; daq++) {
    //run_number=CALTCBread_RUN_NUMBER(sid, mid[daq])+1;
    CALTCBwrite_RUN_NUMBER(sid, mid[daq], run_number);
    
    //CALTCBwrite_THR(sid, mid[daq], 1, 40);
   // CALTCBwrite_THR(sid, mid[daq], 3, 25);
    //CALTCBwrite_THR(sid, mid[daq], 15, 40);
  }
  
  
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
  printf("Run number = %ld\n", CALTCBread_RUN_NUMBER(sid, mid[0]));

//CALTCBwrite_DRS_CALIB(sid, mid, 1);
  
  
  // close TCB
  CALTCBclose(sid);

  // exit LIBUSB
  USB3Exit();

  return 0;
}



