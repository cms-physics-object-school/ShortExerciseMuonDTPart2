import FWCore.ParameterSet.Config as cms
import subprocess

process = cms.Process("CMSPOStiming")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag.globaltag = "94X_dataRun2_v11"

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Run2017C/SingleMuon/AOD/17Nov2017-v1/70000/E2E0CA7E-7ADA-E711-9AC5-02163E019D1B.root'
    )
)

from RecoMuon.TrackingTools.MuonSegmentMatcher_cff import *

process.segmentNtupleFiller = cms.EDAnalyzer("MatchedSegmentTiming",
    MuonSegmentMatcher,

    Muons = cms.untracked.InputTag("muons"),
    TKtracks = cms.untracked.InputTag("generalTracks"),
    PrimaryVertex = cms.untracked.InputTag("offlinePrimaryVertices"),

    open = cms.string('recreate'),
    out = cms.string('muon_timing.root'),
)

process.jobPath = cms.Path(process.segmentNtupleFiller)

