#include "BlindStatus.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class BlindStatusRegisterer {
public:
	BlindStatusRegisterer() {
		struct factory_methods f;
		f.factory = &BlindStatus::factory;
		Registrar::get_instance().add(0xec, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	BlindStatusRegisterer BlindStatus; /* Create a static instance: register with the registrar */
}

BlindStatus::BlindStatus( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 8 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xec ) throw FormError("Wrong type");

	m_blind_channel = data[1];
	m_timeout = data[2];
	m_blind_status = data[3];
	m_led_status = data[4];
	m_delay = ( (data[5]&0xff) << 16 ) | ( (data[6]&0xff) << 8 ) | (data[7]&0xff);
}

std::string BlindStatus::data() throw() {
	std::string ret("\xec", 1);
	ret.append(1, m_blind_channel);
	ret.append(1, m_timeout);
	ret.append(1, m_blind_status);
	ret.append(1, m_led_status);
	ret.append(1, m_delay>>16 ); ret.append(1, m_delay>>8 ); ret.append(1, m_delay );
	return ret;
}

std::string BlindStatus::string() throw() {
	std::ostringstream o;
	o << "BlindStatus from 0x" << hex(m_addr) << ":";

	o << " Blind=";
	switch(m_blind_channel) {
	case 0x03: o << "1"; break;
	case 0x0c: o << "2"; break;
	default: o << "Unknown[0x" << hex(m_blind_channel) << "]"; break;
	}

	o << " Timeout=";
	switch(m_timeout) {
	case 0x00: o << "15s"; break;
	case 0x01: o << "30s"; break;
	case 0x02: o << "1m"; break;
	case 0x03: o << "2m"; break;
	default: o << "Unknown[0x" << hex(m_timeout) << "]"; break;
	}

	o << " Status=";
	switch(m_blind_status) {
	case 0x00: o << "off"; break;
	case 0x01: o << "1 up"; break;
	case 0x02: o << "1 down"; break;
	case 0x04: o << "2 up"; break;
	case 0x08: o << "1 down"; break;
	default: o << "Unknown[0x" << hex(m_blind_status) << "]"; break;
	}

	o << " Led=";
	switch(m_led_status) {
	case 0x00: o << "off"; break;
	case 0x80: o << "down on"; break;
	case 0x40: o << "down slow blink"; break;
	case 0x20: o << "down fast blink"; break;
	case 0x10: o << "down very fast blink"; break;
	case 0x08: o << "up on"; break;
	case 0x04: o << "up slow blink"; break;
	case 0x02: o << "up fast blink"; break;
	case 0x01: o << "up very fast blink"; break;
	default: o << "Unknown[0x" << hex(m_led_status) << "]"; break;
	}

	o << " Timer=" << m_delay;

	return o.str();
}

} // namespace
