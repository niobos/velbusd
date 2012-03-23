#ifndef __VELBUSMESSAGE_ModuleStatusRequest_HPP__
#define __VELBUSMESSAGE_ModuleStatusRequest_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class ModuleStatusRequest : public VelbusMessage {
public:
	unsigned char m_blind_channel;

protected:
	ModuleStatusRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new ModuleStatusRequest(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_ModuleStatusRequest_HPP__
