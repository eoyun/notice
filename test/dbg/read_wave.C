void read_wave(const int runnum, const int mid){
    char filename[100];
	sprintf(filename,"WaveformSave_Wave_Run%d_Mid%d.root",runnum,mid);
	TFile *fp_root = new TFile(filename, "read");
    TTree *fp_tree = (TTree*)fp_root->Get("")
}
