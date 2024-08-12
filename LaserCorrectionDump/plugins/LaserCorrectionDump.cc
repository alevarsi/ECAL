// system include files
#include <memory>
#include <vector>
#include <fstream>

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
#include "CondFormats/DataRecord/interface/EcalLaserDbRecord.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"

class LaserCorrectionDump : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
    explicit LaserCorrectionDump(const edm::ParameterSet&);
    ~LaserCorrectionDump();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    edm::EDGetTokenT<EcalRecHitCollection> ebRecHitToken_;
    edm::EDGetTokenT<EcalRecHitCollection> eeRecHitToken_;
    edm::ESGetToken<EcalLaserDbService, EcalLaserDbRecord> laserDbToken_;
    
    std::ofstream outputFile_;
};

LaserCorrectionDump::LaserCorrectionDump(const edm::ParameterSet& iConfig)
    : ebRecHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("EBRecHitTag"))),
      eeRecHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("EERecHitTag"))),
      laserDbToken_(esConsumes<EcalLaserDbService, EcalLaserDbRecord>())
{
    // Apri un file per il dump della correzione laser
    outputFile_.open("laser_correction_dump.txt");
}

LaserCorrectionDump::~LaserCorrectionDump() {
    // Chiudi il file quando l'analizzatore viene distrutto
    outputFile_.close();
}

void LaserCorrectionDump::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    edm::Handle<EcalRecHitCollection> ebRecHits;
    iEvent.getByToken(ebRecHitToken_, ebRecHits);

    const auto& laser = iSetup.getData(laserDbToken_);

    for (const auto& hit : *ebRecHits) {
        float laserCorrection = laser.getLaserCorrection(hit.detid(), iEvent.time());
        outputFile_ << "EB: DetId: " << hit.detid().rawId() << ", LaserCorrection: " << laserCorrection << std::endl;
    }

    edm::Handle<EcalRecHitCollection> eeRecHits;
    iEvent.getByToken(eeRecHitToken_, eeRecHits);

    for (const auto& hit : *eeRecHits) {
        float laserCorrection = laser.getLaserCorrection(hit.detid(), iEvent.time());
        outputFile_ << "EE: DetId: " << hit.detid().rawId() << ", LaserCorrection: " << laserCorrection << std::endl;
    }
}

void LaserCorrectionDump::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("EBRecHitTag", edm::InputTag("ecalRecHit", "EcalRecHitsEB"));
    desc.add<edm::InputTag>("EERecHitTag", edm::InputTag("ecalRecHit", "EcalRecHitsEE"));
}

DEFINE_FWK_MODULE(LaserCorrectionDump);
