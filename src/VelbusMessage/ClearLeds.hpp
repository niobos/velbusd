#ifndef __VELBUSMESSAGE_CLEARLEDS_HPP__
#define __VELBUSMESSAGE_CLEARLEDS_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class ClearLeds : public VelbusMessage {
public:
	unsigned char m_led_bitmap;

protected:
	ClearLeds( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new ClearLeds(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_CLEARLEDS_HPP__
