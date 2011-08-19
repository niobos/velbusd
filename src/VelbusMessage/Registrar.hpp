#ifndef VELBUSMESSAGE_REGISTRAR_H__
#define VELBUSMESSAGE_REGISTRAR_H__

#include "../utils/Registrar.t"
#include "VelbusMessage.hpp"

namespace VelbusMessage {
	struct factory_methods {
		VelbusMessage* (*factory)(unsigned char, unsigned char, unsigned char, std::string const &);
	};

	typedef ::Registrar<
		char,
		struct factory_methods
	  > Registrar;
}

#endif // VELBUSMESSAGE_REGISTRAR_H__
