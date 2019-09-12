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

  // define access to all the variables in the ntuple
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

  TH1D *hi_dttime =  new TH1D("hi_dttime","DT Segment Time",80 ,-50,50);

  // define histograms here
  
  // Loop over all entries of the TTree
  myReader.SetEntry(0);
  while (myReader.Next()) {

      hi_dttime->Fill(*timeDT);

      // insert analysis here

  }
  
  outfile->Write();
  outfile->Close();
  
  file->Close();
}
