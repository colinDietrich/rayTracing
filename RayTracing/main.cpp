#include <iostream>
#include <vector>
#include <array>
#include <complex>
#include "Wall.h"
#include "Beam.h"
#include "Graphics.h"
#include "createMap.h"
#include "Source.h"
#include "utils.h"
#include "Map.h"
#include "Optimization.h"

using namespace std;

int main()
{
    /** ----- DECLARATION DES VARIABLES ----- */

    // nombre de reflexions maximums
    int nbRef = 2;
    // classe qui gere la visualisation graphique
    Graphics graph;
    // on cree la liste des walls a partir du fichier .txt
    vector<Wall> listWalls; createListWalls(&listWalls);
    // liste des sources et sources images
    vector<vector<Source> > listSources;
    // nombre de sources initiales
    const int nbSourceinit = 4;
    // position des sources initiales
    double positionSourceInit[nbSourceinit*2] = {68.0, 66.0, 111.0, 78.0, 9.0, 14.0, 193.0, 34.0};

    // position du recepteur
    double pCell[2] = {40.0,20.0};
    Cell *cell = new Cell(pCell);
    // choix option
    int option = 1;

    /** ----- INITIALISATION ----- */

    // on initialise la liste des sources
    initialiselistSources(nbRef, nbSourceinit, positionSourceInit, &listSources);
    // on cree la liste des sources images
    createListSources(&listWalls, &listSources, nbRef);
    // on ajuste les impedances des murs à partir de la frequence de la source
    for(Wall &wall : listWalls) {
        wall.setImpedance(listSources[0][0].f);
        if(wall.sigma == 0.014) {
            graph.drawLine(wall.position[0], wall.position[1], wall.position[2], wall.position[3], 0);
        } else {
            graph.drawLine(wall.position[0], wall.position[1], wall.position[2], wall.position[3], 100);
        }
    }

    /** ----- CALCUL RAY-TRACING ----- */

    // L'option 0 calcule la puissance reçue par le recepteur pour les
    // differentes sources et affiche les rayons atteignant le recepteur
    if(option == 0) {
        // liste des faisceaux
        vector<Beam> listBeams;
        // affichage des sources et du recepteur
        for(int i = 0; i < nbSourceinit; i++){
            graph.addPoint(positionSourceInit[2*i],positionSourceInit[2*i+1],0);
        }
        graph.addPoint(pCell[0],pCell[1],1);
        // onde directe
        for(int i = 0; i < listSources[0].size(); i++) {
            listSources[0][i].directWave(cell, &listWalls, &listBeams, i);
        }
        // reflexions
        for(int i = 0; i < nbRef; i++) {
            rayTracing(cell, &listWalls, &listBeams, &listSources[i+1], i);
        }
        // calcul puissance captee et affichage des rayons
        for(Beam &beam : listBeams) {
            beam.electricField(listSources[0][0].beta, listSources[0][0].G, listSources[0][0].P_watt);
            cell->updatePower(listSources[0][0].R_a, listSources[0][0].h_e, beam.E);

            int typeOfBeam = beam.size-1;
            for(int j = beam.size-1; j > 0; j--) {
                graph.drawLine(beam.listPositions[j][0], beam.listPositions[j-1][0], beam.listPositions[j][1], beam.listPositions[j-1][1], typeOfBeam);
            }
        }
        std::cout << "power watt : " << cell->P_watt << std::endl;
        std::cout << "power dbm : " << cell->P_dbm << std::endl;
        std::cout << "débit binaire : " << cell->bitRate << std::endl;
        // affichage graphique
        graph.printGraph(200.0, 110.0);
    }

    // L'option 1 calcule et affiche la repartition spatiale de puissance
    // pour les différentes sources
    else if (option == 1) {
        //affichage sources et cell
        for(int i = 0; i < nbSourceinit; i++){
            graph.addPoint(positionSourceInit[2*i],positionSourceInit[2*i+1],0);
        }
        // map contenant toutes les cell
        Map map = Map(2, 200.0, 110.0);
        for(int i = 0; i < map.nbRow; i++) {
            for(int j = 0; j < map.nbCol; j++) {
                // liste des faisceaux
                vector<Beam> listBeams;
                // onde directe
                for(int h = 0; h < listSources[0].size(); h++) {
                    listSources[0][h].directWave(&map.matrix[i][j], &listWalls, &listBeams, h);
                }
                // reflexions
                for(int h = 0; h < nbRef; h++) {
                    rayTracing(&map.matrix[i][j], &listWalls, &listBeams, &listSources[h+1], h);
                }
                // calcul puissance captee
                for(Beam &beam : listBeams) {
                    beam.electricField(listSources[0][0].beta, listSources[0][0].G, listSources[0][0].P_watt);
                    (map.matrix[i][j]).updatePower(listSources[0][0].R_a, listSources[0][0].h_e, beam.E);
                }
            }
            // indicateur de progression (nbr de lignes effectues/nbr de lignes totales)
            std::cout << "iterarion : " << i << " / " << map.nbRow << std::endl;
        }
        // affichage graphique
        map.createMapFile();
        graph.printMap(map.width, map.height);
    }

    // L'option 2 selectionne un repartition optimale  de sources a l'aide d'un algorithme
    // genetique
    else if (option == 2) {
        // on initialise la position initiale de la generation pseudo-aleatoire
        srand (time(NULL));
        // nombre de chromosomes dans la 1ere generation
        int nbChromosomes = 256;
        // classe qui gere la selection et reproduction d'une generation
        Optimization opt(listWalls, 1, 5, 200, 110);
        // generation initiale
        opt.initialisation(nbChromosomes);
        // (log(nbChromosomes)/log(2)) generations
        while(opt.generation < log(nbChromosomes)/log(2)) {
            // indicateur de progression
            std::cout << "--- generation : " << opt.generation << std::endl;
            opt.updateGeneration();
        }
        std::cout << "---- END ---- " << std::endl;

        // repatition optimale
        for (int i = 0; i < opt.listChromosome[opt.generation][0].nbSources; i++) {
            std::cout << "source : " << i << std::endl;
            std::cout << " - posx : " << opt.listChromosome[opt.generation][0].positions[i][0] << std::endl;
            std::cout << " - posy : " << opt.listChromosome[opt.generation][0].positions[i][1] << std::endl;
        }

    }

    // conversion et affichage en Mbps
    else if (option == 3) {
        convertPowerFileToMbsFile("dataPowerFinal4.txt");
        graph.printMapMbps(200.0, 110.0);
    }

    return 0;
}
