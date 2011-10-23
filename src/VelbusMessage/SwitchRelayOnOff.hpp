#ifndef __VELBUSMESSAGE_SwitchRelayOnOff_HPP__
#define __VELBUSMESSAGE_SwitchRelayOnOff_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class SwitchRelayOnOff : public VelbusMessage {
public:
	unsigned char m_direction;
	unsigned char m_relay_channel;

protected:
	SwitchRelayOnOff( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new SwitchRelayOnOff(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_SwitchRelayOnOff_HPP__
