#include "ModuleStatusRequest.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class ModuleStatusRequestRegisterer {
public:
	ModuleStatusRequestRegisterer() {
		struct factory_methods f;
		f.factory = &ModuleStatusRequest::factory;
		Registrar::get_instance().add(0xfa, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	ModuleStatusRequestRegisterer ModuleStatusRequest; /* Create a static instance: register with the registrar */
}

ModuleStatusRequest::ModuleStatusRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 2 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xfa ) throw FormError("Wrong type");

	m_blind_channel = data[1];
}

std::string ModuleStatusRequest::data() throw() {
	std::string ret("\xfa", 1);
	ret.append(1, m_blind_channel);
	return ret;
}

std::string ModuleStatusRequest::string() throw() {
	std::ostringstream o;
	o << "ModuleStatusRequest to 0x" << hex(m_addr) << ": ";

	switch(m_blind_channel) {
	case 0x03: o << "Blind=1"; break;
	case 0x0c: o << "Blind=2"; break;
	case 0x01: o << "Relay=1"; break;
	case 0x02: o << "Relay=2"; break;
	case 0x04: o << "Relay=3"; break;
	case 0x08: o << "Relay=4"; break;
	case 0x10: o << "Relay=5"; break;
	default: o << "Unknown[0x" << hex(m_blind_channel) << "]"; break;
	}

	return o.str();
}

} // namespace
