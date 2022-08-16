#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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
  int ch;
  int adc[1000];
  int lsb;
  int msb;
  float sum;
  float mean[32];
  float rms[32];
  int i;
  int disp;
  int ievt;

  if (argc < 2) {
    printf("Enter mid : ");
    scanf("%d", &mid);
  }
  else
    mid = atoi(argv[1]);

  // assign data array
  data = (char *)malloc(BUF_SIZE * 1024);

  USB3Init();
  CALTCBopen(sid);
  CALTCBreset(sid);
  run_number = CALTCBread_RUN_NUMBER(sid,mid);
  CALTCBclose(sid); 
  USB3Exit();

  // filename here
//  sprintf(wave_filename, "cal_wave.dat");
//  sprintf(fast_filename, "cal_fast.dat");
  sprintf(wave_filename, "./data/cal_wave_%d_%lu.dat",mid,run_number);
  sprintf(fast_filename, "./data/cal_fast_%d_%lu.dat",mid,run_number);

  // open data file
  wave_fp = fopen(wave_filename, "wb");
  fast_fp = fopen(fast_filename, "wb");

  // init LIBUSB
  USB3Init_daq();
   
  // open DAQ
  is_device_exist = CALDAQopen(mid);
 
  if (is_device_exist > 0) {
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
//        printf("Waveform %d events are taken\n", wave_evt);
        disp = 0;
        for (ch = 0; ch < 32; ch++) {
          for (i = 1; i < 1001; i++) {
            lsb = data[i * 64 + ch * 2] & 0xFF;
            msb = data[i * 64 + ch * 2 + 1] & 0xFF;
            adc[i - 1] = msb * 256 + lsb;
          }

          sum = 0;
          for (i = 1; i < 1001; i++)
            sum = sum + adc[i -1];
          mean[ch] = (float)sum / 1000.0;
         
          sum = 0;
          for (i = 1; i < 1001; i++)
            sum = sum + (adc[i -1] - mean[ch]) * (adc[i -1] - mean[ch]);
          rms[ch] = sqrt((float)sum / 1000.0);
         
//          if (rms[ch] > 10.0)
            disp = 1;
        }
        if (disp) {
          printf("%d	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f\n", wave_evt,  mean[0], mean[1], mean[2], mean[3]
          , mean[4], mean[5], mean[6], mean[7]
          , mean[8], mean[9], mean[10], mean[11]
          , mean[12], mean[13], mean[14], mean[15]
          , mean[16], mean[17], mean[18], mean[19]
          , mean[20], mean[21], mean[22], mean[23]
          , mean[24], mean[25], mean[26], mean[27]
          , mean[28], mean[29], mean[30], mean[31]);
          printf("%d	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f	%3.1f\n", wave_evt,  rms[0], rms[1], rms[2], rms[3]
          , rms[4], rms[5], rms[6], rms[7]
          , rms[8], rms[9], rms[10], rms[11]
          , rms[12], rms[13], rms[14], rms[15]
          , rms[16], rms[17], rms[18], rms[19]
          , rms[20], rms[21], rms[22], rms[23]
          , rms[24], rms[25], rms[26], rms[27]
          , rms[28], rms[29], rms[30], rms[31]);
        }
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

