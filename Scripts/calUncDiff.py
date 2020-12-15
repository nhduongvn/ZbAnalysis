import os,sys
import ROOT

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

fC = ROOT.TFile.Open('../Output_unfolding_syst/NONE/unfolding.root','READ')
fC1 = ROOT.TFile.Open('../Output_unfolding_syst/JETNOM//unfolding.root','READ')

#loop over plot 
pltNames = ['pt_b1','pt_b2','pt_Z','dR_2b','dRmin_Z2b','A_Z2b','m_2b','m_Z2b']
#pltNames = ['pt_b1']

fOut = ROOT.TFile.Open('../Output_unfolding_syst/syst_uncertainty_ave.root','RECREATE')

for plt in pltNames:
  for chan in ['Electron','Muon','Combined']:
    print 'Chan: ', chan
    #get central error
    #pt_b1_uf_amcatnlo_Electron
    hC = fC.Get(plt+'_uf_amcatnlo_'+chan)
  
    hSysts = {} 
    systs = ['JES','JER','PU','BTAG','ELE','MUON']
    #systs = ['JER']
    
    hTot = hC.Clone(plt+"_uf_amcatnlo_" + chan + "_totunc")
    hTot.Reset()
    for syst in systs:
      if syst in ['JES','JER']:
        hC = fC1.Get(plt+'_uf_amcatnlo_'+chan)
      fU = ROOT.TFile.Open("../Output_unfolding_syst/"+syst+'U/unfolding.root','READ')
      fD = ROOT.TFile.Open("../Output_unfolding_syst/"+syst+'D/unfolding.root','READ')
      #pt_b1_uf_amcatnlo_Electron
      hU = fU.Get(plt+'_uf_amcatnlo_'+chan)
      hD = fD.Get(plt+'_uf_amcatnlo_'+chan)
      hEU = hU.Clone(hU.GetName()+'_'+syst+'U')
      hEU.Add(hC,-1.0)
      hEU.Divide(hC)
      hED = hC.Clone(hC.GetName()+'_'+syst+'D')
      hED.Add(hD,-1.0)
      hED.Divide(hC)

      hCom = hEU.Clone(hEU.GetName().replace(syst+'U',syst+'COM'))
      hCom.Reset()
      for iB in range(1,hEU.GetNbinsX()+1):
        err = (abs(hEU.GetBinContent(iB)) + abs(hED.GetBinContent(iB)))/2.
        #err = max(abs(hEU.GetBinContent(iB)),abs(hED.GetBinContent(iB)))
        hCom.SetBinContent(iB, err)
        hEU.SetBinError(iB,0)
        hED.SetBinError(iB,0)
        hTot.SetBinContent(iB,hTot.GetBinContent(iB)+err*err) 
      
      fOut.cd()
      hEU.Write()
      hED.Write()
      hCom.Write()

      c = ROOT.TCanvas("c")
      hEU.SetLineColor(2)
      hED.SetLineColor(4)
      hEU.SetLineStyle(2)
      hED.SetLineStyle(2)
      hEU.Draw("hist e0")
      hEU.SetTitle(syst + "uncertainty")
      #hEU.SetOptStat(0)
      hEU.GetYaxis().SetRangeUser(-0.2,0.2)
      hEU.GetYaxis().SetTitle("Relative uncertainty")
      hEU.GetXaxis().SetTitle(plt)
      hED.Draw("same hist e0")
      c.Print("../Output_unfolding_syst/Unc_plot/"+plt+"_"+syst+"_unc_"+chan+".pdf")
    
    for iB in range(1,hTot.GetNbinsX()+1):
      hTot.SetBinContent(iB, ROOT.TMath.Sqrt(hTot.GetBinContent(iB)))
    fOut.cd()
    hTot.Write()
