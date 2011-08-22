#include "BusOff.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class BusOffRegisterer {
public:
	BusOffRegisterer() {
		struct factory_methods f;
		f.factory = &BusOff::factory;
		Registrar::get_instance().add(0x09, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	BusOffRegisterer BusOff; /* Create a static instance: register with the registrar */
}

BusOff::BusOff( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 0 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 1 ) throw FormError("Incorrect length");
	if( data[0] != (char)0x09 ) throw FormError("Wrong type");

	if( addr != 0x00 ) throw FormError("VMB1RS command not from address 0x00");
}

std::string BusOff::data() throw() {
	std::string ret("\x09", 1);
	return ret;
}

std::string BusOff::string() throw() {
	std::ostringstream o;
	o << "BusOff from 0x" << hex(m_addr);
	return o.str();
}

} // namespace
