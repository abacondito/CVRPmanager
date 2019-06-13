#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <vector>
#include <cg3/geometry/2d/point2d.h>

namespace FileUtils {
    std::vector<cg3::Point2Dd> getPointsFromFile(const std::string& filename);
    void generateRandomPointFile(
            const std::string& filename,
            double limit,
            int n);
}

#endif // FILEUTILS_H
