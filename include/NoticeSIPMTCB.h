#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SIPMTCB_VENDOR_ID  (0x0547)
#define SIPMTCB_PRODUCT_ID (0x1501)

#define USB3_SF_READ   (0x82)
#define USB3_SF_WRITE  (0x06)

extern void USB3Init(void);
extern void USB3Exit(void);
extern int SIPMTCBopen(int sid);
extern void SIPMTCBclose(int sid);
extern void SIPMTCBreset(int sid);
extern void SIPMTCBstart_DAQ(int sid);
extern void SIPMTCBstop_DAQ(int sid);
extern void SIPMTCBsend_TRIG(int sid);
extern unsigned long SIPMTCBread_RUN(int sid);
extern void SIPMTCBwrite_FRAME(int sid, unsigned long data);
extern unsigned long SIPMTCBread_FRAME(int sid);
extern void SIPMTCBwrite_SCAN_TIME(int sid, unsigned long data);
extern unsigned long SIPMTCBread_SCAN_TIME(int sid);
extern void SIPMTCBwrite_TRIGGER_MODE(int sid, unsigned long data);
extern unsigned long SIPMTCBread_TRIGGER_MODE(int sid);
extern void SIPMTCBread_LINK(int sid, unsigned long *data);
extern void SIPMTCBread_MID(int sid, unsigned long *data);
extern unsigned long SIPMTCBread_DATASIZE(int sid);
extern void SIPMTCBread_DATA(int sid, unsigned long data_size, char* data);
extern void SIPMTCBwrite_HV(int sid, unsigned long mid, unsigned long ch, float data);
extern float SIPMTCBread_HV(int sid, unsigned long mid, unsigned long ch);
extern float SIPMTCBread_TEMP(int sid, unsigned long mid, unsigned long ch);
extern unsigned long SIPMTCBread_PED(int sid, unsigned long mid, unsigned long ch);
extern void SIPMTCBwrite_THR(int sid, unsigned long mid, unsigned long ch, unsigned long data);
extern unsigned long SIPMTCBread_THR(int sid, unsigned long mid, unsigned long ch);
extern void SIPMTCBwrite_PSW(int sid, unsigned long mid, unsigned long data);
extern unsigned long SIPMTCBread_PSW(int sid, unsigned long mid);
extern void SIPMTCBwrite_RISETIME(int sid, unsigned long mid, unsigned long data);
extern unsigned long SIPMTCBread_RISETIME(int sid, unsigned long mid);
extern void SIPMTCBwrite_PSD_DLY(int sid, unsigned long mid, unsigned long data);
extern unsigned long SIPMTCBread_PSD_DLY(int sid, unsigned long mid);
extern void SIPMTCBwrite_PSD_THR(int sid, unsigned long mid, unsigned long ch, float data);
extern float SIPMTCBread_PSD_THR(int sid, unsigned long mid, unsigned long ch);
extern void SIPMTCBalign_ADC(int sid, unsigned long mid);
extern void SIPMTCBalign_DRAM(int sid, unsigned long mid);

#ifdef __cplusplus
}
#endif




