#include <ostream>

#include <fmt/format.h>

#include "DataFormats/FTLRecHitSoA/interface/BTLRecHitSoA.h"

namespace btlrechit {

  std::ostream& operator<<(std::ostream& out, BTLRecHitSoA::View::const_element const& btlrh) {
    out << "BTL uncalib rechit SoA: "
	<< " detID: "  << btlrh.detId()
        << ", row: " << btlrh.row()
        << ", time1: " << btlrh.time1()
        << ", time1 error : " << btlrh.time1_error()
        << ", time2: " << btlrh.time2() 
        << ", energy: " <<  btlrh.energy() 
        << ", position:	" << btlrh.position()
        << ", position error:	" << btlrh.position_error()
	<< ", flags: " << btlrh.flags();
    return out;
  }

}  // namespace btlrechit
