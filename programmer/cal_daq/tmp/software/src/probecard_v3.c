#include "probecard_v3.h"

enum EManipInterface {kInterfaceClaim, kInterfaceRelease};

struct dev_open {
   libusb_device_handle *devh;
   uint16_t vendor_id;
   uint16_t product_id;
   int serial_id;
   struct dev_open *next;
} *ldev_open = 0;

static unsigned char get_serial_id(libusb_device_handle *devh)
{
// Get serial id of device handle devh. devh may or may not be on open device list 'ldev_open'.
// Returns 0 if error.
  int ret;
  if (!devh) {
    return 0;
  }
  unsigned char data[1];
  ret = libusb_control_transfer(devh, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN, 0xD2, 0, 0, data, 1, 1000);

  if (ret < 0) {
    fprintf(stdout, "Warning: get_serial_id: Could not get serial id.\n");
    return 0;
  }

  return data[0];
}

static int handle_interface_id(struct dev_open **list, uint16_t vendor_id, uint16_t product_id,
                               int sid, int interface, enum EManipInterface manip_type)
{
// Manipulate interface on the device with specified vendor id, product id and serial id
// from open device list. Claim interface if manip_type == kInterfaceClaim, release interface
// if manip_type == kInterfaceRelease. If sid == NK_SID_ANY, all devices with given vendor id
// and product id are handled.

  int ret = 0;
  if (!*list) {
    ret = -1;
    return ret;
  }

  struct dev_open *curr = *list;
  struct libusb_device_descriptor desc;
  libusb_device *dev;

  while (curr) {
    dev =libusb_get_device(curr->devh);
    if (libusb_get_device_descriptor(dev, &desc) < 0) {
      // Ignore with message
      fprintf(stdout, "Warning: remove_device: could not get device device descriptior."
                          " Ignoring.\n");
      continue;
    }
    if (desc.idVendor == vendor_id && desc.idProduct == product_id
    && (sid == 0xFF || sid == get_serial_id(curr->devh))) { 
      // Match.
      if (manip_type == kInterfaceClaim) {
        if ((ret = libusb_claim_interface(curr->devh, interface)) < 0) {
          fprintf(stdout, "Warning: handle_interface_id: Could not claim interface (%d) on device (%u, %u, %u)\n",
                  interface, vendor_id, product_id, sid);
        }
      }
      else if (manip_type == kInterfaceRelease) {
        if ((ret =libusb_release_interface(curr->devh, interface)) < 0) {
          fprintf(stdout, "Warning: handle_interface_id: Could not release interface (%d) on device (%u, %u, %u)\n",
                  interface, vendor_id, product_id, sid);
        }
      }
      else {
        fprintf(stderr, "Error: handle_interface_id: Unknown interface handle request: %d\n.",
                manip_type);
              
        ret = -1;
        return ret;
      }
    }
    // Move forward
    curr = curr->next;
  }

  return ret;
}

static int is_device_open(libusb_device_handle *devh)
{
// See if the device handle "devh" is on the open device list

  struct dev_open *curr = ldev_open;
  libusb_device *dev, *dev_curr;
  int bus, bus_curr, addr, addr_curr;

  while (curr) {
    dev_curr = libusb_get_device(curr->devh);
    bus_curr = libusb_get_bus_number(dev_curr);
    addr_curr = libusb_get_device_address(dev_curr);

    dev = libusb_get_device(devh);
    bus = libusb_get_bus_number(dev);
    addr = libusb_get_device_address(dev);

    if (bus == bus_curr && addr == addr_curr) return 1;
    curr = curr->next;
  }

  return 0;
}

static void add_device(struct dev_open **list, libusb_device_handle *tobeadded,
                       uint16_t vendor_id, uint16_t product_id, int sid)
{
// Add device to the open device list

  struct dev_open *curr;

  curr = (struct dev_open *)malloc(sizeof(struct dev_open));
  curr->devh = tobeadded;
  curr->vendor_id = vendor_id;
  curr->product_id = product_id;
  curr->serial_id = sid;
  curr->next  = *list;
  *list = curr;
}

