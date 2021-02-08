import ROOT
import os,sys
import math
import array


sys.path.append('/uscms_data/d3/duong/CMSSW/CMSSW_7_6_5/src/ZplusC/python/')

import ConfigParser
import myutils.util_funcs as utl_func
import myutils as utl 

import unfolding_utils as uf_utls

####################################################
####################################################
def getCombinedHist(name,samList,fHistIn,lumiScalesIn,year='All'):
  if year == 'All':
    hOut = uf_utls.getHist(name.replace('Combined','Zmm').replace('combined','zmm'),samList['Zmm'],fHistIn,lumiScalesIn,addYear=True).Clone(name.replace('zmm','Combined'))
    hOut.Add(uf_utls.getHist(name.replace('Combined','Zee').replace('combined','zee'),samList['Zee'],fHistIn,lumiScalesIn,addYear=True))
  else:
    hOut = uf_utls.getHist(name.replace('Combined','Zmm').replace('combined','zmm'),samList['Zmm'],fHistIn,lumiScalesIn,addYear=False)[year].Clone(name.replace('zmm','Combined'))
    hOut.Add(uf_utls.getHist(name.replace('Combined','Zee').replace('combined','zee'),samList['Zee'],fHistIn,lumiScalesIn,addYear=False)[year])

  return hOut

def ScaleHist(h,lum,divideBinWidth=True):
  h.Scale(1./lum)
  if divideBinWidth:
    for iB in range(1,h.GetNbinsX()+1):
      h.SetBinContent(iB,h.GetBinContent(iB)/h.GetBinWidth(iB))
      h.SetBinError(iB,h.GetBinError(iB)/h.GetBinWidth(iB))

def GetScaleUnc(NSCALE,hName,f): #hName is name of central
  hC = f.Get(hName)
  hS_err = hC.Clone(hC.GetName()+'_scaleUnc')
  hS_err.Reset()
  #print pltName+'_gen_16_'+gen+'_'+chan+'_'+syst
  for iSyst in range(9):
    #skip muR muF extreme (muR/muF=4 or muF/muR=4)
    if (iSyst == 2 or iSyst == 6):
      continue
    systN = 'SCALE'+str(iSyst)
    hS = f.Get(hName+'_'+systN)
    for iB in range(1,hS.GetNbinsX()+1):
      err = abs(hS.GetBinContent(iB)-hC.GetBinContent(iB))
      if err > hS_err.GetBinContent(iB):
        hS_err.SetBinContent(iB, err)
  hS_err.Divide(hC)
  return hS_err

def GetScaleUnc1(NSCALE,hName,f): #hName is name of central
  hC = f.Get(hName)
  hS_err = hC.Clone(hC.GetName()+'_scaleUnc')
  hS_err.Reset()
  hS_err_up = hC.Clone(hC.GetName()+'_scaleUnc_up')
  hS_err_up.Reset()
  hS_err_down = hC.Clone(hC.GetName()+'_scaleUnc_down')
  hS_err_down.Reset()

  #print pltName+'_gen_16_'+gen+'_'+chan+'_'+syst
  for iSyst in range(9):
    #skip muR muF extreme (muR/muF=4 or muF/muR=4)
    if (iSyst == 2 or iSyst == 6):
      continue
    systN = 'SCALE'+str(iSyst)
    hS = f.Get(hName+'_'+systN)
    for iB in range(1,hS.GetNbinsX()+1):
      err = hS.GetBinContent(iB)-hC.GetBinContent(iB)
      if abs(err) > hS_err.GetBinContent(iB):
        hS_err.SetBinContent(iB, abs(err))
      if err > hS_err_up.GetBinContent(iB):
        hS_err_up.SetBinContent(iB,err)
      if err < hS_err_down.GetBinContent(iB):
        hS_err_down.SetBinContent(iB,err)
  
  hS_relErr = hS_err.Clone(hS_err.GetName().replace('_scaleUnc','_scaleUnc_relErr'))
  hS_err.Divide(hC)
  return hS_relErr,hS_err,hS_err_up,hS_err_down,hC

def GetPDFUnc(NPDF,hName,fOut):
  hC = fOut.Get(hName)
  nPDF = 0
  for iSyst in range(NPDF):
    syst = 'PDF'+str(iSyst)
    if nPDF == 0:
      hM = fOut.Get(hName+'_'+syst).Clone(hName+"_Mean")
      hM2 = fOut.Get(hName+'_'+syst).Clone(hName+"_Meam2")
      hM2.Multiply(hM2)
    if nPDF != 0:
      hTmp = fOut.Get(hName+'_'+syst)
      hM.Add(hTmp)
      hTmp.Multiply(hTmp)
      hM2.Add(hTmp)
    nPDF = nPDF + 1
  #mean
  hM.Scale(1./nPDF)
  #mean of square
  hM2.Scale(1./nPDF)
  #square of mean
  hM.Multiply(hM)
  #mean of square - square of mean
  hM2.Add(hM,-1.0)
  hM2.Scale(nPDF/(nPDF-1))
  for iB in range(1,hM2.GetNbinsX()+1):
    hM2.SetBinContent(iB,math.sqrt(hM2.GetBinContent(iB)))
  hPDF_err = hM2.Clone(hName+'_PDFunc')
  hPDF_err.Divide(hC)
  return hPDF_err

