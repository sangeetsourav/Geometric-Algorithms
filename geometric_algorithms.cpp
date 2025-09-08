// geometric_algorithms.cpp : Defines the entry point for the application.
//

#include "line_sweep.h"
#include "delauney.h"
#include <iostream>
#include <string>
#include <filesystem>
int main()
{	
	bool line_sweep_test = true;
	bool delauney_test = false;

	// Line Sweep
	if (line_sweep_test)
	{
		// filesystem makes the path construction cross-platform
		std::filesystem::path input_file = std::filesystem::path("input") / "line_sweep_input.txt";
		lineSweep::LineSweepAlgorithm lsa;
		lsa.loadSegmentsFromFile(input_file.string());
		lsa.sweep();
	}

	// Delauney Triangulation
	if (delauney_test)
	{
		// filesystem makes the path construction cross-platform
		std::filesystem::path input_file = std::filesystem::path("input") / "input_triangulation.txt";
		delauney::Mesh tri(input_file.string());
	}

	return 0;
}
