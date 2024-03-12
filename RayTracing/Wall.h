#ifndef WALL_H
#define WALL_H
#include <complex>
#include <array>

class Wall
{
    public :
    
    // position de départ du mur
    double position[4];
    // épaisseur du mur
    double width;
    // permittivite relative du mur
    double eps_r;
    // permitivite du mur
    double eps;
    // conductivite du mur
    double sigma;
    // constante de propagation du mur
    std::complex<double> gamma_m;
    // impedance du mur
    std::complex<double> Z;
    // nombre d'onde de la source
    double beta;
    
    
    Wall(double *pPosition, double pWidth, double pEps_r, double pSigma);
    std::complex<double> reflexionCoefficientCalculator(double angle_i);
    std::complex<double> transmissionCoefficientCalculator(double angle_i);
    double snellLaw(double angle_i);
    void setImpedance(double pF);
    int reflexionIntersectionPoint(double positionImage[2], double positionCell[2], double *intersection);
    int transmissionIntersectionPoint(std::array<double, 4> pPosition, double *intersection);
    int intersectionPoint(std::array<double, 4> pPosition, double *intersection);
};

#endif
