#include "planet.h"
#include <vector>
#include <ostream>
#include <QColor>

Planet::Planet()
{
}

std::ostream& operator<<(std::ostream& out, Planet p) {
    out << "Position: " << p.location << std::endl;
    out << "Velocity: " << p.velocity << std::endl;
    out << "Mass: " << p.mass << std::endl;
    out << "Color: " << "R: " << p.color.red() << " G: " << p.color.green() << " B: " << p.color.blue() << std::endl;
    out << "Is simulated: " << p.simulated << std::endl;
    return out;
}
