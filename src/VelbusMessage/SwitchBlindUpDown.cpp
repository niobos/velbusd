#include "SwitchBlindUpDown.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class SwitchBlindUpDownRegisterer {
public:
	SwitchBlindUpDownRegisterer() {
		struct factory_methods f;
		f.factory = &SwitchBlindUpDown::factory;
		Registrar::get_instance().add(0x05, f);
		Registrar::get_instance().add(0x06, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	SwitchBlindUpDownRegisterer SwitchBlindUpDown; /* Create a static instance: register with the registrar */
}

SwitchBlindUpDown::SwitchBlindUpDown( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 0 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 5 ) throw FormError("Incorrect length");
	if( data[0] != (char)0x05 &&
	    data[0] != (char)0x06
	  ) throw FormError("Wrong type");

	m_direction = data[0];
	m_blind_channel = data[1];
	m_timeout = ((unsigned char)data[2] << 16) | ((unsigned char)data[3] << 8) | (unsigned char)data[4];
}

std::string SwitchBlindUpDown::data() throw() {
	std::string ret;
	ret.append(1, m_direction);
	ret.append(1, m_blind_channel);
	ret.append(1, (m_timeout >> 16) & 0xff);
	ret.append(1, (m_timeout >> 8) & 0xff);
	ret.append(1, (m_timeout) & 0xff);
	return ret;
}

std::string SwitchBlindUpDown::string() throw() {
	std::ostringstream o;
	o << "SwitchBlind";
	switch(m_direction) {
	case 0x05: o << "Up"; break;
	case 0x06: o << "Down"; break;
	default: o << "___"; break;
	}
	o << " to 0x" << hex(m_addr) << ": ";

	o << "Blind=";
	switch(m_blind_channel) {
	case 0x03: o << "1"; break;
	case 0x0c: o << "2"; break;
	default: o << "Unknown[0x" << hex(m_blind_channel) << "]"; break;
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
