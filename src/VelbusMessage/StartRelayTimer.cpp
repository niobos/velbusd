#include "StartRelayTimer.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class StartRelayTimerRegisterer {
public:
	StartRelayTimerRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 0;
		k.length   = 5;
		k.command  = 0x03;
		struct factory_methods f;
		f.factory = &StartRelayTimer::factory;
		Registrar::get_instance().add(k, f);
		k.command  = 0x0d;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	StartRelayTimerRegisterer StartRelayTimer; /* Create a static instance: register with the registrar */
}

StartRelayTimer::StartRelayTimer( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 0 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 5 ) throw FormError("Incorrect length");
	if( data[0] != (char)0x03 &&
	    data[0] != (char)0x0d
	  ) throw FormError("Wrong type");

	m_command = data[0];
	m_relay_channel = data[1];
	m_timeout = ((unsigned char)data[2] << 16) | ((unsigned char)data[3] << 8) | (unsigned char)data[4];
}

std::string StartRelayTimer::data() throw() {
	std::string ret;
	ret.append(1, m_command);
	ret.append(1, m_relay_channel);
	ret.append(1, (m_timeout >> 16) & 0xff);
	ret.append(1, (m_timeout >> 8) & 0xff);
	ret.append(1, (m_timeout) & 0xff);
	return ret;
}

std::string StartRelayTimer::string() throw() {
	std::ostringstream o;
	switch(m_command) {
	case 0x03: o << "StartRelayTimer"; break;
	case 0x0d: o << "StartRelayIntervalTimer"; break;
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

	o << " Timeout=";
	switch(m_timeout) {
	case 0: o << "default"; break;
	case 0xffffff: o << "permanent"; break;
	default: o << m_timeout << "s"; break;
	}

	return o.str();
}

} // namespace
