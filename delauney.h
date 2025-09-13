#pragma once

#include "common.h"
#include <map>
#include <string>
#include <iostream>
#include <format>
#include <fstream>
#include <sstream>
#include <memory>
#include <spdlog/spdlog.h>

namespace delauney {
	class Mesh {
		// Map containing point IDs and their corresponding Point2D objects. We use shared_ptr instead of directly storing Point2D objects as
		// the mesh data structures like vertex would use a pointer to a Point2D object and we would like to prevent dangling pointers when a
		// Point2D is deleted from the map. Using shared_ptr makes it possible to use weak_ptr in the vertex class which avoids dangling pointers.
		std::map<int, std::shared_ptr<geo::Point2D>> points;

		// Map containing face IDs and a list of point IDs
		std::map<int, std::vector<int>> faces;

        // Half-edge data structure representing the mesh topology and connectivity
		geo::mesh::HalfEdgeDataStructure mesh;

	public:
		/**
		 * @brief Constructs a Mesh object by reading mesh data from a specified file.
				  The file should contain the number of points and faces, followed by point and face definitions. 
				  Points are stored as 2D coordinates, and faces are defined by point indices.
		 * @param file The path to the mesh file to be loaded.
		 */
		Mesh(std::string file)
		{
			spdlog::info(std::format("Attempting to open file {}", file));

			std::ifstream inputFile(file);
			if (!inputFile.is_open()) {
				spdlog::error("Could not open file.");
				return;
			}
			else
			{
				spdlog::info("Opened file successfully.");
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

					points.emplace(id, std::make_shared<geo::Point2D>(x, y));
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