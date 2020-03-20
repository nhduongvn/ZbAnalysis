#define ZbSelection_cxx

#include <math.h>

#include "TList.h"
#include "TParameter.h"
#include "TLorentzVector.h"

#include "ZbSelection.h"
#include "Global.h"
#include "Obj.cxx"

#include <iostream>
#include "TH1.h"


using namespace std;
float PI = M_PI;

ZbSelection::ZbSelection(bool isData) : Selector(isData), h_zee_jet(0), h_zmm_jet(0) {}

ZbSelection::~ZbSelection() {
  if (h_zee_jet) delete h_zee_jet;
  if (h_zmm_jet) delete h_zmm_jet;
} 

void ZbSelection::SlaveBegin(Reader* r) {

  
  h_zee_jet = new ZbPlots("Zee_jet") ;
  h_zmm_jet = new ZbPlots("Zmm_jet") ;

  h_dR_je = new TH1D("h_dR_je","",500,0,5) ;
  h_dR_jm = new TH1D("h_dR_jm","",500,0,5) ;

  
  h_nJet = new TH1D("h_nJet","",10,-0.5,9.5) ;

  h_ele_cutflow = new TH1D("ele_cutflow","",10,-0.5,9.5) ;
  h_muon_cutflow = new TH1D("muon_cutflow","",10,-0.5,9.5) ;
  h_Jet_cutflow = new TH1D("Jet_cutflow","",10,-0.5,9.5) ;
  h_zee_cutflow = new TH1D("zee_cutflow","",10,-0.5,9.5) ;
  h_zmm_cutflow = new TH1D("zmm_cutflow","",10,-0.5,9.5) ;

  h_nEle_nocut = new TH1D("nEle_nocut","",10,-0.5,9.5) ;
  h_ele_pT_nocut = new TH1D("ele_pT_nocut","",200,0.0,200) ;
  h_ele_eta_nocut = new TH1D("ele_eta_nocut","",300,-3.0,3.0) ;
  h_ele_phi_nocut = new TH1D("ele_phi_nocut","",400,-4.0,4.0) ;

  h_nEle_loose_MVA = new TH1D("nEle_loose_MVA","",10,-0.5,9.5) ;
  h_ele_pT_loose_MVA = new TH1D("ele_pT_loose_MVA","",200,0.0,200) ;
  h_ele_eta_loose_MVA = new TH1D("ele_eta_loose_MVA","",300,-3.0,3.0) ;
  h_ele_phi_loose_MVA = new TH1D("ele_phi_loose_MVA","",400,-4.0,4.0) ;

  h_nEle_kin_cut = new TH1D("nEle_kin_cut","",10,-0.5,9.5) ;
  h_ele_pT_kin_cut = new TH1D("ele_pT_kin_cut","",200,0.0,200) ;
  h_ele_eta_kin_cut = new TH1D("ele_eta_kin_cut","",300,-3.0,3.0) ;
  h_ele_phi_kin_cut = new TH1D("ele_phi_kin_cut","",400,-4.0,4.0) ;

  h_nmuon_nocut = new TH1D("nmuon_nocut","",10,-0.5,9.5) ;
  h_muon_pT_nocut = new TH1D("muon_pT_nocut","",200,0.0,200) ;
  h_muon_eta_nocut = new TH1D("muon_eta_nocut","",300,-3.0,3.0) ;
  h_muon_phi_nocut = new TH1D("muon_phi_nocut","",400,-4.0,4.0) ;

  h_nmuon_loose_id = new TH1D("nmuon_loose_id","",10,-0.5,9.5) ;
  h_muon_pT_loose_id = new TH1D("muon_pT_loose_id","",200,0.0,200) ;
  h_muon_eta_loose_id = new TH1D("muon_eta_loose_id","",300,-3.0,3.0) ;
  h_muon_phi_loose_id = new TH1D("muon_phi_loose_id","",400,-4.0,4.0) ;

  h_nmuon_kin_cut = new TH1D("nmuon_kin_cut","",10,-0.5,9.5) ;
  h_muon_pT_kin_cut = new TH1D("muon_pT_kin_cut","",200,0.0,200) ;
  h_muon_eta_kin_cut = new TH1D("muon_eta_kin_cut","",300,-3.0,3.0) ;
  h_muon_phi_kin_cut = new TH1D("muon_phi_kin_cut","",400,-4.0,4.0) ;

  h_njet_nocut = new TH1D("njet_nocut","",10,-0.5,9.5) ;
  h_jet_pT_nocut = new TH1D("jet_pT_nocut","",200,0.0,200) ;
  h_jet_eta_nocut = new TH1D("jet_eta_nocut","",300,-3.0,3.0) ;
  h_jet_phi_nocut = new TH1D("jet_phi_nocu","",400,-4.0,4.0) ;

  h_njet_ele_rem = new TH1D("njet_ele_rem","",10,-0.5,9.5) ; 
  h_jet_pT_ele_rem = new TH1D("jet_pT_ele_rem","",200,0.0,200) ;
  h_jet_eta_ele_rem = new TH1D("jet_eta_ele_rem","",300,-3.0,3.0) ;
  h_jet_phi_ele_rem = new TH1D("jet_phi_ele_rem","",400,-4.0,4.0) ;

  h_njet_muon_rem = new TH1D("njet_muon_rem","",10,-0.5,9.5) ; 
  h_jet_pT_muon_rem = new TH1D("jet_pT_muon_rem","",200,0.0,200) ;
  h_jet_eta_muon_rem = new TH1D("jet_eta_muon_rem","",300,-3.0,3.0) ;
  h_jet_phi_muon_rem = new TH1D("jet_phi_muon_rem","",400,-4.0,4.0) ;

  h_njet_jetID = new TH1D("njet_jetID","",10,-0.5,9.5) ; 
  h_jet_pT_jetID = new TH1D("jet_pT_jetID","",200,0.0,200) ;
  h_jet_eta_jetID = new TH1D("jet_eta_jetID","",300,-3.0,3.0) ;
  h_jet_phi_jetID = new TH1D("jet_phi_jetID","",400,-4.0,4.0) ;

  h_njet_kin_cut = new TH1D("njet_kin_cut","",10,-0.5,9.5) ; 
  h_jet_pT_kin_cut = new TH1D("jet_pT_kin_cut","",200,0.0,200) ;
  h_jet_eta_kin_cut = new TH1D("jet_eta_kin_cut","",300,-3.0,3.0) ;
  h_jet_phi_kin_cut = new TH1D("jet_phi_kin_cut","",400,-4.0,4.0) ;


  //Add histograms to fOutput so they can be saved in Processor::SlaveTerminate
  std::vector<TH1*> tmp = h_zee_jet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);

  tmp = h_zmm_jet->returnHisto() ;
  for(size_t i=0;i<tmp.size();i++) r->GetOutputList()->Add(tmp[i]);

  r->GetOutputList()->Add(h_dR_je) ;
  r->GetOutputList()->Add(h_dR_jm) ;
  
  
  r->GetOutputList()->Add(h_ele_cutflow) ;
  r->GetOutputList()->Add(h_muon_cutflow) ;
  r->GetOutputList()->Add(h_Jet_cutflow) ;
  r->GetOutputList()->Add(h_zee_cutflow) ;
  r->GetOutputList()->Add(h_zmm_cutflow) ;
  
  r->GetOutputList()->Add(h_nEle_nocut) ;
  r->GetOutputList()->Add(h_ele_pT_nocut) ;
  r->GetOutputList()->Add(h_ele_eta_nocut) ;
  r->GetOutputList()->Add(h_ele_phi_nocut) ;

  r->GetOutputList()->Add(h_nEle_loose_MVA) ;
  r->GetOutputList()->Add(h_ele_pT_loose_MVA) ;
  r->GetOutputList()->Add(h_ele_eta_loose_MVA) ;
  r->GetOutputList()->Add(h_ele_phi_loose_MVA) ;

  r->GetOutputList()->Add(h_nEle_kin_cut) ;
  r->GetOutputList()->Add(h_ele_pT_kin_cut) ;
  r->GetOutputList()->Add(h_ele_eta_kin_cut) ;
  r->GetOutputList()->Add(h_ele_phi_kin_cut) ;

  r->GetOutputList()->Add(h_nmuon_nocut) ;
  r->GetOutputList()->Add(h_muon_pT_nocut) ;
  r->GetOutputList()->Add(h_muon_eta_nocut) ;
  r->GetOutputList()->Add(h_muon_phi_nocut) ;

  r->GetOutputList()->Add(h_nmuon_loose_id) ;
  r->GetOutputList()->Add(h_muon_pT_loose_id) ;
  r->GetOutputList()->Add(h_muon_eta_loose_id) ;
  r->GetOutputList()->Add(h_muon_phi_loose_id) ;

  r->GetOutputList()->Add(h_nmuon_kin_cut) ;
  r->GetOutputList()->Add(h_muon_pT_kin_cut) ;
  r->GetOutputList()->Add(h_muon_eta_kin_cut) ;
  r->GetOutputList()->Add(h_muon_phi_kin_cut) ;

  r->GetOutputList()->Add(h_njet_nocut) ;
  r->GetOutputList()->Add(h_jet_pT_nocut) ;
  r->GetOutputList()->Add(h_jet_eta_nocut) ;
  r->GetOutputList()->Add(h_jet_phi_nocut) ;

  r->GetOutputList()->Add(h_njet_ele_rem) ;
  r->GetOutputList()->Add(h_jet_pT_ele_rem) ;
  r->GetOutputList()->Add(h_jet_eta_ele_rem) ;
  r->GetOutputList()->Add(h_jet_phi_ele_rem) ;

  r->GetOutputList()->Add(h_njet_muon_rem) ;
  r->GetOutputList()->Add(h_jet_pT_muon_rem) ;
  r->GetOutputList()->Add(h_jet_eta_muon_rem) ;
  r->GetOutputList()->Add(h_jet_phi_muon_rem) ;

  r->GetOutputList()->Add(h_njet_jetID) ;
  r->GetOutputList()->Add(h_jet_pT_jetID) ;
  r->GetOutputList()->Add(h_jet_eta_jetID) ;
  r->GetOutputList()->Add(h_jet_phi_jetID) ;

  r->GetOutputList()->Add(h_njet_kin_cut) ;
  r->GetOutputList()->Add(h_jet_pT_kin_cut) ;
  r->GetOutputList()->Add(h_jet_eta_kin_cut) ;
  r->GetOutputList()->Add(h_jet_phi_kin_cut) ;

  const Int_t nx = 2, yx= 4, nzee=3, nzmm=3;
  const char *ele_cut[nx] = {"not cut","loose MVA"};
  const char *muon_cut[nx] = {"not cut","loose muon ID"};
  const char *jet_cut[yx] = {"not cut","ele overlap removal","muon overlap removal","loose jet ID"};
  const char *zee_cut[nzee] = {"all event (not cut)","pass electron  cuts","pass jet  cuts"};
  const char *zmm_cut[nzmm] = {"all event (not cut)","pass muon  cuts","pass jet  cuts"};
  for (int i=1;i<=nx;i++) h_ele_cutflow->GetXaxis()->SetBinLabel(i+1.5,ele_cut[i-1]);
  for (int i=1;i<=nx;i++) h_muon_cutflow->GetXaxis()->SetBinLabel(i+1.5,muon_cut[i-1]);
  for (int i=1;i<=yx;i++) h_Jet_cutflow->GetXaxis()->SetBinLabel(i+1.5,jet_cut[i-1]);
  for (int i=1;i<=nzee;i++) h_zee_cutflow->GetXaxis()->SetBinLabel(i+1.5,zee_cut[i-1]);
  for (int i=1;i<=nzmm;i++) h_zmm_cutflow->GetXaxis()->SetBinLabel(i+1.5,zmm_cut[i-1]);

}

