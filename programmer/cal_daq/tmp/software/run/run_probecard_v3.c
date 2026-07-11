#include "run_probecard_v3.h"

int main(int argc, char *argv[])
{
  int sid;
  int com;

  // check # of argument
  if (argc < 2) {
    printf("please enter SID : ");
    scanf("%d", &sid);
  }
  else {
    sid = atoi(argv[1]);
    sid = sid & 0xFF;
  }

  // initialize USB3
  PC3_USBinit(0);

  // open probecard
  PC3_Open(sid, 0);

  com = 99;

  while(com) {
    printf("\n\n\n");
    printf(" 1. write PWR contact           2. read PWR contact\n");
    printf(" 3. write SIG contact           4. read SIG contact\n");
    printf(" 5. write regulator voltage     6. read regulator current\n");
    printf(" 7. write SHORT combination     8. write operation mode\n");
    printf(" 9. write bidirectional DIR    10. write LEDs\n");
    printf("11. write readback register    12. read readback register\n");
    printf("13. read contact status        14. read clock lock status\n");
    printf(" 0. quit\n");
    printf("enter commands : ");
    scanf("%d", &com);

    if (com == 1) 
      run_PC3_Write_PWR(sid);
    else if (com == 2) 
      run_PC3_Read_PWR(sid);
    else if (com == 3) 
      run_PC3_Write_SIG(sid);
    else if (com == 4) 
      run_PC3_Read_SIG(sid);
    else if (com == 5) 
      run_PC3_Write_V(sid);
    else if (com == 6) 
      run_PC3_Read_I(sid);
    else if (com == 7) 
      run_PC3_Write_SHORT(sid);
    else if (com == 8) 
      run_PC3_Write_MODE(sid);
    else if (com == 9) 
      run_PC3_Write_DIR(sid);
    else if (com == 10) 
      run_PC3_Write_LED(sid);
    else if (com == 11) 
      run_PC3_Write_READBACK(sid);
    else if (com == 12) 
      run_PC3_Read_READBACK(sid);
    else if (com == 13) 
      run_PC3_Read_STATUS(sid);
    else if (com == 14) 
      run_PC3_Read_LOCK(sid);
  }

  // close probecard
  PC3_Close(sid);

  // de-initialize USB3
  PC3_USBexit(0);

  return 0;
}

void run_PC3_Write_PWR(int sid)
{
  int avss;
  int avdd;
  int dvss;
  int dvdd;
  int dvdd33;
  int pwell;
  int sub;

  printf("enter signal to test <AVSS> <AVDD> <DVSS> <DVDD> <DVDD33> <PWELL> <SUB> : ");
  scanf("%X", &avss);
  scanf("%X", &avdd);
  scanf("%X", &dvss);
  scanf("%X", &dvdd);
  scanf("%X", &dvdd33);
  scanf("%X", &pwell);
  scanf("%X", &sub);
    
  PC3_Write_PWR(sid, avss, avdd, dvss, dvdd, dvdd33, pwell, sub);
}

void run_PC3_Read_PWR(int sid)
{
  int avss;
  int avdd;
  int dvss;
  int dvdd;
  int dvdd33;
  int pwell;
  int sub;

  PC3_Read_PWR(sid, &avss, &avdd, &dvss, &dvdd, &dvdd33, &pwell, &sub);

  printf("AVSS = %X\n", avss);
  printf("AVDD = %X\n", avdd);
  printf("DVSS = %X\n", dvss);
  printf("DVDD = %X\n", dvdd);
  printf("DVDD33 = %X\n", dvdd33);
  printf("PWELL = %X\n", pwell);
  printf("SUB = %X\n", sub);
}

void run_PC3_Write_SIG(int sid)
{
  int data;
  int mistral_o;
  int busy;
  int ctrl;
  int chipid;
  int por_dis;
  int rst_n;
  int hsdata;
  int dclk;
  int dctrl;
  int mclk;
  int dacmonv;
  int dacmoni;

  printf("enter signal to test <DATA> <MISTRAL_O> <BUSY> <CTRL> <CHIPID> <POR_DIS> <RST_N> <HSDATA> <DCLK> <DCTRL> <MCLK> <DACMONV> <DACMONI> : ");
  scanf("%X", &data);
  scanf("%X", &mistral_o);
  scanf("%X", &busy);
  scanf("%X", &ctrl);
  scanf("%X", &chipid);
  scanf("%X", &por_dis);
  scanf("%X", &rst_n);
  scanf("%X", &hsdata);
  scanf("%X", &dclk);
  scanf("%X", &dctrl);
  scanf("%X", &mclk);
  scanf("%X", &dacmonv);
  scanf("%X", &dacmoni);

  PC3_Write_SIG(sid, data, mistral_o, busy, ctrl, chipid, por_dis, rst_n, hsdata, dclk, dctrl, mclk, dacmonv, dacmoni);
}

