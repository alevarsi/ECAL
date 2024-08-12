// system include files
#include <memory>

#include "LaserCorrectionTimeAnalyzer.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"

LaserCorrectionTimeAnalyzer::LaserCorrectionTimeAnalyzer(const edm::ParameterSet& iConfig)
    : ebRecHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("EBRecHitTag"))),
      eeRecHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("EERecHitTag"))),
      laserDbToken_(esConsumes<EcalLaserDbService, EcalLaserDbRecord>()) // Inizializza il token
{
    edm::Service<TFileService> fs;
    tree_ = fs->make<TTree>("LaserCorrectionTimeTree", "Tree with laser correction and time");

    tree_->Branch("laserCorrection", &laserCorrection_, "laserCorrection/F");
    tree_->Branch("time", &time_, "time/F");
    tree_->Branch("energy", &energy_, "energy/F");
    tree_->Branch("hitDetId", &hitDetId_, "hitDetId/I");
}



void LaserCorrectionTimeAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    edm::Handle<EcalRecHitCollection> ebRecHits;
    iEvent.getByToken(ebRecHitToken_, ebRecHits);

    const auto& laser = iSetup.getData(laserDbToken_); // Usa il token per ottenere il servizio

    for (const auto& hit : *ebRecHits) {
        //laserCorrection_ = rand_range(1, 2);
        laserCorrection_ = laser.getLaserCorrection(hit.detid(), iEvent.time());
        time_ = hit.time();
        energy_ = hit.energy();
        hitDetId_ = hit.detid().rawId();

        tree_->Fill();
    }

    edm::Handle<EcalRecHitCollection> eeRecHits;
    iEvent.getByToken(eeRecHitToken_, eeRecHits);

    for (const auto& hit : *eeRecHits) {

        laserCorrection_ = laser.getLaserCorrection(hit.detid(), iEvent.time());
        //laserCorrection_ = rand_range(1, 2);
        time_ = hit.time();
        energy_ = hit.energy();
        hitDetId_ = hit.detid().rawId();

        tree_->Fill();
    }
}


void LaserCorrectionTimeAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("EBRecHitTag", edm::InputTag("ecalRecHit", "EcalRecHitsEB"));
    desc.add<edm::InputTag>("EERecHitTag", edm::InputTag("ecalRecHit", "EcalRecHitsEE"));
    //descriptions.add("laserCorrectionTimeAnalyzer", desc);
} 

LaserCorrectionTimeAnalyzer::~LaserCorrectionTimeAnalyzer() {}

DEFINE_FWK_MODULE(LaserCorrectionTimeAnalyzer);
