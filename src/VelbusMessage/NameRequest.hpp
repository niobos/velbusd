#ifndef __VELBUSMESSAGE_NameRequest_HPP__
#define __VELBUSMESSAGE_NameRequest_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class NameRequest : public VelbusMessage {
public:
	unsigned char m_what;

protected:
	NameRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new NameRequest(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_NameRequest_HPP__
