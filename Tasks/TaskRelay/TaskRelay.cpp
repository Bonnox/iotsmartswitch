#include "TaskRelay.h"

#include "../../multitasklib.h"
#include "../../system/MQTTManager/MQTTManager.h"
#include "../TaskFailureDetector/TaskFailureDetector.h"

#include "Mode/Mode.h"

#include "../../system/HAL/HAL.h"
#include "../../system/MQTTManager/MQTTManager.h"

void TaskRelay_setup(HALdata* machine)
{
	HAL_setupPin(relaisRuntime.pinRelay, OUTPUT, machine);
	HAL_setupPin(relaisRuntime.pinButtonLocalBypass, INPUT, machine);
	HAL_setupPin(relaisRuntime.pinButtonLocalState, INPUT, machine);

	MQTTsubscribe(MQTT_TOPIC_NAME_MODE);
	MQTTsubscribe(MQTT_TOPIC_NAME_STATUS_AUTO);
	MQTTsubscribe(MQTT_TOPIC_NAME_STATUS_MANU);
	
	//startDelay(TASK_RELAY_WAIT, &(relaisRuntime.timer));
}

void switchLed()
{

	if (relaisRuntime.relayState==0)
	{
		relaisRuntime.relayState = 1;
	}
	else
	{
		relaisRuntime.relayState = 0;
	}

}

/*
Writes the current relay status on the simulated physical world of the PLC
*/
void handleRelayTransition(RelaisTaskData dataStruct, HALdata* machine)
{
	if (dataStruct.relayState)
	{
		HAL_write(true, dataStruct.pinRelay, HIGH, machine);
	}
	else
	{
		HAL_write(true, dataStruct.pinRelay, LOW, machine);
	}
}



void debug_relay_tasti(bool buttonLocalBypassPressed, bool buttonLocalStateOnPressed, int mode)
{
	Serial.println("============================");
	Serial.print("tasti " + (String)buttonLocalBypassPressed);
	Serial.println(" " + (String)buttonLocalStateOnPressed);
	Serial.println("mode " + (String)mode + " " +(String) (mode== MANU_REMOT));
	Serial.println("============================");
	delay(600);
}

/**
determines the state of the relay, given the topic messages and the buttons.
has a side effect.
*/

void relayLogicSide(RelaisTaskData* structure, String messageManu, String messageAuto, HALdata* machine)
{
	bool buttonLocalBypassPressed= HAL_read(true, relaisRuntime.pinButtonLocalBypass, machine);
	bool buttonLocalStateOnPressed= HAL_read(true, relaisRuntime.pinButtonLocalState, machine);

	//debug_relay_tasti(buttonLocalBypassPressed, buttonLocalStateOnPressed, structure->mode);
	
	if (buttonLocalBypassPressed) // MANU_LOCAL
	{
		structure->relayState = buttonLocalStateOnPressed;
	}
	else
	{
		/*if (structure->mode == AUTO_LOCAL)
		{
			// no false positives induced by a switched off relay
			if (FailureDetectorRuntime.failure && relaisRuntime.relayState)
			{
				//Serial.println("FAILURE aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
				structure->resetting = true;
				startDelay(RESET_WAIT_AFTER_SHUTDOWN, &(structure->resetTimer));
			}
			Serial.println("resetting " + (String)(structure->resetTimer.remainingMillis));
				structure->relayState = !relaisRuntime.resetting;
		}*/
		if (structure->mode == MANU_REMOT)
		{
			if (messageManu.indexOf(PUBSUB_MESSAGE_ON) >= 0)
			{
				structure->relayState=true;
			}
			if (messageManu.indexOf(PUBSUB_MESSAGE_OF) >= 0)
			{
				structure->relayState = false;
			}
		}
		if (structure->mode == AUTO_REMOT)
		{
			if (messageAuto.indexOf(PUBSUB_MESSAGE_ON) >= 0)
			{
				structure->relayState = true;
			}
			if (messageAuto.indexOf(PUBSUB_MESSAGE_OF) >= 0)
			{
				structure->relayState = false;
			}
		}
	}
	
}

/**
changes the operation mode based on the message.
*/
void handleSwitchMode(RelaisTaskData* relaytask, String message)
{

		if (message.indexOf(ModeStrings[AUTO_LOCAL]) >= 0)
		{
			//relaytask->mode = AUTO_LOCAL;
		}
		if (message.indexOf(ModeStrings[AUTO_REMOT]) >= 0)
		{
			relaytask->mode = AUTO_REMOT;
		}
		if (message.indexOf(ModeStrings[MANU_REMOT]) >= 0)
		{
			relaytask->mode = MANU_REMOT;
		}
		// else we keep the previous, for security reasons

		// TODO: Riduci duplicazione facendo
		// result= enumSearch(ModeStrings, message, Mode.last)
		// if (result> 0) mode = result
		//https://stackoverflow.com/questions/159006/max-and-min-values-in-a-c-enum
	
}



void TaskRelay_loop(HALdata* machine, MQTTManagerData* topics)
{
	/*
	if (finished(&(relaisRuntime.timer)))
	{
		switchLed(); // used when the LED is not controlled from MQTT
		startDelay(TASK_RELAY_WAIT, &(relaisRuntime.timer));
	}
	*/

	//----------------------------------------------------
	// code to execute every time, not only every x amount:

	handleSwitchMode(&relaisRuntime, MQTTread(MQTT_TOPIC_NAME_MODE, topics));
	
	
	String msgManu = MQTTread(MQTT_TOPIC_NAME_STATUS_MANU, topics);	
	String msgAuto = MQTTread(MQTT_TOPIC_NAME_STATUS_AUTO, topics);
	relayLogicSide(&relaisRuntime,msgManu, msgAuto, machine);
	
	// return the value instead of setting it inside to minimize the side effects

	handleRelayTransition(relaisRuntime, machine);

	//updateTimer(&relaisRuntime.timer);
}