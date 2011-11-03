#include "RTCstatus.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class RTCstatusRegisterer {
public:
	RTCstatusRegisterer() {
		struct factory_methods f;
		f.factory = &RTCstatus::factory;
		Registrar::get_instance().add(0xd8, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	RTCstatusRegisterer RTCstatus; /* Create a static instance: register with the registrar */
}

RTCstatus::RTCstatus( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 4 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xd8 ) throw FormError("Wrong type");
	m_day = data[1];
	m_hour = data[2];
	m_minute = data[3];
}

std::string RTCstatus::data() throw() {
	std::string ret("\xd8", 1);
	ret.append(1, m_day);
	ret.append(1, m_hour);
	ret.append(1, m_minute);
	return ret;
}

std::string RTCstatus::string() throw() {
	std::ostringstream o;
	o << "RTCstatus to 0x" << hex(m_addr) << ": ";

	switch(m_day) {
	case 0: o << "Monday"; break;
	case 1: o << "Tuesday"; break;
	case 2: o << "Wednesday"; break;
	case 3: o << "Thursday"; break;
	case 4: o << "Friday"; break;
	case 5: o << "Saturday"; break;
	case 6: o << "Sunday"; break;
	default: o << "<unknown weekday>"; break;
	}

	o << " " << (unsigned int)m_hour << ":" << dec(m_minute,2);

	return o.str();
}

} // namespace
