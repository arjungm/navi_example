#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "navi_example/Environment.h"

using namespace std;

struct GraphState{
  Cell coords;
};

class Graph{
  public:
    typedef boost::shared_ptr<Graph> Ptr;
    typedef boost::shared_ptr<const Graph> ConstPtr;
     
    Graph(Environment::Ptr env);
    int getHeuristicCost( const GraphState& state );
    void getValidSuccessors( const GraphState& state, vector<GraphState>& successors, vector<int>& costs );
    bool isGoalState( const GraphState& state );
  private:
    Environment::Ptr env_;
};

#endif
