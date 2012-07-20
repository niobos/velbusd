#include "Registrar.hpp"

namespace VelbusMessage {

registrar_key::operator std::string() const throw() {
	std::string ret("TODO"); // TODO
	return ret;
}

bool registrar_key::operator < (struct registrar_key b) const throw() {
	if( rtr < b.rtr ) return true;
	if( rtr > b.rtr ) return false;

	if( priority < b.priority ) return true;
	if( priority > b.priority ) return false;

	if( length < b.length ) return true;
	if( length > b.length ) return false;

	if( command < b.command ) return true;
	return false;
}

} /* namespace */
