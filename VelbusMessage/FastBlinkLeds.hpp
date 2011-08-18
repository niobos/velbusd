#ifndef __VELBUSMESSAGE_FASTBLINKLEDS_HPP__
#define __VELBUSMESSAGE_FASTBLINKLEDS_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class FastBlinkLeds : public VelbusMessage {
public:
	char m_led_bitmap;

protected:
	FastBlinkLeds( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new FastBlinkLeds(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_FASTBLINKLEDS_HPP__
