

#include "HAL.h"

#include "../Pin/Pin.h"

#include <stdbool.h>
#include <LinkedList.h>

/*
to call at the beginning of every cycle
*/
void HAL_cycleBegin(HALdata* machine)
{
	 
	for (int i = 0; i < machine->pinsSize; i++)
	{
		//Pin *tmp = getElementFromList(pins, i);
		//auto index = pins.begin();
		//std::advance(index, i);
		//Pin tmp = *index;
		//PinData tmp = machine->pins[i];
		Pin_physicalRead(&(machine->pins[i]));
	}
}

/*
to call at the end of every cycle
*/
void HAL_cycleEnd(HALdata* machine)
{
	 
	for (int i = 0; i < machine->pinsSize; i++)
	{
		//Pin *tmp = getElementFromList(pins, i);
		//auto index = pins.begin();
		//std::advance(index, i);
		//Pin tmp = *index;
		//PinData tmp = (machine->pins)[i];
		Pin_physicalWrite(&(machine->pins[i]));
	}
}

/**
	reads a pin.
	returns -1 if pin not initialized,
	otherwise 
		if digital: 1 or 0
		if analog: raw value
*/
int HAL_read(bool digital, int number, HALdata* machine)
{

	//PinData pins[] = machine->pins;

	int ret = -1;
	int search = HAL_searchPin(number, machine);
	if (search >= 0)
	{
		//Pin *tmp = getElementFromList(pins, search);
		//auto index = pins.begin();
		//std::advance(index, search);
		//Pin tmp = *index;
		//PinData tmp = machine->pins[search];
		return Pin_read(digital, &(machine->pins[search]));
	}
	return ret;
}

/*
asks to write on a pin.
*/
void HAL_write(bool digital, int number, int value, HALdata* machine)
{
	//PinData pins[] = machine->pins;
	int search = HAL_searchPin(number, machine);
	if (search >= 0)
	{			
		//Pin *tmp = getElementFromList(pins, search);
		//auto index = pins.begin();
		//std::advance(index, search);
		//Pin tmp = *index;
		//PinData tmp = machine->pins[search];

		//Pin_write(digital, value, &tmp);
		Pin_write(digital, value, &(machine->pins[search]));
	}
}

/**
	requests to use a pin.
	direction is INPUT or OUTPUT constant from arduino
*/
void HAL_setupPin(int number, int direction, HALdata* machine)
{
	// expand the array
	machine->pinsSize++;
	//PinData newarray[] = malloc(sizeof(Pin) * machine->pinsSize);

	//for (int i = 0; i < pinssize - 1; i++)
	//{
	//	newarray[i] = machine->pins[i];
	//}
	//--------------------------------

	PinData tempElem;
	Pin(number, direction, &tempElem);
	//newarray[machine->pinsSize - 1] = tempElem;
	//pins = malloc(sizeof(Pin) * pinssize);
	//machine->pins = newarray;
	machine->pins[machine->pinsSize-1]=tempElem;
	//machine->pins.add(temp);

}

void debug_search(int ret)
{
	if (ret == -1)
	{
		Serial.println("no found");
		delay(100);
	}
	else
	{
		Serial.print("found ");
		Serial.println(ret);
		delay(100);
	}
}

/**
	returns the inxed of the argument pin in the pin array
	returns -1 if not found
	*/
int HAL_searchPin(int number, HALdata* machine)
{

	//PinData pins[] = machine->pins;
	int ret = -1;

	//Serial.println(0< pins.size());
	//Serial.println(pins.size());
	for (int i = 0; i < machine->pinsSize; i++)
	{
		//Pin *tmp = getElementFromList(pins,i);
		//auto index = pins.begin();
		//std::advance(index, i);
		//Pin tmp = *index;
		PinData tmp = machine->pins[i];

		if (Pin_getNumber(&tmp) == number)
		{
			ret = i;
		}

	}

	//debug_search(ret);
	return ret;
}