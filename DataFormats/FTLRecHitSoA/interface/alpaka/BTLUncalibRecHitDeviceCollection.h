#ifndef DataFormats_FTLRecHitSoA_interface_alpaka_BTLUncalibRecHitDeviceCollection_h
#define DataFormats_FTLRecHitSoA_interface_alpaka_BTLUncalibRecHitDeviceCollection_h

#include "DataFormats/FTLRecHitSoA/interface/BTLUncalibRecHitHostCollection.h"
#include "DataFormats/FTLRecHitSoA/interface/BTLUncalibRecHitSoA.h"
#include "DataFormats/Portable/interface/alpaka/PortableCollection.h"
#include "HeterogeneousCore/AlpakaInterface/interface/config.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit {

  // Make the names from the top-level btlrechit namespace visible for unqualified lookup
  // inside the ALPAKA_ACCELERATOR_NAMESPACE::btlrechit namespace.
  using namespace ::btlrechit;

  using BTLUncalibRecHitDeviceCollection = PortableCollection<BTLUncalibRecHitSoA>;

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::btlrechit

// Check that the portable device collection for the host device is the same as the portable host collection.
ASSERT_DEVICE_MATCHES_HOST_COLLECTION(btlrechit::BTLUncalibRecHitDeviceCollection, btlrechit::BTLUncalibRecHitHostCollection);

#endif  // DataFormats_FTLRecHitSoA_interface_alpaka_BTLUncalibRecHitDeviceCollection_h