static void remove_device_id(struct dev_open **list, uint16_t vendor_id, uint16_t product_id, int sid)
{
// Close and remove device with specified vendor id, product id and serial id
// from open device list. If sid == NK_SID_ANY, all devices with given vendor id
// and product id are removed.

  if (!*list) return;

  struct dev_open *curr = *list;
  struct dev_open *prev = 0;
  struct libusb_device_descriptor desc;
  libusb_device *dev;

  while (curr) {
    dev =libusb_get_device(curr->devh);
    if (libusb_get_device_descriptor(dev, &desc) < 0) {
      // Ignore with message
      fprintf(stdout, "Warning, remove_device: could not get device device descriptior." " Ignoring.\n");
      continue;
    }
    if (desc.idVendor == vendor_id && desc.idProduct == product_id
    && (sid == 0xFF || sid == get_serial_id(curr->devh))) { 
      // Match.
      if (*list == curr) { 
        // Update head, remove current element and move cursor forward.
        *list = curr->next;
        libusb_close(curr->devh);
        free(curr); 
        curr = *list;
      }
      else {
        // Update link, remove current element and move cursor forward.
        prev->next = curr->next;
        libusb_close(curr->devh);
        free(curr); 
        curr = prev->next;
      }
    }
    else {
      // No match. Move cursor forward.
      prev = curr;
      curr = curr->next;
    }    
  }
}

libusb_device_handle* nkusb_get_device_handle(uint16_t vendor_id, uint16_t product_id, int sid) 
{
// Get open device handle with given vendor id, product id and serial id.
// Return first found device handle if sid == NK_SID_ANY.

  struct dev_open *curr = ldev_open;
  while (curr) {
    if (curr->vendor_id == vendor_id && curr->product_id == product_id) {
      if (sid == 0xFF)
        return curr->devh;
      else if (curr->serial_id == sid)
        return curr->devh;
    }

    curr = curr->next;
  }

  return 0;
}

// initialize libusb library
void PC3_USBinit(libusb_context **ctx)
{
  if (libusb_init(ctx) < 0) {
    fprintf(stderr, "failed to initialise libusb\n");
    exit(1);
  }
}

// de-initialize libusb library
void PC3_USBexit(libusb_context *ctx)
{
  libusb_exit(ctx); 
}

// open probecard
void PC3_Open(int sid, libusb_context *ctx)
{
  struct libusb_device **devs;
  struct libusb_device *dev;
  struct libusb_device_handle *devh;
  size_t i = 0;
  int nopen_devices = 0; //number of open devices
  int r;
  int interface = 0;
  int sid_tmp;
  int speed;

  if (libusb_get_device_list(ctx, &devs) < 0) 
    fprintf(stderr, "Error: open_device: Could not get device list\n");

  fprintf(stdout, "Info: open_device: opening device Vendor ID = 0x%X, Product ID = 0x%X, Serial ID = %u\n",
                   PC3_VENDOR_ID, PC3_PRODUCT_ID, sid);

  while ((dev = devs[i++])) {
    struct libusb_device_descriptor desc;
    r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0) 
      fprintf(stdout, "Warning, open_device: could not get device device descriptior." " Ignoring.\n");

    if (desc.idVendor == PC3_VENDOR_ID && desc.idProduct == PC3_PRODUCT_ID)  {
      r = libusb_open(dev, &devh);
      if (r < 0) 
        fprintf(stdout, "Warning, open_device: could not open device." " Ignoring.\n");

      // do not open twice
      if (is_device_open(devh)) {
        fprintf(stdout, "Info, open_device: device already open." " Ignoring.\n");
        libusb_close(devh);
        continue;
      }

      // See if sid matches
      // Assume interface 0
      if (libusb_claim_interface(devh, interface) < 0) {
        fprintf(stdout, "Warning, open_device: could not claim interface 0 on the device." " Ignoring.\n");
        libusb_close(devh);
        continue;
      }

      sid_tmp = get_serial_id(devh);

      if (sid == 0xFF || sid == sid_tmp) {
        add_device(&ldev_open, devh, PC3_VENDOR_ID, PC3_PRODUCT_ID, sid_tmp);
        nopen_devices++;
  
        // Print out the speed of just open device 
        speed = libusb_get_device_speed(dev);
        switch (speed) {
          case 4:
            fprintf(stdout, "Info: open_device: super speed device opened");
            break;
          case 3:
            fprintf(stdout, "Info: open_device: high speed device opened");
            break;
          case 2:
            fprintf(stdout, "Info: open_device: full speed device opened");
            break;
          case 1:
            fprintf(stdout, "Info: open_device: low speed device opened");
            break;
          case 0:
            fprintf(stdout, "Info: open_device: unknown speed device opened");
            break;
        }
        
        fprintf(stdout, " (bus = %d, address = %d, serial id = %u).\n",
                    libusb_get_bus_number(dev), libusb_get_device_address(dev), sid_tmp);
        libusb_release_interface(devh, interface);
        break;
      }
      else {
        libusb_release_interface(devh, interface);
        libusb_close(devh);
      }
    }
  }

  libusb_free_device_list(devs, 1);

  handle_interface_id(&ldev_open, PC3_VENDOR_ID, PC3_PRODUCT_ID, sid, 0, kInterfaceClaim);
}

