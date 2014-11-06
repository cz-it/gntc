#include <gnt/gnt_os.h>


void usleep(unsigned long usec)
{
	SleepEx(usec/1000, TRUE);
}
