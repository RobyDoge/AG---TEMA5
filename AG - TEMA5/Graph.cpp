#include "Graph.h"
#include <fstream>
#include <ranges>
#include <algorithm>
Graph::Graph(const std::string& filename)
{
	std::ifstream input(filename);
	std::string line;
	while (getline(input, line) && !line.contains("<nodes>")) {}
	ReadNodes(input);
	while (getline(input, line) && !line.contains("<arcs>")) {}
	ReadArches(input);

}

std::map<int, Graph::Coordinates> Graph::GetNodes() const
{
	return m_nodes;
}

void Graph::SetNodes(const std::map<int, Coordinates>& nodes)
{
	m_nodes = nodes;
}

std::map<Graph::Arch, int> Graph::GetArches() const
{
	return m_arches;
}

void Graph::SetArches(const std::map<Arch, int>& arches)
{
	m_arches = arches;
}

std::map<Graph::Node, std::vector<Graph::Node>> Graph::GetAdjacencyList() const
{
	return m_adjacencyList;
}

void Graph::SetAdjacencyList(const std::map<Node, std::vector<Node>>& adjacencyList)
{
	m_adjacencyList = adjacencyList;
}


void Graph::ReadNodes(std::ifstream& input)
{
	std::string line;
	
	while (getline(input, line) && !line.contains("</nodes>"))
	{
		// Extract attributes
		const auto id = std::stol(GetAttributeValueNode(line, "id"));
		const auto latitude = std::stol(GetAttributeValueNode(line, "latitude"));
		const auto longitude = std::stol(GetAttributeValueNode(line, "longitude"));
		m_nodes.insert({ id, {latitude, longitude} });
	}
}

void Graph::ReadArches(std::ifstream& input)
{
	std::string line;

	while (getline(input, line) && !line.contains("</arcs>"))
	{
		// Extract attributes
		const auto idFirstNode = std::stol(GetAttributeValueArch(line, "from"));
		const auto idSecondNode = std::stol(GetAttributeValueArch(line, "to"));
		const auto distance = std::stoi(GetAttributeValueArch(line, "length"));
		Node firstNode;
		if (m_nodes.contains(idFirstNode))
		{
			const auto coordinates = m_nodes.at(idFirstNode);
			firstNode = {idFirstNode, coordinates};
		}
		else { continue; }
		Node secondNode;
		if (m_nodes.contains(idSecondNode))
		{
			const auto coordinates = m_nodes.at(idSecondNode);
			secondNode = {idSecondNode, coordinates};
		}
		else { continue; }
		m_arches.insert({{firstNode, secondNode}, distance});

		if(m_adjacencyList.contains(firstNode))
		{
			m_adjacencyList.at(firstNode).emplace_back(secondNode);
		}
		else
		{
			m_adjacencyList.insert({firstNode, {secondNode}});
		}
	}
}

std::string Graph::GetAttributeValueNode(const std::string& xmlString, const std::string& attributeName) {
	size_t start = xmlString.find(attributeName + "=\"");
	if (start == std::string::npos) {
		return "";  // Attribute not found
	}

	start += attributeName.length() + 2;  // Move to the beginning of the attribute value
	const size_t end = xmlString.find('\"', start);

	if (end == std::string::npos) {
		return "";  // Closing quote not found
	}

	return xmlString.substr(start, end - start);
}

std::string Graph::GetAttributeValueArch(const std::string& xmlString, const std::string& attributeName) {
	size_t start = xmlString.find(attributeName + "=\"");
	if (start == std::string::npos) {
		return "";
	}
	start += attributeName.length() + 2;
	const size_t end = xmlString.find('\"', start);
	if (end == std::string::npos) {
		return "";
	}
	return xmlString.substr(start, end - start);
}