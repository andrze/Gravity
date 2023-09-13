#include "vector2d.h"
#include <ostream>

Vector2d::Vector2d(){
}
Vector2d::Vector2d(double x, double y){
    this->x = x;
    this->y = y;
}

Vector2d& Vector2d::operator+= (Vector2d rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}
Vector2d& Vector2d::operator*= (double rhs) {
    x *= rhs;
    y *= rhs;
    return *this;
}
Vector2d& Vector2d::operator-= (Vector2d rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}
Vector2d& Vector2d::operator/= (double rhs) {
    x /= rhs;
    y /= rhs;
    return *this;
}

Vector2d operator + (Vector2d lhs, Vector2d rhs) {
    return lhs += rhs;
}
Vector2d operator - (Vector2d lhs, Vector2d rhs) {
    return lhs -= rhs;
}
Vector2d operator * (double lhs, Vector2d rhs) {
    return rhs *= lhs;
}
Vector2d operator * (Vector2d lhs, double rhs) {
    return lhs *= rhs;
}
Vector2d operator / (Vector2d lhs, double rhs) {
    return lhs /= rhs;
}

double operator * (Vector2d lhs, Vector2d rhs) {
    return lhs.x*rhs.x+lhs.y*rhs.y;
}

std::ostream& operator << (std::ostream& out, Vector2d v) {
    out << "X: " << v.x << " Y: " << v.y;
    return out;
}
