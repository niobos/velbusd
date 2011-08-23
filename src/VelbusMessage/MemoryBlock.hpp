#ifndef __VELBUSMESSAGE_MemoryBlock_HPP__
#define __VELBUSMESSAGE_MemoryBlock_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class MemoryBlock : public VelbusMessage {
public:
	unsigned short m_address;
	unsigned char m_data[4];

protected:
	MemoryBlock( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new MemoryBlock(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_MemoryBlock_HPP__
