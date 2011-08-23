#ifndef __VELBUSMESSAGE_ModuleTypeRequest_HPP__
#define __VELBUSMESSAGE_ModuleTypeRequest_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class ModuleTypeRequest : public VelbusMessage {
public:

protected:
	ModuleTypeRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new ModuleTypeRequest(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_ModuleTypeRequest_HPP__
