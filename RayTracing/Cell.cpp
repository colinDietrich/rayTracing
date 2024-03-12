#include <iostream>
#include <complex>
#include "Cell.h"
#include "Graphics.h"

/**
 * Constructeur
 * @param pPosition : position du recepteur
 */
Cell::Cell(double *pPosition)
{
    P_watt = 0;
    memcpy(position, pPosition,2*sizeof(double));
}
/**
* Ajoute la contribution du champ E a la puissance de reception.
* @param Ra : resistance de rayonnement
* @param he : hauteur equivalente
* @param E : champ electrique propage par une onde sur le recepteur
*/
void Cell::updatePower(double Ra, double he, std::complex<double> E) {
    // ajout de la contribution du champ E a la puissance
    P_watt += (1/(8*Ra))*pow(abs(he*E),2);

    // conversion
    P_dbm = 10 * log10(P_watt/(1e-3));
    if(P_dbm > -73) {
        bitRate = 320;
    } else if(P_dbm < -82) {
        bitRate = 40;
    } else {
        double alpha = (320.0 - 40.0) / (-73.0 + 82.0);
        bitRate = alpha*(P_dbm + 73.0) + 320.0;
    }
}








