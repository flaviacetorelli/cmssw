#include <utility>

#include "DataFormats/FTLRecHitSoA/interface/alpaka/BTLUncalibRecHitDeviceCollection.h"
#include "DataFormats/FTLRecHitSoA/interface/alpaka/BTLRecHitDeviceCollection.h"
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

#include "BTLRecHitSoAProducerAlgo.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit {

  using namespace ::btlrechit;

  class BTLRecHitSoAProducer : public global::EDProducer<> {
  public:
    // constructor	  
    BTLRecHitSoAProducer(edm::ParameterSet const& config)
        : EDProducer<>(config),
          uncalibrh_{consumes(config.getParameter<edm::InputTag>("uncalibrh"))},
          rh_{produces()},
          c_LYSO_(config.getParameter<double>("c_LYSO"))
	  {}
          

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
      edm::ParameterSetDescription desc;
      desc.add<edm::InputTag>("uncalibrh");
      desc.add<double>("c_LYSO");
      descriptions.addWithDefaultLabel(desc);
    }

    void produce(edm::StreamID sid, device::Event& event, device::EventSetup const& setup) const override {
      // NB should be inserted a method to retrieve calibrations, now they are fixed to default values
      // Get the uncalib from the Event.
      BTLUncalibRecHitDeviceCollection const& uncalibrh = event.get(uncalibrh_);

      // Allocate a new SoA for the rechit.
      BTLRecHitDeviceCollection rh(uncalibrh.view().metadata().size(), event.queue());

      // Apply the corrections and fill the new SoA. // these launch the kernel, and will run on gpu async
      BTLRecHitSoAProducerAlgo::fromUncalibToReco(
          event.queue(), uncalibrh.view(), rh.view(),  c_LYSO_);

      // Move the SoA with the rh into the Event.
      event.emplace(rh_, std::move(rh));
    }

  private:
    const device::EDGetToken<BTLUncalibRecHitDeviceCollection> uncalibrh_;
    const device::EDPutToken<BTLRecHitDeviceCollection> rh_;
    const double c_LYSO_;

  };

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit

#include "HeterogeneousCore/AlpakaCore/interface/alpaka/MakerMacros.h"
DEFINE_FWK_ALPAKA_MODULE(btlrechit::BTLRecHitSoAProducer);
