#include <gnt_os.h>

/**
* for windows api
*/
#if defined (_WIN32) || defined (_WIN64)
void usleep(unsigned long usec)
{
	SleepEx(usec/1000, TRUE);
}
#endif /* end of defined (_WIN32) || defined (_WIN64) */