#include "topology.h"

Topology::Topology()
{

}

int Topology::getNode_num() const
{
    return node_num;
}

void Topology::setNode_num(int value)
{
    node_num = value;
}

int Topology::getVehicle_num() const
{
    return vehicle_num;
}

void Topology::setVehicle_num(int value)
{
    vehicle_num = value;
}


int Topology::getCapacity() const
{
    return capacity;
}

void Topology::setCapacity(int value)
{
    capacity = value;
}

std::vector<Node> Topology::getLinehaulNodes() const
{
    return linehaulNodes;
}

void Topology::setLinehaulNodes(const std::vector<Node> &value)
{
    linehaulNodes = value;
}

std::vector<Node> Topology::getBackhaulNodes() const
{
    return backhaulNodes;
}

void Topology::setBackhaulNodes(const std::vector<Node> &value)
{
    backhaulNodes = value;
}

