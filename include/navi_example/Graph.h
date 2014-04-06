#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "navi_example/Environment.h"

using namespace std;

struct GraphState{
    typedef boost::shared_ptr<GraphState> Ptr;
    typedef boost::shared_ptr<const GraphState> ConstPtr;
    Cell coords;
    
    GraphState( Cell coordinates );
    GraphState& operator=( const GraphState& other );
};

ostream& operator<<(ostream& os, const GraphState& s);

class Graph{
  public:
    typedef boost::shared_ptr<Graph> Ptr;
    typedef boost::shared_ptr<const Graph> ConstPtr;
     
    Graph(Environment::Ptr env);
    double getHeuristicCost( const GraphState::Ptr& state );
    void getValidSuccessors( const GraphState::Ptr& state, vector<GraphState::Ptr>& successors, vector<double>& costs );
    bool isGoalState( const GraphState::Ptr& state );
    GraphState::Ptr getStart();
  private:
    Environment::Ptr env_;
};

#endif
