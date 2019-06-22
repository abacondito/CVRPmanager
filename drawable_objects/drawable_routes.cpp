#include "drawable_routes.h"

Drawable_routes::Drawable_routes(const cg3::Pointd center,const double radius)
{
    this->center = center;
    this->radius = radius;
}

void Drawable_routes::draw() const {
    //draw each active triangle and his points



    for (size_t i = 0;i < this->getRoutes().size();i++) {

        Node previousNode = this->getRoutes()[0].getNodeByIndex(0);

        for (size_t j = 0; j < this->getRoutes()[i].getRouteSize();j++) {
            Node node = this->getRoutes()[i].getNodeByIndex(j);
            cg3::viewer::drawPoint2D(node.getCoordinates(),cg3::Color(),15);
            cg3::viewer::drawLine2D(previousNode.getCoordinates(),node.getCoordinates(),cg3::Color(),7);
            previousNode = node;

        }
    }
}

/**
 * @brief gets the scene center
 * @return the scene center
 */

cg3::Pointd Drawable_routes::sceneCenter() const {
    return this->center;
}

/**
 * @brief gets the scene radius
 * @return the scene radius
 */

double Drawable_routes::sceneRadius() const {
    return this->radius;
}


