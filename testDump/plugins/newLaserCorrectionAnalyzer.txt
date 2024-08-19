#include <memory>
#include <iostream>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"  // Includere EBDetId per l'ECAL Barrel
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TGraph.h"

class LaserCorrectionAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit LaserCorrectionAnalyzer(const edm::ParameterSet&);
  ~LaserCorrectionAnalyzer() override = default;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  edm::EDGetTokenT<EcalRecHitCollection> recHitsToken_;
  edm::ESGetToken<EcalLaserDbService, EcalLaserDbRecord> laserDbServiceToken_;

  TH1F* laserCorrectionHist_;
  TGraph* laserCorrectionGraph_;
  std::vector<double> times_;
  std::vector<double> corrections_;
};

LaserCorrectionAnalyzer::LaserCorrectionAnalyzer(const edm::ParameterSet& iConfig) :
    recHitsToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("recHits"))),
    laserDbServiceToken_(esConsumes<EcalLaserDbService, EcalLaserDbRecord>()) {
  edm::Service<TFileService> fs;
  laserCorrectionHist_ = fs->make<TH1F>("laserCorrection", "Laser Correction;Correction;Entries", 100, 0, 2);
  laserCorrectionGraph_ = fs->make<TGraph>();
  laserCorrectionGraph_->SetName("laserCorrectionGraph");
  laserCorrectionGraph_->SetTitle("Laser Correction vs Time for One Crystal");
  laserCorrectionGraph_->GetXaxis()->SetTitle("Time (s)");
  laserCorrectionGraph_->GetYaxis()->SetTitle("Laser Correction");
}

void LaserCorrectionAnalyzer::beginJob() {}

void LaserCorrectionAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  const auto& laserDbService = iSetup.getData(laserDbServiceToken_);
  const auto& recHits = iEvent.get(recHitsToken_);

  for (const auto& hit : recHits) {
    double correction = laserDbService.getLaserCorrection(hit.detid(), iEvent.time());
    laserCorrectionHist_->Fill(correction);

    // Seleziona un cristallo specifico, ad esempio (ieta, iphi) = (1, 1) nella Barrel
    if (hit.id().subdetId() == EcalBarrel) {
      EBDetId ebId(hit.id());
      if (ebId.ieta() == 1 && ebId.iphi() == 1) {
        times_.push_back(iEvent.time().value());
        corrections_.push_back(correction);
      }
    }
  }
}

void LaserCorrectionAnalyzer::endJob() {
  // Riempie il grafico con i valori salvati
  for (size_t i = 0; i < times_.size(); ++i) {
    laserCorrectionGraph_->SetPoint(i, times_[i], corrections_[i]);
  }
}

void LaserCorrectionAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("recHits", edm::InputTag("ecalRecHit", "EcalRecHitsEB"));
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(LaserCorrectionAnalyzer);
