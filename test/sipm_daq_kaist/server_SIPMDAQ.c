#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "NoticeSIPMTCB.h"

#define TCB_BUF_SIZE (131072)           // in byte
static int client_data;

// subroutines
char TCP_get_CMD(int client_fd);
void TCP_send_DATA(int client_fd, char cmd, int length, char *data);
int TCP_data_AVAILABLE(int data_size_read);
void TCP_send_IMAGE(int client_fd, int *image_data);

// get command from client
char TCP_get_CMD(int client_fd)
{
  char rbuf[256];
  int received_len;
  int length;
  int check_sum_read;
  int check_sum_sent;
  int i;
  char cmd;

  received_len = recv(client_fd, rbuf, 256, 0);
  if (received_len < 0) 
    return 'Q';
  else if (received_len > 0) { 
    // check header
    if (rbuf[0] != 0x02)
      return 'X';

    // get length
    length = rbuf[2] & 0xFF;
        
    // check checksum
    check_sum_read = length;
    check_sum_read = check_sum_read + (rbuf[1] & 0xFF);
    check_sum_read = check_sum_read + (rbuf[3] & 0xFF);
    for (i = 0; i < length; i++) 
      check_sum_read = check_sum_read + (rbuf[4 + i] & 0xFF);
    check_sum_read = check_sum_read & 0xFF;
    check_sum_sent = rbuf[length + 4] & 0xFF;
    if (check_sum_read != check_sum_sent)
      return 'X';
          
    // check trailer
    if (rbuf[length + 5] != 0x03)
      return 'X';
        
    // get cmd
    cmd = rbuf[1] & 0xFF;
         
    // get data
    client_data = 0;
    for (i = 0; i < length; i++) 
      client_data = client_data + (rbuf[4 + i] & 0xFF);
      
    return cmd;
  }
  
  return 0;
}

// send data to client
void TCP_send_DATA(int client_fd, char cmd, int length, char *data)
{
  char wbuf[1024];
  int byte_to_send;
  int i;
  int check_sum;
  
  // byte to send
  byte_to_send = length + 6;

  wbuf[0] = 0x02;
  wbuf[1] = cmd & 0xFF;
  wbuf[2] = length & 0xFF;
  wbuf[3] = (length >> 8) & 0xFF;
  for (i = 0; i < length; i++)
    wbuf[i + 4] = data[i];
    
  // get check sum
  check_sum = wbuf[1] & 0xFF;
  check_sum = check_sum + (wbuf[2] & 0xFF);
  check_sum = check_sum + (wbuf[3] & 0xFF);
  for (i = 0; i < length; i++)
    check_sum = check_sum + (wbuf[i + 4] & 0xFF);
  wbuf[length + 4] = check_sum & 0xFF;
  wbuf[length + 5] = 0x03;

  send(client_fd, wbuf, byte_to_send, 0);
}

// check image data available
int TCP_data_AVAILABLE(int data_size_read)
{
  FILE *fp_size;
  int data_size_written;

  fp_size = fopen("/dev/shm/sipm_size.txt", "rt");
  fscanf(fp_size, "%d", &data_size_written);
  fclose(fp_size);
  if (data_size_written > data_size_read) 
    return 1;
  else {
    if ((access("/dev/shm/sipm_daq_done.txt", 0)) == 0) 
      return 2;
    else
      return 0;
  }
}

