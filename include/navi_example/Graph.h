#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "navi_example/Environment.h"

using namespace std;

/**
 * @brief Wrapper class for Cell class.
 * 
 * GraphState represents the graph representation of the
 * real world state. In our case our real world state is
 * simply a discrete state. If it were continuous coordinates
 * then GraphState would take care of discretization of the
 * state space
 */
struct GraphState{
    typedef boost::shared_ptr<GraphState> Ptr;
    typedef boost::shared_ptr<const GraphState> ConstPtr;
    /**
     * @brief Environment coordinates
     */
    Cell coords;
    /**
     * @brief Constructor that takes in coordinates as a cell
     * @param coordinates coordinates in the environment
     */
    GraphState( Cell coordinates );
    /**
     * @brief Assignment operator overload
     */
    GraphState& operator=( const GraphState& other );
};

/**
 * @brief operator<< overload for iostream for GraphState
 * @param os output stream
 * @param s GraphState instance
 * @return output stream
 */
ostream& operator<<(ostream& os, const GraphState& s);

/**
 * @brief Intermediate class between environment and planner
 *
 * Graph handles generating successors from a given state in
 * the graph and it also computers the heuristic for a given
 * state in the graph.
 *
 * In order to extend Graph to have more nuanced motion primitves
 * then a MotionPrimitive class should be implemented, which would
 * be used by Environment to generate real world successors. Graph
 * would then discretize them to the appropriate GraphState.
 */
class Graph{
  public:
    typedef boost::shared_ptr<Graph> Ptr;
    typedef boost::shared_ptr<const Graph> ConstPtr;
     
    /**
     * @brief Constructor for Graph that takes in an environment pointer
     */
    Graph(Environment::Ptr env);
    /**
     * @brief gets the heuristic cost to the goal from the current state
     *
     * Uses euclidean distance between the coordinates of current and goal state
     * @param state current state pointer
     * @return heuristic cost to the goal state
     */
    double getHeuristicCost( const GraphState::Ptr& state );
    /**
     * @brief gets the connected successors from the current state
     *
     * Uses the 8-connected fashion for a given cell to determine its neighbors. 
     * Then it checks if those neighbors are collision free.
     * Cost of the motion is the Grid world displacement in the coordinates
     *
     * @param state current state
     * @param successors variable size list of GraphState pointers that are valid (collision free) neighbors
     * @param costs variable size list of costs for moving from the current state to the successor. For diagonal motions the cost is higher.
     */
    void getValidSuccessors( const GraphState::Ptr& state, vector<GraphState::Ptr>& successors, vector<double>& costs );
    /**
     * @brief checks if given state is a goal state
     * @param state current state
     * @return whether the current state is a goal
     */
    bool isGoalState( const GraphState::Ptr& state );
    /**
     * @brief gets the start state
     * @return pointer to the start state
     */
    GraphState::Ptr getStart();
  private:
    /**
     * @brief Pointer to real world environment object
     *
     * used for collision checking generated GraphState
     */
    Environment::Ptr env_;
};

#endif
