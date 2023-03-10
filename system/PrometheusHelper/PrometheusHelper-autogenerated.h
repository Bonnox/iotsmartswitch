void generatePushURL(String pushJob,String pushInstance,String pushNamespace);
String generateMetricStringGauge(String name,String description,float value);
bool preliminaryChecks();
void writeMetricWithHttpRequest(AsyncClient *cli,String metrics,char *server);
bool initConnection();
void handleNotConnected();
void setupOnError();
void setupOnDisconnect(AsyncClient *cli);
void timerStuffPrint();
void sthPrint(uint32_t code);
void closeAndDisconnect(AsyncClient *c);
void pushMetrics(String metrics,char *server);
#define INTERFACE 0
#define EXPORT_INTERFACE 0
#define LOCAL_INTERFACE 0
#define EXPORT
#define LOCAL static
#define PUBLIC
#define PRIVATE
#define PROTECTED
