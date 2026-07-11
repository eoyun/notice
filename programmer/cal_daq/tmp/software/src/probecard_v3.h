#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>

#define PC3_VENDOR_ID  (0x0547)
#define PC3_PRODUCT_ID (0xCE03)

#define USB3_SF_READ   (0x82)
#define USB3_SF_WRITE  (0x06)

void PC3_USBinit(libusb_context **ctx);
void PC3_USBexit(libusb_context *ctx);
void PC3_Open(int sid, libusb_context *ctx);
void PC3_Close(int sid);
int PC3_Receive(int sid, unsigned int count, unsigned int addr, unsigned char *data);
int PC3_Write(int sid, unsigned int addr, unsigned int data);
unsigned int PC3_Read(int sid, unsigned int addr);
void PC3_Write_PWR(int sid, int avss, int avdd, int dvss, int dvdd, int dvdd33, int pwell, int sub);
void PC3_Read_PWR(int sid, int *avss, int *avdd, int *dvss, int *dvdd, int *dvdd33, int *pwell, int *sub);
void PC3_Write_SIG(int sid, int data, int mistral_o, int busy, int ctrl, int chipid, int por_dis, int rst_n, int hsdata, int dclk, int dctrl, int mclk, int dacmonv, int dacmoni);
void PC3_Read_SIG(int sid, int *data, int *mistral_o, int *busy, int *ctrl, int *chipid, int *por_dis, int *rst_n, int *hsdata, int *dclk, int *dctrl, int *mclk, int *dacmonv, int *dacmoni);
void PC3_Write_V(int sid, int avdd, int dvdd, int pwell, int sub, float avdd_value, float dvdd_value, float pwell_value, float sub_value);
void PC3_Read_I(int sid, float *avdd, float *dvdd, float *dvdd33, float *pwell, float *sub, float *dacmonv, float *dacmoni);
void PC3_Write_SHORT(int sid, int pwell, int sub);
void PC3_Write_MODE(int sid, int value);
void PC3_Write_DIR(int sid, int data, int busy);
void PC3_Write_LED(int sid, int value);
unsigned int PC3_Read_STATUS(int sid);
unsigned int PC3_Read_LOCK(int sid);
void PC3_Write_READBACK(int sid, unsigned int data);
unsigned int PC3_Read_READBACK(int sid);



