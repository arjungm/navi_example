#ifndef PLANNER_H
#define PLANNER_H

#include <fstream>
#include <queue>
#include <functional>
#include <utility>

#include "navi_example/Environment.h"
#include "navi_example/Graph.h"

#include <boost/unordered_map.hpp>

using namespace std;

struct SearchState{
  typedef boost::shared_ptr<SearchState> Ptr;
  typedef boost::shared_ptr<const SearchState> ConstPtr;
  int g;
  int h;
  GraphState::Ptr graph_state_;
  SearchState::Ptr parent_;

  SearchState();
  GraphState::Ptr getGraphState();
  void setGraphState(const GraphState::Ptr& gstate);
};

bool operator==(SearchState::Ptr const& s1, SearchState::Ptr const& s2);
bool operator<(SearchState::Ptr const& lhs, SearchState::Ptr const& rhs);
size_t hash_value(SearchState::Ptr const& s);

class Planner{
  public:
    typedef boost::unordered_map<SearchState::Ptr, pair<SearchState::Ptr,bool> > HashTable;
    typedef boost::shared_ptr<Planner> Ptr;
    typedef boost::shared_ptr<Planner const> ConstPtr;
    Planner(Environment::Ptr env, Graph::Ptr graph);
    vector<GraphState::Ptr> plan();
    void unwind(const SearchState::Ptr& state, vector<GraphState::Ptr>& plan);
  private:
    Environment::Ptr env_;
    Graph::Ptr graph_;
    vector<SearchState::Ptr> open_list_;
    HashTable search_state_space_;
};


#endif

