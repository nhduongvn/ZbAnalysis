#define ZbSelection_cxx

#include <math.h>
#include <iostream>

#include "TList.h"
#include "TParameter.h"
#include "TLorentzVector.h"

#include "ZbSelection.h"
#include "Global.h"
#include "Obj.cxx"

#include "TH1.h"

//#include "BTagCalibrationStandalone.h"
//#include "BTagCalibrationStandalone.cpp"

using namespace std;
float PI = M_PI;

//ZbSelection::ZbSelection(bool isData, std::string year) : Selector(isData, year), h_zee_jet(0), h_zmm_jet(0) {}

ZbSelection::~ZbSelection() {
  //if (h_zee_jet) delete h_zee_jet;
  //if (h_zmm_jet) delete h_zmm_jet;
} 

void ZbSelection::SlaveBegin(Reader* r) {
  h_evt = new TH1D("Nevt","",3,-1.5,1.5) ;
  h_zee_jet = new ZbPlots("Zee_jet") ;
  h_zee_bjet = new ZbPlots("Zee_bjet") ;
  h_zee_bjet_afterMET = new ZbPlots("Zee_bjet_afterMET") ;
  h_zee_2bjet = new Z2bPlots("Zee_2bjet") ;

  h_zee_2bjet_bb = new Z2bPlots("Zee_2bjet_bb") ; //two tagged bjets are bb
  h_zee_2bjet_bX = new Z2bPlots("Zee_2bjet_bX") ; //two tagged bjets are bb
  h_zee_2bjet_XX = new Z2bPlots("Zee_2bjet_XX") ; //two tagged bjets are bb
  h_zee_bjet_deepJet = new ZbPlots("Zee_bjetDeepJet") ;
  h_zee_2bjet_deepJet = new Z2bPlots("Zee_2bjetDeepJet") ;
  
  h_zmm_jet = new ZbPlots("Zmm_jet") ;
  h_zmm_bjet = new ZbPlots("Zmm_bjet") ;
  h_zmm_bjet_afterMET = new ZbPlots("Zmm_bjet_afterMET") ;
  h_zmm_2bjet = new Z2bPlots("Zmm_2bjet") ;
  h_zmm_2bjet_bb = new Z2bPlots("Zmm_2bjet_bb") ; //two tagged bjets are bb
  h_zmm_2bjet_bX = new Z2bPlots("Zmm_2bjet_bX") ; //two tagged bjets are bb
  h_zmm_2bjet_XX = new Z2bPlots("Zmm_2bjet_XX") ; //two tagged bjets are bb
  h_zmm_bjet_deepJet = new ZbPlots("Zmm_bjetDeepJet") ;
  h_zmm_2bjet_deepJet = new Z2bPlots("Zmm_2bjetDeepJet") ;
  
  unsigned nBins = 9 ;
  float bins[10] = {20, 30, 50, 70, 100, 140, 200, 300, 600, 1000} ;
  h_eff_b = new EffPlots("b", nBins, bins) ;
  h_eff_c = new EffPlots("c", nBins, bins) ;
  h_eff_l = new EffPlots("l", nBins, bins) ;
  h_eff_bdj = new EffPlots("bdj", nBins, bins) ;
  h_eff_cdj = new EffPlots("cdj", nBins, bins) ;
  h_eff_ldj = new EffPlots("ldj", nBins, bins) ;

  //Unfolding plots
  h_zee_unfolding = new UnfoldingPlots("zee");
  h_zmm_unfolding = new UnfoldingPlots("zmm");
  
  h_zee_afterMET_unfolding = new UnfoldingPlots("zee_afterMET");
  h_zmm_afterMET_unfolding = new UnfoldingPlots("zmm_afterMET");

  h_dR_je = new TH1D("h_dR_je","",500,0,5) ;
  h_dR_jm = new TH1D("h_dR_jm","",500,0,5) ;

  h_Zee_ZmassFull = new TH1D("Zee_ZmassFull","",300,0,300) ;
  h_Zmm_ZmassFull = new TH1D("Zmm_ZmassFull","",300,0,300) ;

  h_nMuon = new TH1D("h_nMuon","",10,-0.5,9.5) ;
  h_nEle = new TH1D("h_nEle","",10,-0.5,9.5) ;
  h_nJet = new TH1D("h_nJet","",10,-0.5,9.5) ;

  h_ele_cutflow = new TH1D("ele_cutflow","",10,-0.5,9.5) ;
  h_muon_cutflow = new TH1D("muon_cutflow","",10,-0.5,9.5) ;
  h_Jet_cutflow = new TH1D("Jet_cutflow","",10,-0.5,9.5) ;
  h_zee_cutflow = new TH1D("zee_cutflow","",10,-0.5,9.5) ;
  h_zmm_cutflow = new TH1D("zmm_cutflow","",10,-0.5,9.5) ;

  h_pt_jet_after_ele_rem = new TH1D("jet_pt_after_ele_rem","",200,0.0,200) ;
  h_dR1_muonTrig = new TH1D("dR1_muonTrig","",100,0,10) ;
  h_dR2_muonTrig = new TH1D("dR2_muonTrig","",100,0,10) ;
  h_pt1_muonTrig = new TH1D("pt1_muonTrig","",200,0,200) ;
  h_pt2_muonTrig = new TH1D("pt2_muonTrig","",200,0,200) ;
  
  h_dR1_eleTrig = new TH1D("dR1_eleTrig","",100,0,10) ;
  h_dR2_eleTrig = new TH1D("dR2_eleTrig","",100,0,10) ;
  h_pt1_eleTrig = new TH1D("pt1_eleTrig","",200,0,200) ;
  h_pt2_eleTrig = new TH1D("pt2_eleTrig","",200,0,200) ;

  //Sumw2
  h_Zee_ZmassFull->Sumw2() ;
  h_Zmm_ZmassFull->Sumw2() ;

  //Add histograms to fOutput so they can be saved in Processor::SlaveTerminate
  r->GetOutputList()->Add(h_evt) ;

  std::vector<TH1*> tmp = h_zee_jet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);

  tmp = h_zmm_jet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_zee_bjet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_zee_bjet_afterMET->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_zmm_bjet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_zmm_bjet_afterMET->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);

  tmp = h_zee_2bjet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_zee_2bjet_bb->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_zee_2bjet_bX->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);

  tmp = h_zee_2bjet_XX->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_zmm_2bjet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);

  tmp = h_zmm_2bjet_bb->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_zmm_2bjet_bX->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);

  tmp = h_zmm_2bjet_XX->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_zee_bjet_deepJet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_zmm_bjet_deepJet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);

  tmp = h_zee_2bjet_deepJet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_zmm_2bjet_deepJet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  
  tmp = h_eff_b->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  tmp = h_eff_c->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  tmp = h_eff_l->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  tmp = h_eff_bdj->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  tmp = h_eff_cdj->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  tmp = h_eff_ldj->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);

  //Unfolding plots 
  tmp = h_zee_unfolding->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  tmp = h_zmm_unfolding->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  tmp = h_zee_afterMET_unfolding->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);
  tmp = h_zmm_afterMET_unfolding->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);


  r->GetOutputList()->Add(h_dR_je) ;
  r->GetOutputList()->Add(h_dR_jm) ;
  //r->GetOutputList()->Add(h_nMuon) ;
  //r->GetOutputList()->Add(h_nEle) ;
  //r->GetOutputList()->Add(h_nJet) ;
  r->GetOutputList()->Add(h_Zee_ZmassFull) ;
  r->GetOutputList()->Add(h_Zmm_ZmassFull) ;
  r->GetOutputList()->Add(h_ele_cutflow) ;
  r->GetOutputList()->Add(h_muon_cutflow) ;
  r->GetOutputList()->Add(h_Jet_cutflow) ;
  r->GetOutputList()->Add(h_zee_cutflow) ;
  r->GetOutputList()->Add(h_zmm_cutflow) ;
  r->GetOutputList()->Add(h_pt_jet_after_ele_rem) ;
  r->GetOutputList()->Add(h_dR1_muonTrig) ;
  r->GetOutputList()->Add(h_dR2_muonTrig) ;
  r->GetOutputList()->Add(h_pt1_muonTrig) ;
  r->GetOutputList()->Add(h_pt2_muonTrig) ;
  r->GetOutputList()->Add(h_dR1_eleTrig) ;
  r->GetOutputList()->Add(h_dR2_eleTrig) ;
  r->GetOutputList()->Add(h_pt1_eleTrig) ;
  r->GetOutputList()->Add(h_pt2_eleTrig) ;

  
  const Int_t nx = 4, nx1 = 4, yx= 6, nzee=8, nzmm=8;
  const char *ele_cut[nx] = {"all","ip","kine","ID"};
  const char *muon_cut[nx1] = {"all","kine","medium muon ID", "iso"};
  const char *jet_cut[yx] = {"all", "kine", "ele overlap removal","muon overlap removal","loose jet ID", "loose jet puID"};
  const char *zee_cut[nzee] = {"all event (not cut)","trigger","pass electron  cuts","pass Z mass cuts`", "pass jet cuts", "pass b-jet", "pass 2 b-jets", "Met"};
  const char *zmm_cut[nzmm] = {"all event (not cut)","trigger","pass muon  cuts", "pass Z mass cuts", "pass jet cuts", "pass b-jet", "pass 2 b-jets", "Met"};
  for (int i=1;i<=nx;i++) h_ele_cutflow->GetXaxis()->SetBinLabel(i+1.5,ele_cut[i-1]);
  for (int i=1;i<=nx1;i++) h_muon_cutflow->GetXaxis()->SetBinLabel(i+1.5,muon_cut[i-1]);
  for (int i=1;i<=yx;i++) h_Jet_cutflow->GetXaxis()->SetBinLabel(i+1.5,jet_cut[i-1]);
  for (int i=1;i<=nzee;i++) h_zee_cutflow->GetXaxis()->SetBinLabel(i+1.5,zee_cut[i-1]);
  for (int i=1;i<=nzmm;i++) h_zmm_cutflow->GetXaxis()->SetBinLabel(i+1.5,zmm_cut[i-1]);

}

