import os,sys
import ROOT

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)


fS = ROOT.TFile.Open('../Output_unfolding_syst/syst_uncertainty_ave.root','READ')

pltNames = ['pt_b1','pt_b2','pt_Z','dR_2b','dRmin_Z2b','A_Z2b','m_2b','m_Z2b']
#pltNames = ['pt_b1']

#A_Z2b_uf_amcatnlo_Muon_MUONU
#A_Z2b_uf_amcatnlo_Muon_MUOND
#A_Z2b_uf_amcatnlo_Muon_MUONCOM
#A_Z2b_uf_amcatnlo_Muon_totunc

for plt in pltNames:
  for chan in ['Electron','Muon','Combined']:
    print 'Chan: ', chan
    #get central error
    #pt_b1_uf_amcatnlo_Electron
  
    hSs = {} 
    systs = ['JES','JER','PU','BTAG','ELE','MUON']
    legName = ['JES','JER','Pileup','btagging','Electron','Muon']
    #systs = ['PU']
    
    #get total unc.
    hN = plt + '_uf_amcatnlo_' + chan + '_totunc'
    print hN
    hT = fS.Get(hN)
    
    c = ROOT.TCanvas(plt+'_all_unc_'+chan)
    hT.Draw("hist")
    hT.GetYaxis().SetTitle("Relative uncertainty")
    hT.GetYaxis().SetRangeUser(0,0.5)
    hT.GetXaxis().SetTitle(plt)

    leg = ROOT.TLegend(0.6,0.6,0.9,0.9)
    leg.AddEntry(hT,'Total syst. unc.','l')
    
    lineStyle = [2,3,4,5,6,7,8,9,10]
    lineColor = [2,3,4,5,6,7,8,9,10]
    
    i = 0
    for syst in systs:
      hN = plt + '_uf_amcatnlo_' + chan + '_' + syst + 'COM'
      hSs[syst] = fS.Get(hN)
      hSs[syst].SetLineStyle(lineStyle[i])
      hSs[syst].SetLineColor(lineColor[i])
      hSs[syst].Draw("same")
      leg.AddEntry(hSs[syst],legName[i],'l')
      i+=1

    leg.Draw()

    

    c.Print("../Output_unfolding_syst/Unc_plot/"+c.GetName()+'.pdf')


