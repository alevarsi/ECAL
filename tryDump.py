import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

# Create a VarParsing object to handle command-line arguments
options = VarParsing('analysis')

# Define default values for the options
options.inputFiles = ['file:default.root']  # Default input file, can be overridden from command line
options.outputFile = 'output.root'  # Default output file

# Parse command-line arguments
options.parseArguments()

# Define the process
process = cms.Process('RECO')

# Import standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Set the GlobalTag (adjust this if necessary)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2022_realistic', '')

# Set the number of events to process
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Define the source with the input files from the command line
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles)
)

# Define a simple output module
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(options.outputFile)
)

# Define the path and end path
process.p = cms.Path()
process.e = cms.EndPath(process.out)

# Define the schedule
process.schedule = cms.Schedule(
    process.p,
    process.e
)

# Set options (number of threads, etc.)
process.options = cms.untracked.PSet(
    numberOfThreads = cms.untracked.uint32(4),
    numberOfStreams = cms.untracked.uint32(4)
)
