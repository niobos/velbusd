#include "ModuleType.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage { namespace ModuleType {

class ModuleTypeRegisterer {
public:
	ModuleTypeRegisterer() {
		struct factory_methods f;
		f.factory = &ModuleType::factory;
		Registrar::get_instance().add(0xff, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	ModuleTypeRegisterer ModuleType; /* Create a static instance: register with the registrar */
}

VelbusMessage* ModuleType::factory( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() < 2 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xff ) throw FormError("Wrong type");

	switch(data[1]) {
	case '\x09':
		return new VMB2BL(prio, addr, rtr, data.substr(2) );
	case '\x11':
		return new VMB4RYNO(prio, addr, rtr, data.substr(2) );
	default:
		return new Unknown(prio, addr, rtr, data.substr(1) );
	}
}

Unknown::Unknown( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) :
		ModuleType(prio, addr, rtr, data[0]) {
	m_data = data.substr(1);
}

std::string Unknown::data() throw() {
	std::string ret("\xff", 1);
	ret.append(1, m_module_type);
	ret.append(m_data);
	return ret;
}

std::string Unknown::string() throw() {
	std::ostringstream o;
	o << "ModuleType from 0x" << hex(m_addr) << ": "
	  << "Unknown module [0x" << hex(m_module_type)
	  << "] Data[" << m_data.length() << "]:";
	for( unsigned int i=0; i<m_data.length(); i++ ) {
		o << " " << hex(m_data[i]);
	}
	return o.str();
}

VMB2BL::VMB2BL(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) :
		ModuleType(prio, addr, rtr, 0x09) {
	if( data.length() != 3 ) throw FormError("Incorrect length");
	m_dip_switch_timeout = data[0];
	m_build_year = data[1];
	m_build_week = data[2];
}

std::string VMB2BL::data() throw() {
	std::string ret("\xff", 1);
	ret.append(1, m_module_type);
	ret.append(1, m_dip_switch_timeout);
	ret.append(1, m_build_year);
	ret.append(1, m_build_week);
	return ret;
}

std::string VMB2BL::string() throw() {
	std::ostringstream o;
	o << "ModuleType from 0x" << hex(m_addr) << ": "
	  << "VMB2BL  ";

	o << "Timeout blind 1=";
	switch( m_dip_switch_timeout & 0x03 ) {
	case 0: o << "15s"; break;
	case 1: o << "30s"; break;
	case 2: o << "1m"; break;
	case 3: o << "2m"; break;
	default: o << "Unknown[" << hex(m_dip_switch_timeout & 0x03) << "]"; break;
	}

	o << "Timeout blind 2=";
	switch( (m_dip_switch_timeout>>2) & 0x03 ) {
	case 0: o << "15s"; break;
	case 1: o << "30s"; break;
	case 2: o << "1m"; break;
	case 3: o << "2m"; break;
	default: o << "Unknown[" << hex((m_dip_switch_timeout>>2) & 0x03) << "]"; break;
	}

	o << " Build " << static_cast<int>(m_build_year)
	  << "-W" << static_cast<int>(m_build_week);

	return o.str();
}

VMB4RYNO::VMB4RYNO(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) :
		ModuleType(prio, addr, rtr, 0x11) {
	if( data.length() != 5 ) throw FormError("Incorrect length");
	m_serial_number = ((unsigned char)data[0] << 8) | ((unsigned char)data[1]);
	m_memorymap = data[2];
	m_build_year = data[3];
	m_build_week = data[4];
}

std::string VMB4RYNO::data() throw() {
	std::string ret("\xff", 1);
	ret.append(1, m_module_type);
	ret.append(1, m_serial_number >> 8);
	ret.append(1, m_serial_number);
	ret.append(1, m_memorymap);
	ret.append(1, m_build_year);
	ret.append(1, m_build_week);
	return ret;
}

std::string VMB4RYNO::string() throw() {
	std::ostringstream o;
	o << "ModuleType from 0x" << hex(m_addr) << ": "
	  << "VMB4RYNO  "
	  << "S/N: 0x" << hex(m_serial_number)
	  << " mmap v" << (int)m_memorymap
	  << " Build " << static_cast<int>(m_build_year)
	  << "-W" << static_cast<int>(m_build_week);

	return o.str();
}

}} // namespace
