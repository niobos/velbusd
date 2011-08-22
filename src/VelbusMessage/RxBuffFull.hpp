#ifndef __VELBUSMESSAGE_RxBuffFull_HPP__
#define __VELBUSMESSAGE_RxBuffFull_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class RxBuffFull : public VelbusMessage {
public:

protected:
	RxBuffFull( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new RxBuffFull(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_RxBuffFull_HPP__
