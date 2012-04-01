#include "SwitchToMode.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class SwitchToModeRegisterer {
public:
	SwitchToModeRegisterer() {
		struct factory_methods f;
		f.factory = &SwitchToMode::factory;
		Registrar::get_instance().add(0xdb, f);
		Registrar::get_instance().add(0xdc, f);
		Registrar::get_instance().add(0xdd, f);
		Registrar::get_instance().add(0xde, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	SwitchToModeRegisterer SwitchToMode; /* Create a static instance: register with the registrar */
}

SwitchToMode::SwitchToMode( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 3 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xdb &&
	    data[0] != (char)0xdc &&
	    data[0] != (char)0xdd &&
	    data[0] != (char)0xde    ) throw FormError("Wrong type");

	m_mode = data[0];
	m_timer = ((unsigned char)data[1] << 8) | (unsigned char)data[2];
}

std::string SwitchToMode::data() throw() {
	std::string ret(1, m_mode);
	ret.append(1, m_timer >> 8);
	ret.append(1, m_timer);
	return ret;
}

std::string SwitchToMode::string() throw() {
	std::ostringstream o;
	o << "SwitchToMode to 0x" << hex(m_addr) << ": ";

	o << "switch to ";
	switch(m_mode) {
	case 0xdb: o << "comfort"; break;
	case 0xdc: o << "day"; break;
	case 0xdd: o << "night"; break;
	case 0xde: o << "safe"; break;
	default: o << "UNKNOWN"; break;
	}

	o << "; Timer: ";
	switch(m_timer) {
	case 0x0000: o << "Cancel"; break;
	case 0xff00: o << "Program step"; break;
	case 0xffff: o << "Manual"; break;
	default: o << m_timer << "sec"; break;
	}

	return o.str();
}

} // namespace
