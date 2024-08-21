#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"
#include "CondFormats/RunInfo/interface/FillInfo.h"
#include "CondFormats/DataRecord/interface/FillInfoRcd.h"  // Includi FillInfoRcd
#include "FWCore/Utilities/interface/typelookup.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TGraph.h"
#include <vector>
#include <memory>

class LaserResponseAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit LaserResponseAnalyzer(const edm::ParameterSet&);
  ~LaserResponseAnalyzer() override = default;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  edm::ESGetToken<EcalLaserDbService, EcalLaserDbRecord> laserDbServiceToken_;
  edm::ESGetToken<FillInfo, FillInfoRcd> lhcInfoToken_;

  uint32_t detId_;

  TGraph* laserResponseGraph_;
  std::vector<double> times_;
  std::vector<double> responses_;
};

LaserResponseAnalyzer::LaserResponseAnalyzer(const edm::ParameterSet& iConfig)
  : laserDbServiceToken_(esConsumes<EcalLaserDbService, EcalLaserDbRecord>()),
    lhcInfoToken_(esConsumes<FillInfo, FillInfoRcd>()),
    detId_(iConfig.getParameter<uint32_t>("detId")) {

  edm::Service<TFileService> fs;
  laserResponseGraph_ = fs->make<TGraph>();
  laserResponseGraph_->SetName("laserResponseGraph");
  laserResponseGraph_->SetTitle("Laser Response vs Time for One Crystal");
  laserResponseGraph_->GetXaxis()->SetTitle("Time (s)");
  laserResponseGraph_->GetYaxis()->SetTitle("Laser Response");
  laserResponseGraph_->SetLineColor(kRed);
}

void LaserResponseAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  const auto& laserDbService = iSetup.getData(laserDbServiceToken_);
  const auto& lhcInfo = iSetup.getData(lhcInfoToken_);
  
  // Ottieni il tempo di inizio del fill
  cond::Time_t fillStartTime = lhcInfo.createTime();
  std::cout << "fillStartTime = " << fillStartTime << std::endl;
  // Calcola il tempo dall'inizio del fill
  double timeFromFillStart = (iEvent.time().value() - fillStartTime) / 1e9; // Converti da ns a secondi
  
  // Ottieni la correzione laser per un dato DetId e tempo dell'evento
  DetId myDetId(detId_);
  double correction = laserDbService.getLaserCorrection(myDetId, iEvent.time());

  // Salva il tempo e la correzione per il grafico
  times_.push_back(timeFromFillStart);
  responses_.push_back(correction);
}

void LaserResponseAnalyzer::endJob() {
  // Riempie il TGraph con i valori salvati
  for (size_t i = 0; i < times_.size(); ++i) {
    laserResponseGraph_->SetPoint(i, times_[i], responses_[i]);
  }
}

void LaserResponseAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<uint32_t>("detId", 838861517);  // Default DetId
  descriptions.add("laserResponseAnalyzer", desc);
}

// Define this as a plug-in
DEFINE_FWK_MODULE(LaserResponseAnalyzer);
