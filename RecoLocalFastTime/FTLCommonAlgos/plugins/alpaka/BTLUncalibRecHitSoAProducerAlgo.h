#ifndef RecoLocalFastTime_FTLCommonAlgos_plugins_alpaka_BTLUncalibRecHitSoAProducerAlgo_h
#define RecoLocalFastTime_FTLCommonAlgos_plugins_alpaka_BTLUncalibRecHitSoAProducerAlgo_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/ForwardDetId/interface/MTDDetId.h"
#include "DataFormats/FTLRecHitSoA/interface/BTLUncalibRecHitSoA.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit {

  using namespace ::btlrechit;

  struct BTLUncalibRecHitSoAProducerAlgo {

    static void  fromDigiToUncalib(Queue& queue,
                                    btldigi::BTLDigiSoA::ConstView const& input,
                                    BTLUncalibRecHitSoA::View& output,
		                    double adcLSB_, 
				    double timeCorr_p0_, 
			            double timeCorr_p1_, 
			            double timeCorr_p2_);

  };

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit

#endif  // RecoLocalFastTime_FTLCommonAlgos_plugins_alpaka_BTLUncalibRecHitSoAProducerAlgo_h

