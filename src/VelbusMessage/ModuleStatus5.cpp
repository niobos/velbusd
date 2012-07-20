#include "ModuleStatus5.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class ModuleStatus5Registerer {
public:
	ModuleStatus5Registerer() {
		struct factory_methods f;
		f.factory = &ModuleStatus5::factory;
		Registrar::get_instance().add(0xed, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	ModuleStatus5Registerer ModuleStatus5; /* Create a static instance: register with the registrar */
}

ModuleStatus5::ModuleStatus5( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data )
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 5 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xed ) throw FormError("Wrong type");

	m_input_status = data[1];
	m_leds_continuous = data[2];
	m_leds_slow_blink = data[3];
	m_leds_fast_blink = data[4];
}

std::string ModuleStatus5::data() throw() {
	std::string ret("\xed", 1);
	ret.append(1, m_input_status);
	ret.append(1, m_leds_continuous);
	ret.append(1, m_leds_slow_blink);
	ret.append(1, m_leds_fast_blink);
	return ret;
}

std::string ModuleStatus5::string() throw() {
	std::ostringstream o;
	o << "ModuleStatus5 from 0x" << hex(m_addr) << ":"
	  << " Inputs=0b" << bin(m_input_status)
	  << " LEDs: cont=0b" << bin(m_leds_continuous)
	  << " slow=0b" << bin(m_leds_slow_blink)
	  << " fast=0b" << bin(m_leds_fast_blink);
	return o.str();
}

} // namespace
