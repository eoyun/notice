void test()
{
  //std::vector<double> ch;
  //std::vector<double> mean;
  //std::vector<double> stddev;
  // ch.clear();
  // mean.clear();
  // stddev.clear();

  // for(int i = 0; i < 32 ; i ++)  ch.push_back(i);
  // for(int i = 0; i < 32 ; i ++)  mean.push_back(i+10);
  // for(int i = 0; i < 32 ; i ++)  stddev.push_back(i+2);


  //TGraphErrors* gr = new TGraphErrors(ch.size(), &(ch[0]), &(mean[0]), 0, &(stddev[0]));
  //gr->Draw();
  int i,plot_index;

  for(i =0; i<64;i++){   
//     c1->cd(i+1);
     if ((i+1)%2!=0) {
       //std::cout<<((i+1)/16)<<std::endl;
       plot_index = (i+1)/2 + 1 + 8 * ((i+1)/16)-1;
     }
     else plot_index = (i+1)/2 + 8 * ((i)/16+1)-1;
     printf("i is %d | plot index is %d\n",i+1,plot_index+1);
  }
}