// close probecard
void PC3_Close(int sid)
{
  handle_interface_id(&ldev_open, PC3_VENDOR_ID, PC3_PRODUCT_ID, sid, 0, kInterfaceRelease);
  remove_device_id(&ldev_open, PC3_VENDOR_ID, PC3_PRODUCT_ID, sid);
}

// Receive data from probecard
int PC3_Receive(int sid, unsigned int count, unsigned int addr, unsigned char *data)
{
  const unsigned int timeout = 1000; 
  int length = 8;
  int transferred;
  unsigned char *buffer;
  int stat = 0;
  int nbulk;
  int remains;
  int loop;
  int size = 16384; // 16 kB

  nbulk = count / 4096;
  remains = count % 4096;

  if (!(buffer = (unsigned char *)malloc(size))) {
    fprintf(stderr, "USB3Read: Could not allocate memory (size = %d\n)", size);
    return -1;
  }
  
  buffer[0] = count & 0xFF;
  buffer[1] = (count >> 8)  & 0xFF;
  buffer[2] = (count >> 16)  & 0xFF;
  buffer[3] = (count >> 24)  & 0xFF;
  
  buffer[4] = addr & 0xFF;
  buffer[5] = (addr >> 8)  & 0xFF;
  buffer[6] = (addr >> 16)  & 0xFF;
  buffer[7] = (addr >> 24)  & 0x7F;
  buffer[7] = buffer[7] | 0x80;

  libusb_device_handle *devh = nkusb_get_device_handle(PC3_VENDOR_ID, PC3_PRODUCT_ID, sid);
  if (!devh) {
    fprintf(stderr, "USB3Write: Could not get device handle for the device.\n");
    return -1;
  }

  if ((stat = libusb_bulk_transfer(devh, USB3_SF_WRITE, buffer, length, &transferred, timeout)) < 0) {
    fprintf(stderr, "USB3Read: Could not make write request; error = %d\n", stat);
    free(buffer);
    return stat;
  }

  for (loop = 0; loop < nbulk; loop++) {
    if ((stat = libusb_bulk_transfer(devh, USB3_SF_READ, buffer, size, &transferred, timeout)) < 0) {
      fprintf(stderr, "USB3Read: Could not make read request; error = %d\n", stat);
      return 1;
    }
    memcpy(data + loop * size, buffer, size);
  }

  if (remains) {
    if ((stat = libusb_bulk_transfer(devh, USB3_SF_READ, buffer, remains * 4, &transferred, timeout)) < 0) {
      fprintf(stderr, "USB3Read: Could not make read request; error = %d\n", stat);
      return 1;
    }
    memcpy(data + nbulk * size, buffer, remains * 4);
  }

  free(buffer);
  
  return 0;
}

// write probecard register
int PC3_Write(int sid, unsigned int addr, unsigned int data)
{
  int transferred = 0;  
  const unsigned int timeout = 1000;
  int length = 8;
  unsigned char *buffer;
  int stat = 0;
  
  if (!(buffer = (unsigned char *)malloc(length))) {
    fprintf(stderr, "USB3Write: Could not allocate memory (size = %d\n)", length);
    return -1;
  }
  
  buffer[0] = data & 0xFF;
  buffer[1] = (data >> 8)  & 0xFF;
  buffer[2] = (data >> 16)  & 0xFF;
  buffer[3] = (data >> 24)  & 0xFF;
  
  buffer[4] = addr & 0xFF;
  buffer[5] = (addr >> 8)  & 0xFF;
  buffer[6] = (addr >> 16)  & 0xFF;
  buffer[7] = (addr >> 24)  & 0x7F;
  
  libusb_device_handle *devh = nkusb_get_device_handle(PC3_VENDOR_ID, PC3_PRODUCT_ID, sid);
  if (!devh) {
    fprintf(stderr, "USB3Write: Could not get device handle for the device.\n");
    return -1;
  }
  
  if ((stat = libusb_bulk_transfer(devh, USB3_SF_WRITE, buffer, length, &transferred, timeout)) < 0) {
    fprintf(stderr, "USB3Write: Could not make write request; error = %d\n", stat);
    free(buffer);
    return stat;
  }
  
  free(buffer);

  usleep(1000);

  return stat;
}

