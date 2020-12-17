import ROOT
import sys,os
import copy
import math
sys.path.append('/uscms_data/d3/duong/CMSSW/CMSSW_7_6_5/src/ZplusC/python/')
import ConfigParser
import myutils.util_funcs as utl_func
import myutils as utl

ROOT.gROOT.Macro(os.path.expanduser('~/rootLogOn_forPyROOT.C' ))
ROOT.gROOT.SetBatch(True)


def getVarAttribute(varName, varAtt):
    atts = varAtt.replace(' ','').split('},{')
    return [att.replace('{','').replace('}','').replace(varName + ':','') for att in atts if varName in att][0]

#function to get a dictionary of plots in bins
def getHists_in_bins(bins_dict, parent_folder = 'zHFjet/Ele/', plotName = ['jet_vtxMass','DY_bjet_zHFjet_Ele'], varIn = ['jet_pt','V_pt']): 
  hists = {}
  for varN,bins in bins_dict.items():
    if varN not in varIn: continue
    for ib in range(len(bins)-1):
      binN = varN + '_' +  bins[ib] + '_' + bins[ib+1]
      if varN == 'All': binN = 'All'
      plotN = parent_folder + binN + '/' + plotName[0] + '_' + binN + '_' + plotName[1]
      print '>>>>>>>>>>', plotN
      hists[binN] = fIn.Get(plotN).Clone(plotN + '_clone')
    
  return hists
    

def printEventCountTwoChannels(nums,label_nums,label_translate,k,fLatex,withErr=False):
    #print nums
    fLatex.write('\n')
    fLatex.write('\\begin{table}[h]\n')
    fLatex.write('  \\caption{'+k.replace('_','\_')+'}\n')
    fLatex.write('  \\label{tab:}\n')
    fLatex.write('  \\centering\n')
    fLatex.write('  \\begin{tabular}{lcc}\n')
    fLatex.write('  \\hline\n')
    fLatex.write('  \\hline\n')
    l = ['', 'Electron','Muon']
    st = utl_func.makeLatexLine(l)
    fLatex.write(st)
    fLatex.write('  \\hline\n')
    for label in label_nums:
      if 'MC' == label:
          fLatex.write('\\hline\n')
      #l = [label_translate[label], str(nums['Ele'][label][0]) + ' $\pm$ ' + str(nums['Ele'][label][1]), str(nums['Muon'][label][0]) + ' $\pm$ ' + str(nums['Muon'][label][1])]
      #l = [label_translate[label], str(nums['Ele'][label][0]) + ' $\pm$ ' + str(nums['Ele'][label][1]), str(nums['Muon'][label][0]) + ' $\pm$ ' + str(nums['Muon'][label][1])]
      if label != 'DMCratio':
        if withErr:
          l = [label_translate[label], "{0:.0f} $\pm$ {1:.0f}".format(nums['Ele'][label][0],nums['Ele'][label][1]), "{0:.0f} $\pm$ {1:.0f}".format(nums['Muon'][label][0],nums['Muon'][label][1])]
        else:
          l = [label_translate[label], "{0:.0f}".format(nums['Ele'][label][0]), "{0:.0f}".format(nums['Muon'][label][0])]
      else:
        if withErr:
          l = [label_translate[label], "{0:.3f} $\pm$ {1:.3f}".format(nums['Ele'][label][0],nums['Ele'][label][1]), "{0:.3f} $\pm$ {1:.3f}".format(nums['Muon'][label][0],nums['Muon'][label][1])]
        else:
          l = [label_translate[label], "{0:.2f}".format(nums['Ele'][label][0]), "{0:.2f}".format(nums['Muon'][label][0])]

      fLatex.write(utl_func.makeLatexLine(l))
    fLatex.write('  \\hline\n')
    fLatex.write('  \\hline\n')
    fLatex.write('\\end{tabular}\n')
    fLatex.write('\\end{table}\n')

