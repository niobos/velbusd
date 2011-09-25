#ifndef __VELBUSMESSAGE_MemoryDataWrite_HPP__
#define __VELBUSMESSAGE_MemoryDataWrite_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class MemoryDataWrite : public VelbusMessage {
public:
	unsigned short m_data_addr;
	unsigned char m_data;

protected:
	MemoryDataWrite( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new MemoryDataWrite(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_MemoryDataWrite_HPP__
