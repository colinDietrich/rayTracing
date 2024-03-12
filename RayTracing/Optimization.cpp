#include <iostream>
#include <vector>
#include "Optimization.h"
#include "Wall.h"
#include "Beam.h"
#include "Source.h"
#include "utils.h"
#include "Chromosome.h"

/**
 * Constructeur
 * @param pListWalls : liste des murs sur la map
 * @param pNbRef : nombre de reflexons maximales
 * @param pDiscretisation : discretisation de la map
 * @param pWidth : largeur de la map
 * @param pHeight : hauteur de la map
 */
Optimization::Optimization(std::vector<Wall>  &pListWalls, int pNbRef, int pDiscretisation, double pWidth, double pHeight) {

    // liste des murs present sur la map
    listWalls = pListWalls;
    // nombre de reflexions maximales utilisees par l'algorithme
    nbRef = pNbRef;
    // largeur de la map
    width = pWidth;
    // hauteur de la map
    height = pHeight;
    // discretisation de la map
    discretisation = pDiscretisation;
    // probabilite pour le croisement uniforme
    Pc = 0.7;
    // probabilite pour la mutation
    Pm = 0.4;
    // numero de la generation
    generation = 0;
    // on calcul l'aire totale de la map en fonction de la discretistation
    double nbRow = floor((height/discretisation)) + 1;
    double nbCol = floor((width/discretisation)) + 1;
    totalArea = nbRow*nbCol;
}

/**
 * Cette fonction permet de passer d'une generation a une autre
 * cela ce fait en trois etapes :
 * le croisement, la mutation et la selection
 */
void Optimization::updateGeneration() {

    /* --- CROISEMENT --- */
    // probabilite pour le croisement a un point
    double Po = 1 - Pc;
    // conversion de la probabilite Pc en un nombre entier de chromosomes
    int nb_Pc = (int)(Pc*(double)(listChromosome[generation].size()));
    if(nb_Pc%2 == 1) { nb_Pc -= 1; }
    // conversion de la probabilite 1 - Pc en un nombre entier de chromosomes
    int anti_Pc = (int)(Po*(double)(listChromosome[generation].size()));
    if(anti_Pc%2 == 1) { anti_Pc += 1; }
    // conversion de la probabilite Pm en un nombre entier de chromosomes
    int nb_Pm = (int)(Pm*(double)(listChromosome[generation].size()));
    if(nb_Pc%2 != 1) { nb_Pc += 1; }

    // on realise le croisement uniforme sur les chromosomes
    for (int i = 0; i < nb_Pc/2; i++) {
        uniformCrossover(&listChromosome[generation][2*i], &listChromosome[generation][2*i+1]);
    }
    // on realise le croisement a un point sur les chromosomes
    for (int i = 0; i < anti_Pc/2; i++) {
        onePointCrossover(&listChromosome[generation][2*i], &listChromosome[generation][2*i+1]);
    }

    /* --- MUTATION --- */
    // on realise la mutation sur les chromosomes
    for (int i = 0; i < nb_Pm; i++) {
        mutation(&listChromosome[generation][i]);
    }

    // on melange l'ensemble des chromosomes avant de les selectionner
    shuffle(&listChromosome[generation]);

    /* --- HELITISME --- */
    // on reporte le meilleur chromosome de la generation precedente
    // a une position aleatoire dans la generation suivante
    int posRand = rand() % (listChromosome[generation].size());
    listChromosome[generation][posRand] = Chromosome(best.nbSources, best.positions);

    // on calcul la zone couverture pour chacun des chromosomes
    initialiseChromosome();

    /* --- SELECTION --- */
    // valeur moyenne de la zone de couverture pour la generation
    double mean = 0;
    // on itère sur tous les chromosomes pour calculer la zone de couverture moyenne
    for(int i = 0; i < listChromosome[generation].size(); i++) {
        // cette condition initialise le meilleur chromosome a la première generation
        if(generation == 0 && i == 0) {
            best = Chromosome(listChromosome[generation][i].nbSources, listChromosome[generation][i].positions);
            best.area = listChromosome[generation][i].area;
            best.overlap = listChromosome[generation][i].overlap;
        }
        mean += listChromosome[generation][i].area/totalArea;
    }
    // affichage console
    std::cout << "moyenne de l'aire de la generatioin " << generation << " = " << mean << " et : " << mean/listChromosome[generation].size() << std::endl;

    // on augmente le numero de la generation
    generation += 1;
    // on selectionne les chromosomes en les faisant s'affronter 2 par 2
    for (int i = 0; i <  listChromosome[generation-1].size()/2; i++) {
        std::vector<Chromosome> vInit;
        listChromosome.push_back(vInit);
        Chromosome newChro = selection(listChromosome[generation-1][2*i], listChromosome[generation-1][2*i+1]);
        listChromosome[generation].push_back(newChro);
    }

    // affichage console
    std::cout << "---- BEST DE LA GENERATION : aire = " << best.area/totalArea << " - overlap =  " << best.overlap/best.area << " - nbSources = " << best.nbSources << " - listCells size = " << best.listCells.size() << std::endl;
    for (int i = 0; i < best.nbSources; i++) {
        std::cout << "source : " << i << " - posx : " << best.positions[i][0] << " - posy : " << best.positions[i][1] << std::endl;
    }
}

