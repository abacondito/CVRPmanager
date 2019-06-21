#include "fileutils.h"

#include <fstream>
#include <random>
#include <iomanip>
#include <cg3/core/cg3/geometry/2d/point2d.h>

namespace FileUtils {

std::vector<cg3::Point2Dd> getPointsFromFile(const std::string& filename) {

    std::vector<cg3::Point2Dd> points;
    std::ifstream infile;
    infile.open(filename);

    int n;
    infile >> n;

    for (int i = 0; i < n; i++) {
        double x = 0.0;
        double y = 0.0;
        infile >> std::setprecision(10) >> x >> std::setprecision(10) >> y;
        cg3::Point2Dd p(x,y);
        points.push_back(p);
    }

    infile.close();

    return points;
}

void generateRandomPointFile(const std::string& filename, double limit, int n) {
    std::setprecision(10);
    std::ofstream outfile;
    outfile.open(filename);

    double lower_bound = -limit;
    double upper_bound = limit;

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_real_distribution<double> dist(lower_bound,upper_bound); // distribution in range [0, 100]

    outfile << n << std::endl;

    for (int i = 0; i < n; i++) {
        double x = dist(rng);
        double y = dist(rng);
        outfile << std::setprecision(10) << x << " " << std::setprecision(10) << y << std::endl;
    }

    outfile.close();
}

Topology getTopologyFromFile(const std::string& filename) {

    Topology topology = Topology();
    std::ifstream infile;
    infile.open(filename);

    int nNodes,nVehicles, aux, cap,delivery,pickup;
    infile >> nNodes;
    topology.setNode_num(nNodes);
    infile >> nVehicles;
    infile >> nVehicles;
    topology.setVehicle_num(nVehicles);

    double x = 0.0;
    double y = 0.0;
    infile >> std::setprecision(10) >> x >> std::setprecision(10) >> y >> aux >> cap;
    Node start = Node();
    start.setCoordinates(cg3::Point2Dd(x,y));
    start.setIndex(0);
    topology.setCapacity(cap);
    topology.addLinehaulNode(start);
    topology.addBackhaulNode(start);


    for (size_t i = 0; i < nNodes; i++) {


        infile >> std::setprecision(10) >> x >> std::setprecision(10) >> y >> delivery >> pickup >> aux;
        Node node = Node();
        node.setCoordinates(cg3::Point2Dd(x,y));
        node.setDelivery(delivery);
        node.setPickup(pickup);
        node.setIndex(i+1);
        if (delivery != 0.0){
            topology.addLinehaulNode(node);
        }
        else {
            topology.addBackhaulNode(node);
        }

    }

    infile.close();

    return topology;
}

}
