#include "WriteAddr.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class WriteAddrRegisterer {
public:
	WriteAddrRegisterer() {
		struct factory_methods f;
		f.factory = &WriteAddr::factory;
		Registrar::get_instance().add(0x6a, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	WriteAddrRegisterer WriteAddr; /* Create a static instance: register with the registrar */
}

WriteAddr::WriteAddr( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 7 ) throw FormError("Incorrect length");
	if( data[0] != (char)0x6a ) throw FormError("Wrong type");

	m_module_type = data[1];
	m_cur_serial_number = ((unsigned char)data[2] << 8) | ((unsigned char)data[3]);
	m_new_address = data[4];
	m_new_serial_number = ((unsigned char)data[5] << 8) | ((unsigned char)data[6]);
}

std::string WriteAddr::data() throw() {
	std::string ret("\x6a", 1);

	ret.append(1, m_module_type);
	ret.append(1, m_cur_serial_number >> 8);
	ret.append(1, m_cur_serial_number);
	ret.append(1, m_new_address);
	ret.append(1, m_new_serial_number >> 8);
	ret.append(1, m_new_serial_number);
	return ret;
}

std::string WriteAddr::string() throw() {
	std::ostringstream o;
	o << "WriteAddr to 0x" << hex(m_addr) << ": "
	  << "Current S/N: 0x" << hex(m_cur_serial_number)
	  << "  New addr: 0x" << hex(m_new_address)
	  << "  New S/N: 0x" << hex(m_new_serial_number);
	return o.str();
}

} // namespace