/**
 * cette fonction prend une liste de chromosomes en entree
 * et retourne cette même liste melangee
 * @param listBis : liste de chromosomes a melanger
 */
void Optimization::shuffle(std::vector<Chromosome> *listBis) {
    std::vector<Chromosome> tempList = *listBis;
    for (int i = 0; i < (*listBis).size(); i++) {
        (*listBis)[i] = tempList[tempList.size()-(i+1)];
    }
}

/**
 * cette fonction permet d'initialiser la première
 * generation de chromosomes
 * @param nbChromsome : nombre de chromosomes dans la generation
 */
void Optimization::initialisation(int nbChromsome) {
    // on cree tous les chromosomes et on les ajoutes a la liste initiale
    for (int i = 0; i < nbChromsome; i++) {
        // on genère un nombre aleatoire de sources initiales
        // -> compris entre 2 et 4
        int nRand = rand() % (3) + 2;
        std::vector<std::array<double,2> > listPositions;
        for (int j = 0; j < nRand; j++) {
            // on genère une position aleatoire selon x et y pour chaque source
            double xRand = rand() % ((int)(width) + 1);
            double yRand = rand() % ((int)(height) + 1);
            listPositions.push_back((std::array<double,2>){xRand, yRand} );
        }
        std::vector<Chromosome> vInit;
        listChromosome.push_back(vInit);
        // on ajoute le chromosome a la liste initiale
        listChromosome[generation].push_back(Chromosome(nRand, listPositions));
    }
}

/**
 * Cette fonction prend deux chromosomes en entree
 * et renvoie le meilleur des deux
 * @param chromosome1
 * @param chromosome2
 * @return
 */
