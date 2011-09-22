#ifndef __VELBUSMESSAGE_BlindRelayStatusRequest_HPP__
#define __VELBUSMESSAGE_BlindRelayStatusRequest_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class BlindRelayStatusRequest : public VelbusMessage {
public:
	unsigned char m_blind_channel;

protected:
	BlindRelayStatusRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new BlindRelayStatusRequest(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_BlindRelayStatusRequest_HPP__
