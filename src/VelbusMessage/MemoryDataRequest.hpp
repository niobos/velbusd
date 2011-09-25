#ifndef __VELBUSMESSAGE_MemoryDataRequest_HPP__
#define __VELBUSMESSAGE_MemoryDataRequest_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class MemoryDataRequest : public VelbusMessage {
public:
	unsigned short m_data_addr;

protected:
	MemoryDataRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new MemoryDataRequest(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_MemoryDataRequest_HPP__
