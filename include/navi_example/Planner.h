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

  SearchState();
  int operator<(const SearchState& other) const;
};

class Planner{
  public:
    Planner(Environment::Ptr env_, Graph::Ptr graph_);
    vector<GraphState::Ptr> plan(GraphState::Ptr start);
  private:
    Environment::Ptr env_;
    Graph::Ptr graph_;
    priority_queue<SearchState> open_list_;
    boost::unordered_set<SearchState::Ptr, boost::hash<SearchState::Ptr> > search_state_space_;
};

bool operator==(SearchState::Ptr const& s1, SearchState::Ptr const& s2);
size_t hash_value(SearchState::Ptr const& s);

#endif