// send image data
void TCP_send_IMAGE(int client_fd, int *image_data)
{
  int line_number;
  int image_n[144];
  int image_g[144];
  int ref_n;
  int ref_g;
  int mod;
  char wbuf[1024];
  int i;

  line_number = image_data[0] - 1;

  for (mod = 0; mod < 9; mod++) {
    image_n[16 * mod + 0] = image_data[64 * mod + 2] + image_data[64 * mod + 16];
    image_n[16 * mod + 1] = image_data[64 * mod + 4] + image_data[64 * mod + 14];
    image_n[16 * mod + 2] = image_data[64 * mod + 6] + image_data[64 * mod + 12];
    image_n[16 * mod + 3] = image_data[64 * mod + 8] + image_data[64 * mod + 10];
    image_n[16 * mod + 4] = image_data[64 * mod + 18] + image_data[64 * mod + 32];
    image_n[16 * mod + 5] = image_data[64 * mod + 20] + image_data[64 * mod + 30];
    image_n[16 * mod + 6] = image_data[64 * mod + 22] + image_data[64 * mod + 28];
    image_n[16 * mod + 7] = image_data[64 * mod + 24] + image_data[64 * mod + 26];
    image_n[16 * mod + 8] =  image_data[64 * mod + 34] + image_data[64 * mod + 48];
    image_n[16 * mod + 9] =  image_data[64 * mod + 36] + image_data[64 * mod + 46];
    image_n[16 * mod + 10] = image_data[64 * mod + 38] + image_data[64 * mod + 44];
    image_n[16 * mod + 11] = image_data[64 * mod + 40] + image_data[64 * mod + 42];
    image_n[16 * mod + 12] = image_data[64 * mod + 50] + image_data[64 * mod + 64];
    image_n[16 * mod + 13] = image_data[64 * mod + 52] + image_data[64 * mod + 62];
    image_n[16 * mod + 14] = image_data[64 * mod + 54] + image_data[64 * mod + 60];
    image_n[16 * mod + 15] = image_data[64 * mod + 56] + image_data[64 * mod + 58];

    image_g[16 * mod + 0] = image_data[64 * mod + 3] + image_data[64 * mod + 17];
    image_g[16 * mod + 1] = image_data[64 * mod + 5] + image_data[64 * mod + 15];
    image_g[16 * mod + 2] = image_data[64 * mod + 7] + image_data[64 * mod + 13];
    image_g[16 * mod + 3] = image_data[64 * mod + 9] + image_data[64 * mod + 11];
    image_g[16 * mod + 4] = image_data[64 * mod + 19] + image_data[64 * mod + 33];
    image_g[16 * mod + 5] = image_data[64 * mod + 21] + image_data[64 * mod + 31];
    image_g[16 * mod + 6] = image_data[64 * mod + 23] + image_data[64 * mod + 29];
    image_g[16 * mod + 7] = image_data[64 * mod + 25] + image_data[64 * mod + 27];
    image_g[16 * mod + 8] =  image_data[64 * mod + 35] + image_data[64 * mod + 49];
    image_g[16 * mod + 9] =  image_data[64 * mod + 37] + image_data[64 * mod + 47];
    image_g[16 * mod + 10] = image_data[64 * mod + 39] + image_data[64 * mod + 45];
    image_g[16 * mod + 11] = image_data[64 * mod + 41] + image_data[64 * mod + 43];
    image_g[16 * mod + 12] = image_data[64 * mod + 51] + image_data[64 * mod + 65];
    image_g[16 * mod + 13] = image_data[64 * mod + 53] + image_data[64 * mod + 63];
    image_g[16 * mod + 14] = image_data[64 * mod + 55] + image_data[64 * mod + 61];
    image_g[16 * mod + 15] = image_data[64 * mod + 57] + image_data[64 * mod + 59];
  }

  ref_n = image_n[128] + image_n[129] + image_n[130] + image_n[131]
        + image_n[132] + image_n[133] + image_n[134] + image_n[135]
        + image_n[136] + image_n[137] + image_n[138] + image_n[139]
        + image_n[140] + image_n[141] + image_n[142] + image_n[143];
  ref_n = ref_n / 16;

  ref_g = image_g[128] + image_g[129] + image_g[130] + image_g[131]
        + image_g[132] + image_g[133] + image_g[134] + image_g[135]
        + image_g[136] + image_g[137] + image_g[138] + image_g[139]
        + image_g[140] + image_g[141] + image_g[142] + image_g[143];
  ref_g = ref_g / 16;

  wbuf[0] = line_number & 0xFF;
  wbuf[1] = (line_number >> 8) & 0xFF;
  wbuf[2] = (line_number >> 16) & 0xFF;
  wbuf[3] = (line_number >> 24) & 0xFF;

  wbuf[4] = ref_n & 0xFF;
  wbuf[5] = (ref_n >> 8) & 0xFF;
  wbuf[6] = (ref_n >> 16) & 0xFF;
  wbuf[7] = (ref_n >> 24) & 0xFF;

  wbuf[8] = ref_g & 0xFF;
  wbuf[9] = (ref_g >> 8) & 0xFF;
  wbuf[10] = (ref_g >> 16) & 0xFF;
  wbuf[11] = (ref_g >> 24) & 0xFF;
  
  for (i = 0; i < 116; i++) {
    wbuf[8 * i + 12] = image_n[i] & 0xFF;
    wbuf[8 * i + 13] = (image_n[i] >> 8) & 0xFF;
    wbuf[8 * i + 14] = (image_n[i] >> 16) & 0xFF;
    wbuf[8 * i + 15] = (image_n[i] >> 24) & 0xFF;

    wbuf[8 * i + 16] = image_g[i] & 0xFF;
    wbuf[8 * i + 17] = (image_g[i] >> 8) & 0xFF;
    wbuf[8 * i + 18] = (image_g[i] >> 16) & 0xFF;
    wbuf[8 * i + 19] = (image_g[i] >> 24) & 0xFF;
  }  

  // send data
  TCP_send_DATA(client_fd, 'D', 940, wbuf);
}

