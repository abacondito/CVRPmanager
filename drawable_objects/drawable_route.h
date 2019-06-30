#ifndef DRAWABLE_ROUTE_H
#define DRAWABLE_ROUTE_H


#include <cg3/viewer/interfaces/drawable_object.h>
#include <cg3/utilities/color.h>
#include <cg3/viewer/renderable_objects/2d/renderable_objects2d.h>
#include <data_structures/route.h>


class Drawable_route  : public Route, public cg3::DrawableObject
{
public:
    Drawable_route(double max_capacity,int routeIndex,const cg3::Pointd center,const double radius);
    void draw() const;
    cg3::Pointd sceneCenter() const;
    double sceneRadius() const;
protected:
    cg3::Pointd center;
    double radius;
};


#endif // DRAWABLE_ROUTE_H
