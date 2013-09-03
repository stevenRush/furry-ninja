#include "Game.h"
#include <iostream>
#include <unistd.h>
#include <string>
#include <sstream>

int main()
{
	Manager manager("");
	for(size_t i = 0; i < 100; ++i)
	{
		if (true || i % 10 == 0)
		{
			manager.PrintState();
			sleep(1);
		}
		manager.NextStep();
	}
	return 0;
}
