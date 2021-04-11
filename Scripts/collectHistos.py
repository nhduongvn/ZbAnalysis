import os,sys
import ROOT

#reduce the number of bins in z+1b by 1 (Meena uses one extra bin for overflow)
def reduceBin(hIn,hIn_z2b):
  hOut = hIn_z2b.Clone(hIn.GetName()+'_newBinning') #Clone z2b his to have the same binning as z2b
  hOut.Reset() #NEVER USE Clear()
  for i in range(1,hIn_z2b.GetNbinsX()+1):
    hOut.SetBinContent(i,hIn.GetBinContent(i))
    hOut.SetBinError(i,hIn.GetBinError(i))
  return hOut


pltNames = ['pt_b1']

chans = ['Combined']

systs = ['JES','JER','PU','METJES','METJER','METUNCLUST','TTZ2B','DYXSEC','WWXSEC','WZXSEC','ZZXSEC','TTZ1B','CLO','PUMVA', 'PDF', 'SCALE']
#systs = ['PU','L1PREFIRING','BTAG','ELE','MUON','JES','JER','METJES','METJER','METUNCLUST','TT','DYXSEC','WWXSEC','WZXSEC','ZZXSEC']

fOut = ROOT.TFile.Open('ratio_unc.root','RECREATE')

for chan in chans:
  for pltName in pltNames:
    #################
    #get central distributions
    #################
    
    #z+2b
    fname = "../Output_unfolding/unfolding_central_withNormDistributions.root"
    fname_theo = "../Output_theory_pred/theory_pred.root"
 
    #z+1b
    fFolder_z1b = '../Z1b_forRatio_syst/' #folder of z1b root file
    #unfoldedSpectra_Muon_v2_0M_DatawithMCNLO2016_JetPt_for_ratio_TT_UP.root
    #unfoldedSpectra_Electron_v2_0M_DatawithMCNLO2016_JetPt_for_ratio_TT_UP.root
    #unfoldedSpectra_Leptons_v3_0M_DatawithMCNLO2016_JetPt_for_ratio_TT_UP.root
    fname_z1b_chan = chan
    if chan == 'Combined': fname_z1b_chan = 'Leptons'

    fname_z1b_pltName = ''
    if pltName == 'pt_b1': fname_z1b_pltName = 'JetPt'
    
    if pltName == 'pt_Z':  fname_z1b_pltName = 'ZPt'
      
    fname_z1b_other = ''
    if chan == 'Electron' or chan == 'Muon': fname_z1b_other = 'v2'
    if chan == 'Combined': fname_z1b_other = 'v3'
    
    fFullName_z1b = fFolder_z1b+'/unfoldedSpectra_' + fname_z1b_chan + '_' + fname_z1b_other + '_0M_DatawithMCNLO2016_' + fname_z1b_pltName + '_for_ratio.root'#central
    #unfoldedSpectra_Muon_v2_0M_onlyGenPred_MCNLO_012jets2018_MCNLO_012jets_JetPt_for_ratio.root
    #fFullName_z1b_gen_amcnlo = fFolder_z1b+'/unfoldedSpectra_' + fname_z1b_chan + '_' + fname_z1b_other + '_0M_onlyGenPred_MCNLO_012jets2018_MCNLO_012jets_' + fname_z1b_pltName + '_for_ratio.root'#central
    fFullName_z1b_gen_amcnlo = fFolder_z1b+'/unfoldedSpectra_' + fname_z1b_chan + '_' + fname_z1b_other + '_0M_onlyGenPred_MCNLO_012jets2018_MCNLO_012jets_' + fname_z1b_pltName + '_for_ratio.root'#central
    #unfoldedSpectra_Muon_v2_0M_onlyGenPred_MG2018_MG_eta_for_ratio.root
    fFullName_z1b_gen_mg = fFolder_z1b+'/unfoldedSpectra_' + fname_z1b_chan + '_' + fname_z1b_other + '_0M_onlyGenPred_MG2018_MG_' + fname_z1b_pltName + '_for_ratio.root'#central
    #unfoldedSpectra_Electron_v2_0M_onlyGenPred_sherpa2016_sherpa_JetPt_for_ratio.root 
    fFullName_z1b_gen_sherpa = fFolder_z1b+'/unfoldedSpectra_' + fname_z1b_chan + '_' + fname_z1b_other + '_0M_onlyGenPred_sherpa2016_sherpa_' + fname_z1b_pltName + '_for_ratio.root'#central
    #Muon_xsec_systematic_2016_JetPt_for_ratio.root
    fFullName_z1b_gen_amcnlo_pdfScale = fFolder_z1b+'/PDF_Scale_for_Gen/'+fname_z1b_chan+'_xsec_systematic_2016_'+fname_z1b_pltName+'_for_ratio.root'
    

    fC_z2b = ROOT.TFile.Open(fname, 'READ')
    fC_z2b_theo_unc = ROOT.TFile.Open(fname_theo,'READ')

    fC_z1b = ROOT.TFile.Open(fFullName_z1b, 'READ')
    fC_z1b_gen_amcnlo = ROOT.TFile.Open(fFullName_z1b_gen_amcnlo, 'READ')
    fC_z1b_gen_mg = ROOT.TFile.Open(fFullName_z1b_gen_mg, 'READ')
    fC_z1b_gen_sherpa = ROOT.TFile.Open(fFullName_z1b_gen_sherpa, 'READ')
    fC_z1b_gen_amcatnlo_pdfScale = ROOT.TFile.Open(fFullName_z1b_gen_amcnlo_pdfScale,'READ')

    #central
    h_z2b_nom = fC_z2b.Get(pltName + '_uf_amcatnlo_NONE_'+chan)
    h_rat_nom = h_z2b_nom.Clone(pltName + '_NOM_rat_'+chan)
    h_z1b_nom = fC_z1b.Get('data_bw_xsec')
    h_z1b_nom = reduceBin(h_z1b_nom,h_z2b_nom)
    h_rat_nom.Divide(h_z1b_nom)
    
    ######################
    #gen
    ######################
    #pt_b1_gen_mg_NONE_Electron
    h_z2b_mg_nom = fC_z2b.Get(pltName+"_gen_mg_NONE_"+chan) 
    h_z2b_amcatnlo_nom = fC_z2b.Get(pltName+"_gen_amcatnlo_NONE_"+chan) 
    h_z2b_sherpa_nom = fC_z2b.Get(pltName+"_gen_sherpa_NONE_"+chan)
    #note these are relative errors
    #pt_b1_gen_16_amcnlo_Electron_PDFunc
    h_z2b_amcatnlo_pdf = fC_z2b_theo_unc.Get(pltName+"_gen_16_amcnlo_"+chan+'_PDFunc') 
    h_z2b_amcatnlo_scale = fC_z2b_theo_unc.Get(pltName+"_gen_16_amcnlo_"+chan+'_scaleUnc') 
    #h_z2b_amcatnlo_pdf.Multiply(h_z2b_amcatnlo_nom) #this is absolute error 
    #h_z2b_amcatnlo_pdf.Add(h_z2b_amcatnlo_nom) #this is nom+error
    #h_z2b_amcatnlo_scale.Multiply(h_z2b_amcatnlo_nom) #this is absolute error 
    #h_z2b_amcatnlo_scale.Add(h_z2b_amcatnlo_nom) #this is nom+error
    
    h_z1b_amcatnlo_nom = fC_z1b_gen_amcnlo.Get("genL_bw_xsec")
    h_z1b_amcatnlo_nom = reduceBin(h_z1b_amcatnlo_nom,h_z2b_amcatnlo_nom)
    h_z1b_mg_nom = fC_z1b_gen_mg.Get("genL_bw_xsec")
    h_z1b_mg_nom = reduceBin(h_z1b_mg_nom,h_z2b_mg_nom)
    h_z1b_sherpa_nom = fC_z1b_gen_sherpa.Get("genL_bw_xsec")
    h_z1b_sherpa_nom = reduceBin(h_z1b_sherpa_nom,h_z2b_sherpa_nom)
    #hist_UP_pdf_JetPt_for_ratio, this is relative unc. note UP and DOWN are the same
    h_z1b_amcatnlo_pdf = fC_z1b_gen_amcatnlo_pdfScale.Get("hist_UP_pdf_"+fname_z1b_pltName+'_for_ratio') 
    h_z1b_amcatnlo_pdf = reduceBin(h_z1b_amcatnlo_pdf,h_z2b_amcatnlo_pdf)
    #hist_0_JetPt_for_ratio
    hTmp = fC_z1b_gen_amcatnlo_pdfScale.Get('hist_0_'+fname_z1b_pltName+'_for_ratio')
    #hist_UP_JetPt_for_ratio, scale unc.
    h_z1b_amcatnlo_scaleU = fC_z1b_gen_amcatnlo_pdfScale.Get("hist_UP_"+fname_z1b_pltName+'_for_ratio')
    h_z1b_amcatnlo_scaleU.Add(hTmp,-1)
    h_z1b_amcatnlo_scaleU.Divide(hTmp)
    h_z1b_amcatnlo_scaleU = reduceBin(h_z1b_amcatnlo_scaleU,h_z2b_amcatnlo_scale)
    h_z1b_amcatnlo_scaleD = fC_z1b_gen_amcatnlo_pdfScale.Get("hist_Down_"+fname_z1b_pltName+'_for_ratio')
    h_z1b_amcatnlo_scaleD.Add(hTmp,-1)
    h_z1b_amcatnlo_scaleD.Divide(hTmp)
    h_z1b_amcatnlo_scaleD = reduceBin(h_z1b_amcatnlo_scaleD,h_z2b_amcatnlo_scale)

    h_rat_amcatnlo_nom = h_z2b_amcatnlo_nom.Clone(pltName+'_NOM_gen_amcatnlo_rat_'+chan)
    h_rat_amcatnlo_nom.Divide(h_z1b_amcatnlo_nom)
    h_rat_mg_nom = h_z2b_mg_nom.Clone(pltName+'_NOM_gen_mg_rat_'+chan)
    h_rat_mg_nom.Divide(h_z1b_mg_nom)
    h_rat_sherpa_nom = h_z2b_sherpa_nom.Clone(pltName+'_NOM_gen_sherpa_rat_'+chan)
    h_rat_sherpa_nom.Divide(h_z1b_sherpa_nom)

    fOut.cd()
    h_z2b_nom.SetName(pltName + '_NOM_z2b_'+chan)
    h_z2b_nom.Write()
    h_z1b_nom.SetName(pltName + '_NOM_z1b_'+chan)
    h_z1b_nom.Write()
    h_rat_nom.Write()
    h_z2b_amcatnlo_nom.SetName(pltName + '_NOM_z2b_gen_amcatnlo_'+chan)
    h_z2b_amcatnlo_nom.Write()
    h_z2b_mg_nom.SetName(pltName + '_NOM_z2b_gen_mg_'+chan)
    h_z2b_mg_nom.Write()
    h_z2b_sherpa_nom.SetName(pltName + '_NOM_z2b_gen_sherpa_'+chan)
    h_z2b_sherpa_nom.Write()
    h_z2b_amcatnlo_pdf.SetName(pltName+'_pdf_z2b_gen_amcatnlo_'+chan)
    h_z2b_amcatnlo_pdf.Write()
    h_z2b_amcatnlo_scale.SetName(pltName+'_scale_z2b_gen_amcatnlo_'+chan)
    h_z2b_amcatnlo_scale.Write()
    h_z1b_amcatnlo_nom.SetName(pltName + '_NOM_z1b_gen_amcnlo_'+chan)
    h_z1b_amcatnlo_nom.Write()
    h_z1b_mg_nom.SetName(pltName + '_NOM_z1b_gen_mg_'+chan)
    h_z1b_mg_nom.Write()
    h_z1b_sherpa_nom.SetName(pltName + '_NOM_z1b_gen_sherpa_'+chan)
    h_z1b_sherpa_nom.Write()
    h_z1b_amcatnlo_pdf.SetName(pltName+'_pdf_z1b_gen_amcatnlo_'+chan)
    h_z1b_amcatnlo_pdf.Write()
    h_z1b_amcatnlo_scaleU.SetName(pltName+'_scaleU_z1b_gen_amcatnlo_'+chan)
    h_z1b_amcatnlo_scaleU.Write()
    h_z1b_amcatnlo_scaleD.SetName(pltName+'_scaleD_z1b_gen_amcatnlo_'+chan)
    h_z1b_amcatnlo_scaleD.Write()

    h_rat_amcatnlo_nom.Write()
    h_rat_mg_nom.Write()
    h_rat_sherpa_nom.Write()

    for syst in systs:
      z1b_systs = True #flag syst available for Z+1b
      z2b_systs = True #flag syst avaliable for Z+2b
      if syst in ['TTZ1B','CLO','PUMVA']: z2b_systs = False
      if syst in ['TTZ2B','DYXSEC','WWXSEC','WZXSEC','ZZXSEC']: z1b_systs = False

      fname_syst = "../Output_unfolding/syst_all_uncertainty_max.root"
      fname_syst_theo = "../Output_unfolding/syst_uncertainty_uf_pdfScale.root"
      fname_syst_theo_z1b = "../Z1b_forRatio_syst/PDF_Scale_for_Reco/"+fname_z1b_chan+"_xsec_systematic_2016_"+fname_z1b_pltName+"_for_ratio.root"

      #get input name for syst z1b
      fname_z1b_syst = syst
      if syst == 'JES': fname_z1b_syst = 'JEC'
      if syst == 'METJES': fname_z1b_syst = 'MET_JetEn'
      if syst == 'METJER': fname_z1b_syst = 'MET_JetRes'
      if syst == 'METUNCLUST': fname_z1b_syst = 'MET_UnclusteredEn'
      if syst == 'TTZ1B': fname_z1b_syst = 'TT'

      fFullName_z1b_syst = [] #to store UP and DOWN
      for v in ['UP','DOWN']:
        fFullName_z1b_syst.append(fFolder_z1b+'/unfoldedSpectra_' + fname_z1b_chan + '_' + fname_z1b_other + '_0M_DatawithMCNLO2016_' + fname_z1b_pltName + '_for_ratio_' + fname_z1b_syst + '_'+v+'.root')
      if z2b_systs:
        fSyst_z2b = ROOT.TFile.Open(fname_syst,'READ')#Z+2b uses all up and down are stored in the same root file
      if z1b_systs and syst not in ['PDF','SCALE']:
        fSyst_z1bU = ROOT.TFile.Open(fFullName_z1b_syst[0],'READ')#Z+1b uses separate files to store up and down
        fSyst_z1bD = ROOT.TFile.Open(fFullName_z1b_syst[1],'READ')#Z+1b uses separate files to store up and down
      
      #theo unc.
      fTheo_z2b = ROOT.TFile.Open(fname_syst_theo,'READ')
      fTheo_z1b = ROOT.TFile.Open(fname_syst_theo_z1b,'READ')
      
      ##################
      #get up and down variations. NEED to be in differences: syst - central, except PDF and SCALE in relative
      ##################
      for v in ['U','D']:
        #Z+2b
        if z2b_systs:
          systTmp = syst 
          if syst == 'TTZ2B': systTmp = 'TT'
          if syst not in ['PDF','SCALE']:
            h_z2b = fSyst_z2b.Get(pltName + '_uf_amcatnlo_'+systTmp+v+'_'+chan+'_relErr') #this is signed relative unc.
            h_z2b.Multiply(h_z2b_nom) #multiply to central to get differences
            h_z2b.Add(h_z2b_nom) #actual distribution with variations
          else:
            if v == 'U': #only one variation for PDF and scale
              #pt_b1_uf_amcatnlo_pdfUnc_Electron
              h_z2b = fTheo_z2b.Get(pltName+'_uf_amcatnlo_'+syst.lower()+'Unc_'+chan)
        else:
          h_z2b = h_z2b_nom #use nominal one for unavailabe syst
        ####################
        #Z+1b
        ###################
        if z1b_systs:
          if syst not in ['PDF','SCALE']:
            h_z1b = fSyst_z1bU.Get('data_bw_xsec')
            if v == 'D':
              h_z1b = fSyst_z1bD.Get('data_bw_xsec')
            h_z1b = reduceBin(h_z1b,h_z2b_nom)
          else:
            n1 = 'UP_'
            if v == 'D': n1 = 'Down_'
            n2 = 'pdf_'
            if syst == 'SCALE': n2 = ''
            if syst == 'SCALE' or (syst == 'PDF' and v == 'U'): #pdf has one variation (up and down are the same), scale has up and down
              #hist_UP_pdf_"+fname_z1b_pltName+'_for_ratio'
              h_z1b = fTheo_z1b.Get('hist_'+n1+n2+fname_z1b_pltName+'_for_ratio')
              h_z1b = reduceBin(h_z1b,h_z2b_nom)
              #convert to relative unc.
              if syst == 'SCALE': #scale unc in z1b inputs is nom+err, so need to subtract norm and divide results to norm
                h_z1b.Add(h_z1b_nom,-1)
                h_z1b.Divide(h_z1b_nom)
              #pdf is already relative, do nothing

        else:
          h_z1b = h_z1b_nom
        
        #pdf and scale do not need variations for ratios
        if syst not in ['PDF', 'SCALE']:
          h_rat = h_z2b.Clone(h_z2b.GetName()+'_rat')
          h_rat.Divide(h_z1b)
        
        ###########################
        ###########################
        fOut.cd()
        if syst not in ['PDF','SCALE']:
          h_z2b.SetName(pltName + '_'+syst+v+'_z2b_'+chan)
          h_z2b.Write()
          h_z1b.SetName(pltName + '_'+syst+v+'_z1b_'+chan)
          h_z1b.Write()
          h_rat.SetName(pltName + '_'+syst+v+'_rat_'+chan)
          h_rat.Write()
        else:
          if syst == 'PDF' and v == 'U':
            h_z2b.SetName(pltName + '_'+syst+'_z2b_'+chan)
            h_z2b.Write()
            h_z1b.SetName(pltName + '_'+syst+'_z1b_'+chan)
            h_z1b.Write()
          if syst == 'SCALE':
            if v == 'U':
              h_z2b.SetName(pltName + '_'+syst+'_z2b_'+chan)
              h_z2b.Write()
            h_z1b.SetName(pltName + '_'+syst+v+'_z1b_'+chan)
            h_z1b.Write()
