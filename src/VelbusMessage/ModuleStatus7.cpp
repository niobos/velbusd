#include "ModuleStatus7.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class ModuleStatus7Registerer {
public:
	ModuleStatus7Registerer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 3;
		k.length   = 7;
		k.command  = 0xed;
		struct factory_methods f;
		f.factory = &ModuleStatus7::factory;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	ModuleStatus7Registerer ModuleStatus7; /* Create a static instance: register with the registrar */
}

ModuleStatus7::ModuleStatus7( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data )
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 7 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xed ) throw FormError("Wrong type");

	m_input_status = data[1];
	m_enabled_status = data[2];
	m_not_inverted_status = data[3];
	m_locked_status = data[4];
	m_program_disabled = data[5];

	m_program       = data[6] & 0x03;
	m_alarm1        = data[6] & 0x04;
	m_alarm1_global = data[6] & 0x80;
	m_alarm2        = data[6] & 0x10;
	m_alarm2_global = data[6] & 0x20;
	m_sunrise       = data[6] & 0x40;
	m_sunset        = data[6] & 0x80;
}

std::string ModuleStatus7::data() throw() {
	std::string ret("\xed", 1);
	ret.append(1, m_input_status);
	ret.append(1, m_enabled_status);
	ret.append(1, m_not_inverted_status);
	ret.append(1, m_locked_status);
	ret.append(1, m_program_disabled);

	unsigned char ap = 0x00;
	ap |= m_program & 0x03;
	ap |= m_alarm1        ? 0x04 : 0x00;
	ap |= m_alarm1_global ? 0x08 : 0x00;
	ap |= m_alarm2        ? 0x10 : 0x00;
	ap |= m_alarm2_global ? 0x20 : 0x00;
	ap |= m_sunrise       ? 0x40 : 0x00;
	ap |= m_sunset        ? 0x80 : 0x00;
	ret.append(1, ap);
	return ret;
}

std::string ModuleStatus7::string() throw() {
	std::ostringstream o;
	o << "ModuleStatus from 0x" << hex(m_addr) << ":"
	  << " inputs=0b" << bin(m_input_status)
	  << " enabled=0b" << bin(m_enabled_status)
	  << " noninverted=0b" << bin(m_not_inverted_status)
	  << " locked=0b" << bin(m_locked_status)
	  << " progdisabled=0b" << bin(m_program_disabled)
	  << " ";

	switch(m_program) {
	case 0: o << "program=None"; break;
	case 1: o << "program=Summer"; break;
	case 2: o << "program=Winter"; break;
	case 3: o << "program=Holiday"; break;
	// no default, we listed all;
	}

	o << " Alarms: 1(" << (m_alarm1 ? "on" : "off")
	  << " " << (m_alarm1_global ? "global" : "local")
	  << ") 2(" << (m_alarm2 ? "on" : "off")
	  << " " << (m_alarm2_global ? "global" : "local")
	  << ") ";

	o << " Sunrise " << ( m_sunrise ? "enabled" : "disabled" );
	o << " Sunset " << ( m_sunset ? "enabled" : "disabled" );

	return o.str();
}

} // namespace
