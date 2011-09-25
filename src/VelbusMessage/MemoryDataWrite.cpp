#include "MemoryDataWrite.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class MemoryDataWriteRegisterer {
public:
	MemoryDataWriteRegisterer() {
		struct factory_methods f;
		f.factory = &MemoryDataWrite::factory;
		Registrar::get_instance().add(0xfc, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	MemoryDataWriteRegisterer MemoryDataWrite; /* Create a static instance: register with the registrar */
}

MemoryDataWrite::MemoryDataWrite( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 4 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xfc ) throw FormError("Wrong type");
	m_data_addr = ((unsigned char)data[1] << 8) | ((unsigned char)data[2]);
	m_data = data[3];
}

std::string MemoryDataWrite::data() throw() {
	std::string ret("\xfc", 1);
	ret.append(1, m_data_addr >> 8);
	ret.append(1, m_data_addr);
	ret.append(1, m_data);
	return ret;
}

std::string MemoryDataWrite::string() throw() {
	std::ostringstream o;
	o << "MemoryDataWrite to 0x" << hex(m_addr)
	  << ": @0x" << hex(m_data_addr) << " <= 0x" << hex(m_data);
	return o.str();
}

} // namespace
