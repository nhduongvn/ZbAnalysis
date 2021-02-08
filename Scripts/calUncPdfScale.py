import os,sys
import ROOT
import math

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

def PlotSyst(plt,syst,outFolder,chan,hEU,hED,hComU,hComD):
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
  
  c.Print(outFolder+"/Unc_plot/"+plt+"_"+syst+"_unc_"+chan+".pdf")

def GetScaleUnc(NSCALE,hNameIn,f,chan): #hName is name of central
  hName = hNameIn+'_NONE_' + chan
  if chan == 'All':
    hName = hNameIn+'_NONE'

  hC = f.Get(hName)
  hS_err = hC.Clone(hC.GetName()+'_scaleUnc')
  hS_err.Reset()
  #print pltName+'_gen_16_'+gen+'_'+chan+'_'+syst
  for iSyst in range(9):
    #skip muR muF extreme (muR/muF=4 or muF/muR=4)
    if (iSyst == 2 or iSyst == 6):
      continue
    systN = 'SCALE'+str(iSyst) 
    hName = hNameIn+'_'+systN + '_' + chan
    if chan == 'All':
      hName = hNameIn+'_'+systN
    hS = f.Get(hName)
    for iB in range(1,hS.GetNbinsX()+1):
      err = abs(hS.GetBinContent(iB)-hC.GetBinContent(iB))
      if err > hS_err.GetBinContent(iB):
        hS_err.SetBinContent(iB, err)
  hS_err.Divide(hC)
  return hS_err

def GetScaleUnc1(NSCALE,hNameIn,f,chan): #hName is name of central
  hName = hNameIn+'_NONE_' + chan
  if chan == 'All':
    hName = hNameIn+'_NONE'

  hC = f.Get(hName)
  print 'hC integral: ', hC.Integral(1,10000)
  hS_err = hC.Clone(hC.GetName()+'_scaleUnc')
  hS_err.Reset()
  hS_err_up = hC.Clone(hC.GetName()+'_scaleUnc_up')
  hS_err_up.Reset()
  hS_err_down = hC.Clone(hC.GetName()+'_scaleUnc_down')
  hS_err_down.Reset()
  for iB in range(1,hS_err.GetNbinsX()+1):
    hS_err.SetBinContent(iB,0)
  
  #print pltName+'_gen_16_'+gen+'_'+chan+'_'+syst
  for iSyst in range(9):
    #skip muR muF extreme (muR/muF=4 or muF/muR=4)
    if (iSyst == 2 or iSyst == 6):
      continue
    systN = 'SCALE'+str(iSyst) 
    hName = hNameIn+'_'+systN + '_' + chan
    if chan == 'All':
      hName = hNameIn+'_'+systN
    hS = f.Get(hName)
    for iB in range(1,hS.GetNbinsX()+1):
      err = hS.GetBinContent(iB)-hC.GetBinContent(iB)
      if abs(err) > hS_err.GetBinContent(iB):
        hS_err.SetBinContent(iB, abs(err))
      if err > hS_err_up.GetBinContent(iB):
        hS_err_up.SetBinContent(iB,err)
      if err < hS_err_down.GetBinContent(iB):
        hS_err_down.SetBinContent(iB,err)

  hS_relErr = hS_err.Clone(hS_err.GetName().replace('_scaleUnc','_scaleUnc_relErr'))
  hS_relErr.Divide(hC)
  return hS_relErr,hS_err,hS_err_up,hS_err_down,hC

def GetPDFUnc(NPDF,hNameIn,fOut,chan):
  hNameNone = hNameIn+'_NONE_' + chan
  if chan == 'All':
    hNameNone = hNameIn+'_NONE'
  nPDF = 0
  for iSyst in range(NPDF):
    #use corresponding nominal for each pdf group
    if iSyst in range(35):
      fTmp = fOut[0]
    if iSyst in range(35,70):
      fTmp = fOut[1]
    if iSyst in range(70,102):
      fTmp = fOut[2]

    hC = fTmp.Get(hNameNone)
    
    syst = 'PDF'+str(iSyst)
    hName = hNameIn+'_'+syst + '_' + chan
    if chan == 'All':
      hName = hNameIn+'_'+syst
    if nPDF == 0:
      hM = fTmp.Get(hName).Clone(hName+"_Mean")
      hM2 = fTmp.Get(hName).Clone(hName+"_Meam2")
      hM2.Multiply(hM2)
    if nPDF != 0:
      hTmp = fTmp.Get(hName)
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
#####################################
#####################################

pltType = 'uf' #uf: for differential, uf_norm: for normalized distributions

sourceFolder = '../Output_unfolding/' #for MET uncertainty no missing file

