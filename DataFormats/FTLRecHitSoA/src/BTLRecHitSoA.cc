#include <ostream>

#include <fmt/format.h>

#include "DataFormats/FTLRecHitSoA/interface/BTLRecHitSoA.h"

namespace btlrechit {

  std::ostream& operator<<(std::ostream& out, BTLRecHitSoA::View::const_element const& btlrh) {
    out << fmt::format("BTL uncalib rechit SoA: \n 
		       detID, row : {}, {} \n 
		       time1 {} , time_error {} \n 
		       time2 {} \n 
		       energy {} \n
		       position {} , position_error {} \n
		       flags {} \n ", 
		       btlrh.detId(), btlrh.row(), 
		       btlrh.time1(), btlrh.time1_error(),
		       btlrh.time2(), 
		       btlrh.energy(),
		       btlrh.position(),  btlrh.position_error(),
		       btlrh.flags());
    return out;
  }



}  // namespace btlrechit
