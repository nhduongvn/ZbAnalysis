import os,sys
import ROOT

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

f = ROOT.TFile.Open("../Output_unfolding/unfolding_pdfScale_test2.root","READ")

for plt in ['Int_xSec','pt_b1_uf_amcatnlo']:
  for chan in ['Electron','Muon','Combined']:
    if plt == 'Int_xSec' and chan != 'Combined': continue
    if plt != 'Int_xSec':
      hN = plt + '_NONE_'+chan
    else:
      hN = plt + '_NONE'
    hC = f.Get(hN)
    c = ROOT.TCanvas(plt+"_scaleUnc_"+chan)
    leg = ROOT.TLegend(0.6,0.6,0.9,0.9)
    for iS in range(9):
      if plt!='Int_xSec':hN = plt + '_SCALE'+str(iS)+'_'+chan
      else: hN = plt + '_SCALE'+str(iS)
      h = f.Get(hN)
      h.Add(hC,-1)
      h.Divide(hC)
      if iS == 0:
        h.Draw("hist")
        h.GetYaxis().SetRangeUser(-0.05,0.05)
      else:
        h.Draw("hist same")
      h.SetLineColor(iS)
      leg.AddEntry(h,"Scale "+str(iS),'l')
    leg.Draw()
    c.Print(c.GetName()+'.pdf')


