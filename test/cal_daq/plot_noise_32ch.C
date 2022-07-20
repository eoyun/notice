#include <stdio.h>

int plot_noise_32ch(const char* filename)
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
  int cont;

  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 1600, 1000);
  c1->Divide(8,4,0.01,0.01);
  TH1F *plot1 = new TH1F("plot1", "Waveform", 400, 3300, 3700); 
  TH1F *plot2 = new TH1F("plot2", "Waveform", 400, 3300, 3700); 
  TH1F *plot3 = new TH1F("plot3", "Waveform", 400, 3300, 3700); 
  TH1F *plot4 = new TH1F("plot4", "Waveform", 400, 3300, 3700); 
  TH1F *plot5 = new TH1F("plot5", "Waveform", 400, 3300, 3700); 
  TH1F *plot6 = new TH1F("plot6", "Waveform", 400, 3300, 3700); 
  TH1F *plot7 = new TH1F("plot7", "Waveform", 400, 3300, 3700); 
  TH1F *plot8 = new TH1F("plot8", "Waveform", 400, 3300, 3700); 
  TH1F *plot9 = new TH1F("plot9", "Waveform", 400, 3300, 3700); 
  TH1F *plot10 = new TH1F("plot10", "Waveform", 400, 3300, 3700); 
  TH1F *plot11 = new TH1F("plot11", "Waveform", 400, 3300, 3700); 
  TH1F *plot12 = new TH1F("plot12", "Waveform", 400, 3300, 3700); 
  TH1F *plot13 = new TH1F("plot13", "Waveform", 400, 3300, 3700); 
  TH1F *plot14 = new TH1F("plot14", "Waveform", 400, 3300, 3700); 
  TH1F *plot15 = new TH1F("plot15", "Waveform", 400, 3300, 3700); 
  TH1F *plot16 = new TH1F("plot16", "Waveform", 400, 3300, 3700); 
  TH1F *plot17 = new TH1F("plot17", "Waveform", 400, 3300, 3700); 
  TH1F *plot18 = new TH1F("plot18", "Waveform", 400, 3300, 3700); 
  TH1F *plot19 = new TH1F("plot19", "Waveform", 400, 3300, 3700); 
  TH1F *plot20 = new TH1F("plot20", "Waveform", 400, 3300, 3700); 
  TH1F *plot21 = new TH1F("plot21", "Waveform", 400, 3300, 3700); 
  TH1F *plot22 = new TH1F("plot22", "Waveform", 400, 3300, 3700); 
  TH1F *plot23 = new TH1F("plot23", "Waveform", 400, 3300, 3700); 
  TH1F *plot24 = new TH1F("plot24", "Waveform", 400, 3300, 3700); 
  TH1F *plot25 = new TH1F("plot25", "Waveform", 400, 3300, 3700); 
  TH1F *plot26 = new TH1F("plot26", "Waveform", 400, 3300, 3700); 
  TH1F *plot27 = new TH1F("plot27", "Waveform", 400, 3300, 3700); 
  TH1F *plot28 = new TH1F("plot28", "Waveform", 400, 3300, 3700); 
  TH1F *plot29 = new TH1F("plot29", "Waveform", 400, 3300, 3700); 
  TH1F *plot30 = new TH1F("plot30", "Waveform", 400, 3300, 3700); 
  TH1F *plot31 = new TH1F("plot31", "Waveform", 400, 3300, 3700); 
  TH1F *plot32 = new TH1F("plot32", "Waveform", 400, 3300, 3700); 

    plot1->Reset();
    plot2->Reset();
    plot3->Reset();
    plot4->Reset();
    plot5->Reset();
    plot6->Reset();
    plot7->Reset();
    plot8->Reset();
    plot9->Reset();
    plot10->Reset();
    plot11->Reset();
    plot12->Reset();
    plot13->Reset();
    plot14->Reset();
    plot15->Reset();
    plot16->Reset();
    plot17->Reset();
    plot18->Reset();
    plot19->Reset();
    plot20->Reset();
    plot21->Reset();
    plot22->Reset();
    plot23->Reset();
    plot24->Reset();
    plot25->Reset();
    plot26->Reset();
    plot27->Reset();
    plot28->Reset();
    plot29->Reset();
    plot30->Reset();
    plot31->Reset();
    plot32->Reset();
 
  // get # of events in file
  fp = fopen(filename, "rb");
  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  fclose(fp);
  nevt = file_size / 65536;
  if (nevt > 1000)
    nevt = 1000;
  
  printf("-----------------------------------------------------------------------\n");
  fp = fopen(filename, "rb");

  for (evt = 0; evt < nevt; evt++) {
    // read header
    fread(data, 1, 64, fp);
    
    // read waveform
    fread(adc, 2, 32736, fp);
    
    // fill waveform for channel to plotgecit 
    
    for (i = 1; i < 1001; i++) {
      plot1->Fill(adc[i * 32 + 0]);
      plot2->Fill(adc[i * 32 + 1]);
      plot3->Fill(adc[i * 32 + 2]);
      plot4->Fill(adc[i * 32 + 3]);
      plot5->Fill(adc[i * 32 + 4]);
      plot6->Fill(adc[i * 32 + 5]);
      plot7->Fill(adc[i * 32 + 6]);
      plot8->Fill(adc[i * 32 + 7]);
      plot9->Fill(adc[i * 32 + 8]);
      plot10->Fill(adc[i * 32 + 9]);
      plot11->Fill(adc[i * 32 + 10]);
      plot12->Fill(adc[i * 32 + 11]);
      plot13->Fill(adc[i * 32 + 12]);
      plot14->Fill(adc[i * 32 + 13]);
      plot15->Fill(adc[i * 32 + 14]);
      plot16->Fill(adc[i * 32 + 15]);
      plot17->Fill(adc[i * 32 + 16]);
      plot18->Fill(adc[i * 32 + 17]);
      plot19->Fill(adc[i * 32 + 18]);
      plot20->Fill(adc[i * 32 + 19]);
      plot21->Fill(adc[i * 32 + 20]);
      plot22->Fill(adc[i * 32 + 21]);
      plot23->Fill(adc[i * 32 + 22]);
      plot24->Fill(adc[i * 32 + 23]);
      plot25->Fill(adc[i * 32 + 24]);
      plot26->Fill(adc[i * 32 + 25]);
      plot27->Fill(adc[i * 32 + 26]);
      plot28->Fill(adc[i * 32 + 27]);
      plot29->Fill(adc[i * 32 + 28]);
      plot30->Fill(adc[i * 32 + 29]);
      plot31->Fill(adc[i * 32 + 30]);
      plot32->Fill(adc[i * 32 + 31]);

      //if (adc[i * 32 + ch_to_plot] < 3000) printf("abnormal value = %d @ %d\n", adc[i * 32 + ch_to_plot], evt);
    }
    
    printf("%d/%d filled\n", evt + 1, nevt);
  }

    c1->Modified();
    c1->Update();
    c1->cd(1); plot1->Draw();
    c1->cd(2); plot2->Draw();
    c1->cd(3); plot3->Draw();
    c1->cd(4); plot4->Draw();
    c1->cd(5); plot5->Draw();
    c1->cd(6); plot6->Draw();
    c1->cd(7); plot7->Draw();
    c1->cd(8); plot8->Draw();
    c1->cd(9); plot9->Draw();
    c1->cd(10); plot10->Draw();
    c1->cd(11); plot11->Draw();
    c1->cd(12); plot12->Draw();
    c1->cd(13); plot13->Draw();
    c1->cd(14); plot14->Draw();
    c1->cd(15); plot15->Draw();
    c1->cd(16); plot16->Draw();
    c1->cd(17); plot17->Draw();
    c1->cd(18); plot18->Draw();
    c1->cd(19); plot19->Draw();
    c1->cd(20); plot20->Draw();
    c1->cd(21); plot21->Draw();
    c1->cd(22); plot22->Draw();
    c1->cd(23); plot23->Draw();
    c1->cd(24); plot24->Draw();
    c1->cd(25); plot25->Draw();
    c1->cd(26); plot26->Draw();
    c1->cd(27); plot27->Draw();
    c1->cd(28); plot28->Draw();
    c1->cd(29); plot29->Draw();
    c1->cd(30); plot30->Draw();
    c1->cd(31); plot31->Draw();
    c1->cd(32); plot32->Draw();

  fclose(fp);

  return 0;
}

