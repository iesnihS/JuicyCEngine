#pragma once

#include <chrono>
#include <ctime>

using namespace std;
using namespace std::chrono;

class Lib {
public:
	//in secs
	static double getTimeStamp() //retourne le temps actuel en seconde
	{
		std::chrono::nanoseconds ns =
			duration_cast<std::chrono::nanoseconds>(system_clock::now().time_since_epoch());
		return ns.count() / 1000000000.0;
	}

	static constexpr double pi() { return 3.141592653589793238462643383279502884; }
};