void ZbSelection::Process(Reader* r) {
  //Weights
  float genWeight = 1.;
  float puSF = 1.;
  float l1preW = 1.;
#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
  if (*(r->genWeight) < 0) genWeight = -1. ;
  if (*(r->genWeight) == 0) h_evt->Fill(0) ; 
  if (*(r->genWeight) < 0) h_evt->Fill(-1) ; 
  if (*(r->genWeight) > 0) h_evt->Fill(1) ;
  puSF = PileupSF(*(r->Pileup_nTrueInt));
#endif

#if defined(MC_2016) || defined(MC_2017)
  l1preW = *(r->L1PreFiringWeight_Nom);  
  //std::cout << "\nPrefiring: " << l1preW;
#endif

#if defined(DATA_2016) || defined(DATA_2017) || defined(DATA_2018)
  h_evt->Fill(-1) ;
  if (!m_lumiFilter.Pass(*(r->run),*(r->luminosityBlock))) return;
  h_evt->Fill(1) ;
#endif

  float evtW = 1. ;
  
  if (!m_isData) evtW *= genWeight*puSF*l1preW;

  //Gen events
  //Z+2bjets
  //
#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
  //2 gen lepton pt > 35 and 25 eta < 2.4, mass 71,111, two bjets with pt > 30 and fabs(eta) < 2.4
  std::vector<TLorentzVector> gen_eles;
  std::vector<TLorentzVector> gen_muons;
  std::vector<TLorentzVector> zee_gen_uf;//0: gen leading electron, 1: subleading electron, 2: leading gen bjet, 3: subleading gen bjet
  std::vector<TLorentzVector> zmm_gen_uf;
  if (*(r->nGenDressedLepton)>=2) {
    TLorentzVector lz1;
    lz1.SetPtEtaPhiM((r->GenDressedLepton_pt)[0],(r->GenDressedLepton_eta)[0],(r->GenDressedLepton_phi)[0],(r->GenDressedLepton_mass)[0]) ;
    TLorentzVector lz2;
    lz2.SetPtEtaPhiM((r->GenDressedLepton_pt)[1],(r->GenDressedLepton_eta)[1],(r->GenDressedLepton_phi)[1],(r->GenDressedLepton_mass)[1]) ;
    TLorentzVector lz3 = lz1 + lz2;
    float mll = lz3.M();
    if ((r->GenDressedLepton_pt)[0] > CUTS.Get<float>("lep_pt0") && (r->GenDressedLepton_pt)[1] > CUTS.Get<float>("lep_pt1") &&
        fabs((r->GenDressedLepton_eta)[0]) < CUTS.Get<float>("lep_eta")  && fabs((r->GenDressedLepton_eta)[1]) < CUTS.Get<float>("lep_eta")  &&
        mll > CUTS.Get<float>("ZMassL")  && mll < CUTS.Get<float>("ZMassH") ) {
      if (abs((r->GenDressedLepton_pdgId)[0]) == 11 && abs((r->GenDressedLepton_pdgId)[1]) == 11) {
        for (unsigned iL = 0 ; iL < 2 ; ++iL) {
          TLorentzVector tmp ;
          tmp.SetPtEtaPhiM((r->GenDressedLepton_pt)[iL],(r->GenDressedLepton_eta)[iL],(r->GenDressedLepton_phi)[iL],(r->GenDressedLepton_mass)[iL]) ;
          gen_eles.push_back(tmp);
        }
      } //Zee
      if (abs((r->GenDressedLepton_pdgId)[0]) == 13 && abs((r->GenDressedLepton_pdgId)[1]) == 13) {
        for (unsigned iL = 0 ; iL < 2 ; ++iL) {
          TLorentzVector tmp ;
          tmp.SetPtEtaPhiM((r->GenDressedLepton_pt)[iL],(r->GenDressedLepton_eta)[iL],(r->GenDressedLepton_phi)[iL],(r->GenDressedLepton_mass)[iL]) ;
          gen_muons.push_back(tmp);
        }
      } //Zmm
      //get gen bjets
      std::vector<TLorentzVector> gen_bs ;
      for (unsigned iJ = 0 ; iJ < *(r->nGenJet) ; ++iJ) {
        if ((r->GenJet_pt)[iJ] > CUTS.Get<float>("jet_pt")  && fabs((r->GenJet_eta)[iJ]) < CUTS.Get<float>("jet_eta")  && (r->GenJet_hadronFlavour)[iJ] == 5) {
          TLorentzVector tmp ;
          tmp.SetPtEtaPhiM((r->GenJet_pt)[iJ],(r->GenJet_eta)[iJ],(r->GenJet_phi)[iJ],(r->GenJet_mass)[iJ]) ;
          gen_bs.push_back(tmp);
        }
      }
      //Zee + 2 bjet
      if (gen_eles.size()>=2 && gen_bs.size()>=2) {
        zee_gen_uf.push_back(gen_eles[0]);
        zee_gen_uf.push_back(gen_eles[1]);
        zee_gen_uf.push_back(gen_bs[0]);
        zee_gen_uf.push_back(gen_bs[1]);
      }
      //Zmm + 2 bjet
      if (gen_muons.size()>=2 && gen_bs.size()>=2) {
        zmm_gen_uf.push_back(gen_muons[0]);
        zmm_gen_uf.push_back(gen_muons[1]);
        zmm_gen_uf.push_back(gen_bs[0]);
        zmm_gen_uf.push_back(gen_bs[1]);
      }
    }
  }
#endif
  
  //=============Get objects============= 
  //electrons
  std::vector<LepObj> eles ;
  std::vector<LepObj> eles_jetOverlap ;
  for (unsigned int i = 0 ; i < *(r->nElectron) ; ++i) {
    
    h_ele_cutflow->Fill(1) ;
    
    float etaSC = (r->Electron_eta)[i]-(r->Electron_deltaEtaSC[i]) ;
    LepObj ele((r->Electron_pt)[i],(r->Electron_eta)[i],etaSC,(r->Electron_phi)[i],(r->Electron_mass)[i],i,(r->Electron_charge)[i],0) ;

    float dz = (r->Electron_dz)[i]; 
    float dxy = (r->Electron_dxy)[i]; //dxy=d0
    if ((fabs(etaSC) < 1.4442) && (fabs(dz) > CUTS.Get<float>("ele_dz_b") || fabs(dxy) > CUTS.Get<float>("ele_d0_b"))) continue;
    if ((fabs(etaSC) >= 1.4442) && (fabs(dz) > CUTS.Get<float>("ele_dz_e") || fabs(dxy) > CUTS.Get<float>("ele_d0_e"))) continue;
    h_ele_cutflow->Fill(2) ;

    int eleId = r->Electron_cutBased[i] ; //cut-based ID Fall17 V2 (0:fail, 1:veto, 2:loose, 3:medium, 4:tight)
    //electron for electron jet overlap removal
    if (ele.m_lvec.Pt() > CUTS.Get<float>("lep_jetOverlap_pt") && fabs(ele.m_lvec.Eta()) < CUTS.Get<float>("lep_jetOverlap_eta")) {
      if (eleId >= 4) { //tightId for jet removal
        eles_jetOverlap.push_back(ele) ;
      }
    }

    if (ele.m_lvec.Pt() < CUTS.Get<float>("lep_pt1") || fabs(ele.m_lvec.Eta()) > CUTS.Get<float>("lep_eta")) continue ; //use lower pt threshold to select both leading and subleading lepton
    if (fabs(ele.m_scEta) < 1.566 && fabs(ele.m_scEta) > 1.442) continue ; //EB-EE gap removal
    h_ele_cutflow->Fill(3)  ;

    if (eleId < 4) continue; //use tightId to be consistent with single electron trigger
    h_ele_cutflow->Fill(4)  ;

	  eles.push_back(ele) ;
    //h_nEle->Fill(eles.size()) ;
  }
  
  //muons
  std::vector<LepObj> muons ;
  std::vector<LepObj> muons_jetOverlap ;
  for (unsigned int i = 0 ; i < *(r->nMuon) ; ++i) {

    h_muon_cutflow->Fill(1) ;
    
    LepObj muon((r->Muon_pt)[i],(r->Muon_eta)[i],-1,(r->Muon_phi)[i],(r->Muon_mass)[i],i,(r->Muon_charge)[i],(r->Muon_pfRelIso04_all)[i]) ;
    float sf_rc = MuonRcSF(r, muon, 13) ;
    //TEMP
    //sf_rc = 1.0;
    //std::cout << "\n Before correction: " <<  sf_rc << " " << muon.m_lvec.Pt() ;
    //muon.m_lvec.Print() ;
    muon.m_lvec.SetPtEtaPhiM(muon.m_lvec.Pt()*sf_rc,muon.m_lvec.Eta(),muon.m_lvec.Phi(),muon.m_lvec.M()) ;
    //std::cout << "\n After " << muon.m_lvec.Pt() ;
    //muon.m_lvec.Print() ;
    
    //muon for muon jet overlap removal
    if (muon.m_lvec.Pt() > CUTS.Get<float>("lep_jetOverlap_pt") && fabs(muon.m_lvec.Eta()) < CUTS.Get<float>("lep_jetOverlap_eta")) {
      if (r-> Muon_mediumId[i] > 0 && r->Muon_pfRelIso04_all[i] < CUTS.Get<float>("muon_iso")) {
        muons_jetOverlap.push_back(muon) ;
      }
    }
    
    if (muon.m_lvec.Pt() < CUTS.Get<float>("lep_pt1") || fabs(muon.m_lvec.Eta()) > CUTS.Get<float>("lep_eta")) continue ; 
    h_muon_cutflow->Fill(2) ;
    
    if (r-> Muon_mediumId[i] <= 0) continue;
    h_muon_cutflow->Fill(3) ;
	  
    if (muon.m_iso > CUTS.Get<float>("muon_iso")) continue ; 
    h_muon_cutflow->Fill(4) ;

    muons.push_back(muon) ;
    //h_nMuon->Fill(muons.size()) ;
  }
  
  //sv
  std::vector<TLorentzVector> lvec_SVs ;
  for (unsigned i = 0 ; i < *(r->nSV) ; ++i) {
    TLorentzVector tmp ;
    tmp.SetPtEtaPhiM((r->SV_pt)[i], (r->SV_eta)[i], (r->SV_phi)[i], (r->SV_mass)[i]) ;
    lvec_SVs.push_back(tmp);
  }
  
  /////////////////////////////////
  //jets
  /////////////////////////////////
  std::vector<JetObj> jets ;
  std::vector<JetObj> jets_ele_removal ;
  std::vector<JetObj> bjets ;
  std::vector<JetObj> bjets_deepJet ;
  
  float btagM_cut = CUTS.Get<float>("jet_deepCSVM_"+m_year) ;
  float btagT_cut = CUTS.Get<float>("jet_deepCSVT_"+m_year) ;
  float btagDeepJetM_cut = CUTS.Get<float>("jet_deepJetM_"+m_year) ;

  for (unsigned int i = 0 ; i < *(r->nJet) ; ++i) {
    
    h_Jet_cutflow->Fill(1) ;
    
    int jetFlav = -1 ;
#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
    jetFlav = (r->Jet_hadronFlavour)[i];
#endif 
    
    float jetPt = (r->Jet_pt)[i];

#if defined(JETMETSYST)
    if (m_jetmetSystType == "jetnom" || m_jetmetSystType == "jetmetnom") jetPt = (r->Jet_pt_nom)[i];
    if (m_jetmetSystType == "jesu") jetPt = (r->Jet_pt_jesTotalUp)[i];
    if (m_jetmetSystType == "jesd") jetPt = (r->Jet_pt_jesTotalDown)[i];
    if (m_jetmetSystType == "jeru") jetPt = (r->Jet_pt_jerUp)[i];
    if (m_jetmetSystType == "jerd") jetPt = (r->Jet_pt_jerDown)[i];
#endif

    //std::cout << "\n Jet pt: " << m_jetmetSystType << " " << jetPt;

    JetObj jet(jetPt,(r->Jet_eta)[i],(r->Jet_phi)[i],(r->Jet_mass)[i],jetFlav,(r->Jet_btagDeepB)[i],(r->Jet_btagDeepFlavB)[i]) ;
    jet.SetSV(lvec_SVs);

    if (jet.m_lvec.Pt() < CUTS.Get<float>("jet_pt") || fabs(jet.m_lvec.Eta()) > CUTS.Get<float>("jet_eta")) continue ;
    h_Jet_cutflow->Fill(2) ;
    
    if (jet.IsLepton(eles_jetOverlap)) continue ;
    jets_ele_removal.push_back(jet) ;
    h_Jet_cutflow->Fill(3) ;
      
    if (jet.IsLepton(muons_jetOverlap)) continue ;
    h_Jet_cutflow->Fill(4) ;
    
    if (r->Jet_jetId[i] < 2 ) continue ;
    //FIXME test jet id
    //if (r->Jet_jetId[i] <= 0 ) continue ;
    h_Jet_cutflow->Fill(5) ;
    
    //FIXME: turn this on for v7
    if (jet.m_lvec.Pt()<50 && jet.m_lvec.Pt()>30 && (r->Jet_puIdDisc)[i]<0.61 ) continue;
    h_Jet_cutflow->Fill(6) ;
	  
    jets.push_back(jet) ;

    if (jet.m_deepCSV >= btagT_cut) bjets.push_back(jet) ;
    if (jet.m_deepJet >= btagDeepJetM_cut) bjets_deepJet.push_back(jet) ;

  }

  //calculate SF for eletron, muon and btagging
  float eleSF_w(1.);
  float muonSF_w(1.);
  float btag_w(1.);
  if (!m_isData) {
    //note this is SF for medium DeepCSV
    btag_w = CalBtagWeight(jets,CUTS.GetStr("jet_main_btagWP"),m_btagUncType) ;
    //std::cout << "\n Btag: " << btag_w;
    if (eles.size()>=2) eleSF_w = CalEleSF(eles[0],eles[1]) ; //unc already set and calculated in Selector.cxx
    if (muons.size()>=2) {
      muonSF_w = CalMuonSF_id_iso(muons[0],muons[1]);
      //std::cout << "\n Muon SF, " << m_muonUncType << " " << muonSF_w;
    }
  }

  //if (bjets.size()>=1) std::cout << "\n btagWeight: " << btagWeight ;

  //trigger stuffs  
  bool eleTrig(false) ;
  bool muonTrig(false) ;
  float ptThr_ele(0) ;
  float ptThr_muon(0) ;
//Get trigger objects
  TLorentzVector muon_trigObj ;
  TLorentzVector ele_trigObj ;
  unsigned ele_bits ;
  unsigned muon_bits ;
#if defined(MC_2016) || defined(DATA_2016) 
  if (*(r->HLT_Ele27_WPTight_Gsf)) eleTrig = true ;
  if (*(r->HLT_IsoMu24) || *(r->HLT_IsoTkMu24)) muonTrig = true ;
  //sel.qualityBitsDoc = cms.string("1 = TrkIsoVVL, 2 = Iso, 4 = OverlapFilter PFTau, 8 = IsoTkMu, 1024 = 1mu (Mu50)")
  //https://github.com/cms-nanoAOD/cmssw/blob/master-102X/PhysicsTools/NanoAOD/python/triggerObjects_cff.py
  muon_bits = (2+8) ; //iso + IsoTkMu
  //qualityBitsDoc = cms.string("1 = CaloIdL_TrackIdL_IsoVL, 2 = 1e (WPTight), 4 = 1e (WPLoose), 8 = OverlapFilter PFTau, 16 = 2e, 32 = 1e-1mu, 64 = 1e-1tau, 128 = 3e, 256 = 2e-1mu, 512 = 1e-2mu, 1024 = 1e (32_L1DoubleEG_AND_L1SingleEGOr), 2048 = 1e (CaloIdVT_GsfTrkIdT), 4096 = 1e (PFJet), 8192 = 1e (Photon175_OR_Photon200)"),),
  ele_bits = (2) ;
  ptThr_ele = 27 ;
  ptThr_muon = 24 ;
#endif

#if defined(MC_2017) || defined(DATA_2017) 
  //https://indico.cern.ch/event/787358/contributions/3504805/attachments/1883444/3112792/trigger_sf_vgg_HLTEle32_2017_290719.pdf
  if (*(r->HLT_Ele32_WPTight_Gsf_L1DoubleEG)) eleTrig = true ;
  if (*(r->HLT_IsoMu27)) muonTrig = true ;
  //qualityBitsDoc = cms.string("1 = TrkIsoVVL, 2 = Iso, 4 = OverlapFilter PFTau, 8 = 1mu, 16 = 2mu, 32 = 1mu-1e, 64 = 1mu-1tau, 128 = 3mu, 256 = 2mu-1e, 512 = 1mu-2e, 1024 = 1mu (Mu50), 2048 = 1mu (Mu100)"),),
  ele_bits = (2) ;
  muon_bits = (2) ;
  ptThr_ele = 32 ;
  ptThr_muon = 27 ;
#endif

#if defined(MC_2018) || defined(DATA_2018) 
  if (*(r->HLT_Ele32_WPTight_Gsf)) eleTrig = true ;
  if (*(r->HLT_IsoMu24)) muonTrig = true ;
  ele_bits = (2) ;
  muon_bits = (2) ;
  ptThr_ele = 32 ;
  ptThr_muon = 24 ;
#endif
  
  TLorentzVector trigObj_ele = GetTrigObj(r, 11, ele_bits, ptThr_ele) ;
  TLorentzVector trigObj_muon = GetTrigObj(r, 13, muon_bits, ptThr_muon) ;

  float trigSF_ele(1.0) ;
  if (!m_isData && (eles.size() >= 2)) {
    trigSF_ele =  CalTrigSF(11,eles[0], eles[1], trigObj_ele, h_dR1_eleTrig, h_dR2_eleTrig, h_pt1_eleTrig, h_pt2_eleTrig) ;
  }
  
  float trigSF_muon(1) ;
  if (!m_isData && (muons.size() >= 2)) trigSF_muon =  CalTrigSF(13,muons[0], muons[1], trigObj_muon, h_dR1_muonTrig, h_dR2_muonTrig, h_pt1_muonTrig, h_pt2_muonTrig) ; //uncertainties already calculate inside CalTrigSF

  //TEMP
  //eleTrig = true ;
  ////////////////////////////////////
  //Zee + jets
  ////////////////////////////////////
  h_zee_cutflow->Fill(1); //all events (not cut)
  std::vector<TLorentzVector> zee_rec_uf; //0: leading lepton, 1: subleading lepton, 2: bjet 1, 3: bjet 2
  std::vector<TLorentzVector> zee_rec_afterMET_uf; //0: leading lepton, 1: subleading lepton, 2: bjet 1, 3: bjet 2
  float w_zee_rec_uf(1);
  float w_zee_rec_afterMET_uf(1);
  if (eleTrig) {
    h_zee_cutflow->Fill(2); //trigger
    if (eles.size() >= 2 && eles[0].m_lvec.Pt() >= CUTS.Get<float>("lep_pt0")) {

	    h_zee_cutflow->Fill(3) ; //pass electron  cuts
      
      float zee_w = evtW*eleSF_w*trigSF_ele ;
      //TEMP
      //std::cout << "\n Ele weights: " << "  " << evtW << "  " << eleSF_w << "  " << trigSF_ele << "  " << zee_w; 
      float zeeb_w = zee_w*btag_w;
      
      ZObj Z(eles[0],eles[1]) ;

      h_Zee_ZmassFull->Fill(Z.m_lvec.M(), zee_w) ;

      if (Z.m_lvec.M() >= CUTS.Get<float>("ZMassL") && Z.m_lvec.M() <= CUTS.Get<float>("ZMassH")) { 
      
        h_zee_cutflow->Fill(4) ; //pass Z mass cuts

        h_zee_jet->FillNjet(jets.size(), zee_w) ;
        
        //at least on jet
        if (jets.size() >= 1) {

          h_zee_cutflow->Fill(5) ; //pass jet cuts

          h_zee_jet->Fill(Z, jets[0], zee_w) ;
          h_zee_jet->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zee_w);
        
          // isolation
          float deltaRelelep0 = Z.m_lep0.m_lvec.DeltaR(jets[0].m_lvec);
          float deltaRelelep1 = Z.m_lep1.m_lvec.DeltaR(jets[0].m_lvec); 
          h_dR_je->Fill(std::min(deltaRelelep0,deltaRelelep1), zee_w) ;

          //tagging efficiency
          if (CUTS.GetStr("jet_main_btagWP")=="deepCSVT" || CUTS.GetStr("jet_main_btagWP")=="deepJetT")
            Fill_btagEffi(jets,"T",zee_w) ; //Eff for tight WP
          if (CUTS.GetStr("jet_main_btagWP")=="deepCSVM" || CUTS.GetStr("jet_main_btagWP")=="deepJetM")
            Fill_btagEffi(jets,"M",zee_w) ; //Eff for tight WP

        } //end at least one jet
        
        //at least one b-tagged jets
        h_zee_bjet->FillNjet(bjets.size(),evtW) ; 
        if (*(r->MET_pt) < CUTS.Get<float>("MET")) h_zee_bjet_afterMET->FillNjet(bjets.size(),evtW);

        if (bjets.size() >=1) {
           
          h_zee_cutflow->Fill(6) ;

          h_zee_bjet->Fill(Z, bjets[0], zeeb_w) ;
          h_zee_bjet->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zeeb_w);
          if (*(r->MET_pt) < CUTS.Get<float>("MET")) {
            h_zee_bjet_afterMET->Fill(Z, bjets[0], zeeb_w) ;
            h_zee_bjet_afterMET->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zeeb_w);
          }
        }

        //at least two b-tagged jets
        if (bjets.size() >=2) {
          
          h_zee_cutflow->Fill(7) ;

#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
          //rec objects for unfolding before METCUT
          if (bjets[0].m_flav == 5 && bjets[1].m_flav == 5) {
            w_zee_rec_uf = zeeb_w;
            zee_rec_uf.push_back(eles[0].m_lvec);
            zee_rec_uf.push_back(eles[1].m_lvec);
            zee_rec_uf.push_back(bjets[0].m_lvec);
            zee_rec_uf.push_back(bjets[1].m_lvec);
          }
#endif          
          if (*(r->MET_pt) < CUTS.Get<float>("MET")) {
            
            h_zee_cutflow->Fill(8) ;

            h_zee_2bjet->Fill(Z, bjets[0], bjets[1], zeeb_w) ;
            h_zee_2bjet->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zeeb_w);

#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
            if (bjets[0].m_flav == 5 && bjets[1].m_flav == 5) {
              h_zee_2bjet_bb->Fill(Z, bjets[0], bjets[1], zeeb_w) ;
              h_zee_2bjet_bb->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zeeb_w);
              w_zee_rec_afterMET_uf = zeeb_w;
              zee_rec_afterMET_uf.push_back(eles[0].m_lvec);
              zee_rec_afterMET_uf.push_back(eles[1].m_lvec);
              zee_rec_afterMET_uf.push_back(bjets[0].m_lvec);
              zee_rec_afterMET_uf.push_back(bjets[1].m_lvec);
            }
            else if ((bjets[0].m_flav == 5 && bjets[1].m_flav != 5) || (bjets[0].m_flav != 5 && bjets[1].m_flav == 5)) {
              h_zee_2bjet_bX->Fill(Z, bjets[0], bjets[1], zeeb_w) ;
              h_zee_2bjet_bX->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zeeb_w);
            }
            else {
              h_zee_2bjet_XX->Fill(Z, bjets[0], bjets[1], zeeb_w) ;
              h_zee_2bjet_XX->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zeeb_w);
            }
