#include "SwitchRelayOnOff.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class SwitchRelayOnOffRegisterer {
public:
	SwitchRelayOnOffRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 0;
		k.length   = 2;
		k.command  = 0x01;
		struct factory_methods f;
		f.factory = &SwitchRelayOnOff::factory;
		Registrar::get_instance().add(k, f);
		k.command  = 0x02;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	SwitchRelayOnOffRegisterer SwitchRelayOnOff; /* Create a static instance: register with the registrar */
}

SwitchRelayOnOff::SwitchRelayOnOff( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 0 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 2 ) throw FormError("Incorrect length");
	if( data[0] != (char)0x01 &&
	    data[0] != (char)0x02
	  ) throw FormError("Wrong type");

	m_direction = data[0];
	m_relay_channel = data[1];
}

std::string SwitchRelayOnOff::data() throw() {
	std::string ret;
	ret.append(1, m_direction);
	ret.append(1, m_relay_channel);
	return ret;
}

std::string SwitchRelayOnOff::string() throw() {
	std::ostringstream o;
	o << "SwitchRelay";
	switch(m_direction) {
	case 0x01: o << "Off"; break;
	case 0x02: o << "On"; break;
	default: o << "___"; break;
	}
	o << " to 0x" << hex(m_addr) << ": ";

	o << "Relay=";
	switch(m_relay_channel) {
	case 0x01: o << "1"; break;
	case 0x02: o << "2"; break;
	case 0x04: o << "3"; break;
	case 0x08: o << "4"; break;
	case 0x10: o << "5"; break;
	default: o << "Unknown[0x" << hex(m_relay_channel) << "]"; break;
	}

	return o.str();
}

} // namespace
