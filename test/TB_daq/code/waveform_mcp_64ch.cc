#include <stdio.h>

int waveform_mcp_64ch(const int runnum, const int min, const int max)
{
  const int mid1 = 19; // left side of cells (front view)
  const int mid2 = 8; // right side of cells (front view)

  char filename1[100];
  char filename2[100];
  FILE *fp1;
  FILE *fp2;
  int file_size;
  int nevt;
  char data1[64];
  char data2[64];
  short adc1[32736];
  short adc2[32736];
  int evt;
  int i;
  int j;
  int cont;
    
  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 700);
  c1->Divide(8,8,0.0001,0.0001);
  gPad->SetLeftMargin(0);
  gPad->SetRightMargin(0);
  gPad->SetBottomMargin(0);
  gPad->SetTopMargin(0);

  TH1F *plot[64];
  for( i = 0 ; i < 64 ; i ++) {
    plot[i] = new TH1F(Form("ch%d",i+1), Form("ch%d", i+1), 1023,0,1023);
    plot[i]->SetBit(TH1::kNoTitle);  
    plot[i]->SetStats(0);  
  }

  // const int runnumCopy = runnum;

  // For mid 1
  sprintf(filename1,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,mid1,runnum,mid1);
  fp1 = fopen(filename1, "rb");
  fseek(fp1, 0L, SEEK_END);
  file_size = ftell(fp1);
  fclose(fp1);
  nevt = file_size / 65536;
  std::cout << "Load file : " << filename1 << std::endl;
  fp1 = fopen(filename1, "rb");

  // For mid 2
  sprintf(filename2,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_0.dat",runnum,runnum,runnum,mid2,runnum,mid2);
  std::cout << "Load file : " << filename2 << std::endl;
  fp2 = fopen(filename2, "rb");

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

    fread(data1, 1, 64, fp1);
    fread(adc1, 2, 32736, fp1);

    fread(data2, 1, 64, fp2);
    fread(adc2, 2, 32736, fp2);

    for( i = 0 ; i < 64 ; i ++) 
      plot[i]->Reset();
    
    for (i = 0; i < 1023; i++) {
      for ( j = 0; j < 32 ; j++ )
        plot[moduleCh.at(j)-1]->Fill(i, adc1[i * 32 + daqCh.at(j)-1]);
      for ( j = 32; j < 64; j++ ) 
        plot[moduleCh.at(j)-1]->Fill(i, adc2[i * 32 + daqCh.at(j)-1]);
    }

    for ( i = 0; i < 64; i++ ) {   
      c1->cd(i+1);
      plot[i]->GetYaxis()->SetRangeUser(min,max);
      plot[i]->Draw("HIST");
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
  
  fclose(fp1);
  fclose(fp2);

  return 0;
}


