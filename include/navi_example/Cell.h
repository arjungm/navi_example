#include <vector>
#include <ostream>
#include <boost/shared_ptr.hpp>
#include <boost/functional/hash.hpp>

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
 * @brief Direction class that represents the 8 possible motions
 * in the grid.
 *
 * It supports simple arithmetic like Cell+Direction = Cell and
 * Cell-Cell = Direction
 */
class Direction{
    public:
        typedef boost::shared_ptr<Direction> Ptr;
        typedef boost::shared_ptr<const Direction> ConstPtr;
        /**
         * @brief Empty constructor
         */
        Direction();
        /** 
         * @brief Constructor that takes in direction args
         */
        Direction(double dx, double dy);
        /**
         * @brief returns the projection of this vector onto
         * the other vector
         *
         * @param d the other direction vector
         * @return the vector this is projected to
         */
        Direction dot(const Direction& d) const;
        /**
         * @brief gets the angular direction of the vector
         * @return the angle in radians
         */
        double getAngle() const;
        /**
         * @brief rotates the current direction by angle
         * @param angle the amount to rotate the current vector by
         */
        void rotate(double angle);
        /**
         * @brief gets the x component of the vector
         * @return the x component
         */
        double getX() const;
        /**
         * @brief gets the y component of the vector
         * @return the y component
         */
        double getY() const;
        /**
         * @brief check if the direction is a diagonal direction
         * @return verity of whether it is diagonal
         */
        bool isDiagonal() const;
        /**
         * @brief returns the length of the direction
         * @return the length of the direction vector
         */
        double norm() const;
    private:
        /**
         * @brief x component
         */
        double dx_;
        /**
         * @brief y component
         */
        double dy_;
};

/**
 * @brief operator overload for "Cell minus Cell = Direction"
 * @return the direction from rhs to lhs
 * @param lhs left hand side Cell
 * @param rhs right hand side Cell
 */
Direction operator-(Cell lhs, Cell rhs);
/**
 * @brief operator overload for "Cell plus Direction = Cell"
 * @return the new Cell
 * @param lhs the cell to be moved from
 * @param rhs the direction to move along
 */
Cell operator+(Cell lhs, const Direction& rhs);
/**
 * @brief operator overload for << and printing out Directions
 */
ostream& operator<<(ostream& os, const Direction& dir);
