import os,sys
import ROOT

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)


#fS = ROOT.TFile.Open('../Output_unfolding_postProc/syst_uncertainty_max.root','READ')
pltType = 'uf_norm'

fN = 'syst_uncertainty_max.root'
if pltType == 'uf_norm':
  fN = 'syst_uncertainty_withNormDistributions_max.root'


fS = ROOT.TFile.Open('../Output_unfolding_postProc/'+fN,'READ')


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
    systs = ['JES','JER','MET','PU','L1PREFIRING','BTAG','ELE','MUON']
    legName = ['JES','JER','MET','Pileup','L1 prefiring','btagging','Electron','Muon']
    #systs = ['PU']
    
    #get total unc.
    hN = plt + '_'+pltType+'_amcatnlo_totUnc_' + chan
    print hN
    hT = fS.Get(hN)
    
    c = ROOT.TCanvas(plt+'_'+pltType+'_all_unc_'+chan)
    hT.Draw("hist")
    hT.SetTitle("Uncertainties ("+chan+')')
    hT.GetYaxis().SetTitle("Relative uncertainty")
    hT.GetYaxis().SetRangeUser(0,0.5)
    hT.GetXaxis().SetTitle(plt)

    leg = ROOT.TLegend(0.6,0.6,0.9,0.9)
    leg.AddEntry(hT,'Total syst. unc.','l')
    
    lineStyle = [2,3,4,5,6,7,8,9,10]
    lineColor = [2,3,4,5,6,7,8,9,10]
    
    i = 0
    for syst in systs:
      #pt_b1_uf_amcatnlo_METJERU_COM_Electron_relErr
      hN = plt + '_'+pltType+'_amcatnlo_' + syst + 'U_COM_' + chan + '_relErr'
      print hN
      hSs[syst] = fS.Get(hN)
      hSs[syst].SetLineStyle(lineStyle[i])
      hSs[syst].SetLineColor(lineColor[i])
      hSs[syst].Draw("same")
      leg.AddEntry(hSs[syst],legName[i],'l')
      i+=1

    leg.Draw()

    
    c.Print("../Output_unfolding_postProc/Unc_plot_"+pltType+"/"+c.GetName()+'.pdf')