Chromosome Optimization::selection(Chromosome &chromosome1, Chromosome &chromosome2) {
    // on definit les poids pour les differents paramètres de selection :
    // -> zone de couverture, intersection des aires, et nombre de sources
    double weightArea = 1, weightOverlap = 0.1, weightNbSources = 0.1;
    // calcul de la performance des deux chromosomes
    double comparateur1 = weightArea*(chromosome1.area/totalArea) - weightOverlap*(chromosome1.overlap/chromosome1.area) - weightNbSources*((double)chromosome1.nbSources);
    double comparateur2 = weightArea*(chromosome2.area/totalArea) - weightOverlap*(chromosome2.overlap/chromosome2.area) - weightNbSources*((double)chromosome2.nbSources);
    // calcul de la performance du meilleur chromosome actuel de la generation
    double comparateurBest = weightArea*(best.area/totalArea) - weightOverlap*(best.overlap/best.area) - weightNbSources*((double)best.nbSources);

    // on verifie que les deux chromosomes ne sont pas plus performants que le
    // meilleur chromosome actuel -> sinon on change le meilleur chromosome
    if(comparateurBest <= comparateur1) {
        best = Chromosome(chromosome1.nbSources, chromosome1.positions);
        best.area = chromosome1.area;
        best.overlap = chromosome1.overlap;
    }
    if(comparateurBest <= comparateur2) {
        best = Chromosome(chromosome2.nbSources, chromosome2.positions);
        best.area = chromosome2.area;
        best.overlap = chromosome2.overlap;
    }

    // affichage console
    std::cout << "----- SELECTION -----" << std::endl;
    std::cout << "chromosome 1 : aire = " << chromosome1.area/252 << " - overlap =  " << chromosome1.overlap/chromosome1.area << " - nbSources = " << weightNbSources*((double)chromosome1.nbSources) << " - listCells size = " << chromosome1.listCells.size() << std::endl;
    for (int i = 0; i < chromosome1.nbSources; i++) {
        std::cout << "source : " << i << " - posx : " << chromosome1.positions[i][0] << " - posy : " << chromosome1.positions[i][1] << std::endl;
    }
    std::cout << "chromosome 2 : aire = " << chromosome2.area/252 << " - overlap =  " << chromosome2.overlap/chromosome2.area << " - nbSources = " << weightNbSources*((double)chromosome2.nbSources) << " - listCells size = " << chromosome2.listCells.size() << std::endl;
    for (int i = 0; i < chromosome2.nbSources; i++) {
        std::cout << "source : " << i << " - posx : " << chromosome2.positions[i][0] << " - posy : " << chromosome2.positions[i][1] << std::endl;
    }

    // on retourne le chromosome le plus performant
    if(comparateur1 > comparateur2) {
        return Chromosome(chromosome1.nbSources, chromosome1.positions);
    } else {
        return Chromosome(chromosome2.nbSources, chromosome2.positions);
    }
}

/**
 * cette fonction realise le croisement uniforme
 * @param chromosome1
 * @param chromosome2
 */
void Optimization::uniformCrossover(Chromosome *chromosome1, Chromosome *chromosome2) {
    // on genère une source aleatoirement dans chacun des chromosomes
    int c1 = rand() % (int)(chromosome1->nbSources);
    int c2 = rand() % (int)(chromosome2->nbSources);

    // on intervertit les positions selon x et y l'un avec l'autre
    chromosome1->positions[c1][1] = chromosome2->positions[c2][1];
    chromosome2->positions[c2][0] = chromosome1->positions[c1][0];
}

/**
 * cette fonction realise le croisement a un point
 * @param chromosome1
 * @param chromosome2
 */
void Optimization::onePointCrossover(Chromosome *chromosome1, Chromosome *chromosome2) {
    // on verifie que les chromosomes possèdent plus d'une source
    // -> sinon ça n'est la peine de faire le croisement (nb : ce cas est très rare)
    if(chromosome1->nbSources > 1 && chromosome2->nbSources > 1) {
        // on genère une position aleatoire en lequel on coupe les deux chromosomes
        int c1 = rand() % (chromosome1->nbSources-1) + 1;
        int c2 = rand() % (chromosome2->nbSources-1) + 1;

        // on decoupe chacun des chromosomes en deux
        std::vector<std::array<double,2> > copyFirst1 = slice(chromosome1->positions, 0, c1);
        std::vector<std::array<double,2> > copyLast1 = slice(chromosome1->positions, c1, chromosome1->nbSources);
        std::vector<std::array<double,2> > copyFirst2 = slice(chromosome2->positions, 0, c2);
        std::vector<std::array<double,2> > copyLast2 = slice(chromosome2->positions, c2, chromosome2->nbSources);

        // on fusionne les differentes parties ensemble pour croiser les chromosomes entre eux
        chromosome1->positions.clear();
        copy(copyFirst1, copyLast2, &(chromosome1->positions));
        chromosome1->nbSources = chromosome1->positions.size();
        chromosome2->positions.clear();
        copy(copyFirst2, copyLast1, &(chromosome2->positions));
        chromosome2->nbSources = chromosome2->positions.size();
    }
}

/**
 * cette fonction realise la mutation sur un chromosome
 * @param chromosome
 */
