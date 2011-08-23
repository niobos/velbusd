#ifndef __VELBUSMESSAGE_BusErrorCounter_HPP__
#define __VELBUSMESSAGE_BusErrorCounter_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class BusErrorCounter : public VelbusMessage {
public:
	unsigned char m_tx_errors;
	unsigned char m_rx_errors;
	unsigned char m_bus_off;

protected:
	BusErrorCounter( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new BusErrorCounter(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_BusErrorCounter_HPP__
