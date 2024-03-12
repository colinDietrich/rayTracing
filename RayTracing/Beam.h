#ifndef BEAM_H
#define BEAM_H
#include <complex>
#include <vector>
#include <array>
#include "Wall.h"

class Beam
{
    public :

    // nombre de reflexions subies par le faisceau
    int size;
    // coefficient de reflexion total du faisceau
    std::complex<double> coeff;
    // positions intermediaires des reflexions du faisceau
    std::vector<std::array<double,2> > listPositions;
    // champ electrique capte par le recepteur
    std::complex<double> E;

    Beam(std::vector<std::array<double,2> > pListPositions = std::vector<std::array<double,2> >());
    void addR(std::array<double,2> newPos);
    double angleCalculator(double *wallPosition, int n);
    std::complex<double> updateTransferCoefficient(std::vector<Wall> listWalls, int n);
    double getDistance();
    void electricField(double beta, double Gtx, double Ptx);

};

#endif
