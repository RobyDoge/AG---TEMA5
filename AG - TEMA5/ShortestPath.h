#pragma once
#include <vector>
#include "Graph.h"
class ShortestPath
{
public:
	ShortestPath(const Graph& graph);
	std::vector<Graph::Node> GetPath(const Graph::Node& firstNode, const Graph::Node& secondNode);
private:
	void Dijkstra();

private:
	typedef std::pair<int, long> idNode_distanceClosestNode;
	struct Compare {
		bool operator()(const idNode_distanceClosestNode& lhs, const idNode_distanceClosestNode& rhs) const {
			return lhs.second > rhs.second;
		}
	};
private:
	std::vector<Graph::Node>m_path{};
	Graph m_graph{};
	std::vector<int> m_predecessor{};
	Graph::Node m_firstNode{};
	Graph::Node m_secondNode{};
};



