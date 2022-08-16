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
  int total_nevt;
  char flag_name[256];
  char mkdir_data_folder_wave[256];
  char mkdir_data_folder_fast[256];
  int wave_split=10;
  int fast_split=100;
  int file_num_wave=0;
  int file_num_fast=0;
  int wave_tmp=0;
  int fast_tmp=0;
  if (argc < 3) {
    printf("Enter mid : ");
    scanf("%d", &mid);
	printf("Enter number of evt : ");
	scanf("%d", &total_nevt);
  }
  else{ 
    mid = atoi(argv[1]);
	total_nevt = atoi(argv[2]);
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
  //if (mid == 7) mid_file_name =1;
  //if (mid == 10) mid_file_name =2;
  sprintf(mkdir_data_folder_wave,"mkdir -p /Users/drc_daq/scratch/202208TB/SSD/SSD_Run_%lu/Run_%lu_Wave/Run_%lu_Wave_MID_%d/",run_number,run_number,run_number,mid);
  sprintf(mkdir_data_folder_fast,"mkdir -p /Users/drc_daq/scratch/202208TB/SSD/SSD_Run_%lu/Run_%lu_Fast/Run_%lu_Fast_MID_%d/",run_number,run_number,run_number,mid);
  printf("%s\n",mkdir_data_folder_wave);
  printf("%s\n",mkdir_data_folder_fast);
  
  system(mkdir_data_folder_wave);

  system(mkdir_data_folder_fast);
  //sprintf(wave_filename, "/media/eoyun/Expansion/DAQ_data/220723/cal_wave_%d_%lu.dat",mid_file_name,run_number);
  //sprintf(fast_filename, "/media/eoyun/Expansion/DAQ_data/220723/cal_fast_%d_%lu.dat",mid_file_name,run_number);
  sprintf(wave_filename, "/Users/drc_daq/scratch/202208TB/SSD/SSD_Run_%lu/Run_%lu_Wave/Run_%lu_Wave_MID_%d/Run_%lu_Wave_MID_%d_FILE_%d.dat",run_number,run_number,run_number,mid,run_number,mid,file_num_wave);
  sprintf(fast_filename, "/Users/drc_daq/scratch/202208TB/SSD/SSD_Run_%lu/Run_%lu_Fast/Run_%lu_Fast_MID_%d/Run_%lu_Fast_MID_%d_FILE_%d.dat",run_number,run_number,run_number,mid,run_number,mid,file_num_fast);


  //evt flag name
  sprintf(flag_name, "touch KILLTCB%d",mid);

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
	  if(wave_evt - wave_tmp > wave_split){
        fclose(wave_fp);
		rewind(wave_fp);
		wave_filename[0]='\0';
		wave_tmp = wave_evt;
		file_num_wave+=1;
		printf("wave file split!! wave evt is %d\n",wave_evt);
        sprintf(wave_filename, "/Users/drc_daq/scratch/202208TB/SSD/SSD_Run_%lu/Run_%lu_Wave/Run_%lu_Wave_MID_%d/Run_%lu_Wave_MID_%d_FILE_%d.dat",run_number,run_number,run_number,mid,run_number,mid,file_num_wave);
		wave_fp = fopen(wave_filename,"wb");

	  }
	  
	  if(fast_evt - fast_tmp > fast_split){
        fclose(fast_fp);
		rewind(fast_fp);
		fast_filename[0]='\0';
		fast_tmp = fast_evt;
		file_num_fast+=1;
		printf("fast file split!! fast evt is %d\n",fast_evt);
        sprintf(fast_filename, "/Users/drc_daq/scratch/202208TB/SSD/SSD_Run_%lu/Run_%lu_Fast/Run_%lu_Fast_MID_%d/Run_%lu_Fast_MID_%d_FILE_%d.dat",run_number,run_number,run_number,mid,run_number,mid,file_num_fast);
		fast_fp = fopen(fast_filename,"wb");

	  }
	  
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
      if (wave_evt>total_nevt){
        printf("%s\n",flag_name);
		system(flag_name);
		total_nevt = 2147483647;
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

