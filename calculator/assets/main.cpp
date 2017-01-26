//main.cpp
#include "calculate.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
 	cerr << "Before Init" << endl;
	try
	{
		Calculate* cal = new Calculate();
 		cal->init();
		cal->run();
		cal->halt();
		delete cal;
	}
	/*
	 * Catch all Guichan exceptions
	 */
	catch (gcn::Exception e)
	{
		std::cerr << e.getMessage() << std::endl;
		return 1;
	}
	/*
	 * Catch all Std exceptions
	 */
	catch (std::exception e)
	{
		std::cerr << "Std exception: " << e.what() << std::endl;
		return 1;
	}
	/*
	 * Catch all Unknown exceptions
	 */
	catch (...)
	{
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
	return 0;
}