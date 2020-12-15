#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "StdArg.hpp"
#include "src/Reader.h" 
#include "src/Processor.h"
#include "src/Selector.h"
#include "src/ZbSelection.h"

#include "src/Global.h"

std::vector<std::string> splitNames(const std::string& files, std::string sep = ",")
{
  std::vector<std::string> fileList;
  for (size_t i=0,n; i <= files.length(); i=n+1)
  {
    n = files.find_first_of(sep,i);
    if (n == string::npos)
      n = files.length();
    std::string tmp = files.substr(i,n-i);
    std::string ttmp;
    for(unsigned int j=0; j<tmp.size(); j++)
    {
      if(tmp[j]==' ' || tmp[j]=='\n') continue;
      ttmp+=tmp[j];
    }
    fileList.push_back(ttmp);
  }
  return fileList;

}

void SetParameters(std::string fName, glob::Parameters& para) {
  std::string line;
  std::ifstream myfile (fName);
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      
      //skip comment line start with "//"
      if (line.find("//") != std::string::npos) continue ;
      
      std::vector<std::string> cont ;
      std::vector<std::string> cont_no_space ;
      //cout << line << '\n';
      std::string delim(" ") ;
      boost::split(cont, line, boost::is_any_of(delim));
      //std::cout << "\nCont size: " << cont.size() ;
      for (size_t i = 0 ; i < cont.size() ; ++i) {
        //std::cout << "\n" << cont[i] << cont[i].find(" ") << " " << std::string::npos;
        if (cont[i].find_first_not_of(' ') != std::string::npos) {
          cont_no_space.push_back(cont[i]) ;
        }
      }
      //std::cout << std::endl ;
      //std::cout << "\nCont no splace size: " << cont_no_space.size() ;
      //for (size_t i = 0 ; i < cont_no_space.size() ; ++i) {
      //  std::cout << " " << cont_no_space[i] ;
      //}
      //std::cout << std::endl ;

      if (cont_no_space.size() != 3) {
        std::cout << "\n Need name, value, value type. Will skip this \"" << line << "\"" << std::endl ;
      }
      else {
        std::string name = cont_no_space[0] ;
        if (cont_no_space[2] == "int") para.Set(name, std::stoi(cont[1])) ;
        if (cont_no_space[2] == "float") para.Set(name, std::stof(cont_no_space[1])) ;
        if (cont_no_space[2] == "string") para.SetStr(name, cont_no_space[1]) ;
      }
    }

    myfile.close();
  }
  
  else cout << "Unable to open file"; 

}



