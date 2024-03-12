#ifndef COLIN_CPP_MAP_H
#define COLIN_CPP_MAP_H

#include <vector>
#include "Cell.h"


class Map {

public:
    // largeur de la map
    double width;
    // longueur de la map
    double height;
    // matrice contenant toutes les cell
    std::vector<std::vector<Cell> > matrix;
    // précision choisie pour la discrétisation
    double discretisation;
    // nbre de lignes / colonnes
    int nbRow;
    int nbCol;

    Map(double pDiscretisation, double pWidth, double pHeight);
    void createMap();
    void createMapFile();
};


#endif
