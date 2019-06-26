#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <data_structures/node.h>


class Route
{
public:
    Route(double max_capacity);
    bool addLinehaul(Node& linehaul);
    bool addBackhaul(Node& backhaul);
    void addStartingPoint(Node& startingPoint);
    Node& getLastNode();
    Node& getNodeByIndex(size_t index);
    size_t getRouteSize();


    double getMax_capacity() const;
    void setMax_capacity(double value);



    double getTotCost() const;

    double getCurrent_capacity_linehaul() const;
    void setCurrent_capacity_linehaul(double value);

    double getCurrent_capacity_backhaul() const;
    void setCurrent_capacity_backhaul(double value);

protected:
    double totCost = 0.0;
    double max_capacity;
    double current_capacity_linehaul;
    double current_capacity_backhaul;
    std::vector<Node> nodes;
};

#endif // ROUTE_H
