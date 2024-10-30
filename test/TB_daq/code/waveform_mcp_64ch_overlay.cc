#include <stdio.h>

int waveform_mcp_64ch_overlay(const int runnum, const int min, const int max)
{
  const int midS1 = 1; // left side of cells (front view)
  const int midS2 = 2; // right side of cells (front view)
  const int midC1 = 3; 
  const int midC2 = 4;

  char filenameS1[100];
  char filenameS2[100];
  char filenameC1[100];
  char filenameC2[100];
  FILE *fpS1;
  FILE *fpS2;
  FILE *fpC1;
  FILE *fpC2;
  int file_size;
  int nevt;
  char dataS1[64];
  char dataS2[64];
  char dataC1[64];
  char dataC2[64];
  short adcS1[32736];
  short adcS2[32736];
  short adcC1[32736];
  short adcC2[32736];
  int evt;
  int i;
  int j;
  int cont;
    
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 900, 800);
  c1->Divide(8,8,0.0001,0.0001);
  gPad->SetLeftMargin(0);
  gPad->SetRightMargin(0);
  gPad->SetBottomMargin(0);
  gPad->SetTopMargin(0);

  TH1F *plotS[64];
  TH1F *plotC[64];

  for( i = 0 ; i < 64 ; i ++) {
    plotS[i] = new TH1F(Form("S%d",i+1), Form("S%d", i+1), 1023,0,1023);
    plotS[i]->SetBit(TH1::kNoTitle);  
    plotS[i]->SetStats(0);  
    plotC[i] = new TH1F(Form("C%d",i+1), Form("C%d", i+1), 1023,0,1023);
    plotC[i]->SetBit(TH1::kNoTitle);  
    plotC[i]->SetStats(0);  
  }

  // const int runnum = runnum;

  // For mid S1
  sprintf(filenameS1,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,midS1,runnum,midS1);
  fpS1 = fopen(filenameS1, "rb");
  fseek(fpS1, 0L, SEEK_END);
  file_size = ftell(fpS1);
  fclose(fpS1);
  nevt = file_size / 65536;
  std::cout << "Load file : " << filenameS1 << std::endl;
  fpS1 = fopen(filenameS1, "rb");

  // For mid S2
  sprintf(filenameS2,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,midS2,runnum,midS2);
  std::cout << "Load file : " << filenameS2 << std::endl;
  fpS2 = fopen(filenameS2, "rb");

  // For mid C1
  sprintf(filenameC1,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,midC1,runnum,midC1);
  std::cout << "Load file : " << filenameC1 << std::endl;
  fpC1 = fopen(filenameC1, "rb");

  // For mid C2
  sprintf(filenameC2,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,midC2,runnum,midC2);
  std::cout << "Load file : " << filenameC2 << std::endl;
  fpC2 = fopen(filenameC2, "rb");

  // Read the mapping file.
  int module, daq;
  std::vector<int> moduleCh;
  std::vector<int> daqCh;

  std::ifstream in;
  in.open("mcpMap_TB2024.csv", std::ios::in);
  while (true) {
    in >> module >> daq;

    if ( !in.good() )
      break;

    moduleCh.push_back(module);
    daqCh.push_back(daq);
  }
  in.close();

  for (evt = 0; evt < nevt; evt++) {

    fread(dataS1, 1, 64, fpS1);
    fread(adcS1, 2, 32736, fpS1);
    fread(dataS2, 1, 64, fpS2);
    fread(adcS2, 2, 32736, fpS2);
    fread(dataC1, 1, 64, fpC1);
    fread(adcC1, 2, 32736, fpC1);
    fread(dataC2, 1, 64, fpC2);
    fread(adcC2, 2, 32736, fpC2);

    for( i = 0 ; i < 64 ; i ++) {
      plotS[i]->Reset();
      plotC[i]->Reset();
    }
    
    for (i = 0; i < 1023; i++) {
      for ( j = 0; j < 32 ; j++ ) {
        plotS[moduleCh.at(j)-1]->Fill(i, adcS1[i * 32 + daqCh.at(j)-1]);
        plotC[moduleCh.at(j)-1]->Fill(i, adcC1[i * 32 + daqCh.at(j)-1]);
      }
      for ( j = 32; j < 64; j++ ) {
        plotS[moduleCh.at(j)-1]->Fill(i, adcS2[i * 32 + daqCh.at(j)-1]);
        plotC[moduleCh.at(j)-1]->Fill(i, adcC2[i * 32 + daqCh.at(j)-1]);
      }
    }

    for ( i = 0; i < 64; i++ ) {   
      c1->cd(i+1);
      plotS[i]->GetYaxis()->SetRangeUser(min,max);
      plotS[i]->SetLineColor(kRed);
      plotS[i]->Draw("HIST");

      plotC[i]->GetYaxis()->SetRangeUser(min,max);
      plotC[i]->SetLineColor(kBlue);
      plotC[i]->Draw("HIST SAME");
    }

    // c1->SaveAs((TString)std::to_string(evt) + ".pdf");
    c1->Modified();
    c1->Update();

    std::cout << "Evt " << evt + 1 << ", Continue? ";
    scanf("%d", &cont);
    std::cout << std::endl;
    
    if (cont == 0)
      evt = nevt;
  }
  
  fclose(fpS1);
  fclose(fpS2);
  fclose(fpC1);
  fclose(fpC2);

  return 0;
}


