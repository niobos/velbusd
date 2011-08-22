#ifndef __VELBUSMESSAGE_RxReady_HPP__
#define __VELBUSMESSAGE_RxReady_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class RxReady : public VelbusMessage {
public:

protected:
	RxReady( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new RxReady(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_RxReady_HPP__
