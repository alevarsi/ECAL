Main dumper. I'm currently working on

```

CalibCalorimetry/EcalLaserCorrection/src/EcalLaserDbService.cc

RecoLocalCalo/EcalRecProducers/plugins/EcalRecHitWorkerSimple.cc

```
## Run
```
cmsRun test/step3_RAW2DIGI_L1Reco_RECO.py
```
## 21 Ago 2024
Currently using `LaserResponseAnalyzer.cc`.

## 20 Ago 2024
It uses the plugin `LaserCorrectionAnalyzer.cc`. Should output `output.root`. The graph of Laser vs Time is currently empty because the xtal I chose doesn't get hit. 

