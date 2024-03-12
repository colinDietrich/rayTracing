#include <iostream>
#include <cmath>
#include <complex>
#include <vector>
#include <array>
#include <cmath>
#include "Beam.h"
#include "Wall.h"
#define pi 3.141592653589793238462643383279502884

/**
 * Constructeur
 * @param pListPositions : liste des positions du rayon
 */
Beam::Beam(std::vector<std::array<double,2> > pListPositions)
{
    // // nombre de reflexions subies par le faisceau + 2 (nbr total de positions)
    size = pListPositions.size();
    // positions intermediaires des reflexions du faisceau
    listPositions = std::vector<std::array<double,2> >(pListPositions);
    // coefficient de reflexion total du faisceau
    coeff = 1;
}
/**
 * Ajoute une position a la liste des positions
 * @param newPos : position a ajouter
 */
void Beam::addR(std::array<double,2> newPos) {
    size += 1;
    listPositions.push_back(newPos);
}
/**
 * Calcule l'angle que fait le rayon avec la normale du mur
 * @param wallPosition : position du mur
 * @param n : indice dans la liste des positions de la derniere position du rayon avant l'arrivee sur le mur
 * @return : angle incident
 */
double Beam::angleCalculator(double *wallPosition, int n) {
    // composantes vecteur mur
    double v_a = wallPosition[1] - wallPosition[0];
    double u_a = wallPosition[3] - wallPosition[2];
    // norme vecteur mur
    double d_a = sqrt(pow(v_a,2) + pow(u_a,2));
    // composantes vecteur faisceau
    double v_b = listPositions[n+1][0] - listPositions[n][0];
    double u_b = listPositions[n+1][1] - listPositions[n][1];
    // norme vecteur faisceau
    double d_b = sqrt(pow(v_b,2) + pow(u_b,2));
    // calcul de l'angle incident entre le faisceau et la normale au mur
    double angle_i = pi/2 - acos((v_a*v_b + u_a*u_b) / (d_a*d_b));
    return abs(angle_i);
}
/**
 * Calcule le coefficient de transmission total du rayon direct (sans reflexion).
 * @param listWalls : liste des murs
 * @param n : indice dans la liste des positions de la position de la position de depart du rayon
 * @return : coefficient de transmission
 */
std::complex<double> Beam::updateTransferCoefficient(std::vector<Wall> listWalls, int n) {
    // on cree le coefficient
    std::complex<double> transferCoefficient = 1.0;
    std::vector<std::array<double,2> > listIntersection;
    // pour chaque mur on verifie si le rayon le traverse
    for (Wall &wall : listWalls) {
        bool condx = (listPositions[n+1][0] >= std::min(wall.position[0],wall.position[1]) && listPositions[n+1][0] <= std::max(wall.position[0],wall.position[1]));
        bool condy = (listPositions[n+1][1] >= std::min(wall.position[2],wall.position[3]) && listPositions[n+1][1] <= std::max(wall.position[2],wall.position[3]));
        bool condx2 = (listPositions[n][0] >= std::min(wall.position[0],wall.position[1]) && listPositions[n][0] <= std::max(wall.position[0],wall.position[1]));
        bool condy2 = (listPositions[n][1] >= std::min(wall.position[2],wall.position[3]) && listPositions[n][1] <= std::max(wall.position[2],wall.position[3]));
        if(! ((condx && condy) || (condx2 && condy2)) ) {
            double intersection[2];
            // true si il existe un point d'intersection
            if(wall.transmissionIntersectionPoint((std::array<double,4>){listPositions[n][0],listPositions[n+1][0],listPositions[n][1],listPositions[n+1][1]}, intersection)) {
                for (int i = 0; i < listIntersection.size(); i++) {
                    if(( intersection[0] != listIntersection[i][0]) || (intersection[1] != listIntersection[i][1]) ) {
                        // angle incident entre le faisceau et la normale au mur
                        double angle_i = angleCalculator(wall.position, n);
                        // on ajoute le coeff de transmission du au mur
                        transferCoefficient *= wall.transmissionCoefficientCalculator(angle_i);
                        listIntersection.push_back((std::array<double,2>){intersection[0], intersection[1]});
                    }
                }
                if(listIntersection.size() == 0) {
                    // angle incident entre le faisceau et la normale au mur
                    double angle_i = angleCalculator(wall.position, n);
                    // on ajoute le coeff de transmission du au mur
                    transferCoefficient *= wall.transmissionCoefficientCalculator(angle_i);
                    listIntersection.push_back((std::array<double,2>){intersection[0], intersection[1]});
                }
            }

        }
    }
    return transferCoefficient;
}
/**
 * Calcule la distance totale parcourue par le rayon
 * @return : distance
 */
double Beam::getDistance() {
    double distance = 0;
    for(int i = 1; i < size; i++) {
        distance += sqrt(pow(listPositions[size-i][0]-listPositions[size-(i+1)][0],2) + pow(listPositions[size-i][1]-listPositions[size-(i+1)][1],2));
    }
    return distance;
}
/**
 * Calcule le champ electrique du rayon
 * @param beta : nombre d'onde
 * @param Gtx : gain de l'antenne
 * @param Ptx : puissance d'emission
 */
void Beam::electricField(double beta, double Gtx, double Ptx) {
    std::complex<double> e(0, -beta);
    double dn = getDistance();
    // si on est sur la source -> on lui donne la valeur juste a cote
    if(dn == 0.0) {
        dn = 0.1;
    }
    // champ electrique capte par le recepteur
    E = coeff * sqrt(60*Gtx*Ptx)*exp(e*dn)/(dn);
}

