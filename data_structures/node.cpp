#include "node.h"

Node::Node()
{

}

cg3::Point2Dd Node::getCoordinates() const
{
    return coordinates;
}

void Node::setCoordinates(const cg3::Point2Dd &value)
{
    coordinates = value;
}

int Node::getDelivery() const
{
    return delivery;
}

void Node::setDelivery(int value)
{
    delivery = value;
}

int Node::getPickup() const
{
    return pickup;
}

void Node::setPickup(int value)
{
    pickup = value;
}

size_t Node::getIndex() const
{
    return index;
}

void Node::setIndex(const size_t &value)
{
    index = value;
}
