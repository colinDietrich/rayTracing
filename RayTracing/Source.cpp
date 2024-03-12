#include <iostream>
#include <vector>
#include <array>
#include <complex>
#include "Source.h"
#include "Wall.h"
#include "Beam.h"
#define pi 3.141592653589793238462643383279502884
#define c 299792458.0

/**
 * Constructeur
 * @param pPosition : position de la source
 * @param pIsImage : booleen qui est vrai si la source est une image
 */
Source::Source(double pPosition[2], bool pIsImage)
{
    // position de la source (x,y)
    memcpy(position, pPosition,2*sizeof(double));
    // frequence d'emission de l'antenne
    f = 27.0e9;
    // nombre d'onde
    beta = (2.0*pi*f) / c;
    // hauteur equivalente
    h_e = -2.0/beta;
    // puissance d'emission en décibels
    P_dbm = 20.0;
    // puissance d'emission en watts -> Ptx
    P_watt = pow(10,P_dbm/10)*1.0e-3;
    // resistance de rayonnement
    R_a = 71.0;
    // rendement de l'antenne
    rend = 1.0;
    // gain de l'antenne -> Gtx
    G = rend * (16.0/(3.0*pi));
    // booleen qui est vrai si la source est une image
    isImage = pIsImage;
}
/**
 * Calcule la position de l'image de la source associee a un mur
 * @param pWall : mur associe a la source image
 * @param image : position de la source image passee par reference pour etre modifiee
 */
void Source::imageSource(Wall pWall, double *image)
{
    // pente du mur
    double alphaMur = (pWall.position[3] - pWall.position[2]) / (pWall.position[1] - pWall.position[0]);
    //pente de la droite perpendiculaire au mur
    double alphaPerp = -(1/alphaMur);
    // point d'intersection
    double x, y;

    if(isinf(alphaMur)) {
        x = pWall.position[0];
        y = position[1];
    } else if(isinf(alphaPerp)) {
        x = position[0];
        y = pWall.position[2];
    } else{
        x = (position[1] - pWall.position[2] + alphaMur*pWall.position[0] - alphaPerp*position[0]) / (alphaMur - alphaPerp);
        y = alphaPerp * (x-position[0]) + position[1];
    }
    image[0]= x + (x-position[0]);
    image[1]= y + (y-position[1]);
}
/**
 * Cree le rayon direct entre la source et le recepteur et l'ajoute a la liste des rayons.
 * Calcule le coefficient de transmission total du rayon.
 * @param cell : recepteur
 * @param listWalls : liste des murs
 * @param listBeams : liste des rayons, passee par reference pour etre modifiee
 * @param n : indice de la source initiale dans le liste des sources
 */
void Source::directWave(Cell *cell, std::vector<Wall> *listWalls, std::vector<Beam> *listBeams, int n) {
    // liste des positions du rayon
    std::vector<std::array<double,2> > listPosBeam;
    listPosBeam.push_back((std::array<double,2>){position[0], position[1]});
    listPosBeam.push_back((std::array<double,2>){cell->position[0], cell->position[1]});
    // on cree le faisceau d'onde directe et on l'ajoute a la liste
    (*listBeams).push_back(Beam(listPosBeam));
    // on verifie si il traverse des murs et on change le coefficient total en fonction
    (*listBeams)[n].coeff *= (*listBeams)[n].updateTransferCoefficient((*listWalls), 0);
}



