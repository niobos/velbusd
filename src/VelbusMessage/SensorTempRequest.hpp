#ifndef __VELBUSMESSAGE_SensorTempRequest_HPP__
#define __VELBUSMESSAGE_SensorTempRequest_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class SensorTempRequest : public VelbusMessage {
public:
	unsigned char m_autosend_interval;

protected:
	SensorTempRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new SensorTempRequest(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_SensorTempRequest_HPP__
