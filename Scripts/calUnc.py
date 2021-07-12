import os,sys
import ROOT

#f = ROOT.TFile.Open('../Output_unfolding_postProc/unfolding.root','READ')
#fStat = ROOT.TFile.Open('../Output_unfolding/unfolding_central_withNormDistributions.root','READ')
#fBkgrUnc = ROOT.TFile.Open('../Output_unfolding/unfolding_bkgrUnc.root','READ')
f = ROOT.TFile.Open('../Output_unfolding_postProc/unfolding_withJETMET_correlation.root','READ')
fStat = ROOT.TFile.Open('../Output_unfolding/unfolding_central_withNormDistributions.root','READ')
fBkgrUnc = ROOT.TFile.Open('../Output_unfolding/unfolding_bkgrUnc.root','READ')

useJETMET_correlation = True 

hC = f.Get('Int_xSec_NONE')
hCstat = fStat.Get('Int_xSec_NONE')
hCBkgr = fBkgrUnc.Get('Int_xSec_NONE')

scaleUnc = {'Electron':0.025,'Muon':0.023,'Combined':0.025}
pdfUnc = {'Electron':0.003,'Muon':0.003,'Combined':0.003}
asUnc = {'Electron':0.001,'Muon':0.001,'Combined':0.001}

debug = False 
for chan in ['Electron','Muon','Combined']:
  print '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>'
  print 'Chan: ', chan
  errTU = 0
  errTD = 0
  if useJETMET_correlation:
    upS=['JESU_METJESU','JERU_METJERU','PUU','L1PREFIRINGU','BTAGU','ELEU','MUONU','METUNCLUSTU','TTU','DYXSECU','WWXSECU','WZXSECU','ZZXSECU']
    downS=['JESD_METJESD','JERD_METJERD','PUD','L1PREFIRINGD','BTAGD','ELED','MUOND','METUNCLUSTD','TTD','DYXSECD','WWXSECD','WZXSECD','ZZXSECD']
  else:
    upS=['JESU','JERU','PUU','L1PREFIRINGU','BTAGU','ELEU','MUONU','METJESU','METJERU','METUNCLUSTU','TTU','DYXSECU','WWXSECU','WZXSECU','ZZXSECU']
    downS=['JESD','JERD','PUD','L1PREFIRINGD','BTAGD','ELED','MUOND','METJESD','METJERD','METUNCLUSTD','TTD','DYXSECD','WWXSECD','WZXSECD','ZZXSECD']

  systs = upS + downS
  errSum = {'JES':[],'JER':[],'PU':[],'L1PREFIRING':[],'BTAG':[],'ELE':[],'MUON':[],'METJES':[],'METJER':[],'METUNCLUST':[], 'TT':[], 'DYXSEC':[], 'WWXSEC':[],'WZXSEC':[],'ZZXSEC':[]}
  if useJETMET_correlation:
    errSum = {'JES_METJES':[],'JER_METJER':[],'PU':[],'L1PREFIRING':[],'BTAG':[],'ELE':[],'MUON':[],'METUNCLUST':[], 'TT':[], 'DYXSEC':[], 'WWXSEC':[],'WZXSEC':[],'ZZXSEC':[]}

  errSumMET = {'METJES':[],'METJER':[],'METUNCLUST':[]}
  if useJETMET_correlation:
    errSumMET = {'METUNCLUST':[]}

  for syst in systs: 
    iB = 1
    if chan == 'Muon': iB = 2
    if chan == 'Combined': iB = 3
    hInt_xSec = f.Get('Int_xSec_'+syst)
    hInt_xSec_bkgrUnc = fBkgrUnc.Get('Int_xSec_'+syst)
    if 'TT' in syst or 'DY' in syst or 'WW' in syst or 'WZ' in syst or 'ZZ' in syst:
      derr = hInt_xSec_bkgrUnc.GetBinContent(iB) - hCBkgr.GetBinContent(iB)
      cen = hCBkgr.GetBinContent(iB)
    else:
      derr = hInt_xSec.GetBinContent(iB) - hC.GetBinContent(iB)
      cen = hC.GetBinContent(iB)

    if syst in upS:
      errTU += derr*derr
    if syst in downS:
      errTD += derr*derr
    relErr = derr/cen
    if debug: print syst, ' ', derr, ' ', relErr
    for k in errSum.keys():
      tmp = syst[:-1]
      if k in ['JES_METJES','JER_METJER']:
        tmp = tmp.replace('JESU','JES')
        tmp = tmp.replace('JESD','JES')
        tmp = tmp.replace('JERU','JER')
        tmp = tmp.replace('JERD','JER')
      if k == tmp:
        errSum[k].append(relErr)
    for k in errSumMET.keys():
      if k == syst[:-1]:
        errSumMET[k].append(relErr)

  print ">>>>Uncertainty breakdown>>>>>"
  totSyst1 = 0
  print errSum.keys()
  for k in errSum.keys():
    #systTmp = (abs(errSum[k][0])+abs(errSum[k][1]))/2
    systTmp = max(abs(errSum[k][0]),abs(errSum[k][1]))
    print k, " ", systTmp
    totSyst1 += systTmp*systTmp
  
  totSystJETMET = 0
  for k in ['JES_METJES','JER_METJER']:
    systTmp = max(abs(errSum[k][0]),abs(errSum[k][1]))
    totSystJETMET += systTmp*systTmp
    
  totSystMET = 0
  for k in errSumMET.keys():
    #systTmp = (abs(errSumMET[k][0])+abs(errSumMET[k][1]))/2
    systTmp = max(abs(errSumMET[k][0]),abs(errSumMET[k][1]))
    #print k, " ", systTmp
    totSystMET += systTmp*systTmp
  
  totSystBkgr = 0
  for k in ['TT','DYXSEC','WWXSEC','WZXSEC','ZZXSEC']:
    systTmp = max(abs(errSum[k][0]),abs(errSum[k][1]))
    totSystBkgr+= systTmp*systTmp
  
  totTheoUnc = 0
  totTheoUnc = scaleUnc[chan]*scaleUnc[chan]+pdfUnc[chan]*pdfUnc[chan]+asUnc[chan]*asUnc[chan]

  #totSystMET = ROOT.TMath.Sqrt(totSystMET)
  #print "MET ", totSystMET
  #totSystBkgr = ROOT.TMath.Sqrt(totSystBkgr)
  #print "Bkgr unc.: ", totSystBkgr
  #totSyst1 = ROOT.TMath.Sqrt(totSyst1)
  #totSyst2 = ROOT.TMath.Sqrt(totSyst2)
  
  totSyst = totSyst1 + totTheoUnc + (0.016*0.016)
  totSyst = ROOT.TMath.Sqrt(totSyst)
  totSyst1 = ROOT.TMath.Sqrt(totSyst1) #syst without lumi 
  totTheoUnc = ROOT.TMath.Sqrt(totTheoUnc)
  totSystJETMET = ROOT.TMath.Sqrt(totSystJETMET)
  totSystMET = ROOT.TMath.Sqrt(totSystMET)
  totSystBkgr = ROOT.TMath.Sqrt(totSystBkgr)
  
  print ">>>>>>>>>>>>>>>>>>>>>>>"
  print 'JETMET ', totSystJETMET
  print 'Bkgr ', totSystBkgr
  print 'MET  ', totSystMET


  errTU = ROOT.TMath.Sqrt(errTU)
  errTD = ROOT.TMath.Sqrt(errTD)
  #errT = (errTU+errTD)/2.
  errT = max(abs(errTU),abs(errTD))
  if debug: 
    print 'Errors:    ', errSum
    print 'Syst up:   ', errTU/hC.GetBinContent(iB)
    print 'Syst down: ', errTD/hC.GetBinContent(iB)
  
  xSec = hCstat.GetBinContent(iB)
  statUnc = hCstat.GetBinError(iB)/xSec

  print 'Tot syst:  ', totSyst1, ' (from summing) ', totSyst, ' (include lumi)'

  print 'Stat unc:  ', statUnc
  print 'Xsec:      ', xSec, ' +/- ', xSec*statUnc, ' +/- ', xSec*totSyst, ' +/- ', xSec*totTheoUnc
