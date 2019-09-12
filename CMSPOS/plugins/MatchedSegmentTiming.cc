#include "MatchedSegmentTiming.h"

// system include files
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include <TFile.h>

//
// constructors and destructor
//
MatchedSegmentTiming::MatchedSegmentTiming(const edm::ParameterSet& iConfig) 
  :
  MuonTags_(iConfig.getUntrackedParameter<edm::InputTag>("Muons")),
  VtxTags_(iConfig.getUntrackedParameter<edm::InputTag>("PrimaryVertex")),
  out(iConfig.getParameter<string>("out")),
  open(iConfig.getParameter<string>("open"))
{
  edm::ConsumesCollector collector(consumesCollector());

  edm::ParameterSet matchParameters = iConfig.getParameter<edm::ParameterSet>("MatchParameters");
  theMatcher_ = std::make_unique<MuonSegmentMatcher>(matchParameters, collector);

  muonToken_ = consumes<reco::MuonCollection>(MuonTags_);
  vertexToken_ = consumes<reco::VertexCollection>(edm::InputTag(VtxTags_));
}


MatchedSegmentTiming::~MatchedSegmentTiming()
{
  if (hFile!=0) {
    hFile->Close();
    delete hFile;
  }
}


// ------------ method called to for each event  ------------
void
MatchedSegmentTiming::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // We need to get the primary vertex as it is needed for the Tight Muon ID definition
  edm::Handle<reco::VertexCollection> recVtxs;
  iEvent.getByToken(vertexToken_,recVtxs);

  // Get the muon collection from the Event
  edm::Handle<reco::MuonCollection> MuCollection;
  iEvent.getByToken(muonToken_,MuCollection);
  
  // Loop over reconstructed muons
  const reco::MuonCollection muonC = *(MuCollection.product());
  for(reco::MuonCollection::const_iterator imuon = muonC.begin(); imuon != muonC.end(); ++imuon){

    // we need the muon to have the standalone track
    if ((imuon->standAloneMuon().isNull()) ) continue;

    // fill muon ID
    isLoose = muon::isLooseMuon(*imuon);
    isTight = muon::isHighPtMuon(*imuon, recVtxs->at(0) );

    // fill muon kinematics
    pt = imuon->pt();
    eta = imuon->eta();
    phi = imuon->phi();

    // ignore low pT muons
    if (pt<5.0) continue;

    nHitsDT=0;
    nHitsCSC=0;
    timeDT=0;
    timeCSC=0;
    timeMuon=0;
    
    if (imuon->isTimeValid()) timeMuon = imuon->time().timeAtIpInOut;
    
    int count=0;
    std::vector<const CSCSegment*> matchedSegmentsCSC = theMatcher_->matchCSC(*(imuon->standAloneMuon()),iEvent);
    for (std::vector<const CSCSegment*>::iterator hiti = matchedSegmentsCSC.begin(); hiti!=matchedSegmentsCSC.end();++hiti) {
      if ( !(*hiti)->isValid()) continue; 
      nHitsCSC+= (*hiti)->nRecHits();
      timeCSC+= (*hiti)->time();
      count++;
    }
    
    if (count) timeCSC/=(double)count;

    count=0;
    std::vector<const DTRecSegment4D*> matchedSegmentsDT = theMatcher_->matchDT(*(imuon->standAloneMuon()),iEvent);
    for (std::vector<const DTRecSegment4D*>::iterator hiti = matchedSegmentsDT.begin(); hiti!=matchedSegmentsDT.end();++hiti) {
      if ( !(*hiti)->isValid()) continue; 

      // only look at DT segments with the Phi projection present
      if (!(*hiti)->hasPhi()) continue;
    
      // segments at -999 have no timing measurement
      if ((*hiti)->phiSegment()->t0()==-999) continue;
      
      // only take segments with 6 hits or more
      if ((*hiti)->phiSegment()->recHits().size()<5) continue;

      nHitsDT+= (*hiti)->phiSegment()->recHits().size();
      timeDT+= (*hiti)->phiSegment()->t0();
      count++;
    }

    if (count) timeDT/=(double)count;

    // fill the ntuple
    t->Fill();
  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
MatchedSegmentTiming::beginJob()
{

   hFile = new TFile( out.c_str(), open.c_str() );
   hFile->cd();

   t = new TTree("Segments", "Segments");
   t->Branch("isLoose", &isLoose, "isLoose/I");
   t->Branch("isTight", &isTight, "isTight/I");
   t->Branch("pt", &pt, "pt/D");
   t->Branch("eta", &eta, "eta/D");
   t->Branch("phi", &phi, "phi/D");
   t->Branch("nHitsDT", &nHitsDT, "nHitsDT/I");
   t->Branch("nHitsCSC", &nHitsCSC, "nHitsCSC/I");
   t->Branch("timeDT", &timeDT, "timeDT/D");
   t->Branch("timeCSC", &timeCSC, "timeCSC/D");
   t->Branch("timeMuon", &timeMuon, "timeMuon/D");

}

// ------------- write ntuple to file  ------------
void MatchedSegmentTiming::endJob() {

  hFile->cd();
  t->Write();
  hFile->Write();
  delete t;  

}

//define this as a plug-in
DEFINE_FWK_MODULE(MatchedSegmentTiming);
