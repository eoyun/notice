#include <stdio.h>
#include <stdlib.h>

int show_dwc_timediff(const int runnum, const int ch1, const int ch2)
{
  int channel;
  int ch_to_plot;
  FILE *fp;
  FILE *fp_cut;
  int file_size;
  int nevt;
  char header[64];
  short data[96];
  int evt;
  int i;
  int energy;
  int timing1;
  int timing2;
  char cutfilename[256];
  char filename[256];
  char rootname[256];
  TH1F *timing1_histo = new TH1F("timing1","timing1",1000,0,16000);
  TH1F *timing2_histo = new TH1F("timing2","timing2",1000,0,16000);
//  char filename[100];

  // get channel to plot, channel = 1 ~ 32

  // get # of events in file
  sprintf(rootname,"dwc_timing_%d_%d.root",ch1,ch2);
  sprintf(cutfilename,"dwc_timing_%d_%d.txt",ch1,ch2);
  sprintf(filename,"/Volumes/HDD_16TB_2/HDD_Run_%d/Run_%d_Fast/Run_%d_Fast_MID_1/Run_%d_Fast_MID_1_FILE_0.dat",runnum,runnum,runnum,runnum);
  fp_cut = fopen(cutfilename,"wt");
  fp = fopen(filename, "rb");
  TFile *fp_root = new TFile(rootname,"recreate");
  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  fclose(fp);
  nevt = file_size / 256;
  
  fp = fopen(filename, "rb");

  for (evt = 0; evt < nevt; evt++) {
    // read header
    fread(header, 1, 64, fp);
    
    // read fast data
    fread(data, 2, 96, fp);
    
    // fill waveform for channel to plot
    energy = data[ch_to_plot * 3 + 1] & 0xFFFF;
    energy = energy * 65536;
    energy = energy + (data[ch_to_plot * 3] & 0xFFFF);

    timing1 = data[(ch1-1) * 3 + 2] & 0xFFFF;
    timing2 = data[(ch2-1) * 3 + 2] & 0xFFFF;
	//printf("timing ch1 is %d | timing ch2 is %d\n",timing1,timing2);
    if (abs(timing1-timing2)<200) {
		fprintf(fp_cut,"%d\n",evt); 
		timing1_histo->Fill(timing1);
		timing2_histo->Fill(timing2);
	    printf("timing ch1 is %d | timing ch2 is %d\n",timing1,timing2);
		//printf("%d\n",abs(timing1-timing2));
	}
	//if (timing>10000){
    //printf("energy : %d evt : %d\n",energy,evt);
    //plot_e->Fill(energy);
    //plot_t->Fill(timing);
    //}
  }
  timing1_histo->Write();
  timing2_histo->Write();

  fp_root->Close();
  fclose(fp_cut);
  fclose(fp);
  return 0;
}

