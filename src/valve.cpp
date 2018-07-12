#include "valve.h"

Valve::Valve(uint8_t pin){
	_pin = pin;
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
	digitalWrite(_pin, !digitalRead(_pin));
	_is_on = !_is_on;
}