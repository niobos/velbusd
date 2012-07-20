#include "SwitchBlindOff.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class SwitchBlindOffRegisterer {
public:
	SwitchBlindOffRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 0;
		k.length   = 2;
		k.command  = 0x04;
		struct factory_methods f;
		f.factory = &SwitchBlindOff::factory;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	SwitchBlindOffRegisterer SwitchBlindOff; /* Create a static instance: register with the registrar */
}

SwitchBlindOff::SwitchBlindOff( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 0 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 2 ) throw FormError("Incorrect length");
	if( data[0] != (char)0x04 ) throw FormError("Wrong type");
	m_blind_channel = data[1];
}

std::string SwitchBlindOff::data() throw() {
	std::string ret("\x04", 1);
	ret.append(1, m_blind_channel);
	return ret;
}

std::string SwitchBlindOff::string() throw() {
	std::ostringstream o;
	o << "SwitchBlindOff to 0x" << hex(m_addr) << ": ";

	o << "Blind=";
	switch(m_blind_channel) {
	case 0x03: o << "1"; break;
	case 0x0c: o << "2"; break;
	default: o << "Unknown[0x" << hex(m_blind_channel) << "]"; break;
	}

	return o.str();
}

} // namespace
