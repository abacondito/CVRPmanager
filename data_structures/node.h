#ifndef NODE_H
#define NODE_H
#include <cg3/core/cg3/geometry/2d/point2d.h>

class Node
{
public:
    Node();

    cg3::Point2Dd getCoordinates() const;
    void setCoordinates(const cg3::Point2Dd &value);

    int getDelivery() const;
    void setDelivery(int value);

    int getPickup() const;
    void setPickup(int value);

    size_t getIndex() const;
    void setIndex(const size_t &value);

protected:
    cg3::Point2Dd coordinates;
    int delivery = 0;
    int pickup = 0;
    size_t index;
};

#endif // NODE_H
