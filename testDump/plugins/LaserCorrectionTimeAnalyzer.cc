/*  // system include files
#include <memory>
#include <vector>
#include <iostream>

// CMSSW framework include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/ESGetToken.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CondFormats/RunInfo/interface/LHCInfo.h"
#include "CondFormats/DataRecord/interface/LHCInfoRcd.h"


#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"  // Includere EBDetId per l'ECAL Barrel
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TGraph.h"


// ROOT include files
#include "TGraph.h"
#include "TH1.h"

class LaserCorrectionTimeAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
    explicit LaserCorrectionTimeAnalyzer(const edm::ParameterSet&);
    ~LaserCorrectionTimeAnalyzer();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    // Member variables
    edm::EDGetTokenT<EcalRecHitCollection> ebRecHitToken_;
    edm::EDGetTokenT<EcalRecHitCollection> eeRecHitToken_;
    edm::ESGetToken<EcalLaserDbService, EcalLaserDbRecord> laserDbToken_;
    edm::ESGetToken<LHCInfo, LHCInfoRcd> lhcInfoToken_;
    
    float laserCorrection_;
    float time_;

    TGraph* laserCorrectionGraph_;  // Aggiungi un grafico per un cristallo specifico
    uint32_t selectedCrystal_; 
};

LaserCorrectionTimeAnalyzer::LaserCorrectionTimeAnalyzer(const edm::ParameterSet& iConfig)
    : ebRecHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("EBRecHitTag"))),
      eeRecHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("EERecHitTag"))),
      laserDbToken_(esConsumes<EcalLaserDbService, EcalLaserDbRecord>()),
      lhcInfoToken_(esConsumes<LHCInfo, LHCInfoRcd>())

{
    edm::Service<TFileService> fs;

    //graph LC
    laserCorrectionGraph_ = fs->make<TGraph>();
    laserCorrectionGraph_->SetName("laserCorrectionGraph");
    laserCorrectionGraph_->SetTitle("Laser Correction vs Time for Selected Crystal");
    laserCorrectionGraph_->GetXaxis()->SetTitle("Time");
    laserCorrectionGraph_->GetYaxis()->SetTitle("Laser Correction");
}

LaserCorrectionTimeAnalyzer::~LaserCorrectionTimeAnalyzer() {}

void LaserCorrectionTimeAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    
    const auto& lhcInfo = iSetup.getData(lhcInfoToken_);
    const auto& laser = iSetup.getData(laserDbToken_);

    auto fillStartTime = lhcInfo.beginTime();
    std::cout << "fillStartTime = " << fillStartTime << std::endl;

    laserCorrection_ = laser.getLaserCorrection(838861517, iEvent.time());
    float t_0 = 1722977545;
    time_ = iEvent.time().value() - t_0;

    int nPoints = laserCorrectionGraph_->GetN();
    laserCorrectionGraph_->SetPoint(nPoints, time_, laserCorrection_);

}

void LaserCorrectionTimeAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("EBRecHitTag", edm::InputTag("ecalRecHit", "EcalRecHitsEB"));
    desc.add<edm::InputTag>("EERecHitTag", edm::InputTag("ecalRecHit", "EcalRecHitsEE"));
    desc.add<uint32_t>("selectedCrystal", 838861517);  // ID del cristallo da analizzare 
    descriptions.add("laserCorrectionTimeAnalyzer", desc);
}

DEFINE_FWK_MODULE(LaserCorrectionTimeAnalyzer); 
*/