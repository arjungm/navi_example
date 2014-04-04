#ifndef ENV_H
#define ENV_H

#include <fstream>
#include <set>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>

using namespace std;

struct Cell{
  int x;
  int y;
  Cell(int cx, int cy);
}

class Environment{
  public:
    typedef boost::shared_ptr<Environment> Ptr;
    typedef boost::shared_ptr<const Environment> ConstPtr;

    Environment();
    void readDescription( const ifstream& json );
    bool isCollisionFree( const Cell& cell );

  private:
    Cell start_;
    Cell goal_;
    boost::unordered_set<Cell> obstacles_;
};

#endif
