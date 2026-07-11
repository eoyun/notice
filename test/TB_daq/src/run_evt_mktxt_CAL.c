#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "NoticeCALTCB.h"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

int main(int argc, char *argv[])
{
  signal(SIGINT, SIG_IGN);
  int sid = 0;
  int nsec=2147483647;
  int sec;
  int ndaq=0;
  int i;
  char flag_name[512];
  char flag_file_name[512];
  int flag;
  FILE *fp_mid;
  int mid[20];
  int nevt = 0;

  if(access("/Users/yhep/scratch/notice/test/TB_daq/turn_on_mid.txt",F_OK)==0){
    fp_mid= fopen("/Users/yhep/scratch/notice/test/TB_daq/turn_on_mid.txt","rt");
    while(fscanf(fp_mid,"%d",&mid[ndaq]) != EOF){
      ndaq+=1;
	  printf("ndaq is %d\n",ndaq);
    }
    for ( i = 0; i < ndaq; i++){
      printf("total number of daq is %d, mid is %d\n",ndaq,mid[i]);
    }
	fclose(fp_mid);
	system("rm turn_on_mid.txt");
  nevt = atoi(argv[1]);
  } else{
    if (argc < 2) {
      printf("Enter number of DAQ : ");
      scanf("%d",&ndaq);
    }
    else {
      ndaq = atoi(argv[1]);
    }
  }
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
    printf("%d sec passed\n", sec);
    fflush(stdout);
    if (access("/Users/yhep/scratch/notice/test/TB_daq/KILLME",F_OK)==0){
      system("rm /Users/yhep/scratch/notice/test/TB_daq/KILLME");
      system("touch /Users/yhep/scratch/notice/test/apix_daq/astropix-python/stop.flag");
      break;
    }
	//printf("not KILLME\n");
	flag = 0;
	for (i=0;i<20;i++){
      sprintf(flag_file_name,"/Users/yhep/scratch/notice/test/TB_daq/KILLTCB%d",i+1);
	 // printf("%s\n",flag_name);
	  if (access(flag_file_name,F_OK)==0) flag=flag+1;
	  flag_file_name[0]='\0';
	}
	//printf("flag is %d",flag);
	//printf("111111111\n");
    if (flag==ndaq) {
	 // printf("dbgdbgdbg\n");
      for (i=0;i<20;i++){
        sprintf(flag_name,"rm /Users/yhep/scratch/notice/test/TB_daq/KILLTCB%d",i+1);
        sprintf(flag_file_name,"/Users/yhep/scratch/notice/test/TB_daq/KILLTCB%d",i+1);
		//printf("%s\n",flag_name);
		if (access(flag_file_name,F_OK)==0) system(flag_name);
		flag_name[0]='\0';
		flag_file_name[0]='\0';
	  }
	  printf("run is done!!\n");
	  break;
	}



  }

  // stop DAQ
  CALTCBstop_DAQ(sid);

  // close TCB
  CALTCBclose(sid);
  
  // exit LIBUSB
  USB3Exit();
  printf(RED "\n========================================================================================\n" RESET);
	printf(YELLOW "                               ⚠️  Received termination ⚠️\n" RESET);
  printf(YELLOW "               number of evt : %d | taken time : %d sec | evt rate : %d Hz \n" RESET,nevt, sec, nevt/sec );
	printf(GREEN "                               Ending the run gracefully... \n" RESET);
	printf(RED  "========================================================================================\n" RESET);

	// ASCII Art for exit message
	printf("\n");
	printf(BLUE "  ██████  ██████  ███████  █████  ███    ███     ██   ██ ███████ ██████  ███████  █████ \n");
	printf(BLUE "  ██   ██ ██   ██ ██      ██   ██ ████  ████     ██ ██   ██   ██ ██   ██ ██      ██   ██\n");
	printf(BLUE "  ██   ██ ██████  █████   ███████ ██ ████ ██     ███     ██   ██ ██████  █████   ███████\n");
	printf(BLUE "  ██   ██ ██   ██ ██      ██   ██ ██  ██  ██     ██ ██   ██   ██ ██   ██ ██      ██   ██\n");
	printf(BLUE "  ██████  ██   ██ ███████ ██   ██ ██      ██     ██   ██ ███████ ██   ██ ███████ ██   ██\n");

  printf(RESET);

  return 0;
}