def printEventCountTwoChannelsAll(nums,label_nums,label_translate,k,fLatex,withErr=False):
    #print nums
    fLatex.write('\n')
    fLatex.write('\\begin{table}[h]\n')
    fLatex.write('  \\caption{'+k.replace('_','\_')+'}\n')
    fLatex.write('  \\label{tab:}\n')
    fLatex.write('  \\centering\n')
    fLatex.write('  \\begin{tabular}{lcccccc}\n')
    fLatex.write('  \\hline\n')
    fLatex.write('  \\hline\n')
    fLatex.write('&\multicolumn{2}{c}{2016} & \multicolumn{2}{c}{2017} & \multicolumn{2}{c}{2018}\\\\\n')
    l = ['', 'Electron','Muon','Electron','Muon','Electron','Muon']
    st = utl_func.makeLatexLine(l)
    fLatex.write(st)
    fLatex.write('  \\hline\n')
    
    for label in label_nums:
      
      if 'MC' == label:
          fLatex.write('\\hline\n')

      if label != 'DMCratio':
        if withErr:
          l = [label_translate[label],\
              "{0:.0f} $\pm$ {1:.0f}".format(nums['Zee']['16'][label][0],nums['Zee']['16'][label][1]), "{0:.0f} $\pm$ {1:.0f}".format(nums['Zmm']['16'][label][0],nums['Zmm']['16'][label][1]),\
              "{0:.0f} $\pm$ {1:.0f}".format(nums['Zee']['17'][label][0],nums['Zee']['17'][label][1]), "{0:.0f} $\pm$ {1:.0f}".format(nums['Zmm']['17'][label][0],nums['Zmm']['17'][label][1]),\
              "{0:.0f} $\pm$ {1:.0f}".format(nums['Zee']['18'][label][0],nums['Zee']['18'][label][1]), "{0:.0f} $\pm$ {1:.0f}".format(nums['Zmm']['18'][label][0],nums['Zmm']['18'][label][1])]
        else:
          l = [label_translate[label],\
              "{0:.0f}".format(nums['Zee']['16'][label][0]), "{0:.0f}".format(nums['Zmm']['16'][label][0]),\
              "{0:.0f}".format(nums['Zee']['17'][label][0]), "{0:.0f}".format(nums['Zmm']['17'][label][0]),\
              "{0:.0f}".format(nums['Zee']['18'][label][0]), "{0:.0f}".format(nums['Zmm']['18'][label][0])]
      else:
        if withErr:
          l = [label_translate[label],\
              "{0:.3f} $\pm$ {1:.3f}".format(nums['Zee']['16'][label][0],nums['Zee']['16'][label][1]), "{0:.3f} $\pm$ {1:.3f}".format(nums['Zmm']['16'][label][0],nums['Zmm']['16'][label][1]),\
              "{0:.3f} $\pm$ {1:.3f}".format(nums['Zee']['17'][label][0],nums['Zee']['17'][label][1]), "{0:.3f} $\pm$ {1:.3f}".format(nums['Zmm']['17'][label][0],nums['Zmm']['17'][label][1]),\
              "{0:.3f} $\pm$ {1:.3f}".format(nums['Zee']['18'][label][0],nums['Zee']['18'][label][1]), "{0:.3f} $\pm$ {1:.3f}".format(nums['Zmm']['18'][label][0],nums['Zmm']['18'][label][1])]
        else:
          l = [label_translate[label],\
              "{0:.2f}".format(nums['Zee']['16'][label][0]), "{0:.2f}".format(nums['Zmm']['16'][label][0]),\
              "{0:.2f}".format(nums['Zee']['17'][label][0]), "{0:.2f}".format(nums['Zmm']['17'][label][0]),\
              "{0:.2f}".format(nums['Zee']['18'][label][0]), "{0:.2f}".format(nums['Zmm']['18'][label][0])]

      fLatex.write(utl_func.makeLatexLine(l))
    fLatex.write('  \\hline\n')
    fLatex.write('  \\hline\n')
    fLatex.write('\\end{tabular}\n')
    fLatex.write('\\end{table}\n')