outFolder = '../Output_unfolding/'

fS = ROOT.TFile.Open(sourceFolder+'/unfolding_scale.root','READ')
fPdf = [ROOT.TFile.Open(sourceFolder+'/unfolding_pdfg0.root','READ'),
        ROOT.TFile.Open(sourceFolder+'/unfolding_pdfg1.root','READ'),
        ROOT.TFile.Open(sourceFolder+'/unfolding_pdfg2.root','READ')]
 

NPDF = 102
NSCALE = 9


pltNames = ['pt_b1','pt_b2','pt_Z','dR_2b','dRmin_Z2b','A_Z2b','m_2b','m_Z2b']
pltNames = ['pt_b1']


fOut = ROOT.TFile.Open(outFolder+'/syst_uncertainty_uf_pdfScale_tmp.root','RECREATE')

#loop over plot 
for plt in pltNames:
  for chan in ['Electron','Muon','Combined']:
  #for chan in ['Electron']:
    #print 'Chan: ', chan
    #get central error
    #pt_b1_uf_amcatnlo_NONE_Electron 
    #m_Z2b_uf_amcatnlo_PDF94_Combined
    hPdfUnc = GetPDFUnc(NPDF, plt+'_'+pltType+'_amcatnlo', fPdf, chan)
    hScaleUnc,hScaleUncAbs,hScaleUncUp,hScaleUncDown,hC1 = GetScaleUnc1(NSCALE, plt+'_'+pltType+'_amcatnlo', fS, chan)
    #print hPdfUnc.GetName(), ' ', hScaleUnc.GetName()
    hPdfUnc.SetName(plt+'_'+pltType+'_amcatnlo_pdfUnc_'+chan)
    hScaleUnc.SetName(plt+'_'+pltType+'_amcatnlo_scaleUnc_'+chan)
    hTot = hPdfUnc.Clone(hPdfUnc.GetName().replace('_pdfUnc_','_theoUnc_'))
    hTot.Multiply(hTot)
    hTmp = hScaleUnc.Clone(hScaleUnc.GetName()+'_clone')
    hTmp.Multiply(hTmp)
    hTot.Add(hTmp)
    for iB in range(1,hTot.GetNbinsX()+1):
      hTot.SetBinContent(iB,math.sqrt(hTot.GetBinContent(iB)))

    fOut.cd()
    hPdfUnc.Write()
    hScaleUnc.Write()
    hTot.Write()
    
    errPdf = 0
    for iB in range(hPdfUnc.GetNbinsX()+1):
      errPdf += hPdfUnc.GetBinContent(iB)*hC1.GetBinContent(iB)

    errPdf = errPdf/hC1.Integral()

    errU = hScaleUncUp.Integral()/hC1.Integral()
    errD = hScaleUncDown.Integral()/hC1.Integral()
    err1 = (errU-errD)/2. 
    err = hScaleUncAbs.Integral()/hC1.Integral()
    print ">>>>>>>>"+chan+">>>>>>>>>>"
    print 'Scale unc. up, down:       ', errU, ' ', errD
    print 'Scale unc. (max, average): ', err, ' ', err1
    print 'Pdf unc                  : ', errPdf

hPdfUnc = GetPDFUnc(NPDF, 'Int_xSec_noUF', fPdf, 'All')
hScaleUnc = GetScaleUnc(NSCALE, 'Int_xSec_noUF', fS, 'All')
hPdfUnc.SetName('Int_xSec_amcatnlo_pdfUnc')
hScaleUnc.SetName('Int_xSec_amcatnlo_scaleUnc')
hTot = hPdfUnc.Clone(hPdfUnc.GetName().replace('_pdfUnc','_theoUnc'))
hTot.Multiply(hTot)
hTmp = hScaleUnc.Clone(hScaleUnc.GetName()+'_clone')
hTmp.Multiply(hTmp)
hTot.Add(hTmp)
for iB in range(1,hTot.GetNbinsX()+1):
  hTot.SetBinContent(iB,math.sqrt(hTot.GetBinContent(iB)))

print 'Uncertainty using total cross section (scale, pdf, total): '
print 'Electron: ', hScaleUnc.GetBinContent(1), ' ', hPdfUnc.GetBinContent(1), ' ', hTot.GetBinContent(1)
print 'Muon:     ', hScaleUnc.GetBinContent(2), ' ', hPdfUnc.GetBinContent(2), ' ', hTot.GetBinContent(2)
print 'Combined: ', hScaleUnc.GetBinContent(3), ' ', hPdfUnc.GetBinContent(3), ' ', hTot.GetBinContent(3)


fOut.cd()
hPdfUnc.Write()
hScaleUnc.Write()
hTot.Write()

