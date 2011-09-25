#include "MemoryDumpRequest.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class MemoryDumpRequestRegisterer {
public:
	MemoryDumpRequestRegisterer() {
		struct factory_methods f;
		f.factory = &MemoryDumpRequest::factory;
		Registrar::get_instance().add(0xcb, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	MemoryDumpRequestRegisterer MemoryDumpRequest; /* Create a static instance: register with the registrar */
}

MemoryDumpRequest::MemoryDumpRequest( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 1 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xcb ) throw FormError("Wrong type");
}

std::string MemoryDumpRequest::data() throw() {
	std::string ret("\xcb", 1);
	return ret;
}

std::string MemoryDumpRequest::string() throw() {
	std::ostringstream o;
	o << "MemoryDumpRequest to 0x" << hex(m_addr);
	return o.str();
}

} // namespace
