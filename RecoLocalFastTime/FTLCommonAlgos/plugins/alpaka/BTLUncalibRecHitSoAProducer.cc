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
          uncalibrh_{produces()}
    {}

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
      edm::ParameterSetDescription desc;
      desc.add<edm::InputTag>("digi");
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
          event.queue(), digi.view(), uncalibrh.view());

      // Move the SoA with the uncalibrh jets into the Event.
      event.emplace(uncalibrh_, std::move(uncalibrh));
    }

  private:
    const device::EDGetToken<btldigi::BTLDigiDeviceCollection> digi_;
    const device::EDPutToken<BTLUncalibRecHitDeviceCollection> uncalibrh_;


  };

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit

#include "HeterogeneousCore/AlpakaCore/interface/alpaka/MakerMacros.h"
DEFINE_FWK_ALPAKA_MODULE(btlrechit::BTLUncalibRecHitSoAProducer);
