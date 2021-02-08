#ifndef Plots_cxx
#define Plots_cxx

#include "Obj.cxx"
#include "TString.h"
#include "TH1.h"
#include "TH1D.h"
#include "TMath.h"
#include "TLorentzVector.h"

const unsigned NBIN_PT_JET = 300; 
const float X_PT_JET[2] = {0,300};
const unsigned NBIN_PT_Z = 300;
const float X_PT_Z[2] = {0,300};
const unsigned NBIN_DR_2B = 100;
const float X_DR_2B[2] = {0,10};
const unsigned NBIN_ETA = 300;
const float X_ETA[2] = {-3,3};
const unsigned NBIN_PHI = 120;
const float X_PHI[2] = {-TMath::Pi(),TMath::Pi()};
const unsigned NBIN_DRMIN_Z2B = 100;
const float X_DRMIN_Z2B[2] = {0,10};
const unsigned NBIN_A_Z2B = 100;
const float X_A_Z2B[2] = {0,1};
const unsigned NBIN_M_2B = 500;
const float X_M_2B[2] = {0,500};
const unsigned NBIN_M_Z2B = 600;
const float X_M_Z2B[2] = {0,600};

const unsigned NBIN_DPHI_2B(31);
const float X_DPHI_2B[NBIN_DPHI_2B+1] = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,
                                           1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,
                                           2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0,TMath::Pi()};

class ZbPlots 
{
  
  float DR(float x, float y){return (sqrt(pow(x,2.0) + pow(y,2.0)));}
  float Deta(float x, float y){return (x-y);}
  float Dphi(float x, float y){return (x-y);}
  Double_t const  kPI        = TMath::Pi();
  Double_t const  kTWOPI     = 2.*kPI;
  double DphiC(double x){
    if ((x<=kPI && x>=0) or (x<0 && x> -kPI)) return x;
    else if (x >= kPI) return DphiC (x-kTWOPI);
    else if (x < -kPI) return DphiC(x + kTWOPI);
    return 0;
  }


  public:
    ZbPlots(TString name) : m_name(name) {
      //FIXME add ptZ
      h_pt_lep0 = new TH1D(name + "_pt_lep0","",200,0,200) ;
      h_phi_lep0 = new TH1D(name + "_phi_lep0","",NBIN_PHI,X_PHI[0],X_PHI[1]) ;
      h_eta_lep0 = new TH1D(name + "_eta_lep0","",NBIN_ETA,X_ETA[0],X_ETA[1]) ;
      h_deltaRroot_lep0 = new TH1D(name + "_deltaRroot_lep0","",10000,-0.5,9.5) ;
      h_deltaPhiRoot_lep0 = new TH1D(name + "_deltaPhiRoot_lep0","",800,-4.0,4.0) ;
      h_deltaEtaRoot_lep0 = new TH1D(name + "_deltaEtaRoot_lep0","",800,-4.0,4.0) ;
      h_deltaRAlgo_lep0 = new TH1D(name + "_deltaRAlgo_lep0","",10000,-0.5,9.5) ;
      h_deltaPhiAlgo_lep0 = new TH1D(name + "_deltaPhiAlgo_lep0","",800,-4.0,4.0) ;
      h_deltaEtaAlgo_lep0 = new TH1D(name + "_deltaEtaAlgo_lep0","",800,-4.0,4.0) ;
      h_pt_lep0->Sumw2() ;
      h_phi_lep0->Sumw2() ;
      h_eta_lep0->Sumw2() ;
      h_deltaRroot_lep0->Sumw2() ;
      h_deltaPhiRoot_lep0->Sumw2() ;
      h_deltaEtaRoot_lep0->Sumw2() ;
      h_deltaRAlgo_lep0->Sumw2() ;
      h_deltaPhiAlgo_lep0->Sumw2() ;
      h_deltaEtaAlgo_lep0->Sumw2() ;

      h_pt_lep1 = new TH1D(name + "_pt_lep1","",200,0,200) ;
      h_phi_lep1 = new TH1D(name + "_phi_lep1","",NBIN_PHI,X_PHI[0],X_PHI[1]) ;
      h_eta_lep1 = new TH1D(name + "_eta_lep1","",NBIN_ETA,X_ETA[0],X_ETA[1]) ;
      h_ZMass = new TH1D(name + "_ZMass","",300,0,300) ;
      h_deltaRroot_lep1 = new TH1D(name + "_deltaRroot_lep1","",10000,-0.5,9.5) ;
      h_deltaPhiRoot_lep1 = new TH1D(name + "_deltaPhiRoot_lep1","",800,-4.0,4.0) ;
      h_deltaEtaRoot_lep1 = new TH1D(name + "_deltaEtaRoot_lep1","",800,-4.0,4.0) ;
      h_deltaRAlgo_lep1 = new TH1D(name + "_deltaRAlgo_lep1","",10000,-0.5,9.5) ;
      h_deltaPhiAlgo_lep1 = new TH1D(name + "_deltaPhiAlgo_lep1","",800,-4.0,4.0) ;
      h_deltaEtaAlgo_lep1 = new TH1D(name + "_deltaEtaAlgo_lep1","",800,-4.0,4.0) ;
      
      h_pt_lep1->Sumw2() ;
      h_phi_lep1->Sumw2() ;
      h_eta_lep1->Sumw2() ;
      h_ZMass->Sumw2() ;
      h_deltaRroot_lep1->Sumw2() ;
      h_deltaPhiRoot_lep1->Sumw2() ;
      h_deltaEtaRoot_lep1->Sumw2() ;
      h_deltaRAlgo_lep1->Sumw2() ;
      h_deltaPhiAlgo_lep1->Sumw2() ;
      h_deltaEtaAlgo_lep1->Sumw2() ;

      h_pt_jet = new TH1D(name + "_pt_jet","",NBIN_PT_JET,X_PT_JET[0],X_PT_JET[1]) ;
      h_phi_jet = new TH1D(name + "_phi_jet","",NBIN_PHI,X_PHI[0],X_PHI[1]) ;
      h_eta_jet = new TH1D(name + "_eta_jet","",NBIN_ETA,X_ETA[0],X_ETA[1]) ;
      h_mSV_jet = new TH1D(name + "_mSV_jet","",100,0,10) ;
      h_Njet = new TH1D(name + "_Njet","",15,0,15) ;
      h_pt_jet->Sumw2() ;
      h_phi_jet->Sumw2() ;
      h_eta_jet->Sumw2() ;
      h_Njet->Sumw2() ;
      h_mSV_jet->Sumw2();

      h_pt_dilepton = new TH1D(name + "_pt_dilepton","",NBIN_PT_Z,X_PT_Z[0],X_PT_Z[1]) ;
      h_phi_dilepton = new TH1D(name + "_phi_dilepton","",NBIN_PHI,X_PHI[0],X_PHI[1]) ;
      h_eta_dilepton = new TH1D(name + "_eta_dilepton","",NBIN_ETA,X_ETA[0],X_ETA[1]) ;
      h_pt_dilepton->Sumw2() ;
      h_phi_dilepton->Sumw2() ;
      h_eta_dilepton->Sumw2() ;

      h_met = new TH1D(name + "_met","",500,0,500);
      h_met->Sumw2();
      h_met_smeared = new TH1D(name + "_met_smeared","",500,0,500);
      h_met_smeared->Sumw2();
      h_met_puppi = new TH1D(name + "_met_puppi","",500,0,500);
      h_met_puppi->Sumw2();

    } ;
    
