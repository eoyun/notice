#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "NoticeCALDAQ.h"
#include "NoticeCALTCB.h"
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 

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
  char adc[65536];
 // char header_buf[64];
  int sock;
  struct sockaddr_in serv_addr;
  int adc_bin =0;

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

  //socket connect
  
  sock = socket(PF_INET, SOCK_STREAM, 0);
  
  if(sock==-1)
	  printf("error on socket\n");

  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("192.168.0.51");  
  serv_addr.sin_port = htons(37326);

  if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1){
  	printf("ERROR on connect\n");
  }

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
  int count=0;
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
        // read & send data
        CALDAQread_DATA(mid, wave_data_size, data);

        /*for(int idx =0 ; idx <1024; idx++){ 
          for(int idata=0; idata<64; idata++){
	       adc_bin = (idx*64) + idata;			  
               adc[adc_bin] = data[adc_bin];	       
	  }
	      write(sock,adc,64);	     
        }*/
	//send data
	/write(sock,data,wave_data_size*1024);
	//write(sock,data,wave_data_size);
	printf("%ld byte sending\n",1024*wave_data_size);
	//sleep(2);
        fwrite(data, 1, wave_data_size * 1024, wave_fp);
	//count +=1;
        wave_evt = wave_evt + (wave_data_size / 64);   // 1 event = 64 kB
        printf("Waveform %d events are taken\n", wave_evt);
     // }
       // if (count == 64){
       // 	count=0;
       // 	wave_evt = wave_evt + 1;   // 1 event = 64 kB
       // 	printf("Waveform %d events are taken\n", wave_evt);}
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

