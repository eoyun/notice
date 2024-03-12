#include <unistd.h>
#include <stdio.h>
#include "NoticeJBNU_DAQ.h"
#include "NoticeCALTCB.h"

int main(int argc, char *argv[])
{
  int sid=0;
  int mid;
  char filename[100];
  int run;
  int data_size;
  int data_written;
  char *data;
  int is_device_exist = 0;
  char anc_filename[256];
  FILE *anc_fp;
  FILE *anc_log_fp;
  FILE *runnum_fp;
  unsigned long run_number;
  unsigned long run_number_file;
  unsigned long anc_data_size;
  int anc_evt = 0;
  char mkdir_data_folder_anc[256];
  int anc_split=100000;
  int file_num_anc=0;
  int anc_tmp=0;
  char anc_log_name[256];
  int mid_file_name=0;
  struct timespec begin, point_anc;
  long now_anc;
  
  if (argc > 1) 
    mid = atoi(argv[1]);
  else {
    printf("Enter DAQ mid : ");
    scanf("%d", &mid);
  }
  printf("hello\n");
  runnum_fp = fopen("/Users/yhep/scratch/notice/test/TB_daq/runnum.txt","rt");
  fscanf(runnum_fp,"%ld",&run_number_file);
  run_number = run_number_file-1;


  sprintf(mkdir_data_folder_anc,"mkdir -p /Users/yhep/scratch/YUdaq/Run_%lu/Run_%lu_Anc/Run_%lu_Anc_MID_%d/",run_number,run_number,run_number,mid);
  printf("%s\n",mkdir_data_folder_anc);
  
  system(mkdir_data_folder_anc);

  mkdir_data_folder_anc[0]='\0';

  sprintf(mkdir_data_folder_anc,"/Users/yhep/scratch/YUdaq/Run_%lu/Run_%lu_Anc/Run_%lu_Anc_log/",run_number,run_number,run_number);
  if((access(mkdir_data_folder_anc,0))!=0) {
    mkdir_data_folder_anc[0]='\0';
    sprintf(mkdir_data_folder_anc,"mkdir -p /Users/yhep/scratch/YUdaq/Run_%lu/Run_%lu_Anc/Run_%lu_Anc_log/",run_number,run_number,run_number);
    system(mkdir_data_folder_anc);
  } else printf("directory exist!!\n");
  
  sprintf(anc_filename, "/Users/yhep/scratch/YUdaq/Run_%lu/Run_%lu_Anc/Run_%lu_Anc_MID_%d/Run_%lu_Anc_MID_%d_FILE_%d.dat",run_number,run_number,run_number,mid,run_number,mid,file_num_anc);

  sprintf(anc_log_name, "/Users/yhep/scratch/YUdaq/Run_%lu/Run_%lu_Anc/Run_%lu_Anc_log/Run_%lu_Anc_MID_%d_buffer.txt",run_number,run_number,run_number,run_number,mid);

  // open data file
  anc_fp = fopen(anc_filename, "wb");
  // open log file
  anc_log_fp = fopen(anc_log_name,"wt");
  // open file
  data_written = 0;
  
  // assign data array
  data = (char *)malloc(65536);
  
  USB3Init_daq();
  is_device_exist = DAQopen(mid);
 
  if (is_device_exist>0){
    run = 0;
    while (!run)
      run = DAQread_RUN(mid);
    clock_gettime(CLOCK_MONOTONIC,&begin);
    while (run) {
      // check data size
      data_size = DAQread_DATASIZE(mid);
     
      // read data
      if (data_size) {
        printf("data_size = %d\n", data_size);    
        if (data_size > 16) data_size = 16;
        DAQread_DATA(mid, data_size, data);
        fwrite(data, 1, data_size * 1024, anc_fp);
        data_written = data_written + data_size;
        printf("%d kbytes are taken\n", data_written);
      }
    
      run = DAQread_RUN(mid);
    }
    printf("DAQ is stopped and read remaining data\n");
    
    // read remaining data  
    data_size = 16;
    while (data_size) {
      // check data size
      data_size = DAQread_DATASIZE(mid);
     
      // read data
      if (data_size) {
        printf("data_size = %d\n", data_size);    
        if (data_size > 16) data_size = 16;
        DAQread_DATA(mid, data_size, data);
        fwrite(data, 1, data_size * 1024, anc_fp);
        data_written = data_written + data_size;
        printf("%d kbytes are taken\n", data_written);
      }
    }    
  } 
  // release data array
  free(data);

    printf("hello\n");
  // close file
  fclose(anc_fp);
  fclose(anc_log_fp);

  // close DAQ
  DAQclose(sid);
  
  // close usb
  USB3Exit_daq();
  
  return 0;
}