#endif
          } //end MET cut
        } //end two b-tagged deepCSV jets

        //at least one b-tagged jets using deepJet
        h_zee_bjet_deepJet->FillNjet(bjets_deepJet.size(),evtW) ; //FIXME 
        if (bjets_deepJet.size() >=1) {
          
          h_zee_cutflow->Fill(8) ;

          h_zee_bjet_deepJet->Fill(Z, bjets_deepJet[0], evtW) ; //FIXME
          h_zee_bjet_deepJet->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), evtW); //FIXME
        } //end one b-tagged jets using deepJet

        //at least two b-tagged jets using deepJet
        if (bjets_deepJet.size() >=2) {
          
          h_zee_cutflow->Fill(9) ;

          h_zee_2bjet_deepJet->Fill(Z, bjets_deepJet[0], bjets_deepJet[1], evtW) ; //FIXME
          h_zee_2bjet_deepJet->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), evtW); //FIXME
        } //end at least two b-tagged jets using deepJet

      }//end Z mass
    } //end two electrons
  } //end ele. trigger
  
  if (jets_ele_removal.size() > 0) {
    JetPlot J_cut1(jets_ele_removal[0]);
    h_pt_jet_after_ele_rem->Fill(J_cut1.m_lvec.Pt(), evtW);
  }
  
  

  ///////////////////////////
  //Zmm + jets
  ///////////////////////////
  h_zmm_cutflow->Fill(1); //all events (not cut)
  
  std::vector<TLorentzVector> zmm_rec_uf; //0: leading lepton, 1: subleading lepton, 2: bjet 1, 3: bjet 2
  float w_zmm_rec_uf(1);
  std::vector<TLorentzVector> zmm_rec_afterMET_uf; //0: leading lepton, 1: subleading lepton, 2: bjet 1, 3: bjet 2
  float w_zmm_rec_afterMET_uf(1);
  //trigger cuts
  if (muonTrig) {
    h_zmm_cutflow->Fill(2); //trigger
    if (muons.size() >= 2 && muons[0].m_lvec.Pt() >= CUTS.Get<float>("lep_pt0")) {
	    h_zmm_cutflow->Fill(3) ; //pass muon  cuts

      float zmm_w = evtW*muonSF_w*trigSF_muon ;
      //TEMP
      //zmm_w = evtW*muonSF_w;
      //std::cout << "\n evtW: " << evtW << " " << muonSF_w << " " << trigSF_muon << " " << zmm_w; 
      float zmmb_w = zmm_w*btag_w;
      
      ZObj Z(muons[0],muons[1]) ;
      
      h_Zmm_ZmassFull->Fill(Z.m_lvec.M(), zmm_w) ;
      
      if (Z.m_lvec.M() >= CUTS.Get<float>("ZMassL") && Z.m_lvec.M() <= CUTS.Get<float>("ZMassH")) { 
      
        h_zmm_cutflow->Fill(4) ; //pass Z mass cuts
        
        h_zmm_jet->FillNjet(jets.size(), zmm_w) ;
        
        if (jets.size() >= 1) {

          h_zmm_cutflow->Fill(5) ; //pass jet cuts

          float deltaRmuonlep0 = Z.m_lep0.m_lvec.DeltaR(jets[0].m_lvec);
          float deltaRmuonlep1 = Z.m_lep1.m_lvec.DeltaR(jets[0].m_lvec);
        
          h_zmm_jet->Fill(Z, jets[0], zmm_w) ;
          h_zmm_jet->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zmm_w);

          h_dR_jm->Fill(std::min(deltaRmuonlep0,deltaRmuonlep1), zmm_w) ;
 
          //tagging efficiency
          if (CUTS.GetStr("jet_main_btagWP")=="deepCSVT" || CUTS.GetStr("jet_main_btagWP")=="deepJetT")
            Fill_btagEffi(jets,"T",zmm_w) ;
          if (CUTS.GetStr("jet_main_btagWP")=="deepCSVM" || CUTS.GetStr("jet_main_btagWP")=="deepJetM")
            Fill_btagEffi(jets,"M",zmm_w) ;
        } //end at least one jet
        
        //Zmm+bjets
        h_zmm_bjet->FillNjet(bjets.size(),zmmb_w) ; 

        if (*(r->MET_pt) < CUTS.Get<float>("MET")) h_zmm_bjet_afterMET->FillNjet(bjets.size(),zmmb_w) ; 

        if (bjets.size() >= 1) {
          
          h_zmm_cutflow->Fill(6) ;
          
          h_zmm_bjet->Fill(Z, bjets[0], zmmb_w) ; 
          h_zmm_bjet->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zmmb_w);
          
          if (*(r->MET_pt) < CUTS.Get<float>("MET")) {
            h_zmm_bjet_afterMET->Fill(Z, bjets[0], zmmb_w) ; 
            h_zmm_bjet_afterMET->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zmmb_w);
          }

        }
      
        //Zmm+2bjets
        if (bjets.size() >= 2) {
          
          h_zmm_cutflow->Fill(7) ;
      
#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
          //Unfolding plots
          if (bjets[0].m_flav == 5 && bjets[1].m_flav == 5) {
            w_zmm_rec_uf = zmmb_w;
            zmm_rec_uf.push_back(muons[0].m_lvec);
            zmm_rec_uf.push_back(muons[1].m_lvec);
            zmm_rec_uf.push_back(bjets[0].m_lvec);
            zmm_rec_uf.push_back(bjets[1].m_lvec);
          }
#endif

          if (*(r->MET_pt) < CUTS.Get<float>("MET")) {
            
            h_zmm_cutflow->Fill(8) ;
            
            h_zmm_2bjet->Fill(Z, bjets[0], bjets[1], zmmb_w) ; 
            h_zmm_2bjet->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zmmb_w);

#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
            if (bjets[0].m_flav == 5 && bjets[1].m_flav == 5) {
              h_zmm_2bjet_bb->Fill(Z, bjets[0], bjets[1], zmmb_w) ;
              h_zmm_2bjet_bb->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zmmb_w);
              w_zmm_rec_afterMET_uf = zmmb_w;
              zmm_rec_afterMET_uf.push_back(muons[0].m_lvec);
              zmm_rec_afterMET_uf.push_back(muons[1].m_lvec);
              zmm_rec_afterMET_uf.push_back(bjets[0].m_lvec);
              zmm_rec_afterMET_uf.push_back(bjets[1].m_lvec);
            }
            else if ((bjets[0].m_flav == 5 && bjets[1].m_flav != 5) || (bjets[0].m_flav != 5 && bjets[1].m_flav == 5)) {
              h_zmm_2bjet_bX->Fill(Z, bjets[0], bjets[1], zmmb_w) ;
              h_zmm_2bjet_bX->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zmmb_w);
            }
            else {
              h_zmm_2bjet_XX->Fill(Z, bjets[0], bjets[1], zmmb_w) ;
              h_zmm_2bjet_XX->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), zmmb_w);
            }

