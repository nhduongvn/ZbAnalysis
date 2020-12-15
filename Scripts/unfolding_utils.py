import os,sys
import ROOT
import math
import array

sys.path.append('/uscms_data/d3/duong/CMSSW/CMSSW_7_6_5/src/ZplusC/python/')

import ConfigParser
import myutils.util_funcs as utl_func
import myutils as utl 


def EmptyUFOF(h):
  hO = h.Clone(h.GetName()+"_emptyUFOF")
  nBinX = hO.GetNbinsX()
  nBinY = hO.GetNbinsY()
  for iX in range(0, nBinX+2):
    hO.SetBinContent(iX,0,0)
    hO.SetBinError(iX,0,0)
    hO.SetBinContent(iX,nBinY+1,0)
    hO.SetBinError(iX,nBinY+1,0)
  for iY in range(0, nBinY+2):
    hO.SetBinContent(0,iY,0)
    hO.SetBinError(0,iY,0)
    hO.SetBinContent(nBinX+1,iY,0)
    hO.SetBinError(nBinX+1,iY,0)
  return hO

class Unfolder:
  
  def __init__(self, hRec, hBkgr, hRes, hGenAll, hGenNotMatch, genbins, recobins): 
    self.hRec = hRec.Clone(hRec.GetName()+"_1")
    self.hBkgr = hBkgr.Clone(hBkgr.GetName()+"_1")
    self.hRes = hRes.Clone(hRes.GetName()+"_1")
    self.hGenAll = hGenAll.Clone(hGenAll.GetName()+"_1")
    self.hGenNotMatch = hGenNotMatch.Clone(hGenNotMatch.GetName()+"_1")
    self.genbins = genbins
    self.recobins = recobins
  
  def CalEfficiency(self):
    self.hEff = self.hGenAll.Clone(self.hGenAll.GetName()+"_eff")
    self.hEff.Add(self.hGenNotMatch,-1)
    self.hEff.Divide(self.hGenAll)
    for i in range(1,self.hEff.GetNbinsX()+1):
      print self.hEff.GetBinContent(i)

  def EmptyUFOF(self):
    self.hRes1 = self.hRes.Clone(self.hRes.GetName()+"_emptyUFOF")
    nBinX = self.hRes1.GetNbinsX()
    nBinY = self.hRes1.GetNbinsY()
    for iX in range(0, nBinX+2):
      self.hRes1.SetBinContent(iX,0,0)
      self.hRes1.SetBinError(iX,0,0)
      self.hRes1.SetBinContent(iX,nBinY+1,0)
      self.hRes1.SetBinError(iX,nBinY+1,0)
    for iY in range(0, nBinY+2):
      self.hRes1.SetBinContent(0,iY,0)
      self.hRes1.SetBinError(0,iY,0)
      self.hRes1.SetBinContent(nBinX+1,iY,0)
      self.hRes1.SetBinError(nBinX+1,iY,0)

  def Unfold(self):
    self.CalEfficiency()
    self.EmptyUFOF()
    generatorBinning = ROOT.TUnfoldBinning("generator")
    detectorBinning = ROOT.TUnfoldBinning("detector")
    generatorBinning.AddAxis("genpt", len(self.genbins) - 1, self.genbins, True, True )
    detectorBinning.AddAxis("recopt", len(self.recobins) - 1, self.recobins, True, True )
    unfold = ROOT.TUnfoldDensity(self.hRes1,ROOT.TUnfold.kHistMapOutputHoriz,ROOT.TUnfold.kRegModeSize, ROOT.TUnfold.kEConstraintArea, ROOT.TUnfoldDensity.kDensityModeBinWidth,generatorBinning,detectorBinning)
    status = unfold.SetInput(self.hRec)
    unfold.SubtractBackground(self.hBkgr,"background1")
    nScan = 1000
    #iBest = unfold.ScanTau( nScan, 0, 0.1, ROOT.MakeNullPointer(ROOT.TSpline), ROOT.TUnfoldDensity.kEScanTauRhoSquareAvg, 'generator','*[OUB]')
    unfold.DoUnfold(0.)

    print '>>>>>>Unfold status ... '
    print 'Status:        ', status
    print 'Tau:           ', unfold.GetTau()
    print 'Chi**2:        ', unfold.GetChi2A(), unfold.GetChi2L(), unfold.GetNdf()
    print 'Chi**2 (syst): ', unfold.GetChi2Sys()
    self.hUF = unfold.GetOutput("Unfolded")
    self.hUF.Divide(self.hEff)

