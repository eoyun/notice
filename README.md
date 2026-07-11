# notice
Install the libusb library

Set the proper library directroy in `notice_env.sh` according OS

# src
comfile the library code

    cd src/cal_daq/cal_daq
    make clean
    make install
    cd ../cal_tcb
    make clean
    make install

## test
comfile the operation code (change the data directory properly)

    cd test/TB_daq/src
    make -f Makefile_set_run_number
    make -f Makefile_set_bymid_mktxt
    make -f Makefile_get_evt_dir_log
    make -f Makefile_run_evt_mktxt

daq operation

    ./run_all.sh (config) (number of events)`

### Config file

TCB board

    cat config/setup/set_TCB.txt

    cw_tcb 10
    ptrigger_interval 0
    trig_enable 8
    mthr_tcb 1
    prescale 1

cw_tcb : coincidence width in TCB board (clock = 1 / 90MHz)

ptrigger_interval : time interval of pedestal trigger (ms)

trig_enable : select the trigger ( 1: self trigger, 2: pedestal trigger, 4: software trigger, 8: external trigger)

mthr_tcb : multiplicity threshold in TCB board

prescale : prescale of waveform mode


DAQ board

    cat config/setup/set_DAQ_1.txt

    cw_daq 5
    hv 0.0 0.0 0.0 0.0
    mthr_daq 1
    trig_latancy 250
    trig_dly 0
    down_sampling 0
    pulse_width 250
    risetime 120
    fraction 0.3
    peak_srch_dly 0 0 0 0
    peak_srch_wth 1000 1000 1000 1000
    thr
    4095 4095 4095 4095 4095 4095 4095 4095
    4095 4095 4095 4095 4095 4095 4095 4095
    4095 4095 4095 4095 4095 4095 4095 4095
    4095 4095 4095 4095 4095 4095 4095 4095

cw_daq : coincidence width in DAQ board (clock = 1 / 90MHz)

hv : high voltage of SiPM (0 - 45 V)

mthr_daq : multiplicity threshold in DAQ board

trig_latancy : trigger latancy, the time waiting flag of TCB board after stopping DRS4 chip in DAQ board (clock = 1 / 90MHz)

trig_dly : trigger delay (clock = 1 / 90MHz)

down_sampling : down sampling of DRS4 chip (0: 5 GHz, 1: 2.5 GHz, 2: 1.25GHz)

pulse_width : In fastmode, integral width (bin)

risetime : In fastmode, risetime of signal (bin)

fraction : In fastmode, constant fraction (0.01 - 0.99)

peak_srch_dly : In fastmode, start point of searching the peak (bin)

peak_srch_width : In fastmode, range of searching the peak (bin)

thr : threshold of self trigger (0.5 mV)

## programmer
daq firmware upgrade

### compile
Also change the directory of libusb on make file

    make -f Makefile_daq
    make -f Makefile_tcb

### check firmware

    ./programmer_cal_tcb.exe -dev all -info
    ./programmer_cal_daq.exe -dev all -info

### calibration
calibration the DRS4 chip

    make -f Makefile_cal
    ./calibration.exe (mid)
    ./programmer_cal_daq.exe -dev (usb number) -lut calLUT_(mid).dat


