// geometric_algorithms.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <cmath>
#include <list>
#include <set>
#include <format>
#include <fstream>
#include <sstream>
#include <optional>

namespace lineSweep {
	/**
 * @brief Represents a 2D point and provides basic arithmetic operations and norm calculation.
 */
	class Point {
	public:
		double x;
		double y;

		Point(double xCoord = 0, double yCoord = 0) : x{ xCoord }, y{ yCoord } {}

		Point operator-(const Point& other) const {
			return Point(x - other.x, y - other.y);
		}

		Point operator+(const Point& other) const {
			return Point(x + other.x, y + other.y);
		}

		Point operator/(double k) const {
			return Point(x / k, y / k);
		}

		Point mult(double k) const {
			return Point(x * k, y * k);
		}

		double dot(const Point& other) const {
			return x * other.x + y * other.y;
		}

		double cross(const Point& other) const {
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

	/**
	 * @brief Represents a 2D line segment and provides functionality for sweep line algorithms, including computing the segment's x-coordinate at a given sweep line position and comparison based on this value.
	 */
	class LineSegment {
	public:
		Point upper;
		Point lower;
		Point dir; // Direction vector from start to end
		double sweepYPosition{}; // Current position of the sweep line
		double xAtSweepY{}; // x-coordinate of the segment at the sweep line

		LineSegment(double x1, double y1, double x2, double y2)
		{
			if (y2 > y1)
			{
				upper.set(x2, y2);
				lower.set(x1, y1);
			}
			else if (y1 > y2)
			{
				upper.set(x1, y1);
				lower.set(x2, y2);
			}
			else
			{
				if (x1 < x2)
				{
					upper.set(x1, y1);
					lower.set(x2, y2);
				}
				else
				{
					upper.set(x2, y2);
					lower.set(x1, y1);
				}
			}

			dir = lower - upper;
		}

		void setSweepPosition(double y) {
			sweepYPosition = y;
			xAtSweepY = upper.x + dir.x * (sweepYPosition - upper.y) / dir.y;
		}

		bool operator<(const LineSegment& other) const {
			return xAtSweepY < other.xAtSweepY;
		}

		double orientation_test(const Point& p) const {
			return (lower - upper).cross(p - upper);
		}

		std::optional<Point> intersect(const LineSegment& other) const {

			double result1 = orientation_test(other.upper) * orientation_test(other.lower);
			double result2 = other.orientation_test(upper) * other.orientation_test(lower);

			if (result1 <= 0 and result2 <= 0)
			{
				double numer = (lower.x - upper.x) * (other.upper.y - upper.y) - (lower.y - upper.y) * (other.upper.x - upper.x);
				double denom = (lower.y - upper.y) * (other.lower.x - other.upper.x) - (other.lower.y - other.upper.y) * (lower.x - upper.x);

				double t = numer / denom;

				Point intersection = other.upper + other.dir.mult(t);

				return intersection;
			}
			else
			{
				return std::nullopt;
			}
		}
	};

	/**
	 * @brief Defines the types of events that can occur.
	 */
	enum class EventType {
		INTERSECTION,
		UPPER,
		LOWER
	};

	/**
	 * @brief Represents an event in the sweep line algorithm, containing information about the event type, associated line segments, and the event point.
	 */
	class Event {
	public:
		Point p;
		LineSegment* seg1;
		LineSegment* seg2; // nullptr if not an intersection
		EventType type;

		Event(EventType t, LineSegment* s1, LineSegment* s2 = nullptr, Point intersection_point = Point(0, 0))
			: type{ t }, seg1{ s1 }, seg2{ s2 } {

			if (t == EventType::INTERSECTION) {
				p = intersection_point;
			}
			else if (t == EventType::UPPER) {
				p = s1->upper;
			}
			else { // LOWER
				p = s1->lower;
			}
		}
	};

	struct SegmentComparator{
		bool operator()(const LineSegment* s1, const LineSegment* s2) const {
			return s1->xAtSweepY < s2->xAtSweepY;
		}
	};

	struct EventComparator {
		bool operator()(const Event& e1, const Event& e2) const {
			if (e1.p.y != e2.p.y)
			{
				return e1.p.y > e2.p.y; // Higher y first
			}
			else if (e1.p.x != e2.p.x)
			{
				return e1.p.x < e2.p.x; // Lower x first
			}
			else
			{
				return static_cast<int>(e1.type) < static_cast<int>(e2.type); // INTERSECTION < UPPER < LOWER
			}
		}
	};

	class LineSweepAlgorithm {
	public:
		// List instead of vector so that the iterators don't get invalidated while inserting new segments
		std::list < LineSegment > segments;
		std::set < LineSegment*, SegmentComparator > status;
		std::set < Event, EventComparator > eventQueue;
		double sweepY;

		void addLineSegment(double x1, double y1, double x2, double y2) {
			segments.emplace_back(x1, y1, x2, y2);
			LineSegment* seg = &segments.back();
			eventQueue.emplace(EventType::UPPER, seg);
			eventQueue.emplace(EventType::LOWER, seg);
		}

		void loadSegmentsFromFile(std::string file)
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
			while (std::getline(inputFile, line)) {
				double x1, y1, x2, y2;
				std::stringstream ss{ line };
				ss >> x1 >> y1 >> x2 >> y2;
				addLineSegment(x1, y1, x2, y2);
			}

			inputFile.close();

			std::cout << std::format("Loaded {} segments from file.\n", segments.size());
		}

		void handleUpperEvent(const Event& event)
		{
			event.seg1->setSweepPosition(sweepY);
			auto result = status.insert(event.seg1);

			if (result.second)
			{
				auto right_neighbour = std::next(result.first);

				if (right_neighbour!=status.end())
				{
					checkIntersections(event.seg1, *right_neighbour);
				}

				if (result.first!=status.begin())
				{
					auto left_neighbour = std::prev(result.first);
					checkIntersections(event.seg1, *left_neighbour);
				}
			}
		}

		void handleLowerEvent(const Event& event)
		{
			auto loc = status.find(event.seg1);

			if (loc != status.end())
			{
				auto right_neighbour = std::next(loc);

				if (right_neighbour != status.end() && loc != status.begin())
				{
					auto left_neighbour = std::prev(loc);
					checkIntersections(*left_neighbour, *right_neighbour);
				}
			}

			status.erase(loc);
		}

		void checkIntersections(LineSegment* s1, LineSegment* s2)
		{
			auto result = s1->intersect(*s2);

			// Only insert the intersection if it is below the sweep line
			if (result.has_value() && result.value().y < sweepY)
			{
				eventQueue.emplace(EventType::INTERSECTION, s1, s2, result.value());
			}
		}

		void handleIntersectionEvent(const Event& event)
		{
			// We remove the segments from the status first (DO NOT MODIFY the sweep position key while the elements are still in the set, this will make the internal tree structure invalid)
			status.erase(event.seg1);
			status.erase(event.seg2);

			// We modify the sweep position slightly so that we get position changes
			event.seg1->setSweepPosition(sweepY - 0.00001);
			event.seg2->setSweepPosition(sweepY - 0.00001);

			// We reinsert the segment and positions would be swapped now
			status.insert(event.seg1);
			status.insert(event.seg2);

			// Now we check for new intersections
			auto loc1 = status.find(event.seg1);
			auto loc2 = status.find(event.seg2);

			if (std::next(loc1)!=status.end() && std::next(loc1) != loc2)
			{
				auto neighbour = std::next(loc1);
				checkIntersections(*loc1, *neighbour);
			}

			if (loc1 != status.begin() && std::prev(loc1) != loc2)
			{
				auto neighbour = std::prev(loc1);
				checkIntersections(*loc1, *neighbour);
			}

			if (std::next(loc2) != status.end() && std::next(loc2) != loc1)
			{
				auto neighbour = std::next(loc2);
				checkIntersections(*loc2, *neighbour);
			}

			if (loc2 != status.begin() && std::prev(loc2) != loc1)
			{
				auto neighbour = std::prev(loc2);
				checkIntersections(*loc2, *neighbour);
			}
		}

		void sweep()
		{
			// Start going through the event queue
			int eventCount = 0;
			int intersectionCount = 0;
			while (!eventQueue.empty())
			{
				eventCount++;

				auto ev = eventQueue.extract(eventQueue.begin()).value(); // Get the event

				// Set sweep position
				sweepY = ev.p.y;

				std::cout << std::format("Event no. {}: Type {}\n", eventCount, static_cast<int>(ev.type));

				if (ev.type == EventType::UPPER)
				{
					handleUpperEvent(ev);
				}
				else if (ev.type == EventType::LOWER)
				{
					handleLowerEvent(ev);
				}
				else // INTERSECTION
				{
					handleIntersectionEvent(ev);
					intersectionCount++;
				}

			}

			std::cout << std::format("Total events processed: {}, Intersections found: {}\n", eventCount, intersectionCount);
		}
	};

}