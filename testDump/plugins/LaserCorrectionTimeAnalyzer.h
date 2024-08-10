#ifndef LASERCORRECTIONTIMEANALYZER_H
#define LASERCORRECTIONTIMEANALYZER_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "RecoLocalCalo/EcalRecProducers/interface/EcalRecHitWorkerBaseClass.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"


class LaserCorrectionTimeAnalyzer : public edm::one::EDAnalyzer<> {
public:
    explicit LaserCorrectionTimeAnalyzer(const edm::ParameterSet&);
    ~LaserCorrectionTimeAnalyzer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    edm::EDGetTokenT<EcalRecHitCollection> ebRecHitToken_;
    edm::EDGetTokenT<EcalRecHitCollection> eeRecHitToken_;
    edm::ESGetToken<EcalLaserDbService, EcalLaserDbRecord> laserDbToken_;

    //edm::EDGetTokenT<LumiScalersCollection> lumiScalerToken_;
    
    TTree *tree_;
    
    float laserCorrection_;
    float time_;
    float energy_;
    //float luminosity_;
    int hitDetId_;
};

#endif // LASERCORRECTIONTIMEANALYZER_H
