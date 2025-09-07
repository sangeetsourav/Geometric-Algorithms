#pragma once

#include<cmath>
#include<map>
#include<vector>
#include<set>
#include<iostream>
#include<format>
#include<memory>

namespace geo {
	/**
	 * @brief Represents a 2D point and provides basic arithmetic operations and norm calculation.
	*/
	class Point2D {
	public:
		double x;
		double y;

		Point2D(double xCoord = 0, double yCoord = 0) : x{ xCoord }, y{ yCoord } {}

		Point2D operator-(const Point2D& other) const {
			return Point2D(x - other.x, y - other.y);
		}

		Point2D operator+(const Point2D& other) const {
			return Point2D(x + other.x, y + other.y);
		}

		Point2D operator/(double k) const {
			return Point2D(x / k, y / k);
		}

		Point2D mult(double k) const {
			return Point2D(x * k, y * k);
		}

		double dot(const Point2D& other) const {
			return x * other.x + y * other.y;
		}

		double cross(const Point2D& other) const {
			return x * other.y - y * other.x;
		}

		double norm() const {
			return std::sqrt(x * x + y * y);
		}

		void set(double xCoord, double yCoord) {
			x = xCoord;
			y = yCoord;
		}
	};

	namespace mesh {
		/**
		 * @brief A half-edge data structure for representing and manipulating polygonal meshes.
		 * 
		 * This class encapsulates the half-edge representation of a mesh, including vertices, edges, faces, and half-edges.
		 * It provides methods for constructing the mesh from a set of points and faces. This implementation uses smart pointers for memory management.
		*/
		class HalfEdgeDataStructure
		{
			// Forward declarations
			class Vertex;
			class Face;
			class Edge;

			class HalfEdge {
			public:
				std::shared_ptr<Vertex> origin;
				std::shared_ptr<HalfEdge> twin;
				std::shared_ptr<HalfEdge> next;
				std::shared_ptr<Face> face;
			};

			class Vertex {
			public:
				// Using a weak_ptr since the Point2D is owned elsewhere but we want prevent dangling pointers when the Point2D object is deleted
				std::weak_ptr<const Point2D> position;
				std::shared_ptr<HalfEdge> incident_half_edge;

				Vertex(std::shared_ptr<Point2D> &pos) : position(pos), incident_half_edge(nullptr) {}
			};

			class Edge {
			public:
				std::shared_ptr<HalfEdge> half_edge;
			};

			class Face {
			public:
				std::shared_ptr<HalfEdge> half_edge;
			};

			std::map<int, std::shared_ptr<Vertex>> m_vertices;
			std::map<int, std::shared_ptr<Face>> m_faces;
			std::map<int, std::shared_ptr<Edge>> m_edges;
			std::map<int, std::shared_ptr<HalfEdge>> m_half_edges;

		public:
			HalfEdgeDataStructure() = default;

			HalfEdgeDataStructure(std::map<int, std::shared_ptr<geo::Point2D>> &points, std::map<int, std::vector<int>> &faces)
			{
				// Initialize vertices
				for (auto p: points)
				{
					m_vertices.emplace(p.first, std::make_shared<Vertex>(p.second));
				}

				//for (const auto f: faces )
				//{
				//	for (size_t i = 0; i < f.second.size(); i++)
				//	{
				//		if (i!=f.second.size()-1)
				//		{

				//		}
				//	}
				//}

			}
		};
	}

}