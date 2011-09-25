#include "MemoryBlockWrite.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class MemoryBlockWriteRegisterer {
public:
	MemoryBlockWriteRegisterer() {
		struct factory_methods f;
		f.factory = &MemoryBlockWrite::factory;
		Registrar::get_instance().add(0xca, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	MemoryBlockWriteRegisterer MemoryBlockWrite; /* Create a static instance: register with the registrar */
}

MemoryBlockWrite::MemoryBlockWrite( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 7 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xca ) throw FormError("Wrong type");
	m_data_addr = ((unsigned char)data[1] << 8) | ((unsigned char)data[2]);
	m_data[0] = data[3];
	m_data[1] = data[4];
	m_data[2] = data[5];
	m_data[3] = data[6];
}

std::string MemoryBlockWrite::data() throw() {
	std::string ret("\xca", 1);
	ret.append(1, m_data_addr >> 8);
	ret.append(1, m_data_addr);
	ret.append(1, m_data[0]);
	ret.append(1, m_data[1]);
	ret.append(1, m_data[2]);
	ret.append(1, m_data[3]);
	return ret;
}

std::string MemoryBlockWrite::string() throw() {
	std::ostringstream o;
	o << "MemoryBlockWrite to 0x" << hex(m_addr)
	  << ": @0x" << hex(m_data_addr) << " <= "
	  << hex(m_data[0]) << " "
	  << hex(m_data[1]) << " "
	  << hex(m_data[2]) << " "
	  << hex(m_data[3]);
	return o.str();
}

} // namespace
