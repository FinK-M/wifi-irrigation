#include <Arduino.h>
// Standard time functions and types
#include <TimeLib.h>

class Valve
{

	static uint8_t count;

	public:
		// Constructor
		Valve(uint8_t pin);

		bool turn_on(void);
		bool turn_off(void);
		void toggle_state(void);
		bool get_state(void);

		void set_start_time(uint8_t hour, uint8_t minute);
		void set_run_time(uint8_t minutes);

		void update_state(void);

		uint8_t code;

	private:
		// Arduino Pin solenoid is attached to
		uint8_t _pin;
		// Keep track of if solenoid is on
		bool _is_on;
		// For automatic timing
		time_t _switch_on_time;
		time_t _run_time;
};