int main(void)
{
  struct sockaddr_in server_addr; 
  struct sockaddr_in client_addr; 
  int server_fd;
  unsigned int client_size;
  int client_fd;
  char client_buf[20];
  int connected;
  char client_cmd;
  char data[1024];
  int daq_mode = 0;
  int scan_time;
  char cmd[256];
  int data_size_read = 0;
  int image_data[1024];
  FILE *fp_data;
  int daq_started = 0;
  int data_valid = 0;
  int final_data = 0;

  if ((access("/dev/shm/sipm_daq_ready.txt", 0)) == 0) 
    system("rm /dev/shm/sipm_daq_ready.txt");
  if ((access("/dev/shm/sipm_daq_done.txt", 0)) == 0) 
    system("rm /dev/shm/sipm_daq_done.txt");
  if ((access("/dev/shm/sipm_daq_stop.txt", 0)) == 0) 
    system("rm /dev/shm/sipm_daq_stop.txt");
  
  // register server
  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    return -1;

  memset(&server_addr, 0x00, sizeof(server_addr));
 
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("192.168.0.7");
  server_addr.sin_port = htons(5000);

  // bind
  if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0) 
    return -1;

  // listen   
  if(listen(server_fd, 5) < 0) 
    return -1;
 
  client_size = sizeof(client_addr);
  
  while(1) {
    // wait for client being connected
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_size);
    if(client_fd < 0) 
      return -1;

    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_buf, sizeof(client_buf));

    connected = 1;
    printf("client connected\n");
    while (connected) {
      client_cmd = TCP_get_CMD(client_fd);
      
      // wrong command
      if (client_cmd == 'X') { 
        data[0] = 0;
        TCP_send_DATA(client_fd, 'X', 1, data);
      }
      
      // check DAQ status
      else if (client_cmd == 'S') {
        if ((access("/dev/shm/sipm_daq_ready.txt", 0)) == 0) 
          data[0] = 0x01;
        else
          data[0] = 0x02;
        TCP_send_DATA(client_fd, 'S', 1, data);
      }
      
      // DAQ mode
      else if (client_cmd == 'M') {
        if (client_data == 0x11) {
          data[0] = daq_mode & 0xFF;
          TCP_send_DATA(client_fd, 'M', 1, data);
        }
        else {
          daq_mode = client_data;
          data[0] = daq_mode;
          TCP_send_DATA(client_fd, 'M', 1, data);
        }
      }
      
      // scan time
      else if (client_cmd == 'T') {
        scan_time = client_data;
        USB3Init();
        SIPMTCBopen(0);
        SIPMTCBwrite_SCAN_TIME(0, scan_time);
        SIPMTCBclose(0);
        USB3Exit();
        data[0] = scan_time & 0xFF;
        data[1] = (scan_time >> 8) & 0xFF;
        data[2] = (scan_time >> 16) & 0xFF;
        TCP_send_DATA(client_fd, 'T', 3, data);
      }
      
      // begin image acquisition
      else if (client_cmd == 'B') {
        if ((access("/dev/shm/sipm_daq_ready.txt", 0)) == 0) 
          system("rm /dev/shm/sipm_daq_ready.txt");
        if ((access("/dev/shm/sipm_daq_done.txt", 0)) == 0) 
          system("rm /dev/shm/sipm_daq_done.txt");
        if ((access("/dev/shm/sipm_size.txt", 0)) == 0) 
          system("rm /dev/shm/sipm_size.txt");
        sprintf(cmd, "./run_SIPMDAQ.exe %d&", daq_mode);
        system(cmd);
        data[0] = 0;
        TCP_send_DATA(client_fd, 'B', 1, data);
        data_size_read = 0;
        final_data = 0;
        fp_data = fopen("sipm_tcb.dat", "rb");
        // send first data
        daq_started = 0;
        while (!daq_started) {
          if ((access("/dev/shm/sipm_size.txt", 0)) == 0)
            daq_started = 1;
        }
        data_valid = 0;
        while (!data_valid) 
          data_valid = TCP_data_AVAILABLE(data_size_read);
        if (data_valid == 1) {
          fread(image_data, 4, 1024, fp_data);
          TCP_send_IMAGE(client_fd, image_data);
          data_size_read = data_size_read + 1;
        }
        else {
          data[0] = 0;
          TCP_send_DATA(client_fd, 'D', 1, data);
          final_data = 1;
          system("rm /dev/shm/sipm_daq_done.txt");
          fclose(fp_data);
        }
      }
      
      // end image acquisition
      else if (client_cmd == 'E') {
        system("touch /dev/shm/sipm_daq_stop.txt");
        data[0] = 0;
        TCP_send_DATA(client_fd, 'E', 1, data);
      }

      // disconnect from server
      else if (client_cmd == 'Q') {
        printf("client disconnected\n");
        connected = 0;
      }

      // send image data
      else if (client_cmd == 'D') {
        if (!final_data) {      
          if (client_data == 0x0A) {
            data_valid = 0;
            while (!data_valid) 
              data_valid = TCP_data_AVAILABLE(data_size_read);
            if (data_valid == 1) {
              fread(image_data, 4, 1024, fp_data);
              TCP_send_IMAGE(client_fd, image_data);
              data_size_read = data_size_read + 1;
            }
            else {
              data[0] = 0;
              TCP_send_DATA(client_fd, 'D', 1, data);
              final_data = 1;
            }
          }
          else {
            data_valid = 0;
            while (!data_valid) 
              data_valid = TCP_data_AVAILABLE(data_size_read);
            if (data_valid == 1) {
              TCP_send_IMAGE(client_fd, image_data);
            }
            else {
              data[0] = 0;
              TCP_send_DATA(client_fd, 'D', 1, data);
              final_data = 1;
              system("rm /dev/shm/sipm_daq_done.txt");
              fclose(fp_data);
            }
          }
        }
      }
    }

    // close client connection
    close(client_fd);
  }

  return 0;
}

