import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("LaserCorrection")

options = VarParsing.VarParsing('analysis')
options.inputFiles = 'file:/afs/cern.ch/user/a/avarsi/CMSSW_14_0_13_patch1/src/ECAL/testDump/python/testCmsDriver.root'
options.maxEvents = -1
options.parseArguments()

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles)
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.maxEvents))

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run3_data', '')

process.TFileService = cms.Service("TFileService", fileName = cms.string('laser_correction_output.root'))

process.laserCorrectionAnalyzer = cms.EDAnalyzer('EcalLaserCorrectionAnalyzer',
    crystalID = cms.int32(300)  #cristallo a caso
)

process.p = cms.Path(process.laserCorrectionAnalyzer)
