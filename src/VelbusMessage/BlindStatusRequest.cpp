#include "BlindStatusRequest.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class BlindStatusRequestRegisterer {
public:
	BlindStatusRequestRegisterer() {
		struct factory_methods f;
		f.factory = &BlindStatusRequest::factory;
		Registrar::get_instance().add(0xfa, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	BlindStatusRequestRegisterer BlindStatusRequest; /* Create a static instance: register with the registrar */
}

BlindStatusRequest::BlindStatusRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 2 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xfa ) throw FormError("Wrong type");

	m_blind_channel = data[1];
}

std::string BlindStatusRequest::data() throw() {
	std::string ret("\xfa", 1);
	ret.append(1, m_blind_channel);
	return ret;
}

std::string BlindStatusRequest::string() throw() {
	std::ostringstream o;
	o << "BlindStatusRequest to 0x" << hex(m_addr) << ": ";

	o << "Blind=";
	switch(m_blind_channel) {
	case 0x03: o << "1"; break;
	case 0x0c: o << "2"; break;
	default: o << "Unknown[0x" << hex(m_blind_channel) << "]"; break;
	}

	return o.str();
}

} // namespace
