#include "BusErrorCounter.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class BusErrorCounterRegisterer {
public:
	BusErrorCounterRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 3;
		k.length   = 4;
		k.command  = 0xda;
		struct factory_methods f;
		f.factory = &BusErrorCounter::factory;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	BusErrorCounterRegisterer BusErrorCounter; /* Create a static instance: register with the registrar */
}

BusErrorCounter::BusErrorCounter( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 4 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xda ) throw FormError("Wrong type");

	m_tx_errors = data[1];
	m_rx_errors = data[2];
	m_bus_off = data[3];
}

std::string BusErrorCounter::data() throw() {
	std::string ret("\xda", 1);
	ret.append(1, m_tx_errors);
	ret.append(1, m_rx_errors);
	ret.append(1, m_bus_off);
	return ret;
}

std::string BusErrorCounter::string() throw() {
	std::ostringstream o;
	o << "BusErrorCounter from 0x" << hex(m_addr) << ":"
	  << " Tx=" << static_cast<unsigned int>(m_tx_errors)
	  << " Rx=" << static_cast<unsigned int>(m_rx_errors)
	  << " Bus off=" << static_cast<unsigned int>(m_bus_off);
	return o.str();
}

} // namespace
