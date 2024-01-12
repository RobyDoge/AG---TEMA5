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
	Dijkstra();

}

std::vector<Graph::Node> ShortestPath::GetPath(const Graph::Node firstNode, const Graph::Node secondNode) const
{
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
	std::vector distance(m_predecessor.size(),-1);
	std::vector visited(m_predecessor.size(), false);
	std::priority_queue<idNode_distanceClosestNode, std::vector<idNode_distanceClosestNode>, Compare> distancePriority;
	auto currentNode = *m_graph.GetNodes().begin();
	distance[currentNode.first] = 0;
	distancePriority.emplace(currentNode.first, 0);

	while(!distancePriority.empty())
	{
		
		const auto [idNewNode, distanceClosestNode] = distancePriority.top();
		distancePriority.pop();

		if(visited[idNewNode])
		{
			continue;
		}
		visited[idNewNode] = true;


		/*for(const auto& [firstNode,secondNode] : m_graph.GetArches() | std::views::keys)*/
		Graph::Node firstNode{ idNewNode,m_graph.GetNodes().at(idNewNode) };
		auto secondNodes = m_graph.GetAdjacencyList().at(firstNode);
		for(const auto& secondNode : secondNodes)
		{
			if (distance[secondNode.id] == -1 || distance[secondNode.id] > distance[idNewNode] + m_graph.
				GetArches().at({firstNode, secondNode}))
			{
				distance[secondNode.id] = distance[idNewNode] + m_graph.GetArches().at({firstNode, secondNode});
				distancePriority.emplace(secondNode.id, distance[secondNode.id]);
				m_predecessor[secondNode.id] = idNewNode;
			}
			
		}
	}
	SavePredecessor();

}

void ShortestPath::SavePredecessor() const
{
	std::ofstream output("output.txt");
	for(long i=0;i<m_predecessor.size();i++)
	{
		output << i << " "<<m_predecessor[i]<<"\n";
	}
}
