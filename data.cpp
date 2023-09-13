#include "data.h"
#include "planet.h"
#include <cmath>
#include <array>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

Data::Data(){

}

Data::Data(std::ifstream &stream, int forceExponent){
    step = 0.0001;
    this->forceExponent = forceExponent;
    std::string line;
    while(getline(stream, line)){
        std::istringstream sstream(line);
        Planet planet;
        sstream >> planet.location.x >> planet.location.y;
        sstream >> planet.velocity.x >> planet.velocity.y;
        sstream >> planet.mass;
        int red, green, blue;
        sstream >> red >> green >> blue;
        planet.color.setRed(std::abs(red%256));
        planet.color.setGreen(std::abs(green%256));
        planet.color.setBlue(std::abs(blue%256));
        planets.push_back(planet);
        if(!sstream){
            throw std::invalid_argument("Invalid file formatting");
        }
    }

}

Data::Data(std::vector<Planet> planets, int forceExponent){
    step = 0.0001;
    this->planets = planets;
    this->forceExponent = forceExponent;
}

Planet Data::center_mass(){
    Planet center_mass;
    for(auto&& planet: planets){
        center_mass.location += planet.mass*planet.location;
        center_mass.velocity += planet.mass*planet.velocity;
        center_mass.mass += planet.mass;
    }
    center_mass.location /= center_mass.mass;
    center_mass.velocity /= center_mass.mass;
    return center_mass;
}

std::vector<Vector2d> Data::acceleration(){
    size_t n_planets = planets.size();
    Vector2d vector0(0.,0.);
    std::vector<Vector2d> acceleration_(n_planets,vector0);
    for(size_t i=0; i<n_planets; i++){
        if (planets[i].simulated){
            for(size_t j=i+1; j<n_planets; j++){
                if (planets[j].simulated){
                    Vector2d distance = planets[i].location-planets[j].location;
                    auto distance2 = distance*distance;
                    auto acc = Vector2d();
                    switch(forceExponent){
                        case(0):
                            acc = distance * pow(distance2,-0.5);
                            break;
                        case(1):
                            acc = distance / distance2;
                            break;
                        case(3):
                            acc = distance * pow(distance2,-2);
                            break;
                        default:
                            acc = distance * pow(distance2,-1.5);
                            break;
                    }

                    acceleration_[i] -= planets[j].mass * acc;
                    acceleration_[j] += planets[i].mass * acc;
                }
            }
        }
    }
    return acceleration_;
}

void Data::init(){
    Planet c_mass = center_mass();
    for(auto&& planet: planets){
        planet.location -= c_mass.location;
        planet.velocity -= c_mass.velocity;
    }
}

void Data::move(){
    size_t n_planets = planets.size();
    std::array<std::vector<Vector2d>,4> kr, kv;
    for (size_t i=0; i<n_planets; i++){
        for (size_t j=i+1; j<n_planets; j++){
            if (planets[i].simulated && planets[j].simulated){
                Vector2d distance = planets[i].location-planets[j].location;
                auto distance2 = distance*distance;
                if(distance2 < 0.01){
                    collide(j, i);
                }
            }
        }
    }
    Data temp_data = *this;
    for(int j=0; j<4; j++){
        kv[j] = temp_data.acceleration();
        for(size_t i=0; i<n_planets; i++){
            kr[j].push_back(temp_data.planets[i].velocity);
            if(j<2){
                temp_data.planets[i].location = planets[i].location + step*0.5*kr[j][i];
                temp_data.planets[i].velocity = planets[i].velocity + step*0.5*kv[j][i];
            } else if(j==2){
                temp_data.planets[i].location = planets[i].location + step*kr[j][i];
                temp_data.planets[i].velocity = planets[i].velocity + step*kv[j][i];
            } else{
                planets[i].location += step/6*(kr[0][i]+2*kr[1][i]+2*kr[2][i]+kr[3][i]);
                planets[i].velocity += step/6*(kv[0][i]+2*kv[1][i]+2*kv[2][i]+kv[3][i]);
            }
        }
    }
}

void Data::collide (size_t i, size_t j){
    double new_mass = planets[i].mass + planets[j].mass;
    planets[j].location = (planets[i].location*planets[i].mass+planets[j].location*planets[j].mass)/new_mass;
    planets[j].velocity = (planets[i].velocity*planets[i].mass+planets[j].velocity*planets[j].mass)/new_mass;
    planets[j].mass = new_mass;
    planets[j].color = QColor((planets[j].color.red()+planets[i].color.red())%255,
                              (planets[j].color.green()+planets[i].color.green())%255,
                              (planets[j].color.blue()+planets[i].color.blue())%255);
    planets[i].simulated = false;
}

std::ostream& operator<<(std::ostream& out, Data data) {
    for(auto&& planet : data.planets){
        out << planet.location.x << ' ' << planet.location.y << ' ';
        out << planet.velocity.x << ' ' << planet.velocity.y << ' ';
        out << planet.mass << ' ';
        out << planet.color.red() << ' ' << planet.color.green() << ' ' << planet.color.blue() << std::endl;
    }
    return out;
}
