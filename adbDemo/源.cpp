#include <iostream>
#include "cppAdb.h"
int main()
{
	Ctsdmm::cppAdb ad;
	//ad.Connect("23");
	//ad.TapPoint(840,661);
	ad.GetDevices();
	return 0;
}