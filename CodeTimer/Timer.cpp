#include <stdlib.h>
#include <chrono>
#include <iostream>

#include "Timer.h"

Timer::Timer()
{
	n = 0;
	std::fill(times, &times[40], 0.0f);
}

Timer::~Timer()
{
}

void Timer::print()
{
	//std::cout << "Action :: time/s Time resolution = " << 1.f / (float)CLOCKS_PER_SEC << "\n";
	std::cout << "-----------" << "\n";

	for (auto i = 0; i < n; ++i)
		std::cout << labels[i] << " :: " << (times[2 * i + 1] - times[2 * i + 0]) / (float)CLOCKS_PER_SEC << "\n";
}

void Timer::start(std::string label)
{
	if (n < 20)
	{
		labels[n] = label;
		times[2 * n] = clock();
	}
	else {
		std::cerr << "No more timers, " << label
			<< " will not be timed." << std::endl;
	}
}

float Timer::stop()
{
	int tmp = n;
	times[2 * n + 1] = clock();
	n++;
	return (times[2 * tmp + 1] - times[2 * tmp + 0]) / (float)CLOCKS_PER_SEC;
}

void Timer::reset()
{
	n = 0;
}