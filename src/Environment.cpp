#include "navi_example/Environment.h"

#include <sstream>
#include <ostream>
#include <boost/foreach.hpp>

using namespace std;

ostream& operator<<(ostream& os, const Point& pt)
{
  os << "(" << pt.x << ", " << pt.y << ")";
  return os;
}

Environment::Environment( ifstream& ifs ){
  //read the file in
  stringstream buffer;
  buffer << ifs.rdbuf();
  ifs.close();

  //populate parse tree
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(buffer, pt);
  
  //fill in the environment
  start_ = readTuple( pt.get_child("robotStart") );
  goal_ = readTuple( pt.get_child("robotEnd") );

  cout << "Start:" << start_ << endl;
  cout << "Goal:" << goal_ << endl;
  
  //put the obstacles
  BOOST_FOREACH( const boost::property_tree::ptree::value_type& obstacle, pt.get_child("obstacles")){
    int coord[2];
    int ind = 0;
    BOOST_FOREACH( const boost::property_tree::ptree::value_type& coordinates, obstacle.second ){
      coord[ind++]=coordinates.second.get_value<double>();
    }
    Point p(coord[0], coord[1]);
    obstacles_.insert(p);
  }
}

Point Environment::readTuple( boost::property_tree::ptree& node ){
  int coord[2]; size_t counter = 0;
  BOOST_FOREACH( const boost::property_tree::ptree::value_type& child, node)
    coord[counter++] = child.second.get_value<double>();

  Point xy(coord[0], coord[1]);
  return xy;
}
