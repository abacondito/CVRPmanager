#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <data_structures/node.h>


class Route
{
public:
    Route(double max_capacity,int routeIndex);
    bool addLinehaul(Node& linehaul);
    bool addBackhaul(Node& backhaul);
    void reverse();
    void addStartingPoint(Node& startingPoint);
    Node& getLastNode();
    Node& getNodeByIndex(size_t index);
    size_t getRouteSize();


    double getMax_capacity() const;
    void setMax_capacity(double value);

    void setNodeAtIndex(size_t index,Node& newNode);

    double getTotCost() const;

    double getCurrent_capacity_linehaul() const;
    void setCurrent_capacity_linehaul(double value);

    double getCurrent_capacity_backhaul() const;
    void setCurrent_capacity_backhaul(double value);

    int getRouteIndex() const;
    void setRouteIndex(int value);

protected:
    double totCost = 0.0;
    double max_capacity;
    double current_capacity_linehaul;
    double current_capacity_backhaul;
    std::vector<Node> nodes;
    int routeIndex;
};

#endif // ROUTE_H