void Optimization::mutation(Chromosome *chromosome) {
    // on selectionne une source du chromosome aleatoirement
    int index = rand() % (chromosome->nbSources);
    // on genère une valeurs aleatoire selon x et y
    // dans les limites des dimensions de la map
    int xRand = rand() % ((int)(width)+1) - chromosome->positions[index][0];
    int yRand = rand() % ((int)(height)+1) - chromosome->positions[index][1];

    // on ajoute les valeurs aleatoires a la position de la source
    chromosome->positions[index][0] += xRand;
    chromosome->positions[index][1] += yRand;
}

/**
 * cette fonction prend un vecteur en enree
 * et rentourne un vecteur possedant ses valeurs
 * entre l'indice m et n
 * @param v : vecteur a decouper
 * @param m : position de depart de la decoupe
 * @param n : position finale de la decoupe
 * @return : retourne le vecteur decoupe a partir de v
 */
std::vector<std::array<double,2> > Optimization::slice(std::vector<std::array<double,2> > const &v, int m, int n) {
    std::vector<std::array<double,2> > vec;
    for(int i = 0; m+i < n; i++) {
        vec.push_back(v[m+i]);
    }
    return vec;
}

/**
 * cette fonction fusionne deux vecteurs v1 et v2 entre
 * eux pour former un nouveau vecteur vout
 * @param v1 : première partie a fusionner
 * @param v2 : seconde partie a fusionner
 * @param vOut : vecteur resultant de la fusion de v1 et v2
 */
void Optimization::copy(std::vector<std::array<double,2> > const &v1, std::vector<std::array<double,2> > const &v2, std::vector<std::array<double,2> > *vOut) {
    for(int i = 0; i < v1.size(); i++) {
        vOut->push_back(v1[i]);
    }
    for(int i = 0; i < v2.size(); i++) {
        vOut->push_back(v2[i]);
    }
}

/**
 * Cette fonction initialise les chromosomes avant
 * de faire la selection
 */
void Optimization::initialiseChromosome() {
    for (int i = 0; i < listChromosome[generation].size(); i++) {
        for (int j = 0; j < listChromosome[generation][i].nbSources; j++) {
            coverageArea(listChromosome[generation][i].positions[j], &(listChromosome[generation][i]));
        }
        std::cout << "avancement " << i << " / " << listChromosome[generation].size() << std::endl;
    }
}

/**
 * Cette fonction calcule la zone de couverture d'une source pour un chromosome
 * -> elle est optimisee de sorte a ne pas devoir calculer la puissance
 *  captee sur l'entierete de la map
 * @param positionSourceInit : position de la source dont on calcul la zone de couverture
 * @param chromosome : chromosome dont on calcul la zone de couverture
 */
