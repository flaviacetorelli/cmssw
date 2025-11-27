//#define EDM_ML_DEBUG
#include <alpaka/alpaka.hpp>

#include "RecoLocalFastTime/FTLCommonAlgos/interface/MTDTimeCalib.h"
#include "DataFormats/FTLRecHitSoA/interface/BTLUncalibRecHitSoA.h"
#include "DataFormats/FTLRecHitSoA/interface/BTLRecHitSoA.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"
#include "HeterogeneousCore/AlpakaInterface/interface/workdivision.h"

#include "BTLRecHitSoAProducerAlgo.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit {

  using namespace ::btlrechit;
  ALPAKA_FN_ACC float
	 timeResolutionInNs(float amp){
	 return 0.0593858*pow(amp,-1.02826)+0.0156719; 
	 }


  class BTLUncalibToRecoKernel {
  public:

  
    ALPAKA_FN_ACC void operator()(Acc1D const& acc, 
                                  BTLUncalibRecHitSoA::ConstView input, 
                                  BTLRecHitSoA::View output,
				  const double c_LYSO_, 
				  const double thresholdToKeep_,
				  const double calibration_) const { 
	                    // make a strided loop over the kernel grid, covering up to "size" elements

      	    
      for (int32_t i : cms::alpakatools::uniform_elements(acc, input.metadata().size())) { 


        auto entry = input[i];
        float time1 = -9999;
        float time2 = -9999;
	float position = -9999;
	float position_error = -9999;
	float time_error = -9999;
	float energy = -9999;
	uint8_t flag = 0; 

        
        //!!!!!!! time error calculation to be added
        //!!!!!!! position error calculation to be added
	
	
        // --- If available, reconstruct the amplitude and time of the first SiPM
	if (entry.time1L() < 0 && entry.time1R() > 0){
          time1 = entry.time1R(); 
          time2 = entry.time2R();	  
          energy = entry.ampR(); 
          flag |= 0x1;
	}
        // --- If available, reconstruct the amplitude and time of the second SiPM
	else if (entry.time1L() > 0 && entry.time1R() < 0){
	  time1 = entry.time1L(); 
          time2 = entry.time2L();	  
          energy = entry.ampL();
          flag |= (0x1 << 1);
	}
	// -- if you have both sipm info
        else if (entry.time1L() > 0 && entry.time1R() > 0){
          time1 = 0.5f *( entry.time1L() + entry.time1R() );
	  time2 = 0.5f *( entry.time2L() + entry.time2R() ); // to be discussed
	  position = 0.5f * c_LYSO_ * (entry.time1L() - entry.time1R()); 
          position_error = 0.; //to be implemented
	  energy = (entry.ampR() + entry.ampL() ) / 2.; 
        }

        // energy calibration
	energy *= calibration_;

        // --- Time calibration: for the time being just removes a time offset in BTL
        //time1 += time_calib_->getTimeCalib(entry.detId());
	
	time_error = timeResolutionInNs(energy);


        // Now fill flags
        // all rechits from the digitizer are "good" at present, good is 1, bad is 0
        if (energy > thresholdToKeep_) {
          flag = 1;
        } else {
          flag = 0;
        }

	#ifdef EDM_ML_DEBUG

	    printf("RecHit SoA with raw id %i \n", entry.detId().rawId()); 
	    printf("Time 1  L,R (%f, %f) and average, error (%f, %f) \n", entry.time1L(), entry.time1R(), time1, time_error); 
	    printf("Time 2  L,R (%f, %f) and average %f \n", entry.time2L(), entry.time2R(), time2); 
	    printf("Energy  L,R (%f, %f) and average %f \n", entry.ampL(), entry.ampR(), energy); 
	    printf("Position and error (%f, %f) \n", position , position_error); 

        #endif
        


	// fill the rechit 
        output[i] = {entry.detId(),
		    entry.row() , //dummy
		    time1,
		    time2,
		    energy, 
		    position, 
		    time_error, 
		    position_error, // dummy
		    flag	
	};
      }
    }
  };

  void BTLRecHitSoAProducerAlgo::fromUncalibToReco(Queue& queue,
                                                   BTLUncalibRecHitSoA::ConstView const& input,
                                                   BTLRecHitSoA::View& output,
                                                   const double c_LYSO_,
						   const double thresholdToKeep_, 
						   const double calibration_) {
    // Use 64 items per group.
    // This value is arbitrary, but it's a reasonable starting point.
    uint32_t items = 64;

    // Use as many groups as needed to cover the whole problem.
    // If this value is too large, a smaller number of blocks can give better performance.
    uint32_t groups = cms::alpakatools::divide_up_by(input.metadata().size(), items);

    auto grid = cms::alpakatools::make_workdiv<Acc1D>(groups, items);
    alpaka::exec<Acc1D>(queue, grid, BTLUncalibToRecoKernel{}, input, output, c_LYSO_, thresholdToKeep_, calibration_);
  }

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit
