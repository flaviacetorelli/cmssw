#ifndef DataFormats_FTLRecHitSoA_interface_BTLUncalibRechHitHostCollection_h
#define DataFormats_FTLRecHitSoA_interface_BTLUncalibRechHitHostCollection_h

#include "DataFormats/FTLRecHitSoA/interface/BTLUncalibRechHitSoA.h"
#include "DataFormats/Portable/interface/PortableHostCollection.h"

namespace btlrechit {
  
  using BTLUncalibRecHitHostCollection = PortableHostCollection<BTLUncalibRechHitSoA>;

} // namespace btluncalibrechit

#endif  // DataFormats_FTLRecHitSoA_interface_BTLUncalibRechHitHostCollection_h
