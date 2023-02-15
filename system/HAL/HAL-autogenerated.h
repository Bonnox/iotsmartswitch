void HAL_cycleBegin(HALdata *machine);
void HAL_cycleEnd(HALdata *machine);
int HAL_read(bool digital,int number,HALdata *machine);
void HAL_write(bool digital,int number,int value,HALdata *machine);
void HAL_setupPin(int number,int direction,HALdata *machine);
void debug_search(int ret);
int HAL_searchPin(int number,HALdata *machine);
#define INTERFACE 0
#define EXPORT_INTERFACE 0
#define LOCAL_INTERFACE 0
#define EXPORT
#define LOCAL static
#define PUBLIC
#define PRIVATE
#define PROTECTED
