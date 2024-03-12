#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "createMap.h"
#include "Wall.h"

/**
 * Fonction qui remplit la liste des murs a l'aide d'un fichier .txt contenant les
 * caracteristiques des murs.
 * @param listWalls : liste des murs, passee par reference pour etre modifiee
 * @return un entier qui informe de l'ouverture correcte du fichier
 */
int createListWalls(std::vector<Wall> *listWalls) {
    //ouverture du fichier
    std::ifstream file;
    file.open ("wall.txt");
    // Verifie que l'ouverture s'est deroulee correctement
    if(file)
    {
        // stocke chacunes des lignes du fichier
        std::string line;
        // continue tant qu'il y'a encore une ligne a lire
        while(getline(file, line))
        {
            // liste des caracteristique d'un mur
            std::string list[7];
            double listCaracteristcs[7];
            // iterateur
            int i = 0;
            for (const char &c: line){
                if(c != '\\'){
                    list[i] += c;
                }
                else{
                    listCaracteristcs[i] = stod(list[i]);
                    i++;
                }
            }
            // positions du mur
            double positions[4] = {listCaracteristcs[0], listCaracteristcs[2], listCaracteristcs[1], listCaracteristcs[3]};
            // creation du mur
            Wall newWall(positions, listCaracteristcs[4], listCaracteristcs[5], listCaracteristcs[6]);
            // ajout a la liste des murs
            (*listWalls).push_back(newWall);
        }
        return 1;
    }
    else
    {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
        return 0;
    }
    file.close();
}

int convertPowerFileToMbsFile(std::string fileName) {

    // matrice contenant toutes les cell
    std::vector<std::vector<std::array<double, 3> > > matrix;
    std::ifstream file;
    file.open (fileName);

    // Verifie que l'ouverture s'est deroulee correctement
    if(file)
    {
        bool newSet = true;
        // stocke chacunes des lignes du fichier
        std::string line;
        // iterateur
        int index = 0;
        // continue tant qu'il y'a encore une ligne a lire
        while(getline(file, line))
        {
            if(newSet) {
                newSet = false;
                std::vector<std::array<double, 3> > vec;
                matrix.push_back(vec);
            }
            // liste des caractéristique d'un mur
            std::string list[3];
            double listCaracteristcs[3];
            // iterateur
            int i = 0;
            for (const char &c: line)
            {
                if(c != '\t')
                {
                        list[i] += c;
                }
                else
                {
                    i++;
                }
            }

            if(line != "") {
                // on convertit les nombres récupérés en double
                for (int n = 0; n < 3; n++) {
                    listCaracteristcs[n] = stod(list[n]);
                }
                // on ajoute les valeurs à la matrice
                matrix[index].push_back((std::array<double, 3>){listCaracteristcs[0], listCaracteristcs[1], listCaracteristcs[2]});
            } else {
                newSet = true;
                index ++;
            }
        }

        // crée le fichier contenant les mb/s
        createMbsFile(matrix);
        file.close();
        return 1;
    }
    else
    {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
        return 0;
    }
}

void createMbsFile(std::vector<std::vector<std::array<double, 3> > > matrix) {
    std::ofstream file;
    file.open ("dataPowerMbs.txt");

    // Verifie que l'ouverture s'est deroulee correctement
    if(file) {
        for(int i = 0; i < matrix.size(); i++) {
            for(int j = 0; j < matrix[i].size(); j++) {
                // on convertit les watts en Mb/s
                double bitRate;
                if(matrix[i][j][2] > -73.0) {
                    bitRate = 320;
                } else if(matrix[i][j][2] < -82.0) {
                    bitRate = 40;
                } else {
                    double alpha = (320.0 - 40.0) / (-73.0 + 82.0);
                    bitRate = alpha*(matrix[i][j][2] + 73.0) + 320.0;
                }
                file << matrix[i][j][0] << "\t" << matrix[i][j][1] << "\t" << bitRate << "\n";
            }
            file << "\n";
        }
    } else {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
    }
}