// read probecard register
unsigned int PC3_Read(int sid, unsigned int addr)
{
  unsigned char data[4];
  unsigned int value;
  unsigned int tmp;

  PC3_Receive(sid, 1, addr, data);

  value = data[0] & 0xFF;
  tmp = data[1] & 0xFF;
  value = value + (unsigned int)(tmp << 8);
  tmp = data[2] & 0xFF;
  value = value + (unsigned int)(tmp << 16);
  tmp = data[3] & 0xFF;
  value = value + (unsigned int)(tmp << 24);

  return value;
}  

// contact test AVSS, AVDD, DVSS, DVDD, DVDD33, PWELL and SUB
void PC3_Write_PWR(int sid, int avss, int avdd, int dvss, int dvdd, int dvdd33, int pwell, int sub)
{
  unsigned int data;

  data = ((dvdd & 0xFF) << 24) | ((dvss & 0xFF) << 16) | ((avdd & 0xFF) << 8) | (avss & 0xFF);
  PC3_Write(sid, 0x0, data);

  data = ((sub & 0x1F) << 5) | ((pwell & 0xF) << 1) | (dvdd33 & 0x1);
  PC3_Write(sid, 0x1, data);
}

// read contaction AVSS, AVDD, DVSS, DVDD, DVDD33, PWELL and SUB
void PC3_Read_PWR(int sid, int *avss, int *avdd, int *dvss, int *dvdd, int *dvdd33, int *pwell, int *sub)
{
  unsigned int data;
  
  data = PC3_Read(sid, 0x0);
  *avss = data & 0xFF;
  *avdd = (data >> 8) & 0xFF;
  *dvss = (data >> 16) & 0xFF;
  *dvdd = (data >> 24) & 0xFF;

  data = PC3_Read(sid, 0x1);
  *dvdd33 = data & 0x1;
  *pwell = (data >> 1) & 0xF;
  *sub = (data >> 5) & 0x1F;
}

// contact test signal 
void PC3_Write_SIG(int sid, int data, int mistral_o, int busy, int ctrl, int chipid, int por_dis, int rst_n, int hsdata, int dclk, int dctrl, int mclk, int dacmonv, int dacmoni)
{
  unsigned int wdat;

  wdat = ((dacmoni & 0x1) << 27) | ((dacmonv & 0x1) << 26) | ((mclk & 0x3) << 24)  | ((dctrl & 0x3) << 22)
       | ((dclk & 0x3) << 20)    | ((hsdata & 0x3) << 18)  | ((rst_n & 0x1) << 17) | ((por_dis & 0x1) << 16)
       | ((chipid & 0x7F) << 9)  | ((ctrl & 0x1) << 8)     | ((busy & 0x1) << 7)   | ((mistral_o & 0x7) << 4)
       | (data & 0xF);
  PC3_Write(sid, 0x2, wdat);
}

// read contaction signals
void PC3_Read_SIG(int sid, int *data, int *mistral_o, int *busy, int *ctrl, int *chipid, int *por_dis, int *rst_n, int *hsdata, int *dclk, int *dctrl, int *mclk, int *dacmonv, int *dacmoni)
{
  unsigned int rdat;
  
  rdat = PC3_Read(sid, 0x2);
  *data = rdat & 0xF;
  *mistral_o = (rdat >> 4) & 0x7;
  *busy = (rdat >> 7) & 0x1;
  *ctrl = (rdat >> 8) & 0x1;
  *chipid = (rdat >> 9) & 0x7F;
  *por_dis = (rdat >> 16) & 0x1;
  *rst_n = (rdat >> 17) & 0x1;
  *hsdata = (rdat >> 18) & 0x3;
  *dclk = (rdat >> 20) & 0x3;
  *dctrl = (rdat >> 22) & 0x3;
  *mclk = (rdat >> 24) & 0x3;
  *dacmonv = (rdat >> 26) & 0x1;
  *dacmoni = (rdat >> 27) & 0x1;
}

