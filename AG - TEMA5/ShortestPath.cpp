#include "ShortestPath.h"
#include <iostream>
#include <queue>
#include <QtWidgets/QMainWindow>
#include <fstream>

ShortestPath::ShortestPath(const Graph& graph)
{
	m_graph = graph;
	const auto idLastNode = m_graph.GetNodes().rbegin()->first;
	for (auto _ = 0; _ <= idLastNode; _++)
	{
		m_predecessor.push_back(0);
	}
}

std::vector<Graph::Node> ShortestPath::GetPath(const Graph::Node& firstNode, const Graph::Node& secondNode)
{
	/*m_firstNode = firstNode;
	m_secondNode = secondNode;*/

	m_firstNode = { 0,m_graph.GetNodes().at(0) };
	m_secondNode = { 4,m_graph.GetNodes().at(4) };

	Dijkstra();

	std::vector<Graph::Node> path;
	auto currentNode = secondNode;
	while(currentNode.id != firstNode.id)
	{
		path.push_back(currentNode);
		auto idNextNode = m_predecessor[currentNode.id];
		currentNode = { idNextNode,m_graph.GetNodes().at(idNextNode) };
	}
	path.push_back(firstNode);
	std::ranges::reverse(path);
	return path;
}

void ShortestPath::Dijkstra()
{
	std::vector distance(m_predecessor.size(), INT_MAX);
	std::vector<int> visited;
	auto compare = [](const idNode_distanceClosestNode& a, const idNode_distanceClosestNode& b) {
		return a.second > b.second;
		};

	std::priority_queue<idNode_distanceClosestNode, std::vector<idNode_distanceClosestNode>, decltype(compare)> distancePriority(compare);
	distance[m_firstNode.id] = 0;
	distancePriority.emplace(m_firstNode.id, 0);

	while(!distancePriority.empty())
	{
		
		const auto [idNewNode, distanceClosestNode] = distancePriority.top();
		distancePriority.pop();
		if (idNewNode == m_secondNode.id)
		{
			break;
		}
		if(std::ranges::find(visited,idNewNode)!= visited.end())
		{
			continue;
		}
		visited.push_back(idNewNode);

		const Graph::Node& firstNode{ idNewNode,m_graph.GetNodes().at(idNewNode) };
		const auto secondNodes = m_graph.GetAdjacencyList().at(firstNode);
		for(const auto& secondNode : secondNodes)
		{
			const auto cost = m_graph.GetArches().at({firstNode, secondNode});
			const auto maybeNewCost = distanceClosestNode + cost;

			const auto& oldCost = distance[secondNode.id];
			if (oldCost > maybeNewCost)
			{
				distance[secondNode.id] = maybeNewCost;
				distancePriority.emplace(secondNode.id, distance[secondNode.id]);
				m_predecessor[secondNode.id] = idNewNode;
			}
			
		}
	}

}

