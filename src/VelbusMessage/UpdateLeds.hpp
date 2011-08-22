#ifndef __VELBUSMESSAGE_UPDATELEDS_HPP__
#define __VELBUSMESSAGE_UPDATELEDS_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class UpdateLeds : public VelbusMessage {
public:
	unsigned char m_led_on;
	unsigned char m_led_slow_blink;
	unsigned char m_led_fast_blink;

protected:
	UpdateLeds( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new UpdateLeds(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_UPDATELEDS_HPP__
