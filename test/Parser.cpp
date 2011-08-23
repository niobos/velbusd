#include <config.h>
#include <iostream>
#include <fstream>
#include "../src/VelbusMessage/VelbusMessage.hpp"

int main(int argc, char* argv[]) {
	char buffer[32];
	std::cin.read(buffer,sizeof(buffer));

	std::string data(buffer, std::cin.gcount() );
	std::string data_copy(data);

	try {
		std::auto_ptr<VelbusMessage::VelbusMessage> m( VelbusMessage::parse_and_consume( data ) );
		std::cout << m->string() << "\n";

		std::string b = m->message();
		if( b.compare(data_copy) != 0 ) {
			std::cout << "message() does not return same data as passed in\n";
			return 1;
		}

	} catch( VelbusMessage::InsufficientData &e ) {
		std::cout << "InsufficientData\n";

	} catch( VelbusMessage::FormError &e ) {
		std::cout << "FormError: " << e.details() << "\n";
	}
}
