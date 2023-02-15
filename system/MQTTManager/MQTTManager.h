#pragma once

#include "MqttCfg.h"

#include "MQTTMessage/MQTTMessage.h"

/**
an arbitrarily chosen number that represents the 
maximum amount of subscription.
can be increased to an epirically determined value
that can keep the system stable.
*/
#define MAX_NUMBER_OF_TOPICS (16)

#define MAX_NUMBER_OF_TOPICS_ERROR_MESSAGE "max number of mqtt topics reached"

typedef struct
{
	EspMQTTClient client;
	MQTTMessage messagesAsync[MAX_NUMBER_OF_TOPICS];
	MQTTMessage messages[MAX_NUMBER_OF_TOPICS];

	//bool connecting = true; not needed because we do not support subscribing during loop, only once at boot
	int topicsLength=0;

}MQTTManagerData;

MQTTManagerData MQTTRuntime;






#include "MQTTManager-autogenerated.h"