#include "VelbusMessage.hpp"
#include <stdint.h>
#include <sstream>
#include <iomanip>

#include <stdio.h>

namespace VelbusMessage {

std::string hex(int number) {
	std::ostringstream o;
	o << std::hex << std::setw(2) << std::setfill('0') << (number&0xff);
	return o.str();
}
std::string bin(char bitmap) {
	std::string o;
	for( int i = 7; i>=0; i-- ) {
		o.append( ((bitmap >> i) & 0x01) ? "1" : "0" );
	}
	return o;
}

std::auto_ptr<VelbusMessage> parse(std::string const &msg)
		throw(InsufficientData, FormError) {

	if( msg.length() < 6 ) throw InsufficientData();
	int sum = 0;

	sum += msg[0];
	if( msg[0] != 0x0f ) throw FormError("data[0] != 0x0f");

	sum += msg[1];
	if( (msg[1] & 0xfc) != 0xf8 ) throw FormError("data[1] & 0xfc != 0xf8");
	unsigned char prio = msg[1] & 0x03;

	sum += msg[2];
	unsigned char addr = msg[2];

	sum += msg[3];
	if( (msg[3] & 0xb0) != 0x00 ) throw FormError("data[3] & 0xb0 != 0x00");
	unsigned char rtr = (msg[3]>>6)&0x01;
	size_t length = msg[3] & 0x0f;

	if( msg.length() < 6+length ) throw InsufficientData();

	for(int i=4; i<4+length; i++) sum += msg[i];
	std::string data = msg.substr(4, length);

	sum = (-sum)&0xff;
	if( (msg[4+length]&0xff) != sum ) throw FormError("Checksum incorrect");

	if( msg[4+length+1] != 0x04 ) throw FormError("data[-1] != 0x04");

	// Now identify the type of message
	if( length == 0 ) {
		return std::auto_ptr<VelbusMessage>( new Unknown(prio, addr, rtr, data) );
	}
	switch( data[0] ) {
	case 0x00:
		return std::auto_ptr<VelbusMessage>( new PushButtonStatus(prio, addr, rtr, data) );
	default:
		return std::auto_ptr<VelbusMessage>( new Unknown(prio, addr, rtr, data) );
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

std::string Unknown::string() throw() {
	std::ostringstream o;
	o << "Unknown message:"
	  << " Prio=" << static_cast<unsigned int>(m_prio)
	  << " Addr=0x" << std::hex << static_cast<int>(m_addr) << std::dec
	  << " RTR=" << static_cast<unsigned int>(m_rtr)
	  << " data[" << m_data.length() << "]="
	  << std::hex << std::setw(2) << std::setfill('0');
	for( int i = 0; i < m_data.length(); i++ ) {
		o << (static_cast<unsigned int>(m_data[i])&0xff) << " "; // Ugly hex-dump hack
	}
	o << std::dec << std::setw(0) << std::setfill(' ');
	return o.str();
}

PushButtonStatus::PushButtonStatus( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 0 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 4 ) throw FormError("Incorrect length");
	if( data[0] != 0x00 ) throw FormError("Wrong type");
	m_just_pressed = data[1];
	m_just_released = data[2];
	m_long_pressed = data[3];
}

std::string PushButtonStatus::data() throw() {
	std::string ret("\x00", 1);
	ret.append(1, m_just_pressed);
	ret.append(1, m_just_released);
	ret.append(1, m_long_pressed);
	return ret;
}

std::string PushButtonStatus::string() throw() {
	std::ostringstream o;
	o << "PushButtonStatus from 0x" << hex(m_addr) << ":"
	  << " on=" << bin(m_just_pressed)
	  << " long=" << bin(m_long_pressed)
	  << " off=" << bin(m_just_released)
	  ;
	return o.str();
}

std::auto_ptr<VelbusMessage> Deframer::get_message() throw() {
	std::auto_ptr<VelbusMessage> ret;
	while(1) {
		try {
			ret = parse( m_buffer );
			m_buffer = m_buffer.substr(ret->length());
			return ret;
		} catch( InsufficientData &e ) {
			// try again later
			return ret; // NULL pointer
		} catch( FormError &e ) {
			// Strip of first byte and try again
			m_buffer = m_buffer.substr(1);
		}
	}
}

} // namespace
