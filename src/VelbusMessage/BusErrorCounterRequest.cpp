#include "BusErrorCounterRequest.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class BusErrorCounterRequestRegisterer {
public:
	BusErrorCounterRequestRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 3;
		k.length   = 1;
		k.command  = 0xd9;
		struct factory_methods f;
		f.factory = &BusErrorCounterRequest::factory;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	BusErrorCounterRequestRegisterer BusErrorCounterRequest; /* Create a static instance: register with the registrar */
}

BusErrorCounterRequest::BusErrorCounterRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 1 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xd9 ) throw FormError("Wrong type");
}

std::string BusErrorCounterRequest::data() throw() {
	std::string ret("\xd9", 1);
	return ret;
}

std::string BusErrorCounterRequest::string() throw() {
	std::ostringstream o;
	o << "BusErrorCounterRequest to 0x" << hex(m_addr);
	return o.str();
}

} // namespace
