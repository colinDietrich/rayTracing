#include <iostream>
#include <cmath>
#include <complex>
#include <array>
#include "Wall.h"
#define pi 3.141592653589793238462643383279502884
#define c 299792458.0
#define eps_0 (8.85418782 * 1e-12)
#define Z_0 (120.0 * 3.14159)
#define mu_0 (4.0 * 3.141592653589793238462643383279502884 * 1e-7)

/**
 * Constucteur
 * @param pPosition : positions des extremites du mur
 * @param pWidth : epaisseur
 * @param pEps_r : permittivite relative
 * @param pSigma : conductivite
 */
Wall::Wall(double *pPosition, double pWidth, double pEps_r, double pSigma)
{
    // position de départ du mur
    memcpy(position, pPosition,4*sizeof(double));
    // epaisseur du mur
    width = pWidth;
    // permittivite relative du mur
    eps_r = pEps_r;
    // permitivite du mur
    eps = eps_r * eps_0;
    // conductivite du mur
    sigma = pSigma;
}
/**
 * Calcule le coefficient de réflexion du rayon qui se reflechit sur le mur.
 * @param angle_i : angle d'incidence par rapport à la normale
 * @return coefficient de reflexion
 */
std::complex<double> Wall::reflexionCoefficientCalculator(double angle_i)
{
    // angle de transmission
    double angle_t = snellLaw(angle_i);
    // distance de propagation dans le mur
    double s = width/cos(angle_t);
    // coefficient de reflexion de l’interface air/mur
    std::complex<double> gamma1 = (Z*cos(angle_i)-Z_0*cos(angle_t)) / (Z*cos(angle_i) + Z_0*cos(angle_t));
    // definition des deux arguments des 2 exponentielles
    std::complex<double> e1 = -2.0*gamma_m*s;
    std::complex<double> j(0.0,1);
    std::complex<double> e2 = beta*2*s*sin(angle_t)*sin(angle_i) * j;
    // coefficient de reflexion total
    std::complex<double> gamma_tot = gamma1 - (1.0-pow(gamma1,2)) * ((gamma1*exp(e1)*exp(e2)) / (1.0-pow(gamma1,2)*exp(e1)*exp(e2)));
    return gamma_tot;
}
/**
 * Calcule le coefficient de transmission du rayon qui traverse le mur.
 * @param angle_i : angle d'incidence par rapport à la normale
 * @return coefficient de transmission
 */
std::complex<double> Wall::transmissionCoefficientCalculator(double angle_i)
{
    // angle de transmission
    double angle_t = snellLaw(angle_i);
    // distance de propagation dans le mur
    std::complex<double> s = width/cos(angle_t);
    // coefficient de reflexion de l’interface air/mur
    std::complex<double> gamma1 = (Z*cos(angle_i)-Z_0*cos(angle_t)) / (Z*cos(angle_i) + Z_0*cos(angle_t));
    // definition des deux arguments des 2 exponentielles
    std::complex<double> e1 = -gamma_m*s;
    std::complex<double> j(0.0,1);
    std::complex<double> e2 = beta*2*s*sin(angle_t)*sin(angle_i) * j;
    // coefficient de transmission total
    std::complex<double> gamma_tot = ((1.0-pow(gamma1,2)) * exp(e1)) / (1.0 - pow(gamma1,2)*exp(2.0*e1)*exp(e2));
    return gamma_tot;
}
/**
 * Calcule l'angle de transmission a l'aide de la loi de Snell.
 * @param angle_i : angle d'incidence par rapport à la normale
 * @return angle de transmission
 */
double Wall::snellLaw(double angle_i)
{
    // quotient des permittivités
    double frac = eps_0 / eps;
    // angle de transmission
    double angle_t = asin(sqrt(frac) * sin(angle_i));
    return angle_t;
}
/**
 * Verifie que le point d'intersection de la droite prolongeant le mur et de
 * la droite passant par 2 points existe et le calcule.
 * @param pPosition : position des 2 points de la droite
 * @param intersection : position de l'intersection, passee par reference pour etre modifiee
 * @return 1 si ce point existe et 0 sinon
 */