int main(int argc, char *argv[]) {
  
  StdArg arg(argc,argv);
  std::cout << "======================================================================" << std::endl;
  std::cout << "Z+b analysis call:" << std::endl;
  for (int i=0;i<argc;i++) std::cout << argv[i] << " ";
  std::cout << endl;
  std::cout << "======================================================================" << std::endl;
  arg.keys << "-in" << "-filelist" << "-cfg" << "-out" << "-data" << "-year" << "-syst" 
           << "-firstentry" << "-lastentry" ; 
  arg.Process();

  std::vector<std::string> filenames;
  if ( arg.Key("-in")) 
    { 
      string inputfilenames;
      inputfilenames  = arg.Get<string>("-in"); 
      filenames = splitNames(inputfilenames);
    }
  else 
    { 
      string inputfilename;
      if (arg.Key("-filelist"))
	{
	  inputfilename  = arg.Get<string>("-filelist");
	  std::ifstream fList(inputfilename.c_str());
	  if (!fList){
	    cerr << ">>Can't open file " << inputfilename << endl;
	    return 1;
	  }
	  char lineFromFile[10000];
	  while (fList.getline(lineFromFile,10000)){
	    if ( strlen(lineFromFile)>0 ) filenames.push_back(string(lineFromFile));
	  }
	}
    }

  std::string cfgfilename =  "Configs/inputParameters.txt";
  if (arg.Key("-cfg")) cfgfilename = arg.Get<string>("-cfg");
  else std::cout << "\nUsing default configuration " << cfgfilename << std::endl; 
  
  std::string outputfilename =  "output.root";
  if (arg.Key("-out")) outputfilename = arg.Get<string>("-out");
  
  bool isData(false) ;
  
  int intisdata=0;
  int intfirstentry=0;
  long int intlastentry=-1;
  string syst = "none";
  string year = "";

  if (arg.Key("-data")) intisdata = arg.Get<int>("-data");
  if (arg.Key("-year")) year = arg.Get<string>("-year");
  if (arg.Key("-firstentry")) intfirstentry = arg.Get<int>("-firstentry");
  if (arg.Key("-lastentry")) intlastentry = arg.Get<int>("-lastentry");
  if (arg.Key("-syst")) syst = arg.Get<string>("-syst");

  if(intisdata!=0) isData=true;
  
  std::cout << "\n=================================" << std::endl ;
  std::cout << "\nIs data:              " << isData ;
  std::cout << "\nYear:                 " << year ;
  std::cout << "\nFirst and last entry: " << intfirstentry << " " << intlastentry ;
  std::cout << "\nSystematic:           " << syst ;
  
  std::cout << std::endl ;

  SetParameters(cfgfilename,CUTS) ;

#if defined(TFILE)
  TFile* f = TFile::Open(filenames[0].c_str());
  TTree* chain = (TTree*)f->Get("Events");
#endif
#if defined(TCHAIN)
  std::string chainName("Events") ;
  TChain* chain = new TChain(chainName.c_str()) ;
  for ( std::vector<std::string>::const_iterator it = filenames.begin();it != filenames.end(); it++) {
    cout << "reading file " << it->c_str() << endl;
    int retcode = chain->Add(it->c_str(),-1);
    if ( retcode == 0 ) throw std::invalid_argument("the file "+*it+" does not exist of does not contain the tree named "+chainName);
  }
#endif 
  //chain.Add("/uscms/home/duong/Scratch/test/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7_18644D51-7512-5642-B506-C8B6BA051047.root") ;
  
  std::cout << "\n Number of events: " << chain->GetEntries() ;
  if (intlastentry == -1) intlastentry = chain->GetEntries() ;
  
  Processor ana ;
  ana.setOutPutFileName(outputfilename) ;
  ana.SetDataInfo(isData,year) ;
  
  //collection of all selectors
  std::vector<Selector*> sels;
  
  //Selection for Zb
  ZbSelection sel ;
  
  std::string fName_btagSF;
  std::vector<std::string> fName_eleTrig ;
  std::string fName_eleRecSF;
  std::string fName_eleIDSF;
  std::vector<float> lw_eleTrig; //weights estimated from luminosity fraction used to calculate the scale factors from different run periods
  std::vector<std::string> fName_muonTrig ;
  std::vector<std::string> fName_muonID ;
  std::vector<std::string> fName_muonIso ;
  std::vector<float> lw_muonTrig; //weights estimated from luminosity fraction used to calculate the scale factors from different run periods
  std::vector<float> lw_muonID;
  std::vector<float> lw_muonIso;

  std::string fName_puSF;

#if defined(MC_2016) || defined(DATA_2016)
  std::string fName_roc("CalibData/RoccoR2016.txt") ;
#endif
#if defined(MC_2017) || defined(DATA_2017)
  std::string fName_roc("CalibData/RoccoR2017.txt") ;
#endif
#if defined(MC_2018) || defined(DATA_2018)
  std::string fName_roc("CalibData/RoccoR2018.txt") ;
#endif

#if defined(DATA_2016)
  //std::string fName_lumiMaskFilter("CalibData/Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt");
  std::string fName_lumiMaskFilter("CalibData/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt");
#endif
#if defined(DATA_2017)
  std::string fName_lumiMaskFilter("CalibData/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt");
#endif
#if defined(DATA_2018)
  //std::string fName_lumiMaskFilter("CalibData/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt");
  std::string fName_lumiMaskFilter("CalibData/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt");
#endif
  
  std::string btagUncType = "central";
  if (syst == "BTAGU") btagUncType = "up";
  if (syst == "BTAGD") btagUncType = "down";

  std::string eleUncType = "central";
  std::string muonUncType = "central";
  if (syst == "ELEU") eleUncType = "up";
  if (syst == "ELED") eleUncType = "down";
  if (syst == "ELETU") eleUncType = "trigup";
  if (syst == "ELETD") eleUncType = "trigdown";
  if (syst == "MUONU") muonUncType = "up";
  if (syst == "MUOND") muonUncType = "down";
  if (syst == "MUONTU") muonUncType = "trigup";
  if (syst == "MUONTD") muonUncType = "trigdown";


#ifdef MC_2016
  fName_btagSF = "CalibData/DeepCSV_2016LegacySF_WP_V1.csv" ;
  fName_eleTrig.push_back("CalibData/sf_ele_2016_trig_v5.root");
  fName_eleRecSF = "CalibData/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root" ;
  fName_eleIDSF = "CalibData/2016LegacyReReco_ElectronTight_Fall17V2.root" ;
  lw_eleTrig.push_back(1.0);//FIXME for place holder
  fName_muonTrig.push_back("CalibData/EfficienciesAndSF_RunBtoF_trigger_2016_muon.root");
  fName_muonTrig.push_back("CalibData/EfficienciesAndSF_Period4_trigger_mu2016_muon.root");
  lw_muonTrig.push_back(0.5);//FIXME
  lw_muonTrig.push_back(0.5);//FIXME
  fName_muonID.push_back("CalibData/RunBCDEF_SF_ID_2016_muon.root");
  fName_muonID.push_back("CalibData/RunGH_SF_ID_2016_muon.root");
  lw_muonID.push_back(0.5);//FIXME
  lw_muonID.push_back(0.5);//FIXME
  fName_muonIso.push_back("CalibData/RunBCDEF_SF_ISO_2016_muon.root");
  fName_muonIso.push_back("CalibData/RunGH_SF_ISO_2016_muon.root");
  lw_muonIso.push_back(0.5);//FIXME
  lw_muonIso.push_back(0.5);//FIXME

  fName_puSF = "CalibData/2016_pileup_ratio.root";
  if (syst == "PUU") fName_puSF = "CalibData/2016_pileup_ratio_up.root";
  if (syst == "PUD") fName_puSF = "CalibData/2016_pileup_ratio_down.root";
#endif
#ifdef MC_2017
  fName_btagSF = "CalibData/DeepCSV_94XSF_WP_V4_B_F.csv";
  fName_eleTrig.push_back("CalibData/sf_ele_2017_trig_v5.root");
  fName_eleRecSF = "CalibData/egammaEffi.txt_EGM2D_runBCDEF_passingRECO_2017.root";
  fName_eleIDSF = "CalibData/2017_ElectronTight.root";
  lw_eleTrig.push_back(1.0);//FIXME for place holder
  fName_muonTrig.push_back("CalibData/EfficienciesAndSF_RunBtoF_Nov17Nov2017_trigger_muon.root");
  lw_muonTrig.push_back(1.);
  fName_muonID.push_back("CalibData/RunBCDEF_SF_ID_syst_2017_muon.root");
  lw_muonID.push_back(1.);
  fName_muonIso.push_back("CalibData/RunBCDEF_SF_ISO_syst_2017_muon.root");
  lw_muonIso.push_back(1.);

  fName_puSF = "CalibData/2017_pileup_ratio.root";
  if (syst == "PUU") fName_puSF = "CalibData/2017_pileup_ratio_up.root";
  if (syst == "PUD") fName_puSF = "CalibData/2017_pileup_ratio_down.root";
#endif
#ifdef MC_2018
  fName_btagSF = "CalibData/DeepCSV_102XSF_WP_V1.csv";
  fName_eleTrig.push_back("CalibData/sf_ele_2018_trig_v5.root");
  fName_eleRecSF = "CalibData/egammaEffi.txt_EGM2D_updatedAll_2018.root";
  fName_eleIDSF = "CalibData/2018_ElectronTight.root";
  lw_eleTrig.push_back(1.0);//FIXME for place holder
  fName_muonTrig.push_back("CalibData/EfficienciesAndSF_2018Data_BeforeMuonHLTUpdate_trigger_muon.root");
  fName_muonTrig.push_back("CalibData/EfficienciesAndSF_2018Data_AfterMuonHLTUpdate_trigger_muon.root");
  lw_muonTrig.push_back(0.5);//FIXME
  lw_muonTrig.push_back(0.5);//FIXME 
  fName_muonID.push_back("CalibData/RunABCD_SF_ID_2018_muon.root");
  lw_muonID.push_back(1.);
  fName_muonIso.push_back("CalibData/RunABCD_SF_ISO_2018_muon.root");
  lw_muonIso.push_back(1.);

  fName_puSF = "CalibData/2018_pileup_ratio.root";
  if (syst == "PUU") fName_puSF = "CalibData/2018_pileup_ratio_up.root";
  if (syst == "PUD") fName_puSF = "CalibData/2018_pileup_ratio_down.root";
#endif
#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
  sel.SetRandom() ; //used for muon rochestor correction (used when correcting for MC)
  if (CUTS.GetStr("jet_main_btagWP")=="deepCSVT") sel.SetBtagCalib(fName_btagSF,"DeepCSV","CalibData/effT.root",btagUncType);
  if (CUTS.GetStr("jet_main_btagWP")=="deepJetT") sel.SetBtagCalib(fName_btagSF,"DeepJet","CalibData/effT.root",btagUncType);
  if (CUTS.GetStr("jet_main_btagWP")=="deepCSVM") sel.SetBtagCalib(fName_btagSF,"DeepCSV","CalibData/effM.root",btagUncType);
  if (CUTS.GetStr("jet_main_btagWP")=="deepJetM") sel.SetBtagCalib(fName_btagSF,"DeepJet","CalibData/effM.root",btagUncType);
  sel.SetEleEffCorr(fName_eleTrig,fName_eleRecSF,fName_eleIDSF,lw_eleTrig,eleUncType);
  sel.SetMuonEffCorr(fName_muonTrig,fName_muonID,fName_muonIso,lw_muonTrig,lw_muonID,lw_muonIso,muonUncType);
  sel.SetPileupSF(fName_puSF);
#endif
  sel.SetRochesterCorr(fName_roc) ;
#if defined(DATA_2016) || defined(DATA_2017) || defined(DATA_2018)
  sel.SetLumiMaskFilter(fName_lumiMaskFilter);
#endif

  if (syst == "JETNOM") sel.SetJetMetSyst("jetnom");
  if (syst == "METNOM") sel.SetJetMetSyst("metnom");
  if (syst == "JETMETNOM") sel.SetJetMetSyst("jetmetnom");
  if (syst == "JESU") sel.SetJetMetSyst("jesu");
  if (syst == "JESD") sel.SetJetMetSyst("jesd");
  if (syst == "JERU") sel.SetJetMetSyst("jeru");
  if (syst == "JERD") sel.SetJetMetSyst("jerd");
  if (syst == "METJESU") sel.SetJetMetSyst("metjesu");
  if (syst == "METJESD") sel.SetJetMetSyst("metjesd");
  if (syst == "METJERU") sel.SetJetMetSyst("metjeru");
  if (syst == "METJERD") sel.SetJetMetSyst("metjerd");

  sels.push_back(&sel) ;
  
  //add all selectors to processors
  for (std::vector<Selector*>::iterator it = sels.begin() ; it != sels.end() ; it++) ana.addSelector(*it) ;
  
  chain->Process(&ana,"",intlastentry,intfirstentry) ;
  
  return 0 ;
}
