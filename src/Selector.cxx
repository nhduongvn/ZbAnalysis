#include "Selector.h"
#include "Global.h"
#include "math.h"

void Selector::Process(Reader* r) { 
} 

void Selector::SetRandom() {
  m_rand = new TRandom3() ;
}

void Selector::SetRochesterCorr(std::string fName_roc) {
  m_rc.init(fName_roc) ;
}

void Selector::SetLumiMaskFilter(std::string fName_lumiMaskFilter) {
  m_lumiFilter.Set(fName_lumiMaskFilter) ;
}

void Selector::SetBtagCalib(std::string csvFileName, std::string taggerName, std::string effFileName) {
  m_btagCal = BTagCalibration(taggerName, csvFileName) ;
  m_btagReader = BTagCalibrationReader(BTagEntry::OP_MEDIUM,  // operating point
                                       "central",            //central sys type
                                       {"up","down"});       //other sys type
  
  m_btagReader.load(m_btagCal,     // calibration instance
            BTagEntry::FLAV_B,    // btag flavour
            "comb") ;             // measurement type
  m_btagReader.load(m_btagCal,  
            BTagEntry::FLAV_C,    
            "comb") ;            
  m_btagReader.load(m_btagCal, 
            BTagEntry::FLAV_UDSG,
            "incl") ;           
  
  m_btagEffFile = new TFile(effFileName.c_str(),"READ") ;

}

void Selector::SetEleEffCorr(std::vector<std::string> fName_trig,std::string fName_recSF, std::string fName_IDSF, std::vector<float> w_trig) {
  std::string trigN("EGamma_SF2D");
  TFile* fRec = new TFile(fName_recSF.c_str(),"READ") ;
  TFile* fID = new TFile(fName_IDSF.c_str(),"READ") ;
  m_hSF_eleRec = (TH2F*)fRec->Get("EGamma_SF2D") ;
  m_hSF_eleID = (TH2F*)fID->Get("EGamma_SF2D") ;
  for(std::string fN : fName_trig) {
    TFile* f = new TFile(fN.c_str(),"READ");
    m_hSF_eleTrig.push_back((TH2F*)f->Get(trigN.c_str()));
    m_hSF_eleTrig.back()->SetDirectory(0);
  }

  for(float w : w_trig) m_eleTrig_w.push_back(w) ;
}

//multiple inputs to deal with different SFs for different run periods 
void Selector::SetMuonEffCorr(std::vector<std::string> fName_trig, std::vector<std::string> fName_ID, std::vector<std::string> fName_iso, std::vector<float> w_trig, std::vector<float> w_ID, std::vector<float> w_iso) {
  std::string trigN("IsoMu24_OR_IsoTkMu24_PtEtaBins/abseta_pt_ratio");
  std::string idN("NUM_MediumID_DEN_genTracks_eta_pt_syst");
  std::string isoN("NUM_LooseRelIso_DEN_MediumID_eta_pt_syst");//FIXME Loose iso?
  std::string isoN1("NUM_LooseRelIso_DEN_MediumID_eta_pt");//FIXME: only stat available for GH for 2016 legacy
#if defined(MC_2017)
  trigN = "IsoMu27_PtEtaBins/abseta_pt_ratio";
  idN = "NUM_MediumID_DEN_genTracks_pt_abseta_syst";
  isoN = "NUM_LooseRelIso_DEN_MediumID_pt_abseta_syst";
#endif
#if defined(MC_2018)
  trigN = "IsoMu24_PtEtaBins/abseta_pt_ratio";
  idN = "NUM_MediumID_DEN_TrackerMuons_pt_abseta_syst";
  isoN = "NUM_LooseRelIso_DEN_MediumID_pt_abseta_syst";
#endif
  for(std::string fN : fName_trig) {
    TFile* f = new TFile(fN.c_str(),"READ");
    m_hSF_muonTrig.push_back((TH2F*)f->Get(trigN.c_str()));
    m_hSF_muonTrig.back()->SetDirectory(0);
  }
  for(std::string fN : fName_ID) {
    TFile* f = new TFile(fN.c_str(),"READ");
    m_hSF_muonID.push_back((TH2F*)f->Get(idN.c_str()));
    m_hSF_muonID.back()->SetDirectory(0);
  }
  for(std::string fN : fName_iso) {
    TFile* f = new TFile(fN.c_str(),"READ");
    if (fN.find("RunGH") != std::string::npos) {
      m_hSF_muonIso.push_back((TH2F*)f->Get(isoN1.c_str()));
    }
    else {
      m_hSF_muonIso.push_back((TH2F*)f->Get(isoN.c_str()));
    }
    m_hSF_muonIso.back()->SetDirectory(0);
  }
  for(float w : w_trig) m_muonTrig_w.push_back(w) ;
  for(float w : w_iso) m_muonIso_w.push_back(w) ;
  for(float w : w_ID) m_muonID_w.push_back(w) ;
}

