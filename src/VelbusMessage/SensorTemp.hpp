#ifndef __VELBUSMESSAGE_SensorTemp_HPP__
#define __VELBUSMESSAGE_SensorTemp_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class SensorTemp : public VelbusMessage {
public:
	int m_temp_cur_512thC; // In 512th of degrees Celcius
	int m_temp_min_512thC;
	int m_temp_max_512thC;

protected:
	SensorTemp( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new SensorTemp(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_SensorTemp_HPP__
