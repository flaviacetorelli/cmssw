#ifndef DataFormats_FTLRecHitSoA_interface_BTLUncalibRecHitSoA_h
#define DataFormats_FTLRecHitSoA_interface_BTLUncalibRecHitSoA_h

#include <ostream>

#include "DataFormats/SoATemplate/interface/SoALayout.h"
#include "DataFormats/DetId/interface/DetId.h"

namespace btlrechit {
    GENERATE_SOA_LAYOUT(BTLUncalibRecHitSoALayout,
                        SOA_COLUMN(DetId, detId), 
                        SOA_COLUMN(uint8_t, row), 
                        SOA_COLUMN(float, time1L),   
                        SOA_COLUMN(float, time1R),   
                        SOA_COLUMN(float, time2L),   
                        SOA_COLUMN(float, time2R),   
                        SOA_COLUMN(float, ampL), 
                        SOA_COLUMN(float, ampR), 
                        SOA_COLUMN(uint8_t, flagsL),   
                        SOA_COLUMN(uint8_t, flagsR),   
                        SOA_COLUMN(uint16_t, idleTimeL),   
                        SOA_COLUMN(uint16_t, idleTimeR))

    using BTLUncalibRecHitSoA = BTLUncalibRecHitSoALayout<>;
    using BTLUncalibRecHitSoAView = BTLUncalibRecHitSoA::View;
    using BTLUncalibRecHitSoAConstView = BTLUncalibRecHitSoA::ConstView;


    std::ostream& operator<<(std::ostream& out, BTLUncalibRecHitSoA::View::const_element const& btlrh);
} // namespace btlrechit
#endif  // DataFormats_FTLRecHitSoA_interface_BTLUncalibRecHitSoA_h