def printEventCountOneChannels(nums,label_nums,label_translate,k,chan,fLatex):
    #print nums
    fLatex.write('\n')
    fLatex.write('\\begin{table}[h]\n')
    fLatex.write('  \\caption{'+k.replace('_','\_')+'}\n')
    fLatex.write('  \\label{tab:}\n')
    fLatex.write('  \\centering\n')
    fLatex.write('  \\begin{tabular}{lc}\n')
    fLatex.write('  \\hline\n')
    fLatex.write('  \\hline\n')
    l = ['', 'Number of events']
    st = utl_func.makeLatexLine(l)
    fLatex.write(st)
    fLatex.write('  \\hline\n')
    for label in label_nums:
      if 'MC' == label:
          fLatex.write('\\hline\n')
      l = [label_translate[label], str(nums[chan][label][0]) + ' $\pm$ ' + str(nums[chan][label][1])]
      fLatex.write(utl_func.makeLatexLine(l))
    fLatex.write('  \\hline\n')
    fLatex.write('  \\hline\n')
    fLatex.write('\\end{tabular}\n')
    fLatex.write('\\end{table}\n')


def getHist(pN,samName,fH,lS): #samName = ['Electron'],['DY_0J','DY_1J','DY_2J']...
  hOut = {}
  for y in years:
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
  return hOut

def getHistIntegral(h):
  tmp = [0,0]
  for iB in range(h.GetNbinsX()+2): #include under and overflow bins
    tmp[0] += h.GetBinContent(iB)
    tmp[1] += h.GetBinError(iB)*h.GetBinError(iB)
  tmp[1] = math.sqrt(tmp[1])
  return tmp

##########################
#Main
##########################

years = ['16','17','18']
#regions = ['Z_jet','Z_bjet','Z_2bjet', 'Z_2bjetDeepJet']
regions = ['Z_jet','Z_bjet','Z_2bjet']
dy_type = 'amcnlo' #mg amcnlo

summary_control_plot_name = 'summary_control_plot_'+dy_type+'.txt'
summary_eventCount_name = 'summary_eventCount_'+dy_type+'.txt'
summary_eventCount_name_allYears = 'summary_eventCount_allYears_'+dy_type+'.txt'

cfg = utl.BetterConfigParser()
cfg.read('../Configs/config.ini')
print(cfg)

#plotFolder = '../Test/V1_nanoV7_unfolding_allWeight_tightMuonIso_1/'
#plotFolder = '../Test/V1_nanoV7_unfolding_allWeight_tightMuonIso_tightJetID_newFillUnfolding_newDphiBBbinning/'
plotFolder = '../Test/Check/'

for y in years:
  os.system('mkdir -p ' + plotFolder + '/20' + y)

os.system('mkdir -p ' + plotFolder + '/All')

lumiS = {}
for y in years:
  lumiTmp = float(cfg.get('General','lumi_'+y))/1000.
  lumiTmp = format("%.1f" % lumiTmp)
  lumiS[y] = str(lumiTmp)
print lumiS 

#sys.exit()

#############
#get scale to lumi
#FIXME ---> change to eos files
#FIXME ---> save histogram of event counts to the histogram file so the we do not need to find this number at the source file 

fLatex_all = open(plotFolder+'/'+summary_control_plot_name,'w')
fLatex_all.write('\documentclass[12pt]{article}\n')
fLatex_all.write('\usepackage{graphicx}\n')
fLatex_all.write('\\title{Control plots}\n')
fLatex_all.write('\\begin{document}\n')

fNames = {}
xSecs = {}
lumiScales = {}

fHist = {}

print cfg.get('General','lumi_16')

#for s in ['Electron','Muon','DY_0J','DY_1J','DY_2J','TT','ST','WW','WZ','ZZ']:
ss = ['Electron','Muon','DY_0J','DY_1J','DY_2J','TT','ST','WW','WZ','ZZ']
if dy_type == 'mg':
  ss = ['Electron','Muon','DY_MG','TT','ST','WW','WZ','ZZ']

for s in ss:

  fNames[s] = {}
  xSecs[s] = {}
  lumiScales[s] = {}
  fHist[s] = {}
  
  for y in years:
    #for y in ['17']:
    
    lumi = float(cfg.get('General','lumi_'+y))
    
    names = cfg.get(s,'file_'+y).split(',') #multiple names is possible, for example: Single top = t-channels, s-channels ...
    xSecTmps = ['1']*len(names) #each name corresponds to a cross section
    kfactor = ['1']*len(names) #each name corresponds to a cross section
    if s not in ['Electron','Muon']:
      xSecTmps = cfg.get(s,'xSec_'+y).split(',')
      xSecTmps = cfg.get(s,'xSec_'+y).split(',')
    
    fNames[s][y] = []
    xSecs[s][y] = []
    fHist[s][y] = []
    for iN in names:
      fNames[s][y].append(cfg.get('Paths','path') + '/' + iN)
      fHist[s][y].append(ROOT.TFile.Open(fNames[s][y][-1],'READ'))
    
    print xSecTmps
    for iS in xSecTmps:
      #in case there is kfactor in cross section
      if '*' in iS:
        iS = iS.split('*')
      if len(iS) == 2:
        xSecs[s][y].append(float(iS[0])*float(iS[1]))
      else:
        xSecs[s][y].append(float(iS))

    lumiScales[s][y] = [1]*len(names)
    for iN in range(len(fNames[s][y])):
      if s not in ['Electron','Muon']:
        print s, y, iN, fNames[s][y][iN]
        lumiScales[s][y][iN] = utl_func.scaleToLumi1(fNames[s][y][iN],xSecs[s][y][iN],lumi)

    

#print fNames['ST']['16']
#print xSecs['ST']['16']
#print xSecs['Electron']['16']
#print lumiScales['ST']['16']

#print fNames['TT']["16"]
#print xSecs['TT']["16"]
#print lumiScales['TT']["16"]

#print fNames['TT']["17"]
#print xSecs['TT']["17"]
#print lumiScales['TT']["17"]
#print fHist['TT']['17'][0].GetName()
#print fHist['TT']['17'][1].GetName()

#sys.exit()

getHist('Zee_jet_pt_lep0',['TT'],fHist,lumiScales)

nums = {}

for r in regions:
  
  nums[r] = {}

  fLatex_all.write('\section{'+r+'}\n')

  fLatex = open(plotFolder + '/event_counts_' + r + '_' + dy_type +'.txt','w')  

  plotNames = cfg.get('Plots',r + '_plot').split(',')
  
  for chan in ['Zee','Zmm']:
  #for chan in ['Zmm']:
    
    nums[r][chan] = {}

    for plN in plotNames:
      hN = chan + '_' + r.split('_')[1] + '_' + plN
      print hN, plN
      
      datName = 'Electron'
      if chan == 'Zmm':
        datName = 'Muon' 
      
      hDat = getHist(hN,[datName],fHist,lumiScales)
      
      if dy_type != 'mg':
        hDY = getHist(hN,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales)
      if dy_type == 'mg':
        hDY = getHist(hN,['DY_MG'],fHist,lumiScales)
      
      if r.split('_')[1] == '2bjet':
        hN1 = chan + '_' + r.split('_')[1] + '_bb_' + plN
        hN2 = chan + '_' + r.split('_')[1] + '_bX_' + plN
        hN3 = chan + '_' + r.split('_')[1] + '_XX_' + plN
        print hN1, hN2, hN3
        if dy_type != 'mg':
          hDY1 = getHist(hN1,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales)
          hDY2 = getHist(hN2,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales)
          hDY3 = getHist(hN3,['DY_0J','DY_1J','DY_2J'],fHist,lumiScales)
        else:
          hDY1 = getHist(hN1,['DY_MG'],fHist,lumiScales)
          hDY2 = getHist(hN2,['DY_MG'],fHist,lumiScales)
          hDY3 = getHist(hN3,['DY_MG'],fHist,lumiScales)


      hTT = getHist(hN,['TT'],fHist,lumiScales)
      hST = getHist(hN,['ST'],fHist,lumiScales)
      hVV = getHist(hN,['WW','WZ','ZZ'],fHist,lumiScales)
      for y in years:
        #for y in ['17']:
        if plN == 'pt_lep0':
          
          nums[r][chan][y] = {}
          
          fLatex.write('\n' + y + ' ' + chan)
          
          tmp = getHistIntegral(hDat[y])
          nums[r][chan][y]['Data'] = tmp
          fLatex.write('\n Data: '+str(tmp[0]))

          if r.split('_')[1] != '2bjet':
            tmp = getHistIntegral(hDY[y])
            nums[r][chan][y]['DY'] = tmp
            fLatex.write('\n DY: '+str(tmp[0]))
          else:
            tmp = getHistIntegral(hDY1[y])
            nums[r][chan][y]['DYbb'] = tmp
            fLatex.write('\n DY (bb): '+str(tmp[0]))
            tmp = getHistIntegral(hDY2[y])
            nums[r][chan][y]['DYbX'] = tmp
            fLatex.write('\n DY (bX): '+str(tmp[0]))
            tmp = getHistIntegral(hDY3[y])
            nums[r][chan][y]['DYXX'] = tmp
            fLatex.write('\n DY (XX): '+str(tmp[0]))
          tmp = getHistIntegral(hTT[y])
          nums[r][chan][y]['TT'] = tmp
          fLatex.write('\n tt: '+str(tmp[0]))
          tmp = getHistIntegral(hST[y])
          nums[r][chan][y]['ST'] = tmp
          fLatex.write('\n ST: '+str(tmp[0]))
          tmp = getHistIntegral(hVV[y])
          nums[r][chan][y]['VV'] = tmp
          fLatex.write('\n VV: '+str(tmp[0]))

        tmps = cfg.get(plN,'xAxisRange').split(',')
        xA_range = []
        if 'Pi' not in tmps[1]:
          xA_range = [float(tmps[0]),float(tmps[1])]
        else:
          #xA_range = [-ROOT.TMath.Pi(),ROOT.TMath.Pi()]
          xA_range = [0,ROOT.TMath.Pi()]
        xA_title = cfg.get(plN,'xAxisTitle')
        nRebin = int(cfg.get(plN,'rebin'))
        
        if r.split('_')[1] != '2bjet':
          plots_process = [hDat[y].Clone().Rebin(nRebin),hST[y].Clone().Rebin(nRebin),hVV[y].Clone().Rebin(nRebin),hTT[y].Clone().Rebin(nRebin),hDY[y].Clone().Rebin(nRebin)]
        else:
          plots_process = [hDat[y].Clone().Rebin(nRebin),hST[y].Clone().Rebin(nRebin),hVV[y].Clone().Rebin(nRebin),hTT[y].Clone().Rebin(nRebin),hDY3[y].Clone().Rebin(nRebin),hDY2[y].Clone().Rebin(nRebin),hDY1[y].Clone().Rebin(nRebin)]
        plotNames_process = []
        dataTitle = 'Data ('
        if r == 'Z_jet' and chan == 'Zee': dataTitle = dataTitle + 'Z(ee) + jets)'
        if r == 'Z_jet' and chan == 'Zmm': dataTitle = dataTitle + 'Z(#mu#mu) + jets)'
        if r == 'Z_bjet' and chan == 'Zee': dataTitle = dataTitle + 'Z(ee) + b jets)'
        if r == 'Z_bjet' and chan == 'Zmm': dataTitle = dataTitle + 'Z(#mu#mu) + b jets)'
        if r == 'Z_2bjet' and chan == 'Zee': dataTitle = dataTitle + 'Z(ee) + 2 b jets)'
        if r == 'Z_2bjet' and chan == 'Zmm': dataTitle = dataTitle + 'Z(#mu#mu) + 2 b jets)'
        plotNames_process = [dataTitle, 'Single top', 'Diboson', 't#bar{t}', 'Z+jets']
        if r.split('_')[1] == '2bjet':
          plotNames_process = [dataTitle, 'Single top', 'Diboson', 't#bar{t}', 'Z+XX', 'Z+bX', 'Z+bb']

        utl_func.makeStackPlot(plots_process, plotNames_process, plN + '_' + r + '_'+chan+'_'+y+'_'+dy_type, plotFolder + '/20'+y, xA_title, xA_range, 'MC unc. (stat.)', False, lumi=lumiS[y])

      ############################
      #Plot control plot for all years
      ############################
      print ">>>>>>>>>>>>>>"
      print hDat
      
      hDatA = hDat['16'].Clone(hDat['16'].GetName()+'_all')
      hDYA = hDY['16'].Clone(hDY['16'].GetName()+'_all')
      if r.split('_')[1] == '2bjet':
        hDY1A = hDY1['16'].Clone(hDY1['16'].GetName()+'_all')
        hDY1A = hDY1['16'].Clone(hDY1['16'].GetName()+'_all')
        hDY1A = hDY1['16'].Clone(hDY1['16'].GetName()+'_all')
        hDY2A = hDY2['16'].Clone(hDY2['16'].GetName()+'_all')
        hDY2A = hDY2['16'].Clone(hDY2['16'].GetName()+'_all')
        hDY2A = hDY2['16'].Clone(hDY2['16'].GetName()+'_all')
        hDY3A = hDY3['16'].Clone(hDY3['16'].GetName()+'_all')
        hDY3A = hDY3['16'].Clone(hDY3['16'].GetName()+'_all')
        hDY3A = hDY3['16'].Clone(hDY3['16'].GetName()+'_all')
      hTTA = hTT['16'].Clone(hTT['16'].GetName()+'_all')
      hSTA = hST['16'].Clone(hST['16'].GetName()+'_all')
      hVVA = hVV['16'].Clone(hVV['16'].GetName()+'_all')
      
      for y in ['17','18']:
        hDatA.Add(hDat[y])
        hDYA.Add(hDY[y])
        if r.split('_')[1] == '2bjet':
          hDY1A.Add(hDY1[y])
          hDY2A.Add(hDY2[y])
          hDY3A.Add(hDY3[y])
        hTTA.Add(hTT[y])
        hSTA.Add(hST[y])
        hVVA.Add(hVV[y])

      if r.split('_')[1] != '2bjet':
        plots_process = [hDatA.Clone().Rebin(nRebin),hSTA.Clone().Rebin(nRebin),hVVA.Clone().Rebin(nRebin),hTTA.Clone().Rebin(nRebin),hDYA.Clone().Rebin(nRebin)]
      else:
        plots_process = [hDatA.Clone().Rebin(nRebin),hSTA.Clone().Rebin(nRebin),hVVA.Clone().Rebin(nRebin),hTTA.Clone().Rebin(nRebin),hDY3A.Clone().Rebin(nRebin),hDY2A.Clone().Rebin(nRebin),hDY1A.Clone().Rebin(nRebin)]
      
      utl_func.makeStackPlot(plots_process, plotNames_process, plN + '_' + r + '_'+chan+'_All_'+dy_type, plotFolder + '/All', xA_title, xA_range, 'MC unc. (stat.)', False, lumi='137.1')

