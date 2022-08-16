#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "NoticeCALTCB.h"

int main(int argc, char *argv[])
{
  int sid = 0;
  int nsec=2147483647;
  int sec;
  int ndaq=0;
  int i;
  char flag_name[256];
  char flag_file_name[256];
  int flag;
  FILE *fp_mid;
  int mid[15];

  if(access("turn_on_mid.txt",F_OK)==0){
    fp_mid= fopen("turn_on_mid.txt","rt");
    while(fscanf(fp_mid,"%d",&mid[ndaq]) != EOF){
      ndaq+=1;
    }
    for ( i = 0; i < ndaq; i++){
      printf("total number of daq is %d, mid is %d\n",ndaq,mid[i]);
    }
	fclose(fp_mid);
	system("rm turn_on_mid.txt");
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
    if (access("KILLME",F_OK)==0){
      system("rm KILLME");
      break;
    }
	//printf("not KILLME\n");
	flag = 0;
	for (i=0;i<15;i++){
      sprintf(flag_name,"KILLTCB%d",i+1);
	 // printf("%s\n",flag_name);
	  if (access(flag_file_name,F_OK)==0) flag=flag+1;
	  flag_file_name[0]='\0';
	}
    if (flag==ndaq) {
      for (i=0;i<15;i++){
        sprintf(flag_name,"rm KILLTCB%d",i+1);
        sprintf(flag_file_name,"KILLTCB%d",i+1);
		//printf("%s\n",flag_name);
		if (access(flag_name,F_OK)==0) system(flag_name);
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

  return 0;
}

