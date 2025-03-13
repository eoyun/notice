// internal functions *********************************************************************************
void TCBstart_DRAM(int tcp_Handle, unsigned long mid);
void TCBstop_DRAM(int tcp_Handle, unsigned long mid);
int MINITCB_V2read_Module_DRAM_READY(int tcp_Handle, unsigned long mid);
void TCBsetup_ADC(int tcp_Handle, unsigned long mid, int addr, int data);
int MINITCB_V2read_Module_ADC_ALIGN(int tcp_Handle, unsigned long mid, int ch);
void MINITCB_V2write_Module_DRAM_TEST(int tcp_Handle, unsigned long mid, int data);
int MINITCB_V2read_Module_DRAM_ALIGN(int tcp_Handle, unsigned long mid, int ch);
void TCBreset_REF_CLK(int tcp_Handle, unsigned long mid);
void TCBreset_ADC(int tcp_Handle, unsigned long mid);
void MINITCB_V2write_Module_ADC_IDLY(int tcp_Handle, unsigned long mid, int ch, int data);
void MINITCB_V2write_Module_ADC_BITSLIP(int tcp_Handle, unsigned long mid, int ch);
void MINITCB_V2write_Module_ADC_IMUX(int tcp_Handle, unsigned long mid, int ch, int data);
void MINITCB_V2write_Module_DRAM_IDLY(int tcp_Handle, unsigned long mid, int ch, int data);
void MINITCB_V2write_Module_DRAM_BITSLIP(int tcp_Handle, unsigned long mid, int ch);


// start DRAM
void TCBstart_DRAM(int tcp_Handle, unsigned long mid)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x20000001, 1);
}

// stop DRAM
void TCBstop_DRAM(int tcp_Handle, unsigned long mid)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x20000001, 0);
}

// read DRAM ready
int MINITCB_V2read_Module_DRAM_READY(int tcp_Handle, unsigned long mid)
{
  return MINITCB_V2read_Module(tcp_Handle, mid, 0x20000001);
}

// write acquisition time
void MINITCB_V2write_Module_ACQ_TIME(int tcp_Handle, int data)
{
  MINITCB_V2write_Module(tcp_Handle, 0, 0x20000002, data);
}

// read acquisition time
int MINITCB_V2read_Module_ACQ_TIME(int tcp_Handle)
{
  return MINITCB_V2read_Module(tcp_Handle, 0, 0x20000002);
}

// write High voltage
void MINITCB_V2write_Module_HV(int tcp_Handle, unsigned long mid, int ch, float data)
{
  float fval;
  int value;
  int addr;
  
  addr = (ch - 1) * 0x10000 + 0x20000002;
  fval = 4.63 * (data - 4.5);
  value = (int)(fval);
  if (value > 254)
    value = 254;
  else if (value < 0)
    value = 0;

  MINITCB_V2write_Module(tcp_Handle, mid, addr, value);
}

// read high voltage
float MINITCB_V2read_Module_HV(int tcp_Handle, unsigned long mid, int ch)
{
  int data;
  float value;
  int addr;

  addr = (ch - 1) * 0x10000 + 0x20000002;
  data = MINITCB_V2read_Module(tcp_Handle, mid, addr);
  value = data;
  value = value / 4.63 + 4.5;

  return value;
}

// write coincidence width
void MINITCB_V2write_Module_COIN_WIDTH(int tcp_Handle, unsigned long mid, int data)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x20000003, data);
}

// read coincidence width
int MINITCB_V2read_Module_COIN_WIDTH(int tcp_Handle, unsigned long mid)
{
  return MINITCB_V2read_Module(tcp_Handle, mid, 0x20000003);
}

// write multiplicity threshold
void MINITCB_V2write_Module_MULTI_THR(int tcp_Handle, unsigned long mid, int data)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x20000004, data);
}

// read multiplicity threshold
int MINITCB_V2read_Module_MULTI_THR(int tcp_Handle, unsigned long mid)
{
  return MINITCB_V2read_Module(tcp_Handle, mid, 0x20000004);
}

// write pedestal trigger interval, 0 ~ 65535 ms, when 0 disabled
void MINITCB_V2write_Module_PEDESTAL_TRIGGER_INTERVAL(int tcp_Handle, int data)
{
  MINITCB_V2write_Module(tcp_Handle, 0, 0x20000005, data);
}

