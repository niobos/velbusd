#ifndef __VELBUSMESSAGE_MemoryDumpRequest_HPP__
#define __VELBUSMESSAGE_MemoryDumpRequest_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class MemoryDumpRequest : public VelbusMessage {
public:

protected:
	MemoryDumpRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new MemoryDumpRequest(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_MemoryDumpRequest_HPP__
