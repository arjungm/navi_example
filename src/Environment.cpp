#include "navi_example/Environment.h"

#include <sstream>
#include <ostream>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;

Environment::Environment(){
}

void Environment::readDescription( const ifstream& json ){
  //read the file in
  stringstream buffer;
  buffer << json.rdbuf();
  //populate parse tree
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(buffer, pt);
  
  //fill in the environment
  start_ = boost::make_shared<Cell>( readCoordinates( pt.get_child("robotStart") ) );
  goal_ = boost::make_shared<Cell>( readCoordinates( pt.get_child("robotEnd") ) );

  cout << "Start:" << *start_ << endl;
  cout << "Goal:" << *goal_ << endl;
  
  //put the obstacles
  BOOST_FOREACH( const boost::property_tree::ptree::value_type& obstacle, pt.get_child("obstacles")){
    vector<int> coords;
    BOOST_FOREACH( const boost::property_tree::ptree::value_type& coordinates, obstacle.second ){
      coords.push_back(coordinates.second.get_value<double>());
    }
    Cell::Ptr obs_cell = boost::make_shared<Cell>(coords);
    obstacles_.insert(*obs_cell);
  }
}

bool Environment::isCollisionFree( const Cell& cell ){
   return (obstacles_.count(cell)==0); 
}


vector<int> Environment::readCoordinates( boost::property_tree::ptree& node ){
  vector<int> coord;
  BOOST_FOREACH( const boost::property_tree::ptree::value_type& child, node)
    coord.push_back(child.second.get_value<double>());
  return coord;
}

Cell::Ptr Environment::getGoal(){
    return goal_;
}

Cell::Ptr Environment::getStart(){
    return start_;
}

ostream& operator<<(ostream& os, const Environment& env){
    env.printStart(os);
    env.printGoal(os);
    env.printObstacles(os);
    return os;    
}

void Environment::printObstacles(ostream& os) const {
    for(boost::unordered_set<Cell, boost::hash<Cell> >::iterator obs_it = obstacles_.begin();
            obs_it != obstacles_.end(); ++obs_it){
        os << *obs_it << endl;
    }
}
void Environment::printStart(ostream& os) const {
    os << *(start_) << endl;
}
void Environment::printGoal(ostream& os) const {
    os << *goal_ << endl;
}