#endif
          } //end met cut
        } //end at least two b-tagged jets

        //at least one b-tagged jets using deepJet
        h_zmm_bjet_deepJet->FillNjet(bjets_deepJet.size(),evtW) ; //FIXME 
        if (bjets_deepJet.size() >=1) {
          
          h_zmm_cutflow->Fill(8) ;

          h_zmm_bjet_deepJet->Fill(Z, bjets_deepJet[0], evtW) ; //FIXME
          h_zmm_bjet_deepJet->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), evtW); //FIXME
        }

        //at least two b-tagged jets using deepJet
        if (bjets_deepJet.size() >=2) {
          
          h_zmm_cutflow->Fill(9) ;

          h_zmm_2bjet_deepJet->Fill(Z, bjets_deepJet[0], bjets_deepJet[1], evtW) ; //FIXME
          h_zmm_2bjet_deepJet->FillMet(*(r->MET_pt), *(r->PuppiMET_pt), evtW); //FIXME
        }

      } //end Z mass cut

    } //end two muons
  } //end trigger

  //Fill additional unfolding plots
#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
  //if (zee_rec_uf.size() != 0 || zee_gen_uf.size() != 0) std::cout << "\n Size zee: " << zee_rec_uf.size() << " " << zee_gen_uf.size() ;
  //if (zmm_rec_uf.size() != 0 || zmm_gen_uf.size() != 0) std::cout << "\n Size zmm: " << zmm_rec_uf.size() << " " << zmm_gen_uf.size() ;
  //FillUnfolding(zee_rec_uf,zee_gen_uf,h_zee_unfolding,w_zee_rec_uf,genWeight); 
  //FillUnfolding(zmm_rec_uf,zmm_gen_uf,h_zmm_unfolding,w_zmm_rec_uf,genWeight); 
  //For MC validation turn rec SF off
  //TEMP
  //w_zee_rec_uf = genWeight;
  //w_zmm_rec_uf = genWeight;
  //w_zee_rec_afterMET_uf = genWeight;
  //w_zmm_rec_afterMET_uf = genWeight;
  FillUnfolding_1(zee_rec_uf,zee_gen_uf,h_zee_unfolding,w_zee_rec_uf,genWeight); 
  FillUnfolding_1(zmm_rec_uf,zmm_gen_uf,h_zmm_unfolding,w_zmm_rec_uf,genWeight); 
  FillUnfolding_1(zee_rec_afterMET_uf,zee_gen_uf,h_zee_afterMET_unfolding,w_zee_rec_afterMET_uf,genWeight); 
  FillUnfolding_1(zmm_rec_afterMET_uf,zmm_gen_uf,h_zmm_afterMET_unfolding,w_zmm_rec_afterMET_uf,genWeight); 
