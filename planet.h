#ifndef PLANET_H
#define PLANET_H
#include "vector2d.h"
#include <QColor>


struct Planet{
    Planet();

    Vector2d location;
    Vector2d velocity;
    double mass;
    QColor color;
    bool simulated = true;

};

std::ostream& operator << (std::ostream& out, Planet p);

#endif // PLANET_H
