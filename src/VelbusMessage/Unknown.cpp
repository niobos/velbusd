#include <config.h>
#include "Unknown.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

std::string Unknown::string() throw() {
	std::ostringstream o;
	o << "Unknown message:"
	  << " Prio=" << static_cast<unsigned int>(m_prio)
	  << " Addr=0x" << std::hex << static_cast<int>(m_addr) << std::dec
	  << " RTR=" << static_cast<unsigned int>(m_rtr)
	  << " data[" << m_data.length() << "]=";
	for( unsigned int i = 0; i < m_data.length(); i++ ) {
		o << hex(m_data[i]) << " "; // Ugly hex-dump hack
	}
	return o.str();
}

} // namespace