class Unfolder1:
  
  def __init__(self, hRec, hBkgr, hRes, hGenAll, hGenMatch, genbins, recobins): 
    self.hRec = hRec.Clone(hRec.GetName()+"_1")
    self.hBkgr = hBkgr.Clone(hBkgr.GetName()+"_1")
    self.hRes = hRes.Clone(hRes.GetName()+"_1")
    self.hGenAll = hGenAll.Clone(hGenAll.GetName()+"_1")
    self.hGenMatch = hGenMatch.Clone(hGenMatch.GetName()+"_1")
    self.genbins = genbins
    self.recobins = recobins
  
  def CalEfficiency(self):
    self.hEff = self.hGenMatch.Clone(self.hGenMatch.GetName()+"_eff")
    self.hEff.Divide(self.hGenAll)
    for i in range(1,self.hEff.GetNbinsX()+1):
      print self.hEff.GetBinContent(i)

  def EmptyUFOF(self):
    self.hRes1 = self.hRes.Clone(self.hRes.GetName()+"_emptyUFOF")
    nBinX = self.hRes1.GetNbinsX()
    nBinY = self.hRes1.GetNbinsY()
    for iX in range(0, nBinX+2):
      self.hRes1.SetBinContent(iX,0,0)
      self.hRes1.SetBinError(iX,0,0)
      self.hRes1.SetBinContent(iX,nBinY+1,0)
      self.hRes1.SetBinError(iX,nBinY+1,0)
    for iY in range(0, nBinY+2):
      self.hRes1.SetBinContent(0,iY,0)
      self.hRes1.SetBinError(0,iY,0)
      self.hRes1.SetBinContent(nBinX+1,iY,0)
      self.hRes1.SetBinError(nBinX+1,iY,0)

  def Unfold(self):
    self.CalEfficiency()
    self.EmptyUFOF()
    generatorBinning = ROOT.TUnfoldBinning("generator")
    detectorBinning = ROOT.TUnfoldBinning("detector")
    generatorBinning.AddAxis("genpt", len(self.genbins) - 1, self.genbins, True, True )
    detectorBinning.AddAxis("recopt", len(self.recobins) - 1, self.recobins, True, True )
    unfold = ROOT.TUnfoldDensity(self.hRes1,ROOT.TUnfold.kHistMapOutputHoriz,ROOT.TUnfold.kRegModeSize, ROOT.TUnfold.kEConstraintArea, ROOT.TUnfoldDensity.kDensityModeBinWidth,generatorBinning,detectorBinning)
    status = unfold.SetInput(self.hRec)
    unfold.SubtractBackground(self.hBkgr,"background1")
    nScan = 1000
    #iBest = unfold.ScanTau( nScan, 0, 0.1, ROOT.MakeNullPointer(ROOT.TSpline), ROOT.TUnfoldDensity.kEScanTauRhoSquareAvg, 'generator','*[OUB]')
    unfold.DoUnfold(0.)

    print '>>>>>>Unfold status ... '
    print 'Status:        ', status
    print 'Tau:           ', unfold.GetTau()
    print 'Chi**2:        ', unfold.GetChi2A(), unfold.GetChi2L(), unfold.GetNdf()
    print 'Chi**2 (syst): ', unfold.GetChi2Sys()
    self.hUF = unfold.GetOutput("Unfolded")
    print "Axis limit hEff: ", self.hEff.GetXaxis().GetXmin()," ", self.hEff.GetXaxis().GetXmax()
    print "Axis limit hUD:  ", self.hUF.GetXaxis().GetXmin()," ", self.hUF.GetXaxis().GetXmax()
    self.hUF.Divide(self.hEff)


