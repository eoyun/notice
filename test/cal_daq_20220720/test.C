void test()
{
  std::vector<double> ch;
  std::vector<double> mean;
  std::vector<double> stddev;
   ch.clear();
   mean.clear();
   stddev.clear();

   for(int i = 0; i < 32 ; i ++)  ch.push_back(i);
   for(int i = 0; i < 32 ; i ++)  mean.push_back(i+10);
   for(int i = 0; i < 32 ; i ++)  stddev.push_back(i+2);


  TGraphErrors* gr = new TGraphErrors(ch.size(), &(ch[0]), &(mean[0]), 0, &(stddev[0]));
  gr->Draw();
}
