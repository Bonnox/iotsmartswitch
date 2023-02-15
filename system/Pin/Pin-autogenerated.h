void Pin(int _number,int _direction,PinData *structure);
void Pin_setup(PinData *structure);
void Pin_physicalRead(PinData *structure);
void Pin_physicalWrite(PinData *structure);
int Pin_read(bool digital,PinData *structure);
void Pin_write(bool digital,int value,PinData *structure);
int Pin_getNumber(PinData *structure);
#define INTERFACE 0
#define EXPORT_INTERFACE 0
#define LOCAL_INTERFACE 0
#define EXPORT
#define LOCAL static
#define PUBLIC
#define PRIVATE
#define PROTECTED
