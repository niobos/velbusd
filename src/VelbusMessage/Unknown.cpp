#include <config.h>
#include "Unknown.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

std::string Unknown::string() throw() {
	std::ostringstream o;
	o << "Unknown message:"
	  << " Prio=" << dec(m_prio)
	  << " Addr=0x" << hex(m_addr) << std::dec
	  << " RTR=" << dec(m_rtr)
	  << " data[" << m_data.length() << "]=";
	for( unsigned int i = 0; i < m_data.length(); i++ ) {
		o << hex(m_data[i]) << " "; // Ugly hex-dump hack
	}
	return o.str();
}

} // namespace
