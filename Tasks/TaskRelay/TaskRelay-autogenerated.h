void TaskRelay_setup(HALdata *machine);
void switchLed();
void handleRelayTransition(RelaisTaskData dataStruct,HALdata *machine);
void debug_relay_tasti(bool buttonLocalBypassPressed,bool buttonLocalStateOnPressed,int mode);
void relayLogicSide(RelaisTaskData *structure,String messageManu,String messageAuto,HALdata *machine);
void handleSwitchMode(RelaisTaskData *relaytask,String message);
void TaskRelay_loop(HALdata *machine,MQTTManagerData *topics);
#define INTERFACE 0
#define EXPORT_INTERFACE 0
#define LOCAL_INTERFACE 0
#define EXPORT
#define LOCAL static
#define PUBLIC
#define PRIVATE
#define PROTECTED
