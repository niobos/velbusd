#include "FastBlinkLeds.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class FastBlinkLedsRegisterer {
public:
	FastBlinkLedsRegisterer() {
		struct factory_methods f;
		f.factory = &FastBlinkLeds::factory;
		Registrar::get_instance().add(0xf8, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	FastBlinkLedsRegisterer FastBlinkLeds; /* Create a static instance: register with the registrar */
}

FastBlinkLeds::FastBlinkLeds( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 2 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xf8 ) throw FormError("Wrong type");
	m_led_bitmap = data[1];
}

std::string FastBlinkLeds::data() throw() {
	std::string ret("\xf8", 1);
	ret.append(1, m_led_bitmap);
	return ret;
}

std::string FastBlinkLeds::string() throw() {
	std::ostringstream o;
	o << "FastBlinkLeds to 0x" << hex(m_addr) << ":"
	  << " " << bin(m_led_bitmap)
	  ;
	return o.str();
}

} // namespace
