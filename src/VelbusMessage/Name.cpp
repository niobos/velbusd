#include "Name.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class NameRegisterer {
public:
	NameRegisterer() {
		struct factory_methods f;
		f.factory = &Name::factory;
		Registrar::get_instance().add(0xf0, f);
		Registrar::get_instance().add(0xf1, f);
		Registrar::get_instance().add(0xf2, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	NameRegisterer Name; /* Create a static instance: register with the registrar */
}

Name::Name( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 8 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xf0 &&
	    data[0] != (char)0xf1 &&
	    data[0] != (char)0xf2
	  ) throw FormError("Wrong type");

	m_part = data[0] & 0x03;
	m_channel = data[1];
	int i = 2-1;
	while( data[++i] != (char)0xff ) {
		m_name.append(1, data[i]);
	}
}

std::string Name::data() throw() {
	std::string ret;
	ret.append(1, 0xf0+m_part);
	ret.append(1, m_channel);
	ret.append(m_name.data(), m_name.length());
	if( m_name.length() < 6 ) ret.append("\xff\xff\xff\xff\xff\xff", 6-m_name.length());
	return ret;
}

std::string Name::string() throw() {
	std::ostringstream o;
	o << "Name from 0x" << hex(m_addr) << ": "
	  << "Item " << bin(m_channel) << " pt" << (int)m_part << "/3"
	  << " \"" << m_name << "\"";
	return o.str();
}

} // namespace
