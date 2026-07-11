#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "NoticeCALTCB.h"
#include "NoticeCALDAQ.h"


#define BUF_SIZE (65536)           // in kbyte

int main(int argc, char *argv[])
{
  int sid=0;
  int mid[40];
  char *data;
  char wave_filename[256];
  char fast_filename[256];
  FILE *wave_fp;
  FILE *fast_fp;
  unsigned long run;
  unsigned long run_number;
  unsigned long wave_data_size;
  unsigned long fast_data_size;
  int wave_evt = 0;
  int fast_evt = 0;
  int sum_is_device_exist=0;
  int iargc;
  int num_of_daq;
  int daq;
  
  // default mid
  mid[0] = 1;
  num_of_daq= 1;

  if (argc>1){
    for(iargc=0;iargc<argc;iargc++){
      mid[iargc-1] = atoi(argv[iargc]);
    }
    num_of_daq = argc-1;
  }

  // assign data array
  data = (char *)malloc(BUF_SIZE * 1024); 

 

  // init LIBUSB
  USB3Init();
  CALTCBopen(sid);
  CALTCBreset(sid);
  run_number = CALTCBread_RUN_NUMBER(sid,mid[0]);
  CALTCBclose(sid); 
  USB3Exit();
 
  USB3Init_daq();
  // open DAQ
  for (daq=0; daq<num_of_daq; daq++){
    sum_is_device_exist+=CALDAQopen(mid[daq]);
    
    printf("daq num is %d\n",mid[daq]);
  }
  

  // filename here
  sprintf(wave_filename, "cal_wave_%lu.dat",run_number);
  sprintf(fast_filename, "cal_fast_%lu.dat",run_number);

  // open data file
  wave_fp = fopen(wave_filename, "wb");
  fast_fp = fopen(fast_filename, "wb");

  // wait if TCB not yet runs DAQ
  if (sum_is_device_exist>num_of_daq-1){//end is 145
    run = 0;
    while (!run)
      run = CALDAQread_RUN(mid[0]);
    
    // taking data
    while (run) {
      // get waveform data
      wave_data_size = CALDAQread_DATASIZE(mid[0]);
      if (wave_data_size) {
        if (wave_data_size > BUF_SIZE)
          wave_data_size = BUF_SIZE;
        // read data
        for (daq=0;daq<num_of_daq;daq++){
          CALDAQread_DATA(mid[daq], wave_data_size, data);
          fwrite(data, 1, wave_data_size * 1024, wave_fp);
        }
        wave_evt = wave_evt + (wave_data_size / 64);   // 1 event = 64 kB
        printf("Waveform %d events are taken\n", wave_evt);
      }
          
      // get fast data
      fast_data_size = CALDAQread_FAST_DATASIZE(mid[0]);
      if (fast_data_size) {
        if (fast_data_size > BUF_SIZE)
          fast_data_size = BUF_SIZE;
        // read data
        for (daq=0;daq<num_of_daq;daq++){
          CALDAQread_FAST_DATA(mid[daq], fast_data_size, data);
          fwrite(data, 1, fast_data_size * 1024, fast_fp);
        }
        fast_evt = fast_evt + (fast_data_size * 4);   // 1 event = 256 byte
        printf("Fast %d events are taken\n", fast_evt);
      }
      
      // check run status
      run = CALDAQread_RUN(mid[0]);
    }
    
    // read remaining waveform data
    wave_data_size = CALDAQread_DATASIZE(mid[0]);
    while (wave_data_size) {
      wave_data_size = CALDAQread_DATASIZE(mid[0]);
      if (wave_data_size) {
        if (wave_data_size > BUF_SIZE)
          wave_data_size = BUF_SIZE;
        // read data
        for (daq=0;daq<num_of_daq;daq++){
          CALDAQread_DATA(mid[daq], wave_data_size, data);
          fwrite(data, 1, wave_data_size * 1024, wave_fp);
        }
        wave_evt = wave_evt + (wave_data_size / 64);   // 1 event = 64 kB
        printf("Waveform %d events are taken\n", wave_evt);
      }
    }
  
    // read remaining fast data
    fast_data_size = CALDAQread_FAST_DATASIZE(mid[0]);
    while (fast_data_size) {
      fast_data_size = CALDAQread_FAST_DATASIZE(mid[0]);
      if (fast_data_size) {
        if (fast_data_size > BUF_SIZE)
          fast_data_size = BUF_SIZE;
        // read data
        for (daq=0;daq<num_of_daq;daq++){
          CALDAQread_FAST_DATA(mid[daq], fast_data_size, data);
          fwrite(data, 1, fast_data_size * 1024, fast_fp);
        }
        fast_evt = fast_evt + (fast_data_size * 4);   // 1 event = 256 byte
        printf("Fast %d events are taken\n", fast_evt);
      }
    }
  }
  // close file  
  fclose(wave_fp);  
  fclose(fast_fp);  
  
  // release memory
  free(data);

  // close DAQ
  for (daq=0;daq<num_of_daq;daq++){
    CALDAQclose(mid[daq]);
  }

  // exit LIBUSB
  USB3Exit_daq();

  return 0;
}

