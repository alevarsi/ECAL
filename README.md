## 12 Nov 2024

in `testDump/test` do RECO with

```
cmsRun hlt_reco.py inputFiles=file:/eos/user/a/avarsi/DATA/124c7812-eae6-4cdd-8b07-edde106a1609.root outputFile=outputHLT.root

cmsRun prompt_reco.py inputFiles=file:/eos/user/a/avarsi/DATA/124c7812-eae6-4cdd-8b07-edde106a1609.root outputFile=outputPROMPTRECO.root

```

Changes in `testDump/test/EcalLaserDbService.cc` are at line 165

##  Working on CMMSW in

```
CalibCalorimetry/EcalLaserCorrection/src/EcalLaserDbService.cc

```

## How to download this repository
```
in CMSSW_release/src
git clone git@github.com:alevarsi/ECAL
```
## How to create configuration file for digi -> reco

```
cmsDriver.py step3 --filein file:/eos/user/a/amassiro/ECAL/2024/Era2024G/Run384187/1eabd076-2d7c-48ce-9f3f-d101256e304f.root --fileout file:testCmsDriver.root --eventcontent RECO --datatier RECO --conditions auto:run3_data --step RAW2DIGI,L1Reco,RECO --nThreads 1 --geometry DB:Extended --era Run3 --no_exec -n 100
```

Test with
```

cmsRun step3_RAW2DIGI_L1Reco_RECO.py

```
Should output the file `testCmsDriver.root`






