#ifndef COLIN_CPP_GRAPHICS_H
#define COLIN_CPP_GRAPHICS_H

#include <string>
#include <vector>


class Graphics {
public:
    std::vector<std::string> commandLine;

    Graphics();
    void printGraph(double width, double height);
    void drawLine(double x1, double x2, double y1, double y2, int type);
    void addPoint(double x, double y, int type);
    void printMap(double width, double height);
    void printMapMbps(double width, double height);

};


#endif //COLIN_CPP_GRAPHICS_H