def GetPdfUncEnvelop(NSCALE,hName,f): #hName is name of central
  hC = f.Get(hName)
  hS_err = hC.Clone(hC.GetName()+'_scaleUnc')
  hS_err.Reset()
  #print pltName+'_gen_16_'+gen+'_'+chan+'_'+syst
  for iSyst in range(9):
    #skip muR muF extreme (muR/muF=4 or muF/muR=4)
    if (iSyst == 2 or iSyst == 6):
      continue
    systN = 'SCALE'+str(iSyst)
    hS = f.Get(hName+'_'+systN)
    for iB in range(1,hS.GetNbinsX()+1):
      err = abs(hS.GetBinContent(iB)-hC.GetBinContent(iB))
      if err > hS_err.GetBinContent(iB):
        hS_err.SetBinContent(iB, err)
  hS_err.Divide(hC)
  return hS_err

####################################################
#######
####################################################

fOut = ROOT.TFile.Open('../Output_theory_pred/theory_pred.root','RECREATE')

cfg = utl.BetterConfigParser()
cfg.read('../Configs/config.ini')

systs = ['NONE']
systs_scale = []
systs_pdf = []
NPDF = 101
NSCALE = 9
for i in range(NSCALE):
  systs_scale.append('SCALE'+str(i))
for i in range(NPDF):
  systs_pdf.append('PDF'+str(i))
systs = systs + systs_scale + systs_pdf

print '>>>>>Syst>>>>>>>>'
print systs


lumis = {}
totLumis = 0
for y in ['16','17','18']:
  lumis[y] = float(cfg.get('General','lumi_'+y))
  totLumis += lumis[y]

yy = '16'

#pltNames = ['pt_b1','dR_2b','pt_Z','dR_2b','dPhi_2b','dRmin_Z2b','A_Z2b','m_2b','m_Z2b']
pltNames = ['pt_b1','pt_b2','pt_Z','dR_2b','dRmin_Z2b','A_Z2b','m_2b','m_Z2b']
#pltNames=['pt_b1']
chans = ['Electron','Muon','Combined']