// read pedestal trigger interval
int MINITCB_V2read_Module_PEDESTAL_TRIGGER_INTERVAL(int tcp_Handle)
{
  return MINITCB_V2read_Module(tcp_Handle, 0, 0x20000005);
}

// write discriminator threshold
void MINITCB_V2write_Module_DISC_THR(int tcp_Handle, unsigned long mid, int ch, int data)
{
  int addr;
  
  addr = (ch - 1) * 0x10000 + 0x20000005;
  MINITCB_V2write_Module(tcp_Handle, mid, addr, data);
}

// read discriminator threshold
int MINITCB_V2read_Module_DISC_THR(int tcp_Handle, unsigned long mid, int ch)
{
  int addr;
  
  addr = (ch - 1) * 0x10000 + 0x20000005;
  return MINITCB_V2read_Module(tcp_Handle, mid, addr);
}

// send software trigger
void TCBsend_TRIG(int tcp_Handle)
{
  MINITCB_V2write_Module(tcp_Handle, 0, 0x20000006, 0);
}

// write gate width
void MINITCB_V2write_Module_GATE_WIDTH(int tcp_Handle, unsigned long mid, int data)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x20000006, data);
}

// read gate width
int MINITCB_V2read_Module_GATE_WIDTH(int tcp_Handle, unsigned long mid)
{
  return MINITCB_V2read_Module(tcp_Handle, mid, 0x20000006);
}

// write trigger enable, self = 1, pedestal = 2, software = 4, external = 8
void MINITCB_V2write_Module_TRIGGER_ENABLE(int tcp_Handle, int data)
{
  MINITCB_V2write_Module(tcp_Handle, 0, 0x20000007, data);
}

// read trigger enable
int MINITCB_V2read_Module_TRIGGER_ENABLE(int tcp_Handle)
{
  return MINITCB_V2read_Module(tcp_Handle, 0, 0x20000007);
}

// write input delay
void MINITCB_V2write_Module_DELAY(int tcp_Handle, unsigned long mid, int data)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x20000007, data);
}

// read input deay
int MINITCB_V2read_Module_DELAY(int tcp_Handle, unsigned long mid)
{
  return MINITCB_V2read_Module(tcp_Handle, mid, 0x20000007);
}

// measure pedestal
void TCBmeasure_PED(int tcp_Handle, unsigned long mid)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x20000008, 0);
}

// read pedestal
int MINITCB_V2read_Module_PED(int tcp_Handle, unsigned long mid, int ch)
{
  int addr;
  
  addr = (ch - 1) * 0x10000 + 0x20000008;
  return MINITCB_V2read_Module(tcp_Handle, mid, addr);
}

// write peak sum width
void MINITCB_V2write_Module_PEAK_SUM_WIDTH(int tcp_Handle, unsigned long mid, int data)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x20000009, data);
}

// read peak sum width
int MINITCB_V2read_Module_PEAK_SUM_WIDTH(int tcp_Handle, unsigned long mid)
{
  return MINITCB_V2read_Module(tcp_Handle, mid, 0x20000009);
}

// write zero suppression
void MINITCB_V2write_Module_ZERO_SUP(int tcp_Handle, unsigned long mid, int data)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x2000000A, data);
}

// read zero suppression
int MINITCB_V2read_Module_ZERO_SUP(int tcp_Handle, unsigned long mid)
{
  return MINITCB_V2read_Module(tcp_Handle, mid, 0x2000000A);
}

// write waveform prescale
void MINITCB_V2write_Module_PRESCALE(int tcp_Handle, unsigned long mid, int data)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x2000000B, data);
}

// read waveform prescale
int MINITCB_V2read_Module_PRESCALE(int tcp_Handle, unsigned long mid)
{
  return MINITCB_V2read_Module(tcp_Handle, mid, 0x2000000B);
}

// read temperature
void MINITCB_V2read_Module_TEMP(int tcp_Handle, unsigned long mid, float *temp)
{
  int data;
  float fval;

  MINITCB_V2write_Module(tcp_Handle, mid, 0x2000000C, 0);

  data = MINITCB_V2read_Module(tcp_Handle, mid, 0x2000000C);
  fval = data;
  fval = fval * 0.04476 - 33.3;
  temp[0] = fval;

  data = MINITCB_V2read_Module(tcp_Handle, mid, 0x2001000C);
  fval = data;
  fval = fval * 0.04476 - 33.3;
  temp[1] = fval;
}

