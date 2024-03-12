#include <iostream>
#include <fstream>
#include <vector>
#include "Map.h"
#include "Cell.h"

/**
 * Constructeur
 * @param pDiscretisation : precision choisie pour la discretisation
 * @param pWidth : largeur
 * @param pHeight : longueur
 */
Map::Map(double pDiscretisation, double pWidth, double pHeight) {
    // precision choisie pour la discrétisation
    discretisation = pDiscretisation;
    // largeur de la map
    width = pWidth;
    // longueur de la map
    height = pHeight;
    createMap();
}
/**
 * Cree la map en ajoutant un recepteur a chaque element de la matrice
 */
void Map::createMap() {
    // nbre de lignes / colonnes
    nbRow = floor((height/discretisation)) + 1;
    nbCol = floor((width/discretisation)) + 1;
    // pour chaque element de discretisation, on ajoute une cell
    for(int i = 0; i < nbRow; i++) {
        std::vector<Cell> vec;
        matrix.push_back(vec);
        for(int j = 0; j < nbCol; j++) {
            // position du recepteur
            double pPosition[2] = {j*discretisation, i*discretisation};
            Cell cell = Cell(pPosition);
            matrix[i].push_back(cell);
        }
    }
}
/**
 * Ouvre un fichier .txt pour y stocker les donnees de puissance en fonction de la position
 */
void Map::createMapFile() {
    std::ofstream file;
    file.open ("dataPower.txt");
    // Verifie que l'ouverture s'est deroulee correctement
    if(file) {
        for (int j = 0; j < nbCol; j++) {
            for(int i = 0; i < nbRow; i++) {
                file << matrix[i][j].position[0] << "\t" << matrix[i][j].position[1] << "\t" << matrix[i][j].P_dbm << std::endl;
            }
            file << "\n";
        }
    } else {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
    }
}

