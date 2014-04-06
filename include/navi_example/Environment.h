#ifndef ENV_H
#define ENV_H

#include <fstream>
#include <set>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <boost/functional/hash.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace std;
/**
 * @brief Struct to wrap grid world coordinates
 */
struct Cell{
  typedef boost::shared_ptr<Cell> Ptr;
  typedef boost::shared_ptr<const Cell> ConstPtr;
  /**
   * @brief x coordinate
   */
  int x;
  /**
   * @brief y coordinate
   */
  int y;
  /**
   * @brief Empty constructor. 
   *
   * X and Y are set to 0.
   */
  Cell();
  /**
   * @brief Constructor that takes individual coordinates
   *
   * @param cx x coordinate
   * @param cy y coordinate
   */
  Cell(int cx, int cy);
  /**
   * @brief Constructor that takes coordinates as a vector
   *
   * TODO If the number of elements in the vector are wrong
   * then it should throw an exception.
   * @param coords vector of coordinates
   */
  Cell(vector<int> coords);
  /**
   * @brief Assignment operator overload
   *
   * @param other another Cell instance
   */
  Cell& operator=(const Cell& other);
};

/**
 * @brief Operator overload for <<
 *
 * Used for easily printing out the coordinates
 * to an appropriate iostream
 * @param os ostream object (can be cout or ofstream)
 * @param cell Cell instance
 */
ostream& operator<<(ostream& os, const Cell& cell);
/**
 * @brief Comparison operator overload for Cell
 *
 * @param c1 left hand side Cell object
 * @param c2 right hand side Cell object
 * @return verity of the equality
 */
bool operator==(Cell const& c1, Cell const& c2);
/**
 * @brief overloading hash function for boost for Cell
 *
 * used for hashing when combined with boost hashtable
 * @param c Cell for computing hash value
 */
size_t hash_value(Cell const& c);

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

#endif