#endif

  return;

} //end Process



void ZbSelection::Terminate(TList* mergedList, std::string outFileName) {
  
  TList* aList = new TList() ;
  TParameter<double>* a = new TParameter<double>("lep_eta",CUTS.Get<float>("lep_eta")) ;
  aList->Add(a) ;
  a = new TParameter<double>("lep_pt0",CUTS.Get<float>("lep_pt0")) ;
  aList->Add(a) ;
  a = new TParameter<double>("lep_pt1",CUTS.Get<float>("lep_pt1")) ;
  aList->Add(a) ;
  a = new TParameter<double>("ZMassL",CUTS.Get<float>("ZMassL")) ;
  aList->Add(a) ;
  a = new TParameter<double>("ZMassH",CUTS.Get<float>("ZMassH")) ;
  aList->Add(a) ;
  a = new TParameter<double>("muon_iso",CUTS.Get<float>("muon_iso")) ;
  aList->Add(a) ;
  a = new TParameter<double>("lep_jetOverlap_pt",CUTS.Get<float>("lep_jetOverlap_pt")) ;
  aList->Add(a) ;
  a = new TParameter<double>("lep_jetOverlap_eta",CUTS.Get<float>("lep_jetOverlap_eta")) ;
  aList->Add(a) ;
  a = new TParameter<double>("jet_pt",CUTS.Get<float>("jet_pt")) ;
  aList->Add(a) ;
  a = new TParameter<double>("jet_eta",CUTS.Get<float>("jet_eta")) ;
  aList->Add(a) ;
  std::string name = "jet_deepCSVT_" + m_year ;
  a = new TParameter<double>(name.c_str(),CUTS.Get<float>(name)) ;
  aList->Add(a) ;
  name = "jet_deepJetM_" + m_year ;
  a = new TParameter<double>(name.c_str(),CUTS.Get<float>(name)) ;
  aList->Add(a) ;

  
  TFile* f = new TFile(outFileName.c_str(), "UPDATE") ;
  aList->Write("ZbSelectionCuts",TObject::kSingleKey) ;

  f->Close() ;

}

