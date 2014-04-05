#ifndef ENV_H
#define ENV_H

#include <fstream>
#include <set>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <boost/functional/hash.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace std;

struct Cell{
  typedef boost::shared_ptr<Cell> Ptr;
  typedef boost::shared_ptr<const Cell> ConstPtr;
  int x;
  int y;
  Cell();
  Cell(int cx, int cy);
  Cell(vector<int> coords);
};

class Environment{
  public:
    typedef boost::shared_ptr<Environment> Ptr;
    typedef boost::shared_ptr<const Environment> ConstPtr;

    Environment();
    void readDescription( const ifstream& json );
    bool isCollisionFree( const Cell& cell );
    vector<int> readCoordinates( boost::property_tree::ptree& node );
    Cell::Ptr getGoal();
    Cell::Ptr getStart();

  private:
    Cell::Ptr start_;
    Cell::Ptr goal_;
    boost::unordered_set<Cell, boost::hash<Cell> > obstacles_;
};

ostream& operator<<(ostream& os, const Cell& cell);
bool operator==(Cell const& c1, Cell const& c2);
size_t hash_value(Cell const& c);

#endif
