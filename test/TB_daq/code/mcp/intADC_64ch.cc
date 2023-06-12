#include <stdio.h>

int intADC_64ch(const int runnum, const int Mid, const int min, const int max)
{
  int ch_to_plot;
  FILE *fp;
  FILE *fp2;
  long long file_size;
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
  int cont;
  char filename[100];
  char rootfilename[100];
  float intADC;
  int ifile;
  int i;
  int j;
  int ped;
  int pedsum;


  TFile *file = new TFile("./roots/IntADC_Run_" + (TString)(std::to_string(runnum)) + "_64ch.root", "RECREATE");

  std::vector<float> tmpWave;
  std::vector<TH1F*> intADCHist;

  for ( i = 0; i < 64; i++ ) {
    intADCHist.push_back(new TH1F((TString)(std::to_string(i+1)), "IntADC;Int.ADC;Evts", 1000, -1000., 50000.));
    intADCHist.at(i)->SetLineColor(kBlack);
  }

  gStyle->SetOptFit(1);

  for ( ifile = 0; ifile < 100; ifile++ ) {  
    sprintf(filename,"/Users/yhep/scratch/YUdaq/Run_%d/Run_%d_Wave/Run_%d_Wave_MID_%d/Run_%d_Wave_MID_%d_FILE_%d.dat",runnum,runnum,runnum,Mid,runnum,Mid,ifile);
    if (access(filename,0)!=0)break;
    printf("%s\n",filename);
    fp = fopen(filename, "rb");
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fclose(fp);
    nevt = file_size / 65536;
    
    printf("-----------------------------------------------------------------------\n");
    fp = fopen(filename, "rb");
   
   
    for (evt = 0; evt < nevt; evt++) {
     
      fread(data, 1, 64, fp);
      fread(adc, 2, 32736, fp);

      for ( j = 0; j < 64; j++ ) {

        tmpWave.clear();

        if ( j > 31 ) {
          intADCHist.at(j)->Fill(0);
        }

        else {
          ped = 0;
          pedsum = 0;
          for ( i = 1; i < 101; i++) {
            pedsum += adc[i * 32 + j];
          }
          ped += pedsum / 100.;
      
             for ( i = 0; i < 1023; i++ ) {
               tmpWave.push_back(adc[i * 32 + j]);
             }
    
              float intADC = 0.f;
              for ( i = min; i < max; i++ ) {
                  int waveformBin = i + 1;
                  intADC += ped - tmpWave[waveformBin];
              }
          
          intADCHist.at(j)->Fill(intADC);
        }
      }
    }
    fclose(fp);
  }

  for ( i = 0; i < 64; i++ ) {
    intADCHist.at(i)->SetOption("HIST");
    intADCHist.at(i)->Write();
  }

    file->Close();

  return 0;
}