//////////////////////////////////
void ZbSelection::Fill_btagEffi(std::vector<JetObj> jets, std::string bTagWP, float w) { //bTagWP = L,M,T
  for(unsigned iJ = 0 ; iJ < jets.size() ; ++iJ) {
    if(jets[iJ].m_flav == 5) {
      h_eff_b->Fill(jets[iJ].m_lvec.Pt(),"deno",w) ;
      if (jets[iJ].m_deepCSV > CUTS.Get<float>("jet_deepCSV"+bTagWP+"_" + m_year)) h_eff_b->Fill(jets[iJ].m_lvec.Pt(),"num",w) ; 
      h_eff_bdj->Fill(jets[iJ].m_lvec.Pt(),"deno",w) ;
      if (jets[iJ].m_deepJet > CUTS.Get<float>("jet_deepJet"+bTagWP+"_" + m_year)) h_eff_bdj->Fill(jets[iJ].m_lvec.Pt(),"num",w) ; 
    }
    if(jets[iJ].m_flav == 4) {
      h_eff_c->Fill(jets[iJ].m_lvec.Pt(),"deno",w) ;
      if (jets[iJ].m_deepCSV > CUTS.Get<float>("jet_deepCSV"+bTagWP+"_" + m_year)) h_eff_c->Fill(jets[iJ].m_lvec.Pt(),"num",w) ; 
      h_eff_cdj->Fill(jets[iJ].m_lvec.Pt(),"deno",w) ;
      if (jets[iJ].m_deepJet > CUTS.Get<float>("jet_deepJet"+bTagWP+"_" + m_year)) h_eff_cdj->Fill(jets[iJ].m_lvec.Pt(),"num",w) ; 
    }
    if(jets[iJ].m_flav == 0) {
      h_eff_l->Fill(jets[iJ].m_lvec.Pt(),"deno",w) ;
      if (jets[iJ].m_deepCSV > CUTS.Get<float>("jet_deepCSV"+bTagWP+"_" + m_year)) h_eff_l->Fill(jets[iJ].m_lvec.Pt(),"num",w) ; 
      h_eff_ldj->Fill(jets[iJ].m_lvec.Pt(),"deno",w) ;
      if (jets[iJ].m_deepJet > CUTS.Get<float>("jet_deepJet"+bTagWP+"_" + m_year)) h_eff_ldj->Fill(jets[iJ].m_lvec.Pt(),"num",w) ; 
    }
  }
}

