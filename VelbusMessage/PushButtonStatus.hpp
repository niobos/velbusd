#ifndef __VELBUSMESSAGE_PUSHBUTTONSTATUS_HPP__
#define __VELBUSMESSAGE_PUSHBUTTONSTATUS_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class PushButtonStatus : public VelbusMessage {
public:
	char m_just_pressed;
	char m_just_released;
	char m_long_pressed;

protected:
	PushButtonStatus( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new PushButtonStatus(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual size_t length() throw() { return 4; }
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_PUSHBUTTONSTATUS_HPP__
