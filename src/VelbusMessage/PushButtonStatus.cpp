#include "PushButtonStatus.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class PushButtonStatusRegisterer {
public:
	PushButtonStatusRegisterer() {
		struct factory_methods f;
		f.factory = &PushButtonStatus::factory;
		Registrar::get_instance().add(0x00, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	PushButtonStatusRegisterer PushButtonStatus; /* Create a static instance: register with the registrar */
}

PushButtonStatus::PushButtonStatus( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 0 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 4 ) throw FormError("Incorrect length");
	if( data[0] != 0x00 ) throw FormError("Wrong type");
	m_just_pressed = data[1];
	m_just_released = data[2];
	m_long_pressed = data[3];
}

std::string PushButtonStatus::data() throw() {
	std::string ret("\x00", 1);
	ret.append(1, m_just_pressed);
	ret.append(1, m_just_released);
	ret.append(1, m_long_pressed);
	return ret;
}

std::string PushButtonStatus::string() throw() {
	std::ostringstream o;
	o << "PushButtonStatus from 0x" << hex(m_addr) << ":"
	  << " on=" << bin(m_just_pressed)
	  << " long=" << bin(m_long_pressed)
	  << " off=" << bin(m_just_released)
	  ;
	return o.str();
}

} // namespace
