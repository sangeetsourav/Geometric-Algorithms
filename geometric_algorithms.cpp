// geometric_algorithms.cpp : Defines the entry point for the application.
//

#include "line_sweep.h"
#include <iostream>
#include <string>

int main()
{
	std::string input_file = R"(.\input\line_sweep_input.txt)";
	lineSweep::LineSweepAlgorithm lsa;
	lsa.loadSegmentsFromFile(input_file);
	lsa.sweep();
	return 0;
}
