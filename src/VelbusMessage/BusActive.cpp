#include "BusActive.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class BusActiveRegisterer {
public:
	BusActiveRegisterer() {
		struct factory_methods f;
		f.factory = &BusActive::factory;
		Registrar::get_instance().add(0x0A, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	BusActiveRegisterer BusActive; /* Create a static instance: register with the registrar */
}

BusActive::BusActive( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 0 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 1 ) throw FormError("Incorrect length");
	if( data[0] != (char)0x0A ) throw FormError("Wrong type");

	if( addr != 0x00 ) throw FormError("VMB1RS command not from address 0x00");
}

std::string BusActive::data() throw() {
	std::string ret("\x0A", 1);
	return ret;
}

std::string BusActive::string() throw() {
	std::ostringstream o;
	o << "BusActive from 0x" << hex(m_addr);
	return o.str();
}

} // namespace
