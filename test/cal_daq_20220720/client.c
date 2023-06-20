#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 

#define Port 37326

int main(int argc, char ** argv){
  
  FILE *fp;
  char adc[65536];
  int sock;
  struct sockaddr_in serv_addr;
  sock = socket(PF_INET, SOCK_STREAM, 0);

  fp = fopen("Run_2954_Wave_MID_7_FILE_0.dat","rb");
  if(sock==-1)
	  printf("error on socket\n");

  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("192.168.0.51");  
  serv_addr.sin_port = htons(37326);

  if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1){
  	printf("ERROR on connect\n");
  }
  //while(1){
  for (int iter=0;iter<100;iter++){
/*
	int n_count = 0;
	
	for(int idx = 0; idx < 64; idx++) {
	    for (int itext =0; itext<16; itext++){
	      n_count++;
	    }
  */   
	fread(adc,1,65536,fp);
	//printf("send message \n");
	write(sock,adc,65536);
	printf("event : %d\n", iter+1);
	//sleep(0.2);
	
  //	}
  }
  fclose(fp);
  close(sock);
  return 0; 
}