void Selector::SetPileupSF(std::string fName_puSF) {
  TFile* f = new TFile(fName_puSF.c_str(),"READ") ;
  m_hSF_pu = (TH1D*)f->Get("pileup_ratio") ;
  m_hSF_pu->SetDirectory(0);
}

float Selector::PileupSF(int nTrueInt) {
  int iBin = m_hSF_pu->FindFixBin(nTrueInt) ;
  return m_hSF_pu->GetBinContent(iBin); 
}

float Selector::CalBtagWeight(std::vector<JetObj>& jets, std::string jet_main_btagWP, std::string uncType) {
  //get calibration file
  std::string bN = "b_pt_eff_"+m_year;
  std::string cN = "c_pt_eff_"+m_year;
  std::string lN = "l_pt_eff_"+m_year;
  if (jet_main_btagWP.find("deepJet") != std::string::npos) {
    bN = "bdj_pt_eff_"+m_year;
    cN = "cdj_pt_eff_"+m_year;
    lN = "ldj_pt_eff_"+m_year;
  }
  TH1D* hEff_b = (TH1D*)m_btagEffFile->Get(bN.c_str());
  TH1D* hEff_c = (TH1D*)m_btagEffFile->Get(cN.c_str());
  TH1D* hEff_l = (TH1D*)m_btagEffFile->Get(lN.c_str());
  float pMC(1.);
  float pData(1.);
  for (std::vector<JetObj>::iterator jetIt = jets.begin() ; jetIt != jets.end() ; ++jetIt) {
    float jetPt = (jetIt->m_lvec).Pt() ;
    int iBin = hEff_b->FindFixBin(jetPt) ; //return overflow bin if jetPt > max pt range
    unsigned flav = jetIt->m_flav ;
    float eff = hEff_l->GetBinContent(iBin); //jet with pt > max pt range of efficinecy histogram will get the eff of overflow bins
    if (eff <= 0) std::cout << "\n Warning: Efficiency <=0, " << eff ; //we do not want eff = 0 
    BTagEntry::JetFlavor flavCode(BTagEntry::FLAV_UDSG) ;
    if (flav == 5) {
      eff = hEff_b->GetBinContent(iBin);
      flavCode = BTagEntry::FLAV_B;
    }
    if (flav == 4) {
      eff = hEff_c->GetBinContent(iBin);
      flavCode = BTagEntry::FLAV_C;
    }
    
    float sf = m_btagReader.eval_auto_bounds(
                 uncType, 
                 flavCode, 
                 fabs((jetIt->m_lvec).Eta()), // absolute value of eta
                 jetPt
    );
    //pass b-tagging requirement
    if (jetIt->m_deepCSV > CUTS.Get<float>("jet_"+jet_main_btagWP+"_" + m_year)) {
      pData = pData*sf*eff ;
      pMC = pMC*eff ;
    }
    else {
      pData = pData*(1-sf*eff) ;
      pMC = pMC*(1-eff) ;
    }
  } //end loop over jet 
  float sf(1.) ;
  if (pMC > 0) sf = pData/pMC ;
  return sf ;
}

