#ifndef ENV_H
#define ENV_H

#include <fstream>
#include <set>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;

struct Point{
  int x;
  int y;
  Point() : x(0), y(0) {}
  Point(int xx, int yy) : x(xx), y(yy) {}
};

struct PointComp {
  bool operator() (const Point& lhs, const Point& rhs) const {return (lhs.x+lhs.y)<(rhs.x+rhs.y);}
};

class Environment{
  public:
    Environment( ifstream& json );
  private:
    Point readTuple( boost::property_tree::ptree& node );
    boost::property_tree::ptree pt_;

    Point start_;
    Point goal_;
    set<Point, PointComp> obstacles_;
};

#endif
