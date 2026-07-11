#include <stdio.h>

//int plot_stddev_32ch(const TString filename, const TString condition)
int plot_stddev_32ch_SaveAll(const TString filename, const int rmin, const int rmax)
{
	int channel;
	int ch_to_plot;
	FILE *fp;
	int file_size;
	int nevt;
	char data[64];
	short adc[32736];
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
	int j;
	int cont;
	double time[2000];
	//std::vector<double> time;
	//std::vector<double> mean[channel];
	double mean[32][2000];
	double stddev[32][2000];
	//std::vector<double> mean[32];
	//std::vector<double> stddev[32];
//	std::vector<std::vector<double>> mean;
//	std::vector<std::vector<double>> stddev;
	//time.clear();
//	mean.clear();
//	stddev.clear();
	  //for( i = 0 ; i < 32 ; i ++){
	 //  mean[i].clear();
	 //  stddev[i].clear();
	 // }
	// get channel to plot, channel = 1 ~ 32
	//  printf("Channel to plot(1~32) : ");
	//  scanf("%d", &channel);
	//  if (channel < 1)
	//    ch_to_plot = 0;
	//  else if (channel > 32)
	//    ch_to_plot = 31;
	//  else
	//    ch_to_plot = channel - 1;

	TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 1800, 1000);
	c1->Divide(8,4,0.001,0.001);
	gPad->SetLeftMargin(0);
	gPad->SetRightMargin(0);
	gPad->SetBottomMargin(0);
	gPad->SetTopMargin(0);

	TH1F *plot[32];
	for( i = 0 ; i < 32 ; i ++)
	{
		plot[i] = new TH1F(Form("plot%d",i+1), Form("Spectrum ch%d", i+1), 4096,0,4096);
		plot[i]->SetStats(0);  
	}
    TFile *tfile = new TFile(filename+"_stddevAllCh_.root","RECREATE");

	// get # of events in file
	fp = fopen(filename, "rb");
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	fclose(fp);
	nevt = file_size / 65536;

	printf("-----------------------------------------------------------------------\n");
	fp = fopen(filename, "rb");

	for (evt = 0; evt < nevt; evt++) {
		// read header
		fread(data, 1, 64, fp);

		// read waveform
		fread(adc, 2, 32736, fp);

		// fill waveform for channel to plotgecit 
		for( i = 0 ; i < 32 ; i ++)
		{
			plot[i]->Reset();
		}

		for (i = 0; i < 1023; i++) {
			for( j = 0; j < 32 ; j ++) {
				plot[j]->Fill(adc[i * 32 + j]);
			}
		}
		time[evt] = evt;
		//for( i = 1; i < nevt ; i ++)  {time.push_back(i); zero.push_back(0);} // 1evt=1s
		for( i = 0 ; i < 32 ; i ++)
		{
//			cout<<evt<< " evt , " << i <<" Channel : " <<plot[i]->GetMean() << ", " << plot[i]->GetStdDev() << endl;
			mean[i][evt] = plot[i]->GetMean();
			stddev[i][evt] = plot[i]->GetStdDev();
			//mean[i].push_back(plot[i]->GetMean());
			//stddev[i].push_back(plot[i]->GetStdDev());
		}
		if (cont == 0)
			evt = nevt;
	}

	TGraphErrors* gr[32];
	for( i = 0 ; i < 32 ; i ++)
	{
		gr[i] = new TGraphErrors(nevt, time, mean[i], 0, stddev[i]);
		//gr[i] = new TGraphErrors(time.size(), &(time[0]), &(mean[0][i]), 0, &(stddev[0][i]));
		//gr[i] = new TGraphErrors(&(time), &(mean[i]), &time, &(stddev[i]));
		gr[i]->SetTitle(Form("Pedestal Std.Dev ch%d",i+1));
		gr[i]->GetXaxis()->SetTitle("[s]");
		gr[i]->GetYaxis()->SetTitle("[ADC]");
        gr[i]->GetYaxis()->SetLabelSize(0.05);
		gr[i]->GetYaxis()->SetRangeUser( rmin, rmax);
		c1->cd(i+1);
		gr[i]->Draw("AP");
	}


	c1->Modified();
	c1->Update();
	//c1->SaveAs(filename+"_"+condition+Form("_AllchStdDev_%ds.png", nevt));      
	//c1->SaveAs(filename+Form("_AllchStdDev_%ds.png", nevt));      
    c1->Write();
	tfile->Close();
	fclose(fp);

	return 0;
}

