#ifndef __VELBUSMESSAGE_WriteAddr_HPP__
#define __VELBUSMESSAGE_WriteAddr_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class WriteAddr : public VelbusMessage {
public:
	unsigned char m_module_type;
	unsigned short m_cur_serial_number;
	unsigned char m_new_address;
	unsigned short m_new_serial_number;

protected:
	WriteAddr( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new WriteAddr(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_WriteAddr_HPP__
