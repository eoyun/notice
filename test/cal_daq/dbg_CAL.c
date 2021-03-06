#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "NoticeCALTCB.h"

int main(void)
{
  int sid = 0;
  int mid;
  unsigned long link_data[2];
  int linked[40];
  unsigned long mid_data[40];
  int com;
  float hv;
  int rofs;
  int oofs;
  int thr;
  int ch;
  
  // init LIBUSB
  USB3Init();
    
  // open TCB
  CALTCBopen(0);

  // get link status
  CALTCBread_LINK(0, link_data);
  CALTCBread_LINK(sid, link_data);

  for (ch = 0; ch < 32; ch++)
    linked[ch] = (link_data[0] >> ch) & 0x1;
  for (ch = 32; ch < 40; ch++)
    linked[ch] = (link_data[1] >> (ch - 32)) & 0x1;
  
  // read mid of linked DAQ modules
  CALTCBread_MID(sid, mid_data);

  com = 0;
  for (ch = 0; ch < 40; ch++) {
    if (linked[ch]) {
      mid = mid_data[ch];
      printf("mid %d is found at ch%d\n", mid, ch + 1);
      ch = 40;
      com = 99;
    }
  }

  if (com) {
    while(com) {
      printf("\n\n");
      printf("1. hv   2. DRS ofs   3. thr   0. quit\n");
      printf("9. debug\n");
      printf("enter command : ");
      scanf("%d", &com);
    
      if (com == 1) {
        printf("enter ch & hv : ");
        scanf("%d", &ch);
        scanf("%f", &hv);
        CALTCBwrite_HV(sid, mid, ch, hv);
        printf("hv = %f\n", CALTCBread_HV(sid, mid, ch));
      }
      else if (com == 2) {
        printf("enter ROFS & OOFS : ");
        scanf("%d", &rofs);
        scanf("%d", &oofs);
        CALTCBwrite_DRS_OFS(sid, mid, rofs, oofs);
      }
      else if (com == 3) {
        printf("enter ch & thr : ");
        scanf("%d", &ch);
        scanf("%d", &thr);
        CALTCBwrite_THR(sid, mid, ch, thr);
        printf("thr = %ld\n", CALTCBread_THR(sid, mid, ch));
      }
      else if (com == 9) {
        CALTCB_DRSinit(sid, mid);
      }
    }
  }

  // close TCB
  CALTCBclose(0);

  // exit LIBUSB
  USB3Exit();

  return 0;
}



