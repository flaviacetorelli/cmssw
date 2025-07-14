#include <ostream>

#include <fmt/format.h>

#include "DataFormats/FTLRecHitSoA/interface/BTLUncalibRecHitSoA.h"

namespace btlrechit {

  std::ostream& operator<<(std::ostream& out, BTLUncalibRecHitSoA::View::const_element const& btlrh) {
    out << fmt::format("BTL uncalib rechit SoA: \n 
		       detID, row : {}, {} \n 
		       time1 L {}, R {} \n 
		       time2 L {}, R {} \n 
		       amplitude L  {}, R {} \n
		       flags L {}, R {} \n 
		       idleTime L {}, R {} \n", 
		       btlrh.detId(), btlrh.row(), 
		       btlrh.time1L(),  btlrh.time1R(),
		       btlrh.time2L(),  btlrh.time2R(),
		       btlrh.amp1L(),  btlrh.amp1R(),
		       btlrh.flagsL(),  btlrh.flagsR(),
		       btlrh.idleTimeL(),  btlrh.idleTimeR());
    return out;
  }

}  // namespace btlrechit
