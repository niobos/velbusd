#ifndef __VELBUSMESSAGE_BusErrorCounterRequest_HPP__
#define __VELBUSMESSAGE_BusErrorCounterRequest_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class BusErrorCounterRequest : public VelbusMessage {
public:

protected:
	BusErrorCounterRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new BusErrorCounterRequest(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_BusErrorCounterRequest_HPP__
