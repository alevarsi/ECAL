#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

class LaserCorrectionTimeAnalyzer : public edm::EDAnalyzer {
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
        laserCorrection_ = hit.laserCorrection();
        time_ = hit.time();
        tree_->Fill();
    }

    edm::Handle<EcalRecHitCollection> eeRecHits;
    iEvent.getByToken(eeRecHitToken_, eeRecHits);

    for (const auto& hit : *eeRecHits) {
        laserCorrection_ = hit.laserCorrection();
        time_ = hit.time();
        tree_->Fill();
    }
}

LaserCorrectionTimeAnalyzer::~LaserCorrectionTimeAnalyzer() {}

// Define this as a plug-in
DEFINE_FWK_MODULE(LaserCorrectionTimeAnalyzer);
