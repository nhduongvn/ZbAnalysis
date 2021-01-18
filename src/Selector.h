#ifndef Selector_h
#define Selector_h

#include "Reader.h"
#include "Obj.cxx"
#include "TLorentzVector.h"

#include "BTagCalibrationStandalone.h"
//#include "BTagCalibrationStandalone.cpp"
#include "RoccoR.h"
#include "LumiMaskFilter.h"

#include "TFile.h"
#include "TH2F.h"

#include "TRandom3.h"

//Base class for all selectors
class Selector
{
  public:
    //const member needs to be initialized in intialisation list meaning m_isData(isData)
    Selector() {} ;
    virtual ~Selector() {} ;
    // These methods are called at the corresponding stage of processing of TSelector
  virtual void SlaveBegin(Reader* r) {} ;
  virtual void Process(Reader* r) ;
  virtual void SlaveTerminate(Reader* r) {} ;
  virtual void Terminate(TList* mergedList, std::string outFileName) {} ; //outFileName is used to write parameter, like cuts, to output file
  virtual void SetRandom() ;
  virtual void SetDataInfo(bool isData, std::string year) {m_isData = isData ; m_year = year ; } ;
  
  virtual void SetBtagCalib(std::string csvFileName, std::string taggerName, std::string effFileName, std::string btagUncType) ;
  virtual void SetEleEffCorr(std::vector<std::string> fName_trig, std::string fName_recSF, std::string fName_IDSF, std::vector<float> w_trig, std::string eleUncType) ;
  virtual void SetMuonEffCorr(std::vector<std::string> fName_trig, std::vector<std::string> fName_ID, std::vector<std::string> fName_iso, std::vector<float> w_trig, std::vector<float> w_ID, std::vector<float> w_iso, std::string muonUncType) ;
  virtual void SetRochesterCorr(std::string fName_roc) ;
  virtual void SetLumiMaskFilter(std::string fName_lumiMaskFilter);
  virtual void SetPileupSF(std::string fName_puSF);
  virtual float PileupSF(int nTrueInt);
  virtual std::vector<float> GetSF_2DHist(float x, float y, std::vector<TH2F*> h, std::vector<float> w);
  virtual float CalBtagWeight(std::vector<JetObj>& jets, std::string jet_main_bTagWP="deepCSVT", std::string uncType="central") ;
  virtual float CalEleSF(LepObj e1, LepObj e2);
  virtual float CalSingleEleSF(LepObj e1);
  virtual float CalMuonSF_id_iso(LepObj e1, LepObj e2);
  virtual float CalSingleMuonSF_id_iso(LepObj e1);
  virtual float CalTrigSF(int id, LepObj lep1, LepObj lep2, TLorentzVector trigObj, TH1D* h_dR1_trig, TH1D* h_dR2_trig, TH1D* h_pt1_trig, TH1D* h_pt2_trig) ;
  virtual float CalTrigSF_singleLepton(int id, LepObj lep1, TLorentzVector trigObj);
  virtual TLorentzVector GetTrigObj(Reader* r, unsigned id, unsigned bits, float ptThresh) ;

#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
  virtual unsigned MatchGenLep(Reader* r, LepObj lep, int pdgId) ;
#endif
  virtual float MuonRcSF(Reader* r, LepObj lep, int pdgId) ;

  virtual void SetJetMetSyst(std::string jetmetsystType) {m_jetmetSystType = jetmetsystType;};
  virtual void SetPdfScaleSyst(std::string pdfScaleSystType) {
    m_nScale = 0;
    if (pdfScaleSystType=="scale") m_nScale=9;
    m_iPdfStart = 0;
    m_iPdfStop = 0;
    if (pdfScaleSystType == "pdfg0") {m_iPdfStart=0; m_iPdfStop=35;}
    if (pdfScaleSystType == "pdfg1") {m_iPdfStart=35; m_iPdfStop=70;}
    if (pdfScaleSystType == "pdfg2") {m_iPdfStart=70; m_iPdfStop=103;}
  };


  bool m_isData ;
  std::string m_year ;

  TRandom3* m_rand ;
  //for btagging SFs
  TFile* m_btagEffFile ;
  BTagCalibration m_btagCal ;
  BTagCalibrationReader m_btagReader ;
  RoccoR m_rc ;
  LumiMaskFilter m_lumiFilter ;

  //for electron SFs
  std::vector<TH2F*> m_hSF_eleTrig ;
  TH2F* m_hSF_eleRec ;
  TH2F* m_hSF_eleID ;
  std::vector<float> m_eleTrig_w ;
  
  std::vector<TH2F*> m_hSF_muonTrig ;
  std::vector<TH2F*> m_hSF_muonIso ;
  std::vector<TH2F*> m_hSF_muonID ;
  std::vector<float> m_muonTrig_w ;
  std::vector<float> m_muonIso_w ;
  std::vector<float> m_muonID_w ;

  std::string m_btagUncType;
  std::string m_eleUncType;
  std::string m_muonUncType;
  std::string m_jetmetSystType;

  unsigned m_nScale;
  unsigned m_iPdfStart;
  unsigned m_iPdfStop;

  TH1D* m_hSF_pu;

};
#endif
