#ifndef VELBUSMESSAGE_REGISTRAR_H__
#define VELBUSMESSAGE_REGISTRAR_H__

#include "../utils/Registrar.t"
#include "VelbusMessage.hpp"

namespace VelbusMessage {
	struct registrar_key {
		bool rtr;
		unsigned char priority;
		unsigned char length;
		unsigned char command;

		operator std::string() const throw();
		bool operator < (struct registrar_key b) const throw();
	};

	struct factory_methods {
		VelbusMessage* (*factory)(unsigned char, unsigned char, unsigned char, std::string const &);
	};

	typedef ::Registrar<
		struct registrar_key,
		struct factory_methods
	  > Registrar;
}

#endif // VELBUSMESSAGE_REGISTRAR_H__
