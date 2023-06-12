#include <stdio.h>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

void mcp_intADC_2D(const int runnum)
{
	 TFile *file = new TFile("./roots/IntADC_Run_" + (TString)(std::to_string(runnum)) + "_64ch.root");

	 std::vector<TH1F*> intADC;
    std::vector<int> intADCmean;
    std::vector<int> ch;

	 for ( int i = 0; i < 64; i++ ) {
      
	 		intADC.push_back((TH1F*)file->Get((TString)(std::to_string(i+1))));
         intADCmean.push_back(intADC.at(i)->GetMean());
         ch.push_back(i+1);
	 }

   int xPos, yPos, zPos;
   std::vector<float> xPosVec;
   std::vector<float> yPosVec;
   std::vector<float> zPosVec;

   std::ifstream in("./mapping/mcp_mapping_64ch_dummy.csv");
   // std::ifstream in("./mapping/mapping_TB.csv");

   while (in) {

      std::string oneLine;
      std::getline(in, oneLine);
      std::stringstream ss(oneLine);

      std::vector<double> data;
      std::string value;

      while(std::getline(ss, value, ',')) {
         double tmpDouble;
         std::stringstream ssDouble(value);
         ssDouble >> tmpDouble;
         data.push_back(tmpDouble);
      }

      // std::cout << xPosVec.size() << " " << data.size() << std::endl;
      if (data.size() < 3) 
         break;

      xPosVec.push_back(data.at(0));
      yPosVec.push_back(data.at(1));   
      zPosVec.push_back(data.at(2));

   }

   TH2F* test2DHist = new TH2F("", "intADC;# of column;# of row", 8, 1., 9., 8, 9., 17.);
   test2DHist->GetXaxis()->SetNdivisions(9);
   test2DHist->GetYaxis()->SetNdivisions(9);

   for ( int i = 1; i < 9; i++ ) 
      test2DHist->GetXaxis()->SetBinLabel(i, std::to_string(i).data());

   for ( int i = 1; i < 9; i++ ) 
      test2DHist->GetYaxis()->SetBinLabel(i, std::to_string(i+8).data());

   test2DHist->GetXaxis()->SetTickLength(0.);
   test2DHist->GetYaxis()->SetTickLength(0.);
   test2DHist->GetXaxis()->SetLabelFont(42);
   test2DHist->GetYaxis()->SetLabelFont(42);
   // test2DHist->GetXaxis()->SetLabelSize(0.04);
   test2DHist->GetXaxis()->SetLabelSize(0.025);
   test2DHist->GetYaxis()->SetLabelSize(0.025);
   test2DHist->GetXaxis()->SetTitleSize(0.03);
   test2DHist->GetYaxis()->SetTitleSize(0.03);
   test2DHist->Sumw2();


   for ( int i = 0; i < xPosVec.size(); i++ ) {

      if ( zPosVec.at(i) == ch.at(i) ) 
         // test2DHist->Fill(xPosVec.at(i), yPosVec.at(i), zPosVec.at(i));
         test2DHist->Fill(xPosVec.at(i), yPosVec.at(i), intADCmean.at(i));

   }


   TCanvas* c = new TCanvas("", "", 850, 800);
   TPad* aPad = new TPad("", "", 0.01, 0.01, 0.99, 0.99);
   gStyle->SetOptStat(0);
   test2DHist->SetMarkerSize(1.0);
   aPad->SetGrid();
   // c->SetGrid();
   // c->SetLeftMargin(0.1);
   // c->SetBottomMargin(0.1);

   c->cd();
   aPad->Draw();
   aPad->SetRightMargin(0.15);

   c->cd();
   aPad->cd();
   test2DHist->Draw("COLZ1,TEXT");
   c->SaveAs("./plots/IntADC_2D_Run_" + (TString)(std::to_string(runnum)) + "_64ch.png");	

}