def Setup(cfg, runSyst = False, exclude_spliting_dy=True):
  
  fNames = {}
  xSecs = {}
  lumiScales = {}
  fHist = {}
  
  #sNames = ['Electron','Muon','DY_0J','DY_1J','DY_2J','DY_0J_p1','DY_1J_p1','DY_2J_p1','DY_0J_p2','DY_1J_p2','DY_2J_p2','DY_MG','TT','ST','WW','WZ','ZZ']
  #sNames = ['Electron','Muon','DY_0J','DY_1J','DY_2J','DY_0J_p1','DY_1J_p1','DY_2J_p1','DY_0J_p2','DY_1J_p2','DY_2J_p2','DY_MG','DY_Sherpa','TT','ST','WW','WZ','ZZ']
  #for syst
  sNames = ['Electron','Muon','DY_0J','DY_1J','DY_2J','DY_0J_p1','DY_1J_p1','DY_2J_p1','DY_0J_p2','DY_1J_p2','DY_2J_p2','DY_MG','DY_Sherpa','TT','ST','WW','WZ','ZZ']
  #sNames = ['DY_0J','DY_1J','DY_2J']
  if exclude_spliting_dy:
    #sNames = ['Electron','Muon','DY_0J','DY_1J','DY_2J','DY_MG','TT','ST','WW','WZ','ZZ']
    sNames = ['Electron','Muon','DY_0J','DY_1J','DY_2J','DY_MG','DY_Sherpa','TT','ST','WW','WZ','ZZ']
    #sNames = ['DY_0J','DY_1J','DY_2J']
  if runSyst:
    sNames = ['Electron','Muon','DY_0J','DY_1J','DY_2J','TT','ST','WW','WZ','ZZ']


  for s in sNames: 
  #for s in ['Electron','Muon','DY_0J','DY_1J','DY_2J','DY_0J_p1','TT','ST','WZ','ZZ']:

    fNames[s] = {}
    xSecs[s] = {}
    lumiScales[s] = {}
    fHist[s] = {}
    
    for y in ['16','17','18']:
      #for y in ['17']:
      
      lumi = float(cfg.get('General','lumi_'+y))
      
      names = cfg.get(s,'file_'+y).split(',') #multiple names is possible, for example: Single top = t-channels, s-channels ...
      xSecTmps = [1]*len(names) #each name corresponds to a cross section
      if s not in ['Electron','Muon']:
        xSecTmps = cfg.get(s,'xSec_'+y).split(',')
      
      fNames[s][y] = []
      xSecs[s][y] = []
      xSecTmps = ['1']*len(names) #each name corresponds to a cross section
      kfactor = ['1']*len(names) #each name corresponds to a cross section
      if s not in ['Electron','Muon']:
        xSecTmps = cfg.get(s,'xSec_'+y).split(',')
        xSecTmps = cfg.get(s,'xSec_'+y).split(',')
      
      #input histogram file
      fHist[s][y] = []
      for iN in names:
        if (s != 'DY_Sherpa') or (s == 'DY_Sherpa' and y == '16'): #only 16 available for Sherpa
          path = cfg.get('Paths','path_uf_Data') + '/' + iN
          if s not in ['Electron','Muon','ST','WW','WZ','ZZ']:
            path = cfg.get('Paths','path_uf_MC') + '/' + iN
          #if s in ['ST','WW','WZ','ZZ']:
          #  path = cfg.get('Paths','path_uf_Data') + '/' + iN
          print s, path
          fNames[s][y].append(path) 
          fHist[s][y].append(ROOT.TFile.Open(fNames[s][y][-1],'READ'))

      #for iS in xSecTmps:
      #  xSecs[s][y].append(float(iS))
      for iS in xSecTmps:
        #in case there is kfactor in cross section
        if '*' in iS:
          iS = iS.split('*')
        if len(iS) == 2:
          if (s != 'DY_Sherpa') or (s == 'DY_Sherpa' and y == '16'):
            xSecs[s][y].append(float(iS[0])*float(iS[1]))
        else:
          if (s != 'DY_Sherpa') or (s == 'DY_Sherpa' and y == '16'):
            xSecs[s][y].append(float(iS))


      if (s != 'DY_Sherpa') or (s == 'DY_Sherpa' and y == '16'):
        lumiScales[s][y] = [1]*len(names)
      for iN in range(len(fNames[s][y])):
        #if s not in ['Electron','Muon']:
          
        if (s != 'DY_Sherpa') or (s == 'DY_Sherpa' and y == '16'):
          #print s, y, iN, fNames[s][y][iN]
          lumiScales[s][y][iN] = utl_func.scaleToLumi1(fNames[s][y][iN],xSecs[s][y][iN],lumi)
  
  return  fNames,xSecs,lumiScales,fHist



