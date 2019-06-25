#ifndef CVRPUTILS_H
#define CVRPUTILS_H

#include <data_structures/node.h>
#include <cg3/data_structures/arrays/array2d.h>
#include <vector>
#include <data_structures/routes.h>
#include <list>
#include <iostream>
#include <fstream>

void computeDistTable(const std::vector<Node>& nodes, cg3::Array2D<double>& table);
void computeSaveTable(const cg3::Array2D<double>& distTable, cg3::Array2D<double>& saveTable);
void computeSaveList(cg3::Array2D<double>& saveTable, std::list<std::array<size_t,2>>& saveList);

void writeOnFile(Routes& routes);
std::array<size_t,2> getMaxIndexes(cg3::Array2D<double> matrix);
void eraseFromSaveListByItem(std::list<std::array<size_t,2>>& saveList,size_t index);

#endif // CVRPUTILS_H
