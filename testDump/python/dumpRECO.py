import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("CondCore.CondDB.CondDB_cfi")
process.load("CalibCalorimetry.EcalLaserCorrection.ecalLaserCorrectionService_cfi")


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data', '')


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/a/avarsi/CMSSW_14_0_13_patch1/src/ECAL/testDump/python/dataRECO.root')
)

process.demo = cms.EDAnalyzer('LaserCorrectionTimeAnalyzer',
    recHits = cms.InputTag("ecalRecHit", "EcalRecHitsEB")
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('outputDUMP.root')
)

process.p = cms.Path(process.demo)
