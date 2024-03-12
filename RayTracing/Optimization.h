#ifndef COLIN_CPP_OPTIMIZATION_H
#define COLIN_CPP_OPTIMIZATION_H

#include <vector>
#include "Wall.h"
#include "Beam.h"
#include "Graphics.h"
#include "Source.h"
#include "Chromosome.h"

class Optimization {

public:
    std::vector<Wall> listWalls;
    Graphics graph;
    int nbRef;
    int discretisation;
    double width;
    double height;
    // numéro de la génération
    int generation;
    // liste des différentes générations de chromosomes
    std::vector<std::vector<Chromosome> > listChromosome;
    // crossoverRate
    double Pc;
    // mutationRate
    double Pm;
    // chromosome Optimal
    Chromosome best;
    // aire totale de la map en fonction de la discretistation
    double totalArea;

    Optimization(std::vector<Wall>  &pListWalls, int pNbRef, int pDiscretisation, double pWidth, double pHeight);
    void initialiseChromosome();
    void coverageArea(std::array<double,2> positionSourceInit, Chromosome *chromosome);
    double calculatePower(std::vector<std::vector<Source> > listSources, std::array<double,2>);
    std::vector<std::array<double,2> > slice(std::vector<std::array<double,2> > const &v, int m, int n);
    void uniformCrossover(Chromosome *chromosome1, Chromosome *chromosome2);
    void onePointCrossover(Chromosome *chromosome1, Chromosome *chromosome2);
    void mutation(Chromosome *chromosome);
    void copy(std::vector<std::array<double,2> > const &v1, std::vector<std::array<double,2> > const &v2, std::vector<std::array<double,2> > *vOut);
    Chromosome selection(Chromosome &chromosome1, Chromosome &chromosome2);
    void initialisation(int nbChromsome);
    void updateGeneration();
    void shuffle(std::vector<Chromosome> *listBis);
};


#endif //COLIN_CPP_OPTIMIZATION_H