    void Fill(ZObj& Z, JetObj& J, float w=1) {
  
      h_pt_lep0->Fill(Z.m_lep0.m_lvec.Pt(), w) ;
      h_phi_lep0->Fill(Z.m_lep0.m_lvec.Phi(), w) ;
      h_eta_lep0->Fill(Z.m_lep0.m_lvec.Eta(), w) ;
      h_deltaRroot_lep0->Fill(Z.m_lep0.m_lvec.DeltaR(J.m_lvec), w) ;
      h_deltaPhiRoot_lep0 ->Fill(Z.m_lep0.m_lvec.DeltaPhi(J.m_lvec), w) ;
      h_deltaEtaRoot_lep0 ->Fill(Z.m_lep0.m_lvec.Eta()-J.m_lvec.Eta(), w) ;
      h_deltaRAlgo_lep0->Fill(DR(DphiC(Dphi(Z.m_lep0.m_lvec.Phi(), J.m_lvec.Phi())), Deta(Z.m_lep0.m_lvec.Eta(), J.m_lvec.Eta())), w) ;
      h_deltaPhiAlgo_lep0 ->Fill(DphiC(Dphi(Z.m_lep0.m_lvec.Phi(), J.m_lvec.Phi())), w) ;
      h_deltaEtaAlgo_lep0 ->Fill(Deta(Z.m_lep0.m_lvec.Eta(), J.m_lvec.Eta()), w) ;
      
      h_pt_lep1->Fill(Z.m_lep1.m_lvec.Pt(), w) ;
      h_phi_lep1->Fill(Z.m_lep1.m_lvec.Phi(), w) ;
      h_eta_lep1->Fill(Z.m_lep1.m_lvec.Eta(), w) ;
      h_ZMass->Fill(Z.m_lvec.M(),w) ;
      h_deltaRroot_lep1->Fill(Z.m_lep1.m_lvec.DeltaR(J.m_lvec), w) ;
      h_deltaPhiRoot_lep1 ->Fill(Z.m_lep1.m_lvec.DeltaPhi(J.m_lvec), w) ;
      h_deltaEtaRoot_lep1 ->Fill(Z.m_lep1.m_lvec.Eta()-J.m_lvec.Eta(), w) ;
      h_deltaRAlgo_lep1->Fill(DR(DphiC(Dphi(Z.m_lep1.m_lvec.Phi(), J.m_lvec.Phi())), Deta(Z.m_lep0.m_lvec.Eta(), J.m_lvec.Eta())), w) ;
      h_deltaPhiAlgo_lep1 ->Fill(DphiC(Dphi(Z.m_lep1.m_lvec.Phi(), J.m_lvec.Phi())), w) ;
      h_deltaEtaAlgo_lep1 ->Fill(Deta(Z.m_lep1.m_lvec.Eta(), J.m_lvec.Eta()), w) ;
  
      h_pt_jet->Fill(J.m_lvec.Pt(), w) ;
      h_phi_jet->Fill(J.m_lvec.Phi(), w) ;
      h_eta_jet->Fill(J.m_lvec.Eta(), w) ;

      h_pt_dilepton->Fill((Z.m_lep0.m_lvec + Z.m_lep1.m_lvec).Pt(), w);
      h_phi_dilepton->Fill((Z.m_lep0.m_lvec + Z.m_lep1.m_lvec).Phi(), w);
      h_eta_dilepton->Fill((Z.m_lep0.m_lvec + Z.m_lep1.m_lvec).Eta(), w);

      h_mSV_jet->Fill(J.m_mSV, w);

    } ;

    void FillNjet(size_t nJet, float w) {
      h_Njet->Fill(nJet,w) ;
    } ;

    void FillMet(float met, float met_puppi, float w, float met_smeared) { 
      h_met->Fill(met,w);
      h_met_smeared->Fill(met_smeared,w);
      h_met_puppi->Fill(met_puppi,w);
    };
    
    std::vector<TH1*> returnHisto(){
      std::vector<TH1*> histolist ;
      
      histolist.push_back(h_pt_lep0) ;
      histolist.push_back(h_phi_lep0) ;
      histolist.push_back(h_eta_lep0) ; 
      histolist.push_back(h_deltaRroot_lep0) ;
      histolist.push_back(h_deltaPhiRoot_lep0) ;
      histolist.push_back(h_deltaEtaRoot_lep0 ) ;
      histolist.push_back(h_deltaRAlgo_lep0) ;
      histolist.push_back(h_deltaPhiAlgo_lep0) ;
      histolist.push_back(h_deltaEtaAlgo_lep0 ) ;
      histolist.push_back(h_pt_lep1) ;
      histolist.push_back(h_phi_lep1) ;
      histolist.push_back(h_eta_lep1) ;
      histolist.push_back(h_ZMass) ;
      histolist.push_back(h_deltaRroot_lep1) ;
      histolist.push_back(h_deltaPhiRoot_lep1) ;
      histolist.push_back(h_deltaEtaRoot_lep1 ) ;
      histolist.push_back(h_deltaRAlgo_lep1) ;
      histolist.push_back(h_deltaPhiAlgo_lep1) ;
      histolist.push_back(h_deltaEtaAlgo_lep1 ) ;
      histolist.push_back(h_pt_jet) ;
      histolist.push_back(h_phi_jet) ;
      histolist.push_back(h_eta_jet) ;   
      histolist.push_back(h_mSV_jet) ;
      histolist.push_back(h_Njet) ;
      histolist.push_back(h_pt_dilepton) ;
      histolist.push_back(h_phi_dilepton) ;
      histolist.push_back(h_eta_dilepton) ;
      histolist.push_back(h_met);
      histolist.push_back(h_met_smeared);
      histolist.push_back(h_met_puppi);
      
      return histolist ; 
    }

  protected:
    TString m_name ;
    TH1D* h_pt_lep0 ;
    TH1D* h_phi_lep0 ;
    TH1D* h_eta_lep0 ;
    TH1D* h_deltaRroot_lep0 ;
    TH1D* h_deltaPhiRoot_lep0 ;
    TH1D* h_deltaEtaRoot_lep0 ;
    TH1D* h_deltaRAlgo_lep0 ;
    TH1D* h_deltaPhiAlgo_lep0 ;
    TH1D* h_deltaEtaAlgo_lep0 ;
    TH1D* h_pt_lep1 ;
    TH1D* h_phi_lep1 ;
    TH1D* h_eta_lep1 ;
    TH1D* h_deltaRroot_lep1 ;
    TH1D* h_deltaPhiRoot_lep1 ;
    TH1D* h_deltaEtaRoot_lep1 ;
    TH1D* h_deltaRAlgo_lep1 ;
    TH1D* h_deltaPhiAlgo_lep1 ;
    TH1D* h_deltaEtaAlgo_lep1 ;
    TH1D* h_ZMass ;
    TH1D* h_pt_jet ;
    TH1D* h_phi_jet ;
    TH1D* h_eta_jet ;
    TH1D* h_Njet ;
    TH1D* h_pt_dilepton ;
    TH1D* h_phi_dilepton;
    TH1D* h_eta_dilepton ;
    TH1D* h_mSV_jet;
    TH1D* h_met;
    TH1D* h_met_smeared;
    TH1D* h_met_puppi;
} ;

class Z2bPlots : public ZbPlots{
  
