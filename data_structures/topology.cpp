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


double Topology::getCapacity() const
{
    return capacity;
}

void Topology::setCapacity(double value)
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

void Topology::addLinehaulNode(Node& node){
    this->linehaulNodes.push_back(node);
}

void Topology::addBackhaulNode(Node& node){
    this->backhaulNodes.push_back(node);
}

