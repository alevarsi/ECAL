Main dumper. I'm currently working on

```

CalibCalorimetry/EcalLaserCorrection/src/EcalLaserDbService.cc

RecoLocalCalo/EcalRecProducers/plugins/EcalRecHitWorkerSimple.cc

```
## Run
```
cmsRun test/step3_RAW2DIGI_L1Reco_RECO.py
```
Should output `output.root`. The graph of Laser vs Time is currently empty because the xtal I chose doesn't get hit. 

