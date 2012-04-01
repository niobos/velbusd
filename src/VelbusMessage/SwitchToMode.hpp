#ifndef __VELBUSMESSAGE_SwitchToMode_HPP__
#define __VELBUSMESSAGE_SwitchToMode_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class SwitchToMode : public VelbusMessage {
public:
	char m_mode;
	unsigned int m_timer;

protected:
	SwitchToMode( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new SwitchToMode(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_SwitchToMode_HPP__
