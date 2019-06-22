#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <data_structures/node.h>


class Route
{
public:
    Route();
    bool addLinehaul(Node& linehaul);
    bool addBackhaul(Node& backhaul);
    void addStartingPoint(Node& startingPoint);
    Node& getLastNode();
    Node& getNodeByIndex(size_t index);
    size_t getRouteSize();


    double getMax_capacity() const;
    void setMax_capacity(double value);

    double getCurrent_capacity() const;
    void setCurrent_capacity(double value);

protected:
    double totCost = 0.0;
    double max_capacity;
    double current_capacity;
    std::vector<Node> nodes;
};

#endif // ROUTE_H