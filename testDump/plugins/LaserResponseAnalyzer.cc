#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"

#include "CondCore/ESSources/interface/registration_macros.h"
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"
#include "CondFormats/RunInfo/interface/RunSummary.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/DataRecord/interface/L1TriggerScalerRcd.h"
#include "CondFormats/RunInfo/interface/L1TriggerScaler.h"
#include "CondFormats/DataRecord/interface/MixingRcd.h"
#include "CondFormats/RunInfo/interface/MixingModuleConfig.h"
#include "CondFormats/DataRecord/interface/LHCInfoRcd.h"
#include "CondFormats/RunInfo/interface/LHCInfo.h"
#include "CondFormats/DataRecord/interface/LHCInfoPerLSRcd.h"
#include "CondFormats/RunInfo/interface/LHCInfoPerLS.h"
#include "CondFormats/DataRecord/interface/LHCInfoPerFillRcd.h"
#include "CondFormats/RunInfo/interface/LHCInfoPerFill.h" 
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
  
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  edm::ESGetToken<EcalLaserDbService, EcalLaserDbRecord> laserDbServiceToken_;
  //#edm::ESHandle<FillInfo> fillRcd;
  //edm::ESGetToken<LHCInfoPerFill, LHCInfoPerFillRcd> lhcInfoToken_;

  uint32_t detId_;

  TGraph* laserResponseGraph_;
  //std::vector<double> times_;
  //std::vector<double> responses_;
};

LaserResponseAnalyzer::LaserResponseAnalyzer(const edm::ParameterSet& iConfig)
  : laserDbServiceToken_(esConsumes<EcalLaserDbService, EcalLaserDbRecord>()),
    //lhcInfoToken_(esConsumes<LHCInfoPerFill, LHCInfoPerFillRcd>()),
    detId_(iConfig.getParameter<uint32_t>("detId")) {
   
  usesResource("TFileService");
  edm::Service<TFileService> fs;

  laserResponseGraph_ = fs->make<TGraph>();
  laserResponseGraph_->SetName("laserResponseGraph");
  laserResponseGraph_->SetTitle("Laser Correction vs Time for One Crystal");
  laserResponseGraph_->GetXaxis()->SetTitle("Time (s)");
  laserResponseGraph_->GetYaxis()->SetTitle("Laser Correction");
  laserResponseGraph_->SetLineColor(kRed);
  laserResponseGraph_->SetLineWidth(kRed);
  laserResponseGraph_->SetMarkerStyle(21);
  laserResponseGraph_->SetMarkerColor(kRed);
  laserResponseGraph_->SetMarkerSize(0.7);

}

void LaserResponseAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  const auto& laserDbService = iSetup.getData(laserDbServiceToken_);
  //const auto& lhcInfo = iSetup.getData(lhcInfoToken_);

  double runStartTime = iEvent.getRun().beginTime().value();

  double timeFromFillStart = (iEvent.time().value() - runStartTime) / 1e9; // Converti da ns a secondi

  DetId myDetId(detId_);
  double correction = laserDbService.getLaserCorrection(myDetId, iEvent.time());

  laserResponseGraph_->SetMarkerStyle(21);
  laserResponseGraph_->SetPoint(laserResponseGraph_->GetN(), timeFromFillStart, correction);
  

  // --- some checks ---
  
  //std::cout << "\n runStartTime = " << runStartTime << std::endl;
  //std::cout << "\n EventTime" << iEvent.time().value() << std::endl;
  //std::cout << "\n TimeFromRunStart = " << timeFromFillStart << std::endl;
  //std::cout << "\n Correction = " << correction << std::endl;


  /* times_.push_back(timeFromFillStart);
  responses_.push_back(correction); */
}


void LaserResponseAnalyzer::beginJob()
{
}

void LaserResponseAnalyzer::endJob() 
{
}

void LaserResponseAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<uint32_t>("detId", 838861517);  // Default DetId
  descriptions.add("laserResponseAnalyzer", desc);
}

// Define this as a plug-in
DEFINE_FWK_MODULE(LaserResponseAnalyzer);
