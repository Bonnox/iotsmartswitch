#pragma once

#include <stdbool.h>

#include "../Pin/Pin.h"

#define ALERT_THRESHOLD 0.1

typedef struct
{
	PinData pins[19*2]; // the esp32 has 19 pins each side
	int pinsSize = 0;
} HALdata;


#include "HAL-autogenerated.h"