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

    // fill muon ID
    isLoose = muon::isLooseMuon(*imuon);
    isTight = muon::isHighPtMuon(*imuon, recVtxs->at(0) );

    timeMuon=0;
    // fill reco::Muon timing
    if (imuon->isTimeValid()) timeMuon = imuon->time().timeAtIpInOut;
    
    // we need the muon to have the standalone track
    if ((imuon->standAloneMuon().isNull()) ) continue;
	
	// get DT and CSC segements attached to the muon track
    std::vector<const CSCSegment*> matchedSegmentsCSC = theMatcher_->matchCSC(*(imuon->standAloneMuon()),iEvent);
    std::vector<const DTRecSegment4D*> matchedSegmentsDT = theMatcher_->matchDT(*(imuon->standAloneMuon()),iEvent);

	
	
	// insert analysis here
	
	
	
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
   
   // Loose muon ID bit
   t->Branch("isLoose", &isLoose, "isLoose/I");            

   // Tight muon ID bit
   t->Branch("isTight", &isTight, "isTight/I");

   // basic kinematical quantities for the muon
   t->Branch("pt", &pt, "pt/D");
   t->Branch("eta", &eta, "eta/D");
   t->Branch("phi", &phi, "phi/D");

   // numbers of hits used to calculate timing
   t->Branch("nHitsDT", &nHitsDT, "nHitsDT/I");
   t->Branch("nHitsCSC", &nHitsCSC, "nHitsCSC/I");

   // timing calculated from DT and/or CSC hits
   t->Branch("timeDT", &timeDT, "timeDT/D");
   t->Branch("timeCSC", &timeCSC, "timeCSC/D");

   // timing value cancluated but the muon reco code
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
