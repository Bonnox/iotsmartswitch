void TaskCurrentMonitor_setup(HALdata *machine);
void handleReadyness();
void calculateAverage();
void TaskCurrentMonitor_callBack(HALdata *machine);
void TaskCurrentMonitor_loop(HALdata *machine,char **metric);
#define INTERFACE 0
#define EXPORT_INTERFACE 0
#define LOCAL_INTERFACE 0
#define EXPORT
#define LOCAL static
#define PUBLIC
#define PRIVATE
#define PROTECTED
