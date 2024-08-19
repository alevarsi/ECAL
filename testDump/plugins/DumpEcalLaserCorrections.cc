#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/EcalObjects/interface/EcalLaserAPDPNRatios.h"
#include "CondFormats/DataRecord/interface/EcalLaserAPDPNRatiosRcd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/Provenance/interface/Timestamp.h"
#include <fstream>

class DumpEcalLaserCorrections : public edm::one::EDAnalyzer<> {
public:
  explicit DumpEcalLaserCorrections(const edm::ParameterSet&);
  ~DumpEcalLaserCorrections() override = default;

  void analyze(const edm::Event&, const edm::EventSetup&) override;

private:
  edm::EDGetTokenT<EcalRecHitCollection> recHitToken_;
  std::string outputFileName_;
  uint32_t detId_;
  edm::TimeValue_t initialTime_;
  bool firstEvent_;
  std::ofstream outputFile_;
};

DumpEcalLaserCorrections::DumpEcalLaserCorrections(const edm::ParameterSet& iConfig)
  : recHitToken_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("recHits"))),
    outputFileName_(iConfig.getParameter<std::string>("outputFileName")),
    detId_(iConfig.getParameter<uint32_t>("detId")),
    firstEvent_(true) {
  outputFile_.open(outputFileName_, std::ios::out);
  outputFile_ << "TimeSinceStart(s),LaserCorrection\n";
}

void DumpEcalLaserCorrections::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<EcalRecHitCollection> recHits;
  iEvent.getByToken(recHitToken_, recHits);

  edm::ESHandle<EcalLaserAPDPNRatios> laserRatios;
  //iSetup.get<EcalLaserAPDPNRatiosRcd>().get(laserRatios);

  const auto& ratiosMap = laserRatios->getLaserMap();

  edm::TimeValue_t currentTime = iEvent.time().value();
  if (firstEvent_) {
    initialTime_ = currentTime;
    firstEvent_ = false;
  }
  double timeSinceStart = (currentTime - initialTime_) / 1e9; // tempo in secondi

  auto it = ratiosMap.find(DetId(detId_));
  if (it != ratiosMap.end()) {
    outputFile_ << timeSinceStart << "," << it->p1 << "\n";
  } else {
    edm::LogWarning("DumpEcalLaserCorrections")
      << "No laser APD/Pn ratio found for crystal " << detId_ << ".";
  }
}

//DumpEcalLaserCorrections::~DumpEcalLaserCorrections() {
// outputFile_.close();
//}

DEFINE_FWK_MODULE(DumpEcalLaserCorrections);
