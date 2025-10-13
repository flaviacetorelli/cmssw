#include <ostream>

#include <fmt/format.h>

#include "DataFormats/FTLRecHitSoA/interface/BTLUncalibRecHitSoA.h"

namespace btlrechit {

  std::ostream& operator<<(std::ostream& out, BTLUncalibRecHitSoA::View::const_element const& btlrh) {
    out << "BTL uncalib rechit SoA: "
	//<< " detID: "  << btlrh.detId()
        << ", row: " << btlrh.row()
        << ", time1 L: " << btlrh.time1L()
        << ", time1 R: " << btlrh.time1R()
        << ", time2 L: " << btlrh.time2L() 
        << ", time2 R: " << btlrh.time2R() 
        << ", amplitude L: " <<  btlrh.ampL() 
        << ", amplitude R: " <<  btlrh.ampR() 
	<< ", flags L: " << btlrh.flagsL()
	<< ", flags R: " << btlrh.flagsR()
	<< ", idleTime L: " << btlrh.idleTimeL()
	<< ", idleTime R: " << btlrh.idleTimeR();


    return out;
  }

}  // namespace btlrechit
