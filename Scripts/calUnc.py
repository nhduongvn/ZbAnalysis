import os,sys
import ROOT

f = ROOT.TFile.Open('../Output_unfolding_postProc/unfolding.root','READ')
hC = f.Get('Int_xSec_NONE')
debug = False
for chan in ['Electron','Muon','Combined']:
  print '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>'
  print 'Chan: ', chan
  errTU = 0
  errTD = 0
  upS=['JESU','JERU','PUU','L1PREFIRINGU','BTAGU','ELEU','MUONU','METJESU','METJERU','METUNCLUSTU']
  downS=['JESD','JERD','PUD','L1PREFIRINGD','BTAGD','ELED','MUOND','METJESD','METJERD','METUNCLUSTD']
  systs = upS + downS
  errSum = {'JES':[],'JER':[],'PU':[],'L1PREFIRING':[],'BTAG':[],'ELE':[],'MUON':[],'METJES':[],'METJER':[],'METUNCLUST':[]}
  errSumMET = {'METJES':[],'METJER':[],'METUNCLUST':[]}
  for syst in systs: 
    iB = 1
    if chan == 'Muon': iB = 2
    if chan == 'Combined': iB = 3
    hInt_xSec = f.Get('Int_xSec_'+syst)
    hInt_xSec.GetBinContent(iB)
    derr = hInt_xSec.GetBinContent(iB) - hC.GetBinContent(iB)

    if syst in upS:
      errTU += derr*derr
    if syst in downS:
      errTD += derr*derr
    relErr = derr/hC.GetBinContent(iB)
    if debug: print syst, ' ', derr, ' ', relErr
    for k in errSum.keys():
      if k == syst[:-1]:
        errSum[k].append(relErr)
    for k in errSumMET.keys():
      if k == syst[:-1]:
        errSumMET[k].append(relErr)

  print ">>>>Uncertainty breakdown>>>>>"
  totSyst1 = 0
  for k in errSum.keys():
    #systTmp = (abs(errSum[k][0])+abs(errSum[k][1]))/2
    systTmp = max(abs(errSum[k][0]),abs(errSum[k][1]))
    print k, " ", systTmp
    totSyst1 += systTmp*systTmp

  totSystMET = 0
  for k in errSumMET.keys():
    #systTmp = (abs(errSumMET[k][0])+abs(errSumMET[k][1]))/2
    systTmp = max(abs(errSumMET[k][0]),abs(errSumMET[k][1]))
    #print k, " ", systTmp
    totSystMET += systTmp*systTmp
  totSystMET = ROOT.TMath.Sqrt(totSystMET)
  print "MET ", totSystMET

  totSyst2 = totSyst1 + (0.018*0.018)
  totSyst1 = ROOT.TMath.Sqrt(totSyst1)
  totSyst2 = ROOT.TMath.Sqrt(totSyst2)

  errTU = ROOT.TMath.Sqrt(errTU)
  errTD = ROOT.TMath.Sqrt(errTD)
  #errT = (errTU+errTD)/2.
  errT = max(abs(errTU),abs(errTD))
  if debug: print errSum
  print 'Tot syst:  ', errT, ' ', errT/hC.GetBinContent(iB), ' ', totSyst1, ' (from summing) ', totSyst2, ' (include lumi)' 
  print 'Stat unc: ', hC.GetBinError(iB), ' ', hC.GetBinError(iB)/hC.GetBinContent(iB)

