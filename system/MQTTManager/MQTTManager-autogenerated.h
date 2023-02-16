void TaskMqtt_setup();
bool MQTTsubscribe(String topic);
int search(String request,MQTTMessage messages[],int length);
String MQTTread(String requestedTopic,MQTTManagerData *clientInstance);
void onConnectionEstablished();
void copyAsyncBufferToCycleBuffer(MQTTManagerData *structure);
void TaskMqtt_loop();
void onMessageReceived(String topic,String message);
#define INTERFACE 0
#define EXPORT_INTERFACE 0
#define LOCAL_INTERFACE 0
#define EXPORT
#define LOCAL static
#define PUBLIC
#define PRIVATE
#define PROTECTED
