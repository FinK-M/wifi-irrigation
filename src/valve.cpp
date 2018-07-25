#include "valve.h"

Valve::Valve(uint8_t pin){
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
	tmElements_t t;
	t.Hour = hour;
	t.Minute = minute;
	_switch_on_time = makeTime(t);
}

void Valve::set_run_time(uint8_t minutes){
	tmElements_t t;
	t.Minute = minutes;
	_run_time = makeTime(t);
}

void update_state(void){
	
}

uint8_t Valve::count;