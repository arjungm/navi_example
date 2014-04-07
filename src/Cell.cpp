#include "navi_example/Cell.h"
#include <cmath>

Cell::Cell() : x(0), y(0) {}
Cell::Cell(int cx, int cy) : x(cx), y(cy) {}
Cell::Cell(vector<int> coords) : x(0), y(0) {
    //TODO throw exception if wrong size?
    x = coords[0];
    y = coords[1];
}

bool operator==(Cell const& c1, Cell const& c2){
    return (c1.x==c2.x) && (c1.y == c2.y);
}

size_t hash_value(Cell const& c) {
    size_t seed = 0;
    boost::hash_combine(seed, c.x);
    boost::hash_combine(seed, c.y);
    return seed;
}
Cell& Cell::operator=(const Cell& other){
    x = other.x;
    y = other.y;
    return *this;
}

ostream& operator<<(ostream& os, const Cell& cell)
{
  os << cell.x << ", " << cell.y;
  return os;
}


Direction::Direction() : dx_(1), dy_(0) {}
Direction::Direction(double dx, double dy) : dx_(dx), dy_(dy) {}
Direction Direction::dot(const Direction& d) const{
    return Direction(dx_*d.dx_, dy_*d.dy_);
}
double Direction::getAngle() const{
    return atan2(dy_,dx_);
}
void Direction::rotate(double angle){
    double theta = getAngle() + angle;
    dx_ = round(cos(theta));
    dy_ = round(sin(theta));
}
double Direction::getX() const { return dx_; }
double Direction::getY() const { return dy_; }
bool Direction::isDiagonal() const { return dx_!=0&&dy_!=0; }
double Direction::norm() const { return sqrt(pow(dx_,2)+pow(dy_,2)); }

Cell operator+(Cell lhs, const Direction& rhs){
    lhs.x += rhs.getX();
    lhs.y += rhs.getY();
    return lhs;
}

Direction operator-(Cell lhs, Cell rhs){
    double x,y;
    if(lhs.x > rhs.x)
        x = 1;
    else if(lhs.x < rhs.x)
        x = -1;
    else
        x = 0;
    
    if(lhs.y > rhs.y)
        y = 1;
    else if(lhs.y < rhs.y)
        y = -1;
    else
        y = 0;

    return Direction(x,y);
}

ostream& operator<<(ostream& os, const Direction& dir){
    os << "(" << dir.getX() << "," << dir.getY() << ")";
    return os;
}

