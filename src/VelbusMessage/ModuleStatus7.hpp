#ifndef __VELBUSMESSAGE_ModuleStatus7_HPP__
#define __VELBUSMESSAGE_ModuleStatus7_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class ModuleStatus7 : public VelbusMessage {
public:
	unsigned char m_input_status;
	unsigned char m_enabled_status;
	unsigned char m_not_inverted_status;
	unsigned char m_locked_status;
	unsigned char m_program_disabled;

	unsigned char m_program;
	bool m_alarm1;
	bool m_alarm1_global;
	bool m_alarm2;
	bool m_alarm2_global;
	bool m_sunrise;
	bool m_sunset;

protected:
	ModuleStatus7( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new ModuleStatus7(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_ModuleStatus7_HPP__
