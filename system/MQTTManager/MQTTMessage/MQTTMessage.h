#include "../MqttCfg.h"

// TODO memory efficiency, read below

/*
reduce memory usage by having a class that contains
- only one array of topics
- array for async messages
- array for cycle messages

this way we save one entire array of topics, which is big
*/

typedef struct
{

	char* topic= (char*)calloc(1, MAX_TOPIC_LENGTH);
	char* value= (char*)calloc(1, MAX_VALUE_LENGTH);

} MQTTMessage;