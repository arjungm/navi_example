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

/**
 * @brief Wrapper struct for GraphState which bundles it with g and h values
 *
 * This class is used in the actual search algorithm and helps with path
 * unwinding.
 */
struct SearchState{
  typedef boost::shared_ptr<SearchState> Ptr;
  typedef boost::shared_ptr<const SearchState> ConstPtr;
  /**
   * @brief cost to come for the state
   */
  double g;
  /**
   * @brief cost to go for the state
   */
  double h;
  /**
   * @brief the underlying graph state of the search state
   */
  GraphState::Ptr graph_state_;
  /**
   * @brief pointer to the parent search state that generated this
   */
  SearchState::Ptr parent_;
  /**
   * @brief Empty constructor
   *
   * Defaults coordinates to (0,0) and g and h to 0
   */
  SearchState();
  /**
   * @brief getter for the GraphState
   * @return pointer to GraphState
   */
  GraphState::Ptr getGraphState();
  /**
   * @brief setter for the GraphState
   * @param gstate pointer to the graph state to set it to
   */
  void setGraphState(const GraphState::Ptr& gstate);
};

/**
 * @brief operator overload for SearchState* for equality comparison
 *
 * required for using boost unordered_set and map
 *
 * @param s1 first state to compare
 * @param s2 second state to compare
 * @return verity of the comparison
 */
bool operator==(SearchState::Ptr const& s1, SearchState::Ptr const& s2);
/**
 * @brief operator overload for SearchState* for less than comparison
 *
 * Used by the sorting functions of the priority queue
 *
 * @param s1 first state to compare
 * @param s2 second state to comparei
 * @return verity of the comparison
 */
bool operator<(SearchState::Ptr const& lhs, SearchState::Ptr const& rhs);
/**
 * @brief boost hash function overload for SearchState*, uses the Cell hash functions
 *
 * This is used by the HashTable for tracking whether a state is on the open or closed list
 *
 * @return hash value of the SearchState
 * @param s state to be hashed
 */
size_t hash_value(SearchState::Ptr const& s);

/**
 * @brief Planner class that uses the Environment and Graph to conduct a search
 * 
 * Planner uses Graph to get successors and compute heuristics. It manages the priority
 * queue for the search (sorting based on g+h) and also the closed list by using a
 * boost hash table that maps to boolean values of whether a state is open/closed
 */
class Planner{
  public:
    typedef boost::unordered_map<SearchState::Ptr, pair<SearchState::Ptr,bool> > HashTable;
    typedef boost::shared_ptr<Planner> Ptr;
    typedef boost::shared_ptr<Planner const> ConstPtr;

    /**
     * @brief constructor for the planner
     * @param env Environment pointer
     * @param graph Graph pointer
     */
    Planner(Environment::Ptr env, Graph::Ptr graph);
    /**
     * @brief starts the planner to search for a path
     * @return whether a path was found
     * @param path the path in GraphStates from start to goal
     */
    bool plan(vector<GraphState::Ptr>& path);
    /**
     * @brief helper function for unrolling the discovered path
     *
     * Uses parent pointers to previous states to build the path
     *
     * @param state the state to start unrolling from
     * @param plan the list of graph states in the plan
     */
    void unwind(const SearchState::Ptr& state, vector<GraphState::Ptr>& plan);
  private:
    /**
     * @brief Environment pointer with the start, goal, and obstacle information
     */
    Environment::Ptr env_;
    /**
     * @brief Graph pointer with the successor function, heuristic function for search
     */
    Graph::Ptr graph_;
    /**
     * @brief heap data structure using STL vector
     *
     * makes use of std::make_heap, std::push_heap, and std::pop_heap 
     * for standard heap operations
     */
    vector<SearchState::Ptr> open_list_;
    /**
     * @brief a Hashtable that maps a SearchState* to a pair<SearchState*, bool>
     * 
     * The SearchState* is hashed using the same hashing as GraphState (and Cell)
     * The mapped value is a pair because the second pointer points to the exact same
     * object in the priority queue for updateKey operations to be enabled.
     *
     * The boolean value is whether the state is on the open, or closed list.
     *
     * If no Value is returned for a given Key, then this is a brand new state generated
     */
    HashTable search_state_space_;
};


#endif