int Wall::intersectionPoint(std::array<double,4> pPosition, double *intersection)
{
    // coefficient angulaire du mur
    double alpha = (position[3] - position[2]) / (position[1] - position[0]);
    // coefficient angulaire du faisceau
    double pAlpha = (pPosition[3] - pPosition[2]) / (pPosition[1] - pPosition[0]);
    // points d'intersection
    double x, y;
    // 2 segments paralleles
    if(alpha == pAlpha) {
        return 0;
    }
    // Cas avec intersection
    else if(isinf(alpha)) {
        x = position[0];
        y = pAlpha*(x-pPosition[0]) + pPosition[2];
    } else if(isinf(pAlpha)) {
        x = pPosition[0];
        y = alpha*(x-position[0]) + position[2];
    } else if(alpha == 0) {
        y = position[2];
        x = (y-pPosition[2]) / pAlpha + pPosition[0];
    } else if(pAlpha == 0) {
        y = pPosition[2];
        x = (y-position[2]) / alpha + position[0];
    } else {
        x = (pPosition[2] - position[2] + alpha*position[0] - pAlpha*pPosition[0]) / (alpha - pAlpha);
        y = pAlpha * (x-pPosition[0]) + pPosition[2];
    }
    intersection[0] = x;
    intersection[1] = y;
    return 1;
}
/**
 * Verifie qu'il existe et calcule le point de reflexion sur le mur par la methode des images.
 * @param positionImage : position de la source image visee par le rayon
 * @param positionCell : position de depart du rayon
 * @param intersection : position du point de reflexion sur le mur, passee par reference pour etre modifiee
 * @return 1 si ce point existe sur le mur et 0 sinon
 */
int Wall::reflexionIntersectionPoint(double positionImage[2], double positionCell[2], double *intersection) {
    // calcul le point d'intersection
    std::array<double,4> positionImageCell = {positionImage[0], positionCell[0], positionImage[1], positionCell[1]};
    if(intersectionPoint(positionImageCell, intersection)) {
        double x = intersection[0]; double y = intersection[1];
        // epsilon qu'on ajoute ou enlève pour se débarraser de l'erreur de représentation en base binaire
        double epsilon = 1e-5;
        // verifie que le point d'intersection est sur le mur et le faisceau
        bool condx = (x >= std::min(position[0],position[1]) && x <= std::max(position[0],position[1]) && x >= std::min(positionImage[0],positionCell[0]) && x <= std::max(positionImage[0],positionCell[0]));
        bool condy = (y >= std::min(position[2],position[3])  && y <= std::max(position[2],position[3]) && y >= std::min(positionImage[1],positionCell[1]) && y <= std::max(positionImage[1],positionCell[1]));

        if(condx && condy) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}
/**
 * Verifie qu'il existe et calcule le point de transmission sur le mur.
 * @param pPosition : position des extremites du rayon
 * @param intersection : position du point de transmission sur le mur, passee par reference pour etre modifiee
 * @return 1 si ce point existe  sur le mur et 0 sinon
 */
int Wall::transmissionIntersectionPoint(std::array<double,4> pPosition, double *intersection) {
    // calcul le point d'intersection
    if(intersectionPoint(pPosition, intersection)) {
        double x = intersection[0]; double y = intersection[1];
        // epsilon qu'on ajoute ou enlève pour se débarraser de l'erreur de représentation en base binaire
        double epsilon = 1e-5;
        // verifie que le point d'intersection est sur le mur et le faisceau
        bool condx = (x >= std::min(position[0],position[1]) - epsilon && x <= std::max(position[0],position[1]) + epsilon && x >= std::min(pPosition[0],pPosition[1]) - epsilon && x <= std::max(pPosition[0],pPosition[1]) + epsilon);
        bool condy = (y >= std::min(position[2],position[3]) - epsilon && y <= std::max(position[2],position[3]) + epsilon && y >= std::min(pPosition[2],pPosition[3]) - epsilon && y <= std::max(pPosition[2],pPosition[3]) + epsilon);
        if(condx && condy) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}
/**
 * Assigne la valeur des attributs suivants du mur : le nombre d'onde, sa constante
 * de propagation et son impedance.
 * @param pF : frequence de la source
 */
void Wall::setImpedance(double pF)
{
    // nombre d'onde de la source
    beta = (2.0*pi*pF) / c;
    // epsilon tilde
    std::complex<double> esp_tild(eps, -sigma / (2*pi*pF));
    // constante de propagation du mur
    std::complex<double> j(0.0,1);
    gamma_m = (2*pi*pF) * sqrt(mu_0*esp_tild) * j;
    // impedance du mur
    Z = sqrt(mu_0 / esp_tild);
}
