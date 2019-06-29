#include "drawable_routes.h"

Drawable_routes::Drawable_routes(const cg3::Pointd center,const double radius)
{
    this->center = center;
    this->radius = radius;
}

void Drawable_routes::draw() const {
    //draw each active triangle and his points

    std::array<cg3::Color,12> colors;

    colors[11] = cg3::Color(51,255,51);
    colors[10] = cg3::Color(0,76,153);
    colors[9] = cg3::Color(160,160,160);
    colors[8] = cg3::Color(204,0,102);
    colors[7] = cg3::Color(153,0,153);
    colors[6] = cg3::Color(0,153,153);
    colors[5] = cg3::Color(153,153,255);
    colors[4] = cg3::Color(102,255,255);
    colors[3] = cg3::Color(0,153,0);
    colors[2] = cg3::Color(255,153,153);
    colors[1] = cg3::Color(255,128,0);
    colors[0] = cg3::Color(204,204,0);


    for (size_t i = 0;i < this->getRoutes().size();i++) {

        cg3:: Color routeColor = colors[i];
        cg3:: Color nodeColor;

        Node previousNode = this->getRoutes()[0].getNodeByIndex(0);

        for (size_t j = 0; j < this->getRoutes()[i].getRouteSize();j++) {
            Node node = this->getRoutes()[i].getNodeByIndex(j);
            if(j == 0 || j == (this->getRoutes()[i].getRouteSize() -1)){
                nodeColor = cg3::Color(0,0,0);
            }
            else if(node.getDelivery() > 0){
                nodeColor = cg3::Color(0,0,255);

            }
            else {
                nodeColor = cg3::Color(255,0,0);
            }
            cg3::viewer::drawPoint2D(node.getCoordinates(),nodeColor,10);
            cg3::viewer::drawLine2D(previousNode.getCoordinates(),node.getCoordinates(),routeColor,1000000);
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