print nums
#adding all years
for r in regions:
  for chan in ['Zee','Zmm']:
    print nums[r][chan]['18'].keys()
    pp = ['TT', 'ST', 'Data', 'VV', 'DY']
    if r == "Z_2bjet":
      pp = ['DYXX', 'TT', 'DYbX', 'ST', 'DYbb', 'VV', 'Data']
    nums[r][chan]['All'] = {}
    for p in pp:
      nums[r][chan]['All'][p] = [0,0]
      for y in ['16','17','18']:
        nums[r][chan]['All'][p][0] += nums[r][chan][y][p][0]
        nums[r][chan]['All'][p][1] += nums[r][chan][y][p][1]*nums[r][chan][y][p][1]
      nums[r][chan]['All'][p][1] = math.sqrt(nums[r][chan]['All'][p][1])


#calculate total MC and data/MC ratios
for r in regions:
  mc_name = ['DY','TT','ST','VV']
  if r == "Z_2bjet":
    mc_name = ['DYbb','DYbX','DYXX','TT','ST','VV']
  yy = years + ['All']
  for y in yy:
    #for chan in ['Zee','Zmm']:
    for chan in ['Zee','Zmm']:
      nums[r][chan][y]['MC'] = [0,0] 
      for n in mc_name:
        nums[r][chan][y]['MC'][0] += nums[r][chan][y][n][0] 
        nums[r][chan][y]['MC'][1] += nums[r][chan][y][n][1] 
      nums[r][chan][y]['MC'][1] = math.sqrt(nums[r][chan][y]['MC'][1])
      nums[r][chan][y]['DMCratio'] = [0,0]
      nums[r][chan][y]['DMCratio'][0] = nums[r][chan][y]['Data'][0]/nums[r][chan][y]['MC'][0]
      nums[r][chan][y]['DMCratio'][1] = utl_func.getRatioErrors(nums[r][chan][y]['Data'],nums[r][chan][y]['MC'])


