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
	//m_firstNode = firstNode;
	m_firstNode = Graph::Node(26800, m_graph.GetNodes().at(26800));
	//m_secondNode = secondNode;
	m_secondNode = Graph::Node(15735, m_graph.GetNodes().at(15735));

	Dijkstra();

	std::vector<Graph::Node> path;
	auto currentNode = secondNode;
	while(currentNode.id != firstNode.id)
	{
		path.push_back(currentNode);
		auto idNextNode = m_predecessor[currentNode.id];
		currentNode = Graph::Node(idNextNode,m_graph.GetNodes().at(idNextNode));
	}
	path.push_back(firstNode);
	std::ranges::reverse(path);
	return path;
}

void ShortestPath::Dijkstra()
{
	std::vector distance(m_predecessor.size(), INT_MAX);
	std::vector visited(m_predecessor.size(), false);
	std::priority_queue<idNode_distanceClosestNode, std::vector<idNode_distanceClosestNode>, Compare> distancePriority;
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
		if(visited[idNewNode])
		{
			
			continue;
		}
		visited[idNewNode] = true;

		Graph::Node firstNode{ idNewNode,m_graph.GetNodes().at(idNewNode) };
		auto secondNodes = m_graph.GetAdjacencyList().at(firstNode);
		for(const auto& secondNode : secondNodes)
		{
			const auto cost = m_graph.GetArches().at({firstNode, secondNode});
			const auto maybeNewCost = distanceClosestNode + cost;

			const auto oldCost = distance[secondNode.id];
			
			 
			if (oldCost == -1 || oldCost > maybeNewCost)
			{
				distance[secondNode.id] = maybeNewCost;
				distancePriority.emplace(secondNode.id, distance[secondNode.id]);
				m_predecessor[secondNode.id] = idNewNode;
			}
			
		}
	}

}

