## 23 Ago 2024

Working on `RecoLocalCalo/EcalRecProducers/plugins/EcalRecHitWorkerSimple.cc`. The most recent code is here in `ECAL/testDump/test/EcalRecHitWorkerSimple.cc`. I'm currently trying to implement the `FillInfo` library, with little to zero success!

## Run
```
cmsRun test/step3_RAW2DIGI_L1Reco_RECO.py
```
## 21 Ago 2024
Currently using `LaserResponseAnalyzer.cc`.

## 20 Ago 2024
It uses the plugin `LaserCorrectionAnalyzer.cc`. Should output `output.root`. The graph of Laser vs Time is currently empty because the xtal I chose doesn't get hit. 

