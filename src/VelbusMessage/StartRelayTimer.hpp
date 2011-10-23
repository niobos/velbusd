#ifndef __VELBUSMESSAGE_StartRelayTimer_HPP__
#define __VELBUSMESSAGE_StartRelayTimer_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class StartRelayTimer : public VelbusMessage {
public:
	unsigned char m_command;
	unsigned char m_relay_channel;
	unsigned long m_timeout;

protected:
	StartRelayTimer( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new StartRelayTimer(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_StartRelayTimer_HPP__
