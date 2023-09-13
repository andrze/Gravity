#ifndef DATA_H
#define DATA_H
#include <vector>
#include <fstream>
#include "planet.h"
#include "vector2d.h"

class Data
{
public:
    Data();
    Data(std::ifstream& stream, int forceExponent=2);
    Data(std::vector<Planet> planets, int forceExponent=2);

    std::vector<Planet> planets;
    double step;
    int forceExponent;

    std::vector<Vector2d> acceleration();
    Planet center_mass();
    void init();
    void move();
    void collide(size_t first, size_t second);
};

std::ostream& operator<<(std::ostream& out, Data data);

#endif // DATA_H