// write echo register
void MINITCB_V2write_Module_ECHO(int tcp_Handle, unsigned long mid, int data)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x2000000D, data);
}

// read echo register
int MINITCB_V2read_Module_ECHO(int tcp_Handle, unsigned long mid)
{
  return MINITCB_V2read_Module(tcp_Handle, mid, 0x2000000D);
}

void TCBsetup_ADC(int tcp_Handle, unsigned long mid, int addr, int data)
{
  int value;
  value = ((addr & 0xFFFF) << 16) | (data & 0xFFFF);
  MINITCB_V2write_Module(tcp_Handle, mid, 0x2000000E, value);
}

int MINITCB_V2read_Module_ADC_ALIGN(int tcp_Handle, unsigned long mid, int ch)
{
  int addr;
  
  addr = ch * 0x10000 + 0x2000000E;
  return MINITCB_V2read_Module(tcp_Handle, mid, addr);
}

// write DRAM test mode
void MINITCB_V2write_Module_DRAM_TEST(int tcp_Handle, unsigned long mid, int data)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x2000000F, data);
}

// read DRAM alignment pattern
int MINITCB_V2read_Module_DRAM_ALIGN(int tcp_Handle, unsigned long mid, int ch)
{
  int addr;
  
  addr = ch * 0x10000 + 0x2000000F;
  return MINITCB_V2read_Module(tcp_Handle, mid, addr);
}

// read DAQ link status
void MINITCB_V2read_Module_LINK_STATUS(int tcp_Handle, int *data)
{
  data[0] = MINITCB_V2read_Module(tcp_Handle, 0, 0x20000010);
  data[1] = MINITCB_V2read_Module(tcp_Handle, 0, 0x20000011);
}

// read DAQ mid
void MINITCB_V2read_Module_MID(int tcp_Handle, int *data)
{
  int i;

  for (i = 0; i < 40; i ++) 
    data[i] = MINITCB_V2read_Module(tcp_Handle, 0, 0x20000012 + i);
}

// reset delay reference clock
void TCBreset_REF_CLK(int tcp_Handle, unsigned long mid)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x20000020, 0);
}

// reset ADC
void TCBreset_ADC(int tcp_Handle, unsigned long mid)
{
  MINITCB_V2write_Module(tcp_Handle, mid, 0x20000021, 0);
}

void MINITCB_V2write_Module_ADC_IDLY(int tcp_Handle, unsigned long mid, int ch, int data)
{
  int addr;
  
  addr = ch * 0x10000 + 0x20000022;
  MINITCB_V2write_Module(tcp_Handle, mid, addr, data);
}

void MINITCB_V2write_Module_ADC_BITSLIP(int tcp_Handle, unsigned long mid, int ch)
{
  int addr;
  
  addr = ch * 0x10000 + 0x20000023;
  MINITCB_V2write_Module(tcp_Handle, mid, addr, 0);
}

void MINITCB_V2write_Module_ADC_IMUX(int tcp_Handle, unsigned long mid, int ch, int data)
{
  int addr;
  
  addr = ch * 0x10000 + 0x20000024;
  MINITCB_V2write_Module(tcp_Handle, mid, addr, data);
}

void MINITCB_V2write_Module_DRAM_IDLY(int tcp_Handle, unsigned long mid, int ch, int data)
{
  int addr;
  
  addr = ch * 0x10000 + 0x20000025;
  MINITCB_V2write_Module(tcp_Handle, mid, addr, data);
}

void MINITCB_V2write_Module_DRAM_BITSLIP(int tcp_Handle, unsigned long mid, int ch)
{
  int addr;
  
  addr = ch * 0x10000 + 0x20000026;
  MINITCB_V2write_Module(tcp_Handle, mid, addr, 0);
}

