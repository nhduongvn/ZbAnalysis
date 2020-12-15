import os,sys
import ROOT

fC = ROOT.TFile.Open('../Output_unfolding_syst/Central/unfolding.root','READ')
fC1 = ROOT.TFile.Open('../Output_unfolding_syst/JETNOM/unfolding.root','READ')
hC = fC.Get('Int_xSec')
hC1 = fC1.Get('Int_xSec')
for chan in ['Electron','Muon','Combined']:
  print 'Chan: ', chan
  errTU = 0
  errTD = 0
  upS=['JESU','JERU','PUU','BTAGU','ELEU','MUONU']
  downS=['JESD','JERD','PUD','BTAGD','ELED','MUOND']
  systs = upS + downS
  errSum = {'JES':[],'JER':[],'PU':[],'BTAG':[],'ELE':[],'MUON':[]}
  for syst in systs: 
    iB = 1
    if chan == 'Muon': iB = 2
    if chan == 'Combined': iB = 3
    f = ROOT.TFile.Open("../Output_unfolding_syst/"+syst+'/unfolding.root','READ')
    hInt_xSec = f.Get('Int_xSec')
    hInt_xSec.GetBinContent(iB)
    derr = hInt_xSec.GetBinContent(iB) - hC.GetBinContent(iB)
    if syst in ['JESU','JERU','JESD','JERD']:
      derr = hInt_xSec.GetBinContent(iB) - hC1.GetBinContent(iB)

    if syst in upS:
      errTU += derr*derr
    if syst in downS:
      errTD += derr*derr
    if syst in ['JESU','JERU','JESD','JERD']:
      relErr = derr/hC.GetBinContent(iB)
      print syst, ' ', derr, ' ', relErr
      for k in errSum.keys():
        if k in syst:
          errSum[k].append(relErr)
    else:
      relErr = derr/hC1.GetBinContent(iB)
      print syst, ' ', derr, ' ', relErr
      for k in errSum.keys():
        if k in syst:
          errSum[k].append(relErr)
  
  print ">>>>Uncertainty breakdown>>>>>"
  totSyst1 = 0
  for k in ['JES','JER','PU','BTAG','ELE','MUON']:
    systTmp = (abs(errSum[k][0])+abs(errSum[k][1]))/2
    print k, " ", systTmp
    totSyst1 += systTmp*systTmp
  totSyst2 = totSyst1 + (0.018*0.018)
  totSyst1 = ROOT.TMath.Sqrt(totSyst1)
  totSyst2 = ROOT.TMath.Sqrt(totSyst2)

  errTU = ROOT.TMath.Sqrt(errTU)
  errTD = ROOT.TMath.Sqrt(errTD)
  errT = (errTU+errTD)/2.
  print errSum
  print 'Tot syst:  ', errT, ' ', errT/hC.GetBinContent(iB), ' ', totSyst1, ' (from summing) ', totSyst2, ' (include lumi)' 
  print 'Stat unc: ', hC.GetBinError(iB), ' ', hC.GetBinError(iB)/hC.GetBinContent(iB)

