#include <TString.h>
void draw(const TString& filename, const TString& condition, const int rmin, const int rmax)
//void draw()
{
  gDirectory->pwd();
  int rminA = rmin*1000;
  int rmaxA = rmax*1000;
  TString Rmin= to_string(rmin);
  TString Rmax= to_string(rmax);
  TString RminA= to_string(rminA);
  TString RmaxA= to_string(rmaxA);

  //gROOT->ProcessLine(".x plot_spectrum_32ch_test.C");
  TString command1 = ".x plot_spectrum_32ch_update.C\( \"";
  command1 += filename + "\" , \"" + condition +"\" ) ";
  cout << command1 << endl;

  TString command2 = ".x plot_waveform_32ch_AvgTimeStr.C\( \"";
  command2 += filename + "\" ,"+RminA+","+RmaxA+", \"" + condition +"\" )";
  cout << command2 << endl;

  TString command3 = ".x plot_waveform_32ch.C\( \"";
  command3 += filename + "\" ,"+Rmin+","+Rmax+", \"" + condition +"\" )";
  cout << command3 << endl;
  
  gROOT->ProcessLine(command1);
  gROOT->ProcessLine(command2);
  gROOT->ProcessLine(command3);

} 
