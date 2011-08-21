#ifndef __VELBUSMESSAGE_RELAYSTATUS_HPP__
#define __VELBUSMESSAGE_RELAYSTATUS_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class RelayStatus : public VelbusMessage {
public:
	unsigned char m_relay_num;
	unsigned char m_channel_mode;
	unsigned char m_relay_status;
	unsigned char m_led_status;
	unsigned long m_timer;

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
