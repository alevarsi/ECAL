/* // system include files
#include <memory>
#include <vector>

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
#include "TTree.h"

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
    
    TTree* tree_;
    float laserCorrection_;
    float time_;
    float energy_;
    int hitDetId_;
};

LaserCorrectionTimeAnalyzer::LaserCorrectionTimeAnalyzer(const edm::ParameterSet& iConfig)
    : ebRecHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("EBRecHitTag"))),
      eeRecHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("EERecHitTag"))),
      laserDbToken_(esConsumes<EcalLaserDbService, EcalLaserDbRecord>()),
      lhcInfoToken_(esConsumes<LHCInfo, LHCInfoRcd>())

{
    edm::Service<TFileService> fs;
    tree_ = fs->make<TTree>("LaserCorrectionTimeTree", "Tree with laser correction and time");

    tree_->Branch("laserCorrection", &laserCorrection_, "laserCorrection/F");
    tree_->Branch("time", &time_, "time/F");
    tree_->Branch("energy", &energy_, "energy/F");
    tree_->Branch("hitDetId", &hitDetId_, "hitDetId/I");
}

LaserCorrectionTimeAnalyzer::~LaserCorrectionTimeAnalyzer() {}

void LaserCorrectionTimeAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    edm::Handle<EcalRecHitCollection> ebRecHits;
    iEvent.getByToken(ebRecHitToken_, ebRecHits);
    
    const auto& lhcInfo = iSetup.getData(lhcInfoToken_);
    const auto& laser = iSetup.getData(laserDbToken_);

    auto fillStartTime = lhcInfo.beginTime();

    for (const auto& hit : *ebRecHits) {
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
}

DEFINE_FWK_MODULE(LaserCorrectionTimeAnalyzer); */
