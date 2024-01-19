#include "CityMap.h"
#include <QPainter>
#include <ranges>

CityMap::CityMap(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_graph = std::make_unique<Graph>("Harta_Luxemburg.xml");
	CreateBorderValues();
	m_originalNodes = m_graph->GetNodes();
	m_paths = std::make_unique<ShortestPath>(*m_graph);
}

CityMap::~CityMap()
{}

void CityMap::paintEvent(QPaintEvent* event)
{
	ScaleNodeCoordinates();
	QPainter painter(this);
	painter.setPen(Qt::black);
	painter.setBrush(Qt::black);
	auto nodes = m_graph->GetNodes();
	for (const auto& [firstNode, secondNode] : m_graph->GetArches() | std::views::keys)
	{
		const auto& [xFirstNode, yFirstNode] = nodes.at(firstNode.id);
		const auto& [xSecondNode, ySecondNode] = nodes.at(secondNode.id);
		painter.drawLine(xFirstNode, yFirstNode, xSecondNode, ySecondNode);
	}
	if(m_firstNodeSelected)
	{
		painter.setPen(Qt::red);
		painter.setBrush(Qt::red);
		const auto& [xFirstNode, yFirstNode] = nodes.at(m_firstNode.id);
		painter.drawEllipse(xFirstNode, yFirstNode, 10, 10);
	}
	if(m_secondNodeSelected)
	{
		painter.setPen(Qt::green);
		painter.setBrush(Qt::green);
		const auto& [xSecondNode, ySecondNode] = nodes.at(m_secondNode.id);
		painter.drawEllipse(xSecondNode, ySecondNode, 10, 10);
	}
	if(!m_shortestPath.empty())
	{
		painter.setPen(Qt::blue);
		painter.setBrush(Qt::blue);
		auto [xFirstNode, yFirstNode] = m_shortestPath[0].coordinates;
		for (const auto& [idSecondNode, coordinatesSecondNode] : m_shortestPath | std::views::drop(1))
		{
			auto [xSecondNode, ySecondNode] = coordinatesSecondNode;
			painter.drawLine(xFirstNode, yFirstNode, xSecondNode, ySecondNode);
		}
		m_firstNodeSelected = false;
		m_secondNodeSelected = false;
		m_shortestPath.clear();
	}
}

void CityMap::mousePressEvent(QMouseEvent* event)
{
	
	if(!m_firstNodeSelected)
	{
		m_firstNode = FindClosestNode(event->pos().x(), event->pos().y());
		m_firstNodeSelected = true;
		update();
	}
	else if(!m_secondNodeSelected)
	{
		m_secondNode = FindClosestNode(event->pos().x(), event->pos().y());
		m_secondNodeSelected = true;
		update();
	}
	else
	{
		m_shortestPath = m_paths->GetPath(m_firstNode, m_secondNode);
		update();
	}

}


void CityMap::CreateBorderValues()
{
	const auto& nodes = m_graph->GetNodes();
	m_minLatitude = std::ranges::min_element(nodes, [](const auto& first, const auto& second) {return first.second.latitude < second.second.latitude; })->second.latitude;
	m_maxLatitude = std::ranges::max_element(nodes, [](const auto& first, const auto& second) {return first.second.latitude < second.second.latitude; })->second.latitude;
	m_minLongitude = std::ranges::min_element(nodes, [](const auto& first, const auto& second) {return first.second.longitude < second.second.longitude; })->second.longitude;
	m_maxLongitude = std::ranges::max_element(nodes, [](const auto& first, const auto& second) {return first.second.longitude < second.second.longitude; })->second.longitude;
}

void CityMap::ScaleNodeCoordinates() const
{
	auto nodes = m_originalNodes;
	const auto latitudeDifference = m_maxLatitude - m_minLatitude;
	const auto longitudeDifference = m_maxLongitude - m_minLongitude;
	const auto width = this->width();
	const auto height = this->height();
	for (auto& [latitude, longitude] : nodes | std::views::values)
	{
		latitude = (latitude - m_minLatitude) * width / latitudeDifference;
		longitude = (longitude - m_minLongitude) * height / longitudeDifference;
	}
	m_graph->SetNodes(nodes);
}

Graph::Node&& CityMap::FindClosestNode(const int x, const int y) const
{
	const auto& nodes = m_graph->GetNodes();
	double closestDistance = std::numeric_limits<double>::max();
	long idClosestNode = 0;
	for (const auto& [id, coordinates] : nodes | std::views::drop(1))
	{
		const auto& [latitude, longitude] = coordinates;
		const auto distance = 
			std::sqrt(std::pow(latitude - x, 2) +
			std::pow(longitude - y, 2));
		if (distance < closestDistance)
		{
			closestDistance = distance;
			idClosestNode = id;
		}
	}
	return Graph::Node{idClosestNode, nodes.at(idClosestNode)};
}
