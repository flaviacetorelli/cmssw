#include <utility>

#include "DataFormats/FTLRecHitSoA/interface/alpaka/BTLUncalibRecHitDeviceCollection.h"
#include "DataFormats/FTLDigiSoA/interface/alpaka/BTLDigiDeviceCollection.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/EDGetToken.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/EDPutToken.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/Event.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/EventSetup.h"
#include "HeterogeneousCore/AlpakaCore/interface/alpaka/global/EDProducer.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"

#include "BTLUncalibRecHitSoAProducerAlgo.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit {

  using namespace ::btlrechit;

  class BTLUncalibRecHitSoAProducer : public global::EDProducer<> {
  public:
    // constructor	  
    BTLUncalibRecHitSoAProducer(edm::ParameterSet const& config)
        : EDProducer<>(config),
          digi_{consumes(config.getParameter<edm::InputTag>("digi"))},
          uncalibrh_{produces()},
	  adcNBits_(config.getParameter<uint32_t>("adcNbits")),
          adcSaturation_(config.getParameter<double>("adcSaturation")),
          adcLSB_(adcSaturation_ / (1 << adcNBits_)),
	  timeCorr_p0_(config.getParameter<double>("timeCorr_p0")),
          timeCorr_p1_(config.getParameter<double>("timeCorr_p1")),
          timeCorr_p2_(config.getParameter<double>("timeCorr_p2"))
    {}

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
      edm::ParameterSetDescription desc;
      desc.add<edm::InputTag>("digi");
      desc.add<uint32_t>("adcNbits");
      desc.add<double>("adcSaturation");
      desc.add<double>("timeCorr_p0");
      desc.add<double>("timeCorr_p1");
      desc.add<double>("timeCorr_p2");
      descriptions.addWithDefaultLabel(desc);
    }

    void produce(edm::StreamID sid, device::Event& event, device::EventSetup const& setup) const override {
      // NB should be inserted a method to retrieve calibrations, now they are fixed to default values
      // Get the digi from the Event.
      btldigi::BTLDigiDeviceCollection const& digi = event.get(digi_); // this should match Claudio Class name

      // Allocate a new SoA for the uncalibrh jets. // same number of elements we have in input
      BTLUncalibRecHitDeviceCollection uncalibrh(digi.view().metadata().size(), event.queue());

      // Apply the corrections and fill the new SoA. // these launch the kernel, and will run on gpu async
      BTLUncalibRecHitSoAProducerAlgo::fromDigiToUncalib(
          event.queue(), digi.view(), uncalibrh.view(), adcLSB_, timeCorr_p0_, timeCorr_p1_, timeCorr_p2_);

      // Move the SoA with the uncalibrh jets into the Event.
      event.emplace(uncalibrh_, std::move(uncalibrh));
    }

  private:
    const device::EDGetToken<btldigi::BTLDigiDeviceCollection> digi_;
    const device::EDPutToken<BTLUncalibRecHitDeviceCollection> uncalibrh_;
    uint32_t adcNBits_;
    const double adcSaturation_;
    const double adcLSB_;
    const double timeCorr_p0_;
    const double timeCorr_p1_;
    const double timeCorr_p2_;

  };

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit

#include "HeterogeneousCore/AlpakaCore/interface/alpaka/MakerMacros.h"
DEFINE_FWK_ALPAKA_MODULE(btlrechit::BTLUncalibRecHitSoAProducer);
