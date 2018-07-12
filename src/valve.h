#include <Arduino.h>

class Valve
{
	public:
		// Constructor
		Valve(uint8_t pin);

		bool turn_on(void);
		bool turn_off(void);
		void toggle_state(void);


	private:
		// Arduino Pin solenoid is attached to
		uint8_t _pin;
		// Keep track of if solenoid is on
		bool _is_on;
		// For automatic timing
		time_t _switch_on_time;
		time_t _run_time;
};