void Optimization::coverageArea(std::array<double,2> positionSourceInit, Chromosome *chromosome) {
    // liste des sources / source image
    std::vector<std::vector<Source> > listSources;
    // on initialise la liste des sources
    double posInit[2] = {positionSourceInit[0], positionSourceInit[1]};
    initialiselistSources(nbRef, 1, posInit, &listSources);
    // on cree la liste des sources images
    createListSources(&listWalls, &listSources, nbRef);
    // surface totale calculee
    double sUnitaire = pow(discretisation, 2);
    // conditions pour limiter la limite de surface calculee selon l'axe y
    int cond1y = 0, cond2y = 0, cond3y = 0, cond4y = 0;
    double x = listSources[0][0].position[0] - (int)listSources[0][0].position[0] % discretisation;
    double y = listSources[0][0].position[1] - (int)listSources[0][0].position[1] % discretisation;
    // on itère sur la map pour calculer la zone de couverture
    for(int i = 0; i <= height; i += discretisation) {
        // conditions pour limiter la limite de surface calculee selon l'axe x
        int cond1x = 0, cond2x = 0, cond3x = 0, cond4x = 0;
        for (int j = 0; j <= width; j += discretisation) {
            // conditions permettant de verifier que la position n'est pas en dehors
            // de la map et de ne pas calculer plusieurs fois une même zone
            bool check1 = false, check2 = false, check3 = false, check4 = false;
            // position du recepteur
            std::array<double,2>  pCell1, pCell2, pCell3, pCell4;

            // on initialise les conditions
            if(i == 0 && j == 0) {
                pCell1 = (std::array<double,2>){x+j,y+i};
                check1 = true;
            } else if(j == 0) {
                if(y+i <= height && cond1x < 1) {
                    pCell1 = (std::array<double,2>){x+j,y+i};
                    check1 = true;
                }
                if(y-i >= 0 && cond2x < 1) {
                    pCell2 = (std::array<double,2>){x+j,y-i};
                    check2 = true;
                }
            } else if(i == 0) {
                if(x+j <= width && cond1x < 1) {
                    pCell1 = (std::array<double,2>){x+j,y+i};
                    check1 = true;
                }
                if(x-j >= 0 && cond3x < 1) {
                    pCell3 = (std::array<double,2>){x-j,y-i};
                    check3 = true;
                }
            } else {
                if(x+j <= width && y+i <= height && cond1x < 1) {
                    pCell1 = (std::array<double,2>){x+j,y+i};
                    check1 = true;
                }
                if(x+j <= width && y-i >= 0 && cond2x < 1) {
                    pCell2 = (std::array<double,2>){x+j,y-i};
                    check2 = true;
                }
                if(x-j >= 0 && y-i >= 0 && cond3x < 1) {
                    pCell3 = (std::array<double,2>){x-j,y-i};
                    check3 = true;
                }
                if(x-j >= 0 && y+i <= height && cond4x < 1) {
                    pCell4 = (std::array<double,2>){x-j,y+i};
                    check4 = true;
                }
            }

            // on calcule la zone de couverture dans 4 directions differentes en même temps
            if(check1) {
                if(calculatePower(listSources, pCell1) > -82.0) {
                    chromosome->updateCoverage(pCell1, sUnitaire);
                } else {
                    cond1x += 1;
                    if(j == 0) { cond1y += 1; }
                }
            }
            if(check2) {
                if(calculatePower(listSources, pCell2) > -82.0) {
                    chromosome->updateCoverage(pCell2, sUnitaire);
                } else {
                    cond2x += 1;
                    if(j == 0) { cond2y += 1; }
                }
            }
            if(check3) {
                if(calculatePower(listSources, pCell3) > -82.0) {
                    chromosome->updateCoverage(pCell3, sUnitaire);
                } else {
                    cond3x += 1;
                    if(j == 0) { cond3y += 1; }
                }
            }
            if(check4) {
                if(calculatePower(listSources, pCell4) > -82.0) {
                    chromosome->updateCoverage(pCell4, sUnitaire);
                } else {
                    cond4x += 1;
                    if(j == 0) { cond4y += 1; }
                }
            }
            if(!check1 && !check2 && !check3 && !check4) {
                break;
            }
        }
        if(cond1y >= 1 && cond2y >= 1 && cond3y >= 1 && cond4y >= 1) {
            break;
        }
    }
}

/**
 * cette fonction calcul la puissance captee a la position pCell
 * @param listSources : liste des sources images
 * @param pCell : position en laquelle on calcul la puissance
 * @return : retounre la puissance captee
 */
double Optimization::calculatePower(std::vector<std::vector<Source> > listSources, std::array<double,2>  pCell) {
    // creation recepteur
    double positionCell[2] = {pCell[0], pCell[1]};
    Cell *cell = new Cell(positionCell);
    // liste des faisceaux
    std::vector<Beam> listBeams;
    // onde directe
    for(int i = 0; i < listSources[0].size(); i++) {
        listSources[0][i].directWave(cell, &listWalls, &listBeams,i);
    }
    // reflexions
    for(int i = 0; i < nbRef; i++) {
        rayTracing(cell, &listWalls, &listBeams, &listSources[i+1], i);
    }
    // calcul puissance captee
    for(Beam &beam : listBeams) {
        beam.electricField(listSources[0][0].beta, listSources[0][0].G, listSources[0][0].P_watt);
        cell->updatePower(listSources[0][0].R_a, listSources[0][0].h_e, beam.E);
    }
    return cell->P_dbm;
}
