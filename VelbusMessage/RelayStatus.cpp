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

	m_relay_num = bitnum(data[1]);
	if( m_relay_num == -1 ) throw FormError("RelayStatus: No relay bit set");
	m_channel_mode = static_cast<enum channel_mode>(data[2]);
	m_relay_status = static_cast<enum relay_status>(data[3]);
	m_led_status = static_cast<enum led_status>(data[4]);
	m_timer = (data[5]<<16) | (data[6]<<8) | data[7];
}

std::string RelayStatus::data() throw() {
	std::string ret("\xfb", 1);
	ret.append(1, (1<<m_relay_num));
	ret.append(1, m_channel_mode);
	ret.append(1, m_relay_status);
	ret.append(1, m_led_status);
	ret.append(1, m_timer >> 16); ret.append(1, m_timer >> 8); ret.append(1, m_timer);
	return ret;
}

std::string RelayStatus::string() throw() {
	std::ostringstream o;
	o << "RelayStatus of 0x" << hex(m_addr) << " / " << static_cast<int>(m_relay_num) << " : "
	  << "relay=";

	switch(m_relay_status) {
	case r_off: o << "off"; break;
	case r_on: o << "on"; break;
	case r_interval: o << "interval"; break;
	}

	o << " status=";

	switch(m_channel_mode) {
	case c_normal: o << "normal"; break;
	case c_inhibited: o << "inhibited"; break;
	case c_forced_on: o << "forced on"; break;
	case c_disabled: o << "disabled"; break;
	}

	o << " LED=";

	switch(m_led_status) {
	case l_off: o << "off"; break;
	case l_on: o << "on"; break;
	case l_slowblink: o << "SlowBlink"; break;
	case l_fastblink: o << "FastBlink"; break;
	case l_vfastblink: o << "VFastBlink"; break;
	}

	o << " timer=" << m_timer;

	return o.str();
}

} // namespace
