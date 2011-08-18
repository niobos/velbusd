#include "output.hpp"
#include <string>
#include <sstream>
#include <iomanip>

std::string hex(int number) {
	std::ostringstream o;
	o << std::hex << std::setw(2) << std::setfill('0') << (number&0xff);
	return o.str();
}

std::string bin(int bitmap) {
	std::string o;
	for( int i = 7; i>=0; i-- ) {
		o.append( ((bitmap >> i) & 0x01) ? "1" : "0" );
	}
	return o;
}
