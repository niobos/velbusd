#include "RxReady.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class RxReadyRegisterer {
public:
	RxReadyRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 0;
		k.length   = 1;
		k.command  = 0x0C;
		struct factory_methods f;
		f.factory = &RxReady::factory;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	RxReadyRegisterer RxReady; /* Create a static instance: register with the registrar */
}

RxReady::RxReady( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 0 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 1 ) throw FormError("Incorrect length");
	if( data[0] != (char)0x0C ) throw FormError("Wrong type");

	if( addr != 0x00 ) throw FormError("VMB1RS command not from address 0x00");
}

std::string RxReady::data() throw() {
	std::string ret("\x0C", 1);
	return ret;
}

std::string RxReady::string() throw() {
	std::ostringstream o;
	o << "RxReady from 0x" << hex(m_addr);
	return o.str();
}

} // namespace
