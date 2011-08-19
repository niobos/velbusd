#include <config.h>
#include <iostream>
#include <fstream>
#include "../src/VelbusMessage/VelbusMessage.hpp"

int main(int argc, char* argv[]) {
	char buffer[32];
	std::cin.read(buffer,sizeof(buffer));

	std::string data(buffer, std::cin.gcount() );

	std::auto_ptr<VelbusMessage::VelbusMessage> m = VelbusMessage::parse_and_consume( data );

	std::cout << m->string() << "\n";
}