//Get scale factors from a list of calibration histograms h (each histo corresponds to a run periods, for example muon in 2016 has scale factors for B->F and G->H sets. w are weights for each sets 
std::vector<float> Selector::GetSF_2DHist(float x, float y, std::vector<TH2F*> h, std::vector<float> w) {
  std::vector<float> o{1,0};
  unsigned nX = h[0]->GetNbinsX() ;
  unsigned nY = h[0]->GetNbinsY() ;
  unsigned iX = h[0]->GetXaxis()->FindFixBin(x) ;
  unsigned iY = h[0]->GetYaxis()->FindFixBin(y) ;
  if (iX == 0 || iY == 0 || iX > nX || iY > nY) { //underflow and overflow bins
    return o ;
  }
  float sf(0) ;
  float e_sf(0) ;
  for (unsigned i = 0 ; i < h.size() ; ++i) {
    sf += w[i]*h[i]->GetBinContent(iX,iY);
    e_sf += w[i]*w[i]*h[i]->GetBinError(iX,iY)*h[i]->GetBinError(iX,iY);
  }
  e_sf = sqrt(e_sf) ;
  o[0] = sf ;
  o[1] = e_sf ;
  return o ;
}

float Selector::CalEleSF(LepObj e1, LepObj e2) {
  std::vector<TH2F*> h{m_hSF_eleRec};
  std::vector<float> w{1.0};
  float sf = GetSF_2DHist(e1.m_lvec.Eta(),e1.m_lvec.Pt(),h,w)[0] ; 
  sf *= GetSF_2DHist(e2.m_lvec.Eta(),e2.m_lvec.Pt(),h,w)[0] ;
  h[0] = m_hSF_eleID;
  sf *= GetSF_2DHist(e1.m_lvec.Eta(),e1.m_lvec.Pt(),h,w)[0] ; 
  sf *= GetSF_2DHist(e2.m_lvec.Eta(),e2.m_lvec.Pt(),h,w)[0] ; 
  return sf ; 
}

float Selector::CalMuonSF_id_iso(LepObj e1, LepObj e2) {
  //id SFs
  float sf(1.0) ;
#ifdef MC_2016
  sf *= GetSF_2DHist(e1.m_lvec.Eta(),e1.m_lvec.Pt(), m_hSF_muonID, m_muonID_w)[0];
  sf *= GetSF_2DHist(e2.m_lvec.Eta(),e2.m_lvec.Pt(), m_hSF_muonID, m_muonID_w)[0];
  sf *= GetSF_2DHist(e1.m_lvec.Eta(),e1.m_lvec.Pt(), m_hSF_muonIso, m_muonIso_w)[0];
  sf *= GetSF_2DHist(e2.m_lvec.Eta(),e2.m_lvec.Pt(), m_hSF_muonIso, m_muonIso_w)[0];
#endif
#if defined(MC_2017) || defined(MC_2018)
  sf *= GetSF_2DHist(e1.m_lvec.Pt(),fabs(e1.m_lvec.Eta()), m_hSF_muonID, m_muonID_w)[0];
  sf *= GetSF_2DHist(e2.m_lvec.Pt(),fabs(e2.m_lvec.Eta()), m_hSF_muonID, m_muonID_w)[0];
  sf *= GetSF_2DHist(e1.m_lvec.Pt(),fabs(e1.m_lvec.Eta()), m_hSF_muonIso, m_muonIso_w)[0];
  sf *= GetSF_2DHist(e2.m_lvec.Pt(),fabs(e2.m_lvec.Eta()), m_hSF_muonIso, m_muonIso_w)[0];
#endif
  return sf ;
}

TLorentzVector Selector::GetTrigObj(Reader* r, unsigned id, unsigned bits, float ptThresh) { 

  int id_trigObj = -1 ;
  float maxPt = ptThresh ;

  for (unsigned i = 0 ; i < *(r->nTrigObj) ; ++i) {
    if ((abs(r->TrigObj_id[i]) == id) && ((r->TrigObj_filterBits)[i] & bits)) { //has correct id, bits
      //std::cout << "\n" << (r->TrigObj_id)[i] << " " << (r->TrigObj_filterBits)[i] << "  " << bits ;
      if ((r->TrigObj_pt)[i] > maxPt) { //choose maximum pt trigger object > threshold
        id_trigObj = i ;
        maxPt = (r->TrigObj_pt)[i] ;
      }
    }
  }
  
  TLorentzVector tl_out(0,0,0,0) ;
  float mass(0.1) ;
  if (id == 11) mass = 0.0005 ;
  if (id_trigObj>=0) tl_out.SetPtEtaPhiM((r->TrigObj_pt)[id_trigObj],(r->TrigObj_eta)[id_trigObj],(r->TrigObj_phi)[id_trigObj],mass) ;
  return tl_out ;
}

