
void setStyle();

void SetTotUnc(TH1D* h, TH1D* hSyst) {
  for (unsigned iB = 1; iB <= h->GetNbinsX();++iB) {
    float err = h->GetBinContent(iB)*hSyst->GetBinContent(iB);
    err *= err;
    float errStat = h->GetBinError(iB);
    errStat *= errStat;
    h->SetBinError(iB,sqrt(err));
  }
}

void DivideBinWidth(TH1D* h) {
  for (unsigned iB = 1 ; iB <= h->GetNbinsX()+1; ++iB) {
    float bW = h->GetXaxis()->GetBinWidth(iB);
    h->SetBinContent(iB, h->GetBinContent(iB)/bW);
    h->SetBinError(iB, h->GetBinError(iB)/bW);
  }
}

//pltType == "uf","uf_norm","rat"
void plotter_zbb_rat(string pltName = "pt_b1" , TString pltType = "uf", string chan = "Combined", string style = "paper") {
  
  //TString fname = "../Output_unfolding/unfolding_central_withNormDistributions.root";
  //TString fname_syst = "../Output_unfolding_postProc/syst_uncertainty_max.root";
  //TString fname_syst_theo = "../Output_unfolding/syst_uncertainty_uf_pdfScale.root";
  //if (pltType == "uf_norm") {
  //  fname_syst = "../Output_unfolding_postProc/syst_uncertainty_withNormDistributions_max.root";
  //  fname_syst_theo = "../Output_unfolding/syst_uncertainty_uf_norm_pdfScale.root";
  //}

  TString fname = "../Output_unfolding/unfolding_central_withNormDistributions.root";
  TString fname_sherpa = "../Output_unfolding/unfolding_sherpa.root"; //use for Sherpa with new weighting
  TString fname_syst = "../Output_unfolding/syst_all_uncertainty_max.root";
  TString fname_syst_theo = "../Output_unfolding/syst_uncertainty_uf_pdfScale.root";
  if (pltType == "uf_norm") {
    fname_syst = "../Output_unfolding/syst_uf_norm_all_uncertainty_max.root";
    //fname_syst = "../Output_unfolding_postProc/syst_uncertainty_withNormDistributions_max.root";
    fname_syst_theo = "../Output_unfolding/syst_uncertainty_uf_norm_pdfScale.root";
  }
  std::cout << "\n >>>>>>" << pltType << " " << fname_syst_theo;

  TString fname_theo = "../Output_theory_pred/theory_pred.root";

  TString fname_rat = "../Output_unfolding/ratio.root";

  if (pltName == "m_2b") {
    fname = "../Output_unfolding/unfolding_m2b.root";
    fname_syst = "../Output_unfolding/syst_m2b_all_uncertainty_max.root";
    fname_syst_theo = "../Output_unfolding/syst_uncertainty_uf_m2b_pdfScale.root";
    if (pltType == "uf_norm") {
      fname_syst_theo = "../Output_unfolding/syst_uncertainty_uf_norm_m2b_pdfScale.root";
      fname_syst = "../Output_unfolding/syst_uf_norm_m2b_all_uncertainty_max.root";
    }
    fname_theo = "../Output_theory_pred/theory_pred_m2b.root";
  }
  
  TFile* file = TFile::Open( fname );
  TFile* file_sherpa = TFile::Open( fname_sherpa );
  TFile* file_syst = TFile::Open( fname_syst );
  TFile* file_syst_theo = TFile::Open( fname_syst_theo );
  TFile* file_theo = TFile::Open( fname_theo );
  if (!file) {cout << fname << " not found!" << endl; return;}
  TFile* file_rat = TFile::Open(fname_rat,"READ");

  ////////////////////////////////
  ////
  ////////////////////////////////
  float Yx[2] = {0,0.7};
  TString Yt = "d#sigma/d#DeltaR_{bb} [pb]";
  TString Xt = "#DeltaR_{bb}";
  if (pltType == "uf_norm") {  
      Yx[0] = 0.;
      Yx[1] = 0.9;
      Yt = "1/#sigma #times d#sigma/d#DeltaR_{bb} [pb]";
  }

  if (pltName == "pt_b1") {
    Yt = "d#sigma/dp_{T} [pb/GeV]";
    Xt = "Leading b jet p_{T} [GeV]";
    Yx[0] = 0.;
    Yx[1] = 0.02;
    if (pltType == "uf_norm") {  
      Yx[0] = 0.0001;
      Yx[1] = 0.02;
      Yt = "1/#sigma #times d#sigma/dp_{T}";
    }
    if (pltType == "rat") {
      Yt = "#sigma(Z+#geq2 b jets)/#sigma(Z+#geq1 b jet)";
      Yx[0] = 0.0001;
      Yx[1] = 0.5;
    }
  }
  if (pltName == "pt_b2") {
    Yt = "d#sigma/dp_{T} [pb/GeV]";
    Xt = "Sub-leading b jet p_{T} [GeV]";
    Yx[0] = 0.;
    Yx[1] = 0.05;
    if (pltType == "uf_norm") {  
      Yx[0] = 0.;
      Yx[1] = 0.06;
      Yt = "1/#sigma #times d#sigma/dp_{T}";
    }
  }
  if (pltName == "pt_Z") {
    Yt = "d#sigma/dp_{T} [pb/GeV]";
    Xt = "Z boson p_{T} [GeV]";
    Yx[0] = 0.;
    Yx[1] = 0.015;
    if (pltType == "uf_norm") {  
      Yx[0] = 0.0;
      Yx[1] = 0.02;
      Yt = "1/#sigma #times d#sigma/dp_{T}";
    }
  }
  if (pltName == "dRmin_Z2b") {
    Yt = "d#sigma/d#DeltaR^{min}_{Zbb} [pb]";
    Xt = "#DeltaR^{min}_{Zbb}";
    if (pltType == "uf_norm") {  
      Yx[0] = 0.;
      Yx[1] = 0.9;
      Yt = "1/#sigma #times d#sigma/d#DeltaR^{min}_{Zbb}";
    }
  }
  if (pltName == "A_Z2b") {
    Yt = "d#sigma/dA_{Zbb} [pb]";
    Xt = "A_{Zbb}";
    Yx[0] = 0.;
    Yx[1] = 4.0;
   if (pltType == "uf_norm") {  
      Yx[0] = 0.0;
      Yx[1] = 5;
      Yt = "1/#sigma #times d#sigma/dA_{Zbb}";
    }

  }
  if (pltName == "m_2b") {
    Yt = "d#sigma/M_{bb} [pb/GeV]";
    Xt = "M_{bb} [GeV]";
    Yx[0] = 0.;
    Yx[1] = 0.01;
    if (pltType == "uf_norm") {  
      Yx[0] = 0.0;
      Yx[1] = 0.01;
      Yt = "1/#sigma #times d#sigma/M_{bb}";
    }

  }
  if (pltName == "m_Z2b") {
    Yt = "d#sigma/M_{Zbb} [pb/GeV]";
    Xt = "M_{Zbb} [GeV]";
    Yx[0] = 0.;
    Yx[1] = 0.007;
    if (pltType == "uf_norm") {  
      Yx[0] = 0.0;
      Yx[1] = 0.009;
      Yt = "1/#sigma #times d#sigma/M_{Zbb}";
    }
  }
  
  //if (pltType == "uf_norm") {  
  //    Yx[0] = 0.0;
  //   Yx[1] = 0.02;
  //    Yt = "1/#sigma #times d#sigma/dp_{T}";
  //}

  
  float YR[2] = {0.3,1.7};
 
  setStyle();
  TCanvas c("c", "c", 600, 800);
   
  TPad *topPad = new TPad("topPad", "topPad",0,0.47,1,1); //0.45
  topPad->Draw();
//topPad->SetTopMargin(0.000);
  topPad->SetBottomMargin(0.000);

  //TPad *bP1 = new TPad("bP1", "bP1",0,0.23,1,0.4);
  TPad *bP1 = new TPad("bP1", "bP1",0,0.34,1,0.47); //0.31
  bP1->Draw();
  bP1->SetTopMargin(0.03); //0.03
  bP1->SetBottomMargin(0.000);
  
  TPad *bP2 = new TPad("bP2", "bP2",0,0.21,1,0.34); //0.185
  bP2->Draw();
  bP2->SetTopMargin(0.00);
  bP2->SetBottomMargin(0.000);

  TPad *bP3 = new TPad("bP3", "bP3",0,0,1,0.21);
  bP3->Draw();
  bP3->SetTopMargin(0.00);
  bP3->SetBottomMargin(0.37); //

  //bP1->SetBottomMargin(0.35);

  //dR_2b_uf_amcatnlo_Combined
  TString tmp = pltName+"_"+pltType+"_amcatnlo_NONE_"+chan;
  //std::cout << "\n " << tmp;
  TH1D* h_uf = (TH1D*) file->Get(tmp);
  //pt_b1_NOM_rat_Combined
  if (pltType == "rat") {
    tmp = pltName + "_NOM_rat_"+chan;
    h_uf = (TH1D*) file_rat->Get(tmp);
  }
  std::cout << "\n " << h_uf->GetBinContent(1);
  TH1D* h_uf_totUnc = (TH1D*)h_uf->Clone("Data_"+pltType+"_totUnc");
  /////////////////////////////
  //total syst
  /////////////////////////////
  //m_Z2b_uf_amcatnlo_totUnc_Combined
  tmp = pltName+"_"+pltType+"_amcatnlo_totUnc_"+chan;
  TH1D* hSyst = (TH1D*) file_syst->Get(tmp);
  //pt_b1_totUnc_rat_Combined
  if (pltType == "rat") {
    tmp = pltName+"_totUnc_rat_"+chan;
    hSyst = (TH1D*)file_rat->Get(tmp);
  }
  std::cout << "\n " << tmp << " " << hSyst << " " << hSyst->GetNbinsX() << " " << h_uf_totUnc->GetNbinsX();
  hSyst->Multiply(h_uf_totUnc);
  //////////////////////////
  //theo syst
  //////////////////////////
  //pt_b1_uf_norm_amcatnlo_theoUnc_Combined
  tmp = pltName+"_"+pltType+"_amcatnlo_theoUnc_"+chan;
  TH1D* hSyst_theo = (TH1D*)file_syst_theo->Get(tmp);
  //std::cout << "\n >>>>>>>>>>" << " " << file_syst_theo->GetName() << " " << tmp<< " " << hSyst_theo << "  " << hSyst_theo->GetNbinsX() << " " << h_uf_totUnc->GetNbinsX(); 
  if (pltType == "rat") {
    //pt_b1_PDFSCALE_rat_Combined
    tmp = pltName+"_PDFSCALE_rat_"+chan;
    hSyst_theo = (TH1D*)file_rat->Get(tmp);
  }
  hSyst_theo->Multiply(h_uf_totUnc);
  
  if (pltType == "uf_norm") {
    DivideBinWidth(h_uf);
    DivideBinWidth(h_uf_totUnc);
    DivideBinWidth(hSyst);
    DivideBinWidth(hSyst_theo);
  }
  //normalization and calculate total unc = syst + stat
  for (unsigned iB=1;iB<=h_uf_totUnc->GetNbinsX();++iB) {
    float totUnc = h_uf_totUnc->GetBinError(iB)*h_uf_totUnc->GetBinError(iB) + hSyst->GetBinContent(iB)*hSyst->GetBinContent(iB);
    totUnc = sqrt(totUnc);
    h_uf_totUnc->SetBinError(iB,totUnc);
  }

  h_uf_totUnc->Print("all");
  h_uf->Print("all");

  tmp = (TString)h_uf->GetName()+"_1";
  TH1D* h_uf_1 = (TH1D*)h_uf_totUnc->Clone(tmp); //this has value of 1 and error as h_uf_totUnc error
  tmp = (TString)h_uf->GetName()+"_1_stat";
  TH1D* h_uf_1_stat = (TH1D*)h_uf->Clone(tmp); //this has value of 1 and error as h_uf, stat error
  tmp = (TString)h_uf->GetName()+"_line";
  TH1D* h_uf_l = (TH1D*)h_uf->Clone(tmp); //this has value of 1 use to plot the line
  tmp = (TString)h_uf->GetName()+"_theo";
  TH1D* h_uf_theo = (TH1D*)h_uf->Clone(tmp);
  for (unsigned iB = 1; iB <= h_uf_1->GetNbinsX();++iB) {
    h_uf_1->SetBinContent(iB,1);
    h_uf_1->SetBinError(iB,h_uf_totUnc->GetBinError(iB)/h_uf_totUnc->GetBinContent(iB));
    h_uf_1_stat->SetBinContent(iB,1);
    h_uf_1_stat->SetBinError(iB,h_uf->GetBinError(iB)/h_uf->GetBinContent(iB));
    h_uf_theo->SetBinContent(iB,1);
    h_uf_theo->SetBinError(iB,hSyst_theo->GetBinContent(iB)/h_uf->GetBinContent(iB));
    h_uf_l->SetBinContent(iB,1);
    h_uf_l->SetBinError(iB,0);
  }
  std::cout << "\n" << h_uf_1->GetBinError(1) << " " << h_uf->GetBinError(1);
  ////////////////////////////////
  //theorical prediction
  ////////////////////////////////
  //pt_b1_gen_amcnlo_Combined 
  tmp = pltName+"_gen_mg_NONE_"+chan;
  if (pltType == "uf_norm") tmp = pltName+"_gen_norm_mg_NONE_"+chan;
  //pt_b1_NOM_gen_mg_rat_Combined
  TH1D* h_mg = (TH1D*)file->Get(tmp); 
  if (pltType == "rat") {
    tmp = pltName+"_NOM_gen_mg_rat_"+chan;
    h_mg = (TH1D*)file_rat->Get(tmp);
  }
  tmp = pltName+"_gen_amcatnlo_NONE_"+chan;
  if (pltType == "uf_norm") tmp = pltName+"_gen_norm_amcatnlo_NONE_"+chan;
  TH1D* h_amcatnlo = (TH1D*)file->Get(tmp); 
  if (pltType == "rat") {
    //pt_b1_NOM_gen_amcatnlo_rat_Combined
    tmp = pltName+"_NOM_gen_amcatnlo_rat_"+chan;
    h_amcatnlo = (TH1D*)file_rat->Get(tmp);
  }
  tmp = pltName+"_gen_sherpa_NONE_"+chan;
  if (pltType == "uf_norm") tmp = pltName+"_gen_norm_sherpa_NONE_"+chan;
  //TEMP
  //TH1D* h_sherpa = (TH1D*)file_sherpa->Get(tmp); 
  TH1D* h_sherpa = (TH1D*)file->Get(tmp); 
  if (pltType == "rat") {
    //pt_b1_NOM_gen_sherpa_rat_Combined
    tmp = pltName+"_NOM_gen_sherpa_rat_"+chan;
    h_sherpa = (TH1D*)file_rat->Get(tmp);
  }
  TString nTmp = TString(h_mg->GetName())+"_stat";
  if (pltType == "uf_norm") {
    DivideBinWidth(h_mg);
    DivideBinWidth(h_amcatnlo);
    DivideBinWidth(h_sherpa);
  }
  TH1D* h_mg_stat = (TH1D*)h_mg->Clone(nTmp);
  nTmp = TString(h_amcatnlo->GetName())+"_stat";
  TH1D* h_amcatnlo_stat = (TH1D*)h_amcatnlo->Clone(nTmp);
  nTmp = TString(h_sherpa->GetName())+"_stat";
  TH1D* h_sherpa_stat = (TH1D*)h_sherpa->Clone(nTmp);
  
  /////////////////////////
  //uncertainties of theoretical Prediction
  ///////////////////////
  //pt_b1_gen_16_mg_Combined_theoUnc  
  tmp = pltName+"_gen_16_mg_"+chan+"_theoUnc";
  if (pltType == "uf_norm") tmp = pltName+"_norm_gen_16_mg_"+chan+"_theoUnc";
  TH1D* h_mg_theoUnc = (TH1D*)file_theo->Get(tmp);
  //set dummy error for mg_theoUnc for now
  if (pltType == "rat") {
    for (int iB = 1; iB <= h_mg_theoUnc->GetNbinsX();++iB) h_mg_theoUnc->SetBinContent(iB,0.0001);
  }
  tmp = pltName+"_gen_16_amcnlo_"+chan+"_theoUnc";
  if (pltType == "uf_norm") tmp = pltName+"_norm_gen_16_amcnlo_"+chan+"_theoUnc";
  TH1D* h_amcatnlo_theoUnc = (TH1D*)file_theo->Get(tmp); 
  if (pltType == "rat") {
    //pt_b1_PDFSCALE_gen_amcatnlo_rat_Combined
    tmp = pltName+"_PDFSCALE_gen_amcatnlo_rat_"+chan;
    h_amcatnlo_theoUnc = (TH1D*)file_rat->Get(tmp);
  }
  tmp = pltName+"_gen_16_sherpa_"+chan+"_theoUnc";
  if (pltType == "uf_norm") tmp = pltName+"_norm_gen_16_sherpa_"+chan+"_theoUnc";
  //TH1D* h_sherpa_theoUnc = (TH1D*)file_theo->Get(tmp); 
  //if(pltType == "uf_norm") {
  //  DivideBinWidth(h_mg_theoUnc);
  //  DivideBinWidth(h_amcatnlo_theoUnc);
    //DivideBinWidth(h_sherpa_theoUnc);
  //}
  SetTotUnc(h_mg,h_mg_theoUnc); //currently MG theorical uncertainties set = 0 later
  std::cout << "\n I am here"<<std::endl;
  SetTotUnc(h_amcatnlo,h_amcatnlo_theoUnc);
  h_amcatnlo_theoUnc->Print("all");
  //SetTotUnc(h_sherpa,h_sherpa_theoUnc);//not available currently
    /////////////////////////////////////
  //Get all ratios
  /////////////////////////////////////
  TH1D* h_r_amcatnlo = (TH1D*)h_amcatnlo->Clone("r_amcatnlo");
  TH1D* h_r_mg       = (TH1D*)h_mg->Clone("r_mg");
  TH1D* h_r_sherpa   = (TH1D*)h_sherpa->Clone("r_sherpa");
  TH1D* h_r_amcatnlo_stat = (TH1D*)h_amcatnlo_stat->Clone("r_amcatnlo_stat");
  TH1D* h_r_mg_stat       = (TH1D*)h_mg_stat->Clone("r_mg_stat");
  TH1D* h_r_sherpa_stat   = (TH1D*)h_sherpa_stat->Clone("r_sherpa_stat");
  //CONTINUE HERE
  for (unsigned iB = 1; iB <= h_uf->GetNbinsX();++iB) {
    h_mg->SetBinError(iB,0.00001); //dummy error
    if (h_uf->GetBinContent(iB)>0) {
      h_r_amcatnlo->SetBinContent(iB,h_amcatnlo->GetBinContent(iB)/h_uf->GetBinContent(iB));
      h_r_amcatnlo->SetBinError(iB,h_amcatnlo->GetBinError(iB)/h_uf->GetBinContent(iB));
      //std::cout << "\n iB: " << h_r_amcatnlo->GetBinContent(iB) << " " << h_r_amcatnlo->GetBinError(iB);
      h_r_mg->SetBinContent(iB,h_mg->GetBinContent(iB)/h_uf->GetBinContent(iB));
      h_r_mg->SetBinError(iB,h_mg->GetBinError(iB)/h_uf->GetBinContent(iB));
      h_r_sherpa->SetBinContent(iB,h_sherpa->GetBinContent(iB)/h_uf->GetBinContent(iB));
      h_r_sherpa->SetBinError(iB,h_sherpa->GetBinError(iB)/h_uf->GetBinContent(iB));
      h_r_amcatnlo_stat->SetBinContent(iB,h_amcatnlo_stat->GetBinContent(iB)/h_uf->GetBinContent(iB));
      //h_r_amcatnlo_stat->SetBinError(iB,h_amcatnlo_stat->GetBinError(iB)/h_uf->GetBinContent(iB));
      h_r_amcatnlo_stat->SetBinError(iB,0.01);//dummy error
      h_r_mg_stat->SetBinContent(iB,h_mg_stat->GetBinContent(iB)/h_uf->GetBinContent(iB));
      h_r_mg_stat->SetBinError(iB,h_mg_stat->GetBinError(iB)/h_uf->GetBinContent(iB));
      h_r_sherpa_stat->SetBinContent(iB,h_sherpa_stat->GetBinContent(iB)/h_uf->GetBinContent(iB));
      h_r_sherpa_stat->SetBinError(iB,h_sherpa_stat->GetBinError(iB)/h_uf->GetBinContent(iB));
      h_r_mg_stat->SetBinError(iB,0.01);//dummy error
      h_r_sherpa_stat->SetBinError(iB,0.01);//dummy error

    }
  }

  TLegend* leg ;
  //if (pltName == "pt_b2" || pltName == "A_Z2b") leg = new TLegend(.6,.7,.8,.9);
  //if (pltName == "pt_b1") leg = new TLegend(.48,.55,.8,.9);
  //else leg = new TLegend(.48,.55,.8,.9);
  //else leg = new TLegend(.2,.55,.5,.9);
  leg = new TLegend(.48,.55,.8,.9);

  leg->SetTextSize(0.04);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);

  topPad->cd() ;
  h_uf_totUnc->SetMarkerStyle(20);
  //h_uf_totUnc->SetFillColor(kOrange);
  //h_uf_totUnc->SetLineColor(kOrange);
  h_uf_totUnc->SetFillStyle(3354) ;
  h_uf_totUnc->SetFillColor(1) ;
  nTmp = (TString)h_uf_totUnc->GetName()+"_tmp"; 
  TH1D* h_uf_totUnc_tmp = (TH1D*)h_uf_totUnc->Clone(nTmp);
  for (unsigned iB = 1 ; iB <= h_uf_totUnc_tmp->GetNbinsX();++iB) {
    h_uf_totUnc_tmp->SetBinError(iB,0.00001);
    h_uf_totUnc->SetBinError(iB,0.00001); //Do not want to plot error
  }
  //h_uf_totUnc->Draw("E2"); //plot with total unc.
  h_uf_totUnc->Draw(); //plot with total unc.
  if (pltType != "rat" && pltName != "dR_2b" && pltName != "dRmin_Z2b" && pltName != "A_Z2b") h_uf_totUnc->GetYaxis()->SetMaxDigits(2);
  //h_uf_totUnc->Draw("PE2");
  //h_uf->Draw("same X0 E");
  h_uf_totUnc_tmp->SetMarkerColor(1) ;
  h_uf_totUnc_tmp->SetLineColor(1) ;
  h_uf_totUnc_tmp->Draw("same");
  //h_uf_totUnc_tmp->Draw();
  h_uf_totUnc_tmp->SetFillStyle(0) ;
  h_uf_totUnc->GetYaxis()->SetRangeUser(Yx[0],Yx[1]); //FIXME
  if (pltName == "pt_Z") h_uf_totUnc->GetXaxis()->SetRangeUser(0,200);
  h_uf_totUnc->GetYaxis()->SetTitle(Yt);
  h_uf_totUnc->GetYaxis()->SetLabelSize(0.055);
  h_uf_totUnc->GetYaxis()->SetTitleSize(0.06);
  h_uf_totUnc->GetYaxis()->SetTitleOffset(1.1); //0.12


   for (unsigned i = 1 ; i <= h_amcatnlo->GetNbinsX() ;++i) h_amcatnlo->SetBinError(i,0.00001);
   float alpha = 0.3 ;
   h_amcatnlo->SetMarkerStyle(25) ;
   //h_amcatnlo->SetMarkerSize(1.5) ;
   h_amcatnlo->SetMarkerColor(kBlue) ;
   h_amcatnlo->SetLineColor(kBlue) ;
   h_amcatnlo->SetFillColorAlpha(kBlue-4,alpha) ;
   //h_amcatnlo->Draw("same PE2") ;
   h_amcatnlo->Draw("same") ;

  //mgRats[0]->SetMarkerStyle(22) ;
   for (unsigned i = 1 ; i <= h_mg->GetNbinsX() ;++i) h_mg->SetBinError(i,0.00001);
   h_mg->SetMarkerStyle(26) ;
   h_mg->SetMarkerColor(kGreen+2) ;
   h_mg->SetLineColor(kGreen+2) ;
   h_mg->SetFillColorAlpha(kGreen-1,alpha) ; // -7
   //h_mg->Draw("same PE2") ; //PZ
   h_mg->Draw("same") ; //PZ
   
   h_sherpa->SetMarkerStyle(27) ;
   h_sherpa->SetMarkerSize(1.4) ;
   h_sherpa->SetMarkerColor(kMagenta) ;
   h_sherpa->SetLineColor(kMagenta) ;
   h_sherpa->SetFillColorAlpha(kMagenta-4,alpha) ;
   //h_sherpa->Draw("same PE2") ;
   h_sherpa->Draw("same") ;
   
   TLatex text;
    text.SetNDC();
 
    text.SetTextColor(kBlack);
    text.SetTextSize(0.06); //0.05
    text.SetTextFont(61);
    //text.DrawLatex(0.14, 0.956, "CMS");
    //text.DrawLatex(0.23, 0.956, "CMS");
    text.DrawLatex(0.2, 0.83, "CMS");
    if (style == "pas") {
      text.SetTextFont(42);
      text.SetTextSize(0.045); //0.045
      text.DrawLatex(0.22, 0.956, "#it{Preliminary}");
    }

    text.SetTextFont(42);
    text.SetTextSize(0.05); //0.045
    text.DrawLatex(0.72,0.956,"137.1 fb^{#minus1} (13 TeV)"); //0.74
    
    
    //leg->AddEntry(h_uf_totUnc, "Data", "PELF");
    leg->AddEntry(h_uf_totUnc, "Data", "PL");
    //leg->AddEntry(h_amcatnlo, "MG5_aMC [NLO, FxFx]", "P");
    //leg->AddEntry(h_mg, "MG5_aMC [LO, MLM]", "P");
    //leg->AddEntry(h_sherpa, "Sherpa", "P");
    leg->AddEntry(h_amcatnlo, "MG5_aMC + PY8 (#leq 2j NLO + PS)", "PL");
    leg->AddEntry(h_mg, "MG5_aMC + PY8 (#leq 4j LO + PS)", "PL");
    leg->AddEntry(h_sherpa, "Sherpa", "PL");
    TH1D* h_uf_totUnc_tmp1 = (TH1D*)h_uf_totUnc->Clone("uf_totUnc_tmp1"); //just to make legend
    leg->AddEntry(h_uf_totUnc_tmp1, "Total unc.", "f");
    leg->AddEntry(h_uf_theo, "Theoretical syst unc.", "f");
    TH1D* h_uf_totUnc_tmp2 = (TH1D*)h_uf_totUnc->Clone("uf_totUnc_tmp2"); //just to make legend
    leg->AddEntry(h_uf_totUnc_tmp2, "Statistical unc.", "PE");
    std::cout << "\n Error: " << h_uf_totUnc_tmp2->GetBinError(1);
    leg->Draw();

    
    topPad->Modified() ;

   /////////////////////////////////////////////////////////
   bP1->cd() ;
   
   //histogram to plot axis
   TH1D* hA = (TH1D*)h_uf->Clone("hA") ;
   hA->Reset() ;
   //hA->Draw() ;
   hA->GetYaxis()->SetRangeUser(YR[0],YR[1]) ;//0.1, 1.9
   if (pltName == "pt_Z") hA->GetXaxis()->SetRangeUser(0,200);
   hA->GetYaxis()->SetNdivisions(505) ;
   hA->GetYaxis()->SetTitleSize(0.21) ; //0.15
   hA->GetYaxis()->SetLabelSize(0.21) ; //0.15
   hA->GetYaxis()->SetTitle("") ;
   hA->GetYaxis()->SetTitleOffset(0.3) ;
   hA->GetYaxis()->CenterTitle() ;
   //double pad2H = bP1->GetWh()*bP1->GetAbsHNDC();
   //hA->GetXaxis()->SetTickLength(6/pad2H) ;
   
  h_uf_theo->Draw("same E2");
   h_uf_theo->GetYaxis()->SetRangeUser(YR[0],YR[1]) ;//0.1, 1.9
   if (pltName == "pt_Z") h_uf_theo->GetXaxis()->SetRangeUser(0,200);
   h_uf_theo->GetYaxis()->SetNdivisions(505) ;
   h_uf_theo->GetYaxis()->SetTitleSize(0.21) ; //0.15
   h_uf_theo->GetYaxis()->SetLabelSize(0.21) ; //0.15
   h_uf_theo->GetYaxis()->SetTitle("") ;
   h_uf_theo->GetYaxis()->SetTitleOffset(0.3) ;
   h_uf_theo->GetYaxis()->CenterTitle() ;
   double pad2H = bP1->GetWh()*bP1->GetAbsHNDC();
   h_uf_theo->GetXaxis()->SetTickLength(6/pad2H) ;
  //h_uf_theo->SetLineColor(kYellow);
  //h_uf_theo->SetLineColor(kBlack);
  //h_uf_theo->SetLineWidth(2);
  //h_uf_1->SetFillColor(kOrange);
  //h_uf_theo->SetFillStyle(3354) ;
  //h_uf_theo->SetFillColorAlpha(kYellow+1,alpha+0.3);
  h_uf_theo->SetFillColorAlpha(91,0.65);


   //h_uf_l->Draw("hist same");
   //h_uf_l->SetLineStyle(1);
   h_uf_1->SetMarkerStyle(20);
  h_uf_1->SetLineColor(kBlack);
  h_uf_1->SetLineWidth(2);
  //h_uf_1->SetFillColor(kOrange);
  h_uf_1->SetFillColor(1);
  h_uf_1->SetFillStyle(3354) ;
  h_uf_1->Draw("same E2");

  h_uf_1_stat->Draw("same E");
  
   //h_r_amcatnlo->SetMarkerStyle(25) ;
   //h_r_amcatnlo->SetMarkerSize(0) ;
   //h_r_amcatnlo->SetMarkerColor(kBlue) ;
   //h_r_amcatnlo->SetLineColor(kBlue) ;
   //h_r_amcatnlo->SetLineWidth(2);
   h_r_amcatnlo->SetFillColorAlpha(kBlue,alpha) ; //-4
   h_r_amcatnlo->Draw("same E2") ;
   std::cout << "\n h_r: "<<h_r_amcatnlo->GetBinContent(1) << h_r_amcatnlo->GetBinError(1); 
   
   h_r_amcatnlo_stat->SetMarkerStyle(25) ;
   //h_r_amcatnlo->SetMarkerSize(2) ;
   //h_r_amcatnlo_stat->SetLineColor(kBlue) ;
   h_r_amcatnlo_stat->SetMarkerColor(kBlue) ;
   h_r_amcatnlo_stat->SetFillColorAlpha(kBlue,alpha+0.5) ;
   //h_r_amcatnlo_stat->SetFillColor(kBlue-4) ;
   h_r_amcatnlo_stat->Draw("same E2") ;
   //h_r_amcatnlo_stat->Draw("same E") ;
   
   //leg = new TLegend(.2,.1,.32,.3);
   leg = new TLegend(.2,.1,.42,.3);
   leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
   leg->AddEntry(h_r_amcatnlo,"PDF #oplus scales","f");
   //leg->AddEntry(h_r_amcatnlo_stat,"Stat.","f");
   leg->Draw();
  leg = new TLegend(.32,.1,.44,.3);
   leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
   leg->AddEntry(h_r_amcatnlo,"#oplus theo.","f");

   //leg->Draw();


   bP1->Modified() ;

   //////////////////////////////////////////////////////////////////////
   bP2->cd() ;
   
   //histogram to plot axis
   TH1D* hA1 = (TH1D*)h_uf->Clone("hA1") ;
   hA1->Reset() ;
   //hA1->Draw() ;
   //hA1->GetYaxis()->SetTitle("Pred./Data") ;
   hA1->GetYaxis()->SetRangeUser(YR[0],YR[1]) ;
   if (pltName == "pt_Z") hA1->GetXaxis()->SetRangeUser(0,200);
   hA1->GetYaxis()->SetNdivisions(505) ;
   hA1->GetYaxis()->SetTitleSize(0.21) ; //0.15
   hA1->GetYaxis()->SetLabelSize(0.21) ; //0.15
   hA1->GetYaxis()->SetTitleOffset(0.3) ;
   hA1->GetYaxis()->CenterTitle() ;
   pad2H = bP2->GetWh()*bP2->GetAbsHNDC();
   hA1->GetXaxis()->SetTickLength(6/pad2H) ;
   
   TH1D* h_uf_theo_p1 = (TH1D*)h_uf_theo->Clone("uf_theo_p1");
   h_uf_theo_p1->Draw("same E2");
  //h_uf_theo->SetLineColor(kYellow);
  //h_uf_theo->SetLineWidth(2);
  //h_uf_1->SetFillColor(kOrange);
  //h_uf_theo->SetFillStyle(3354) ;
  //h_uf_theo->SetFillColorAlpha(kYellow+1,alpha+0.3);
  h_uf_theo_p1->SetFillColorAlpha(91,0.65);
   h_uf_theo_p1->GetYaxis()->SetTitle("Pred./Data") ;
   h_uf_theo_p1->GetYaxis()->SetRangeUser(YR[0],YR[1]) ;
   if (pltName == "pt_Z") h_uf_theo_p1->GetXaxis()->SetRangeUser(0,200);
   h_uf_theo_p1->GetYaxis()->SetNdivisions(505) ;
   h_uf_theo_p1->GetYaxis()->SetTitleSize(0.21) ; //0.15
   h_uf_theo_p1->GetYaxis()->SetLabelSize(0.21) ; //0.15
   h_uf_theo_p1->GetYaxis()->SetTitleOffset(0.3) ;
   h_uf_theo_p1->GetYaxis()->CenterTitle() ;
   pad2H = bP2->GetWh()*bP2->GetAbsHNDC();
   h_uf_theo_p1->GetXaxis()->SetTickLength(6/pad2H) ;

   //h_uf_l->Draw("hist same");
   h_uf_l->SetLineStyle(1);
   h_uf_1->SetMarkerStyle(20);
  h_uf_1->SetLineColor(kBlack);
  h_uf_1->SetLineWidth(2);
  //h_uf_1->SetFillColor(kOrange);
  h_uf_1->SetFillColor(1);
  h_uf_1->SetFillStyle(3354) ;
  h_uf_1->Draw("same E2");
  h_uf_1_stat->Draw("same E");
  
 //h_r_mg->SetMarkerStyle(26) ;
 //  h_r_mg->SetMarkerColor(kGreen+2) ;
 //  h_r_mg->SetLineColor(kGreen+2) ;
   h_r_mg->SetFillColorAlpha(kGreen+2,alpha+0.5) ; // -1 -7
   //h_r_mg->Draw("same PE2") ; //PZ
   h_r_mg_stat->SetMarkerStyle(26) ;
   h_r_mg_stat->SetMarkerColor(kGreen+2) ;
   //h_r_mg_stat->SetLineColor(kGreen+2) ;
   h_r_mg_stat->SetFillColorAlpha(kGreen+2,alpha+0.5) ; // -7
   h_r_mg_stat->Draw("same E2") ; //PZ

   leg = new TLegend(.2,.1,.32,.3);
   leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
   leg->AddEntry(h_r_mg_stat,"Stat.","f");
   //leg->Draw();
  leg = new TLegend(.32,.1,.44,.3);
   leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
   leg->AddEntry(h_r_mg,"#oplus theo.","f");

   //leg->Draw();

    //text.SetTextFont(42);
    //text.SetTextSize(0.5); //0.045
    //text.SetTextAngle(180);
    //text.DrawLatex(0.1,0.2,"13 TeV"); //0.74

   bP3->cd() ;
   
   //histogram to plot axis
   TH1D* hA3 = (TH1D*)h_uf->Clone("hA3") ;
   hA3->Reset();
   //hA3->Draw() ;
   hA3->GetYaxis()->SetTitle("");
   hA3->GetYaxis()->SetRangeUser(YR[0],YR[1]) ;
   if (pltName == "pt_Z") hA3->GetXaxis()->SetRangeUser(0,200);
   hA3->GetYaxis()->SetNdivisions(505) ;
   hA3->GetYaxis()->SetTitleSize(0.12) ; //0.15
   hA3->GetYaxis()->SetLabelSize(0.12) ; //0.15
   hA3->GetYaxis()->SetTitleOffset(0.3) ;
   hA3->GetYaxis()->CenterTitle() ;
   pad2H = bP3->GetWh()*bP3->GetAbsHNDC();
   hA3->GetXaxis()->SetTitle(Xt) ;
   hA3->GetXaxis()->SetTickLength(6/pad2H) ;
   hA3->GetXaxis()->SetLabelSize(0.12) ; //0.15
   hA3->GetXaxis()->SetTitleSize(0.12) ; //0.15
 TH1D* h_uf_theo_p2 = (TH1D*)h_uf_theo->Clone("uf_theo_p2"); 
 h_uf_theo_p2->Draw("same E2");
  //h_uf_theo->SetLineColor(kYellow);
  //h_uf_theo->SetLineWidth(2);
  //h_uf_1->SetFillColor(kOrange);
  //h_uf_theo->SetFillStyle(3354) ;
  //h_uf_theo->SetFillColorAlpha(kYellow+1,alpha+0.3);
  h_uf_theo_p2->SetFillColorAlpha(91,0.65);

   h_uf_theo_p2->GetYaxis()->SetTitle("");
   h_uf_theo_p2->GetYaxis()->SetRangeUser(YR[0],YR[1]) ;
   if (pltName == "pt_Z") h_uf_theo_p2->GetXaxis()->SetRangeUser(0,200);
   h_uf_theo_p2->GetYaxis()->SetNdivisions(505) ;
   h_uf_theo_p2->GetYaxis()->SetTitleSize(0.13) ; //0.15
   h_uf_theo_p2->GetYaxis()->SetLabelSize(0.13) ; //0.15
   h_uf_theo_p2->GetYaxis()->SetTitleOffset(0.3) ;
   h_uf_theo_p2->GetYaxis()->CenterTitle() ;
   pad2H = bP3->GetWh()*bP3->GetAbsHNDC();
   h_uf_theo_p2->GetXaxis()->SetTitle(Xt) ;
   h_uf_theo_p2->GetXaxis()->SetTickLength(6/pad2H) ;
   h_uf_theo_p2->GetXaxis()->SetLabelSize(0.13) ; //0.15
   h_uf_theo_p2->GetXaxis()->SetTitleSize(0.13) ; //0.15

   //h_uf_l->Draw("hist same");
   h_uf_l->SetLineStyle(1);
   h_uf_1->SetMarkerStyle(20);
  h_uf_1->SetLineColor(kBlack);
  h_uf_1->SetLineWidth(2);
  //h_uf_1->SetFillColor(kOrange);
  h_uf_1->SetFillColor(1);
  h_uf_1->SetFillStyle(3354) ;
  h_uf_1->Draw("same E2");
  h_uf_1_stat->Draw("same E");
  h_r_sherpa_stat->SetMarkerStyle(27) ;
   h_r_sherpa_stat->SetMarkerSize(1.4) ;
   h_r_sherpa_stat->SetMarkerColor(kMagenta) ;
   h_r_sherpa_stat->SetLineColor(kMagenta) ;
   h_r_sherpa_stat->SetFillColorAlpha(kMagenta-4,alpha+0.5) ;
   h_r_sherpa_stat->Draw("same PE2") ;
  
   leg = new TLegend(.2,.42,.32,.55);
   leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
   leg->AddEntry(h_r_sherpa,"Stat.","f");

   //leg->Draw();

  //TH1D* r_clone = 0;
  //r_clone = (TH1D*) file->Get(pltName+"_uf_amcatnlo_Combined"); 
  

  
  //r_clone has syst unc. now
  //for (int iB = 1 ; iB <= r_clone->GetNbinsX() ; ++iB) {
  //  double e1 = h_unfolded->GetBinError(iB) ;
  //  double e2 = r_clone->GetBinError(iB) ;
  //  r_clone->SetBinError(iB,sqrt(e1*e1-e2*e2)) ;
  //}
  /*
  r_clone->SetMarkerStyle(20);
  r_clone->SetLineColor(kBlack);
  r_clone->SetLineWidth(2);
  //r_clone->SetFillColor(kOrange);
  r_clone->SetFillColor(1);
  r_clone->SetFillStyle(3354) ;
  r_clone->Draw("same PE1");

  TH1D* h_amc = (TH1D*) file->Get( pair.first + "_amc_" + type );
  TGraphAsymmErrors* r_amc = graphShift( h_amc, -2 );
  r_amc->SetLineColor(kRed);
  r_amc->SetMarkerColor(kRed);
  r_amc->SetMarkerStyle(32);
  r_amc->SetMarkerSize(1.2);
  r_amc->SetLineWidth(2);
  //r_amc->Draw("same PZ");
  r_amc->Draw("same PX");

  TH1D* h_mad = (TH1D*) file->Get( pair.first + "_mad_" + type );
  TGraphAsymmErrors* r_mad = graphShift( h_mad, -1 );
  r_mad->SetLineColor(kGreen+2);
  r_mad->SetLineStyle(7);
  r_mad->SetLineWidth(2);
  r_mad->SetMarkerColor(kGreen+2);
  r_mad->SetMarkerStyle(26);
  r_mad->SetMarkerSize(1.2);
  r_mad->Draw("same PX");

  for (int iB = 1 ; iB < h_mad->GetNbinsX()+1 ; ++iB) {
    cout << "iB " << iB << " " << h_mad->GetBinError(iB) << "\n";
  }

  TH1D* h_nnpdfnlo = (TH1D*) file->Get( pair.first + "_nnpdfnlo_" + type );
  TGraphAsymmErrors* r_nnpdfnlo = graphShift( h_nnpdfnlo, 1 );
  r_nnpdfnlo->SetLineColor(kBlue);
  r_nnpdfnlo->SetMarkerColor(kBlue);
  r_nnpdfnlo->SetMarkerStyle(25);
  r_nnpdfnlo->SetLineWidth(2);
  r_nnpdfnlo->SetMarkerSize(1.2);
  r_nnpdfnlo->Draw("same PX");

  TH1D* h_nnpdflo = (TH1D*) file->Get( pair.first + "_nnpdflo_" + type );
  TGraphAsymmErrors* r_nnpdflo = graphShift( h_nnpdflo, 2 );
  r_nnpdflo->SetLineColor(kMagenta);
  r_nnpdflo->SetLineStyle(7);
  r_nnpdflo->SetMarkerColor(kMagenta);
  r_nnpdflo->SetMarkerStyle(27);
  r_nnpdflo->SetLineWidth(2);
  r_nnpdflo->SetMarkerSize(1.5);
  r_nnpdflo->Draw("same PX");

  TH1D* h_mmhtnlo = (TH1D*) file->Get( pair.first + "_mmhtnlo_" + type );
  TGraphAsymmErrors* r_mmhtnlo = graphShift( h_mmhtnlo, 3 );
  r_mmhtnlo->SetLineColor(kOrange+4);
  r_mmhtnlo->SetMarkerColor(kOrange+4);
  r_mmhtnlo->SetLineWidth(2);
  r_mmhtnlo->SetMarkerStyle(28);
  r_mmhtnlo->SetMarkerSize(1.5) ;
  r_mmhtnlo->Draw("same PX");

  for (int i=1; i<=h_amc->GetNbinsX(); i++) {
    r_amc->SetPointEYhigh( i-1, m_amc_UP[pair.first][i-1] );  r_amc->SetPointEYlow( i-1, m_amc_DN[pair.first][i-1] );
    r_mad->SetPointEYhigh( i-1, m_mg_UP[pair.first][i-1] );  r_mad->SetPointEYlow( i-1, m_mg_DN[pair.first][i-1] );

    r_nnpdfnlo->SetPointEYhigh( i-1, h_nnpdfnlo->GetBinContent(i)*m_mcfm_UP[pair.first][i-1] );
    r_nnpdfnlo->SetPointEYlow( i-1, h_nnpdfnlo->GetBinContent(i)*m_mcfm_DN[pair.first][i-1] );

    r_nnpdflo->SetPointEYhigh( i-1, h_nnpdflo->GetBinContent(i)*m_mcfm_loUP[pair.first][i-1] );
    r_nnpdflo->SetPointEYlow( i-1, h_nnpdflo->GetBinContent(i)*m_mcfm_loDN[pair.first][i-1] );

    r_mmhtnlo->SetPointEYhigh( i-1, h_mmhtnlo->GetBinContent(i)*m_mcfm_UP[pair.first][i-1] );
    r_mmhtnlo->SetPointEYlow( i-1, h_mmhtnlo->GetBinContent(i)*m_mcfm_DN[pair.first][i-1] );

    leg->AddEntry(r_clone, "Data", "PELF");
    leg->AddEntry(r_amc, "MG5_aMC [NLO, FxFx]", "P");
    leg->AddEntry(r_mad, "MG5_aMC [LO, MLM]", "P");
    leg->Draw();
    leg1->AddEntry(r_mmhtnlo, "MCFM MMHT NLO", "P");
    leg1->AddEntry(r_nnpdfnlo, "MCFM NNPDF 3.0 NLO", "P");
    leg1->AddEntry(r_nnpdflo, "MCFM NNPDF 3.0 LO", "P");
    leg1->Draw();
 
    TLatex text;
    text.SetNDC();
 
    text.SetTextColor(kBlack);
    text.SetTextSize(0.06); //0.05
    text.SetTextFont(61);
    text.DrawLatex(0.14, 0.956, "CMS");
    if (style == "pas") {
      text.SetTextFont(42);
      text.SetTextSize(0.045); //0.045
      text.DrawLatex(0.22, 0.956, "#it{Preliminary}");
    }

    text.SetTextFont(42);
    text.SetTextSize(0.05); //0.045
    text.DrawLatex(0.72,0.956,"35.9 fb^{-1} (13 TeV)"); //0.74
    
    topPad->Modified() ;
    

   //data ratio stat
   
   TGraphAsymmErrors* dRats[2] ;
   const int nP = r_clone->GetNbinsX();
   float ey_lo_Ps[nP] ;
   float ey_hi_Ps[nP] ;
   for (int iB = 0 ; iB < nP ; ++iB) {
     ey_lo_Ps[iB] = r_clone->GetBinError(iB+1) ;
     ey_hi_Ps[iB] = r_clone->GetBinError(iB+1) ;
   }

   dRats[0] = getGraph(r_clone,r_clone,ey_lo_Ps,ey_hi_Ps) ; //data stat
   
   for (int iB = 0 ; iB < nP ; ++iB) {
     ey_lo_Ps[iB] = h_unfolded->GetBinError(iB+1) ;
     ey_hi_Ps[iB] = h_unfolded->GetBinError(iB+1) ;
   }

   dRats[1] = getGraph(h_unfolded,h_unfolded,ey_lo_Ps,ey_hi_Ps) ; //data total error
   
   vector<TGraphAsymmErrors*> amcRats = getGraphMC(h_unfolded,h_amc, m_amc_DN[pair.first], m_amc_UP[pair.first]) ;
   vector<TGraphAsymmErrors*> mgRats = getGraphMC(h_unfolded,h_mad,m_mg_DN[pair.first], m_mg_UP[pair.first]) ;
   vector<TGraphAsymmErrors*> mmhtnloRats = getGraphMC(h_unfolded,h_mmhtnlo,m_mcfm_DN[pair.first], m_mcfm_UP[pair.first], true) ;
   vector<TGraphAsymmErrors*> nnpdfnloRats = getGraphMC(h_unfolded,h_nnpdfnlo,m_mcfm_DN[pair.first], m_mcfm_UP[pair.first], true) ;
   vector<TGraphAsymmErrors*> nnpdfloRats = getGraphMC(h_unfolded,h_nnpdflo,m_mcfm_loDN[pair.first], m_mcfm_loUP[pair.first], true) ;
   //amc@NLO

    bP1->cd() ;
   
   float alpha = 0.3 ;
   //histogram to plot axis
   TH1D* hA = (TH1D*)r_clone->Clone("hA") ;
   hA->Reset() ;
   hA->Draw() ;
   if (type == "jetpt") hA->GetXaxis()->SetRangeUser(30,200) ;
   hA->GetYaxis()->SetRangeUser(0.1,1.9) ;
   hA->GetYaxis()->SetNdivisions(505) ;
   hA->GetYaxis()->SetTitleSize(0.21) ; //0.15
   hA->GetYaxis()->SetLabelSize(0.21) ; //0.15
   //hA->GetYaxis()->SetTitle("Prediction/Data") ;
   hA->GetYaxis()->SetTitleOffset(0.3) ;
   hA->GetYaxis()->CenterTitle() ;
   double pad2H = bP1->GetWh()*bP1->GetAbsHNDC();
   hA->GetXaxis()->SetTickLength(6/pad2H) ;

   dRats[0]->Draw("same PE1") ; //PZ
   dRats[1]->SetFillStyle(3354) ;
   dRats[1]->Draw("same E2") ;
   //Int_t ci = 1756;
//TColor *color = new TColor(ci, 0.1, 0.2, 0.3, "", 0.5); // alpha = 0.5 
   mgRats[1]->SetFillColorAlpha(kGreen-1,alpha) ; //-3
   mgRats[1]->Draw("same E2") ;
   //mgRats[0]->SetMarkerStyle(22) ;
   mgRats[0]->SetMarkerStyle(26) ;
   mgRats[0]->SetMarkerColor(kGreen+2) ;
   mgRats[0]->SetLineColor(kGreen+2) ;
   mgRats[0]->SetFillColorAlpha(kGreen-1,alpha) ; // -7
   mgRats[0]->Draw("same PE1") ; //PZ

   
   amcRats[1]->SetFillColorAlpha(kRed-4,alpha) ;
   amcRats[1]->Draw("same E2") ;
   //amcRats[0]->SetMarkerStyle(23) ;
   amcRats[0]->SetMarkerStyle(32) ;
   amcRats[0]->SetMarkerColor(kRed) ;
   amcRats[0]->SetLineColor(kRed) ;
   amcRats[0]->SetFillColorAlpha(kRed-4,alpha) ;
   amcRats[0]->Draw("same PE1") ;
  
   leg = new TLegend(0.4,0.05,0.53,0.28) ;
   leg->SetLineColor(0) ;
   leg->SetFillColor(0) ;
   leg->AddEntry(amcRats[0], "MG5_aMC [NLO, FxFx]", "PELF") ;
   leg->SetTextSize(0.14) ;
   leg->Draw() ;
   
   leg1 = new TLegend(0.69,0.05,0.79,0.28) ;
   leg1->SetLineColor(0) ;
   leg1->SetFillColor(0) ;
   leg1->AddEntry(mgRats[0], "MG5_aMC [LO, MLM]", "PELF") ;
   leg1->SetTextSize(0.14) ;
   leg1->Draw() ;

   bP1->Modified() ;



   //second data/MC pad
   bP2->cd() ;
   TH1D* hA1 = (TH1D*)r_clone->Clone("hA1") ;
   hA1->Reset() ;
   hA1->Draw() ;
   if (type == "jetpt") hA->GetXaxis()->SetRangeUser(30,200) ;
   hA1->GetYaxis()->SetRangeUser(0.1,1.9) ;

   hA1->Draw() ;
   hA1->GetXaxis()->SetTitle( jetpt ? "p_{T}^{jet} [GeV]" : "p_{T}^Z [GeV]" );
   hA1->GetXaxis()->SetTitleSize(0.09) ;
   hA1->GetXaxis()->SetLabelSize(0.13) ;
   hA1->GetYaxis()->SetNdivisions(505) ;
   hA1->GetYaxis()->SetTitleSize(0.22) ; //0.22
   hA1->GetYaxis()->SetLabelSize(0.22) ; //0.16
   hA1->GetYaxis()->SetTitle("Pred./Data") ;
   hA1->GetYaxis()->SetTitleOffset(0.3) ;
   hA1->GetYaxis()->CenterTitle() ;
   pad2H = bP2->GetWh()*bP2->GetAbsHNDC();
   hA1->GetXaxis()->SetTickLength(6/pad2H) ;
   
   dRats[0]->Draw("same PEZ") ;
   dRats[1]->SetFillStyle(3354) ;
   dRats[1]->Draw("same E2") ;
   
   nnpdfloRats[1]->SetFillColorAlpha(kMagenta-4,alpha) ;
   nnpdfloRats[1]->Draw("same E2") ;
   nnpdfloRats[0]->SetMarkerStyle(27) ;
   nnpdfloRats[0]->SetMarkerSize(1.4) ;
   nnpdfloRats[0]->SetMarkerColor(kMagenta) ;
   nnpdfloRats[0]->SetLineColor(kMagenta) ;
   nnpdfloRats[0]->SetFillColorAlpha(kMagenta-4,alpha) ;
   nnpdfloRats[0]->Draw("same PE1") ;
 
   nnpdfnloRats[1]->SetFillColorAlpha(kBlue-4,alpha) ;
   nnpdfnloRats[1]->Draw("same E2") ;
   nnpdfnloRats[0]->SetMarkerStyle(25) ;
   //nnpdfnloRats[0]->SetMarkerSize(1.5) ;
   nnpdfnloRats[0]->SetMarkerColor(kBlue) ;
   nnpdfnloRats[0]->SetLineColor(kBlue) ;
   nnpdfnloRats[0]->SetFillColorAlpha(kBlue-4,alpha) ;
   nnpdfnloRats[0]->Draw("same PE1") ;
   
   float y1 = 0.7 ;
   float y2 = 0.93 ;
   if ((!jetpt && pair.first == "b_incl") || jetpt) {
     y1 = 0.05 ;
     y2 = 0.28 ;
   }
   leg = new TLegend(0.38,y1,0.48,y2) ;
   leg->SetLineColor(0) ;
   leg->SetFillColor(0) ;
   leg->AddEntry(nnpdfnloRats[0], "MCFM NNPDF 3.0 NLO", "PELF") ;
   leg->SetTextSize(0.15) ;
   leg->Draw() ;
   
   leg1 = new TLegend(0.67,y1,0.77,y2) ;
   leg1->SetLineColor(0) ;
   leg1->SetFillColor(0) ;
   leg1->AddEntry(nnpdfloRats[0], "MCFM NNPDF 3.0 LO", "PELF") ;
   leg1->SetTextSize(0.15) ;
   leg1->Draw() ;




   //third data/MC pad
   bP3->cd() ;
   TH1D* hA2 = (TH1D*)r_clone->Clone("hA2") ;
   hA2->Reset() ;
   hA2->Draw() ;
   if (type == "jetpt") hA2->GetXaxis()->SetRangeUser(30,200) ;
   hA2->GetYaxis()->SetRangeUser(0.1,1.9) ;

   hA2->Draw() ;
   hA2->GetXaxis()->SetTitle( jetpt ? "p_{T}^{jet} [GeV]" : "p_{T}^{Z} [GeV]" );
   //hA2->GetYaxis()->SetTitle("Prediction/Data") ;
   hA2->GetYaxis()->SetTitleOffset(0.4) ;
   hA2->GetYaxis()->CenterTitle() ;
   hA2->GetYaxis()->SetNdivisions(505) ;
   hA2->GetYaxis()->SetTitleSize(0.1) ; //0.1
   hA2->GetYaxis()->SetLabelSize(0.14) ; //0.1
   hA2->GetXaxis()->SetTitleSize(0.15) ; //0.15
   hA2->GetXaxis()->SetLabelSize(0.13) ;
   hA2->GetXaxis()->SetTitleOffset(1.1) ; //0.89
   hA2->GetXaxis()->SetLabelOffset(0.03) ; //0.89
   pad2H = bP3->GetWh()*bP3->GetAbsHNDC();
   hA2->GetXaxis()->SetTickLength(6/pad2H) ; //12

   dRats[0]->Draw("same PEZ") ;
   //dRats[0]->SetLineWidth(2) ;
   
   dRats[1]->SetFillStyle(3354) ;
   dRats[1]->Draw("same E2") ;

   mmhtnloRats[1]->SetFillColorAlpha(kOrange+6,alpha) ;
   //mmhtnloRats[1]->SetFillStyle(3001) ;
   mmhtnloRats[1]->Draw("same E2") ;
   //mmhtnloRats[0]->SetLineWidth(2) ;
   mmhtnloRats[0]->SetMarkerStyle(28) ;
   mmhtnloRats[0]->SetMarkerSize(1.4) ;
   mmhtnloRats[0]->SetMarkerColor(kOrange+4) ;
   mmhtnloRats[0]->SetLineColor(kOrange+4) ;
   mmhtnloRats[0]->SetFillColorAlpha(kOrange+6,alpha) ;
   mmhtnloRats[0]->Draw("same PE1") ;
  
   y1 = 0.77 ;
   y2 = 0.97 ;
   //if (jetpt && pair.first == "b_incl") {
   if (jetpt && !(pair.first=="c_b")) {
     y1 = 0.39 ;
     y2 = 0.59 ;
   }


   leg = new TLegend(0.67,y1,0.78,y2) ;
   leg->SetLineColor(0) ;
   leg->SetFillColor(0) ;
   leg->SetTextSize(0.095) ;
   leg->AddEntry(mmhtnloRats[0], "MCFM MMHT NLO", "PELF") ;
   leg->Draw() ;


  }*/

   //gPad->RedrawAxis();
   TString outPltName = "../Output_unfolding/V1/"+pltName+"_"+chan+".pdf";
   TString outPltName1 = "../Output_unfolding/V1/"+pltName+"_"+chan+".C";
   if (pltType == "uf_norm") {
     outPltName = "../Output_unfolding/V1/"+pltName+"_norm_"+chan+".pdf";
     outPltName1 = "../Output_unfolding/V1/"+pltName+"_norm_"+chan+".C";
   }
   if (pltType == "rat") {
     outPltName = "../Output_unfolding/V1/"+pltName+"_rat_"+chan+".pdf";
     outPltName1 = "../Output_unfolding/V1/"+pltName+"_rat_"+chan+".C";
   }
   if (chan == "Combined") {
     outPltName = "../Output_unfolding/V1/"+pltName+".pdf";
     outPltName1 = "../Output_unfolding/V1/"+pltName+".C";
     if (pltType == "uf_norm") {
       outPltName = "../Output_unfolding/V1/"+pltName+"_norm.pdf";
       outPltName1 = "../Output_unfolding/V1/"+pltName+"_norm.C";
     }
     if (pltType == "rat") {
       outPltName = "../Output_unfolding/V1/"+pltName+"_rat.pdf";
       outPltName1 = "../Output_unfolding/V1/"+pltName+"_rat.C";
     }
   }


   c.Print(outPltName);
   c.Print(outPltName1);
}
 
void setStyle() {
 
  gStyle->SetPadTopMargin(0.065);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadRightMargin(0.03);
  gStyle->SetPadLeftMargin(0.14);
 
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
 
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.05, "XYZ");
  gStyle->SetTitleXOffset(1.1);
  gStyle->SetTitleYOffset(1.2);
  gStyle->SetNdivisions(510, "XYZ");
 
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.007, "XYZ");
  gStyle->SetLabelSize(0.04, "XYZ");
 
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
}
