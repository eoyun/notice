#include <stdio.h>

int show_fast_sipm_22_energyselect(const int runnum)
{
	int ch;
	FILE *fp[13];
	int file_size;
	int nevt;
	char header[13][64];
	short data[13][96];
	int evt;
	int i;
	int energy_c;
	int energy_s;
	char outname[100];
	char filename[100];
	int filenum;
	int run_max=0;
	int mid;
	int energy_total_c;
	int energy_total_s;
    int isceren;
	int timing;
	int time_flag_C;
	int time_flag_S;
	// get channel to plot, channel = 1 ~ 32
	// printf("Channel to plot(1~32) : ");
	//scanf("%d", &channel);
	//ch_to_plot = ch;
	sprintf(outname,"sipm_%d_2by2_energyslect.root",runnum);
	TFile *fp_root = new TFile(outname,"recreate");  
	TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 800, 800);
	c1->Divide(1, 2);
	TH1F *plot_C = new TH1F("plot_c", "Energy_c", 25000, -5000, 20000); 
	TH1F *plot_S = new TH1F("plot_s", "Energy_s", 25000, -5000, 20000); 
	TH1F *plot_C_t = new TH1F("plot_c_t", "timing_c", 1000, 0, 16000); 
	TH1F *plot_S_t = new TH1F("plot_s_t", "timing_s", 1000, 0, 16000); 
	plot_C->Reset();
	plot_S->Reset();

	// get # of events in file
//	for (filenum=0;filenum < 200;filenum++){
	for (mid=3;mid<16;mid++){
    	sprintf(filename,"/Users/drc_daq/scratch/Aug2022TB/SSD/SSD_Run_%d/Run_%d_Fast/Run_%d_Fast_MID_%d/Run_%d_Fast_MID_%d_FILE_0.dat",runnum,runnum,runnum,mid,runnum,mid);
    	//sprintf(filename,"/Volumes/HDD_16TB_1/HDD_Run_%d/Run_%d_Fast/Run_%d_Fast_MID_%d/Run_%d_Fast_MID_%d_FILE_0.dat",runnum,runnum,runnum,mid,runnum,mid);
    	//if (access(filename,0)!=0) break;
    	printf("%s\n",filename);
    	if (mid==3) {
        	fp[mid-3] = fopen(filename, "rb");
        	fseek(fp[mid-3], 0L, SEEK_END);
        	file_size = ftell(fp[mid-3]);
        	fclose(fp[mid-3]);
        	nevt = file_size / 256;
			printf("%d\n",nevt);
        }
    	fp[mid-3] = fopen(filename, "rb");
		
	}
	for (evt = 0; evt < nevt; evt++) {
		// read header
		time_flag_C=0;
		time_flag_S=0;
		energy_total_c=0;
		energy_total_s=0;
		//int kk =0;
		for (mid = 3;mid < 16 ; mid++){
    		fread(header[mid - 3], 1, 64, fp[mid - 3]);
    
    		// read fast data
    		fread(data[mid - 3], 2, 96, fp[mid - 3]);
    		for (ch=0; ch<32; ch++){
         		// fill waveform for channel to plot
         		timing = data[mid-3][ch*3+2] & 0xFFFF;
				//if (mid<9||(mid==11&&ch>23)){
				if ((mid==8&&ch==22)||(mid==6&&ch==17)){
    				energy_c = data[mid - 3][ch * 3 + 1] & 0xFFFF;
             		energy_c = energy_c * 65536;
             		energy_c = energy_c + (data[mid - 3][ch * 3] & 0xFFFF);
         			energy_total_c+=energy_c;
					if( energy_c>1000 ) time_flag_C=1;
					plot_C_t->Fill(timing);
				}
                //if ((mid>8&&!(mid==11))||(mid==11&&ch<8)){
                if ((mid==14&&ch==19)||(mid==13&&ch==14)){
            		energy_s = data[mid - 3][ch * 3 + 1] & 0xFFFF;
            		energy_s = energy_s * 65536;
            		energy_s = energy_s + (data[mid - 3][ch * 3] & 0xFFFF);
        			energy_total_s+=energy_s;
					if( energy_s>1500) time_flag_S=1;
					plot_S_t->Fill(timing);
				//	if( mid==11) printf("ch is %d\n",ch);
				//	kk+=1;
				}
                
    		//printf("energy : %d evt : %d\n",energy,evt);
			}
		}
		//printf("energy c is %d, energy s is %d, %d is number of ceren\n",energy_total_c,energy_total_s,kk);
		//if(time_flag_C)plot_C->Fill(energy_total_c/200);
	    //if(time_flag_S)plot_S->Fill(energy_total_s/200);
		if (time_flag_C+time_flag_S==2){
			printf("%d\n",evt);
			printf("C energy is %d| S energy is %d\n",energy_total_c,energy_total_s);
			plot_C->Fill(energy_total_c);
	    	plot_S->Fill(energy_total_s);
		}	
	}


    for(mid=3;mid<16;mid++){ 
	    fclose(fp[mid-3]);
	}
//	}

	//c1->cd(1);
	plot_C->Write();
	//c1->cd(2);
	plot_S->Write();

	//c1->Write();
	//c1->Modified();
	//c1->Update();
	//c1->cd(1);
	plot_C_t->Write();
	//c1->cd(2);
	plot_S_t->Write();
	//c1->Write();
	fp_root->Close();
	return 0;
}