def getHist(pN,samName,fH,lS,addYear=False): #samName = ['Electron'],['DY_0J','DY_1J','DY_2J']...
  hOut = {}
  for y in ['16','17','18']:
    if 'DY_Sherpa' in samName and y != '16': continue
  #for y in ['17']:
    hOut[y] = fH[samName[0]][y][0].Get(pN).Clone() #first sample, first file
    if samName[0] not in ['Electron','Muon']:
      hOut[y].Scale(lS[samName[0]][y][0])

    for iS in range(len(samName)):
      for fi in range(len(fH[samName[iS]][y])):
        if iS == 0 and fi == 0: continue #first sample and first file is already included above 
        h = fH[samName[iS]][y][fi].Get(pN).Clone()
        if samName[iS] not in ['Electron','Muon']:
          h.Scale(lS[samName[iS]][y][fi])
        hOut[y].Add(h)
      
      #print h.Integral()
      #h.Scale(lS[s][y][0]) #scale to lumi
      #print h.Integral(),fH[s][y][0].Get(pN).Integral()
      #for fi in range(1,len(fH[s][y])):
      #  h.Add(fH[s][y][fi].Get(pN).Scale(lS[s][y][fi]))
  if not addYear:
    return hOut
  else:
    hOutAll = hOut['16'].Clone(hOut['16'].GetName()+'_all')
    hOutAll.Add(hOut['17'])
    hOutAll.Add(hOut['18'])
    return hOutAll


def customBin(h, xDiv):
  hOut = ROOT.TH1D(h.GetName() + '_customBinning','',len(xDiv)-1,array.array('d',xDiv))
  for i in range(1,hOut.GetNbinsX()+1):
    eL = hOut.GetBinLowEdge(i) - 0.000001
    eH = hOut.GetBinLowEdge(i) + hOut.GetBinWidth(i) + 0.000001

    contS = 0
    errS = 0
    for j in range(1,h.GetNbinsX()+1):
      eL1 = h.GetBinLowEdge(j)
      eH1 = eL1 + h.GetBinWidth(j)
      if eL1 >= eL and eH1 <= eH:
        contS += h.GetBinContent(j)
        errS += h.GetBinError(j)*h.GetBinError(j)
    errS = ROOT.TMath.Sqrt(errS)

    hOut.SetBinContent(i,contS)
    hOut.SetBinError(i,errS)
  
  eL = hOut.GetBinLowEdge(1) - 0.000001
  bL = h.GetXaxis().FindFixBin(eL)
  contS = 0
  errS = 0
  for j in range(0,bL+1):
    contS += h.GetBinContent(j)
    errS += h.GetBinError(j)*h.GetBinError(j)
  errS = ROOT.TMath.Sqrt(errS)
  hOut.SetBinError(0,errS)
  hOut.SetBinContent(0,contS)
  
  eL = hOut.GetBinLowEdge(hOut.GetNbinsX()+1) + 0.000001
  bL = h.GetXaxis().FindFixBin(eL)
  contS = 0
  errS = 0
  for j in range(bL,h.GetNbinsX()+2):
    contS += h.GetBinContent(j)
    errS += h.GetBinError(j)*h.GetBinError(j)
  errS = ROOT.TMath.Sqrt(errS)
  hOut.SetBinError(hOut.GetNbinsX()+1,errS)
  hOut.SetBinContent(hOut.GetNbinsX()+1,contS)

  #hOut.SetBinError(hOut.GetNbinsX()+1,h.GetBinError(h.GetNbinsX()+1))
  #hOut.SetBinContent(hOut.GetNbinsX()+1,h.GetBinContent(h.GetNbinsX()+1))

  return hOut


