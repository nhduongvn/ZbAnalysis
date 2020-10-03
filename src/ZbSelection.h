#ifndef ZbSelection_h
#define ZbSelection_h

#include "Reader.h"
#include "Selector.h"
#include "Plots.cxx"


//The Selection does not have Begin, since we do not have anything to do at the begining (overall) at client
//The histograms, ..., are booked and added to output list at SlaveBegin
//We need to have terminate since we might want to do overall tasks related to this selection only. At termination, all inforamtion from slaves is added. Example task is cutflow for this selection, which need information from all slaves.
//SlaveTerminate and Terminate are here for reservation now. histograms are written back in SlaveTerminate of Processor class

class ZbSelection : public Selector {
public:
  ZbSelection() {} ;
  virtual ~ZbSelection() ;
    // These methods are called at the corresponding stage of processing of TSelector
  virtual void SlaveBegin(Reader* r) ;
  virtual void Process(Reader* r) ;
  virtual void SlaveTerminate(Reader* r) {} ;
  virtual void Terminate(TList* mergedList, std::string outFileName) ;
  virtual void Fill_btagEffi(std::vector<JetObj> jets, float w=1) ;
  virtual void FillUnfolding(std::vector<TLorentzVector> recs, std::vector<TLorentzVector> gens, UnfoldingPlots* h, float w_rec, float w_gen) ;
private:

  //histograms
  ZbPlots* h_zee_jet ;
  ZbPlots* h_zmm_jet ;
  ZbPlots* h_zee_bjet ;
  ZbPlots* h_zmm_bjet ;
  Z2bPlots* h_zee_2bjet ;
  Z2bPlots* h_zee_2bjet_bb ;
  Z2bPlots* h_zee_2bjet_bX ;
  Z2bPlots* h_zee_2bjet_XX ;
  Z2bPlots* h_zmm_2bjet ;
  Z2bPlots* h_zmm_2bjet_bb ;
  Z2bPlots* h_zmm_2bjet_bX ;
  Z2bPlots* h_zmm_2bjet_XX ;

  ZbPlots* h_zee_bjet_deepJet ;
  ZbPlots* h_zmm_bjet_deepJet ;
  Z2bPlots* h_zee_2bjet_deepJet ;
  Z2bPlots* h_zmm_2bjet_deepJet ;
 
  EffPlots* h_eff_b ;
  EffPlots* h_eff_c ;
  EffPlots* h_eff_l ;
  EffPlots* h_eff_bdj ;
  EffPlots* h_eff_cdj ;
  EffPlots* h_eff_ldj ;
 
  //Unfolding Plots
  UnfoldingPlots* h_zee_unfolding;
  UnfoldingPlots* h_zmm_unfolding;
  

  TH1D* h_evt ;
  TH1D* h_dR_je ;
  TH1D* h_dR_jm ;
  TH1D* h_nMuon ;
  TH1D* h_nEle ;
  TH1D* h_nJet ;
  TH1D* h_ele_cutflow ;
  TH1D* h_muon_cutflow ;
  TH1D* h_Jet_cutflow ;
  TH1D* h_zee_cutflow ;
  TH1D* h_zmm_cutflow ;
  TH1D* h_pt_jet_after_ele_rem ;
 
  TH1D* h_Zee_ZmassFull ;
  TH1D* h_Zmm_ZmassFull ;

  //check trigger matching
  TH1D* h_dR1_muonTrig ;
  TH1D* h_dR2_muonTrig ;
  TH1D* h_pt1_muonTrig ;
  TH1D* h_pt2_muonTrig ;
  
  TH1D* h_dR1_eleTrig ;
  TH1D* h_dR2_eleTrig ;
  TH1D* h_pt1_eleTrig ;
  TH1D* h_pt2_eleTrig ;

} ;

#endif
