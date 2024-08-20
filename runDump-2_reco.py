# test reco and dump into a tree

import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
options.parseArguments()

process = cms.Process('EcalZee')

# import of standard configurations
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000)

process.TFileService = cms.Service("TFileService",
     fileName = cms.string(options.outputFile)
)

#process.options = cms.untracked.PSet(
 #  SkipEvent = cms.untracked.vstring('ProductNotFound'),
#)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('reco nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

process.CaloGeometryBuilder = cms.ESProducer("CaloGeometryBuilder",
    SelectedCalos = cms.vstring(
        'HCAL',
        'ZDC',
        #'CASTOR',
        'EcalBarrel',
        'EcalEndcap',
        'EcalPreshower',
        'TOWER'
    )
)

process.initialStepClassifier1 = cms.EDProducer("TrackMVAClassifierPrompt",
    beamspot = cms.InputTag("offlineBeamSpot"),
    ignoreVertices = cms.bool(True),
    mightGet = cms.optional.untracked.vstring,
    mva = cms.PSet(
        GBRForestFileName = cms.string(''),
        #GBRForestLabel = cms.string('MVASelectorIter0_13TeV')
    ),
    qualityCuts = cms.vdouble(-0.9, -0.8, -0.7),
    src = cms.InputTag("initialStepTracks"),
    vertices = cms.InputTag("firstStepPrimaryVertices")
)
process.initialStepClassifier2 = cms.EDProducer("TrackMVAClassifierDetached",
    beamspot = cms.InputTag("offlineBeamSpot"),
    ignoreVertices = cms.bool(False),
    mightGet = cms.optional.untracked.vstring,
    mva = cms.PSet(
        GBRForestFileName = cms.string(''),
        #GBRForestLabel = cms.string('MVASelectorIter3_13TeV')
    ),
    qualityCuts = cms.vdouble(-0.5, 0.0, 0.5),
    src = cms.InputTag("initialStepTracks"),
    vertices = cms.InputTag("firstStepPrimaryVertices")
)

process.initialStepClassifier3 = cms.EDProducer("TrackMVAClassifierPrompt",
    beamspot = cms.InputTag("offlineBeamSpot"),
    ignoreVertices = cms.bool(False),
    mightGet = cms.optional.untracked.vstring,
    mva = cms.PSet(
        GBRForestFileName = cms.string(''),
        #GBRForestLabel = cms.string('MVASelectorIter1_13TeV')
    ),
    qualityCuts = cms.vdouble(-0.6, -0.3, -0.1),
    src = cms.InputTag("initialStepTracks"),
    vertices = cms.InputTag("firstStepPrimaryVertices")
)

process.initialStepClassifier3 = cms.EDProducer("TrackMVAClassifierPrompt",
    beamspot = cms.InputTag("offlineBeamSpot"),
    ignoreVertices = cms.bool(False),
    mightGet = cms.optional.untracked.vstring,
    mva = cms.PSet(
        GBRForestFileName = cms.string(''),
        #GBRForestLabel = cms.string('MVASelectorIter1_13TeV')
    ),
    qualityCuts = cms.vdouble(-0.6, -0.3, -0.1),
    src = cms.InputTag("initialStepTracks"),
    vertices = cms.InputTag("firstStepPrimaryVertices")
)


process.displacedGeneralStepClassifier2 = cms.EDProducer("TrackMVAClassifierPrompt",
    beamspot = cms.InputTag("offlineBeamSpot"),
    ignoreVertices = cms.bool(False),
    mightGet = cms.optional.untracked.vstring,
    mva = cms.PSet(
        GBRForestFileName = cms.string(''),
        #GBRForestLabel = cms.string('MVASelectorIter0_13TeV')
    ),
    qualityCuts = cms.vdouble(0.0, 0.0, 0.0),
    src = cms.InputTag("displacedGeneralStepTracks"),
    vertices = cms.InputTag("firstStepPrimaryVertices")
)
process.displacedRegionalStepClassifier2 = cms.EDProducer("TrackMVAClassifierPrompt",
    beamspot = cms.InputTag("offlineBeamSpot"),
    ignoreVertices = cms.bool(False),
    mightGet = cms.optional.untracked.vstring,
    mva = cms.PSet(
        GBRForestFileName = cms.string(''),
        #GBRForestLabel = cms.string('MVASelectorIter0_13TeV')
    ),
    qualityCuts = cms.vdouble(0.0, 0.0, 0.0),
    src = cms.InputTag("displacedRegionalStepTracks"),
    vertices = cms.InputTag("firstStepPrimaryVertices")
)
process.lowPtTripletStep = cms.EDProducer("TrackMVAClassifierPrompt",
    beamspot = cms.InputTag("offlineBeamSpot"),
    ignoreVertices = cms.bool(False),
    mightGet = cms.optional.untracked.vstring,
    mva = cms.PSet(
        GBRForestFileName = cms.string(''),
        #GBRForestLabel = cms.string('MVASelectorIter1_13TeV')
    ),
    qualityCuts = cms.vdouble(-0.6, -0.3, -0.1),
    src = cms.InputTag("lowPtTripletStepTracks"),
    vertices = cms.InputTag("firstStepPrimaryVertices")
)

