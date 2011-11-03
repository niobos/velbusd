#ifndef __VELBUSMESSAGE_RTCstatus_HPP__
#define __VELBUSMESSAGE_RTCstatus_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class RTCstatus : public VelbusMessage {
public:
	unsigned char m_day; // 0=monday, ..., 6=sunday
	unsigned char m_hour;
	unsigned char m_minute;

protected:
	RTCstatus( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new RTCstatus(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_RTCstatus_HPP__
