import FWCore.ParameterSet.Config as cms

process = cms.Process("LaserCorrectionDump")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:your_input_file.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)  # Numero di eventi da processare
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("laser_correction_tree.root")
)

process.laserCorrectionDump = cms.EDAnalyzer('LaserCorrectionDump',
    EBRecHitTag = cms.InputTag("ecalRecHit", "EcalRecHitsEB"),
    EERecHitTag = cms.InputTag("ecalRecHit", "EcalRecHitsEE")
)

process.p = cms.Path(process.laserCorrectionDump)
