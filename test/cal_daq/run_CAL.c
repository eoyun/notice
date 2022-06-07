#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "NoticeCALTCB.h"
#include "NoticeCALDAQ.h"

#define BUF_SIZE (65536)           // in kbyte

int main(int argc, char *argv[])
{
  int nevt;
  int sid = 0;
  int mid[40];
  unsigned long link_data[2];
  int linked[40];
  unsigned long mid_data[40];
  unsigned long ch;
  char *data;
  unsigned long data_size;
  FILE *fp;
  int run;
  int evt = 0;
  int num_of_daq=0;
  int daq;
  int daq_runnumber;
  //int HV;
  int run_num;
  char filename[100];
  unsigned long run_number;

  // assign data array
  data = (char *)malloc(BUF_SIZE * 1024); 
  
  printf("enter the number of evt : ");
  scanf("%d",&nevt);
	

  if (argc>1){
    //HV = atoi(argv[1]);
    run_num = atoi(argv[1]);
  }
  // open data file
  //sprintf(filename,"cal.dat");
  //sprintf(filename,"/media/yu/Expansion/DAQ_data/220602/muon_06_02_%d.dat",run_num);
  //fp = fopen(filename, "wb");

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

  for (ch = 0; ch < 40; ch++) {
    if (linked[ch]) {
      mid[num_of_daq] = mid_data[ch];
      printf("mid %d is found at ch%ld\n", mid[num_of_daq], ch + 1);
      // first come, first served
      //ch = 40;
      num_of_daq = num_of_daq + 1;
    }
  }

  // open DAQ
  for (int i=0;i<num_of_daq;i++) CALDAQopen(mid[i]);

  // reset DAQ
  CALTCBreset(sid);
  
  // set run number 
  for (daq_runnumber=0;daq_runnumber<num_of_daq;daq_runnumber++){
    run_number = CALTCBread_RUN_NUMBER(sid,mid[daq_runnumber]);
  }
  

  //open data file 
  sprintf(filename,"/media/yu/Expansion/DAQ_data/220604/elec_06_04_%lu.dat",run_number);
  //sprintf(filename,"/media/yu/Expansion/DAQ_data/220602/muon_06_02_%d.dat",run_num);
  fp = fopen(filename, "wb");


  // start DAQ
  CALTCBstart_DAQ(sid);
  printf("Run status = %ld\n", CALDAQread_RUN(mid[0]));

  run = 1;  
  while (run) {
//    CALTCBsend_TRIG(sid);  // optional software trigger
    for (daq=0;daq<num_of_daq;daq++){
      data_size = CALDAQread_DATASIZE(mid[daq]);
      if (data_size > BUF_SIZE)
        data_size = BUF_SIZE;

      if (data_size) {
        printf("data size = %ld\n", data_size);
        CALDAQread_DATA(mid[daq], data_size, data);
        fwrite(data, 1, data_size * 1024, fp);
        evt = evt + (data_size / 64);
        //evt = evt + (data_size / 128);
        printf("%d / %d events are taken\n", evt, nevt);
        if (evt >= nevt) {
          CALTCBstop_DAQ(sid);
          run = 0;
        }  
      }
    }

    if (access("KILLME", F_OK) == 0) {
      CALTCBstop_DAQ(sid);
      run = 0;
      system("rm KILLME");
    } 
  }

  printf("Run status = %ld\n", CALDAQread_RUN(mid[0]));

  // close file  
  fclose(fp);  
  
  // release memory
  free(data);

  // close DAQ
  for (int j=0;j<num_of_daq;j++) CALDAQclose(mid[j]);

  // reset DAQ
  CALTCBreset(sid);

  // close TCB
  CALTCBclose(sid);
  
  // exit LIBUSB
  USB3Exit();

  return 0;
}

