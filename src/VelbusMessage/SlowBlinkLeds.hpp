#ifndef __VELBUSMESSAGE_SLOWBLINKLEDS_HPP__
#define __VELBUSMESSAGE_SLOWBLINKLEDS_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class SlowBlinkLeds : public VelbusMessage {
public:
	unsigned char m_led_bitmap;

protected:
	SlowBlinkLeds( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new SlowBlinkLeds(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_SLOWBLINKLEDS_HPP__
