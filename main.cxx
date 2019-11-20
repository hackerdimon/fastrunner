#include "graphics.h"

int main()
{
	initwindow(1000, 1000, "FASTRUNNER");
	circle(100, 50, 40);
	while (!kbhit())
	{
		delay(200);
	}

	return 0;
}