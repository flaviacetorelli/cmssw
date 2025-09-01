#ifndef DataFormats_FTLRecHitSoA_interface_BTLRecHitHostCollection_h
#define DataFormats_FTLRecHitSoA_interface_BTLRecHitHostCollection_h

#include "DataFormats/FTLRecHitSoA/interface/BTLRecHitSoA.h"
#include "DataFormats/Portable/interface/PortableHostCollection.h"

namespace btlrechit {
  
  using BTLRecHitHostCollection = PortableHostCollection<BTLRecHitSoA>;

} // namespace btluncalibrechit

#endif  // DataFormats_FTLRecHitSoA_interface_BTLRecHitHostCollection_h
