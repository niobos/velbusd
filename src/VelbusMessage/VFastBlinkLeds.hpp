#ifndef __VELBUSMESSAGE_VFASTBLINKLEDS_HPP__
#define __VELBUSMESSAGE_VFASTBLINKLEDS_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class VFastBlinkLeds : public VelbusMessage {
public:
	char m_led_bitmap;

protected:
	VFastBlinkLeds( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new VFastBlinkLeds(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_VFASTBLINKLEDS_HPP__
