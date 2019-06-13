/**
 * @author    Stefano Nuvoli (stefano.nuvoli@gmail.com)
 * @copyright Stefano Nuvoli 2017.
 */
#ifndef TRIANGULATIONGEOMETRY_H
#define TRIANGULATIONGEOMETRY_H

#include <cg3/geometry/2d/point2d.h>
#include <cg3/data_structures/arrays/arrays.h>

namespace DelaunayTriangulation {

namespace Checker {

bool isPointLyingInCircle(const cg3::Point2Dd& a, const cg3::Point2Dd& b, const cg3::Point2Dd& c, const cg3::Point2Dd& p, bool includeEndpoints);

bool isDeulaunayTriangulation(const std::vector<cg3::Point2Dd>& points, const cg3::Array2D<unsigned int>& triangles);

}

}

#endif // TRIANGULATIONGEOMETRY_H