  public: 
    Z2bPlots(TString name) : ZbPlots(name) {
      h_pt_jet1 = new TH1D(name + "_pt_jet1","",NBIN_PT_JET,X_PT_JET[0],X_PT_JET[1]) ;
      h_phi_jet1 = new TH1D(name + "_phi_jet1","",NBIN_PHI,X_PHI[0],X_PHI[1]) ;
      h_eta_jet1 = new TH1D(name + "_eta_jet1","",NBIN_ETA,X_ETA[0],X_ETA[1]) ;
      h_mSV_jet1 = new TH1D(name + "_mSV_jet1","",100,0,10) ;
      h_m_2b = new TH1D(name + "_m_2b","",NBIN_M_2B,X_M_2B[0],X_M_2B[1]) ;
      h_m_Z2b = new TH1D(name + "_m_Z2b","",NBIN_M_Z2B,X_M_Z2B[0],X_M_Z2B[1]) ;
      h_dR_2b = new TH1D(name + "_dR_2b","",NBIN_DR_2B,X_DR_2B[0],X_DR_2B[1]) ;
      h_dPhi_2b = new TH1D(name + "_dPhi_2b","",NBIN_DPHI_2B,X_DPHI_2B) ;
      h_dR_Zb = new TH1D(name + "_dR_Zb","",100,0,10) ;
      h_dPhi_Zb = new TH1D(name + "_dPhi_Zb","",60,0,TMath::Pi()) ;
      h_dR_Zb1 = new TH1D(name + "_dR_Zb1","",100,0,10) ;
      h_dPhi_Zb1 = new TH1D(name + "_dPhi_Zb1","",60,0,TMath::Pi()) ;
      h_dRmin_Z2b = new TH1D(name + "_dRmin_Z2b","",NBIN_DRMIN_Z2B,X_DRMIN_Z2B[0],X_DRMIN_Z2B[1]) ;
      h_dRmax_Z2b = new TH1D(name + "_dRmax_Z2b","",100,0,10) ;
      h_A_Z2b = new TH1D(name + "_A_Z2b","",NBIN_A_Z2B,X_A_Z2B[0],X_A_Z2B[1]) ;
      h_pt_Z2b = new TH1D(name + "_pt_Z2b","",300,0,300) ;

      h_pt_jet1->Sumw2() ;
      h_phi_jet1->Sumw2() ;
      h_eta_jet1->Sumw2() ;
      h_mSV_jet1->Sumw2();
      h_m_2b->Sumw2() ;
      h_m_Z2b->Sumw2() ;
      h_dR_2b->Sumw2() ;
      h_dPhi_2b->Sumw2() ;
      h_dR_Zb->Sumw2() ;
      h_dPhi_Zb->Sumw2() ;
      h_dR_Zb1->Sumw2() ;
      h_dPhi_Zb1->Sumw2() ;
      h_dRmin_Z2b->Sumw2() ;
      h_dRmax_Z2b->Sumw2() ;
      h_A_Z2b->Sumw2() ;
      h_pt_Z2b->Sumw2() ;
    } ;

    void Fill(ZObj& Z, JetObj& J, JetObj& J1, float w=1) {
      ZbPlots::Fill(Z, J, w) ;
      h_pt_jet1->Fill(J1.m_lvec.Pt(), w) ;
      h_eta_jet1->Fill(J1.m_lvec.Eta(), w) ;
      h_phi_jet1->Fill(J1.m_lvec.Phi(), w) ;
      h_mSV_jet1->Fill(J1.m_mSV, w);
      TLorentzVector lv_2b = J.m_lvec + J1.m_lvec ;
      h_m_2b->Fill(lv_2b.M(),w) ;
      TLorentzVector lv_Z2b = lv_2b + Z.m_lvec ;
      h_m_Z2b->Fill(lv_Z2b.M(),w) ;
      h_pt_Z2b->Fill(lv_Z2b.Pt(),w);
      h_dR_2b->Fill(J.m_lvec.DeltaR(J1.m_lvec),w) ;
      h_dPhi_2b->Fill(fabs(J.m_lvec.DeltaPhi(J1.m_lvec)),w) ;
      h_dR_Zb->Fill(Z.m_lvec.DeltaR(J.m_lvec),w) ;
      h_dPhi_Zb->Fill(fabs(Z.m_lvec.DeltaPhi(J.m_lvec)),w) ;
      h_dR_Zb1->Fill(Z.m_lvec.DeltaR(J1.m_lvec),w) ;
      h_dPhi_Zb1->Fill(fabs(Z.m_lvec.DeltaPhi(J1.m_lvec)),w) ;
      float dR = Z.m_lvec.DeltaR(J.m_lvec) ;
      float dR1 = Z.m_lvec.DeltaR(J1.m_lvec) ;
      float dRmin = (dR < dR1) ? dR : dR1 ;
      float dRmax = (dR > dR1) ? dR : dR1 ;
      float A_Z2b = (dRmax - dRmin)/(dRmax + dRmin) ;
      h_dRmin_Z2b->Fill(dRmin, w) ;
      h_dRmax_Z2b->Fill(dRmax, w) ;
      h_A_Z2b->Fill(A_Z2b, w) ;
    } ;

    //void FillMet(float met, float met_puppi, float w, float met_smeared=0) { 
    //  h_met->Fill(met,w);
    //  h_met_puppi->Fill(met_puppi,w);
    //  h_met_smeared->Fill(met_smeared,w);
    //};

    std::vector<TH1*> returnHisto(){
      std::vector<TH1*> histolist = ZbPlots::returnHisto() ;
      histolist.push_back(h_pt_jet1) ;
      histolist.push_back(h_eta_jet1) ;
      histolist.push_back(h_phi_jet1) ;
      histolist.push_back(h_mSV_jet1) ;
      histolist.push_back(h_m_2b) ;
      histolist.push_back(h_m_Z2b) ;
      histolist.push_back(h_dR_2b) ;
      histolist.push_back(h_dPhi_2b) ;
      histolist.push_back(h_dPhi_Zb) ;
      histolist.push_back(h_dPhi_Zb1) ;
      histolist.push_back(h_dR_Zb) ;
      histolist.push_back(h_dR_Zb1) ;
      histolist.push_back(h_dRmin_Z2b) ;
      histolist.push_back(h_dRmax_Z2b) ;
      histolist.push_back(h_A_Z2b) ;
      histolist.push_back(h_pt_Z2b);
      return histolist ;
    }

  protected:
    TH1D* h_pt_jet1 ;
    TH1D* h_phi_jet1 ;
    TH1D* h_eta_jet1 ;
    TH1D* h_mSV_jet1 ;
    TH1D* h_m_2b ;
    TH1D* h_m_Z2b ;
    TH1D* h_dR_2b ;
    TH1D* h_dPhi_2b ;
    TH1D* h_dPhi_Zb ;
    TH1D* h_dPhi_Zb1 ;
    TH1D* h_dR_Zb ;
    TH1D* h_dR_Zb1 ;
    TH1D* h_dRmin_Z2b ;
    TH1D* h_dRmax_Z2b ;
    TH1D* h_A_Z2b ;
    TH1D* h_pt_Z2b ;
} ;

class ElMu2bPlots {
  
  public: 
    ElMu2bPlots(TString name) {

      h_pt_ele = new TH1D(name + "_pt_ele","",200,0,200) ;
      h_phi_ele = new TH1D(name + "_phi_ele","",NBIN_PHI,X_PHI[0],X_PHI[1]) ;
      h_eta_ele = new TH1D(name + "_eta_ele","",NBIN_ETA,X_ETA[0],X_ETA[1]) ;
      h_pt_muon = new TH1D(name + "_pt_muon","",200,0,200) ;
      h_phi_muon = new TH1D(name + "_phi_muon","",NBIN_PHI,X_PHI[0],X_PHI[1]) ;
      h_eta_muon = new TH1D(name + "_eta_muon","",NBIN_ETA,X_ETA[0],X_ETA[1]) ;
      
      h_Zmass = new TH1D(name + "_ZMass","",300,0,300) ;
    
      h_pt_jet = new TH1D(name + "_pt_jet","",NBIN_PT_JET,X_PT_JET[0],X_PT_JET[1]) ;
      h_phi_jet = new TH1D(name + "_phi_jet","",NBIN_PHI,X_PHI[0],X_PHI[1]) ;
      h_eta_jet = new TH1D(name + "_eta_jet","",NBIN_ETA,X_ETA[0],X_ETA[1]) ;

      h_pt_jet1 = new TH1D(name + "_pt_jet1","",NBIN_PT_JET,X_PT_JET[0],X_PT_JET[1]) ;
      h_phi_jet1 = new TH1D(name + "_phi_jet1","",NBIN_PHI,X_PHI[0],X_PHI[1]) ;
      h_eta_jet1 = new TH1D(name + "_eta_jet1","",NBIN_ETA,X_ETA[0],X_ETA[1]) ;
      
      h_met = new TH1D(name + "_met","",500,0,500);
      h_met_smeared = new TH1D(name + "_met_smeared","",500,0,500);
      
      h_pt_ele->Sumw2();
      h_phi_ele->Sumw2();
      h_eta_ele->Sumw2();
      h_pt_muon->Sumw2();
      h_phi_muon->Sumw2();
      h_eta_muon->Sumw2();
      h_Zmass->Sumw2();
      h_pt_jet->Sumw2() ;
      h_phi_jet->Sumw2() ;
      h_eta_jet->Sumw2() ;
      h_pt_jet1->Sumw2() ;
      h_phi_jet1->Sumw2() ;
      h_eta_jet1->Sumw2() ;
      h_met->Sumw2();
      h_met_smeared->Sumw2();
    } ;

