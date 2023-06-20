#include <stdio.h>

int main(){

	FILE *fp;
	FILE *fo;

	long long file_size;
	int evt;
	int nevt;
	int i,j;
	char data[64];
	short adc[32736];
	int cont;

	fp=fopen("cal_wave_2_22.dat","rb");
	fo=fopen("test_read_1.txt","w");

	fseek(fp,0L,SEEK_END);
	file_size=ftell(fp);
	fclose(fp);
	nevt = file_size/65536;
	printf("%d",nevt);	
	fp=fopen("cal_wave_2_22.dat","rb");
	for (evt=0;evt<nevt;evt++){
		fread(data,1,64,fp);
		fread(adc,2,32736,fp);
		/*for (i=0;i<32;i++){
			plot[i]->Reset();
		}*/
		for (i=0;i<1023;i++){
			for (j=0;j<32;j++){
			       	//plot[j]->Fill(i,adc[i*32+j]);
				printf("%d\n",adc[i*32+j]);
				fprintf(fo,"%d\n",adc[i*32+j]);
				 
			}
		}
		/*
		for(i=0;i<32;i++){
			c1->cd(i+1);
			plot[i]->GetYaxis()->SetRangeUser(3000,4000);
			plot[i]->GetYaxis()->SetLabelSize(0.05);
			plot[i]->Draw("hist");

		}*/
		/*c1->Modified();
		c1->Update();
		printf("continue?\n");
		printf("%d",evt);
		scanf("%d",&cont);
		if(cont==0)break;*/
		printf("event : %d\n",evt+1);
		if(evt==99)
			break;
	}
	fclose(fo);
	fclose(fp);

  return 0;
}