void ZbSelection::Process(Reader* r) {
  //std::cout << "\n Data type: " << m_isData ;
  //std::cout << "\n Number of jets: " << *(r->nJet) ;
  //if (*(r->nJet)>0) std::cout << "\n First jet: " << (r->Jet_eta)[0] ;

  //=============Get objects============= 
  //electrons
  std::vector<LepObj> eles ;
  int  nele_nocut = 0;
  int  nele_loose_MVA = 0;
  int  nele_kin_cut = 0;
  for (unsigned int i = 0 ; i < *(r->nElectron) ; ++i) {nele_nocut++ ;
    h_ele_cutflow->Fill(1) ;
    LepObj ele((r->Electron_pt)[i],(r->Electron_eta)[i],(r->Electron_phi)[i],(r->Electron_mass)[i],0) ;
    h_nEle_nocut->Fill(nele_nocut) ;
    h_ele_pT_nocut->Fill(ele.m_lvec.Pt()) ;
    h_ele_eta_nocut->Fill(ele.m_lvec.Eta()) ;
    h_ele_phi_nocut->Fill(ele.m_lvec.Phi()) ;


    if (r-> Electron_mvaFall17V1Iso_WP90[i] <= 0) continue;
    nele_loose_MVA++;
    h_ele_cutflow->Fill(2)  ;
    h_nEle_loose_MVA->Fill(nele_loose_MVA) ;
    h_ele_pT_loose_MVA->Fill(ele.m_lvec.Pt()) ;
    h_ele_eta_loose_MVA->Fill(ele.m_lvec.Eta()) ;
    h_ele_phi_loose_MVA->Fill(ele.m_lvec.Phi()) ;

    if (ele.m_lvec.Pt() > CUTS.Get<float>("lep_pt") && fabs(ele.m_lvec.Eta()) < CUTS.Get<float>("lep_eta")){ 
      eles.push_back(ele) ;
        
      nele_kin_cut ++ ;
      h_nEle_kin_cut->Fill(nele_kin_cut) ;
      h_ele_pT_kin_cut->Fill(ele.m_lvec.Pt()) ;
      h_ele_eta_kin_cut->Fill(ele.m_lvec.Eta()) ;
      h_ele_phi_kin_cut->Fill(ele.m_lvec.Phi()) ;
    }

  }
  
  //muons
  int  nmuon_nocut = 0;
  int  nmuon_loose_id = 0;
  int  nmuon_kin_cut = 0;
  std::vector<LepObj> muons ;
  for (unsigned int i = 0 ; i < *(r->nMuon) ; ++i) {nmuon_nocut++ ;
    h_muon_cutflow->Fill(1) ;
    LepObj muon((r->Muon_pt)[i],(r->Muon_eta)[i],(r->Muon_phi)[i],(r->Muon_mass)[i],(r->Muon_pfRelIso03_all)[i]) ;
    h_nmuon_nocut->Fill(nmuon_nocut) ;
    h_muon_pT_nocut->Fill(muon.m_lvec.Pt()) ;
    h_muon_eta_nocut->Fill(muon.m_lvec.Eta()) ;
    h_muon_phi_nocut->Fill(muon.m_lvec.Phi()) ;
 
    if (r-> Muon_looseId[i] <= 0) continue;
    h_muon_cutflow->Fill(2) ;

    nmuon_loose_id ++;
    h_nmuon_loose_id->Fill(nmuon_loose_id) ;
    h_muon_pT_loose_id->Fill(muon.m_lvec.Pt()) ;
    h_muon_eta_loose_id->Fill(muon.m_lvec.Eta()) ;
    h_muon_phi_loose_id->Fill(muon.m_lvec.Phi()) ;
    if (muon.m_lvec.Pt() > CUTS.Get<float>("lep_pt") && fabs(muon.m_lvec.Eta()) < CUTS.Get<float>("lep_eta")){ nmuon_kin_cut++;
      muons.push_back(muon) ;
      
      h_nmuon_kin_cut->Fill(nmuon_kin_cut) ;
      h_muon_pT_kin_cut->Fill(muon.m_lvec.Pt()) ;
      h_muon_eta_kin_cut->Fill(muon.m_lvec.Eta()) ;
      h_muon_phi_kin_cut->Fill(muon.m_lvec.Phi()) ;
    }
  }
  
  //jets
  std::vector<JetObj> jets ;
  int  njet_nocut = 0;
  int  njet_ele_rem = 0;
  int  njet_muon_rem = 0;  
  int  njet_jetID = 0;  
  int  njet_kin_cut = 0;  

  for (unsigned int i = 0 ; i < *(r->nJet) ; ++i) { njet_nocut++;
    h_Jet_cutflow->Fill(1) ;
    JetObj jet((r->Jet_pt)[i],(r->Jet_eta)[i],(r->Jet_phi)[i],(r->Jet_mass)[i],(r->Jet_btagDeepB)[i]) ;

   h_njet_nocut->Fill(njet_nocut) ;
   h_jet_pT_nocut->Fill(jet.m_lvec.Pt()) ;
   h_jet_eta_nocut->Fill(jet.m_lvec.Eta()) ;
   h_jet_phi_nocut->Fill(jet.m_lvec.Phi()) ;	
    if (jet.IsLepton(eles)) continue ;
     njet_ele_rem++ ;
     h_njet_ele_rem->Fill(njet_ele_rem) ;
     h_jet_pT_ele_rem->Fill(jet.m_lvec.Pt()) ;
     h_jet_eta_ele_rem->Fill(jet.m_lvec.Eta()) ;
     h_jet_phi_ele_rem->Fill(jet.m_lvec.Phi()) ;
  
    h_Jet_cutflow->Fill(2) ;
    if (jet.IsLepton(muons)) continue ;
     njet_muon_rem++ ;
     h_njet_muon_rem->Fill(njet_muon_rem) ;
     h_jet_pT_muon_rem->Fill(jet.m_lvec.Pt()) ;
     h_jet_eta_muon_rem->Fill(jet.m_lvec.Eta()) ;
     h_jet_phi_muon_rem->Fill(jet.m_lvec.Phi()) ;
    h_Jet_cutflow->Fill(3) ;
    if (r->Jet_jetId[i] <= 0 ) continue ;
    njet_jetID ++ ;
     h_njet_jetID->Fill(njet_jetID) ;
     h_jet_pT_jetID->Fill(jet.m_lvec.Pt()) ;
     h_jet_eta_jetID->Fill(jet.m_lvec.Eta()) ;
     h_jet_phi_jetID->Fill(jet.m_lvec.Phi()) ;

    h_Jet_cutflow->Fill(4) ;
    //if (r->Jet_btagCSVV2[i] <= 0.8484) continue;
    //if (r->SV_mass[i] < 0) continue;
    if (jet.m_lvec.Pt() > CUTS.Get<float>("jet_pt") && fabs(jet.m_lvec.Eta()) < CUTS.Get<float>("jet_eta")) {
      jets.push_back(jet) ;
      njet_kin_cut++ ;  
     h_njet_kin_cut->Fill(njet_kin_cut) ;
     h_jet_pT_kin_cut->Fill(jet.m_lvec.Pt()) ;
     h_jet_eta_kin_cut->Fill(jet.m_lvec.Eta()) ;
     h_jet_phi_kin_cut->Fill(jet.m_lvec.Phi()) ;  

	}
  }
  
  //Zee + jets
  h_zee_cutflow->Fill(1); //all events (not cut)
  if (eles.size() >= 2) {
    h_zee_cutflow->Fill(2) ; //pass electron  cuts
    if (jets.size() >= 1) {
      h_zee_cutflow->Fill(3) ; //pass jet cuts

      ZObj Z(eles[0],eles[1]) ;
      JetObj J(jets[0]) ;
      
      // isolation
      float deltaRelelep0 = Z.m_lep0.m_lvec.DeltaR(J.m_lvec);
      float deltaRelelep1 = Z.m_lep1.m_lvec.DeltaR(J.m_lvec);
      
      h_zee_jet->Fill(Z, J) ;
      
      h_dR_je->Fill(std::min(deltaRelelep0,deltaRelelep1)) ;
	
    }

  }

     

  //Zmm + jets
  h_zmm_cutflow->Fill(1); //all events (not cut)
  if (muons.size() >= 2) {
    h_zmm_cutflow->Fill(2) ; //pass muon  cuts
    if (jets.size() >= 1) {
      h_zmm_cutflow->Fill(3) ; //pass jet cuts

      ZObj Z(muons[0],muons[1]) ;
      JetObj J(jets[0]) ;
      float deltaRmuonlep0 = Z.m_lep0.m_lvec.DeltaR(J.m_lvec);
      float deltaRmuonlep1 = Z.m_lep1.m_lvec.DeltaR(J.m_lvec);
      
      h_zmm_jet->Fill(Z, J) ;
      h_dR_jm->Fill(std::min(deltaRmuonlep0,deltaRmuonlep1)) ;

    }

  }


 

} //end Process



void ZbSelection::Terminate(TList* mergedList, std::string outFileName) {
  
  TList* aList = new TList() ;
  TParameter<double>* a = new TParameter<double>("lep_eta",CUTS.Get<float>("lep_eta")) ;
  aList->Add(a) ;
  a = new TParameter<double>("lep_pt",CUTS.Get<float>("lep_pt")) ;
  aList->Add(a) ;
  a = new TParameter<double>("jet_pt",CUTS.Get<float>("jet_pt")) ;
  aList->Add(a) ;
  a = new TParameter<double>("jet_eta",CUTS.Get<float>("jet_eta")) ;
  aList->Add(a) ;
  
  TFile* f = new TFile(outFileName.c_str(), "UPDATE") ;
  aList->Write("ZbSelectionCuts",TObject::kSingleKey) ;

  f->Close() ;

}