print nums['Z_jet']['Zee']['16']['Data'],nums['Z_jet']['Zee']['16']['MC'],nums[r][chan][y]['DMCratio'][1]

fLatex_evtCount = open(plotFolder+'/'+summary_eventCount_name,'w')
fLatex_evtCount.write('\documentclass[12pt]{article}\n')
fLatex_evtCount.write('\usepackage{graphicx}\n')
fLatex_evtCount.write('\\title{Control plots}\n')
fLatex_evtCount.write('\\begin{document}\n')

for y in years:
  for r in regions:
    label_nums = ['DY','TT','ST','VV','MC','Data','DMCratio']
    label_translate = {'DY':'DY','TT':'tt','ST':'Single t','VV':'VV','MC':'MC','Data':'Data','DMCratio':'Data/MC'}
    if r == 'Z_2bjet':
      label_nums = ['DYbb','DYbX','DYXX','TT','ST','VV','MC','Data','DMCratio']
      label_translate = {'DYbb':'DY+bb','DYbX':'DY+bX','DYXX':'DY+XX','TT':'tt','ST':'single t','VV':'VV','MC':'MC','Data':'Data','DMCratio':'Data/MC'}
    k = y + '_' + r
    numTmp = {}
    numTmp['Ele'] = nums[r]['Zee'][y]
    numTmp['Muon'] = nums[r]['Zmm'][y]
    #print numTmp
    printEventCountTwoChannels(numTmp,label_nums,label_translate,k,fLatex_evtCount)

fLatex_evtCount.write('\\end{document}\n')

i = 0
for r in regions:
  if i == 0:
    fLatex_evtCount = open(plotFolder+'/'+summary_eventCount_name_allYears,'w')
    fLatex_evtCount.write('\documentclass[12pt]{article}\n')
    fLatex_evtCount.write('\usepackage{graphicx}\n')
    fLatex_evtCount.write('\\title{Control plots}\n')
    fLatex_evtCount.write('\\begin{document}\n')
  else:
    fLatex_evtCount = open(plotFolder+'/'+summary_eventCount_name_allYears,'a')
  
  label_nums = ['DY','TT','ST','VV','MC','Data','DMCratio']
  label_translate = {'DY':'DY','TT':'tt','ST':'Single t','VV':'VV','MC':'MC','Data':'Data','DMCratio':'Data/MC'}
  if r == 'Z_2bjet':
    label_nums = ['DYbb','DYbX','DYXX','TT','ST','VV','MC','Data','DMCratio']
    label_translate = {'DYbb':'DY+bb','DYbX':'DY+bX','DYXX':'DY+XX','TT':'tt','ST':'single t','VV':'VV','MC':'MC','Data':'Data','DMCratio':'Data/MC'}
  k = y + '_' + r

  printEventCountTwoChannelsAll(nums[r],label_nums,label_translate,k,fLatex_evtCount)

  if i == (len(regions)-1):
    fLatex_evtCount.write('\\end{document}\n')
  
  i+=1
