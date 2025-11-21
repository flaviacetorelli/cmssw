#include <alpaka/alpaka.hpp>

#include "DataFormats/FTLRecHitSoA/interface/alpaka/BTLUncalibRecHitDeviceCollection.h"
#include "DataFormats/FTLDigiSoA/interface/alpaka/BTLDigiDeviceCollection.h"

#include "HeterogeneousCore/AlpakaInterface/interface/config.h"
#include "HeterogeneousCore/AlpakaInterface/interface/workdivision.h"

#include "BTLUncalibRecHitSoAProducerAlgo.h"



namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit {

  using namespace ::btlrechit;

  ALPAKA_FN_ACC float	      
  TcoarseTfineToTime(uint32_t rawId, uint8_t chID, uint8_t TACID, uint16_t tcoarse, uint16_t tfine,  bool isT1) { 

    // tdc calibration parameters 
    // (to be modified: these parameters are evaluated by channel and stored in parquet files)
    static constexpr float tclock = 6.25; 
    static constexpr float a0 = 57.244545; 
    static constexpr float a1 = 511.27832; 
    static constexpr float a2 = -7.8838577;
    static constexpr float t0 = -0.048343264; 
 
    float const qT = (-a1 + sqrt(a1 * a1 - 4.0 * (a0 - float(tfine)) * a2)) / (2.0 * a2); 
    float const time  = (tcoarse - qT - t0)*tclock;
    return time; 
  }

  ALPAKA_FN_ACC float	      
  QfineToADC(uint32_t rawId, uint8_t chID, uint8_t TACID, uint16_t qfine, float time1, uint16_t timeEndQ) { 
 
    // qdc calibration parameters 
    // (to be modified: these parameters are evaluated by channel and stored in parquet files)
    static constexpr float p0 = 49.542229; 
    static constexpr float p1 = -0.323424;
    static constexpr float p2 = 0.062578;
    static constexpr float p3 = -0.002484;
    static constexpr float p4 = 0.0;
    static constexpr float p5 = 0.0;
    static constexpr float p6 = 0.0;
    static constexpr float p7 = 0.0;
    static constexpr float p8 = 0.0;
    static constexpr float p9 = 0.0;
    float const ti = float(timeEndQ) - time1; 

    int const pedestal = ( // check the type
        p0
        + p1 * ti
        + p2 * ti * ti
        + p3 * ti * ti * ti
        + p4 * ti * ti * ti * ti
        + p5 * ti * ti * ti * ti * ti
        + p6 * ti * ti * ti * ti * ti * ti
        + p7 * ti * ti * ti * ti * ti * ti * ti
        + p8 * ti * ti * ti * ti * ti * ti * ti * ti
        + p9 * ti * ti * ti * ti * ti * ti * ti * ti * ti
    );


    float energy = float(qfine) - float(pedestal);

    return energy; 


  }

  ALPAKA_FN_ACC uint8_t	      
  createFlag(uint32_t flag) { // function to condensate info from raw in a quality flag
    flag = 1;                   
    return flag; 
  }



  class BTLdigiToUncalibKernel {
  public:

  
    ALPAKA_FN_ACC void operator()(Acc1D const& acc, 
                                  btldigi::BTLDigiSoA::ConstView input, 
                                  BTLUncalibRecHitSoA::View output,
                                  const double adcLSB_,
				  const double timeCorr_p0_,
				  const double timeCorr_p1_,
				  const double timeCorr_p2_
				  ) const { // when condformat for calib ready, add also tdc and qdc in inputs
      // make a strided loop over the kernel grid, covering up to "size" elements
      for (int32_t i : cms::alpakatools::uniform_elements(acc, input.metadata().size())) { 
        auto entry = input[i];
        // here you should call your functions to apply TDC and QDC, reading timecoarse, fine, ... etc from digi
 
	// for the times at first and second th
	// atm tdc and qdc calibs are fixed to dummy values for each channel, hence rawId, ch, and the bool to select branch 1 or 2 are not used.
      	auto time1R = TcoarseTfineToTime(entry.rawId(), entry.chIDR(), entry.TACIDR(),  entry.T1coarseR(),  entry.T1fineR(), true); 
      	auto time1L = TcoarseTfineToTime(entry.rawId(), entry.chIDL(), entry.TACIDL(),  entry.T1coarseL(),  entry.T1fineL(),  true); 

	auto time2R = TcoarseTfineToTime(entry.rawId(), entry.chIDR(), entry.TACIDR(), entry.T2coarseR(), entry.T2fineR(),  false); 
      	auto time2L = TcoarseTfineToTime(entry.rawId(), entry.chIDL(), entry.TACIDL(), entry.T2coarseL(), entry.T2fineL(),  false); 

	// from qfine to energy in adc, NB you need to pass calibrated time
	auto ampL = QfineToADC(entry.rawId(), entry.chIDL(), entry.TACIDL(), entry.ChargeL(), time1L, time2L); //atm EOIcoarseL is 0 so put time2 instead 
	auto ampR = QfineToADC(entry.rawId(), entry.chIDR(), entry.TACIDR(), entry.ChargeR(), time1R, time2R); 

	// flags for the usability of the channel uint_8 
	//auto flagsL = createFlag(entry.PrevTrigFL(),... ); // to be implemented WIP
        //auto flagsR = createFlag(entry.PrevTrigFR(),... );
        uint8_t flagsL = 0; 
        uint8_t flagsR = 0;		

	// detId from rawId
        const DetId detId(entry.rawId());
        
        // converting the energy from ADC to energy 
	ampL = ampL *adcLSB_;
	ampR = ampR *adcLSB_;

        // amp walk corrections, converting the energy from ADC to energy 
        time1R = time1R - ( timeCorr_p0_ * pow(ampR, timeCorr_p1_) + timeCorr_p2_);
        time1L = time1L - ( timeCorr_p0_ * pow(ampL, timeCorr_p1_) + timeCorr_p2_);

	// fill the uncalib rechit
        output[i] = {detId, 
		    1, // just a placeholder, to be fixed
		    time1R, // in ns
		    time2R,
		    ampR, // energy
		    entry.IdleTimeR(),
		    flagsR, 
	            time1L, // in ns
		    time2L,
		    ampL, // energy
		    entry.IdleTimeL(), 
		    flagsL, 

	};
      }
    }
  };

  void BTLUncalibRecHitSoAProducerAlgo::fromDigiToUncalib(Queue& queue,
                                                   btldigi::BTLDigiSoA::ConstView const& input,
                                                   BTLUncalibRecHitSoA::View& output,
                                                   const double adcLSB_,
						   const double timeCorr_p0_,
                                                   const double timeCorr_p1_,
                                                   const double timeCorr_p2_) {
						   //,
                                                   //Table const& tdc,
                                                   //Table const& qdc) {
    // Use 64 items per group.
    // This value is arbitrary, but it's a reasonable starting point.
    uint32_t items = 64;

    // Use as many groups as needed to cover the whole problem.
    // If this value is too large, a smaller number of blocks can give better performance.
    uint32_t groups = cms::alpakatools::divide_up_by(input.metadata().size(), items);

    auto grid = cms::alpakatools::make_workdiv<Acc1D>(groups, items);
    alpaka::exec<Acc1D>(queue, grid, BTLdigiToUncalibKernel{}, input, output, adcLSB_, timeCorr_p0_, timeCorr_p1_, timeCorr_p2_);
  }

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit
