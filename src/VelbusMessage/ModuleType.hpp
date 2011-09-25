#ifndef __VELBUSMESSAGE_ModuleType_HPP__
#define __VELBUSMESSAGE_ModuleType_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage { namespace ModuleType {

class ModuleType : public VelbusMessage {
public:
	unsigned char m_module_type;

protected:
	ModuleType( unsigned char prio, unsigned char addr, unsigned char rtr, unsigned char module_type)
		throw(FormError) :
		VelbusMessage(prio, addr, rtr),
		m_module_type(module_type)
		{}

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data);

	virtual std::string data() throw() =0;
	virtual std::string string() throw() =0;
};

class Unknown : public ModuleType {
public:
	std::string m_data;

	Unknown(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data);

	virtual std::string data() throw();
	virtual std::string string() throw();
};

class VMB2BL : public ModuleType {
public:
	unsigned char m_dip_switch_timeout;
	unsigned char m_build_year;
	unsigned char m_build_week;

	VMB2BL(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data);

	virtual std::string data() throw();
	virtual std::string string() throw();
};

class VMB4RYNO : public ModuleType {
public:
	unsigned short m_serial_number;
	unsigned char m_memorymap;
	unsigned char m_build_year;
	unsigned char m_build_week;

	VMB4RYNO(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data);

	virtual std::string data() throw();
	virtual std::string string() throw();
};

}} // Namespace

#endif // __VELBUSMESSAGE_ModuleType_HPP__
