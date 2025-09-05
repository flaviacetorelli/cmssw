#ifndef RecoLocalFastTime_FTLCommonAlgos_plugins_alpaka_BTLUncalibRecHitSoAProducerAlgo_h
#define RecoLocalFastTime_FTLCommonAlgos_plugins_alpaka_BTLUncalibRecHitSoAProducerAlgo_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/FTLRecHitSoA/interface/BTLUncalibRecHitSoA.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit {

  using namespace ::btlrechit;

  struct BTLUncalibRecHitSoAProducerAlgo {

    static void  fromDigiToUncalib(Queue& queue,
                                    btldigi::BTLDigiSoA::ConstView const& input,
                                    BTLUncalibRecHitSoA::View& output);

  };

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit

#endif  // RecoLocalFastTime_FTLCommonAlgos_plugins_alpaka_BTLUncalibRecHitSoAProducerAlgo_h

