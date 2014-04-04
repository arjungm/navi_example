#ifndef PLANNER_H
#define PLANNER_H

#include <fstream>

#include "navi_example/Environment.h"
#include "navi_example/Graph.h"

using namespace std;

struct SearchState{
  typedef boost::shared_ptr<SearchState> Ptr;
  typedef boost::shared_ptr<const SearchState> ConstPtr;
  int g;
  int h;
  GraphState::Ptr graph_state_;
  SearchState::Ptr parent_;
}

class Planner{
  public:
  private:
};

#endif

