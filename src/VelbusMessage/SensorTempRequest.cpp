#include "SensorTempRequest.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class SensorTempRequestRegisterer {
public:
	SensorTempRequestRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 3;
		k.length   = 2;
		k.command  = 0xe5;
		struct factory_methods f;
		f.factory = &SensorTempRequest::factory;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	SensorTempRequestRegisterer SensorTempRequest; /* Create a static instance: register with the registrar */
}

SensorTempRequest::SensorTempRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 2 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xe5 ) throw FormError("Wrong type");
	m_autosend_interval = data[1];
}

std::string SensorTempRequest::data() throw() {
	std::string ret("\xe5", 1);
	ret.append( 1, m_autosend_interval );
	return ret;
}

std::string SensorTempRequest::string() throw() {
	std::ostringstream o;
	o << "SensorTempRequest to 0x" << hex(m_addr)
	  << ": AutoSend ";

	if( m_autosend_interval == 0 ) {
		o << "disabled";
	} else if( m_autosend_interval <= 9 ) {
		o << "onChange";
	} else {
		o << (int)(m_autosend_interval) << "sec";
	}

	return o.str();
}

} // namespace
