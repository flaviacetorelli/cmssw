#ifndef DataFormats_FTLRecHitSoA_interface_BTLRechHitHostCollection_h
#define DataFormats_FTLRecHitSoA_interface_BTLRechHitHostCollection_h

#include "DataFormats/FTLRecHitSoA/interface/BTLRechHitSoA.h"
#include "DataFormats/Portable/interface/PortableHostCollection.h"

namespace btlrechit {
  
  using BTLRecHitHostCollection = PortableHostCollection<BTLRechHitSoA>;

} // namespace btluncalibrechit

#endif  // DataFormats_FTLRecHitSoA_interface_BTLRechHitHostCollection_h
