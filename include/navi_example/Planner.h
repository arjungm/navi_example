#ifndef PLANNER_H
#define PLANNER_H

#include <fstream>
#include <queue>
#include <functional>

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
    Planner(Environment::Ptr env_, Graph::Ptr graph_);
    vector<GraphState::Ptr> plan(GraphState::Ptr start);
  private:
    Environment::Ptr env_;
    Graph::Ptr graph_;
    priority_queue<int, SearchState, greater<int> > open_list_;
};

#endif

