#include "NameRequest.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class NameRequestRegisterer {
public:
	NameRequestRegisterer() {
		struct factory_methods f;
		f.factory = &NameRequest::factory;
		Registrar::get_instance().add(0xef, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	NameRequestRegisterer NameRequest; /* Create a static instance: register with the registrar */
}

NameRequest::NameRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 2 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xef ) throw FormError("Wrong type");
	m_what = data[1];
}

std::string NameRequest::data() throw() {
	std::string ret("\xef", 1);
	ret.append(1, m_what);
	return ret;
}

std::string NameRequest::string() throw() {
	std::ostringstream o;
	o << "NameRequest to 0x" << hex(m_addr) << ": ";

	switch(m_what) {
	case 0x01: o << "Relay/Pushbutton 1"; break;
	case 0x02: o << "Relay/Pushbutton 2"; break;
	case 0x04: o << "Relay/Pushbutton 3"; break;
	case 0x08: o << "Relay/Pushbutton 4"; break;
	case 0x10: o << "Relay/Pushbutton 5"; break;
	case 0x20: o << "Relay/Pushbutton 6"; break;
	case 0x40: o << "Relay/Pushbutton 7"; break;
	case 0x80: o << "Relay/Pushbutton 8"; break;
	case 0x03: o << "Blind 1"; break;
	case 0x0c: o << "Blind 2"; break;
	default: o << "Unknown[" << hex(m_what) << "]"; break;
	}

	return o.str();
}

} // namespace
