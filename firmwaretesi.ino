//-----------------------------------------------------------------
// program                                                       //
//-----------------------------------------------------------------

#include "Secrets/wifipsw.h"


//-----------------------------------------------------------------
// standard libs                                                 //
//-----------------------------------------------------------------

#include <stdbool.h>

//-----------------------------------------------------------------
// my libs														 //
//-----------------------------------------------------------------

#include "generalpurposelib.h"
#include "multitasklib.h";
//#include "httprequestlib.h"

//-----------------------------------------------------------------
// third pary libs                                               //
//-----------------------------------------------------------------

//#include <PromLokiTransport.h>
//#include <PrometheusArduino.h>
#include <EspMQTTClient.h>
#include <WiFi.h>
//#include <LinkedList.h>



//-----------------------------------------------------------------
// systems                                                        //
//-----------------------------------------------------------------

#include "system/HAL/Hal.cpp"
#include "system/Pin/Pin.cpp"
#include "system/Main/Main.cpp"
#include "system/MQTTManager/MQTTManager.cpp"
#include "system/PrometheusHelper/PrometheusHelper.cpp"

//-----------------------------------------------------------------
// tasks libs                                                    //
//-----------------------------------------------------------------



#include "Tasks/Wifi/Wifi.cpp"
#include "Tasks/TaskRelay/TaskRelay.cpp"
#include "Tasks/TaskCurrentMonitor/TaskCurrentMonitor.cpp"
#include "Tasks/TaskFailureDetector/TaskFailureDetector.cpp"
#include "Tasks/TaskPrometheus/TaskPrometheus.cpp"


//-----------------------------------------------------------------
// program                                                       //
//-----------------------------------------------------------------

#include "Secrets/wifipsw.h"


#define CONFIG_ARDUINO_LOOP_STACK_SIZE 64*1024
	
globalLoopBusyWaitingAvoider_Data loopMeasure;

void setup()
{  
	Main_setup();
}


void core1(void * args)
{
	Main_loop();
}

void loop()
{
	core1(NULL);
}
