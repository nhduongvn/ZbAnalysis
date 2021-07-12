import os,sys
import ROOT

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

def PlotSyst(plt,syst,plotFolder,chan,hEU,hED,hComU,hComD):
  c = ROOT.TCanvas("c")
  hEU.SetLineColor(2)
  hED.SetLineColor(4)
  hEU.SetLineStyle(2)
  hED.SetLineStyle(2)
  hComU.SetLineColor(1)
  #hComU.SetLineStyle(2)
  hComD.SetLineColor(1)
  #hComD.SetLineStyle(2)
  hEU.Draw("hist e0")
  hEU.SetTitle(syst + " uncertainty ("+chan+')')
  #hEU.SetOptStat(0)
  hEU.GetYaxis().SetRangeUser(-0.25,0.25)
  hEU.GetYaxis().SetTitle("Relative uncertainty")
  hEU.GetXaxis().SetTitle(plt)
  hED.Draw("same hist e0")
  hComU.Draw("same hist e0")
  hComD.Draw("same hist e0")

  leg = ROOT.TLegend(0.6,0.7,0.9,0.9)
  leg.AddEntry(hEU,syst+ ' (up)',"l")
  leg.AddEntry(hED,syst+ ' (down)',"l")
  leg.AddEntry(hComU,syst+' (combined)',"l")
  leg.Draw()
  
  c.Print(plotFolder+"/"+plt+"_"+syst+"_unc_"+chan+".pdf")

####################################################
#Begin
####################################################

pltType = 'uf_norm' #'uf': differential, 'uf_norm': norm

#sourceFolder = '../Output_unfolding_postProc/' #for experimental uncertainty no missing file
#sourceFolderBkgr = '../Output_unfolding/' #for background uncertainty 
#outFolder = '../Output_unfolding/'

sourceFolder = '../Output_unfolding_jetEta/' #for experimental uncertainty no missing file
sourceFolderBkgr = '../Output_unfolding_jetEta/' #for background uncertainty 
outFolder = '../Output_unfolding_jetEta/'

#sourceFolder = '../Output_unfolding_postProc/' #for experimental uncertainty no missing file
#sourceFolderBkgr = '../Output_unfolding/' #for background uncertainty 
#outFolder = '../Output_unfolding/'

plotFolder = outFolder + '/Unc_plot/'
if pltType == 'uf_norm':
  plotFolder = outFolder + '/Unc_plot_uf_norm/'

#fN = 'unfolding.root'
#fNbkgr = 'unfolding_bkgrUnc.root'
#fN = 'unfolding_withJETMET_correlation.root'
#fNbkgr = 'unfolding_bkgrUnc.root'
fN = 'unfolding_absEta_syst.root'
fNbkgr = 'unfolding_absEta_bkgrUnc.root'
fN_absEta_jetmet = "unfolding_absEta_syst_JETMET_correlation_only.root"
#fN = 'unfolding_forRatio.root'
#fNbkgr = 'unfolding_bkgrUnc.root'
#fN = 'unfolding_absEta_syst.root'
#fNbkgr = 'unfolding_absEta_bkgrUnc.root'
#if pltType == 'uf_norm':
#  fN = 'unfolding_withNormDistributions.root'
#  fNBkgr = 'unfolding_bkgrUnc.root'

#fN = 'unfolding_absEta_syst.root' #contain both norm and not norm
#fNbkgr = 'unfolding_absEta_bkgrUnc.root'
#fN = 'unfolding_m2b.root' #contain both norm and not norm
#fNbkgr = 'unfolding_bkgrUnc.root'
#if pltType == 'uf_norm':
#  fN = 'unfolding_withNormDistributions.root'
#  fNBkgr = 'unfolding_bkgrUnc.root'

fS = ROOT.TFile.Open(sourceFolder+'/'+fN,'READ')
fS_abseta_jetmet = ROOT.TFile.Open(sourceFolder+'/'+fN_absEta_jetmet,'READ')
fSbkgr = ROOT.TFile.Open(sourceFolderBkgr+'/'+fNbkgr,'READ')

