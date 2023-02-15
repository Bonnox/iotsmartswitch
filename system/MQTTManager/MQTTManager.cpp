#include "MQTTManager.h"
#include "../../Tasks/TaskRelay/TaskRelay.h"

void TaskMqtt_setup()
{
	MQTTRuntime.client.setMqttClientName(MQTT_DEVICE_NAME);
	MQTTRuntime.client.setMqttServer(MQTT_SERVER, MQTT_USERNAME, MQTT_PASSWORD, MQTT_PORT);
}
// il fatto che la libreria MQTT usi un callback chiamato onconnectionestablished
// significa che non posso fare l'iscrizione dei task in modo sincrono
// ma devo memorizzare una lista temporanea e quando questo viene chiamato
// scorrere la lista e iscrivere i suoi membri.
// questa cosa dipende dall'implementazione e non verra' messa nei diagrammi UML.
// inoltre viene usato uno dei 2 vettori come variabile temporanea per la coda di iscrizione.

bool MQTTsubscribe(String topic)
{

	Serial.println("subscribe " + topic);
	

	if (MQTTRuntime.topicsLength >= MAX_NUMBER_OF_TOPICS-1)
	{
		while (true)
		{
			delay(1000);
			Serial.println(MAX_NUMBER_OF_TOPICS_ERROR_MESSAGE);
		}
		// TODO add check on single topic lenght (against max) and do the same. or switch to linked lists
		return false;
	}else
	{
		int i = MQTTRuntime.topicsLength;
		topic.toCharArray(MQTTRuntime.messagesAsync[i].topic, MAX_TOPIC_LENGTH);
		topic.toCharArray(MQTTRuntime.messages[i].topic, MAX_TOPIC_LENGTH);

		MQTTRuntime.topicsLength++; 
		return true;
	}
}

int search(String request, MQTTMessage messages[], int length)
{
	int result = -1;
	for (int i = 0; i < length; i++)
	{
		String tmp = String(messages[i].topic);
		if (tmp.equals(request))
		{
		result = i;
		}
		
	}
	return result;
}

/*
returns the value of a subscribed topic.
returns an empty string ("") if the topic is not found
*/
String MQTTread(String requestedTopic, MQTTManagerData* clientInstance)
{
	
		int found = search(requestedTopic, clientInstance->messages, clientInstance->topicsLength);

		if (found != -1)
		{
			return clientInstance->messages[found].value;
		}
		else
		{
			return "";
		}
	
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
		

	for (int i = 0; i < MQTTRuntime.topicsLength; i++)
	{
		String topicToSub =String(MQTTRuntime.messagesAsync[i].topic);

		MQTTRuntime.client.subscribe(topicToSub, [](const String & topic, const String & message)
			{
				onMessageReceived(topic, message);
			}, 0);
	}
	Serial.println("mqtt ok");
}

void copyAsyncBufferToCycleBuffer(MQTTManagerData * structure)
{
	int len = structure->topicsLength;
	for(int i=0; i<len;i++)
	{
		String tmpmsg = String(structure->messagesAsync[i].value);
		tmpmsg.toCharArray(structure->messages[i].value, MAX_VALUE_LENGTH);
	}
}

void TaskMqtt_loop()
{
	copyAsyncBufferToCycleBuffer(&MQTTRuntime);
	MQTTRuntime.client.loop();
}

void onMessageReceived(String topic, String message)
{
	
	Serial.print("il messaggio mqtt e': "+topic + "///" +message);
	int updating = search(topic, MQTTRuntime.messagesAsync, MQTTRuntime.topicsLength);
	if (updating != -1)
	{
		message.toCharArray(MQTTRuntime.messagesAsync[updating].value, MAX_VALUE_LENGTH - 1);
		MQTTRuntime.messagesAsync[updating].value[MAX_VALUE_LENGTH - 1] = 0;
		// ^^^ TODO more elegantly fix max lenght?
	}
	delay(50);// TODO needed to fix timing issues?
}