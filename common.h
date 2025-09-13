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
				std::shared_ptr<Vertex> origin{ nullptr };
				std::shared_ptr<HalfEdge> twin{ nullptr };
				std::shared_ptr<HalfEdge> next{ nullptr };
				std::shared_ptr<Face> face{ nullptr };
			};

			class Vertex {
			public:
				// Using a weak_ptr since the Point2D is owned elsewhere but we want prevent dangling pointers when the Point2D object is deleted
				std::weak_ptr<const Point2D> position;
				std::shared_ptr<HalfEdge> incident_half_edge{ nullptr };

				Vertex(std::shared_ptr<Point2D> &pos) : position(pos), incident_half_edge(nullptr) {}
			};

			class Edge {
			public:
				std::shared_ptr<HalfEdge> half_edge{ nullptr };
			};

			class Face {
			public:
				std::shared_ptr<HalfEdge> half_edge{ nullptr };
			};

			std::map<int, std::shared_ptr<Vertex>> m_vertices;
			std::map<int, std::shared_ptr<Face>> m_faces;
			std::map<std::pair<int, int>, std::shared_ptr<Edge>> m_edges;

		public:
			HalfEdgeDataStructure() = default;

			HalfEdgeDataStructure(std::map<int, std::shared_ptr<geo::Point2D>> &points, std::map<int, std::vector<int>> &faces)
			{
				// Initialize vertices
				for (auto p: points)
				{
					m_vertices.emplace(p.first, std::make_shared<Vertex>(p.second));
				}

				for (const auto f: faces )
				{
					for (auto it = f.second.begin(); it != f.second.end(); it++)
					{
						std::vector<int>::const_iterator vertex = it;
						std::vector<int>::const_iterator previous_vertex;
						std::vector<int>::const_iterator next_vertex;
						std::vector<int>::const_iterator next_to_next_vertex;

						// Previous vertex
						if (vertex == f.second.begin())
						{
							previous_vertex = std::prev(f.second.end());
						}
						else
						{
							previous_vertex = std::prev(vertex);
						}

						// Next vertex
						if (std::next(vertex)==f.second.end())
						{
							next_vertex = f.second.begin();
						}
						else
						{
							next_vertex = std::next(vertex);
						}

						// Next to next vertex
						if (std::next(next_vertex) == f.second.end())
						{
							next_vertex = f.second.begin();
						}
						else
						{
							next_vertex = std::next(it);
						}

						auto edge_key = std::make_pair(*vertex, *next_vertex);
						auto reverse_edge_key = std::make_pair(*next_vertex, *vertex);

						m_edges.emplace(edge_key, std::make_shared<Edge>());
						m_edges[reverse_edge_key] = m_edges[edge_key];

						// Initialise the half edges
						m_edges.at(edge_key)->half_edge->origin = m_vertices.at(*vertex);
						m_edges.at(edge_key)->half_edge->twin = m_edges.at(reverse_edge_key)->half_edge;

						m_edges.at(reverse_edge_key)->half_edge->origin = m_vertices.at(*next_vertex);
						m_edges.at(reverse_edge_key)->half_edge->twin = m_edges.at(edge_key)->half_edge;

						// Store the face
						m_faces.emplace(f.first, std::make_shared<Face>());

						// Store the half edge only the first time
						if (m_faces.at(f.first)->half_edge == nullptr)
						{
							m_faces.at(f.first)->half_edge = m_edges.at(edge_key)->half_edge;
						}
					}
				}
			}
		};
	}
}