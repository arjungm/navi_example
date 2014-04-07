navi_example
============

A simple C++ implementation of A* applied to a unbounded 2D grid world planning problems. It showcases the effectiveness of Jump Point Search in speeding up navigation planning in 2D grid world.

Jump Point Search can be read about here:
* http://zerowidth.com/2013/05/05/jump-point-search-explained.html
* http://harablog.wordpress.com/2011/09/07/jump-point-search/
* http://gamedevelopment.tutsplus.com/tutorials/how-to-speed-up-a-pathfinding-with-the-jump-point-search-algorithm--gamedev-5818

Uses Boost that comes with Ubuntu 12.04 installations. Needs boost filesystem, system and program options installed.

Build:

$ cd navi_example

$ make all

Usage:

$ ./navigate -e \<DATASETFILE\>
