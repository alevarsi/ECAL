#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h" // Usa la nuova classe base
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "RecoLocalCalo/EcalRecProducers/interface/EcalRecHitWorkerBaseClass.h"

class LaserCorrectionTimeAnalyzer : public edm::one::EDAnalyzer<> { // Deriva da edm::one::EDAnalyzer<>
public:
    explicit LaserCorrectionTimeAnalyzer(const edm::ParameterSet&);
    ~LaserCorrectionTimeAnalyzer();

private:
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    edm::EDGetTokenT<EcalRecHitCollection> ebRecHitToken_;
    edm::EDGetTokenT<EcalRecHitCollection> eeRecHitToken_;
    TTree *tree_;
    float laserCorrection_;
    float time_;
};

LaserCorrectionTimeAnalyzer::LaserCorrectionTimeAnalyzer(const edm::ParameterSet& iConfig)
    : ebRecHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("EBRecHitTag"))),
      eeRecHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("EERecHitTag"))) {

    edm::Service<TFileService> fs;
    tree_ = fs->make<TTree>("LaserCorrectionTimeTree", "Tree with laser correction and time");

    tree_->Branch("laserCorrection", &laserCorrection_, "laserCorrection/F");
    tree_->Branch("time", &time_, "time/F");
}

void LaserCorrectionTimeAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    edm::Handle<EcalRecHitCollection> ebRecHits;
    iEvent.getByToken(ebRecHitToken_, ebRecHits);

    for (const auto& hit : *ebRecHits) {
        laserCorrection_ = hit.getLaserCorrection();
        time_ = hit.time();
        tree_->Fill();
    }

    edm::Handle<EcalRecHitCollection> eeRecHits;
    iEvent.getByToken(eeRecHitToken_, eeRecHits);

    for (const auto& hit : *eeRecHits) {
        laserCorrection_ = hit.getLaserCorrection();
        time_ = hit.time();
        tree_->Fill();
    }
}

void LaserCorrectionTimeAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("EBRecHitTag", edm::InputTag("ecalRecHit", "EcalRecHitsEB"));
    desc.add<edm::InputTag>("EERecHitTag", edm::InputTag("ecalRecHit", "EcalRecHitsEE"));
    descriptions.add("laserCorrectionTimeAnalyzer", desc);
}


LaserCorrectionTimeAnalyzer::~LaserCorrectionTimeAnalyzer() {}

// Define this as a plug-in
DEFINE_FWK_MODULE(LaserCorrectionTimeAnalyzer);
