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

void Topology::addNode(Node node)
{
    this->nodes.push_back(node);
}

int Topology::getCapacity() const
{
    return capacity;
}

void Topology::setCapacity(int value)
{
    capacity = value;
}