float Selector::CalTrigSF(int id, LepObj lep1, LepObj lep2, TLorentzVector trigObj, TH1D* h_dR1_trig, TH1D* h_dR2_trig, TH1D* h_pt1_trig, TH1D* h_pt2_trig) {
  
  float trigSF(1.0) ;
  if (trigObj.Pt() < 0.01) return trigSF ; //empty trigger object
  float dR1 = lep1.m_lvec.DeltaR(trigObj) ;
  float dR2 = lep2.m_lvec.DeltaR(trigObj) ;
  h_dR1_trig->Fill(dR1) ;
  h_dR2_trig->Fill(dR2) ;
  if ((dR1 < dR2) && (dR1 < 0.2)) {
    h_pt1_trig->Fill(lep1.m_lvec.Pt()) ;
    if (id == 13) {
      trigSF = GetSF_2DHist(fabs(lep1.m_lvec.Eta()),lep1.m_lvec.Pt(), m_hSF_muonTrig, m_muonTrig_w)[0] ;
    }
    if (id == 11) {
      //SC eta
      trigSF = GetSF_2DHist(lep1.m_scEta,lep1.m_lvec.Pt(), m_hSF_eleTrig, m_eleTrig_w)[0] ;
    }
  }    
  if ((dR2 < dR1) && (dR2 < 0.2)) {
    h_pt2_trig->Fill(lep2.m_lvec.Pt()) ;
    if (id == 13) {
      trigSF = GetSF_2DHist(fabs(lep2.m_lvec.Eta()),lep2.m_lvec.Pt(), m_hSF_muonTrig, m_muonTrig_w)[0] ;
    }
    if (id == 11) {
      trigSF = GetSF_2DHist(lep2.m_scEta,lep2.m_lvec.Pt(), m_hSF_eleTrig, m_eleTrig_w)[0] ; 
    }
  }
  
  return trigSF ;
}

#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
unsigned Selector::MatchGenLep(Reader* r, LepObj lep, int pdgId) {
  float dRmin(1000) ;
  int indO(-1) ;
  for (unsigned i = 0 ; i < *(r->nGenDressedLepton) ; ++i) {
    if (pdgId == fabs((r->GenDressedLepton_pdgId)[i])) {
      TLorentzVector gLep_lvec ;
      gLep_lvec.SetPtEtaPhiM((r->GenDressedLepton_pt)[i], (r->GenDressedLepton_eta)[i], (r->GenDressedLepton_phi)[i], (r->GenDressedLepton_mass)[i]) ;
      float dRtmp = lep.m_lvec.DeltaR(gLep_lvec) ;
      if (dRtmp < dRmin) {
        dRmin = dRmin ;
        indO = i ;
      }
    }
  }
  return indO ;
}

#endif

float Selector::MuonRcSF(Reader* r, LepObj lep, int pdgId) {
  float sf(1.) ;
  sf = m_rc.kScaleDT(lep.m_charge, lep.m_lvec.Pt(), lep.m_lvec.Eta(), lep.m_lvec.Phi());
#if defined(MC_2016) || defined(MC_2017) || defined(MC_2018)
  sf = 1 ;
  //int gLepInd = MatchGenLep(r, lep, 13) ;
  int gLepInd = r->Muon_genPartIdx[lep.m_idx] ;
  if(gLepInd >= 0) {
    sf = m_rc.kSpreadMC(lep.m_charge, lep.m_lvec.Pt(), lep.m_lvec.Eta(), lep.m_lvec.Phi(),(r->GenPart_pt[gLepInd]));
  }
  else {
    sf = m_rc.kSmearMC(lep.m_charge, lep.m_lvec.Pt(), lep.m_lvec.Eta(), lep.m_lvec.Phi(), (r->Muon_nTrackerLayers)[lep.m_idx], m_rand->Rndm());
  }
#endif
  return sf ;
}

