#ifndef SOURCE_H
#define SOURCE_H
#include <vector>
#include <complex>
#include "Source.h"
#include "Cell.h"
#include "Wall.h"
#include "Beam.h"

class Source
{
    public :

    // sources parentes
    // (historique de projection par la methode des images)
    std::vector<Source *> listParentSources;
    // mur pour source image
    Wall *wallImage;
    // position de la source (x,y)
    double position[2];
    // frequence d'emission de l'antenne
    double f;
    // nombre d'onde
    double beta;
    // hauteur equivalente
    double h_e;
    // puissance d'emission en décibels
    double P_dbm;
    // puissance d'emission en watts
    double P_watt;
    // resistance de rayonnement
    double R_a;
    // rendement de l'antenne
    double rend;
    // gain de l'antenne
    double G;
    // booleen qui est vrai si la source est une image
    bool isImage;

    Source(double pPosition[2], bool pIsImage);
    void imageSource(Wall pWall, double *image);
    void directWave(Cell *cell, std::vector<Wall> *listWalls, std::vector<Beam> *listBeams, int n);
};

#endif
