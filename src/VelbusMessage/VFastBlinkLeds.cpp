#include "VFastBlinkLeds.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class VFastBlinkLedsRegisterer {
public:
	VFastBlinkLedsRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 3;
		k.length   = 2;
		k.command  = 0xf9;
		struct factory_methods f;
		f.factory = &VFastBlinkLeds::factory;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	VFastBlinkLedsRegisterer VFastBlinkLeds; /* Create a static instance: register with the registrar */
}

VFastBlinkLeds::VFastBlinkLeds( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 2 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xf9 ) throw FormError("Wrong type");
	m_led_bitmap = data[1];
}

std::string VFastBlinkLeds::data() throw() {
	std::string ret("\xf9", 1);
	ret.append(1, m_led_bitmap);
	return ret;
}

std::string VFastBlinkLeds::string() throw() {
	std::ostringstream o;
	o << "VFastBlinkLeds to 0x" << hex(m_addr) << ":"
	  << " " << bin(m_led_bitmap)
	  ;
	return o.str();
}

} // namespace
