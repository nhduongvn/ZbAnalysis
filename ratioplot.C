void ratioplot() {
    TString fmcname="out.root";
    TFile* file = TFile::Open( fmcname );
    TH1D* h1 = (TH1D*) file->Get( "jet_eta_nocut" );
    TH1D* h2 = (TH1D*) file->Get( "jet_eta_kin_cut" );
    gStyle->SetOptStat(0);
    auto c1 = new TCanvas("c1", "jet_eta_kin_cut vs. jet_eta_nocut");
    auto rp = new TRatioPlot(h2, h1);
    rp->Draw();
    rp->GetLowerRefYaxis()->SetTitle("kin_cut / nocut");
    rp->GetUpperRefYaxis()->SetTitle("entries");
    
    c1->Update();
}



/*void ratioplot() {
    // Define two gaussian histograms. Note the X and Y title are defined
    // at booking time using the convention "Hist_title ; X_title ; Y_title"
    TString fmcname="out.root";
    TFile* file = TFile::Open( fmcname );
    TH1D* h1 = (TH1D*) file->Get( "ele_pT_loose_MVA" );
    TH1D* h2 = (TH1D*) file->Get( "ele_pT_kin_cut" );
    
    
    // Define the Canvas
    TCanvas *c = new TCanvas("c", "ele pT_nocut/ele pT_kin_cut", 800, 800);
    
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    h1->SetStats(0);          // No statistics on upper plot
    h1->Draw();               // Draw h1
    h2->Draw("same");         // Draw h2 on top of h1
    
    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    h1->GetYaxis()->SetLabelSize(0.);
    TGaxis *axis = new TGaxis( 0.0, 0.0, 0.0, 6000, 20,6000,510,"");
    axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    axis->SetLabelSize(15);
    axis->Draw();
    
    // lower plot will be in pad
    c->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    
    // Define the ratio plot
    TH1F *h3 = (TH1F*)h2->Clone("h3");
    h3->SetLineColor(kBlack);
    h3->SetMinimum(0.0);  // Define Y ..
    h3->SetMaximum(10.0); // .. range
    h3->Sumw2();
    h3->SetStats(0);      // No statistics on lower plot
    h3->Divide(h1);
    h3->SetMarkerStyle(41);
    h3->Draw("ep");       // Draw the ratio plot
    
    // h1 settings
    h1->SetLineColor(kBlue+1);
    h1->SetLineWidth(2);
    
    // Y axis h1 plot settings
    h1->GetYaxis()->SetTitleSize(20);
    h1->GetYaxis()->SetTitleFont(43);
    h1->GetYaxis()->SetTitleOffset(1.55);
    h1->GetXaxis()->SetTitle("pT (GeV)");
    h1->GetYaxis()->SetTitle("Entries");
    
    // h2 settings
    h2->SetLineColor(kRed);
    h2->SetLineWidth(2);
    
    // Ratio plot (h3) settings
    h3->SetTitle(""); // Remove the ratio title
    
    // Y axis ratio plot settings
    h3->GetYaxis()->SetTitle("ratio nocut/kin_cut");
    h3->GetYaxis()->SetNdivisions(505);
    h3->GetYaxis()->SetTitleSize(20);
    h3->GetYaxis()->SetTitleFont(43);
    h3->GetYaxis()->SetTitleOffset(1.55);
    h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    h3->GetYaxis()->SetLabelSize(15);
    
    // X axis ratio plot settings
    h3->GetXaxis()->SetTitleSize(20);
    h3->GetXaxis()->SetTitleFont(43);
    h3->GetXaxis()->SetTitleOffset(4.);
    h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    h3->GetXaxis()->SetLabelSize(15);
}


*/

