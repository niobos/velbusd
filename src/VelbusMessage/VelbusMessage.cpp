#include <config.h>
#include "VelbusMessage.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>
#include <stdio.h>
#include "Unknown.hpp"

#ifdef WANT_PARSER
#include "ModuleTypeRequest.hpp"
#endif

namespace VelbusMessage {

VelbusMessage* parse_and_consume(std::string &msg)
		throw(InsufficientData, FormError) {

	if( msges.length() < 6 ) throw InsufficientData();
	int sum = 0;

	sum += msges[0];
	if( msges[0] != 0x0f ) throw FormError("data[0] != 0x0f");

	sum += msges[1];
	if( (msges[1] & 0xfc) != 0xf8 ) throw FormError("data[1] & 0xfc != 0xf8");
	unsigned char prio = msges[1] & 0x03;

	sum += msges[2];
	unsigned char addr = msges[2];

	sum += msges[3];
	if( (msges[3] & 0xb0) != 0x00 ) throw FormError("data[3] & 0xb0 != 0x00");
	unsigned char rtr = (msges[3]>>6)&0x01;
	size_t length = msges[3] & 0x0f;

	if( msges.length() < 6+length ) throw InsufficientData();

	for(unsigned int i=4; i<4+length; i++) sum += msges[i];
	std::string data = msges.substr(4, length);

	sum = (-sum)&0xff;
	if( (msges[4+length]&0xff) != sum ) {
		std::ostringstream e("Checksum incorrect");
		e << ": got " << (msges[4+length]&0xff)
		  << " expected " << sum;
		throw FormError(e.str());
	}

	if( msges[4+length+1] != 0x04 ) throw FormError("data[-1] != 0x04");

	msges = msges.substr(4+length+2); // Consume


	// Now identify the type of message
	try {
		if( data.length() == 0 ) {
			return ModuleTypeRequest::factory(prio, addr, rtr, data);
		}

		struct factory_methods f = Registrar::get_instance().get( data[0] );
		return f.factory(prio, addr, rtr, data);

	} catch( NotFound &e ) {
		return Unknown::factory(prio, addr, rtr, data);

	} catch( FormError &e ) {
		return Unknown::factory(prio, addr, rtr, data);
	}
}

std::string VelbusMessage::message() throw() {
	std::string data = this->data();

	char header[4];
	header[0] = 0x0f;
	header[1] = 0xf8 | (m_prio & 0x03);
	header[2] = m_addr;
	header[3] = 0x00 | ((m_rtr & 0x01)<<6) | (data.length() & 0x0f);
	std::string out(header, 4);

	out.append(data, 0, data.length() & 0x0f); // Strip length of data (if needed)

	int sum = 0;
	for( int i = out.length()-1; i>=0; --i ) {
		sum += out[i];
	}

	sum = (-sum)&0xff;

	out.append(1, sum);
	out.append(1, 0x04);

	return out;
}

} // namespace
