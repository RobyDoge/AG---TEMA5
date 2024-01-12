#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

class Graph
{
public:
	struct Coordinates
	{
		long latitude;
		long longitude;
	};
	struct Node {
		long id;
		Coordinates coordinates;
		bool operator<(const Node& other) const {
			if (id < other.id) return true;
			return false;
		}
	};
	struct Arch
	{
		Node firstNode;
		Node secondNode;
		bool operator<(const Arch& other) const {
			if (firstNode.id< other.firstNode.id) return true;
			if (firstNode.id > other.firstNode.id) return false;
			return secondNode.id < other.secondNode.id;
		}
	};
	
public:
	Graph(const std::string& filename);
	Graph() =default;
	std::map<int, Coordinates> GetNodes() const;
	void SetNodes(const std::map<int, Coordinates>& nodes);
	std::map<Arch, int> GetArches() const;
	void SetArches(const std::map<Arch, int>& arches);
	std::map<Node, std::vector<Node>> GetAdjacencyList() const;
	void SetAdjacencyList(const std::map<Node, std::vector<Node>>& adjacencyList);

private:
	void ReadNodes(std::ifstream& input);
	void ReadArches(std::ifstream& input);

	static std::string GetAttributeValueNode(const std::string& xmlString, const std::string& attributeName);
	static std::string GetAttributeValueArch(const std::string& xmlString, const std::string& attributeName);

private:
	//{NodeID} -> {Latitude, Longitude}
	std::map<int,Coordinates> m_nodes;
	//{FirstNodeID, SecondNodeID} -> {Distance}
	std::map<Arch,int> m_arches;
	std::map<Node,std::vector<Node>> m_adjacencyList;
};

