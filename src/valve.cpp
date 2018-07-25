#include "valve.h"

Valve::Valve(uint8_t pin):
	_switch_on_time(0),
	_end_time(0)
	{
	_pin = pin;

	code = ++count;

	pinMode(_pin, OUTPUT);
	turn_off();
}

bool Valve::turn_on(void){
	// return false if already on
	if(_is_on){
		return false;
	}
	else{
		digitalWrite(_pin, LOW);
		_is_on = true;
		return true;	
	}
}

bool Valve::turn_off(void){
	// return false if already off
	if(!_is_on){
		return false;
	}
	else{
		digitalWrite(_pin, HIGH);
		_is_on = false;
		return true;	
	}
}

void Valve::toggle_state(void){
	// _is_on is oppsite of actual pin state
	digitalWrite(_pin, _is_on);
	_is_on = !_is_on;
}

bool Valve::get_state(void){
	return _is_on;
}

void Valve::set_start_time(uint8_t hour, uint8_t minute){
	// Time in minutes as seconds not used
	_switch_on_time = hour * 60 +  minute;
	Serial.println(_switch_on_time);
}

void Valve::set_run_time(uint8_t minutes){
	Serial.println(_switch_on_time);
	_end_time = _switch_on_time + minutes;
	Serial.printf("End Time: %d\r\n", _end_time);
}

bool Valve::update_state(void){
	uint16_t current = (uint16_t) hour() * 60 + (uint16_t) minute();
	if(_switch_on_time <= current && current < _end_time){ return turn_on(); }
	else{ return turn_off(); }
}

uint8_t Valve::count;