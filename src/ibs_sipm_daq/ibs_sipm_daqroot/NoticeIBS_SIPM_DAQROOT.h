#ifndef NKIBS_SIPM_DAQ_ROOT_H
#define NKIBS_SIPM_DAQ_ROOT_H

#include "TObject.h"

struct libusb_context;

class NKIBS_SIPM_DAQ : public TObject {
public:
	
  NKIBS_SIPM_DAQ();
  virtual ~NKIBS_SIPM_DAQ();

  int IBS_SIPM_DAQopen(void);
  void IBS_SIPM_DAQclose(int tcp_Handle);
  void IBS_SIPM_DAQreset(int tcp_Handle);
  void IBS_SIPM_DAQstart(int tcp_Handle);
  int IBS_SIPM_DAQread_RUN(int tcp_Handle);
  int IBS_SIPM_DAQread_DATA(int tcp_Handle, unsigned short *data);
  void IBS_SIPM_DAQread_MON(int tcp_Handle, int trig_mode, short *data);
  void IBS_SIPM_DAQwrite_HV(int tcp_Handle, float data);
  float IBS_SIPM_DAQread_HV(int tcp_Handle);
  void IBS_SIPM_DAQwrite_THR(int tcp_Handle, int data);
  int IBS_SIPM_DAQread_THR(int tcp_Handle);
  float IBS_SIPM_DAQread_TEMP(int tcp_Handle);
  int IBS_SIPM_DAQread_PED(int tcp_Handle);

  ClassDef(NKIBS_SIPM_DAQ, 0) // NKIBS_SIPM_DAQ wrapper class for root
};

#endif
