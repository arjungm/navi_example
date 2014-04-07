#ifndef ENV_H
#define ENV_H

#include <fstream>
#include <set>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <boost/property_tree/ptree.hpp>

#include "navi_example/Cell.h"

using namespace std;
/**
 * @brief Class for the unbounded 2D gridded environment
 *
 * It handles collision checking of Cells, and maintains a set of
 * Cells that are occupied for fast collision checking.
 * It also stores the start and goal cells.
 */
class Environment{
  public:
    typedef boost::shared_ptr<Environment> Ptr;
    typedef boost::shared_ptr<const Environment> ConstPtr;

    /**
     * @brief Empty constructor
     */
    Environment();
    /**
     * @brief reads in the environment description from json file
     *
     * records all the obstacles and start and goal coordinates
     * @param json ifstream of the json file
     */
    void readDescription( const ifstream& json );
    /**
     * @brief checks if a given Cell is unoccupied
     * @param cell Cell to be checked
     * @return whether it is free
     */
    bool isCollisionFree( const Cell& cell );
    /**
     * @brief helper function for reading in coordinates from the property tree
     * @see readDescription()
     * @param node boost property tree containing two integers
     * @return variable length vector of coordinates (should always be 2 if the file is good)
     */
    vector<int> readCoordinates( boost::property_tree::ptree& node );
    /**
     * @brief getter for goal cell
     * @return boost shared pointer to goal Cell
     */
    Cell::Ptr getGoal();
    /**
     * @brief getter for start cell
     * @return boost shared pointer to start Cell
     */
    Cell::Ptr getStart();
    /**
     * @brief prints obstacles to ostream
     * @param ostream to print to
     */
    void printObstacles(ostream& os) const;
    /**
     * @brief prints start to ostream
     * @param ostream to print to
     */
    void printStart(ostream& os) const;
    /**
     * @brief prints goal to ostream
     * @param ostream to print to
     */
    void printGoal(ostream& os) const;

  private:
    /**
     * @brief Shared pointer to starting Cell location
     */
    Cell::Ptr start_;
    /**
     * @brief Shared pointer to ending Cell location
     */
    Cell::Ptr goal_;
    /**
     * @brief Set of Cells that are occupied
     *
     * used for fast collision checking
     * O(1) time for contains (count) operation
     */
    boost::unordered_set<Cell, boost::hash<Cell> > obstacles_;
};

ostream& operator<<(ostream& os, const Environment& env);

#endif
