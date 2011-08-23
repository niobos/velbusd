#include "MemoryBlock.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class MemoryBlockRegisterer {
public:
	MemoryBlockRegisterer() {
		struct factory_methods f;
		f.factory = &MemoryBlock::factory;
		Registrar::get_instance().add(0xcc, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	MemoryBlockRegisterer MemoryBlock; /* Create a static instance: register with the registrar */
}

MemoryBlock::MemoryBlock( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 7 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xcc ) throw FormError("Wrong type");

	m_address = ( (data[1]&0xff) << 8 ) | (data[2]&0xff);
	m_data[0] = data[3];
	m_data[1] = data[4];
	m_data[2] = data[5];
	m_data[3] = data[6];
}

std::string MemoryBlock::data() throw() {
	std::string ret("\xcc", 1);
	ret.append(1, (m_address >> 8) & 0xff);
	ret.append(1, (m_address     ) & 0xff);
	ret.append(1, m_data[0]);
	ret.append(1, m_data[1]);
	ret.append(1, m_data[2]);
	ret.append(1, m_data[3]);
	return ret;
}

std::string MemoryBlock::string() throw() {
	std::ostringstream o;
	o << "MemoryBlock from 0x" << hex(m_addr) << ": "
	  << "@0x" << hex(m_address) << " =";
	for( unsigned int i=0; i<sizeof(m_data); i++ ) {
		o << " " << hex(m_data[i]);
	}
	return o.str();
}

} // namespace
