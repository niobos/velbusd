#include "RelayStatus.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class RelayStatusRegisterer {
public:
	RelayStatusRegisterer() {
		struct factory_methods f;
		f.factory = &RelayStatus::factory;
		Registrar::get_instance().add(0xfb, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	RelayStatusRegisterer RelayStatus; /* Create a static instance: register with the registrar */
}

RelayStatus::RelayStatus( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 8 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xfb ) throw FormError("Wrong type");

	m_relay_num = data[1];
	m_channel_mode = data[2];
	m_relay_status = data[3];
	m_led_status = data[4];
	m_timer = (data[5]<<16) | (data[6]<<8) | data[7];
}

std::string RelayStatus::data() throw() {
	std::string ret("\xfb", 1);
	ret.append(1, m_relay_num);
	ret.append(1, m_channel_mode);
	ret.append(1, m_relay_status);
	ret.append(1, m_led_status);
	ret.append(1, m_timer >> 16); ret.append(1, m_timer >> 8); ret.append(1, m_timer);
	return ret;
}

std::string RelayStatus::string() throw() {
	std::ostringstream o;
	o << "RelayStatus of 0x" << hex(m_addr) << " / ";

	switch(m_relay_num) {
	case 0x01:
	case 0x02:
	case 0x04:
	case 0x08:
	case 0x10:
		o << bitnum(m_relay_num)+1;
		break;
	default:
		o << "Unknown[" << bin(m_relay_num) << "]";
	}

	o << " : relay=";
	switch(m_relay_status) {
	case 0: o << "off"; break;
	case 1: o << "on"; break;
	case 3: o << "interval"; break;
	default: o << "unknown[0x" << hex(m_relay_status) << "]"; break;
	}

	o << " status=";

	switch(m_channel_mode) {
	case 0: o << "normal"; break;
	case 1: o << "inhibited"; break;
	case 2: o << "forced on"; break;
	case 3: o << "disabled"; break;
	default: o << "unknown[0x" << hex(m_channel_mode) << "]"; break;
	}

	o << " LED=";

	switch(m_led_status) {
	case 0x00: o << "off"; break;
	case 0x80: o << "on"; break;
	case 0x40: o << "SlowBlink"; break;
	case 0x20: o << "FastBlink"; break;
	case 0x10: o << "VFastBlink"; break;
	default: o << "unknown[0x" << hex(m_led_status) << "]"; break;
	}

	o << " timer=" << m_timer;

	return o.str();
}

} // namespace
