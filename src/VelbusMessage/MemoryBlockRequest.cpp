#include "MemoryBlockRequest.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class MemoryBlockRequestRegisterer {
public:
	MemoryBlockRequestRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 3;
		k.length   = 3;
		k.command  = 0xc9;
		struct factory_methods f;
		f.factory = &MemoryBlockRequest::factory;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	MemoryBlockRequestRegisterer MemoryBlockRequest; /* Create a static instance: register with the registrar */
}

MemoryBlockRequest::MemoryBlockRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 3 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xc9 ) throw FormError("Wrong type");
	m_data_addr = ((unsigned char)data[1] << 8) | ((unsigned char)data[2]);
}

std::string MemoryBlockRequest::data() throw() {
	std::string ret("\xc9", 1);
	ret.append(1, m_data_addr >> 8);
	ret.append(1, m_data_addr);
	return ret;
}

std::string MemoryBlockRequest::string() throw() {
	std::ostringstream o;
	o << "MemoryBlockRequest to 0x" << hex(m_addr) << ": start address 0x" << hex(m_data_addr);
	return o.str();
}

} // namespace