for syst in systs:
  print "###########################################"
  print 'Running on SYST: ', syst
  
  hName = "Int_theo_xSec_amcnlo"
  if syst != 'NONE':
    hName = hName + '_' + syst
  hTot_amcnlo = ROOT.TH1D(hName,"",3,0,3)
  hTot_amcnlo.GetXaxis().SetBinLabel(1,'Ele')
  hTot_amcnlo.GetXaxis().SetBinLabel(2,'Muon')
  hTot_amcnlo.GetXaxis().SetBinLabel(3,'Combined')

  hTot_16_amcnlo = ROOT.TH1D(hName.replace('amcnlo','16_amcnlo'),"",3,0,3)
  hTot_16_amcnlo.GetXaxis().SetBinLabel(1,'Ele')
  hTot_16_amcnlo.GetXaxis().SetBinLabel(2,'Muon')
  hTot_16_amcnlo.GetXaxis().SetBinLabel(3,'Combined')
  
  hTot_mg = ROOT.TH1D(hName.replace('amcnlo','mg'),"",3,0,3)
  hTot_mg.GetXaxis().SetBinLabel(1,'Ele')
  hTot_mg.GetXaxis().SetBinLabel(2,'Muon')
  hTot_mg.GetXaxis().SetBinLabel(3,'Combined')

  hTot_16_mg = ROOT.TH1D(hName.replace('amcnlo','16_mg'),"",3,0,3)
  hTot_16_mg.GetXaxis().SetBinLabel(1,'Ele')
  hTot_16_mg.GetXaxis().SetBinLabel(2,'Muon')
  hTot_16_mg.GetXaxis().SetBinLabel(3,'Combined')

  
  fNames,xSecs,lumiScales,fHist = uf_utls.Setup(syst=syst,cfg=cfg)
  print xSecs

  #loop over plot name
  for pltName in pltNames:
    
    print "###########################################"
    print 'Running on plot: ', pltName
    #loop over channels
    for chan in chans:
      print "##############################"
      print "Channel: ", chan
      name1 = 'Zmm'
      if chan == 'Electron': name1 = 'Zee'
      if chan == 'Combined': name1 = 'Combined'
 
      #"pt_b1_gen_all_zmm"
      nameAll = pltName + '_gen_all_'+name1.lower()
      if syst != 'NONE':
        nameAll = nameAll + '_' + syst.lower()
      if chan != 'Combined':
        hGen = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True)
        hGen_16 = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=False)[yy]
        print ">>>>>>>>>", nameAll
        hGen_mg = uf_utls.getHist(nameAll,['DY_MG'],fHist,lumiScales,addYear=True)
        hGen_16_mg = uf_utls.getHist(nameAll,['DY_MG'],fHist,lumiScales,addYear=False)[yy]
        #hGen_sherpa = uf_utls.getHist(nameAll,['DY_Sherpa'],fHist,lumiScales,addYear=False)[yy] #only 16 available for sherpa
        #print nameAll, hGen_sherpa.Integral()
      else:
        hGen = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')
        hGen_16 = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year=yy)
        hGen_mg = getCombinedHist(nameAll,{'Zee':['DY_MG'],'Zmm':['DY_MG']},fHist,lumiScales,year='All')
        hGen_16_mg = getCombinedHist(nameAll,{'Zee':['DY_MG'],'Zmm':['DY_MG']},fHist,lumiScales,year=yy)

      hGen = uf_utls.customBin(hGen,uf_utls.GENBINS[pltName])
      hGen_16 = uf_utls.customBin(hGen_16,uf_utls.GENBINS[pltName])
      hGen_mg = uf_utls.customBin(hGen_mg,uf_utls.GENBINS[pltName])
      hGen_16_mg = uf_utls.customBin(hGen_16_mg,uf_utls.GENBINS[pltName])
      #hGen_sherpa  = uf_utls.customBin(hGen_sherpa,uf_utls.GENBINS)
      
      hGen_final = hGen.Clone(hGen.GetName()+'_final')
      hGen_16_final = hGen_16.Clone(hGen_16.GetName()+'_final')
      hGen_mg_final = hGen_mg.Clone(hGen_mg.GetName()+'_final')
      hGen_16_mg_final = hGen_16_mg.Clone(hGen_16_mg.GetName()+'_final')
      #hGen_sherpa_final = hGen_sherpa.Clone(hGen_sherpa.GetName()+'_final')
      if chan == 'Combined':
        hGen_final.Scale(0.5)
        hGen_16_final.Scale(0.5)
        hGen_mg_final.Scale(0.5)
        hGen_16_mg_final.Scale(0.5)
        #hGen_sherpa_final.Scale(0.5)
      
      tot_amcnlo = hGen_final.Integral(0,1000)/totLumis
      tot_amcnlo_16 = hGen_16_final.Integral(0,1000)/lumis[yy]
      tot_mg= hGen_mg_final.Integral(0,1000)/totLumis
      tot_mg_16 = hGen_16_mg_final.Integral(0,1000)/lumis[yy]
      
      if pltName == 'pt_b1':
        if chan == 'Electron': 
          hTot_amcnlo.Fill(0.5, tot_amcnlo)
          hTot_16_amcnlo.Fill(0.5, tot_amcnlo_16)
          hTot_mg.Fill(0.5, tot_mg)
          hTot_16_mg.Fill(0.5, tot_mg_16)
        if chan == 'Muon': 
          hTot_amcnlo.Fill(1.5, tot_amcnlo)
          hTot_16_amcnlo.Fill(1.5, tot_amcnlo_16)
          hTot_mg.Fill(1.5, tot_mg)
          hTot_16_mg.Fill(1.5, tot_mg_16)
        if chan == 'Combined': 
          hTot_amcnlo.Fill(2.5, tot_amcnlo)
          hTot_16_amcnlo.Fill(2.5, tot_amcnlo_16)
          hTot_mg.Fill(2.5, tot_mg)
          hTot_16_mg.Fill(2.5, tot_mg_16)
          
      print "amcnlo:       ", hGen_final.Integral()/totLumis, " ", hGen_final.Integral(1,1000)/totLumis
      print yy + " amcnlo:       ", hGen_16_final.Integral()/lumis[yy], " ", hGen_16_final.Integral(1,1000)/lumis[yy]
      print "mg:             ", hGen_mg_final.Integral()/totLumis, " ", hGen_mg_final.Integral(1,1000)/totLumis
      print yy + " mg:       ", hGen_16_mg_final.Integral()/lumis[yy], " ", hGen_16_mg_final.Integral(1,1000)/lumis[yy]
      #print "sherpa:         ", hGen_sherpa_final.Integral()/lumis[yy], " ", hGen_sherpa_final.Integral(1,1000)/lumis[yy]
      
      hName = pltName+'_gen_amcnlo_'+chan
      if syst != 'NONE': hName = hName+'_'+syst
      hGen_final.SetName(hName)
      hGen_16_final.SetName(hName.replace('amcnlo','16_amcnlo'))
      hGen_mg_final.SetName(hName.replace('amcnlo','mg'))
      hGen_16_mg_final.SetName(hName.replace('amcnlo','16_mg'))

      hName = pltName+'_norm_gen_amcnlo_'+chan
      if syst != 'NONE': hName = hName+'_'+syst 
      hGen_final_norm = hGen_final.Clone(hName)
      hGen_final_norm.Scale(1./tot_amcnlo)
      hGen_16_final_norm = hGen_16_final.Clone(hName.replace('amcnlo','16_amcnlo'))
      hGen_16_final_norm.Scale(1./tot_amcnlo_16)
      hGen_mg_final_norm = hGen_mg_final.Clone(hName.replace('amcnlo','mg'))
      hGen_mg_final_norm.Scale(1./tot_mg)
      hGen_16_mg_final_norm = hGen_16_mg_final.Clone(hName.replace('amcnlo','16_mg'))
      hGen_16_mg_final_norm.Scale(1./tot_mg_16)

      #hGen_mg_final.SetName(pltName+'_gen_mg_'+chan)
      #hGen_sherpa_final.SetName(pltName+'_gen_sherpa_'+chan)
      #################
      #scale histogram, divide binwidth
      #################
      ScaleHist(hGen_final,totLumis)
      ScaleHist(hGen_16_final,lumis[yy])
      ScaleHist(hGen_mg_final,totLumis)
      ScaleHist(hGen_16_mg_final,lumis[yy])
      #ScaleHist(hGen_sherpa_final,lumis[yy])
      

      fOut.cd()
      hGen_final.Write()
      hGen_16_final.Write()
      hGen_mg_final.Write()
      hGen_16_mg_final.Write()
      #hGen_mg_final.Write()
      #hGen_sherpa_final.Write()
      hGen_final_norm.Write()
      hGen_16_final_norm.Write()
      hGen_mg_final_norm.Write()
      hGen_16_mg_final_norm.Write()
  
  fOut.cd()
  hTot_amcnlo.Write()
  hTot_16_amcnlo.Write()
  hTot_mg.Write()
  hTot_16_mg.Write()


