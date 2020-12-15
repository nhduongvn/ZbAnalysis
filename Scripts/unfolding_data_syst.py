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
    hOut = uf_utls.getHist(name.replace('Combined','Zmm').replace('combined','zmm'),samList['Zmm'],fHistIn,lumiScalesIn,addYear=True).Clone(name)
    hOut.Add(uf_utls.getHist(name.replace('Combined','Zee').replace('combined','zee'),samList['Zee'],fHistIn,lumiScalesIn,addYear=True))
  else:
    hOut = uf_utls.getHist(name.replace('Combined','Zmm').replace('combined','zmm'),samList['Zmm'],fHistIn,lumiScalesIn,addYear=False)[year].Clone(name)
    hOut.Add(uf_utls.getHist(name.replace('Combined','Zee').replace('combined','zee'),samList['Zee'],fHistIn,lumiScalesIn,addYear=False)[year])

  return hOut

def ScaleHist(h,lum,divideBinWidth=True):
  h.Scale(1./lum)
  if divideBinWidth:
    for iB in range(1,h.GetNbinsX()+1):
      h.SetBinContent(iB,h.GetBinContent(iB)/h.GetBinWidth(iB))
      h.SetBinError(iB,h.GetBinError(iB)/h.GetBinWidth(iB))
####################################################
#######
####################################################

#NONE,PUU,PUD,BTAGU,BTAGD,ELEU,ELED,MUONU,MUOND
fOut = ROOT.TFile.Open('../Output_unfolding_syst/JERD///unfolding.root','RECREATE')
hTot = ROOT.TH1D("Int_xSec","",3,0,3)


cfg = utl.BetterConfigParser()
cfg.read('../Configs/config.ini')

fNames,xSecs,lumiScales,fHist = uf_utls.Setup(cfg,runSyst=True)
print xSecs

lumis = {}
totLumis = 0
for y in ['16','17','18']:
  lumis[y] = float(cfg.get('General','lumi_'+y))
  totLumis += lumis[y]

#pltNames = ['pt_b1','dR_2b','pt_Z','dR_2b','dPhi_2b','dRmin_Z2b','A_Z2b','m_2b','m_Z2b']
pltNames = ['pt_b1','pt_b2','pt_Z','dR_2b','dRmin_Z2b','A_Z2b','m_2b','m_Z2b']
#pltNames=['pt_b1']
chans = ['Electron','Muon','Combined']

