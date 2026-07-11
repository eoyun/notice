LIBUSB_INC = /usr/include/libusb-1.0
LIBUSB_LIB = /usr/lib64
PC3_LIB = ../lib
PC3_INC = ../include

FILENAME = run_probecard_v3

EXE	=	$(FILENAME).exe

CC	=	gcc

COPTS	=	-fPIC -Wall

FLAGS	=	-shared -Wall,-soname -nostartfiles -s

LDFLAGS =        -O2 -Wall 

OutPutOpt = -o 

LIBS	=	-L$(LIBUSB_LIB) -lusb-1.0 -L$(PC3_LIB) -lprobecard_v3 -lm

INCLUDEDIR =	-I$(PC3_INC) -I$(LIBUSB_INC)

OBJS	=	$(FILENAME).o

INCLUDES =	$(FILENAME).h 

all	:	$(EXE)
clean	:
		/bin/rm -f $(OBJS) $(EXE)

$(EXE):      $(OBJS)
		$(CC) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		@echo "$@ done"

$(OBJS)	:	$(INCLUDES) 

%.o	:	%.c
		$(CC) $(COPTS) $(INCLUDEDIR) -c -o $@ $<


