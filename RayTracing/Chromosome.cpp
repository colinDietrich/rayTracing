#include "Chromosome.h"
#include <vector>
#include <array>

/**
 * Constructeur
 * @param pNbSources : nombre de sources que possède le chromosome
 * @param pPositions
 */
Chromosome::Chromosome(int pNbSources, std::vector<std::array<double,2> > pPositions) {
    // nombre de source que possède le chromosome
    nbSources = pNbSources;
    // zone de couverture totale du chromosome
    area = 0;
    // intersection entre les sources du chromosome
    overlap = 0;
    // positions des sources du chromosome
    positions = pPositions;
}

/**
 * cette fonction met a jour la zone de couverture et l'overlap du chromosome
 * @param pPosition : position en laquel la puissance est superieure a -82[dbm]
 * @param pArea : aire ajoutee
 */
void Chromosome::updateCoverage(std::array<double,2>  pPosition, double pArea) {
    // booleen -> true si la cellule n'est pas deja couverte par une autre source du chromosome
    bool check = true;

    // on verifie que les autres sources du chromosomes ne couvre pas deja cette cellule
    for(int i = 0; i < listCells.size(); i++) {
        if (listCells[i][0] == pPosition[0] && listCells[i][1] == pPosition[1]) {
            check = false;
            overlap += 1;
        }
    }
    // sinon on ajoute l'aire a la zone de couverture du chromosome
    if(check) {
        area += 1;
        listCells.push_back(pPosition);
    }
}