//Fill unfolding plots
void ZbSelection::FillUnfolding(std::vector<TLorentzVector> recs, std::vector<TLorentzVector> gens, UnfoldingPlots* h, float w_all, float w_gen) {
  //has reco
  if (recs.size()==4) {
    h->Fill("REC_ALL",recs[0],recs[1],recs[2],recs[3],w_all);
    if (gens.size()!=4) { //has reco but no gen
      //std::cout << "\n Has reco, gens.size != 4" << gens.size();
      h->Fill("REC_NOGENMATCH",recs[0],recs[1],recs[2],recs[3],w_all);
    }
    else { //has gen
      float dR1 = std::min(recs[0].DeltaR(gens[0]),recs[0].DeltaR(gens[1]));
      float dR2 = std::min(recs[1].DeltaR(gens[0]),recs[1].DeltaR(gens[1]));
      float dR3 = std::min(recs[2].DeltaR(gens[2]),recs[2].DeltaR(gens[3]));
      float dR4 = std::min(recs[3].DeltaR(gens[2]),recs[3].DeltaR(gens[3]));
      if (dR1 > 0.2 || dR2 > 0.2 || dR3 > 0.2 || dR4 > 0.2) { //no matching found fill bkgr
        //std::cout << "\n Has reco, has gen, fail dR" << gens.size();
        h->Fill("REC_NOGENMATCH",recs[0],recs[1],recs[2],recs[3],w_all);
      }
      else { //matching found fill response matrix
        //std::cout << "\n Has reco, has gen, pass dR" << gens.size();
        h->FillRes(recs[0],recs[1],recs[2],recs[3],gens[0],gens[1],gens[2],gens[3],w_all,w_gen);
      }
    }
  }
  if (gens.size()==4) { //has gen
    h->Fill("GEN_ALL",gens[0],gens[1],gens[2],gens[3],w_gen);
    if (recs.size()!=4) { //has gen no reco
      h->Fill("GEN_NORECMATCH",gens[0],gens[1],gens[2],gens[3],w_gen);
    }
    else { //has gen and reco but whether they match or not
      float dR1 = std::min(gens[0].DeltaR(recs[0]),gens[0].DeltaR(recs[1]));
      float dR2 = std::min(gens[1].DeltaR(recs[0]),gens[1].DeltaR(recs[1]));
      float dR3 = std::min(gens[2].DeltaR(recs[2]),gens[2].DeltaR(recs[3]));
      float dR4 = std::min(gens[3].DeltaR(recs[2]),gens[3].DeltaR(recs[3]));
      if (dR1 > 0.2 || dR2 > 0.2 || dR3 > 0.2 || dR4 > 0.2) { //no matching found fill efficiency 
        h->Fill("GEN_NORECMATCH",gens[0],gens[1],gens[2],gens[3],w_gen);
      }
    }
  }
}


