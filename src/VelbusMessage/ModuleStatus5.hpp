#ifndef __VELBUSMESSAGE_ModuleStatus5_HPP__
#define __VELBUSMESSAGE_ModuleStatus5_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class ModuleStatus5 : public VelbusMessage {
public:
	unsigned char m_input_status;
	unsigned char m_leds_continuous;
	unsigned char m_leds_slow_blink;
	unsigned char m_leds_fast_blink;

protected:
	ModuleStatus5( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new ModuleStatus5(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_ModuleStatus5_HPP__