void TCBinit_ADC(int tcp_Handle, unsigned long mid)
{
  int retry;
  int all_aligned;
  int ch;
  int dly;
  int value;
  int flag;
  int count;
  int sum;
  int gdly;
  int bitslip;
  int aligned;
  int align_count;
  
  if (mid == MUON_DAQ_MID) 
    align_count = 8;
  else
    align_count = 12;

  // reset input delay ref clock
  TCBreset_REF_CLK(tcp_Handle, mid); 

  // reset ADC
  TCBreset_ADC(tcp_Handle, mid);

  // set ADC registers
  if (mid == MUON_DAQ_MID) {
    TCBsetup_ADC(tcp_Handle, mid, 0x03, 0x0002);
    TCBsetup_ADC(tcp_Handle, mid, 0x01, 0x0010);
    TCBsetup_ADC(tcp_Handle, mid, 0xC7, 0x8001);
    TCBsetup_ADC(tcp_Handle, mid, 0xDE, 0x01C0);
  }
  else {
    TCBsetup_ADC(tcp_Handle, mid, 0x00, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x01, 0x0010);
    TCBsetup_ADC(tcp_Handle, mid, 0xD1, 0x0240);
    TCBsetup_ADC(tcp_Handle, mid, 0x02, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x0F, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x14, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x1C, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x24, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x28, 0x8100);
    TCBsetup_ADC(tcp_Handle, mid, 0x29, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x2A, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x2B, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x38, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x42, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x45, 0x0000);
    TCBsetup_ADC(tcp_Handle, mid, 0x46, 0x8801);
  }

//  for (retry = 0; retry < 2; retry++) {
  for (retry = 0; retry < 1; retry++) {
    all_aligned = 0;

    for (ch = 0; ch < 4; ch++) {
      count = 0;
      sum = 0;
      flag = 0;
      aligned = 0;

      // set test pattern
      if (mid == MUON_DAQ_MID) {
        TCBsetup_ADC(tcp_Handle, mid, 0x45, 0x0001);
      }
      else {
        TCBsetup_ADC(tcp_Handle, mid, 0x26, 0x3330);
        TCBsetup_ADC(tcp_Handle, mid, 0x25, 0x0013);
      }
      
      // search delay
      for (dly = 0; dly < 32; dly++) {
        // set delay
        MINITCB_V2write_Module_ADC_IDLY(tcp_Handle, mid, ch, dly);

        // read bit_alignment status
        value = MINITCB_V2read_Module_ADC_ALIGN(tcp_Handle, mid, ch) & 0x1;

        if(value) {
          count = count + 1;
          sum = sum + dly;
          if(count > align_count) 
            flag = 1;
        }
        else{
          if (flag) 
            dly = 32;
          else {
            count = 0;
            sum = 0;
          }
        }
      }
        
      // get good center
      if (count > align_count) {
        gdly = sum / count;
        aligned = 1;
      }
      else
        gdly = 0;
    
      // set good delay
      MINITCB_V2write_Module_ADC_IDLY(tcp_Handle, mid, ch, gdly);

      // set word sync test pattern
      if (mid == MUON_DAQ_MID) {
        TCBsetup_ADC(tcp_Handle, mid, 0x45, 0x0002);
      }
      else {
        TCBsetup_ADC(tcp_Handle, mid, 0x26, 0x0000);
        TCBsetup_ADC(tcp_Handle, mid, 0x25, 0x0012);
      }

      // get bitslip
      MINITCB_V2write_Module_ADC_IMUX(tcp_Handle, mid, ch, 0);
      if (mid == MUON_DAQ_MID) {
        for (bitslip = 0; bitslip < 12; bitslip++) {
          if (bitslip > 5)
            MINITCB_V2write_Module_ADC_IMUX(tcp_Handle, mid, ch, 1);
          else
            MINITCB_V2write_Module_ADC_IMUX(tcp_Handle, mid, ch, 0);
            
          value = MINITCB_V2read_Module_ADC_ALIGN(tcp_Handle, mid, ch) & 0x2;

          if (value) {
            aligned = aligned + 1;
            bitslip = 12;
          }
          else 
            MINITCB_V2write_Module_ADC_BITSLIP(tcp_Handle, mid, ch);
        }
      }
      else {      
        for (bitslip = 0; bitslip < 8; bitslip++) {
          if (bitslip == 4)
            MINITCB_V2write_Module_ADC_IMUX(tcp_Handle, mid, ch, 1);
            
          value = MINITCB_V2read_Module_ADC_ALIGN(tcp_Handle, mid, ch) & 0x2;

          if (value) {
            aligned = aligned + 1;
            bitslip = 8;
          }
          else 
            MINITCB_V2write_Module_ADC_BITSLIP(tcp_Handle, mid, ch);
        }
      }
      
      all_aligned = all_aligned + aligned;
    }

    if (all_aligned == 8) 
      retry = 2;
  }
        
  // set ADC in normal output mode
  if (mid == MUON_DAQ_MID) {
    TCBsetup_ADC(tcp_Handle, mid, 0x45, 0x0000);
  }
  else {
    TCBsetup_ADC(tcp_Handle, mid, 0x25, 0x0000);
  }

  if (all_aligned == 8)
    printf("mid = %d : ADC is aligned.\n", mid);
  else
    printf("mid = %d : Fail to align ADC!\n", mid);
}