//Fill unfolding plots
void ZbSelection::FillUnfolding_1(std::vector<TLorentzVector> recs, std::vector<TLorentzVector> gens, UnfoldingPlots* h, float w_all, float w_gen) {
  //has reco
  if (recs.size()==4) {
    h->Fill("REC_ALL",recs[0],recs[1],recs[2],recs[3],w_all);
    if (gens.size()==4) {
      float dR1 = std::min(recs[0].DeltaR(gens[0]),recs[0].DeltaR(gens[1]));
      float dR2 = std::min(recs[1].DeltaR(gens[0]),recs[1].DeltaR(gens[1]));
      float dR3 = std::min(recs[2].DeltaR(gens[2]),recs[2].DeltaR(gens[3]));
      float dR4 = std::min(recs[3].DeltaR(gens[2]),recs[3].DeltaR(gens[3]));
      if (dR1 < 0.2 && dR2 < 0.2 && dR3 < 0.2 && dR4 < 0.2) { //found rec matching with gen 
        //std::cout << "\n Has reco, has gen, fail dR" << gens.size();
        h->Fill("REC_GENMATCH",recs[0],recs[1],recs[2],recs[3],w_all);
        h->FillRes(recs[0],recs[1],recs[2],recs[3],gens[0],gens[1],gens[2],gens[3],w_all,w_gen);
      }
      else {
        h->Fill("REC_NOGENMATCH",recs[0],recs[1],recs[2],recs[3],w_all);
      }
    }
    else {
      h->Fill("REC_NOGENMATCH",recs[0],recs[1],recs[2],recs[3],w_all);
    }
  }

  if (gens.size()==4) { //has gen
    h->Fill("GEN_ALL",gens[0],gens[1],gens[2],gens[3],w_gen);
    if (recs.size()==4) { //gen match with reco
      float dR1 = std::min(gens[0].DeltaR(recs[0]),gens[0].DeltaR(recs[1]));
      float dR2 = std::min(gens[1].DeltaR(recs[0]),gens[1].DeltaR(recs[1]));
      float dR3 = std::min(gens[2].DeltaR(recs[2]),gens[2].DeltaR(recs[3]));
      float dR4 = std::min(gens[3].DeltaR(recs[2]),gens[3].DeltaR(recs[3]));
      if (dR1 < 0.2 && dR2 < 0.2 && dR3 < 0.2 && dR4 < 0.2) { //matching found fill efficiency 
        h->Fill("GEN_RECMATCH",gens[0],gens[1],gens[2],gens[3],w_all); //weights by w_all since match with reco
      }
      else {
        h->Fill("GEN_NORECMATCH",gens[0],gens[1],gens[2],gens[3],w_gen);
      }
    }
    else {
      h->Fill("GEN_NORECMATCH",gens[0],gens[1],gens[2],gens[3],w_gen);
    }
  }
}
