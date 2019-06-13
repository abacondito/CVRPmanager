/**
 * @author    Stefano Nuvoli (stefano.nuvoli@gmail.com)
 * @copyright Stefano Nuvoli 2017.
 */
#include "delaunay_checker.h"

#include <Eigen/Dense>

namespace DelaunayTriangulation {

namespace Checker {

/**
 * @brief Check if a point lies inside the circle passing
 * for the three input points t1, t2, t3.
 * Note that the 3 points t1, t2, t3 (the 3 points in the circle)
 * must be in a counter-clockwise order.
 * @param[in] t1 First point of the circle
 * @param[in] t2 Third point of the circle
 * @param[in] t3 Second point of the circle
 * @param[in] p Input point
 * @param[in] includeEndpoints True if we want to include the endpoints
 * @return True if the point lies inside the circle
 */
bool isPointLyingInCircle(
        const cg3::Point2Dd& a,
        const cg3::Point2Dd& b,
        const cg3::Point2Dd& c,
        const cg3::Point2Dd& p,
        bool includeEndpoints)
{
    Eigen::Matrix4d A;

    A << a.x(), a.y(), a.x()*a.x() + a.y()*a.y(), 1,
            b.x(), b.y(), b.x()*b.x() + b.y()*b.y(), 1,
            c.x(), c.y(), c.x()*c.x() + c.y()*c.y(), 1,
            p.x(), p.y(), p.x()*p.x() + p.y()*p.y(), 1;

    if (includeEndpoints) {        
        return (A.determinant() >= -std::numeric_limits<double>::epsilon());
    }
    else {
        return (A.determinant() > 0);
    }
}

/**
 * @brief Check if the triangulation is a Delaunay triangulation
 * (brute force, O(n^2))
 * Note that each triangle must have the points ordered in a
 * counter-clockwise order.
 * @param[in] points Vector of points in the triangulation
 * @param[in] triangles Vector of triangles (represented by a vector
 * of 3 indices of the vector "points")
 * @return True if the triangulation is a Delaunay triangulation,
 * false otherwise
 */
bool isDeulaunayTriangulation(const std::vector<cg3::Point2Dd>& points, const cg3::Array2D<unsigned int>& triangles) {
    assert(triangles.getSizeY() == 3);

    unsigned int n = triangles.getSizeX();
    bool isDelaunay = true;

    #pragma omp parallel for
    for (unsigned int i = 0; i < n; i++) {
        //Get the points of the triangle
        const cg3::Point2Dd& a = points[triangles(i,0)];
        const cg3::Point2Dd& b = points[triangles(i,1)];
        const cg3::Point2Dd& c = points[triangles(i,2)];

        //For each point different by a, b, c
        for (const cg3::Point2Dd& p : points) {
            if (!isDelaunay) break;
            if (p != a && p != b && p != c) {
                //To be a delaunay triangulation, the point p
                //must be outside the circle passing through a, b, c
                if (isPointLyingInCircle(a,b,c,p,false)) {
                    isDelaunay = false;
                    break;
                }
            }
        }
    }
    return isDelaunay;
}


}

}
