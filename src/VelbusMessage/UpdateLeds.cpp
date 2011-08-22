#include "UpdateLeds.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class UpdateLedsRegisterer {
public:
	UpdateLedsRegisterer() {
		struct factory_methods f;
		f.factory = &UpdateLeds::factory;
		Registrar::get_instance().add(0xf4, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	UpdateLedsRegisterer UpdateLeds; /* Create a static instance: register with the registrar */
}

UpdateLeds::UpdateLeds( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 4 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xf4 ) throw FormError("Wrong type");
	m_led_on = data[1];
	m_led_slow_blink = data[2];
	m_led_fast_blink = data[3];
}

std::string UpdateLeds::data() throw() {
	std::string ret("\xf4", 1);
	ret.append(1, m_led_on);
	ret.append(1, m_led_slow_blink);
	ret.append(1, m_led_fast_blink);
	return ret;
}

std::string UpdateLeds::string() throw() {
	unsigned char on = m_led_on;
	unsigned char sb = m_led_slow_blink;
	unsigned char fb = m_led_fast_blink;
	unsigned char vfb = 0;

	// "The continuous on bit overrides the blinking modes"
	sb &= ~on;
	fb &= ~on;
	/* "If the slow and fast blinking bits for a LED are both on, the LED
	 *  blinks very fast." */
	vfb = sb & fb;
	sb &= ~vfb;
	fb &= ~vfb;

	std::ostringstream o;
	o << "UpdateLeds to 0x" << hex(m_addr) << ":"
	  << " on=" << bin(on)
	  << " slowblink=" << bin(sb)
	  << " fastblink=" << bin(fb)
	  << " veryfastblink=" << bin(vfb)
	  ;
	return o.str();
}

} // namespace
