#include "MemoryData.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class MemoryDataRegisterer {
public:
	MemoryDataRegisterer() {
		struct factory_methods f;
		f.factory = &MemoryData::factory;
		Registrar::get_instance().add(0xfe, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	MemoryDataRegisterer MemoryData; /* Create a static instance: register with the registrar */
}

MemoryData::MemoryData( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 4 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xfe ) throw FormError("Wrong type");
	m_address = ((data[1]&0xff) << 8) | (data[2]&0xff);
	m_data = data[3];
}

std::string MemoryData::data() throw() {
	std::string ret("\xfe", 1);
	ret.append(1, (m_address >> 8) & 0xff);
	ret.append(1, (m_address     ) & 0xff);
	ret.append(1, m_data);
	return ret;
}

std::string MemoryData::string() throw() {
	std::ostringstream o;
	o << "MemoryData from 0x" << hex(m_addr) << ": "
	  << "@0x" << hex(m_address) << " = 0x" << hex(m_data);
	return o.str();
}

} // namespace