void run_PC3_Read_SIG(int sid)
{
  int data;
  int mistral_o;
  int busy;
  int ctrl;
  int chipid;
  int por_dis;
  int rst_n;
  int hsdata;
  int dclk;
  int dctrl;
  int mclk;
  int dacmonv;
  int dacmoni;

  PC3_Read_SIG(sid, &data, &mistral_o, &busy, &ctrl, &chipid, &por_dis, &rst_n, &hsdata, &dclk, &dctrl, &mclk, &dacmonv, &dacmoni);

  printf("DATA = %X\n", data);
  printf("MISTRAL_O = %X\n", mistral_o);
  printf("BUSY = %X\n", busy);
  printf("CTRL = %X\n", ctrl);
  printf("CHIPID = %X\n", chipid);
  printf("POR_DIS = %X\n", por_dis);
  printf("RST_N = %X\n", rst_n);
  printf("HSDATA = %X\n", hsdata);
  printf("DCLK = %X\n", dclk);
  printf("DCTRL = %X\n", dctrl);
  printf("MCLK = %X\n", mclk);
  printf("DACMONV = %X\n", dacmonv);
  printf("DACMONI = %X\n", dacmoni);
}

void run_PC3_Write_V(int sid)
{
  int avdd;
  int dvdd;
  int pwell;
  int sub;
  float avdd_value;
  float dvdd_value;
  float pwell_value;
  float sub_value;

  printf("enter voltage to set : <AVDD> <AVDD_VAL> <DVDD> <DVDD_VAL> <PWELL> <PWELL_VAL> <SUB> <SUB_VAL> : ");
  scanf("%d", &avdd);
  scanf("%f", &avdd_value);
  scanf("%d", &dvdd);
  scanf("%f", &dvdd_value);
  scanf("%d", &pwell);
  scanf("%f", &pwell_value);
  scanf("%d", &sub);
  scanf("%f", &sub_value);

  PC3_Write_V(sid, avdd, dvdd, pwell, sub, avdd_value, dvdd_value, pwell_value, sub_value);
}

void run_PC3_Read_I(int sid)
{
  float avdd;
  float dvdd;
  float dvdd33;
  float pwell;
  float sub;
  float dacmonv;
  float dacmoni;

  PC3_Read_I(sid, &avdd, &dvdd, &dvdd33, &pwell, &sub, &dacmonv, &dacmoni);
  printf("AVDD = %f mA\n", avdd);
  printf("DVDD = %f mA\n", dvdd);
  printf("DVDD33 = %f mA\n", dvdd33);
  printf("PWELL = %f mA\n", pwell);
  printf("SUB = %f mA\n", sub);
  printf("DACMONV = %f V\n", dacmonv);
  printf("DACMONI = %f V\n", dacmoni);
}

void run_PC3_Write_SHORT(int sid)
{
  int pwell;
  int sub;

  printf("0 = short to AGND, 1 = short to PWELL, 2 = short to SUB\n");
  printf("enter <pwell> <sub> : ");
  scanf("%d", &pwell);
  scanf("%d", &sub);

  PC3_Write_SHORT(sid, pwell, sub);
}

void run_PC3_Write_MODE(int sid)
{
  int mode;

  printf("0 = manual contact test   1 = auto contact test\n");
  printf("2 = DAQ mode              3 = standalone mode\n");
  printf("enter mode : ");
  scanf("%d", &mode);

  PC3_Write_MODE(sid, mode);
}

void run_PC3_Write_DIR(int sid)
{
  int data;
  int busy;

  printf("enter <data> <busy> : ");
  scanf("%d", &data);
  scanf("%d", &busy);

  PC3_Write_DIR(sid, data, busy);
}

void run_PC3_Write_LED(int sid)
{
  int data;

  printf("enter data(in hex) : ");
  scanf("%X", &data);

  PC3_Write_LED(sid, data);
}

void run_PC3_Read_STATUS(int sid)
{
  unsigned int data;
  
  data = PC3_Read_STATUS(sid);
  printf("data = %X\n", data);
}

void run_PC3_Write_READBACK(int sid)
{
  unsigned int data;

  printf("enter data(in hex) : ");
  scanf("%X", &data);

  PC3_Write_READBACK(sid, data);
}

void run_PC3_Read_READBACK(int sid)
{
  unsigned int data;
  
  data = PC3_Read_READBACK(sid);
  printf("data = %X\n", data);
}

void run_PC3_Read_LOCK(int sid)
{
  unsigned int data;

  data = PC3_Read_LOCK(sid);

  if (data)
    printf("clock is locked.\n");
  else
    printf("clock is not locked.\n");
}




