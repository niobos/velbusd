#ifndef __VELBUSMESSAGE_SwitchBlindUpDown_HPP__
#define __VELBUSMESSAGE_SwitchBlindUpDown_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class SwitchBlindUpDown : public VelbusMessage {
public:
	unsigned char m_direction;
	unsigned char m_blind_channel;
	unsigned long m_timeout;

protected:
	SwitchBlindUpDown( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new SwitchBlindUpDown(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_SwitchBlindUpDown_HPP__
