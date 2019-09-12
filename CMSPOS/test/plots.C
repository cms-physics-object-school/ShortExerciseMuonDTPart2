#include <iostream>
#include <fstream>
#include <TH1.h>
#include <TH2.h>
#include <TROOT.h>
#include <TFile.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

void plots() {

  TFile *file = TFile::Open("muon_timing.root");
  if (!file || file->IsZombie()) {
    return;
  }

  TTreeReader myReader("Segments",file);

  TTreeReaderValue<int>    isLoose(myReader, "isLoose");
  TTreeReaderValue<int>    isTight(myReader, "isTight");
  TTreeReaderValue<int>   nHitsDT(myReader, "nHitsDT");
  TTreeReaderValue<int>   nHitsCSC(myReader, "nHitsCSC");
  TTreeReaderValue<double>  pt(myReader, "pt");
  TTreeReaderValue<double>  phi(myReader, "phi");
  TTreeReaderValue<double>  eta(myReader, "eta");
  TTreeReaderValue<double> timeDT(myReader, "timeDT");
  TTreeReaderValue<double> timeCSC(myReader, "timeCSC");
  TTreeReaderValue<double> timeMuon(myReader, "timeMuon");

  TFile *outfile = TFile::Open("plots.root","recreate");
  outfile->cd();

  TH1D *hi_dttimew = new TH1D("hi_dttimew","DT Segment Time wide",80 ,-200,200);
  TH1D *hi_csctimew= new TH1D("hi_csctimew","CSC Segment Time wide",80 ,-200,200);
  TH1D *hi_dttime =  new TH1D("hi_dttime","DT Segment Time",80 ,-50,50);
  TH1D *hi_csctime = new TH1D("hi_csctime","CSC Segment Time",80 ,-50,50);
  TH1D *hi_mutime =  new TH1D("hi_mutime","reco::Muon Time",80 ,-50,50);
  TH1D *hi_dtcsc =   new TH1D("hi_dtcsc","CSC-DT Segment Time",80 ,-50,50);
  TH2D *hi2_dtcsc =  new TH2D("hi2_dtcsc","CSC vs DT",80 ,-100,100,80,-100,100);

  TH2D *hi_dt_phi = new TH2D("hi_dt_phi","DT Segment Time vs phi",80 ,-50,50,80,-3.14,3.14);
  TH2D *hi_csc_phi= new TH2D("hi_csc_phi","CSC Segment Time vs phi",80 ,-50,50,80,-3.14,3.14);
  TH2D *hi_dt_pt =  new TH2D("hi_dt_pt","DT Segment Time vs pT",80 ,-50,50,40,0,200);
  TH2D *hi_csc_pt = new TH2D("hi_csc_pt","CSC Segment Time vs pT",80 ,-50,50,40,0,200);

  unsigned int counter=0;

  // Loop over all entries of the TTree
  myReader.SetEntry(0);
  while (myReader.Next()) {
    if (*pt<5) continue;
    if (!*isLoose) continue;
    if (*timeMuon!=0) hi_mutime->Fill(*timeMuon);
    if (*nHitsDT>5) {
      hi_dttime->Fill(*timeDT);
      hi_dt_phi->Fill(*timeDT,*phi);
      hi_dt_pt->Fill(*timeDT,*pt);
      hi_dttimew->Fill(*timeDT);
    }
    if (*nHitsCSC>3) {
      hi_csctime->Fill(*timeCSC);
      hi_csc_phi->Fill(*timeCSC,*phi);
      hi_csc_pt->Fill(*timeCSC,*pt);
      hi_csctimew->Fill(*timeCSC);
    }
    if (*nHitsDT>5 && *nHitsCSC>3) {
      hi_dtcsc->Fill(*timeCSC-*timeDT);
      hi2_dtcsc->Fill(*timeCSC,*timeDT);
    }
    
    counter++;
    if (counter/100000*100000==counter) cout << " Processing muon #" << counter << endl;
  }

  hi_dttime->Fit("gaus");
  hi_csctime->Fit("gaus");
  hi_mutime->Fit("gaus");
  
  outfile->Write();
  outfile->Close();
}
