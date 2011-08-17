#ifndef __VELBUSMESSAGE_HPP__
#define __VELBUSMESSAGE_HPP__

#include <string>
#include <stdexcept>

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
};

namespace VelbusMessage {

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
	std::string get() throw();
	virtual size_t length() throw() =0;

	virtual std::string string() throw() =0;
};

std::auto_ptr<VelbusMessage> parse(std::string const &msg) throw(InsufficientData, FormError);

class Unknown : public VelbusMessage {
public:
	std::string m_data;

	Unknown(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) :
		VelbusMessage(prio, addr, rtr), m_data(data) {}

	virtual std::string data() throw() { return m_data; }
	virtual size_t length() throw() { return 4+m_data.length()+2; }
	virtual std::string string() throw();
};

class Deframer {
protected:
	std::string m_buffer;
public:
	void add_data(std::string const &data) throw() { m_buffer.append(data); }
	std::auto_ptr< VelbusMessage > get_message() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_HPP__
