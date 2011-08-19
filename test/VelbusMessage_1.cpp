#include "../src/VelbusMessage/VelbusMessage.hpp"

#include <stdio.h>

int main() {

	{
		printf("Testing parse()\n");
		std::string int_stat_req("\x0f\xf8\x00\x01\x0e\xea\x04", 7);
		std::auto_ptr<VelbusMessage::VelbusMessage> m = VelbusMessage::parse_and_consume(int_stat_req);
		if( m.get() == NULL ) return 1;
	}

	{
		printf("Testing get_message()\n");
		std::string int_stat_req("\x0f\xf8\x00\x01\x0e\xea\x04", 7);
		VelbusMessage::Deframer d;
		d.add_data(int_stat_req);
		std::auto_ptr<VelbusMessage::VelbusMessage> m = d.get_message();
		if( m.get() == NULL ) return 1;
		printf("Testing get_message() another time\n");
		m = d.get_message();
		if( m.get() != NULL ) return 1;
	}

	{
		printf("Testing get_message() on incomplete data\n");
		std::string int_stat_req("\x0f", 1);
		VelbusMessage::Deframer d;
		d.add_data(int_stat_req);
		std::auto_ptr<VelbusMessage::VelbusMessage> m = d.get_message();
		if( m.get() != NULL ) return 1;
	}

	{
		printf("Testing get_message() on prefixed data\n");
		std::string int_stat_req("\0\x0f\xf8\x00\x01\x0e\xea\x04", 8);
		VelbusMessage::Deframer d;
		d.add_data(int_stat_req);
		std::auto_ptr<VelbusMessage::VelbusMessage> m = d.get_message();
		if( m.get() == NULL ) return 1;
		printf("Testing get_message() on prefixed data again\n");
		m = d.get_message();
		if( m.get() != NULL ) return 1;
	}

	{
		printf("Testing get_message() on prefixed double data\n");
		std::string int_stat_req("\0\x0f\xf8\x00\x01\x0e\xea\x04\x12\x0f\xf8\x00\x01\x0e\xea\x04", 16);
		VelbusMessage::Deframer d;
		d.add_data(int_stat_req);
		std::auto_ptr<VelbusMessage::VelbusMessage> m = d.get_message();
		if( m.get() == NULL ) return 1;
		printf("Testing get_message() on prefixed double data, msg2\n");
		m = d.get_message();
		if( m.get() == NULL ) return 1;
		printf("Testing get_message() on prefixed double data, EOF\n");
		m = d.get_message();
		if( m.get() != NULL ) return 1;
	}

	return 0;
}
