#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include "Graphics.h"

/**
 * Constructeur
 */
Graphics::Graphics() {
    std::cout << "Création de l'objet graphique" << std::endl;
}
/**
 * Ajoute la commande qui dessine une ligne d'extrémités (x1,y1),(x2,y2)
 * et dont la couleur et la taille dépendent de l'argument (type)
 */
void Graphics::drawLine(double x1, double x2, double y1, double y2, int type) {
    std::string color;
    int size;
    switch(type) {
        case 0 :
            color = "grey";
            size = 3;
            break;
        case 100:
            color = "grey";
            size = 3;
            break;
        case 1:
            color = "green";
            size = 2;
            break;
        case 2:
            color = "blue";
            size = 1;
            break;
        case 3:
            color = "orange";
            size = 1;
            break;
        case 4:
            color = "red";
            size = 1;
            break;
        case 5:
            color = "pink";
            size = 1;
            break;
        case 6:
            color = "brown";
            size = 1;
            break;
        default:
            color = "black";
            size = 4;
    }
    std::string command = "set arrow nohead from " + std::to_string(x1) + "," + std::to_string(y1) + " to ";
    command += std::to_string(x2) +"," + std::to_string(y2) + " lc '" + color + "' lw " + std::to_string(size) + " front\n";
    std::cout << command;
    commandLine.push_back(command);
}
/**
 * Ajoute la commande qui dessine un point de coordonnées (x,y)
 * et dont la couleur et la taille dépendent de l'argument (type)
 */
void Graphics::addPoint(double x, double y, int type) {
    std::string color;
    double size;
    switch(type) {
        case 0:
            color = "green";
            size = 1.5;
            break;
        case 3:
            color = "black";
            size = 1.5;
            break;
        default:
            color = "black";
            size = 1;
    }
    std::string command = "set object circle at " + std::to_string(x) + "," + std::to_string(y) + " radius char ";
    command +=  std::to_string(size) + "fillstyle empty border lc rgb '"+ color + "' lw 2 front\n";
    commandLine.push_back(command);
}
/**
 * Permet l'affiche du graphe a l'aide de gnuplot en creant un fichier
 * de commandes et en le remplissant.
 * @param width : largeur
 * @param height : hauteur
 */
void Graphics::printGraph(double width, double height) {

    //Création du fichier de commandes
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if(gnuplot == NULL) {
        //printf("\n-----ERREUR-----\nIMPOSSIBLE D'OUVRIR LE FICHIER DANS LA CREATION DU FICHIER DE COMMANDE POUR GNUPLOT\n");
    }

    fprintf(gnuplot, "reset\n");
    fprintf(gnuplot, "set terminal qt size %d, %d font 'Helvetica,15'\n", (int)((width/height)*600), 600);
    fprintf(gnuplot, "unset key\n");
    fprintf(gnuplot, "set xlabel 'x (m)'\n");
    fprintf(gnuplot, "set ylabel 'y (m)'\n");
    fprintf(gnuplot, "set xrange[%f:%f]\n", 0.0, width);
    fprintf(gnuplot, "set yrange[%f:%f]\n", 0.0, height);
    for(int i = 0; i < commandLine.size(); i++) {
        fprintf(gnuplot, "%s", commandLine[i].c_str());
    }
    fprintf(gnuplot, "plot 1/0\n");
    fclose(gnuplot);
}

/**
 * Permet l'affiche de la Map a l'aide de gnuplot en creant un fichier
 * de commandes et en le remplissant.
 * @param width : largeur
 * @param height : hauteur
 */
