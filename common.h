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
		class HalfEdgeDataStructure
		{
			// Forward declarations
			class Vertex;
			class Face;
			class Edge;

			class HalfEdge {
			public:
				std::unique_ptr<Vertex> origin;
				std::unique_ptr<HalfEdge> twin;
				std::unique_ptr<HalfEdge> next;
				std::unique_ptr<Face> face;
			};

			class Vertex {
			public:
				const Point2D* position;
				std::unique_ptr<HalfEdge> incident_half_edge;
				std::unique_ptr<Edge> edge;

				Vertex(const Point2D* pos) : position(pos), incident_half_edge(nullptr), edge(nullptr) {}
			};

			class Edge {
			public:
				std::shared_ptr<HalfEdge> half_edge;
			};

			class Face {
			public:
				HalfEdge* half_edge;
			};

			std::map<int, Vertex> m_vertices;
			std::map<int, Face> m_faces;
			std::map<int, Edge> m_edges;

		public:
			HalfEdgeDataStructure() = default;

			HalfEdgeDataStructure(std::map<int, geo::Point2D> &points, std::map<int, std::vector<int>> &faces)
			{
				// Initialize vertices
				for (const auto p: points)
				{
					m_vertices.emplace(p.first, Vertex(&p.second));
				}

				for (const auto f: faces )
				{
					for (auto it = f.second.begin(); it != f.second.end(); it++)
					{

					}
				}

			}
		};
	}

}