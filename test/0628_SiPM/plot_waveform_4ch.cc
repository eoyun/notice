#include <stdio.h>
#include <chrono>
#include <thread>
#include <TPad.h>
#include <string>
// #include <Windows.h>

constexpr int TIME_TO_SLEEP = 200;
//std::thie_thread::sleep_for( std::chrono::milliseconds(TIME_TO_SLEEP) )

void pad_set(TPad* tPad) {
  tPad->Draw();
  tPad->cd();

  tPad->SetTopMargin(0.08);
  tPad->SetLeftMargin(0.08);    
  tPad->SetRightMargin(0.08);
  tPad->SetBottomMargin(0.08);
}

int plot_waveform_4ch(std::string fileNum, enum runMode opt)
{
  int ch_to_plot;
  FILE *fp;
  int file_size;
  int nevt;
  char data[64];
  short adc[32736];

  int evtNum;
  if (opt == runMode::kPrompt) {
    std::cout << "enter evt num" << std::endl;
    scanf("%d", &evtNum);
  }

  TCanvas* c = new TCanvas("c", "c", 500, 500);

  std::vector<TH1F*> mod1_plotVec;
  for ( int i = 0; i < 32; i++ ) {
    mod1_plotVec.push_back(new TH1F((TString)("mod1 ch"+std::to_string(i)), (TString)("mod1 Waveform ch"+std::to_string(i)), 1000, 0, 200));
    mod1_plotVec.at(i)->SetStats(0);
    mod1_plotVec.at(i)->SetLineWidth(1);
    mod1_plotVec.at(i)->GetYaxis()->SetRangeUser(0, 4096);
    mod1_plotVec.at(i)->SetLineColor(1);
    mod1_plotVec.at(i)->SetTitle("");
  }

  std::vector<TH1F*> mod2_plotVec;
  for ( int i = 0; i < chInput.size(); i++ ) {
    mod2_plotVec.push_back(new TH1F((TString)("mod2 ch"+std::to_string(chInput.at(i))), (TString)("mod2 Waveform ch"+std::to_string(chInput.at(i))), 1000, 0, 200));
    mod2_plotVec.at(i)->SetStats(0);
    mod2_plotVec.at(i)->SetLineWidth(1);
    mod2_plotVec.at(i)->GetYaxis()->SetRangeUser(0, 4096);
    mod2_plotVec.at(i)->SetLineColor(1);
    mod2_plotVec.at(i)->SetTitle(plotName.at(i));
  }

  std::cout << DATA_DIR+FILE_NAME+"_1_"+fileNum+".dat" << std::endl; 

  // get # of events in file
  fp = fopen((DATA_DIR+FILE_NAME+"_1_"+fileNum+".dat").c_str(), "rb");
  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  fclose(fp);
  nevt = file_size / 65536;
  fp = fopen((DATA_DIR+FILE_NAME+"_1_"+fileNum+".dat").c_str(), "rb");

  for (int evt = 0; evt < nevt; evt++) {
  // for (int evt = 0; evt < 10; evt++) {

    for ( int t = 0; t < mod1_plotVec.size(); t++ ) mod1_plotVec.at(t)->Reset();
    fread(data, 1, 64, fp);
    printf("evt = %d\n", evt);
    // read waveform
    fread(adc, 2, 32736, fp);

    if (opt == runMode::kPrompt) {
      if (!(evt == evtNum)) continue;
    }

    for (int i = 0; i < 1000; i++) {
      for ( int t = 0; t < 32; t++ ) {
        mod1_plotVec.at(t)->Fill(i * 0.2, adc[(i + 1) * 32 + t]); 
      }
    }



    c->cd();
    for (int t = 0; t < 32; t++ ) {
      mod1_plotVec.at(t)->Draw("hist");
      c->SaveAs((TString)(BASE_DIR+"plots/waveform/"+(std::string)fileNum+"/1/"+std::to_string(t)+"/"+FILE_NAME+"_"+(std::string)fileNum+"_"+std::to_string(evt)+"_full.png"));
    }
  }
  fclose(fp);

  // fp = fopen((DATA_DIR+FILE_NAME+"_2_"+fileNum+".dat").c_str(), "rb");
  // fseek(fp, 0L, SEEK_END);
  // file_size = ftell(fp);
  // fclose(fp);
  // nevt = file_size / 65536;
  // fp = fopen((DATA_DIR+FILE_NAME+"_2_"+fileNum+".dat").c_str(), "rb");

  // for (int evt = 0; evt < nevt; evt++) {
  // // for (int evt = 0; evt < 10; evt++) {

  //   for ( int t = 0; t < mod2_plotVec.size(); t++ ) mod2_plotVec.at(t)->Reset();
  //   fread(data, 1, 64, fp);
  //   printf("evt = %d\n", evt);
  //   // read waveform
  //   fread(adc, 2, 32736, fp);

  //   if (opt == runMode::kPrompt) {
  //     if (!(evt == evtNum)) continue;
  //   }

  //   for (int i = 0; i < 1000; i++) {
  //     mod2_plotVec.at(0)->Fill(i * 0.2, adc[(i + 1) * 32 + chInput.at(0)]); 
  //     mod2_plotVec.at(1)->Fill(i * 0.2, adc[(i + 1) * 32 + chInput.at(1)]); 
  //     mod2_plotVec.at(2)->Fill(i * 0.2, adc[(i + 1) * 32 + chInput.at(2)]); 
      
  //   }
  //   c->cd();

  //   for (int t = 0; t < 3; t++ ) {
  //     mod2_plotVec.at(t)->Draw("hist");
  //     c->SaveAs((TString)(BASE_DIR+"plots/waveform/"+(std::string)fileNum+"/2/"+std::to_string(chInput.at(t))+"/"+FILE_NAME+"_"+(std::string)fileNum+"_"+std::to_string(evt)+"_full.png"));
  //   }
  // }
  // fclose(fp);

  return 0;
}

