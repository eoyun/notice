#include <cstdio>
#include <algorithm>
#include <vector>
#include <TPad.h>

void pad_set(TPad* tPad) {
    tPad->Draw();
    tPad->cd();

    tPad->SetTopMargin(0.08);
    tPad->SetLeftMargin(0.08);    
    tPad->SetRightMargin(0.08);
    tPad->SetBottomMargin(0.08);
}

void prompt_analysis_using_peak_sw(const char* filename) {

    int channel;
    int ch_to_plot;
 


    int evt;
    int data_length;
    int run_number;
    int tcb_trig_type;
    int tcb_trig_number;
    long long tcb_trig_time;
    int mid;
    int local_trig_number;
    int local_trigger_pattern;
    long long local_trig_time;
    long long diff_time;
    long long fine_time;
    long long coarse_time;
    int itmp;
    long long ltmp;
    int i;
    int cont;
    int pedestal_tmp;
    int j;
    int integral;
    int k;

    // ch 1 : A 
    // ch 5 : B - S
    // ch 11 : B - C
    // ch 15 : C    

    int chNum_1 = 1;
    int chNum_2 = 5;
    int chNum_3 = 11;
    int chNum_4 = 15;

    FILE *fp;
    int file_size;
    int nevt;
    char data[64];
    short adc[32736];

    // kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
    // kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
    // kSpring = 820, kTeal   = 840, kAzure   =  860, kViolet = 880,  kPink   = 900
    
    TH1F* plot1 = new TH1F("ch1", "#font[42]{#scale[0.8]{Module A - #color[634]{S ch}}};ADC;evts", 256, 0., 4095.); plot1->SetStats(0);
    plot1->SetLineWidth(2); plot1->SetLineColor(634); plot1->Sumw2();
    TH1F* plot3 = new TH1F("ch5", "#font[42]{#scale[0.8]{Module B - #color[634]{S ch}}};ADC;evts", 256, 0., 4095.); plot3->SetStats(0);
    plot3->SetLineWidth(2); plot3->SetLineColor(634); plot3->Sumw2();
    TH1F* plot5 = new TH1F("ch11", "#font[42]{#scale[0.8]{Module B - #color[602]{C ch}}};ADC;evts", 256, 0., 4095.); plot5->SetStats(0);
    plot5->SetLineWidth(2); plot5->SetLineColor(602); plot5->Sumw2();
    TH1F* plot7 = new TH1F("ch15", "#font[42]{#scale[0.8]{Module C - #color[634]{S ch}}};ADC;evts", 256, 0., 4095.); plot7->SetStats(0);
    plot7->SetLineWidth(2); plot7->SetLineColor(634); plot7->Sumw2();

    TH1F* plot_wav_1 = new TH1F("plot1", "Waveform", 1000, 1, 1000);
    TH1F* plot_wav_2 = new TH1F("plot2", "Waveform", 1000, 1, 1000);
    TH1F* plot_wav_3 = new TH1F("plot3", "Waveform", 1000, 1, 1000);
    TH1F* plot_wav_4 = new TH1F("plot4", "Waveform", 1000, 1, 1000);

    fp = fopen(filename, "rb");
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fclose(fp);
    nevt = file_size / 65536;
    
    fp = fopen(filename, "rb");
    TCanvas* c1 = new TCanvas("c1", "c1", 1000, 1000);

    for (int evt = 0; evt < nevt; evt++) {
        fread(data, 1, 64, fp);

        // data length
        // data_length = data[0] & 0xFF;
        // itmp = data[1] & 0xFF;
        // itmp = itmp << 8;
        // data_length = data_length + itmp;
        // itmp = data[2] & 0xFF;
        // itmp = itmp << 16;
        // data_length = data_length + itmp;
        // itmp = data[3] & 0xFF;
        // itmp = itmp << 24;
        // data_length = data_length + itmp;

        // // run number
        // run_number = data[4] & 0xFF;
        // itmp = data[5] & 0xFF;
        // itmp = itmp << 8;
        // run_number = run_number + itmp;
        
        // // trigger type
        // tcb_trig_type = data[6] & 0xFF;
        
        // // TCB trigger #
        // tcb_trig_number = data[7] & 0xFF;
        // itmp = data[8] & 0xFF;
        // itmp = itmp << 8;
        // tcb_trig_number = tcb_trig_number + itmp;
        // itmp = data[9] & 0xFF;
        // itmp = itmp << 16;
        // tcb_trig_number = tcb_trig_number + itmp;
        // itmp = data[10] & 0xFF;
        // itmp = itmp << 24;
        // tcb_trig_number = tcb_trig_number + itmp;

        // // TCB trigger time
        // fine_time = data[11] & 0xFF;
        // fine_time = fine_time * 11;     // actually * (1000 / 90)
        // coarse_time = data[12] & 0xFF;
        // ltmp = data[13] & 0xFF;
        // ltmp = ltmp << 8;
        // coarse_time = coarse_time + ltmp;
        // ltmp = data[14] & 0xFF;
        // ltmp = ltmp << 16;
        // coarse_time = coarse_time + ltmp;
        // ltmp = data[15] & 0xFF;
        // ltmp = ltmp << 24;
        // coarse_time = coarse_time + ltmp;
        // ltmp = data[16] & 0xFF;
        // ltmp = ltmp << 32;
        // coarse_time = coarse_time + ltmp;
        // ltmp = data[17] & 0xFF;
        // ltmp = ltmp << 40;
        // coarse_time = coarse_time + ltmp;
        // coarse_time = coarse_time * 1000;   // get ns
        // tcb_trig_time = fine_time + coarse_time;
        
        // // mid
        // mid = data[18] & 0xFF;

        // // local trigger #
        // local_trig_number = data[19] & 0xFF;
        // itmp = data[20] & 0xFF;
        // itmp = itmp << 8;
        // local_trig_number = local_trig_number + itmp;
        // itmp = data[21] & 0xFF;
        // itmp = itmp << 16;
        // local_trig_number = local_trig_number + itmp;
        // itmp = data[22] & 0xFF;
        // itmp = itmp << 24;
        // local_trig_number = local_trig_number + itmp;

        // // local trigger #
        // local_trigger_pattern = data[23] & 0xFF;
        // itmp = data[24] & 0xFF;
        // itmp = itmp << 8;
        // local_trigger_pattern = local_trigger_pattern + itmp;
        // itmp = data[25] & 0xFF;
        // itmp = itmp << 16;
        // local_trigger_pattern = local_trigger_pattern + itmp;
        // itmp = data[26] & 0xFF;
        // itmp = itmp << 24;
        // local_trigger_pattern = local_trigger_pattern + itmp;

        // // local trigger time
        // fine_time = data[27] & 0xFF;
        // fine_time = fine_time * 11;     // actually * (1000 / 90)
        // coarse_time = data[28] & 0xFF;
        // ltmp = data[29] & 0xFF;
        // ltmp = ltmp << 8;
        // coarse_time = coarse_time + ltmp;
        // ltmp = data[30] & 0xFF;
        // ltmp = ltmp << 16;
        // coarse_time = coarse_time + ltmp;
        // ltmp = data[31] & 0xFF;
        // ltmp = ltmp << 24;
        // coarse_time = coarse_time + ltmp;
        // ltmp = data[32] & 0xFF;
        // ltmp = ltmp << 32;
        // coarse_time = coarse_time + ltmp;
        // ltmp = data[33] & 0xFF;
        // ltmp = ltmp << 40;
        // coarse_time = coarse_time + ltmp;
        // coarse_time = coarse_time * 1000;   // get ns
        // local_trig_time = fine_time + coarse_time;

        // diff_time = local_trig_time - tcb_trig_time;

        fread(adc, 2, 32736, fp);

        float ch1_ped = 0;
        float ch3_ped = 0;
        float ch5_ped = 0;
        float ch7_ped = 0;

        std::vector<int> waveform_vec_1;
        std::vector<int> waveform_vec_3;
        std::vector<int> waveform_vec_5;
        std::vector<int> waveform_vec_7;

        plot_wav_1->Reset("ICESM");
        plot_wav_2->Reset("ICESM");
        plot_wav_3->Reset("ICESM");
        plot_wav_4->Reset("ICESM");


        // fill waveform for channel to plotgecit
        float pedNbin = 50.;
        for (int i = 1; i < pedNbin + 1; i++) {
            ch1_ped += (float)adc[i * 32 + chNum_1] / pedNbin;
            ch3_ped += (float)adc[i * 32 + chNum_2] / pedNbin;
            ch5_ped += (float)adc[i * 32 + chNum_3] / pedNbin;
            ch7_ped += (float)adc[i * 32 + chNum_4] / pedNbin;
        }

        for (int i = 1; i < 1001; i++) {
        // for (int i = 0; i < 1023; i++) {
            // waveform_vec_1.push_back(ch1_ped - adc[i * 32 + chNum_1]);
            // waveform_vec_3.push_back(ch3_ped - adc[i * 32 + chNum_2]);
            // waveform_vec_5.push_back(ch5_ped - adc[i * 32 + chNum_3]);
            // waveform_vec_7.push_back(ch7_ped - adc[i * 32 + chNum_4]);

            plot_wav_1->Fill(i,ch1_ped - adc[i * 32 + chNum_1]);
            plot_wav_2->Fill(i,ch3_ped - adc[i * 32 + chNum_2]);
            plot_wav_3->Fill(i,ch5_ped - adc[i * 32 + chNum_3]);
            plot_wav_4->Fill(i,ch7_ped - adc[i * 32 + chNum_4]);
        }

        // plot1->Fill(*std::max_element(waveform_vec_1.begin(), waveform_vec_1.end()));
        // plot3->Fill(*std::max_element(waveform_vec_3.begin(), waveform_vec_3.end()));
        // plot5->Fill(*std::max_element(waveform_vec_5.begin(), waveform_vec_5.end()));
        // plot7->Fill(*std::max_element(waveform_vec_7.begin(), waveform_vec_7.end()));

        plot1->Fill(plot_wav_1->GetMaximum());
        std::cout << plot_wav_1->GetMaximum() << std::endl;
        plot3->Fill(plot_wav_2->GetMaximum());
        plot5->Fill(plot_wav_3->GetMaximum());
        plot7->Fill(plot_wav_4->GetMaximum());

        //if (evt == 15) {
        //    c1->cd();
        //    plot_wav_1->Draw("Hist");
        //    std::cout << plot_wav_1->GetMaximum() << std::endl;
        //}

        
    }
    TCanvas* c = new TCanvas("c", "c", 1000, 1000);

    c->cd();
    TPad* pad_LB = new TPad("pad_LB","pad_LB", 0.01, 0.01, 0.5, 0.5 );
    pad_set(pad_LB);

    c->cd();
    TPad* pad_RB = new TPad("pad_RB","pad_RB", 0.5, 0.01, 0.99, 0.5 );
    pad_set(pad_RB);

    c->cd();
    TPad* pad_LT = new TPad("pad_LT","pad_LT", 0.01, 0.5, 0.5, 0.99 );
    pad_set(pad_LT);

    c->cd();
    TPad* pad_RT = new TPad("pad_RT","pad_RT", 0.5, 0.5, 0.99, 0.99 );
    pad_set(pad_RT);

    c->cd(); pad_LT->cd(); plot1->Draw("Hist");
    c->cd(); pad_RT->cd(); plot7->Draw("Hist");
    c->cd(); pad_LB->cd(); plot3->Draw("Hist");
    c->cd(); pad_RB->cd(); plot5->Draw("Hist");

    TFile* validFile = new TFile("/usr/local/notice/test/prompt_test/validFiles/Peak/"+filename+"_validation.root", "RECREATE");
    validFile->WriteTObject(plot1);
    validFile->WriteTObject(plot3);
    validFile->WriteTObject(plot5);
    validFile->WriteTObject(plot7);
    validFile->Close();
}