void Graphics::printMap(double width, double height) {
    //Création du fichier de commandes
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if(gnuplot == NULL) {
        //printf("\n-----ERREUR-----\nIMPOSSIBLE D'OUVRIR LE FICHIER DANS LA CREATION DU FICHIER DE COMMANDE POUR GNUPLOT\n");
    }
    std::cout << "test : " << (width/height)*600 << std::endl;
    fprintf(gnuplot, "reset\n");
    fprintf(gnuplot, "set terminal qt size %d, %d font 'Helvetica,15'\n", (int)((width/height)*600), 600);
    fprintf(gnuplot, "unset key\n");
    fprintf(gnuplot, "set xlabel 'x (m)'\n");
    fprintf(gnuplot, "set ylabel 'y (m)'\n");
    fprintf(gnuplot, "set xrange[%f:%f]\n", 0.0, width);
    fprintf(gnuplot, "set yrange[%f:%f]\n", 0.0, height);
    fprintf(gnuplot, "set cbrange[%f:%f]\n", -82.0, -73.0);
    fprintf(gnuplot, "set pm3d map\n");
    //fprintf(gnuplot, "set palette defined ( 0 '#FF8000', 1 '#FFFF00', 2 '#80FF00', 3 '#00FF00', 4 '#00FF80', 5 '#00FFFF', 6 '#0080FF', 7 '#0000FF', 8 '#7F00FF', 9 '#FF00FF', 10 '#FF007F', 11 '#FF0000' )\n");
    //fprintf(gnuplot, "set palette rgbformulae 22,13,-31\n");
    for(int i = 0; i < commandLine.size(); i++) {
        fprintf(gnuplot, "%s", commandLine[i].c_str());
    }
    fprintf(gnuplot, "splot 'dataPower.txt' using 1:2:3 with pm3d\n");
    //fprintf(gnuplot, "plot 1/0\n");
    //fprintf(gnuplot, "splot 'dataPowerFinal1.txt' using 1:2:($3 >= -83 ? $3 : 'none') with pm3d\n");
    fclose(gnuplot);
}

/**
 * Permet l'affiche de la Map en Mbps a l'aide de gnuplot en creant un fichier
 * de commandes et en le remplissant.
 * @param width : largeur
 * @param height : hauteur
 */
void Graphics::printMapMbps(double width, double height) {
    //Création du fichier de commandes
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if(gnuplot == NULL) {
        //printf("\n-----ERREUR-----\nIMPOSSIBLE D'OUVRIR LE FICHIER DANS LA CREATION DU FICHIER DE COMMANDE POUR GNUPLOT\n");
    }
    std::cout << "test : " << (width/height)*600 << std::endl;
    fprintf(gnuplot, "reset\n");
    fprintf(gnuplot, "set terminal qt size %d, %d font 'Helvetica,15'\n", (int)((width/height)*600), 600);
    fprintf(gnuplot, "unset key\n");
    fprintf(gnuplot, "set xlabel 'x (m)'\n");
    fprintf(gnuplot, "set ylabel 'y (m)'\n");
    fprintf(gnuplot, "set xrange[%f:%f]\n", 0.0, width);
    fprintf(gnuplot, "set yrange[%f:%f]\n", 0.0, height);
    fprintf(gnuplot, "set cbrange[%f:%f]\n", 40.0, 320.0);
    fprintf(gnuplot, "set pm3d map\n");
    //fprintf(gnuplot, "set palette defined ( 0 '#FF8000', 1 '#FFFF00', 2 '#80FF00', 3 '#00FF00', 4 '#00FF80', 5 '#00FFFF', 6 '#0080FF', 7 '#0000FF', 8 '#7F00FF', 9 '#FF00FF', 10 '#FF007F', 11 '#FF0000' )\n");
    //fprintf(gnuplot, "set palette rgbformulae 22,13,-31\n");
    for(int i = 0; i < commandLine.size(); i++) {
        fprintf(gnuplot, "%s", commandLine[i].c_str());
    }
    fprintf(gnuplot, "splot 'dataPowerMbs.txt' using 1:2:3 with pm3d\n");
    //fprintf(gnuplot, "plot 1/0\n");
    //fprintf(gnuplot, "splot 'dataPowerFinal1.txt' using 1:2:($3 >= -83 ? $3 : 'none') with pm3d\n");
    fclose(gnuplot);
}