def customBin2D(h, xDiv, yDiv):

  hOut = ROOT.TH2D(h.GetName() + '_customBinning','',len(xDiv)-1,array.array('d',xDiv),len(yDiv)-1,array.array('d',yDiv))
  
  for i in range(1,hOut.GetNbinsX()+1):
    #for i in range(1,2):
    eXL = hOut.GetXaxis().GetBinLowEdge(i) - 0.000001
    eXH = hOut.GetXaxis().GetBinLowEdge(i) + hOut.GetXaxis().GetBinWidth(i) + 0.000001
    for iY in range(1,hOut.GetNbinsY()+1):
      #for iY in range(1,2):
      eYL = hOut.GetYaxis().GetBinLowEdge(iY) - 0.000001
      eYH = hOut.GetYaxis().GetBinLowEdge(iY) + hOut.GetYaxis().GetBinWidth(iY) + 0.000001
      
      #print "====================================="
      #print eXL, eXH, eYL, eYH

      contS = 0
      errS = 0
      for j in range(1,h.GetNbinsX()+1):
        eXL1 = h.GetXaxis().GetBinLowEdge(j)
        eXH1 = eXL1 + h.GetXaxis().GetBinWidth(j)
        for jY in range(1,h.GetNbinsY()+1):
          eYL1 = h.GetYaxis().GetBinLowEdge(jY)
          eYH1 = eYL1 + h.GetYaxis().GetBinWidth(jY)
          #print ">>>", eXL1, eXH1, eYL1, eYH1
          if eXL1 >= eXL and eXH1 <= eXH and eYL1 >= eYL and eYH1 <= eYH:
            #print ">>>Add>>>", eXL1, eXH1, eYL1, eYH1
            contS += h.GetBinContent(j,jY)
            errS += h.GetBinError(j,jY)*h.GetBinError(j,jY)
      errS = ROOT.TMath.Sqrt(errS)

      hOut.SetBinContent(i,iY,contS)
      hOut.SetBinError(i,iY,errS)
  
  '''
  eL = hOut.GetBinLowEdge(1) - 0.000001
  bL = h.GetXaxis().FindFixBin(eL)
  contS = 0
  errS = 0
  for j in range(0,bL+1):
    contS += h.GetBinContent(j)
    errS += h.GetBinError(j)*h.GetBinError(j)
  errS = ROOT.TMath.Sqrt(errS)
  hOut.SetBinError(0,errS)
  hOut.SetBinContent(0,contS)
  
  eL = hOut.GetBinLowEdge(hOut.GetNbinsX()+1) + 0.000001
  bL = h.GetXaxis().FindFixBin(eL)
  contS = 0
  errS = 0
  for j in range(bL,h.GetNbinsX()+2):
    contS += h.GetBinContent(j)
    errS += h.GetBinError(j)*h.GetBinError(j)
  errS = ROOT.TMath.Sqrt(errS)
  hOut.SetBinError(hOut.GetNbinsX()+1,errS)
  hOut.SetBinContent(hOut.GetNbinsX()+1,contS)

  #hOut.SetBinError(hOut.GetNbinsX()+1,h.GetBinError(h.GetNbinsX()+1))
  #hOut.SetBinContent(hOut.GetNbinsX()+1,h.GetBinContent(h.GetNbinsX()+1))
  '''

  return hOut
