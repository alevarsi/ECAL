## 23 Ago 2024

Working on `RecoLocalCalo/EcalRecProducers/plugins/EcalRecHitWorkerSimple.cc`. The most recent code is here in `ECAL/testDump/test/EcalRecHitWorkerSimple.cc`. I'm currently trying to implement the `FillInfo` library, with little to zero success!

`compileErrors.txt` è l'errore che sto sperimentando ora alle `12:39` del `23 Ago 2024`

##  What I'm doing
 Working on

```

CalibCalorimetry/EcalLaserCorrection/src/EcalLaserDbService.cc

RecoLocalCalo/EcalRecProducers/plugins/EcalRecHitWorkerSimple.cc

```

## testDump
`testDump` is the repository I'm currently working in, as of 23 Ago 2024

Ignore the rest, it's just testing material.

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

## cc_transp.cc

It's just something cool to look at, not so useful because I used data from 2017 and I still can't obtain current data. 





