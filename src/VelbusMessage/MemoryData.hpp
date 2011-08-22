#ifndef __VELBUSMESSAGE_MemoryData_HPP__
#define __VELBUSMESSAGE_MemoryData_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class MemoryData : public VelbusMessage {
public:
	unsigned short m_address;
	unsigned char m_data;

protected:
	MemoryData( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new MemoryData(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_MemoryData_HPP__