    void Fill(LepObj& ele, LepObj& muon, JetObj& J, JetObj& J1, float met, float w, float met_smeared) {
      h_pt_ele->Fill(ele.m_lvec.Pt(), w) ;
      h_eta_ele->Fill(ele.m_lvec.Eta(), w) ;
      h_phi_ele->Fill(ele.m_lvec.Phi(), w) ;
      h_pt_muon->Fill(muon.m_lvec.Pt(), w) ;
      h_eta_muon->Fill(muon.m_lvec.Eta(), w) ;
      h_phi_muon->Fill(muon.m_lvec.Phi(), w) ;
      h_Zmass->Fill((ele.m_lvec+muon.m_lvec).M(), w) ;
      h_pt_jet->Fill(J.m_lvec.Pt(), w) ;
      h_eta_jet->Fill(J.m_lvec.Eta(), w) ;
      h_phi_jet->Fill(J.m_lvec.Phi(), w) ;
      h_pt_jet1->Fill(J1.m_lvec.Pt(), w) ;
      h_eta_jet1->Fill(J1.m_lvec.Eta(), w) ;
      h_phi_jet1->Fill(J1.m_lvec.Phi(), w) ;
      h_met->Fill(met,w);
      h_met_smeared->Fill(met_smeared,w);
    } ;

    std::vector<TH1*> returnHisto(){
      std::vector<TH1*> histolist;
      histolist.push_back(h_pt_ele);
      histolist.push_back(h_phi_ele);
      histolist.push_back(h_eta_ele);
      histolist.push_back(h_pt_muon);
      histolist.push_back(h_phi_muon);
      histolist.push_back(h_eta_muon);
      histolist.push_back(h_Zmass);
      histolist.push_back(h_pt_jet) ;
      histolist.push_back(h_phi_jet) ;
      histolist.push_back(h_eta_jet) ;
      histolist.push_back(h_pt_jet1) ;
      histolist.push_back(h_phi_jet1) ;
      histolist.push_back(h_eta_jet1) ;
      histolist.push_back(h_met) ;
      histolist.push_back(h_met_smeared) ;

      return histolist ;
    }

  protected:
    TH1D* h_pt_ele;
    TH1D* h_phi_ele;
    TH1D* h_eta_ele;
    TH1D* h_pt_muon;
    TH1D* h_phi_muon;
    TH1D* h_eta_muon;
    TH1D* h_Zmass;
    TH1D* h_pt_jet ;
    TH1D* h_phi_jet ;
    TH1D* h_eta_jet ;
    TH1D* h_pt_jet1 ;
    TH1D* h_phi_jet1 ;
    TH1D* h_eta_jet1 ;
    TH1D* h_met;
    TH1D* h_met_smeared;
} ;

class EffPlots
{
  public:
    EffPlots(TString name, int nBins, float bins[]) : m_name(name) {
      h_pt_deno = new TH1D(name + "_pt_deno","", nBins, bins) ; 
      h_pt_num = new TH1D(name + "_pt_num","", nBins, bins) ;
      h_pt_deno->Sumw2() ;
      h_pt_num->Sumw2() ;
    } ;
    void Fill(float val, TString where, float w=1) {
      if (where == "deno") h_pt_deno->Fill(val, w) ;
      if (where == "num") h_pt_num->Fill(val, w) ;
    } ;
    std::vector<TH1*> returnHisto() {
      std::vector<TH1*> histolist ;
      histolist.push_back(h_pt_deno) ;
      histolist.push_back(h_pt_num) ;
      return histolist ;
    } ;
  private:
    TString m_name ;
    TH1D* h_pt_deno ;
    TH1D* h_pt_num ;
} ;

