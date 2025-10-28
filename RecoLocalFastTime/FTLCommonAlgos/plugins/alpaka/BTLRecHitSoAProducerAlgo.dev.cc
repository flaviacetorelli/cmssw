#include <alpaka/alpaka.hpp>

#include "DataFormats/FTLRecHitSoA/interface/BTLUncalibRecHitSoA.h"
#include "DataFormats/FTLRecHitSoA/interface/BTLRecHitSoA.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"
#include "HeterogeneousCore/AlpakaInterface/interface/workdivision.h"

#include "BTLRecHitSoAProducerAlgo.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit {

  using namespace ::btlrechit;

  class BTLUncalibToRecoKernel {
  public:

  
    ALPAKA_FN_ACC void operator()(Acc1D const& acc, 
                                  BTLUncalibRecHitSoA::ConstView input, 
                                  BTLRecHitSoA::View output,
                                  const double adcLSB_,
                                  const double toaLSBToNS_,
                                  const double timeCorr_p0_,
                                  const double timeCorr_p1_,
                                  const double timeCorr_p2_,
				  const double c_LYSO_) const { 
	                    // make a strided loop over the kernel grid, covering up to "size" elements

      	    
      for (int32_t i : cms::alpakatools::uniform_elements(acc, input.metadata().size())) { 


        auto entry = input[i];
        float time1 = -9999;
        float time2 = -9999;
        float time1L = -9999;
        float time1R = -9999;
	float position = -9999;
	float position_error = -9999;
	float time_error = -9999;
	float energy = -9999;
	float energyR = -9999;
	float energyL = -9999;
	uint8_t flag = 0; 

        // --- If available, reconstruct the amplitude and time of the first SiPM
        if (entry.time1R() > 0) {
	    
	    
            energyR = entry.ampR() * adcLSB_; 
            time1R = entry.time1R() - ( timeCorr_p0_ * pow(entry.ampR(), timeCorr_p1_) + timeCorr_p2_);
        
            flag |= 0x1;
        }
        
        if (entry.time1L() > 0) {
	  
          energyL = entry.ampL() * adcLSB_;
          time1L = entry.time1L() - (timeCorr_p0_ * pow(entry.ampR(), timeCorr_p1_) + timeCorr_p2_);
        
          flag |= (0x1 << 1);
        }
        
        // time error calculation to be added
	
	if (time1L > 0 && time1R > 0){
          time1 = 0.5f *( time1L + time1R );
	  time2 = 0.5f *( entry.time2L() + entry.time2R() ); // to be discussed
	  position = 0.5f * c_LYSO_ * (time1L - time1R); 
          position_error = 0.; //to be implemented
	  energy = (energyL + energyR )/2; 
        }
	else if (time1L > 0 && time1R < 0){
	  time1 = time1L; 
          time2 = entry.time2L();	  
          energy = energyL;
	}
	else if (time1L < 0 && time1R > 0){
          time1 = time1R; 
          time2 = entry.time2R();	  
          energy = energyR;
	}
	// fill the rechit 
        output[i] = {entry.detId(),
		    entry.row() , // dummy
		    time1,
		    time2,
		    energy, 
		    position, 
		    time_error, // dummy 
		    position_error, // dummy
		    flag	
	};
      }
    }
  };

  void BTLRecHitSoAProducerAlgo::fromUncalibToReco(Queue& queue,
                                                   BTLUncalibRecHitSoA::ConstView const& input,
                                                   BTLRecHitSoA::View& output,
						   const double adcLSB_,
						   const double toaLSBToNS_,
						   const double timeCorr_p0_,
						   const double timeCorr_p1_,
						   const double timeCorr_p2_,
						   const double c_LYSO_ ) {
    // Use 64 items per group.
    // This value is arbitrary, but it's a reasonable starting point.
    uint32_t items = 64;

    // Use as many groups as needed to cover the whole problem.
    // If this value is too large, a smaller number of blocks can give better performance.
    uint32_t groups = cms::alpakatools::divide_up_by(input.metadata().size(), items);

    auto grid = cms::alpakatools::make_workdiv<Acc1D>(groups, items);
    alpaka::exec<Acc1D>(queue, grid, BTLUncalibToRecoKernel{}, input, output,  adcLSB_, toaLSBToNS_, timeCorr_p0_, timeCorr_p1_, timeCorr_p2_, c_LYSO_);
    //alpaka::exec<Acc1D>(queue, grid, BTLUncalibToRecoKernel{}, input, output, c_LYSO_);
  }

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit
