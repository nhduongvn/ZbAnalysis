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
  ZbSelection(bool isData=false) ;
  virtual ~ZbSelection() ;
    // These methods are called at the corresponding stage of processing of TSelector
  virtual void SlaveBegin(Reader* r) ;
  virtual void Process(Reader* r) ;
  virtual void SlaveTerminate(Reader* r) {} ;
  virtual void Terminate(TList* mergedList, std::string outFileName) ;
private:
  //histograms
  ZbPlots* h_zee_jet ;
  ZbPlots* h_zmm_jet ;

  TH1D* h_dR_je ;
  TH1D* h_dR_jm ;
  
  
  TH1D* h_nJet ;
  TH1D* h_ele_cutflow ;
  TH1D* h_muon_cutflow ;
  TH1D* h_Jet_cutflow ;
  TH1D* h_zee_cutflow ;
  TH1D* h_zmm_cutflow ;
  
  TH1D* h_nEle_nocut ;
  TH1D* h_ele_pT_nocut ;
  TH1D* h_ele_eta_nocut ;
  TH1D* h_ele_phi_nocut ;

  TH1D* h_nEle_loose_MVA ;
  TH1D* h_ele_pT_loose_MVA ;
  TH1D* h_ele_eta_loose_MVA ;
  TH1D* h_ele_phi_loose_MVA ;

  TH1D* h_nEle_kin_cut ;
  TH1D* h_ele_pT_kin_cut ;
  TH1D* h_ele_eta_kin_cut ;
  TH1D* h_ele_phi_kin_cut ;

  TH1D* h_nmuon_nocut ;
  TH1D* h_muon_pT_nocut ;
  TH1D* h_muon_eta_nocut ;
  TH1D* h_muon_phi_nocut ;

  TH1D* h_nmuon_loose_id ;
  TH1D* h_muon_pT_loose_id ;
  TH1D* h_muon_eta_loose_id ;
  TH1D* h_muon_phi_loose_id ;

  TH1D* h_nmuon_kin_cut ;
  TH1D* h_muon_pT_kin_cut ;
  TH1D* h_muon_eta_kin_cut ;
  TH1D* h_muon_phi_kin_cut ;

  TH1D* h_njet_nocut ;
  TH1D* h_jet_pT_nocut ;
  TH1D* h_jet_eta_nocut ;
  TH1D* h_jet_phi_nocut ;

  TH1D* h_njet_ele_rem ;
  TH1D* h_jet_pT_ele_rem ;
  TH1D* h_jet_eta_ele_rem ;
  TH1D* h_jet_phi_ele_rem ;

  TH1D* h_njet_muon_rem ;
  TH1D* h_jet_pT_muon_rem ;
  TH1D* h_jet_eta_muon_rem ;
  TH1D* h_jet_phi_muon_rem ;

  TH1D* h_njet_jetID ;
  TH1D* h_jet_pT_jetID ;
  TH1D* h_jet_eta_jetID ;
  TH1D* h_jet_phi_jetID ;

  TH1D* h_njet_kin_cut ;
  TH1D* h_jet_pT_kin_cut ;
  TH1D* h_jet_eta_kin_cut ;
  TH1D* h_jet_phi_kin_cut ;

 


} ;

#endif
