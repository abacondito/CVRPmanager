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

    double getCapacity() const;
    void setCapacity(double value);


    std::vector<Node> getLinehaulNodes() const;
    void setLinehaulNodes(const std::vector<Node> &value);

    std::vector<Node> getBackhaulNodes() const;
    void setBackhaulNodes(const std::vector<Node> &value);

    void addLinehaulNode(Node& node);
    void addBackhaulNode(Node& node);



protected:
    int vehicle_num=0;
    int node_num=0;
    double capacity=0;
    std::vector<Node> linehaulNodes;
    std::vector<Node> backhaulNodes;
};

#endif // TOPOLOGY_H
