#ifndef RecoLocalFastTime_FTLCommonAlgos_plugins_alpaka_BTLRecHitSoAProducerAlgo_h
#define RecoLocalFastTime_FTLCommonAlgos_plugins_alpaka_BTLRecHitSoAProducerAlgo_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/FTLRecHitSoA/interface/BTLUncalibRecHitSoA.h"
#include "DataFormats/FTLRecHitSoA/interface/BTLRecHitSoA.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit {

  using namespace ::btlrechit;

  struct BTLRecHitSoAProducerAlgo {

    static void  fromUncalibToReco(Queue& queue,
                                    BTLUncalibRecHitSoA::ConstView const& input,
                                    BTLRecHitSoA::View& output, 
		                    double adcLSB_, 
		                    double toaLSBToNS_, 
				    double timeCorr_p0_, 
			            double timeCorr_p1_, 
			            double timeCorr_p2_, 
				    double c_LYSO_ );
  };

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit

#endif  // RecoLocalFastTime_FTLCommonAlgos_plugins_alpaka_BTLRecHitSoAProducerAlgo_h