class UnfoldingPlots
{
  public:
    UnfoldingPlots(TString name) : m_name(name) {
     unsigned nBins_pt_rec_b(10) ;
     float xBins_pt_rec_b[nBins_pt_rec_b] = {30, 35, 40, 50, 60, 70, 90, 110, 130, 150, 200};
     unsigned nBins_pt_gen_b(6) ;
     float xBins_pt_gen_b[nBins_pt_gen_b] = {30, 40, 50, 70, 110, 150, 200};
     unsigned nBins_pt_Z_rec(10);
     float xBins_pt_Z_rec[nBins_pt_Z_rec] = {0,20,30,40,50,60,70,90,120,150,200};
     unsigned nBins_pt_Z_gen(6);
     float xBins_pt_Z_gen[nBins_pt_Z_gen] = {0,30,50,70,90,150,200};
     unsigned nBin_pt_b(170);
     float x_pt_b[2]={30,200};
     unsigned nBin_pt_Z(200);
     float x_pt_Z[2]={0,200};
     unsigned nBin_dR_2b(46);
     float x_dR_2b[2]={0.4,5};
     //unsigned nBin_dPhi_2b(60);
     //float x_dPhi_2b[2]={0,TMath::Pi()};
     unsigned nBin_dRmin_Z2b(50);
     float x_dRmin_Z2b[2]={0,5};
     unsigned nBin_A_Z2b = 100;
     float x_A_Z2b[2] = {0,1};
     unsigned nBin_m_2b = 480;
     float x_m_2b[2] = {20,500};
     unsigned nBin_m_Z2b = 450;
     float x_m_Z2b[2] = {150,600};

     //unsigned nBins_dR_rec_bb(14);
     //float xBins_dR_rec_bb[nBins_dR_rec_bb] = {0.4,0.8,1.2,1.6,2,2.4,2.8,3,3.2,3.4,3.6,4.0,4.4,5,6};
     //float xBins_dR_rec_bb[nBins_dR_rec_bb] = {0.4,1.0,1.6,2,2.4,2.8,3,3.2,3.4,3.6,4.0,4.4,5,6};
     //unsigned nBins_dR_gen_bb(14);
     //float xBins_dR_gen_bb[nBins_dR_gen_bb] = {0.4,1.2,2.0,2.4,2.8,3.2,3.4,3.6,4.4,5,6};
/*
     h_pt_b1_rec.push_back(new TH1D("pt_b1_rec_all_" + name, "", nBins_pt_rec_b, xBins_pt_rec_b));
     h_pt_b1_rec.push_back(new TH1D("pt_b1_rec_GenMatch_" + name, "", nBins_pt_rec_b, xBins_pt_rec_b));
     h_pt_b1_rec.push_back(new TH1D("pt_b1_rec_noGenMatch_" + name, "", nBins_pt_rec_b, xBins_pt_rec_b));
     h_pt_b1_rec.push_back(new TH1D("pt_b1_rec_UFOFGenMatch_" + name, "", nBins_pt_rec_b, xBins_pt_rec_b)); //rec match with underflow or overflow gen
     h_pt_b2_rec.push_back(new TH1D("pt_b2_rec_all_" + name, "", nBins_pt_rec_b, xBins_pt_rec_b));
     h_pt_b2_rec.push_back(new TH1D("pt_b2_rec_GenMatch_" + name, "", nBins_pt_rec_b, xBins_pt_rec_b));
     h_pt_b2_rec.push_back(new TH1D("pt_b2_rec_noGenMatch_" + name, "", nBins_pt_rec_b, xBins_pt_rec_b));
     h_pt_b2_rec.push_back(new TH1D("pt_b2_rec_UFOFGenMatch_" + name, "", nBins_pt_rec_b, xBins_pt_rec_b));
     h_pt_Z_rec.push_back(new TH1D("pt_Z_rec_all_" + name, "", nBins_pt_Z_rec, xBins_pt_Z_rec));
     h_pt_Z_rec.push_back(new TH1D("pt_Z_rec_GenMatch_" + name, "", nBins_pt_Z_rec, xBins_pt_Z_rec));
     h_pt_Z_rec.push_back(new TH1D("pt_Z_rec_noGenMatch_" + name, "", nBins_pt_Z_rec, xBins_pt_Z_rec));
     h_pt_Z_rec.push_back(new TH1D("pt_Z_rec_UFOFGenMatch_" + name, "", nBins_pt_Z_rec, xBins_pt_Z_rec));

     h_pt_b1_gen.push_back(new TH1D("pt_b1_gen_all_" + name, "", nBins_pt_gen_b, xBins_pt_gen_b));
     h_pt_b1_gen.push_back(new TH1D("pt_b1_gen_RecMatch_" + name, "", nBins_pt_gen_b, xBins_pt_gen_b));
     h_pt_b1_gen.push_back(new TH1D("pt_b1_gen_noRecMatch_" + name, "", nBins_pt_gen_b, xBins_pt_gen_b));
     h_pt_b1_gen.push_back(new TH1D("pt_b1_gen_UFOFRecMatch_" + name, "", nBins_pt_gen_b, xBins_pt_gen_b)); //gen match with underflow or overflow gen
     h_pt_b2_gen.push_back(new TH1D("pt_b2_gen_all_" + name, "", nBins_pt_gen_b, xBins_pt_gen_b));
     h_pt_b2_gen.push_back(new TH1D("pt_b2_gen_RecMatch_" + name, "", nBins_pt_gen_b, xBins_pt_gen_b));
     h_pt_b2_gen.push_back(new TH1D("pt_b2_gen_noRecMatch_" + name, "", nBins_pt_gen_b, xBins_pt_gen_b));
     h_pt_b2_gen.push_back(new TH1D("pt_b2_gen_UFOFRecMatch_" + name, "", nBins_pt_gen_b, xBins_pt_gen_b));
     h_pt_Z_gen.push_back(new TH1D("pt_Z_gen_all_" + name, "", nBins_pt_Z_gen, xBins_pt_Z_gen));
     h_pt_Z_gen.push_back(new TH1D("pt_Z_gen_RecMatch_" + name, "", nBins_pt_Z_gen, xBins_pt_Z_gen));
     h_pt_Z_gen.push_back(new TH1D("pt_Z_gen_noRecMatch_" + name, "", nBins_pt_Z_gen, xBins_pt_Z_gen));
     h_pt_Z_gen.push_back(new TH1D("pt_Z_gen_UFOFRecMatch_" + name, "", nBins_pt_Z_gen, xBins_pt_Z_gen));
     
     h_pt_b1_res = new TH2D("pt_b1_res_" + name, "", nBins_pt_gen_b, xBins_pt_gen_b, nBins_pt_rec_b, xBins_pt_rec_b);
     h_pt_b2_res = new TH2D("pt_b2_res_" + name, "", nBins_pt_gen_b, xBins_pt_gen_b, nBins_pt_rec_b, xBins_pt_rec_b);
     h_pt_Z_res = new TH2D("pt_Z_res_" + name, "", nBins_pt_Z_gen, xBins_pt_Z_gen, nBins_pt_Z_rec, xBins_pt_Z_rec);
*/
     h_pt_b1_rec.push_back(new TH1D("pt_b1_rec_all_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b1_rec.push_back(new TH1D("pt_b1_rec_GenMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b1_rec.push_back(new TH1D("pt_b1_rec_noGenMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b1_rec.push_back(new TH1D("pt_b1_rec_UFOFGenMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1])); //rec match with underflow or overflow gen
     h_pt_b2_rec.push_back(new TH1D("pt_b2_rec_all_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b2_rec.push_back(new TH1D("pt_b2_rec_GenMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b2_rec.push_back(new TH1D("pt_b2_rec_noGenMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b2_rec.push_back(new TH1D("pt_b2_rec_UFOFGenMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_Z_rec.push_back(new TH1D("pt_Z_rec_all_" + name, "", nBin_pt_Z,x_pt_Z[0],x_pt_Z[1]));
     h_pt_Z_rec.push_back(new TH1D("pt_Z_rec_GenMatch_" + name, "", nBin_pt_Z,x_pt_Z[0],x_pt_Z[1]));
     h_pt_Z_rec.push_back(new TH1D("pt_Z_rec_noGenMatch_" + name, "", nBin_pt_Z,x_pt_Z[0],x_pt_Z[1]));
     h_pt_Z_rec.push_back(new TH1D("pt_Z_rec_UFOFGenMatch_" + name, "", nBin_pt_Z,x_pt_Z[0],x_pt_Z[1]));
     h_dR_2b_rec.push_back(new TH1D("dR_2b_rec_all_" + name, "", nBin_dR_2b,x_dR_2b[0],x_dR_2b[1]));
     h_dR_2b_rec.push_back(new TH1D("dR_2b_rec_GenMatch_" + name, "", nBin_dR_2b,x_dR_2b[0],x_dR_2b[1]));
     h_dR_2b_rec.push_back(new TH1D("dR_2b_rec_noGenMatch_" + name, "", nBin_dR_2b,x_dR_2b[0],x_dR_2b[1]));
     h_dR_2b_rec.push_back(new TH1D("dR_2b_rec_UFOFGenMatch_" + name, "", nBin_dR_2b,x_dR_2b[0],x_dR_2b[1]));
     h_dPhi_2b_rec.push_back(new TH1D("dPhi_2b_rec_all_" + name, "", NBIN_DPHI_2B,X_DPHI_2B));
     h_dPhi_2b_rec.push_back(new TH1D("dPhi_2b_rec_GenMatch_" + name, "", NBIN_DPHI_2B,X_DPHI_2B));
     h_dPhi_2b_rec.push_back(new TH1D("dPhi_2b_rec_noGenMatch_" + name, "", NBIN_DPHI_2B,X_DPHI_2B));
     h_dPhi_2b_rec.push_back(new TH1D("dPhi_2b_rec_UFOFGenMatch_" + name, "", NBIN_DPHI_2B,X_DPHI_2B));
     h_dRmin_Z2b_rec.push_back(new TH1D("dRmin_Z2b_rec_all_" + name, "", nBin_dRmin_Z2b,x_dRmin_Z2b[0],x_dRmin_Z2b[1]));
     h_dRmin_Z2b_rec.push_back(new TH1D("dRmin_Z2b_rec_GenMatch_" + name, "", nBin_dRmin_Z2b,x_dRmin_Z2b[0],x_dRmin_Z2b[1]));
     h_dRmin_Z2b_rec.push_back(new TH1D("dRmin_Z2b_rec_noGenMatch_" + name, "", nBin_dRmin_Z2b,x_dRmin_Z2b[0],x_dRmin_Z2b[1]));
     h_dRmin_Z2b_rec.push_back(new TH1D("dRmin_Z2b_rec_UFOFGenMatch_" + name, "", nBin_dRmin_Z2b,x_dRmin_Z2b[0],x_dRmin_Z2b[1]));
     h_A_Z2b_rec.push_back(new TH1D("A_Z2b_rec_all_" + name, "", nBin_A_Z2b,x_A_Z2b[0],x_A_Z2b[1]));
     h_A_Z2b_rec.push_back(new TH1D("A_Z2b_rec_GenMatch_" + name, "", nBin_A_Z2b,x_A_Z2b[0],x_A_Z2b[1]));
     h_A_Z2b_rec.push_back(new TH1D("A_Z2b_rec_noGenMatch_" + name, "", nBin_A_Z2b,x_A_Z2b[0],x_A_Z2b[1]));
     h_A_Z2b_rec.push_back(new TH1D("A_Z2b_rec_UFOFGenMatch_" + name, "", nBin_A_Z2b,x_A_Z2b[0],x_A_Z2b[1]));
     h_m_2b_rec.push_back(new TH1D("m_2b_rec_all_" + name, "", nBin_m_2b,x_m_2b[0],x_m_2b[1]));
     h_m_2b_rec.push_back(new TH1D("m_2b_rec_GenMatch_" + name, "", nBin_m_2b,x_m_2b[0],x_m_2b[1]));
     h_m_2b_rec.push_back(new TH1D("m_2b_rec_noGenMatch_" + name, "", nBin_m_2b,x_m_2b[0],x_m_2b[1]));
     h_m_2b_rec.push_back(new TH1D("m_2b_rec_UFOFGenMatch_" + name, "", nBin_m_2b,x_m_2b[0],x_m_2b[1]));
     h_m_Z2b_rec.push_back(new TH1D("m_Z2b_rec_all_" + name, "", nBin_m_Z2b,x_m_Z2b[0],x_m_Z2b[1]));
     h_m_Z2b_rec.push_back(new TH1D("m_Z2b_rec_GenMatch_" + name, "", nBin_m_Z2b,x_m_Z2b[0],x_m_Z2b[1]));
     h_m_Z2b_rec.push_back(new TH1D("m_Z2b_rec_noGenMatch_" + name, "", nBin_m_Z2b,x_m_Z2b[0],x_m_Z2b[1]));
     h_m_Z2b_rec.push_back(new TH1D("m_Z2b_rec_UFOFGenMatch_" + name, "", nBin_m_Z2b,x_m_Z2b[0],x_m_Z2b[1]));

     h_pt_b1_gen.push_back(new TH1D("pt_b1_gen_all_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b1_gen.push_back(new TH1D("pt_b1_gen_RecMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b1_gen.push_back(new TH1D("pt_b1_gen_noRecMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b1_gen.push_back(new TH1D("pt_b1_gen_UFOFRecMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1])); //gen match with underflow or overflow gen
     h_pt_b2_gen.push_back(new TH1D("pt_b2_gen_all_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b2_gen.push_back(new TH1D("pt_b2_gen_RecMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b2_gen.push_back(new TH1D("pt_b2_gen_noRecMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_b2_gen.push_back(new TH1D("pt_b2_gen_UFOFRecMatch_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1]));
     h_pt_Z_gen.push_back(new TH1D("pt_Z_gen_all_" + name, "", nBin_pt_Z,x_pt_Z[0],x_pt_Z[1]));
     h_pt_Z_gen.push_back(new TH1D("pt_Z_gen_RecMatch_" + name, "", nBin_pt_Z,x_pt_Z[0],x_pt_Z[1]));
     h_pt_Z_gen.push_back(new TH1D("pt_Z_gen_noRecMatch_" + name, "", nBin_pt_Z,x_pt_Z[0],x_pt_Z[1]));
     h_pt_Z_gen.push_back(new TH1D("pt_Z_gen_UFOFRecMatch_" + name, "", nBin_pt_Z,x_pt_Z[0],x_pt_Z[1]));
     h_dR_2b_gen.push_back(new TH1D("dR_2b_gen_all_" + name, "", nBin_dR_2b,x_dR_2b[0],x_dR_2b[1]));
     h_dR_2b_gen.push_back(new TH1D("dR_2b_gen_RecMatch_" + name, "", nBin_dR_2b,x_dR_2b[0],x_dR_2b[1]));
     h_dR_2b_gen.push_back(new TH1D("dR_2b_gen_noRecMatch_" + name, "", nBin_dR_2b,x_dR_2b[0],x_dR_2b[1]));
     h_dR_2b_gen.push_back(new TH1D("dR_2b_gen_UFOFRecMatch_" + name, "", nBin_dR_2b,x_dR_2b[0],x_dR_2b[1]));
     h_dPhi_2b_gen.push_back(new TH1D("dPhi_2b_gen_all_" + name, "", NBIN_DPHI_2B,X_DPHI_2B));
     h_dPhi_2b_gen.push_back(new TH1D("dPhi_2b_gen_RecMatch_" + name, "", NBIN_DPHI_2B,X_DPHI_2B));
     h_dPhi_2b_gen.push_back(new TH1D("dPhi_2b_gen_noRecMatch_" + name, "", NBIN_DPHI_2B,X_DPHI_2B));
     h_dPhi_2b_gen.push_back(new TH1D("dPhi_2b_gen_UFOFRecMatch_" + name, "", NBIN_DPHI_2B,X_DPHI_2B));
     h_dRmin_Z2b_gen.push_back(new TH1D("dRmin_Z2b_gen_all_" + name, "", nBin_dRmin_Z2b,x_dRmin_Z2b[0],x_dRmin_Z2b[1]));
     h_dRmin_Z2b_gen.push_back(new TH1D("dRmin_Z2b_gen_RecMatch_" + name, "", nBin_dRmin_Z2b,x_dRmin_Z2b[0],x_dRmin_Z2b[1]));
     h_dRmin_Z2b_gen.push_back(new TH1D("dRmin_Z2b_gen_noRecMatch_" + name, "", nBin_dRmin_Z2b,x_dRmin_Z2b[0],x_dRmin_Z2b[1]));
     h_dRmin_Z2b_gen.push_back(new TH1D("dRmin_Z2b_gen_UFOFRecMatch_" + name, "", nBin_dRmin_Z2b,x_dRmin_Z2b[0],x_dRmin_Z2b[1]));
     h_A_Z2b_gen.push_back(new TH1D("A_Z2b_gen_all_" + name, "", nBin_A_Z2b,x_A_Z2b[0],x_A_Z2b[1]));
     h_A_Z2b_gen.push_back(new TH1D("A_Z2b_gen_RecMatch_" + name, "", nBin_A_Z2b,x_A_Z2b[0],x_A_Z2b[1]));
     h_A_Z2b_gen.push_back(new TH1D("A_Z2b_gen_noRecMatch_" + name, "", nBin_A_Z2b,x_A_Z2b[0],x_A_Z2b[1]));
     h_A_Z2b_gen.push_back(new TH1D("A_Z2b_gen_UFOFRecMatch_" + name, "", nBin_A_Z2b,x_A_Z2b[0],x_A_Z2b[1]));
     h_m_2b_gen.push_back(new TH1D("m_2b_gen_all_" + name, "", nBin_m_2b,x_m_2b[0],x_m_2b[1]));
     h_m_2b_gen.push_back(new TH1D("m_2b_gen_RecMatch_" + name, "", nBin_m_2b,x_m_2b[0],x_m_2b[1]));
     h_m_2b_gen.push_back(new TH1D("m_2b_gen_noRecMatch_" + name, "", nBin_m_2b,x_m_2b[0],x_m_2b[1]));
     h_m_2b_gen.push_back(new TH1D("m_2b_gen_UFOFRecMatch_" + name, "", nBin_m_2b,x_m_2b[0],x_m_2b[1]));
     h_m_Z2b_gen.push_back(new TH1D("m_Z2b_gen_all_" + name, "", nBin_m_Z2b,x_m_Z2b[0],x_m_Z2b[1]));
     h_m_Z2b_gen.push_back(new TH1D("m_Z2b_gen_RecMatch_" + name, "", nBin_m_Z2b,x_m_Z2b[0],x_m_Z2b[1]));
     h_m_Z2b_gen.push_back(new TH1D("m_Z2b_gen_noRecMatch_" + name, "", nBin_m_Z2b,x_m_Z2b[0],x_m_Z2b[1]));
     h_m_Z2b_gen.push_back(new TH1D("m_Z2b_gen_UFOFRecMatch_" + name, "", nBin_m_Z2b,x_m_Z2b[0],x_m_Z2b[1]));

     h_pt_b1_res = new TH2D("pt_b1_res_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1],nBin_pt_b,x_pt_b[0],x_pt_b[1]);
     h_pt_b2_res = new TH2D("pt_b2_res_" + name, "", nBin_pt_b,x_pt_b[0],x_pt_b[1],nBin_pt_b,x_pt_b[0],x_pt_b[1]);
     h_pt_Z_res = new TH2D("pt_Z_res_" + name, "", nBin_pt_Z,x_pt_Z[0],x_pt_Z[1], nBin_pt_Z,x_pt_Z[0],x_pt_Z[1]);
     h_dR_2b_res = new TH2D("dR_2b_res_" + name, "", nBin_dR_2b,x_dR_2b[0],x_dR_2b[1], nBin_dR_2b,x_dR_2b[0],x_dR_2b[1]);
     h_dPhi_2b_res = new TH2D("dPhi_2b_res_" + name, "", NBIN_DPHI_2B,X_DPHI_2B, NBIN_DPHI_2B,X_DPHI_2B);
     h_dRmin_Z2b_res = new TH2D("dRmin_Z2b_res_" + name, "", nBin_dRmin_Z2b,x_dRmin_Z2b[0],x_dRmin_Z2b[1], nBin_dRmin_Z2b,x_dRmin_Z2b[0],x_dRmin_Z2b[1]);
     h_A_Z2b_res = new TH2D("A_Z2b_res_" + name, "", nBin_A_Z2b,x_A_Z2b[0],x_A_Z2b[1], nBin_A_Z2b,x_A_Z2b[0],x_A_Z2b[1]);
     h_m_2b_res = new TH2D("m_2b_res_" + name, "", nBin_m_2b,x_m_2b[0],x_m_2b[1], nBin_m_2b,x_m_2b[0],x_m_2b[1]);
     h_m_Z2b_res = new TH2D("m_Z2b_res_" + name, "", nBin_m_Z2b,x_m_Z2b[0],x_m_Z2b[1], nBin_m_Z2b,x_m_Z2b[0],x_m_Z2b[1]);


     for(auto h : h_pt_b1_rec) h->Sumw2();
     for(auto h : h_pt_b2_rec) h->Sumw2();
     for(auto h : h_pt_Z_rec) h->Sumw2();
     for(auto h : h_dR_2b_rec) h->Sumw2();
     for(auto h : h_dPhi_2b_rec) h->Sumw2();
     for(auto h : h_dRmin_Z2b_rec) h->Sumw2();
     for(auto h : h_A_Z2b_rec) h->Sumw2();
     for(auto h : h_m_2b_rec) h->Sumw2();
     for(auto h : h_m_Z2b_rec) h->Sumw2();
     
     for(auto h : h_pt_b1_gen) h->Sumw2();
     for(auto h : h_pt_b2_gen) h->Sumw2();
     for(auto h : h_pt_Z_gen) h->Sumw2();
     for(auto h : h_dR_2b_gen) h->Sumw2();
     for(auto h : h_dPhi_2b_gen) h->Sumw2();
     for(auto h : h_dRmin_Z2b_gen) h->Sumw2();
     for(auto h : h_A_Z2b_gen) h->Sumw2();
     for(auto h : h_m_2b_gen) h->Sumw2();
     for(auto h : h_m_Z2b_gen) h->Sumw2();
     
     h_pt_b1_res->Sumw2();
     h_pt_b2_res->Sumw2();
     h_pt_Z_res->Sumw2();
     h_dR_2b_res->Sumw2();
     h_dPhi_2b_res->Sumw2();
     h_dRmin_Z2b_res->Sumw2();
     h_A_Z2b_res->Sumw2();
     h_m_2b_res->Sumw2();
     h_m_Z2b_res->Sumw2();

    }
    
    void Fill(TString type, TLorentzVector l1, TLorentzVector l2, TLorentzVector b1, TLorentzVector b2, float w) {
      int iRec = -1;
      int iGen = -1;
      if (type == "REC_ALL") iRec=0;
      if (type == "REC_GENMATCH") iRec=1; 
      if (type == "REC_NOGENMATCH") iRec=2;
      if (type == "GEN_ALL") iGen=0;
      if (type == "GEN_RECMATCH") iGen=1;
      if (type == "GEN_NORECMATCH") iGen=2;

      TLorentzVector Z = l1 + l2 ;
      float dR = Z.DeltaR(b1) ;
      float dR1 = Z.DeltaR(b2) ;
      float dRmin = (dR <= dR1) ? dR : dR1 ;
      float dRmax = (dR > dR1) ? dR : dR1 ;
      float A_Z2b = (dRmax - dRmin)/(dRmax + dRmin) ;
      float m2b = (b1+b2).M();
      float mZ2b = (Z+b1+b2).M();
      if (iRec != -1){
        h_pt_b1_rec[iRec]->Fill(b1.Pt(),w);
        h_pt_b2_rec[iRec]->Fill(b2.Pt(),w);
        h_pt_Z_rec[iRec]->Fill(Z.Pt(),w);
        h_dR_2b_rec[iRec]->Fill(b1.DeltaR(b2),w);
        h_dPhi_2b_rec[iRec]->Fill(fabs(b1.DeltaPhi(b2)),w);
        h_dRmin_Z2b_rec[iRec]->Fill(dRmin,w);
        h_A_Z2b_rec[iRec]->Fill(A_Z2b,w);
        h_m_2b_rec[iRec]->Fill(m2b,w);
        h_m_Z2b_rec[iRec]->Fill(mZ2b,w);
      }
      if (iGen!=-1){
        h_pt_b1_gen[iGen]->Fill(b1.Pt(),w) ;
        h_pt_b2_gen[iGen]->Fill(b2.Pt(),w) ;
        h_pt_Z_gen[iGen]->Fill(Z.Pt(),w) ;
        h_dR_2b_gen[iGen]->Fill(b1.DeltaR(b2),w);
        h_dPhi_2b_gen[iGen]->Fill(fabs(b1.DeltaPhi(b2)),w);
        h_dRmin_Z2b_gen[iGen]->Fill(dRmin,w);
        h_A_Z2b_gen[iGen]->Fill(A_Z2b,w);
        h_m_2b_gen[iGen]->Fill(m2b,w);
        h_m_Z2b_gen[iGen]->Fill(mZ2b,w);
      }

    }
    
    void FillUFOF(double y_rec, double x_gen, TH2D* hRes, TH1D* hRec, TH1D* hGen, float w_all, float gen_w) {
      //uf and of rec 
      float yaxis_L = hRes->GetYaxis()->GetBinLowEdge(1);
      float yaxis_H = hRes->GetYaxis()->GetBinLowEdge(hRes->GetNbinsY()+1);
      float xaxis_L = hRes->GetXaxis()->GetBinLowEdge(1);
      float xaxis_H = hRes->GetXaxis()->GetBinLowEdge(hRes->GetNbinsX()+1);
      if (y_rec < yaxis_L || y_rec >= yaxis_H) { //consider OF and UF as no rec found
        hGen->Fill(x_gen,gen_w);
      }
      if (x_gen < xaxis_L || x_gen >= xaxis_H) {
        hRec->Fill(y_rec,w_all);
      }
    }

    void FillRes(TLorentzVector rec_l1, TLorentzVector rec_l2, TLorentzVector rec_b1, TLorentzVector rec_b2, TLorentzVector gen_l1, TLorentzVector gen_l2, TLorentzVector gen_b1, TLorentzVector gen_b2, float w_all, float gen_w) {
      TLorentzVector rec_Z = rec_l1 + rec_l2;
      TLorentzVector gen_Z = gen_l1 + gen_l2;
      float rec_dR_2b = rec_b1.DeltaR(rec_b2);
      float rec_dPhi_2b = fabs(rec_b1.DeltaPhi(rec_b2));
      float rec_dR = rec_Z.DeltaR(rec_b1) ;
      float rec_dR1 = rec_Z.DeltaR(rec_b2) ;
      float rec_dRmin_Z2b = (rec_dR <= rec_dR1) ? rec_dR : rec_dR1 ;
      float rec_dRmax = (rec_dR > rec_dR1) ? rec_dR : rec_dR1 ;
      float rec_A_Z2b = (rec_dRmax - rec_dRmin_Z2b)/(rec_dRmax + rec_dRmin_Z2b) ;
      float rec_m_2b = (rec_b1+rec_b2).M();
      float rec_m_Z2b = (rec_Z+rec_b1+rec_b2).M();
      
      float gen_dR_2b = gen_b1.DeltaR(gen_b2);
      float gen_dPhi_2b = fabs(gen_b1.DeltaPhi(gen_b2));
      float gen_dR = gen_Z.DeltaR(gen_b1) ;
      float gen_dR1 = gen_Z.DeltaR(gen_b2) ;
      float gen_dRmin_Z2b = (gen_dR <= gen_dR1) ? gen_dR : gen_dR1 ;
      float gen_dRmax = (gen_dR > gen_dR1) ? gen_dR : gen_dR1 ;
      float gen_A_Z2b = (gen_dRmax - gen_dRmin_Z2b)/(gen_dRmax + gen_dRmin_Z2b) ;
      float gen_m_2b = (gen_b1+gen_b2).M();
      float gen_m_Z2b = (gen_Z+gen_b1+gen_b2).M();

      FillUFOF(rec_b1.Pt(),gen_b1.Pt(),h_pt_b1_res,h_pt_b1_rec[3],h_pt_b1_gen[3],w_all,gen_w); 
      FillUFOF(rec_b2.Pt(),gen_b2.Pt(),h_pt_b2_res,h_pt_b2_rec[3],h_pt_b2_gen[3],w_all,gen_w); 
      FillUFOF(rec_Z.Pt(),gen_Z.Pt(),h_pt_Z_res,h_pt_Z_rec[3],h_pt_Z_gen[3],w_all,gen_w); 
      FillUFOF(rec_dR_2b,gen_dR_2b,h_dR_2b_res,h_dR_2b_rec[3],h_dR_2b_gen[3],w_all,gen_w); 
      FillUFOF(rec_dPhi_2b,gen_dPhi_2b,h_dPhi_2b_res,h_dPhi_2b_rec[3],h_dPhi_2b_gen[3],w_all,gen_w); 
      FillUFOF(rec_dRmin_Z2b,gen_dRmin_Z2b,h_dRmin_Z2b_res,h_dRmin_Z2b_rec[3],h_dRmin_Z2b_gen[3],w_all,gen_w); 
      FillUFOF(rec_A_Z2b,gen_A_Z2b,h_A_Z2b_res,h_A_Z2b_rec[3],h_A_Z2b_gen[3],w_all,gen_w); 
      FillUFOF(rec_m_2b,gen_m_2b,h_m_2b_res,h_m_2b_rec[3],h_m_2b_gen[3],w_all,gen_w); 
      FillUFOF(rec_m_Z2b,gen_m_Z2b,h_m_Z2b_res,h_m_Z2b_rec[3],h_m_Z2b_gen[3],w_all,gen_w); 
      
      h_pt_b1_res->Fill(gen_b1.Pt(),rec_b1.Pt(),gen_w);
      h_pt_b2_res->Fill(gen_b2.Pt(),rec_b2.Pt(),gen_w);
      h_pt_Z_res->Fill(gen_Z.Pt(),rec_Z.Pt(),gen_w);
      h_dR_2b_res->Fill(gen_dR_2b,rec_dR_2b,gen_w);
      h_dPhi_2b_res->Fill(gen_dPhi_2b,rec_dPhi_2b,gen_w);
      h_dRmin_Z2b_res->Fill(gen_dRmin_Z2b,rec_dRmin_Z2b,gen_w);
      h_A_Z2b_res->Fill(gen_A_Z2b,rec_A_Z2b,gen_w);
      h_m_2b_res->Fill(gen_m_2b,rec_m_2b,gen_w);
      h_m_Z2b_res->Fill(gen_m_Z2b,rec_m_Z2b,gen_w);
    }

    std::vector<TH1*> returnHisto() {
      std::vector<TH1*> histolist;
      for (unsigned i = 0 ; i < h_pt_b1_rec.size() ; ++i) histolist.push_back(h_pt_b1_rec[i]);
      for (unsigned i = 0 ; i < h_pt_b2_rec.size() ; ++i) histolist.push_back(h_pt_b2_rec[i]);
      for (unsigned i = 0 ; i < h_pt_Z_rec.size() ; ++i) histolist.push_back(h_pt_Z_rec[i]);
      for (unsigned i = 0 ; i < h_dR_2b_rec.size() ; ++i) histolist.push_back(h_dR_2b_rec[i]);
      for (unsigned i = 0 ; i < h_dPhi_2b_rec.size() ; ++i) histolist.push_back(h_dPhi_2b_rec[i]);
      for (unsigned i = 0 ; i < h_dRmin_Z2b_rec.size() ; ++i) histolist.push_back(h_dRmin_Z2b_rec[i]);
      for (unsigned i = 0 ; i < h_A_Z2b_rec.size() ; ++i) histolist.push_back(h_A_Z2b_rec[i]);
      for (unsigned i = 0 ; i < h_m_2b_rec.size() ; ++i) histolist.push_back(h_m_2b_rec[i]);
      for (unsigned i = 0 ; i < h_m_Z2b_rec.size() ; ++i) histolist.push_back(h_m_Z2b_rec[i]);
      
      for (unsigned i = 0 ; i < h_pt_b1_gen.size() ; ++i) histolist.push_back(h_pt_b1_gen[i]);
      for (unsigned i = 0 ; i < h_pt_b2_gen.size() ; ++i) histolist.push_back(h_pt_b2_gen[i]);
      for (unsigned i = 0 ; i < h_pt_Z_gen.size() ; ++i) histolist.push_back(h_pt_Z_gen[i]);
      for (unsigned i = 0 ; i < h_dR_2b_gen.size() ; ++i) histolist.push_back(h_dR_2b_gen[i]);
      for (unsigned i = 0 ; i < h_dPhi_2b_gen.size() ; ++i) histolist.push_back(h_dPhi_2b_gen[i]);
      for (unsigned i = 0 ; i < h_dRmin_Z2b_gen.size() ; ++i) histolist.push_back(h_dRmin_Z2b_gen[i]);
      for (unsigned i = 0 ; i < h_A_Z2b_gen.size() ; ++i) histolist.push_back(h_A_Z2b_gen[i]);
      for (unsigned i = 0 ; i < h_m_2b_gen.size() ; ++i) histolist.push_back(h_m_2b_gen[i]);
      for (unsigned i = 0 ; i < h_m_Z2b_gen.size() ; ++i) histolist.push_back(h_m_Z2b_gen[i]);

      histolist.push_back(h_pt_b1_res);
      histolist.push_back(h_pt_b2_res);
      histolist.push_back(h_pt_Z_res);
      histolist.push_back(h_dR_2b_res);
      histolist.push_back(h_dPhi_2b_res);
      histolist.push_back(h_dRmin_Z2b_res);
      histolist.push_back(h_A_Z2b_res);
      histolist.push_back(h_m_2b_res);
      histolist.push_back(h_m_Z2b_res);
      return histolist;
    }

  private:
    TString m_name;
    //plots for background (x-axis = rec)
    std::vector<TH1D*> h_pt_b1_rec; //0: all rec, 1: rec not have matched gen
    std::vector<TH1D*> h_pt_b2_rec;
    std::vector<TH1D*> h_pt_Z_rec;
    std::vector<TH1D*> h_dR_2b_rec;
    std::vector<TH1D*> h_dPhi_2b_rec;
    std::vector<TH1D*> h_dRmin_Z2b_rec;
    std::vector<TH1D*> h_A_Z2b_rec;
    std::vector<TH1D*> h_m_2b_rec;
    std::vector<TH1D*> h_m_Z2b_rec;
    //plots for eff (x-axis = gen)
    std::vector<TH1D*> h_pt_b1_gen; //0: all gen, 1: gen not have matched reco
    std::vector<TH1D*> h_pt_b2_gen;
    std::vector<TH1D*> h_pt_Z_gen;
    std::vector<TH1D*> h_dR_2b_gen;
    std::vector<TH1D*> h_dPhi_2b_gen;
    std::vector<TH1D*> h_dRmin_Z2b_gen;
    std::vector<TH1D*> h_A_Z2b_gen;
    std::vector<TH1D*> h_m_2b_gen;
    std::vector<TH1D*> h_m_Z2b_gen;
    //plots for response matrix
    TH2D* h_pt_b1_res;
    TH2D* h_pt_b2_res;
    TH2D* h_pt_Z_res;
    TH2D* h_dR_2b_res;
    TH2D* h_dPhi_2b_res;
    TH2D* h_dRmin_Z2b_res;
    TH2D* h_A_Z2b_res;
    TH2D* h_m_2b_res;
    TH2D* h_m_Z2b_res;
} ;


#endif
