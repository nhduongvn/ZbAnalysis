TCanvas *hstack() {
THStack *hs = new THStack("hs","Stacked 1D histograms");
TString fmcname="out.root";
TFile* file = TFile::Open( fmcname );
/*
TH1D* Zee_jet_pt_lep0 = (TH1D*) file->Get( "Zee_jet_pt_lep0" );
TH1D* Zee_jet_pt_lep1 = (TH1D*) file->Get( "Zee_jet_pt_lep1" );

Zee_jet_pt_lep0->SetFillColor(kRed);
Zee_jet_pt_lep0->SetMarkerStyle(21);
Zee_jet_pt_lep0->SetMarkerColor(kRed);

Zee_jet_pt_lep1->SetFillColor(kBlue);
Zee_jet_pt_lep1->SetMarkerStyle(21);
Zee_jet_pt_lep1->SetMarkerColor(kBlue);


hs->Add(Zee_jet_pt_lep0);
hs->Add(Zee_jet_pt_lep1);
*/
TH1D* ele_pT_nocut = (TH1D*) file->Get( "ele_pT_nocut" );
TH1D* ele_pT_loose_MVA = (TH1D*) file->Get( "ele_pT_loose_MVA" );

ele_pT_nocut->SetFillColor(kRed);
ele_pT_nocut->SetMarkerStyle(21);
ele_pT_nocut->SetMarkerColor(kRed);

ele_pT_loose_MVA->SetFillColor(kBlue);
ele_pT_loose_MVA->SetMarkerStyle(21);
ele_pT_loose_MVA->SetMarkerColor(kBlue);


hs->Add(ele_pT_nocut);
hs->Add(ele_pT_loose_MVA);


TCanvas *cst = new TCanvas("cst","stacked hists",10,10,700,700);
cst->SetFillColor(41);
cst->Divide(2,2);
cst->cd(1);
hs->Draw();
cst->cd(2);
gPad->SetGrid();
hs->Draw("nostack,e1p");
return cst;


}
