#ifndef MatchedSegmentTiming_H
#define MatchedSegmentTiming_H

// Base Class Headers
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "RecoMuon/TrackingTools/interface/MuonSegmentMatcher.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include <TTree.h>

using namespace std;
using namespace edm;

class MatchedSegmentTiming : public edm::EDAnalyzer {
public: 

  explicit MatchedSegmentTiming(const edm::ParameterSet&);
  ~MatchedSegmentTiming();

private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  edm::InputTag MuonTags_; 
  edm::InputTag VtxTags_; 
  edm::EDGetTokenT<reco::MuonCollection> muonToken_;
  edm::EDGetTokenT<reco::VertexCollection> vertexToken_;

  std::unique_ptr<MuonSegmentMatcher> theMatcher_;

  string out, open;
  TFile* hFile;
  TTree* t;
  
  // ntuple content
  int nHitsDT;
  int nHitsCSC;
  int isLoose, isTight;
  int isCollision;
  double eta,phi,timeDT,timeCSC,timeMuon;
  double pt;
};
#endif
