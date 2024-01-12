#pragma once
#include "Graph.h"
#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include "ui_CityMap.h"
#include "ShortestPath.h"
#include <memory>
class CityMap : public QMainWindow
{
    Q_OBJECT

public:
    CityMap(QWidget *parent = nullptr);
    ~CityMap();
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    void CreateBorderValues();
    void ScaleNodeCoordinates() const;
    Graph::Node&& FindClosestNode(const int x,const int y) const;

private:
    Ui::CityMapClass ui;
	std::unique_ptr<Graph> m_graph;
    Graph::Node m_firstNode{};
    Graph::Node m_secondNode{};
    bool m_firstNodeSelected = false;
    bool m_secondNodeSelected = false;
    std::unique_ptr<ShortestPath> m_paths;
    std::vector<Graph::Node> m_shortestPath;
    std::map<int, Graph::Coordinates> m_originalNodes;
    long m_minLatitude;
    long m_maxLatitude;
    long m_minLongitude;
    long m_maxLongitude;

    
};
