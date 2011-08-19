#include "SlowBlinkLeds.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class SlowBlinkLedsRegisterer {
public:
	SlowBlinkLedsRegisterer() {
		struct factory_methods f;
		f.factory = &SlowBlinkLeds::factory;
		Registrar::get_instance().add(0xf7, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	SlowBlinkLedsRegisterer SlowBlinkLeds; /* Create a static instance: register with the registrar */
}

SlowBlinkLeds::SlowBlinkLeds( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 2 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xf7 ) throw FormError("Wrong type");
	m_led_bitmap = data[1];
}

std::string SlowBlinkLeds::data() throw() {
	std::string ret("\xf7", 1);
	ret.append(1, m_led_bitmap);
	return ret;
}

std::string SlowBlinkLeds::string() throw() {
	std::ostringstream o;
	o << "SlowBlinkLeds to 0x" << hex(m_addr) << ":"
	  << " " << bin(m_led_bitmap)
	  ;
	return o.str();
}

} // namespace
