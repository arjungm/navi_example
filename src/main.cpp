#include <boost/program_options.hpp>
#include <iostream> 
#include <string>

int main(int argc, char** argv){

  //some pretty program options
  namespace po = boost::program_options; 
  po::options_description desc("Vanilla Navigation Planner Usage"); 
  desc.add_options() 
    ("env,e",po::value<std::string>()->required(),"input environment json file"); 

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
  po::notify(vm);

  return 0;
}
