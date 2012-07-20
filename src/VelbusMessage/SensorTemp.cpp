#include "SensorTemp.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class SensorTempRegisterer {
public:
	SensorTempRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 3;
		k.length   = 7;
		k.command  = 0xe6;
		struct factory_methods f;
		f.factory = &SensorTemp::factory;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	SensorTempRegisterer SensorTemp; /* Create a static instance: register with the registrar */
}

SensorTemp::SensorTemp( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 7 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xe6 ) throw FormError("Wrong type");

	m_temp_cur_512thC = (data[1]<<8) | ((unsigned char)data[2]);
	m_temp_min_512thC = (data[3]<<8) | ((unsigned char)data[4]);
	m_temp_max_512thC = (data[5]<<8) | ((unsigned char)data[6]);
}

std::string SensorTemp::data() throw() {
	std::string ret("\xe6", 1);
	ret.append(1, m_temp_cur_512thC >> 8);
	ret.append(1, m_temp_cur_512thC);
	ret.append(1, m_temp_min_512thC >> 8);
	ret.append(1, m_temp_min_512thC);
	ret.append(1, m_temp_max_512thC >> 8);
	ret.append(1, m_temp_max_512thC);
	return ret;
}

std::string SensorTemp::string() throw() {
	std::ostringstream o;
	o << "SensorTemp from 0x" << hex(m_addr) << ": "
	  << "Current=" << format_fixed_point( (float)m_temp_cur_512thC/512, 2) << "C "
	  << "Min=" << format_fixed_point( (float)m_temp_min_512thC/512, 2) << "C "
	  << "Max=" << format_fixed_point( (float)m_temp_max_512thC/512, 2) << "C";

	return o.str();
}

} // namespace
