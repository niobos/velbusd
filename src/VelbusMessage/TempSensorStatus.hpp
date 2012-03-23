#ifndef __VELBUSMESSAGE_TempSensorStatus_HPP__
#define __VELBUSMESSAGE_TempSensorStatus_HPP__

#include "VelbusMessage.hpp"

namespace VelbusMessage {

class TempSensorStatus : public VelbusMessage {
public:
	union {
		struct {
			unsigned push_button_locked : 1;
			unsigned mode : 2;
			unsigned auto_send_sensor_temp : 1;
			unsigned mode2 : 3;
			unsigned m_cooler_mode : 1;
		} s;
		unsigned char byte;
	} m_operating_mode;

	union {
		struct {
			unsigned unjamming_pump : 1;
			unsigned unjamming_valve : 1;
			unsigned sensor_program : 1;
			unsigned zone_program : 1;
			unsigned program_step : 3;
			unsigned all_program : 1;
		} s;
		unsigned char byte;
	} m_program_step_mode;

	union {
		struct {
			unsigned valve : 1;
			unsigned boost : 1;
			unsigned day : 1;
			unsigned cooler : 1;
			unsigned pump : 1;
			unsigned low_alarm : 1;
			unsigned high_alarm : 1;
		} s;
		unsigned char byte;
	} m_output_status;

	char m_temp_halfC;
	char m_target_temp_halfC;
	unsigned int m_timer;

protected:
	TempSensorStatus( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data)
		throw(FormError);

public:
	static VelbusMessage* factory(unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data) {
		return new TempSensorStatus(prio, addr, rtr, data);
	}

	virtual std::string data() throw();
	virtual std::string string() throw();
};

} // Namespace

#endif // __VELBUSMESSAGE_TempSensorStatus_HPP__
