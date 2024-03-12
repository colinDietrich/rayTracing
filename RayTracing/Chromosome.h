#ifndef COLIN_CPP_CHROMOSOME_H
#define COLIN_CPP_CHROMOSOME_H

#include <vector>
#include <array>


class Chromosome {
public:
    // liste des positions des sources disposées sur la map
    std::vector<std::array<double,2> > positions;
    // liste des positions couvertes par la source
    std::vector<std::array<double,2> > listCells;
    // nombre de sources dans le chromosomes
    int nbSources;
    // aire totale couverte
    double area;
    // nombre de récepteurs captant plusieurs sources
    double overlap;

    Chromosome(int pNbSources = 0, std::vector<std::array<double,2> > pPositions = std::vector<std::array<double,2> >());
    void updateCoverage(std::array<double,2> , double pArea);

};


#endif
