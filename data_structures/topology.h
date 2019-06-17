#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <vector>
#include <data_structures/node.h>

class Topology
{

public:
    Topology();

    int getNode_num() const;
    void setNode_num(int value);

    int getVehicle_num() const;
    void setVehicle_num(int value);

    void addNode(Node node);

    int getCapacity() const;
    void setCapacity(int value);

protected:
    int vehicle_num=0;
    int node_num=0;
    int capacity=0;
    std::vector<Node> nodes;
};

#endif // TOPOLOGY_H
