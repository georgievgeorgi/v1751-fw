#include"analyse.h"
#include"caen-raw.h"


namespace caen{
  void Analyse::Init(){
    fRootFileP=new TFile("out.root","recreate");
    hist=new TH1F("chan1","chan1",3000,0,3000);
  }

  void Analyse::Process(Event& evt){
    for(Event::channels_iterator it=evt.GetChannel(0).begin();it!=evt.GetChannel(0).end();++it){
      hist->Fill(*it);
    }

    //evt.Info();
  }
  void Analyse::Finish(){
    fRootFileP->Write();
    fRootFileP->Close();
    delete fRootFileP;

  }

};