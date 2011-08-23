#ifndef __VELBUSMESSAGE_IntStatusRequest_HPP__
#define __VELBUSMESSAGE_IntStatusRequest_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class IntStatusRequest : public VelbusMessage {
public:

protected:
	IntStatusRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new IntStatusRequest(prio, addr, rtr, data);
	}

	IntStatusRequest();

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_IntStatusRequest_HPP__
