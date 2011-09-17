#ifndef __VELBUSMESSAGE_BlindStatusRequest_HPP__
#define __VELBUSMESSAGE_BlindStatusRequest_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class BlindStatusRequest : public VelbusMessage {
public:
	unsigned char m_blind_channel;

protected:
	BlindStatusRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new BlindStatusRequest(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_BlindStatusRequest_HPP__
