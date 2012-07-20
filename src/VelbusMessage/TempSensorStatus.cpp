#include "TempSensorStatus.hpp"
#include "Registrar.hpp"
#include "../utils/output.hpp"
#include <sstream>

namespace VelbusMessage {

class TempSensorStatusRegisterer {
public:
	TempSensorStatusRegisterer() {
		struct registrar_key k;
		k.rtr      = 0;
		k.priority = 3;
		k.length   = 8;
		k.command  = 0xea;
		struct factory_methods f;
		f.factory = &TempSensorStatus::factory;
		Registrar::get_instance().add(k, f);
	}
};

extern "C" { /* To make this auto-load when dlopen()ed */
	TempSensorStatusRegisterer TempSensorStatus; /* Create a static instance: register with the registrar */
}

TempSensorStatus::TempSensorStatus( unsigned char prio, unsigned char addr, unsigned char rtr, std::string const &data ) 
		throw( FormError ) :
		VelbusMessage(prio, addr, rtr) {
	if( prio != 3 ) throw FormError("Wrong prio");
	if( rtr != 0 ) throw FormError("Wrong RTR");
	if( data.length() != 8 ) throw FormError("Incorrect length");
	if( data[0] != (char)0xea ) throw FormError("Wrong type");

	m_operating_mode.byte = data[1];
	m_program_step_mode.byte = data[2];
	m_output_status.byte = data[3];
	m_temp_halfC = data[4];
	m_target_temp_halfC = data[5];
	m_timer = (data[6] << 8) | data[7];
}

std::string TempSensorStatus::data() throw() {
	std::string ret("\xea", 1);
	ret.append(1, m_operating_mode.byte);
	ret.append(1, m_program_step_mode.byte);
	ret.append(1, m_output_status.byte);
	ret.append(1, m_temp_halfC);
	ret.append(1, m_target_temp_halfC);
	ret.append(1, m_timer >> 8); ret.append(1, m_timer);
	return ret;
}

std::string TempSensorStatus::string() throw() {
	std::ostringstream o;
	o << "TempSensorStatus from 0x" << hex(m_addr) << ":";

	o << " PB=" << ( m_operating_mode.s.push_button_locked ? "locked" : "unlocked" );

	o << " Mode=";
	switch(m_operating_mode.s.mode) {
	case 0x0: o << "run"; break;
	case 0x1: o << "manual"; break;
	case 0x2: o << "timer"; break;
	case 0x3: o << "disable"; break;
	default:  o << "Unknown[0x" << hex((int)m_operating_mode.s.mode) << "]"; break;
	}

	o << " AutoSend=" << ( m_operating_mode.s.auto_send_sensor_temp ? "on" : "off" );

	o << " Mode=";
	switch(m_operating_mode.s.mode2) {
	case 0x0: o << "safe"; break;
	case 0x1: o << "night"; break;
	case 0x2: o << "day"; break;
	case 0x4: o << "comfort"; break;
	default: o << "unknown[0x" << hex((int)m_operating_mode.s.mode2) << "]"; break;
	}

	o << " " << (m_operating_mode.s.m_cooler_mode ? "Cooler" : "Heater" );

	o << " Unjam={" << (m_program_step_mode.s.unjamming_pump ? "pump" : "no_pump" )
		<< " " << (m_program_step_mode.s.unjamming_valve ? "valve" : "no_valve" )
		<< "}";

	o << " Program={" << ( m_program_step_mode.s.all_program ? "all" : "no_all" )
		<< " " << (m_program_step_mode.s.zone_program ? "zone" : "no_zone" )
		<< " " << (m_program_step_mode.s.sensor_program ? "sensor" : "no_sensor" )
		<< "}";

	o << " ProgramStep=";
	switch(m_program_step_mode.s.program_step) {
	case 0x0: o << "safe"; break;
	case 0x1: o << "night"; break;
	case 0x2: o << "day"; break;
	case 0x4: o << "comfort"; break;
	default: o << "unknown[0x" << hex((int)m_program_step_mode.s.program_step) << "]"; break;
	}

	o << " Output={" << ( m_output_status.s.valve ? "valve" : "no_valve" )
		<< " " << (m_output_status.s.boost ? "boost" : "no_boost" )
		<< " " << (m_output_status.s.day ? "day" : "no_day" )
		<< " " << (m_output_status.s.cooler ? "cooler" : "no_cooler" )
		<< " " << (m_output_status.s.pump ? "pump" : "no_pump" )
		<< " " << (m_output_status.s.low_alarm ? "low_alarm" : "no_low_alarm" )
		<< " " << (m_output_status.s.high_alarm ? "high_alarm" : "no_high_alarm" )
		<< "}";

	o << " Temp=" << format_fixed_point( (float)m_temp_halfC/2, 1) << "C"
		<< " Target=" << format_fixed_point( (float)m_target_temp_halfC/2, 1) << "C";

	o << " Timer=" << m_timer;

	return o.str();
}

} // namespace