#loop over plot name
for pltName in pltNames:
  print "###########################################"
  print "Unfolding: ", pltName
  print "###########################################"
  ################################################
  #Binning skim
  ################################################
  if pltName in ['pt_b1','pt_b2']:
    genbins = array.array('d', [30,50,70,90,110,150,200]) 
    recobins = array.array('d', [30,40,50,60,70,80,90,110,130,150,200])
    if pltName == 'pt_b1': xAtitle='Leading b jet p_{T} [GeV]'
  
  if pltName in ['pt_b2']:
    genbins = array.array('d', [30,40,60,80,110,150,200]) 
    recobins = array.array('d', [30,40,50,60,80,90,110,130,150,200])
    if pltName == 'pt_b2': xAtitle='Sub-leading b jet p_{T} [GeV]'

  if pltName in ['pt_Z']:
    genbins = array.array('d', [0,10,20,40,60,80,100,150,200,300])
    recobins = array.array('d', [0,10,20,30,40,50,60,80,100,120,150,200,300])
    if pltName == 'pt_Z': xAtitle='Z p_{T} [GeV]'
  if pltName == 'dR_2b':
    recobins = array.array('d', [0.4,0.8,1.2,1.6,2.0,2.4,2.8,3.0,3.4,4,4.5,5]) 
    genbins = array.array('d', [0.4,1.2,2.0,2.4,3.0,3.4,4,4.5,5])
    xAtitle='#DeltaR_{bb}'
  if pltName =='dPhi_2b':
    recobins = array.array('d', [0.,0.25,0.5,0.75,1.0,1.25,1.5,1.75,2.0,2.25,2.5,2.75,ROOT.TMath.Pi()]) 
    genbins = array.array('d', [0.,0.5,1.0,1.5,2.0,2.5,2.75,ROOT.TMath.Pi()])
    xAtitle='#DeltaPhi_{bb}'
  if pltName =='dRmin_Z2b':
    recobins = array.array('d', [0.,0.3,0.6,0.9,1.2,1.5,1.8,2.1,2.4,2.7,3,3.3,3.6,4,5])
    genbins = array.array('d', [0.,0.6,1.2,1.8,2.4,2.7,3.0,3.3,4,5])
    xAtitle='Min(#DeltaR_{Zbb})'
  if pltName =='A_Z2b':
    recobins = array.array('d', [0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,1])
    genbins = array.array('d', [0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,1])
    xAtitle='A_{Zbb}'
  if pltName =='m_2b':
    recobins = array.array('d', [20,50,75,100,125,150,175,200,225,250,275,300,350,400,500])
    genbins = array.array('d', [20,50,75,100,125,150,175,200,250,300,400,500])
    xAtitle='M_{bb} [GeV]'
  if pltName =='m_Z2b':
    recobins = array.array('d', [150,200,225,250,275,300,325,350,375,400,425,450,500,600])
    genbins = array.array('d', [150,200,250,300,350,400,450,500,600])
    xAtitle='M_{Zbb} [GeV]'
  
  #loop over channels
  for chan in chans:
    print "##############################"
    print "Channel: ", chan
    name1 = 'Zmm'
    if chan == 'Electron': name1 = 'Zee'
    if chan == 'Combined': name1 = 'Combined'
    name2 = '2bjet'
    #hName = Zmm_2bjet_pt_jet
    #hName1 = Zmm_2bjet_bX_pt_jet"
    #hName2 = Zmm_2bjet_XX_pt_jet
    #hName3 = Zmm_2bjet_bb_pt_jet
    name3 = pltName
    if pltName == 'pt_b1':
      name3 = 'pt_jet'
    if pltName == 'pt_b2':
      name3 = 'pt_jet1'
    if pltName == 'pt_Z':
      name3 = 'pt_dilepton'
      
    #Zmm_2bjet_pt_jet
    nameAll = name1 + '_' + name2 + '_' + name3
    if chan != "Combined":
      hRec_data_all = uf_utls.getHist(nameAll,[chan],fHist,lumiScales,addYear=True)
    else:
      hRec_data_all = getCombinedHist(nameAll,{'Zee':['Electron'],'Zmm':['Muon']},fHist,lumiScales,year='All')

    print 'Data: ', hRec_data_all.Integral()
    
    totBkgr = 0
    if chan != "Combined":
      hRec_others_bkgr = uf_utls.getHist(nameAll,['TT','ST','WW','WZ','ZZ'],fHist,lumiScales,addYear=True)
      #hRec_others_bkgr = uf_utls.getHist(nameAll,['TT'],fHist,lumiScales,addYear=True)
    else:
      hRec_others_bkgr = getCombinedHist(nameAll,{'Zee':['TT','ST','WW','WZ','ZZ'],'Zmm':['TT','ST','WW','WZ','ZZ']},fHist,lumiScales,year='All')
      #hRec_others_bkgr = getCombinedHist(nameAll,{'Zee':['TT'],'Zmm':['TT']},fHist,lumiScales,year='All')
 
    print 'TT and other background: ', hRec_others_bkgr.Integral()
    totBkgr +=  hRec_others_bkgr.Integral()
    
    #Zmm_2bjet_bX_pt_jet"
    nameAll = name1 + '_' + name2 + '_bX_' + name3
    if chan != "Combined":
      hRec_dy_bkgr = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True)
    else:
      hRec_dy_bkgr = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')

    #Zmm_2bjet_XX_pt_jet"
    nameAll = name1 + '_' + name2 + '_XX_' + name3
    if chan != 'Combined':
      hRec_dy_bkgr.Add(uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True))
    else:
      hTmp = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')
      hRec_dy_bkgr.Add(hTmp)

    print 'Other DY background: ', hRec_dy_bkgr.Integral()
    totBkgr +=  hRec_dy_bkgr.Integral()
    
    #Zmm_2bjet_bb_pt_jet"
    nameAll = name1 + '_' + name2 + '_bb_' + name3
    if chan != 'Combined':
      hRec_dy_bb = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True)
    else:
      hRec_dy_bb = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')
       
    print 'Data/MC: ', hRec_data_all.Integral()/(totBkgr+hRec_dy_bb.Integral())

    hRec_data_all.Add(hRec_dy_bkgr,-1.0)
    hRec_data_all.Add(hRec_others_bkgr,-1.0)
    print '(Data-bkgr)/DYbb: ', hRec_data_all.Integral()/hRec_dy_bb.Integral()

    hRec_data_uf = uf_utls.customBin(hRec_data_all,recobins)
    #calculate MET efficiency
    #"pt_b1_rec_all_zmm",
    nameAll = pltName + '_rec_all_'+name1.lower()
    if chan != 'Combined':
      hTmp = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True)
    else:
      hTmp = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')

    hDeno = uf_utls.customBin(hTmp,recobins).Clone(hTmp.GetName()+'_clone')
    #"pt_b1_rec_all_zmm_afterMET"
    nameAll = pltName + '_rec_all_'+name1.lower() + '_afterMET'
    if chan != 'Combined':
      hTmp = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True) #this is bb only
    else:
      hTmp = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')

    hEff = uf_utls.customBin(hTmp,recobins)

    hEff.Divide(hDeno)

    hRec_data_uf.Divide(hEff)
    print 'Data (after MET): ', hRec_data_uf.Integral()
    
    #pt_b1_rec_all_zmm'
    nameAll = pltName + '_rec_all_'+name1.lower()
    if chan != 'Combined':
      hTmp = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True)
    else:
      hTmp = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')
    hRec_DY = uf_utls.customBin(hTmp,recobins)
    
    if chan != 'Combined':
      hRec_bkgr = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True).Clone("bkgr")
    else:
      hRec_bkgr = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All').Clone('bkgr')

 
    #pt_b1_rec_GenMatch_zmm'
    nameAll = pltName + '_rec_GenMatch_'+name1.lower()
    if chan != 'Combined':
      hTmp = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True)
    else:
      hTmp = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')
    hRec_bkgr.Add(hTmp, -1.0)

    #"pt_b1_rec_UFOFGenMatch_zmm"
    nameAll = pltName + '_rec_UFOFGenMatch_'+name1.lower()
    if chan != 'Combined':
      hRec_bkgr.Add(uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True))
    else:
      hRec_bkgr.Add(getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All'))
    hRec_bkgr = uf_utls.customBin(hRec_bkgr,recobins)

    print "Fraction of background: ", hRec_bkgr.Integral()/hRec_DY.Integral()
    hRec_bkgr_frac = hRec_bkgr.Clone("bkgr_frac")
    hRec_bkgr_frac.Divide(hRec_DY)
    #hRec_bkgr_frac.Print("all")

    hRec_bkgr_data = hRec_data_uf.Clone("bkgr_data")
    hRec_bkgr_data.Multiply(hRec_bkgr_frac)
    
    #"pt_b1_res_zmm"
    nameAll = pltName + '_res_'+name1.lower()
    if chan != 'Combined':
      hRes = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True)
    else:
      hRes = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')
    hRes = uf_utls.customBin2D(hRes,genbins,recobins)
    
    #"pt_b1_gen_all_zmm"
    nameAll = pltName + '_gen_all_'+name1.lower()
    if chan != 'Combined':
      hGen = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True)
      #hGen_mg = uf_utls.getHist(nameAll,['DY_MG'],fHist,lumiScales,addYear=True)
      #hGen_sherpa = uf_utls.getHist(nameAll,['DY_Sherpa'],fHist,lumiScales,addYear=False)['16'] #only 16 available for sherpa
      #print nameAll, hGen_sherpa.Integral()
    else:
      hGen = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')
      #hGen_mg = getCombinedHist(nameAll,{'Zee':['DY_MG'],'Zmm':['DY_MG']},fHist,lumiScales,year='All')
      #hGen_sherpa = getCombinedHist(nameAll,{'Zee':['DY_Sherpa'],'Zmm':['DY_Sherpa']},fHist,lumiScales,year='16')
    hGen = uf_utls.customBin(hGen,genbins)
    #hGen_mg = uf_utls.customBin(hGen_mg,genbins)
    #hGen_sherpa  = uf_utls.customBin(hGen_sherpa,genbins)
    
    #"pt_b1_RecMatch_all_zmm"
    nameAll = pltName + '_gen_RecMatch_'+name1.lower()
    if chan != 'Combined':
      hGenMatch = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True)
    else:
      hGenMatch = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')

    
    #"pt_b1_gen_UFOFRecMatch_zmm"
    nameAll = pltName + '_gen_UFOFRecMatch_'+name1.lower()
    if chan != 'Combined':
      hTmp = uf_utls.getHist(nameAll,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales,addYear=True)
    else:
      hTmp = getCombinedHist(nameAll,{'Zee':['DY_0J','DY_1J','DY_2J'],'Zmm':['DY_0J','DY_1J','DY_2J']},fHist,lumiScales,year='All')
    hGenMatch.Add(hTmp,-1.0) #exclude matching with UFOF
    hGenMatch = uf_utls.customBin(hGenMatch,genbins)

    uf = uf_utls.Unfolder1(hRec_data_uf, hRec_bkgr_data, hRes, hGen, hGenMatch,genbins, recobins)
    uf.Unfold()
    
    hUF_final = uf.hUF.Clone(uf.hUF.GetName()+'_final')
    hGen_final = hGen.Clone(hGen.GetName()+'_final')
    #hGen_mg_final = hGen_mg.Clone(hGen_mg.GetName()+'_final')
    #hGen_sherpa_final = hGen_sherpa.Clone(hGen_sherpa.GetName()+'_final')
    if chan == 'Combined':
      hUF_final.Scale(0.5)
      hGen_final.Scale(0.5)
      #hGen_mg_final.Scale(0.5)
      #hGen_sherpa_final.Scale(0.5)
      
    stat_err = 0
    for iB in range(1,hUF_final.GetNbinsX()+1):
      stat_err += hUF_final.GetBinError(iB)*hUF_final.GetBinError(iB)
    stat_err = ROOT.TMath.Sqrt(stat_err)

    corr = hGen_final.Integral(1,1000)/hGen_final.Integral()
    #corr_mg = hGen_mg_final.Integral(1,1000)/hGen_mg_final.Integral()
    print "Total xSection: "
    int_xSec = corr*hUF_final.Integral()/totLumis
    int_xSec_err = corr*stat_err/totLumis
    iB = 1
    if chan == 'Muon': iB = 2
    if chan == 'Combined': iB = 3
    hTot.SetBinContent(iB,int_xSec)
    hTot.SetBinError(iB,int_xSec_err)

    print "Unfolded:       ", hUF_final.Integral()/totLumis, " ", int_xSec, "+/-", int_xSec_err , " (corrected) "
    print "amcatnlo:       ", hGen_final.Integral()/totLumis, " ", hGen_final.Integral(1,1000)/totLumis
    #print "mg:             ", hGen_mg_final.Integral()/totLumis, " ", hGen_mg_final.Integral(1,1000)/totLumis
    #print "sherpa:         ", hGen_sherpa_final.Integral()/lumis['16'], " ", hGen_sherpa_final.Integral(1,1000)/lumis['16']

    hUF_final.Integral()/hGen_final.Integral()
    for i in range(1,hUF_final.GetNbinsX()+1):
      print hUF_final.GetBinLowEdge(i), " ", hUF_final.GetBinContent(i), " ", hGen_final.GetBinContent(i)
    
    utl_func.shapeComparePlot1([hGen_final,hUF_final], ['amc@NLO','Unfolded'], '../Plots_unfolding/Data/'+pltName+'_'+chan, xAxisTitle = xAtitle, xAxisRange = [genbins[0],genbins[len(genbins)-1]])
    
    hGen_final.SetName(pltName+'_gen_amcatnlo_'+chan)
    #hGen_mg_final.SetName(pltName+'_gen_mg_'+chan)
    #hGen_sherpa_final.SetName(pltName+'_gen_sherpa_'+chan)
    hUF_final.SetName(pltName+'_uf_amcatnlo_'+chan)
    #################
    #scale histogram
    #################
    ScaleHist(hGen_final,totLumis)
    #ScaleHist(hGen_mg_final,totLumis)
    #ScaleHist(hGen_sherpa_final,lumis['16'])
    ScaleHist(hUF_final,totLumis)
    print ">>>>>>>>", hGen_final.GetBinContent(1)
    

    fOut.cd()
    hUF_final.Write()
    hGen_final.Write()
    #hGen_mg_final.Write()
    #hGen_sherpa_final.Write()
fOut.cd()
hTot.Write()
