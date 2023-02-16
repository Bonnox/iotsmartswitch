#include "Pin.h"
#include "../../generalpurposelib.h"
#include "../HAL/board.h"

	

	void Pin(int _number, int _direction, PinData* structure)
	{
		structure->direction = _direction;
		structure->number = _number;
		Pin_setup(structure);
		
	}

	void Pin_setup(PinData* structure)
	{
		pinMode(structure->number, structure->direction);

		if (structure->direction == INPUT)
		{
			Pin_physicalRead(structure);
		}

	}

	void Pin_physicalRead(PinData* structure)
	{
		if (structure->direction == INPUT)
		{

			structure->value_analog = analogRead(structure->number);
			structure->value_digital = digitalRead(structure->number);
			if (structure->value_analog > (ADC_SAMPLES * 0.9))
			{
				structure->value_digital = HIGH;
			}
			// TODO replace 0.9 with the high logical level threshold of the physical board; or read below
			/*
				to make an even better system, for example, we could have several arrays of booleans 
				that represent the capabilities of each single pin of the board.
				one array for input, one for output, etcetera.
				thus, when a request is incoming, it is only needed to do

				PSEUDOCODE
				-----------------------------------------------------------
				if(requested_direction && requested_capability[pin_number])
				{
					CapabilityDirection();								(1)
				}
				-----------------------------------------------------------
				in line (1), replace Capability with analog/digital and Direction with read/write.
			*/

		}
	}

	void Pin_physicalWrite(PinData* structure)
	{
		if (structure->direction == OUTPUT)
		{
			if (structure->requestW)
			{
				if (structure->digitalW)
				{
					digitalWrite(structure->number, structure->value_next);
				}
				else
				{
					analogWrite(structure->number, structure->value_next);
				}
				structure->requestW = false;
				structure->digitalW = false;
			}
		}
	}

	int Pin_read(bool digital, PinData* structure)
	{
		if (digital)
		{
			return structure->value_digital;
		}
		else
		{
			return structure->value_analog;
		}
	}
	void Pin_write(bool digital, int value, PinData* structure)
	{
		structure->requestW = true;
		structure->digitalW = digital;
		structure->value_next = value;
	}

	int Pin_getNumber(PinData* structure)
	{
		return structure->number;
	}

