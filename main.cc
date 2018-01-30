#include"main.h"
#include"caen_raw.h"
#include"2016-02-10-ledpmt-analyse_run.h"
#include"2016-02-10-ledpmt-analyse_burst.h"

#include<vector>

#include<algorithm>
#include<string>
#include<iostream>
#include<cctype>


namespace PMT_const{
  //double gain=1.5; //*1E5 1000V
  //double gain=.4; //*1E5 850V
  double gain=.7; //*1E5 900V
}



namespace caen{
  int gDebug;
  unsigned int gNBursts;
  std::string gOutputRootFile;
};

int main(int argc, char* argv[]) {
  int opt;
  std::vector<std::string> inputFiles,inputLists;
  std::string tmpstring;

  caen::gDebug=1; // default value
  caen::gNBursts=-1;


  const struct option longopts[] =
  {
    {"input",   required_argument,        0, 'i'},
    {"list" ,   required_argument,        0, 'l'},
    {0,0,0,0},
  };

  int index;
  int iarg=0;

  while ((opt = getopt_long(argc, argv, "i:l:o:d:b:", longopts, &index)) != -1){
    switch (opt){
      case 'i': tmpstring=optarg;inputFiles.push_back(tmpstring);break;
      case 'l': tmpstring=optarg;inputLists.push_back(tmpstring);break;
      case 'o': caen::gOutputRootFile=optarg;break;
      case 'd': caen::gDebug=atoi(optarg); break;
      case 'b': caen::gNBursts=atoi(optarg);break;
      default:
                fprintf(
                    stderr,
                    "SYNOPSIS: %s [-i input file] [-l list file] [-o root_file]\n",argv[0]);
                return 1;break;
    }
  }

  if(
      caen::gOutputRootFile.size()==0||
      (inputLists.size()==0&&inputFiles.size()==0))
  {
    fprintf(stderr,"SYNOPSIS: %s [-i input file] [-l list file] [-o root_file]\n",argv[0]);
    return 1;}


  char tmpchar[500];


  for(unsigned int i=0;i<inputLists.size();++i){
    std::ifstream list(inputLists[i].c_str(),std::ifstream::in);
    if(!list.is_open())continue;
    std::cerr<<"proc list "<<inputLists[i]<<std::endl;
    while(!list.eof()){
      //list>>tmpstring;
      list.getline((char*)tmpchar,500,'\n');
      tmpstring=tmpchar;
      tmpstring.erase(std::remove(tmpstring.begin(),tmpstring.end(),' '),tmpstring.end());
      if(tmpstring.find_first_of('#')!=std::string::npos)
        tmpstring.erase(tmpstring.find_first_of('#'),tmpstring.length()); // # comment char
      if(tmpstring.size()>0)
        inputFiles.push_back(tmpstring);
    }
  }

  caen::AnalyseRun anarun;
  anarun.Init();

unsigned int burst_i=0;
  for(std::vector<std::string>::iterator if_it=inputFiles.begin();
      if_it!=inputFiles.end();
      ++if_it){
    std::string& filename=*if_it;
    std::cerr<<"Processing \""<<filename<<"\""<<std::endl;
    if(burst_i>caen::gNBursts)break;
    burst_i++;



    RAW::Raw raw(filename);
    if(!raw.IsOpened())continue;
    caen::AnalyseBurst anaburst;
    unsigned int trig_i=0;
    while(!raw.Eof()){
      RAW::Event evt;
      raw.GetEvent(evt);
      if(trig_i==0)anaburst.Init(evt);
      anaburst.Process(evt);
      trig_i++;
    }
    anaburst.PostProcess();
    anarun.Process(anaburst.GetChannelsData());

    if(caen::gDebug>0){
      std::string burstrootfn=filename+".root";
      burstrootfn.erase(0,burstrootfn.find_last_of('/')+1);
      std::cout<<burstrootfn<<std::endl;
      anaburst.WriteToFile(burstrootfn);
    }
  }
  anarun.Finish();
  anarun.WriteToFile(caen::gOutputRootFile);





  return 0;
}
