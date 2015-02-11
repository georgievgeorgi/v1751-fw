#ifndef _caen_analyse_h_
#define _caen_analyse_h_

#include"caen-raw.h"



#include<iostream>

//#include<TCanvas.h>
#include<TH1F.h>
#include<TH2F.h>
#include<TFile.h>
#include<TDirectory.h>





#include<iostream>
#include<sstream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<vector>
#include<map>
#include<iomanip>
#include<fstream>


namespace caen{
  class Analyse{
    public:
      void Init();
      void Process(Event& evt);
      void Finish();
      private:
      TFile* fRootFileP;
      TH1F* hist;
  };
};
#endif