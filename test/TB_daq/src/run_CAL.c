#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "NoticeCALTCB.h"

int main(int argc, char *argv[])
{
  int sid = 0;
  int nsec;
  int sec;

  if (argc < 2) {
    printf("Enter DAQ time : ");
    scanf("%d", &nsec);
  }
  else 
    nsec = atoi(argv[1]);

  // init LIBUSB
  USB3Init();
    
  // open TCB
  CALTCBopen(sid);

  // reset DAQ
  CALTCBreset(sid);

  // start DAQ
  CALTCBstart_DAQ(sid);

  sec = 0;
  while (sec < nsec) {
    usleep(1000000);
    sec = sec + 1;
    printf("%d / %d passed\n", sec, nsec);
    if (access("/Users/yhep/scratch/notice/test/TB_daq/KILLME",F_OK)==0){
      system("rm /Users/yhep/scratch/notice/test/TB_daq/KILLME");
      break;
    }
  }

  // stop DAQ
  CALTCBstop_DAQ(sid);

  // close TCB
  CALTCBclose(sid);
  
  // exit LIBUSB
  USB3Exit();

  return 0;
}

