#ifndef __VELBUSMESSAGE_HPP__
#define __VELBUSMESSAGE_HPP__

#include <string>
#include <stdexcept>
#include <memory>

namespace VelbusMessage {

class InsufficientData : public std::runtime_error {
public:
	InsufficientData() throw() :
		std::runtime_error("Insufficient data") {}
	~InsufficientData() throw() {}
};

class FormError : public std::runtime_error {
private:
	std::string m_details;
public:
	FormError(std::string const &msg = "") throw() :
		std::runtime_error("Form error in message"),
		m_details(msg) {}
	~FormError() throw() {}

	std::string details() throw() { return m_details; }
};

class VelbusMessage {
public:
	unsigned char m_prio;
	unsigned char m_addr;
	unsigned char m_rtr;

protected:
	VelbusMessage(unsigned char prio, unsigned char addr, unsigned char rtr) throw() :
		m_prio(prio), m_addr(addr), m_rtr(rtr) {};

public:
	virtual ~VelbusMessage() {}

	virtual std::string data() throw() =0;

	std::string message() throw();
	virtual std::string string() throw() =0;
};

VelbusMessage* parse_and_consume(std::string &msg) throw(InsufficientData, FormError);

} // Namespace

#endif // __VELBUSMESSAGE_HPP__
