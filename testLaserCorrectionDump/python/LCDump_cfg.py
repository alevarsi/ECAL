import FWCore.ParameterSet.Config as cms

process = cms.Process("LaserCorrectionDump")

# Source of the RAW data
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/eos/user/a/amassiro/ECAL/2024/Era2024G/Run384187/1eabd076-2d7c-48ce-9f3f-d101256e304f.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)  # Numero di eventi da processare
)

# Standard sequences for unpacking and reconstruction
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

# TFileService for output
process.TFileService = cms.Service("TFileService",
    fileName = cms.string("laser_correction_tree.root")
)

# Your EDAnalyzer
process.laserCorrectionDump = cms.EDAnalyzer('LaserCorrectionDump',
    EBRecHitTag = cms.InputTag("ecalRecHit", "EcalRecHitsEB"),
    EERecHitTag = cms.InputTag("ecalRecHit", "EcalRecHitsEE")
)

# Path to run the sequences and analyzer
process.p = cms.Path(
    process.RawToDigi *          # Unpacking RAW data
    process.reconstruction *     # Full reconstruction sequence
    process.laserCorrectionDump  # Your custom analyzer
)

# EndPath if you have an output module (optional)
# process.out = cms.OutputModule("PoolOutputModule",
#     fileName = cms.untracked.string('reco_output.root')
# )
# process.end = cms.EndPath(process.out)
