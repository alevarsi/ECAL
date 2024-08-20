# testDump
Repository I'm currently working, in as of 21 Ago 2024

Ignore the rest, it's just testing material.

# What to do
```

in CMSSW_release/src
git clone git@github.com:alevarsi/ECAL

```
# How to create configuration file for digi -> reco

type
```
cmsDriver.py step3 --filein file:/eos/user/a/amassiro/ECAL/2024/Era2024G/Run384187/1eabd076-2d7c-48ce-9f3f-d101256e304f.root --fileout file:testCmsDriver.root --eventcontent RECO --datatier RECO --conditions auto:run3_data --step RAW2DIGI,L1Reco,RECO --nThreads 1 --geometry DB:Extended --era Run3 --no_exec -n 100
```

Test with
```

cmsRun step3_RAW2DIGI_L1Reco_RECO.py

```
Should output the file `testCmsDriver.root`





