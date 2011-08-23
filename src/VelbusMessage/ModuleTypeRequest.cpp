#include "ModuleTypeRequest.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

ModuleTypeRequest::ModuleTypeRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data )
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 1 ) throw FormError("Wrong RTR");
	if( data.length() != 0 ) throw FormError("Incorrect length");
}

std::string ModuleTypeRequest::data() throw() {
	std::string ret("");
	return ret;
}

std::string ModuleTypeRequest::string() throw() {
	std::ostringstream o;
	o << "ModuleTypeRequest to 0x" << hex(m_addr);
	return o.str();
}

} // namespace
