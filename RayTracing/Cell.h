#ifndef CELL_H
#define CELL_H

#include <complex>
#include "Beam.h"
#include "Wall.h"
#include "Graphics.h"

class Cell
{
    public :

    // position du recepteur (x,y)
    double position[2];
    // puissance de reception en decibels
    double P_dbm;
    // puissance de reception en watts
    double P_watt;
    // debit bianaire
    double bitRate;
    // liste de faisceaux associes au recepteur
    std::vector<Beam> listBeams = std::vector<Beam>();
    
    Cell(double *pPosition);
    void updatePower(double Ra, double he, std::complex<double> E);

};

#endif