#'NONE','PUU','PUD','L1PREFIRINGU','L1PREFIRINGD','BTAGU','BTAGD','ELEU','ELED','MUONU','MUOND','JESU','JESD','JERU','JERD','METJESU','METJESD','METJERU','METJERD','METUNCLUSTU','METUNCLUSTD'
#systs = ['PU','L1PREFIRING','BTAG','ELE','MUON','JES','JER','METJES','METJER','METUNCLUST']
#systs = ['PU','METJES','METJER','METUNCLUST']
#systs = ['TT']
#systs = ['DYXSEC','WWXSEC','WZXSEC','ZZXSEC']
#systs = ['PU','L1PREFIRING','BTAG','ELE','MUON','JES','JER','METJES','METJER','METUNCLUST','TT','DYXSEC','WWXSEC','WZXSEC','ZZXSEC']
#systs = ['PU','L1PREFIRING','BTAGL','BTAGBC','ELET','ELEREC','ELEID','MUONT','MUONID','MUONISO','JES','JER','METJES','METJER','METUNCLUST','TT','DYXSEC','WWXSEC','WZXSEC','ZZXSEC']
systs = ['PU','L1PREFIRING','BTAGL','BTAGBC','ELET','ELEREC','ELEID','MUONT','MUONID','MUONISO','JES_METJES','JER_METJER','METUNCLUST','TT','DYXSEC','WWXSEC','WZXSEC','ZZXSEC']
pltNames = ['Int_xSec','pt_b1','pt_b2','pt_Z','dR_2b','dRmin_Z2b','A_Z2b','m_2b','m_Z2b']
#pltNames = ['Int_xSec']
#pltNames = ['m_2b']
pltNames = ['abseta_b1']
#pltNames = ['pt_b1','pt_Z']

#fOut = ROOT.TFile.Open(outFolder+'/syst_uncertainty_withNormDistributions_max.root','RECREATE')
#fOut = ROOT.TFile.Open(outFolder+'/syst_bkgr_uncertainty_max.root','RECREATE')
#fOut = ROOT.TFile.Open(outFolder+'/syst_forRatio_all_uncertainty_max.root','RECREATE')
#fOut = ROOT.TFile.Open(outFolder+'/syst_uf_norm_abseta_all_uncertainty_max.root','RECREATE')
fOut = ROOT.TFile.Open(outFolder+'/syst_uf_norm_absEta_all_withJETMET_correlation.root','RECREATE')

