#include <iostream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "navi_example/Nav2DGraph.h"

using namespace std;

int main(int argc, char** argv){

  //some program options boilerplate
  namespace po = boost::program_options; 
  po::options_description desc("Vanilla Navigation Planner Usage"); 
  desc.add_options() 
    ("env,e",po::value<string>()->required(),"input environment json file"); 
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
  po::notify(vm);

  //open the json
  string json_filename =  vm["env"].as<string>();
  ifstream input_json_file;
  input_json_file.open( json_filename.c_str() );

  if( input_json_file ){
    //use the json file to construct the environment
    Nav2DGraph graph;
    graph.readDescription( input_json_file );

    input_json_file.close();
    //plan on the environment
    //Planner plnr(env);
  }

  return 0;
}
