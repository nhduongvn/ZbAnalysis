import os,sys
import ROOT
import math

def CombineUnc(hU_in,hD_in):
  hC_out = hU_in.Clone(hU_in.GetName()+'_CombinedUnc') #this is uncertainty combining up and down variations, taking maxium variations in absolute value
  hC_out.Reset()
  for iB in range(1,hC_out.GetNbinsX()+1):
    err = max(abs(hU_in.GetBinContent(iB)),abs(hD_in.GetBinContent(iB)))
    hC_out.SetBinContent(iB, err)
    hC_out.SetBinError(iB,0)
  return hC_out

def CalUnc(hU_1,hU_2): #add uncertainties in quadrature sum
  hU_out = hU_1.Clone(hU_1.GetName()+'_quad_sum')
  hU_out.Reset()
  for i in range(1,hU_1.GetNbinsX()+1):
    err1 = hU_1.GetBinContent(i)
    err2 = hU_2.GetBinContent(i)
    err = math.sqrt(err1*err1+err2*err2)
    hU_out.SetBinContent(i,err)
  return hU_out


pltNames = ['pt_b1']

chans = ['Combined']

#systs = ['JES'] 
systs = ['JES','JER','PU','METJES','METJER','METUNCLUST','TTZ2B','DYXSEC','WWXSEC','WZXSEC','ZZXSEC','TTZ1B','CLO','PUMVA','PDF','SCALE']

fOut = ROOT.TFile.Open('ratio.root','RECREATE')

fIn = ROOT.TFile.Open('ratio_unc.root','READ')

for chan in chans:
  for pltName in pltNames:
    #################
    #get central distributions
    #################
    #if central only fill h
    hN = fIn.Get(pltName + '_NOM_rat_'+chan)
    
    #################
    #loop over syst to get unc
    #################
    hSysts_rel = {}

    for syst in systs:
      #treat PDF and SCALE differently
      if syst in ['PDF','SCALE']: continue
      hU = fIn.Get(pltName + '_' +syst+'U_rat_'+chan)
      hD = fIn.Get(pltName + '_' +syst+'D_rat_'+chan)
      hU_abs = hU.Clone(hU.GetName()+'_abs')
      hU_abs.Add(hN,-1)
      hD_abs = hD.Clone(hD.GetName()+'_abs')
      hD_abs.Add(hN,-1)
      
      hC_abs = CombineUnc(hU_abs,hD_abs) #this is uncertainty combining up and down variations, taking maxium variations in absolute value
      hC_abs.SetName(hU.GetName().replace(syst+'U',syst+'C')+'_abs')
      hC_rel = hC_abs.Clone(hC_abs.GetName().replace('abs','rel'))
      if syst == 'JES': hC_rel.Print("all")
      hC_rel.Divide(hN)
      if syst == 'JES': hC_rel.Print("all") 
      hSysts_rel[syst] = hC_rel
      if syst == 'JES': hSysts_rel['JES'].Print("all") 
    
    hSysts_rel['JES'].Print("all")
    #PDF and SCALE: add in quadrature of z1b and z2b
    hPDF_z2b = fIn.Get(pltName + '_PDF_z2b_' + chan)
    hPDF_z1b = fIn.Get(pltName + '_PDF_z1b_' + chan)
    hScale_z2b = fIn.Get(pltName + '_SCALE_z2b_' + chan)
    hScaleU_z1b = fIn.Get(pltName + '_SCALEU_z1b_' + chan)
    hScaleD_z1b = fIn.Get(pltName + '_SCALED_z1b_' + chan)
    print hScaleU_z1b, " ", hScaleD_z1b
    hScale_z1b = CombineUnc(hScaleU_z1b,hScaleD_z1b)
    hPDF = CalUnc(hPDF_z2b,hPDF_z1b)
    hScale = CalUnc(hScale_z2b,hScale_z1b)
    hSysts_rel['PDF'] = hPDF
    hSysts_rel['SCALE'] = hScale

    #sum up all unc. in quadrature
    hS_tot_rel = hSysts_rel['JES'].Clone(hSysts_rel['JES'].GetName().replace('JES','Tot'))
    hS_tot_rel.Multiply(hS_tot_rel) #square the relative unc.
    for syst in systs:
      if syst != 'JES':
        hTmp = hSysts_rel[syst].Clone(hSysts_rel[syst].GetName()+'_tmp')
        hTmp.Multiply(hTmp)
        hS_tot_rel.Add(hTmp)

    for i in range(1,hS_tot_rel.GetNbinsX()+1):
      err = hS_tot_rel.GetBinContent(i)
      err = math.sqrt(err)
      hS_tot_rel.SetBinContent(i,err)
     
    #############
    #now get gen distributions
    ############
    h_amcatnlo = fIn.Get(pltName+'_NOM_gen_amcatnlo_rat_'+chan)
    #PDF and SCALE: add in quadrature of z1b and z2b
    #pt_b1_pdf_z2b_gen_amcatnlo_Combined
    hPDF_z2b = fIn.Get(pltName + '_pdf_z2b_gen_amcatnlo_' + chan)
    hPDF_z1b = fIn.Get(pltName + '_pdf_z1b_gen_amcatnlo_' + chan)
    hScale_z2b = fIn.Get(pltName + '_scale_z2b_gen_amcatnlo_' + chan)
    hScaleU_z1b = fIn.Get(pltName + '_scaleU_z1b_gen_amcatnlo_' + chan)
    hScaleD_z1b = fIn.Get(pltName + '_scaleD_z1b_gen_amcatnlo_' + chan)
    hScale_z1b = CombineUnc(hScaleU_z1b,hScaleD_z1b)
    hPDF = CalUnc(hPDF_z2b,hPDF_z1b)
    hScale = CalUnc(hScale_z2b,hScale_z1b)
    hTotUnc_amcatnlo = CalUnc(hPDF,hScale)
    h_mg = fIn.Get(pltName+'_NOM_gen_mg_rat_'+chan)
    h_sherpa = fIn.Get(pltName+'_NOM_gen_sherpa_rat_'+chan)
    hTotUnc_amcatnlo.Print("all")

    fOut.cd()
    hN.Write()
    for syst in systs:
      hSysts_rel[syst].SetName(pltName + '_'+syst+'Unc_rat_'+chan)
      hSysts_rel[syst].Write()

    hS_tot_rel.SetName(pltName + '_totUnc_rat_' + chan)
    hS_tot_rel.Write()
    
    h_amcatnlo.Write()
    h_mg.Write()
    h_sherpa.Write()
    hTotUnc_amcatnlo.SetName(pltName+'_PDFSCALE_gen_amcatnlo_rat_'+chan)
    hTotUnc_amcatnlo.Write()
