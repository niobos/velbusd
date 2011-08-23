#ifndef __VELBUSMESSAGE_BlindStatus_HPP__
#define __VELBUSMESSAGE_BlindStatus_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class BlindStatus : public VelbusMessage {
public:
	unsigned char m_blind_channel;
	unsigned char m_timeout;
	unsigned char m_blind_status;
	unsigned char m_led_status;
	unsigned long m_delay;

protected:
	BlindStatus( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new BlindStatus(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_BlindStatus_HPP__
