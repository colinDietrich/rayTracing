#ifndef COLIN_CPP_UTILS_H
#define COLIN_CPP_UTILS_H

#include <vector>
#include "Source.h"

void initialiselistSources(int nbRef, int nbSourceinit, double *positionSourceInit, std::vector<std::vector<Source> > *listSources);
void createListSources(std::vector<Wall> *listWalls, std::vector<std::vector<Source> > *listSources, int nbRef);
void rayTracing(Cell *cell, std::vector<Wall> *listWalls, std::vector<Beam> *listBeams, std::vector<Source> *listSources, int nR);
int backTracing(Beam *beam, std::complex<double> *coeff, Source image);

#endif
