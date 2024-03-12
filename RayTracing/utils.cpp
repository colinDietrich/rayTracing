#include <iostream>
#include <cmath>
#include <vector>
#include <complex>
#include "utils.h"
#include "Source.h"
#include "Wall.h"
#include "Beam.h"
/**
 * Cree la liste des sources et y place les sources initiales
 * @param nbRef : nombre de reflexions
 * @param nbSourceinit : nombre de sources initiales
 * @param positionSourceInit : positions des sources initiales
 * @param listSources : liste des sources
 */
void initialiselistSources(int nbRef, int nbSourceinit, double *positionSourceInit, std::vector<std::vector<Source> > *listSources) {
    for(int i = 0; i<nbRef+1; i++) {
        // une liste pour les sources d'ordre i
        std::vector<Source> sources1;
        // ajout des sources initiales (ordre 0)
        if(i == 0) {
            for(int j = 0; j<nbSourceinit; j++) {
                double pSource[2] = {positionSourceInit[2*j], positionSourceInit[2*j+1]};
                sources1.push_back(Source(pSource, false));
            }
        }
        listSources->push_back(sources1);
    }
}
/**
 * Remplit la liste des sources avec les sources images
 * @param listWalls : liste des murs
 * @param listSources : liste des sources
 * @param nbRef : nombre de reflexions
 */
void createListSources(std::vector<Wall> *listWalls, std::vector<std::vector<Source> > *listSources, int nbRef) {
    for(int i = 0; i < nbRef; i++) {
        for(int j = 0; j<(*listSources)[i].size(); j++) {
            // pour chaque source, on calcule la source image de chaque mur
            for (Wall &wall : (*listWalls)) {
                // le mur par rapport pour lequel on cherche une source image doit etre different
                // du mur de la source initial -> le cas echeant on retombe sur la source parente
                if ((*listSources)[i][j].wallImage != &wall) {
                    // position de la source image
                    double image[2];
                    (*listSources)[i][j].imageSource(wall, image);
                    // on cree la source image
                    Source newSourceImage = Source(image, true);
                    newSourceImage.wallImage = &wall;
                    // remplit la liste de sources parentes
                    for (Source *parentSource : (*listSources)[i][j].listParentSources) {
                        newSourceImage.listParentSources.push_back(parentSource);
                    }
                    newSourceImage.listParentSources.push_back(&((*listSources)[i][j]));
                    // ajout a la liste de sources
                    (*listSources)[i+1].push_back(newSourceImage);
                }
            }
        }
    }
}
/**
 * Cree l'ensemble des rayons reflechis nR fois et les ajoute a la liste des rayons.
 * @param cell : recepteur
 * @param listWalls : liste des murs
 * @param listBeams : liste des rayons, passee par reference pour etre modifiee.
 * @param listSources : liste des sources
 * @param nR : nombre de reflexion
 */
void rayTracing(Cell *cell, std::vector<Wall> *listWalls, std::vector<Beam> *listBeams, std::vector<Source> *listSources, int nR) {
    // on itere pour chacune des sources images (d'ordre nR)
    for (Source &source : (*listSources)) {
        // intersection mur faisceau
        double intersection[2];
        // faisceau contenant tous les points intermediaires
        Beam beam = Beam();
        // check si il y a une intersection
        if (source.wallImage->reflexionIntersectionPoint(source.position, cell->position, intersection)) {
            // on ajoute les 2 premieres intersections
            beam.addR((std::array<double, 2>){cell->position[0], cell->position[1]});
            beam.addR((std::array<double, 2>){intersection[0], intersection[1]});
            // on verifie que la source n'est pas sur le mur -> le cas echeant l'intersection serait au meme endroit
            // que la source ce qui donnerait une valeur 'nan' pour angle_i
            if( (intersection[0] != cell->position[0]) && (intersection[1] != cell->position[1]) ) {
                // coefficient de reflexion
                std::complex<double> coeffR = 1;
                if(backTracing(&beam, &coeffR, source)) {
                    // angle incident
                    double angle_i = beam.angleCalculator(source.wallImage->position,0);
                    // on update le coeff de reflexion
                    beam.coeff *= coeffR;
                    beam.coeff *= source.wallImage->reflexionCoefficientCalculator(angle_i);
                    // on update le coeff de transmission pour chaque partie directe du rayon
                    for(int n = 0; n<beam.size-1; n++) {
                        beam.coeff *= beam.updateTransferCoefficient(*listWalls, n);
                    }
                    // ajout a la liste
                    (*listBeams).push_back(beam);
                } else {
                    continue;
                }
            }
        }
    }
}
/**
 * Verifie que par la methode des images que le chemin de reflexion est valide
 * et calcule le coefficient total.
 * @param beam : rayon reflechi
 * @param coeff : coefficient de reflexion
 * @param image : source image
 * @return : 1 si le chemin est valide et 0 sinon
 */
int backTracing(Beam *beam, std::complex<double> *coeff, Source image) {
    // intersection mur faisceau
    double inter1[2];
    // nbre reflexions
    int n = 1;
    // on itere sur toutes les sources parentes
    for(int i = image.listParentSources.size()-1; i>=0; i--) {
        // intersection mur faisceau
        double inter[2];
        // check si la source parente est une image
        if(image.listParentSources[i]->isImage) {
            double posInter[2] = {beam->listPositions[beam->size-1][0], beam->listPositions[beam->size-1][1]};
            // check si il y'a une intersection
            if (image.listParentSources[i]->wallImage->reflexionIntersectionPoint(image.listParentSources[i]->position, posInter, inter)) {
                // ajout des positions
                beam->addR((std::array<double, 2>){inter[0], inter[1]});
                // ckeck si intersection egale a l'intersection precedente
                if(inter[0] == posInter[0] && inter[1] == posInter[1]) {
                    return 0;
                }
                // angle incident
                double angle_i = beam->angleCalculator(image.listParentSources[i]->wallImage->position,beam->size-2);
                // coeff reflexion
                (*coeff) *= image.listParentSources[i]->wallImage->reflexionCoefficientCalculator(angle_i);
                n++;
            } else {
                return 0;
            }
        } else {
            beam->addR((std::array<double, 2>){image.listParentSources[i]->position[0], image.listParentSources[i]->position[1]});
        }
    }
    return 1;
}