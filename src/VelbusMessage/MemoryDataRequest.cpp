#include "MemoryDataRequest.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class MemoryDataRequestRegisterer {
public:
	MemoryDataRequestRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 3;
		k.length   = 3;
		k.command  = 0xfd;
		struct factory_methods f;
		f.factory = &MemoryDataRequest::factory;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	MemoryDataRequestRegisterer MemoryDataRequest; /* Create a static instance: register with the registrar */
}

MemoryDataRequest::MemoryDataRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 3 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xfd ) throw FormError("Wrong type");
	m_data_addr = ((unsigned char)data[1] << 8) | ((unsigned char)data[2]);
}

std::string MemoryDataRequest::data() throw() {
	std::string ret("\xfd", 1);
	ret.append(1, m_data_addr >> 8);
	ret.append(1, m_data_addr);
	return ret;
}

std::string MemoryDataRequest::string() throw() {
	std::ostringstream o;
	o << "MemoryDataRequest to 0x" << hex(m_addr) << ": Address 0x" << hex(m_data_addr);
	return o.str();
}

} // namespace
