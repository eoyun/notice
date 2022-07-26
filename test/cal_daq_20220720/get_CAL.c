#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "NoticeCALDAQ.h"
#include "NoticeCALTCB.h"

#define BUF_SIZE (65536)           // in kbyte

int main(int argc, char *argv[])
{
  int is_device_exist;
  int sid=0;
  int mid;
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
  int mid_file_name=0;

  if (argc < 2) {
    printf("Enter mid : ");
    scanf("%d", &mid);
  }
  else{ 
    mid = atoi(argv[1]);
  }

  // assign data array
  data = (char *)malloc(BUF_SIZE * 1024); 
  
  USB3Init();
  CALTCBopen(sid);
  CALTCBreset(sid);
  run_number = CALTCBread_RUN_NUMBER(sid,mid);
  CALTCBclose(sid); 
  USB3Exit();

  // filename here
  //
  if (mid == 7) mid_file_name =1;
  if (mid == 10) mid_file_name =2;
 
  //sprintf(wave_filename, "/media/eoyun/Expansion/DAQ_data/220723/cal_wave_%d_%lu.dat",mid_file_name,run_number);
  //sprintf(fast_filename, "/media/eoyun/Expansion/DAQ_data/220723/cal_fast_%d_%lu.dat",mid_file_name,run_number);
  sprintf(wave_filename, "cal_wave_%d_%lu.dat",mid_file_name,run_number);
  sprintf(fast_filename, "cal_fast_%d_%lu.dat",mid_file_name,run_number);


  // open data file
  wave_fp = fopen(wave_filename, "wb");
  fast_fp = fopen(fast_filename, "wb");

  // init LIBUSB
  USB3Init_daq();
    
  // open DAQ
  is_device_exist = CALDAQopen(mid);
  
  if (is_device_exist>0){
    // wait if TCB not yet runs DAQ
    run = 0;
    while (!run)
      run = CALDAQread_RUN(mid);
  
    // taking data
    while (run) {
      // get waveform data
      wave_data_size = CALDAQread_DATASIZE(mid);
      if (wave_data_size) {
        if (wave_data_size > BUF_SIZE)
          wave_data_size = BUF_SIZE;
        // read data
        CALDAQread_DATA(mid, wave_data_size, data);
        fwrite(data, 1, wave_data_size * 1024, wave_fp);
        wave_evt = wave_evt + (wave_data_size / 64);   // 1 event = 64 kB
        printf("Waveform %d events are taken\n", wave_evt);
      }
        
      // get fast data
      fast_data_size = CALDAQread_FAST_DATASIZE(mid);
      if (fast_data_size) {
        if (fast_data_size > BUF_SIZE)
          fast_data_size = BUF_SIZE;
        // read data
        CALDAQread_FAST_DATA(mid, fast_data_size, data);
        fwrite(data, 1, fast_data_size * 1024, fast_fp);
        fast_evt = fast_evt + (fast_data_size * 4);   // 1 event = 256 byte
        printf("Fast %d events are taken\n", fast_evt);
      }
    
      // check run status
      run = CALDAQread_RUN(mid);
    }
  
    // read remaining waveform data
    wave_data_size = CALDAQread_DATASIZE(mid);
    while (wave_data_size) {
      wave_data_size = CALDAQread_DATASIZE(mid);
      if (wave_data_size) {
        if (wave_data_size > BUF_SIZE)
          wave_data_size = BUF_SIZE;
        // read data
        CALDAQread_DATA(mid, wave_data_size, data);
        fwrite(data, 1, wave_data_size * 1024, wave_fp);
        wave_evt = wave_evt + (wave_data_size / 64);   // 1 event = 64 kB
        printf("Waveform %d events are taken\n", wave_evt);
      }
    }

    // read remaining fast data
    fast_data_size = CALDAQread_FAST_DATASIZE(mid);
    while (fast_data_size) {
      fast_data_size = CALDAQread_FAST_DATASIZE(mid);
      if (fast_data_size) {
        if (fast_data_size > BUF_SIZE)
          fast_data_size = BUF_SIZE;
        // read data
        CALDAQread_FAST_DATA(mid, fast_data_size, data);
        fwrite(data, 1, fast_data_size * 1024, fast_fp);
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
  CALDAQclose(mid);

  // exit LIBUSB
  USB3Exit_daq();

  return 0;
}

