#ifndef __VELBUSMESSAGE_RELAYSTATUS_HPP__
#define __VELBUSMESSAGE_RELAYSTATUS_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class RelayStatus : public VelbusMessage {
public:
	char m_relay_num;
	enum channel_mode { c_normal=0, c_inhibited=1, c_forced_on=2, c_disabled=3 } m_channel_mode;
	enum relay_status { r_off=0, r_on=1, r_interval=3 } m_relay_status;
	enum led_status { l_off=0, l_on=0x80, l_slowblink=0x40, l_fastblink=0x20, l_vfastblink=0x10 } m_led_status;
	long m_timer;

protected:
	RelayStatus( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new RelayStatus(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_RELAYSTATUS_HPP__