process.lowPtTripletStep = cms.EDProducer("TrackMVAClassifierPrompt",
    beamspot = cms.InputTag("offlineBeamSpot"),
    ignoreVertices = cms.bool(False),
    mightGet = cms.optional.untracked.vstring,
    mva = cms.PSet(
        GBRForestFileName = cms.string(''),
        #GBRForestLabel = cms.string('MVASelectorIter1_13TeV')
    ),
    qualityCuts = cms.vdouble(-0.6, -0.3, -0.1),
    src = cms.InputTag("lowPtTripletStepTracks"),
    vertices = cms.InputTag("firstStepPrimaryVertices")
)

process.detachedTripletStepClassifier1 = cms.EDProducer("TrackMVAClassifierDetached",
    beamspot = cms.InputTag("offlineBeamSpot"),
    ignoreVertices = cms.bool(False),
    mightGet = cms.optional.untracked.vstring,
    mva = cms.PSet(
        GBRForestFileName = cms.string(''),
        #GBRForestLabel = cms.string('MVASelectorIter3_13TeV')
    ),
    qualityCuts = cms.vdouble(-0.5, 0.0, 0.5),
    src = cms.InputTag("detachedTripletStepTracks"),
    vertices = cms.InputTag("firstStepPrimaryVertices")
)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '140X_dataRun3_v4', '') # 


#--------------------------
# Define PAT sequence
#--------------------------

# Standard PAT Configuration File
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.patElectrons.addElectronID = cms.bool(False)

# ---- remove MC references ----

from PhysicsTools.PatAlgos.tools.coreTools import removeMCMatching
removeMCMatching(process, ['All'], outputModules=[], postfix="")
#removeMCMatching(process, ['All'])
process.cleanPatTaus.preselection = cms.string('tauID("decayModeFinding") > 0.5 & tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") > 0.5 & tauID("againstMuonTight3") > 0.5 ')


process.patMETs.addGenMET = cms.bool(False)

process.options.allowUnscheduled = cms.untracked.bool(True)


# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('reco_RAW2DIGI_RECO.root'),
   # outputCommands = process.RECOSIMEventContent.outputCommands,
    outputCommands= cms.untracked.vstring("drop *",
                                          'keep *_ecal*_*_*'),
    splitLevel = cms.untracked.int32(0)
)


#--------------------------
# Define RECO sequence
#--------------------------

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

##################################
#### costumization for Stage2 ####

from HLTrigger.Configuration.customizeHLTforALL import customizeHLTforAll
#process = customizeHLTforAll(process,"GRun",_customInfo)

from HLTrigger.Configuration.customizeHLTforCMSSW import customizeHLTforCMSSW
process = customizeHLTforCMSSW(process,"GRun")


#--------------------------
# Tree producer
#--------------------------


#process.TreeProducer = cms.EDAnalyzer('TreeProducer',
 #                           EleTag    = cms.InputTag("patElectrons"),
  #                          vtxTag    = cms.InputTag("goodPrimaryVertices"),
   #                         SuperClusterEBTag    = cms.InputTag("particleFlowSuperClusterECAL:particleFlowSuperClusterECALBarrel"),
    #                        SuperClusterEETag    = cms.InputTag("particleFlowSuperClusterECAL:particleFlowSuperClusterECALEndcapWithPreshower"),
     #                      )

#vector<reco::SuperCluster>            "particleFlowSuperClusterECAL"   "particleFlowSuperClusterECALBarrel"   "RECO"    
#vector<reco::SuperCluster>            "particleFlowSuperClusterECAL"   "particleFlowSuperClusterECALEndcapWithPreshower"   "RECO"    


process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(50)

process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(options.inputFiles),
                                secondaryFileNames = cms.untracked.vstring()
                                )


#process.TreeProducer_step = cms.Path(
 #      process.patDefaultSequence *
  #     process.TreeProducer
   #    )


#
# Schedule definition
#
process.schedule = cms.Schedule(
                       process.raw2digi_step,
                       process.reconstruction_step,
                       process.endjob_step,
                       #process.TreeProducer_step,
                       process.RECOSIMoutput_step
                       )

#process.schedule = cms.Schedule(process.pEcalAlignment)
# from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
#associatePatAlgosToolsTask(process)

#Setup FWK for multithreaded
#process.options.numberOfThreads=cms.untracked.uint32(4)
#process.options.numberOfStreams=cms.untracked.uint32(0)

# Customisation from command line
#from Configuration.DataProcessing.RecoTLR import customiseDataRun2Common 

#call to customisation function customiseDataRun2Common imported from Configuration.DataProcessing.RecoTLR
#process = customiseDataRun2Common(process)

# Add early deletion of temporary data products to reduce peak memory need
#from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
#process = customiseEarlyDelete(process)
# End adding early deletion 