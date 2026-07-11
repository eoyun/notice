#include <stdio.h>
#include <string.h>

void avg_32ch()
{
	std::vector<string> filename;
	std::vector<TH1D*> avg;

	for ( int i = 0; i < 32; i++ ) {
		filename.push_back("ch" + std::to_string(i+1) + ".root");
		avg.push_back((TH1D*)(TFile::Open((TString)filename.at(i))->Get("Avg.Time.Str")));
		avg.at(i)->SetName("mcp_ch_" + (TString)(std::to_string(i+1)));
	}

	TCanvas *c1 = new TCanvas("c1", "c1", 1800, 1000);
 	c1->Divide(8,4,0.001,0.001);

  	for ( int i = 0; i < 32; i++ ) {
  		c1->cd(i+1);
  		avg.at(i)->Draw("HIST");
  	}

  c1->SaveAs("avg.png");
}