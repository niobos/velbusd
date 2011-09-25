#ifndef __VELBUSMESSAGE_MemoryBlockWrite_HPP__
#define __VELBUSMESSAGE_MemoryBlockWrite_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class MemoryBlockWrite : public VelbusMessage {
public:
	unsigned short m_data_addr;
	unsigned char m_data[4];

protected:
	MemoryBlockWrite( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new MemoryBlockWrite(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_MemoryBlockWrite_HPP__
