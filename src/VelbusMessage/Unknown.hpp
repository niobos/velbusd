#ifndef __VELBUSMESSAGE_Unknown_HPP__
#define __VELBUSMESSAGE_Unknown_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class Unknown : public VelbusMessage {
public:
	std::string m_data;

protected:
	Unknown(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) :
		VelbusMessage(prio, addr, rtr), m_data(data) {}

public:
	static Unknown* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new Unknown(prio, addr, rtr, data);
	}

	virtual std::string data() throw() { return m_data; }
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_Unknown_HPP__
