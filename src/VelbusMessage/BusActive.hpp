#ifndef __VELBUSMESSAGE_BusActive_HPP__
#define __VELBUSMESSAGE_BusActive_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class BusActive : public VelbusMessage {
public:

protected:
	BusActive( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new BusActive(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_BusActive_HPP__
