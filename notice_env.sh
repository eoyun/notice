#export NKHOME=/usr/local/notice
export NKHOME=/Users/drc_daq/scratch/notice
echo $NKHOME
#export LIBUSB_INC=/usr/include/libusb-1.0
#export LIBUSB_INC=/usr/local/include/libusb-1.0
export LIBUSB_INC=/opt/homebrew/Cellar/libusb/1.0.26/include/libusb-1.0
#export LIBUSB_LIB=/usr/lib64
#export LIBUSB_LIB=/usr/lib/x86_64-linux-gnu
#export LIBUSB_LIB=/usr/local/lib
export LIBUSB_LIB=/opt/homebrew/Cellar/libusb/1.0.26/lib

#ROOTHOME=/usr/local
#. $ROOTHOME/bin/thisroot.sh

if [ -z "${PATH}" ]; then
   PATH=$NKHOME/bin; export PATH
else
   PATH=$NKHOME/bin:$PATH; export PATH
fi

if [ -z "${LD_LIBRARY_PATH}" ]; then
   DYLD_LIBRARY_PATH=$NKHOME/lib; export DYLD_LIBRARY_PATH
else
   DYLD_LIBRARY_PATH=$NKHOME/lib:$DYLD_LIBRARY_PATH; export DYLD_LIBRARY_PATH
fi

