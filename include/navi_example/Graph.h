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
     * @brief this function generates the successors according to the Jump Point Search extension to A*
     *
     * If the parent is null, then all possible directions are tried leading from the current state
     *
     * @param state the graph state to generate the successors for
     * @param parent the graph state that precedes the current state for determining direction fo approach
     * @param successors the list of successor states
     * @param costs the costs of reaching the successor states
     */
    void getJumpPointSuccessors( const GraphState::Ptr& state, const GraphState::Ptr& parent, vector<GraphState::Ptr>& successors, vector<double>& costs );
    /**
     * @brief helper function to perform the jump point search successor generation.
     *
     * firstly this function generates the neighbors that are forced
     * secondly function calls jumpHorizontallyVertically and jumpDiagonally depending on the passed in direction
     * and identifies the jump points this way
     *
     * @param state the state to generate successors from
     * @param dir the direction to jump in
     * @param successors the list of successors
     * @param costs the list of costs for the successors
     */
    void getJumpPointSuccessorsHelper( const GraphState::Ptr& state, const Direction& dir, vector<GraphState::Ptr>& successors, vector<double>& costs );
    /**
     * @brief helper function for jumping horiztonally or vertically; it stops when it collides head on
     * or if it detects the current state has a forced neighbor, or if current is a goal state
     *
     * @param state the state to generate successors from
     * @param dir the direction to jump in
     * @param jump the graphstate to jump to after it terminates; is null if no state is found
     * @param cost the cost to reach the jump point identified above
     * @param start_flag whether the state we are at is the first step in the jump search
     * @return whether a jump is possible
     */
    bool jumpHorizontallyVertically( const GraphState::Ptr& state, const Direction& dir, GraphState::Ptr& jump, double& cost, bool start_flag=false);
    /**
     * @brief helper function for jumping diagonally; it stops when it collides head on
     * or if it detects the current state has a forced neighbor, or if current is a goal state
     * or if it is unable to perform a horiztonal/vertical jump after the diagonal step
     *
     * @param state the state to generate successors from
     * @param dir the direction to jump in
     * @param jump the graphstate to jump to after it terminates; is null if no state is found
     * @param cost the cost to reach the jump point identified above
     * @param start_flag whether the state we are at is the first step in the jump search
     * @return whether a jump is possible
     */
    bool jumpDiagonally( const GraphState::Ptr& state, const Direction& dir, GraphState::Ptr& jump, double& cost, bool start_flag=false);
    /**
     * @brief checks if the current state and direction have a forced neighbor
     * @param state current state
     * @param dir direction heading in
     * @return whether it has a forced neighbor
     */
    bool hasForced (const GraphState::Ptr& state, const Direction& dir);
    /**
     * @brief checks if the current state and direction have a forced neighbor and records them if able
     * @param state current state
     * @param dir direction heading in
     * @param succs the list of forced neighbors
     * @param costs the list of costs to forced neighbors
     * @return whether it has a forced neighbor
     */
    bool getForced (const GraphState::Ptr& state,  const Direction& dir, vector<GraphState::Ptr>& succs, vector<double>& costs );
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
    bool verbose;
};

#endif