for chan in chans:
  for pltName in pltNames:
    for gen in ['amcnlo','mg']:
      for pltType in ['gen','norm_gen']:
        #pt_b1_gen_16_amcnlo_Muon_SCALE2
        hName = pltName+'_'+pltType+'_16_'+gen+'_'+chan
        hScaleUnc = GetScaleUnc(NSCALE,hName,fOut)
        hScaleUnc.Print("all")
        #pt_b1_gen_16_amcnlo_Muon_PDF2
        hPdfUnc = GetPDFUnc(NPDF,hName,fOut)
        hPdfUnc.Print("all")
        #pt_b1_gen_16_mg_Electron_scaleUnc
        hTheoUnc = hScaleUnc.Clone(hScaleUnc.GetName().replace("scaleUnc","theoUnc"))
        hTheoUnc.Multiply(hTheoUnc)
        hTmp = hPdfUnc.Clone(hPdfUnc.GetName().replace("pdfUnc","pdfUncSquared"))
        hTmp.Multiply(hTmp)
        hTheoUnc.Add(hTmp)
        for iB in range(1,hTheoUnc.GetNbinsX()+1):
          hTheoUnc.SetBinContent(iB, math.sqrt(hTheoUnc.GetBinContent(iB)))
        hPdfUnc.Print("all")
        hScaleUnc.Print("all")
        hTheoUnc.Print("all")
        fOut.cd()
        hPdfUnc.Write()
        hScaleUnc.Write()
        hTheoUnc.Write()


for s in ['amcnlo','mg']:
  hName = "Int_theo_xSec_16"
  hInt_scaleUnc = GetScaleUnc(NSCALE,hName+"_"+s,fOut)
  #hInt_scaleUnc.Print("all")
  hInt_pdfUnc = GetPDFUnc(NPDF,hName+"_"+s,fOut)
  #hInt_pdfUnc.Print("all")
  fOut.cd()
  hC = fOut.Get(hName+'_'+s)
  print 'Integrated xSec ('+s+'):'
  for i in range(1,4):
    xSec = hC.GetBinContent(i)
    ch = 'Electron'
    if i == 2: ch = 'Muon    '
    if i == 3: ch = 'Combined'
    print ch + ': ', xSec, ' +/- ', hInt_scaleUnc.GetBinContent(i)*xSec, ' (scale) +/- ', hInt_pdfUnc.GetBinContent(i)*xSec, ' (pdf)'

  hInt_scaleUnc.Write()
  hInt_pdfUnc.Write()

