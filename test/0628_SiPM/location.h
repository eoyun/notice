#include <cstdio>
#include <algorithm>
#include <vector>
#include <numeric>
#include <TPad.h>
#include <utility>
#include <iostream>

enum discriminator {
	kIntegral = 0,
	kPeak,
	kPed,
    kWaveform,
    kTriggerTime,
	kTest
};

enum runMode {
	kFull = 0,
    kPrompt,
    kNoPed
};

std::string BASE_DIR = "/usr/local/notice/test/0628_SiPM/SiPM_test_0628/";
std::string DATA_DIR = "/media/yu/Expansion/DAQ_data/220628/";
std::string FILE_NAME = "cal";

std::vector<int> chInput = {0, 8, 16};
std::vector<int> plotColor = {1, 1, 433};

std::vector<TString> plotName = {
                                "#font[42]{#scale[0.8]{Ext Trigg. 1}}",
                                "#font[42]{#scale[0.8]{Ext Trigg. 2}}",
                                "#font[42]{#scale[0.8]{discriminator}}",
                                };




// kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
// kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
// kSpring = 820, kTeal   = 840, kAzure   =  860, kViolet = 880,  kPink   = 900