#loop over plot 
for plt in pltNames:
  for chan in ['Electron','Muon','Combined']:
    if plt == 'Int_xSec' and chan != 'Combined': continue #since Int_xSec contains integrated xSec of all channels so only need to run once using 'Combined'
    print 'Chan: ', chan
    #get central error
    #pt_b1_uf_amcatnlo_NONE_Electron 
    print fS
    nTmp = plt+'_'+pltType+'_amcatnlo_NONE_'+chan
    #Int_xSec_NONE
    if plt == 'Int_xSec': nTmp = plt+'_NONE'
    hC = fS.Get(nTmp)
  
    hTot = hC.Clone(plt+"_"+pltType+"_amcatnlo_totUnc_" + chan)
    hTot.Reset()
    hMET_EU = hC.Clone(plt+"_"+pltType+"_amcatnlo_METU_" + chan + '_relErr')
    if plt == 'Int_xSec': hMET_EU.SetName(plt+'_METU_relErr')
    hMET_EU.Reset()
    hMET_ED = hC.Clone(plt+"_"+pltType+"_amcatnlo_METD_" + chan+'_relErr')
    if plt == 'Int_xSec': hMET_ED.SetName(plt+'_METD_relErr')
    hMET_ED.Reset()
    hMET_ComU = hC.Clone(plt+"_"+pltType+"_amcatnlo_METU_COM_" + chan + '_relErr') #this is MET tot unc
    if plt == 'Int_xSec': hMET_ComU.SetName(plt+'_METU_COM_relErr')
    hMET_ComU.Reset()
    hMET_ComD = hC.Clone(plt+"_"+pltType+"_amcatnlo_METD_COM_" + chan + '_relErr') #this is MET tot unc
    if plt == 'Int_xSec': hMET_ComD.SetName(plt+'_METD_COM_relErr')
    hMET_ComD.Reset()
    hJESJERMET_ComU = hC.Clone(plt+"_"+pltType+"_amcatnlo_JESJERMETU_COM_" + chan+'_relErr') #only need Up, used to print unc.
    if plt == 'Int_xSec': hJESJERMET_ComU.SetName(plt+'_JESJERMETU_COM_relErr')
    hJESJERMET_ComU.Reset()
    for syst in systs:
      hCforSyst = hC
      if 'DYXSEC' in syst or 'WWXSEC' in syst or 'WZXSEC' in syst or 'ZZXSEC' in syst or 'TT' in syst: 
        #Int_xSec_NONE
        nTmp = plt+'_'+pltType+'_amcatnlo_NONE_'+chan
        if plt == 'Int_xSec': nTmp = plt+'_NONE'
        hCforSyst = fSbkgr.Get(nTmp)
      #pt_b1_uf_amcatnlo_Electron
      #Int_xSec_JERU
      nTmp = plt+'_'+pltType+'_amcatnlo_'+syst+'U_'+chan
      if plt == 'Int_xSec': nTmp = plt+'_'+syst+'U'
      if 'JES' in syst or 'JER' in syst:
        nTmp = nTmp.replace('JES_','JESU_')
        nTmp = nTmp.replace('JER_','JERU_')
      if plt == 'abseta_b1' and syst in ["JES_METJES","JER_METJER"]: 
        hCforSyst = fS_abseta_jetmet.Get(plt+'_'+pltType+'_amcatnlo_NONE_'+chan)
        hU = fS_abseta_jetmet.Get(nTmp)
      else:
        hU = fS.Get(nTmp)

      print "Here: ", nTmp, " ", hU, " ", fS
      print ">>>>>>>>>>>>>>>>>>>>>>>"
      print chan, syst
      hCforSyst.Print("all")
      
      nTmp = plt+'_'+pltType+'_amcatnlo_'+syst+'D_'+chan
      if plt == 'Int_xSec': nTmp = plt+'_'+syst+'D'
      if 'JES' in syst or 'JER' in syst:
        nTmp = nTmp.replace('JES_','JESD_')
        nTmp = nTmp.replace('JER_','JERD_')
      if plt == 'abseta_b1' and syst in ["JES_METJES","JER_METJER"]:
        hD = fS_abseta_jetmet.Get(nTmp)
      else:
        hD = fS.Get(nTmp)

      if 'DYXSEC' in syst or 'WWXSEC' in syst or 'WZXSEC' in syst or 'ZZXSEC' in syst or 'TT' in syst: 
        nTmp = plt+'_'+pltType+'_amcatnlo_'+syst+'U_'+chan
        if plt == 'Int_xSec': nTmp = plt+'_'+syst+'U'
        hU = fSbkgr.Get(nTmp)
        nTmp = plt+'_'+pltType+'_amcatnlo_'+syst+'D_'+chan
        if plt == 'Int_xSec': nTmp = plt+'_'+syst+'D'
        hD = fSbkgr.Get(nTmp) 
      print syst, " ", hU
      hEU = hU.Clone(hU.GetName()+'_relErr')
      hEU.Add(hCforSyst,-1.0)
      hEU.Divide(hCforSyst)
      hED = hD.Clone(hD.GetName()+'_relErr') #FIXME
      hED.Add(hCforSyst,-1.0)
      hED.Divide(hCforSyst)
      ##############################################
      '''
      if plt == 'm_2b' and syst == 'METJER':
        ave = (hEU.GetBinContent(6) + hEU.GetBinContent(7))/2
        hEU.SetBinContent(6,ave)
        hEU.SetBinContent(7,ave)
        ave = (hED.GetBinContent(6) + hED.GetBinContent(7))/2
        hED.SetBinContent(6,ave)
        hED.SetBinContent(7,ave)
      if plt == 'dRmin_Z2b' and (syst == 'METJER' or syst == 'METJES'):
        ave = (hEU.GetBinContent(8) + hEU.GetBinContent(9))/2
        hEU.SetBinContent(8,ave)
        hEU.SetBinContent(9,ave)
        ave = (hED.GetBinContent(8) + hED.GetBinContent(9))/2
        hED.SetBinContent(8,ave)
        hED.SetBinContent(9,ave)
      if plt == 'pt_Z':
        hEU.SetBinContent(9,hEU.GetBinContent(8))
        hED.SetBinContent(9,hED.GetBinContent(8))
        if chan == 'Muon' or chan == 'Combined':
          hEU.SetBinContent(1,(hEU.GetBinContent(1)+hEU.GetBinContent(2))/2)
          hED.SetBinContent(1,(hED.GetBinContent(1)+hEU.GetBinContent(2))/2)
      '''
      #adding MET together, only UNCLUST left
      #if 'MET' in syst:
      if 'METUNCLUST' in syst:
        hTmp = hEU.Clone(syst+'U')
        hTmp.Multiply(hTmp)
        hMET_EU.Add(hTmp)
        hTmp = hED.Clone(syst+'D')
        hTmp.Multiply(hTmp)
        hMET_ED.Add(hTmp)

      hComU = hEU.Clone(hEU.GetName().replace(syst+'U',syst+'U_COM')) #this is uncertainty combining up and down variations, taking maxium variations in absolute value
      hComU.Reset()
      hComD = hEU.Clone(hEU.GetName().replace(syst+'U',syst+'D_COM')) #hComD = -hComU
      hComD.Reset()
      for iB in range(1,hEU.GetNbinsX()+1):
        #err = (abs(hEU.GetBinContent(iB)) + abs(hED.GetBinContent(iB)))/2.
        err = max(abs(hEU.GetBinContent(iB)),abs(hED.GetBinContent(iB)))
        hComU.SetBinContent(iB, err)
        hComU.SetBinError(iB,0)
        hComD.SetBinContent(iB, -err)
        hComD.SetBinError(iB,0)
        hEU.SetBinError(iB,0)
        hED.SetBinError(iB,0)
        hTot.SetBinContent(iB,hTot.GetBinContent(iB)+err*err) 
        #if 'MET' in syst:
        if 'METUNCLUST' in syst:
          hMET_ComU.SetBinContent(iB,hMET_ComU.GetBinContent(iB)+err*err) 
        #if syst in ['JES','JER']:
        if syst in ['JES_METJES','JER_METJER','METUNCLUST']:
          hJESJERMET_ComU.SetBinContent(iB,hJESJERMET_ComU.GetBinContent(iB)+err*err) 
      
      fOut.cd()
      hEU.Write()
      hED.Write()
      hComU.Write()
      hComD.Write()

      PlotSyst(plt,syst,plotFolder,chan,hEU,hED,hComU,hComD)
    
    #take square root of unc.
    for iB in range(1,hTot.GetNbinsX()+1):
      hTot.SetBinContent(iB, ROOT.TMath.Sqrt(hTot.GetBinContent(iB)))
      hJESJERMET_ComU.SetBinContent(iB, ROOT.TMath.Sqrt(hJESJERMET_ComU.GetBinContent(iB)))
      hMET_EU.SetBinContent(iB, ROOT.TMath.Sqrt(hMET_EU.GetBinContent(iB)))
      hMET_ED.SetBinContent(iB, -ROOT.TMath.Sqrt(hMET_ED.GetBinContent(iB)))
      hMET_ComU.SetBinContent(iB, ROOT.TMath.Sqrt(hMET_ComU.GetBinContent(iB)))
      hMET_ComD.SetBinContent(iB, -hMET_ComU.GetBinContent(iB))
      hMET_EU.SetBinError(iB,0)
      hMET_ED.SetBinError(iB,0)
      hMET_ComU.SetBinError(iB,0)
      hMET_ComD.SetBinError(iB,0)
      
    fOut.cd()
    hTot.Write()
    hJESJERMET_ComU.Write()
    hMET_EU.Write()
    hMET_ED.Write()
    hMET_ComU.Write()
    hMET_ComD.Write()
    
    PlotSyst(plt,'MET',plotFolder,chan,hMET_EU,hMET_ED,hMET_ComU,hMET_ComD)
