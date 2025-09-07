// geometric_algorithms.cpp : Defines the entry point for the application.
//

#include "line_sweep.h"
#include "delauney.h"
#include <iostream>
#include <string>

int main()
{	
	bool line_sweep_test = false;
	bool delauney_test = true;

	// Line Sweep
	if (line_sweep_test)
	{
		std::string input_file = R"(.\input\line_sweep_input.txt)";
		lineSweep::LineSweepAlgorithm lsa;
		lsa.loadSegmentsFromFile(input_file);
		lsa.sweep();
	}

	// Delauney Triangulation
	if (delauney_test)
	{
		std::string input_file = R"(.\input\input_triangulation.txt)";
		delauney::Mesh tri(input_file);
	}

	return 0;
}
