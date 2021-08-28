
#include "lvdll.h"
#include <string.h>

EXPORT long lvtest_mul3(long a)
{
	return a*3;
}

static const char *message="One step beyond!";

EXPORT int lvtest_str(char *s)
{
	strcpy(s,message);
	return 3;
}