// set regulator voltage, set it to Volt
void PC3_Write_V(int sid, int avdd, int dvdd, int pwell, int sub, float avdd_value, float dvdd_value, float pwell_value, float sub_value)
{
  unsigned int data;
  unsigned int datL;
  unsigned int datH;
  float fdata;

  if (avdd) { 
    // convert volt to DAC value
    fdata = 314.754 * avdd_value - 384.0;
    if (fdata < 0.0)
      fdata = 0.0;
    if (fdata > 255.0)
      fdata = 255.0;
    datL = (unsigned int)(fdata);
    datL = datL | 0x100;
  }
  else
    datL = 0;

  if (dvdd) { 
    // convert volt to DAC value
    fdata = 314.754 * dvdd_value - 384.0;
    if (fdata < 0.0)
      fdata = 0.0;
    if (fdata > 255.0)
      fdata = 255.0;
    datH = ((unsigned int)(fdata)) << 9;
    datH = datH | 0x20000;
  }
  else
    datH = 0;

  data = datL | datH;
  PC3_Write(sid, 0x3, data);

  if (pwell) { 
    // convert volt to DAC value
    fdata = -102.4 * pwell_value;
    if (fdata < 0.0)
      fdata = 0.0;
    if (fdata > 1023.0)
      fdata = 1023.0;
    datL = (unsigned int)(fdata);
    datL = datL | 0x400;
  }
  else
    datL = 0;

  if (sub) { 
    // convert volt to DAC value
    fdata = -102.4 * sub_value;
    if (fdata < 0.0)
      fdata = 0.0;
    if (fdata > 1023.0)
      fdata = 1023.0;
    datH = ((unsigned int)(fdata)) << 11;
    datH = datH | 0x200000;
  }
  else
    datH = 0;

  data = datL | datH;
  PC3_Write(sid, 0x5, data);
}

// read regulator current, mA reads
void PC3_Read_I(int sid, float *avdd, float *dvdd, float *dvdd33, float *pwell, float *sub, float *dacmonv, float *dacmoni)
{
  unsigned int data;
  unsigned int datL;
  unsigned int datH;

  // start ADC conversion
  PC3_Write(sid, 0xA, 0x3F);

  // read data
  data = PC3_Read(sid, 0x3);
  datL = data & 0xFFF;
  datH = (data >> 12) & 0xFFF;
  *avdd = 0.052579 * ((float)(datL));
  *dvdd = 0.052579 * ((float)(datH));

  data = PC3_Read(sid, 0x4);
  *dvdd33 = 0.052579 * ((float)(data));

  data = PC3_Read(sid, 0x5);
  datL = data & 0xFFF;
  datH = (data >> 12) & 0xFFF;
  *pwell = 0.00269 * ((float)(datL));
  *sub = 0.00269 * ((float)(datH));

  data = PC3_Read(sid, 0x7);
  datL = data & 0xFFF;
  datH = (data >> 12) & 0xFFF;
  *dacmonv = 0.00045 * ((float)(datL));
  *dacmoni = 0.0001345 * ((float)(datH));
}

// make short between AGND-PWELL-SUB
// value -> 0 = short to AGND, 1 = short to PWELL, 2 = short to SUB
void PC3_Write_SHORT(int sid, int pwell, int sub)
{
  unsigned int data;

  data = ((sub & 0x3) << 2) | (pwell & 0x3);
  PC3_Write(sid, 0x6, data);
}

// set mode, 0 = contact-test manual mode, 1 = contact-test auto mode, 2 = DAQ mode, 3 = standalone mode
void PC3_Write_MODE(int sid, int value)
{
  unsigned int data;
  data = value;
  PC3_Write(sid, 0x8, data);
}

// set bidirectional signal direction
void PC3_Write_DIR(int sid, int data, int busy)
{
  unsigned int wdat;
  wdat = ((busy >> 1) & 0x1) | (data & 0x1);
  PC3_Write(sid, 0x9, wdat);
}

// set LED
void PC3_Write_LED(int sid, int value)
{
  unsigned int data;

  data = value;
  PC3_Write(sid, 0xB, data);
}

// read contact status
unsigned int PC3_Read_STATUS(int sid)
{
  return PC3_Read(sid, 0xB);
}

// read clock locking status
unsigned int PC3_Read_LOCK(int sid)
{
  return PC3_Read(sid, 0xC);
}

// write dummy register for readback test
void PC3_Write_READBACK(int sid, unsigned int data)
{
  PC3_Write(sid, 0xF, data);
}

// read dummy register
unsigned int PC3_Read_READBACK(int sid)
{
  unsigned int data;

  data = PC3_Read(sid, 0xF);
  
  return data;
}







