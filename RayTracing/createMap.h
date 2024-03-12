#ifndef createMap_H
#define createMap_H
#include <vector>
#include "Wall.h"

int createListWalls(std::vector<Wall> *listWalls);
int convertPowerFileToMbsFile(std::string fileName);
void createMbsFile(std::vector<std::vector<std::array<double, 3> > > matrix);

#endif
