#pragma once

#include "common.h"
#include <map>
#include <string>
#include <iostream>
#include <format>
#include <fstream>
#include <sstream>

namespace delauney {
	class Mesh {
		std::map<int, geo::Point2D> points;
		std::map<int, std::vector<int>> faces;
		geo::mesh::HalfEdgeDataStructure mesh;

	public:
		Mesh(std::string file)
		{
			std::cout << std::format("Attempting to open file {}\n", file);

			std::ifstream inputFile(file);
			if (!inputFile.is_open()) {
				std::cout << "Error: Could not open file.\n";
				return;
			}
			else
			{
				std::cout << "Opened file successfully.\n";
			}

			std::string line;
			int line_num = 0;

			double n_points{}, n_faces{};

			while (std::getline(inputFile, line)) {
				std::stringstream ss{ line };

				if (line_num == 0)
				{
					ss >> n_points >> n_faces;
				}
				else if (line_num <= n_points)
				{
					int id{};
					double x{}, y{};

					ss >> id >> x >> y;

					points.emplace(id, geo::Point2D{ x, y });
				}
				else
				{
					int id1{}, id2{}, id3{};

					ss >> id1 >> id2 >> id3;

					faces.emplace(line_num - n_points, std::vector<int>{ id1, id2, id3 });
				}

				line_num++;
			}

			inputFile.close();
		
			mesh = geo::mesh::HalfEdgeDataStructure(points, faces);
		}
	};
}