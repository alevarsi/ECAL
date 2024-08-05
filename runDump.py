# test reco and dump into a tree

import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
options.parseArguments()



#import EventFilter.ESRawToDigi.esRawToDigi_cfi
#ecalPreshowerDigis = EventFilter.ESRawToDigi.esRawToDigi_cfi.esRawToDigi.clone()



process = cms.Process('ECALNoise')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

#process.load('Configuration.StandardSequences.Services_cff')
#process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
#process.load('FWCore.MessageService.MessageLogger_cfi')
#process.load('Configuration.EventContent.EventContent_cff')
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')




process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10000)

process.TFileService = cms.Service("TFileService",
     fileName = cms.string(options.outputFile)
)

process.options = cms.untracked.PSet(
#    SkipEvent = cms.untracked.vstring('ProductNotFound'),
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('reco nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag


# 2021 ...
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_realistic', '')
#
#

process.GlobalTag = GlobalTag(process.GlobalTag, '140X_dataRun3_v3', '') # 
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/GTsRun3
# https://cms-conddb.cern.ch/cmsDbBrowser/search/Prod/140X_dataRun3_v3
#https://github.com/cms-sw/cmssw/blob/CMSSW_11_1_X/Configuration/AlCa/python/autoCond.py

#process.GlobalTag.toGet = cms.VPSet(
              #cms.PSet(record = cms.string("EcalPedestalsRcd"),
                       #tag =  cms.string("EcalPedestals_hlt"),
                       #connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS')
                       #),
#)




#process.PulseTreeProducer = cms.EDAnalyzer('ESPulseDumper',
 #                           ESdigiCollection = cms.InputTag("ecalPreshowerDigis")
  #                         )

#edm::SortedCollection<EcalUncalibratedRecHit,edm::StrictWeakOrdering<EcalUncalibratedRecHit> >    "ecalMultiFitUncalibRecHit"   "EcalUncalibRecHitsEB"   "RECO"    
#EBDigiCollection                      "ecalDigis"                 "ebDigis"         "RECO"    


process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(50)

process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(options.inputFiles),
                                secondaryFileNames = cms.untracked.vstring()
                                )



#ecalPreshowerDigis.sourceTag = 'rawDataCollector'

#process.raw2digi_step = cms.Path(process.RawToDigi)
#process.raw2digi_step = cms.Path(ecalPreshowerDigis)



process.raw2digi_step = cms.Path(process.RawToDigi)


#RawToDigiTask = cms.Task(
                         ##L1TRawToDigiTask,
                         #siPixelDigisTask,
                         ##siStripDigis,
                         ##ecalDigisTask,
                         #ecalPreshowerDigis,
                         ##hcalDigis,
                         ##muonCSCDigis,
                         ##muonDTDigis,
                         ##muonRPCDigis,
                         ##castorDigis,
                         ##scalersRawToDigi,
                         ##tcdsDigis,
                         ##onlineMetaDataDigis,
                         #)

#RawToDigi = cms.Sequence(RawToDigiTask)


#process.PulseTreeProducer_step = cms.Path(process.PulseTreeProducer)
process.endjob_step = cms.EndPath(process.endOfProcess)

process.schedule = cms.Schedule(
                                process.raw2digi_step,
                                #process.PulseTreeProducer_step, 
                                process.endjob_step
                                )