void TCBinit_DRAM(int tcp_Handle, unsigned long mid)
{
  int ready;
  int retry;
  int all_aligned;
  int ch;
  int dly;
  int value;
  int flag;
  int count;
  int sum;
  int aflag;
  int gdly;
  int bitslip;
  int aligned;
  
  ready = MINITCB_V2read_Module_DRAM_READY(tcp_Handle, mid);
  
  if (ready)
    TCBstop_DRAM(tcp_Handle, mid);

  TCBstart_DRAM(tcp_Handle, mid);
  
  ready = 0;
  while (!ready)
    ready = MINITCB_V2read_Module_DRAM_READY(tcp_Handle, mid);
  
  // turn on DRAM    
  TCBstart_DRAM(tcp_Handle, mid);
  
  // enter DRAM test mode
  MINITCB_V2write_Module_DRAM_TEST(tcp_Handle, mid, 1); 

  for (retry = 0; retry < 2; retry++) {
    all_aligned = 0;

    // send reset to iodelay  
    TCBreset_REF_CLK(tcp_Handle, mid); 

    // fill DRAM test pattern
    MINITCB_V2write_Module_DRAM_TEST(tcp_Handle, mid, 2); 

    for (ch = 0; ch < 2; ch++) {
      count = 0;
      sum = 0;
      flag = 0;
      aligned = 0;

      // search delay
      for (dly = 0; dly < 32; dly++) {
        // set delay
        MINITCB_V2write_Module_DRAM_IDLY(tcp_Handle, mid, ch, dly);

        // read DRAM test pattern
        MINITCB_V2write_Module_DRAM_TEST(tcp_Handle, mid, 3); 
        value = MINITCB_V2read_Module_DRAM_ALIGN(tcp_Handle, mid, ch); 

        aflag = 0;
        if (value == 0xFFAA5500)
          aflag = 1;
        else if (value == 0xAA5500FF)
          aflag = 1;
        else if (value == 0x5500FFAA)
          aflag = 1;
        else if (value == 0x00FFAA55)
          aflag = 1;
    
        if (aflag) {
          count = count + 1;
          sum = sum + dly;
          if (count > 8)
            flag = 1; 
        }
        else {
          if (flag) {
            dly = 32;
            aligned = 1;
          }
          else {
            count = 0;
            sum = 0;
          }
        }
      }

      // get good delay center
      if (count)
        gdly = sum / count;
      else
        gdly = 9;

      // set delay
      MINITCB_V2write_Module_DRAM_IDLY(tcp_Handle, mid, ch, gdly);
  
      // get bitslip
      for (bitslip = 0; bitslip < 4; bitslip++) {
        // read DRAM test pattern
        MINITCB_V2write_Module_DRAM_TEST(tcp_Handle, mid, 3); 
        value = MINITCB_V2read_Module_DRAM_ALIGN(tcp_Handle, mid, ch); 

        if (value == 0xFFAA5500) {
          aligned = aligned + 1;
          bitslip = 4;
        }
        else 
          MINITCB_V2write_Module_DRAM_BITSLIP(tcp_Handle, mid, ch);
      }
      
      all_aligned = all_aligned + aligned;
    }
    
    if (all_aligned == 4) 
      retry = 2;
  }

  if (all_aligned == 4)
    printf("mid = %d : DRAM is aligned.\n", mid);
  else
    printf("mid = %d : Fail to align DRAM!\n", mid);
   
  // exit DRAM test mode
  MINITCB_V2write_Module_DRAM_TEST(tcp_Handle, mid, 0); 
}


