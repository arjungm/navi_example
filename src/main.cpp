#include <iostream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "navi_example/Environment.h"
#include "navi_example/Graph.h"
#include "navi_example/Planner.h"

using namespace std;
/**
 * @brief main function
 * 
 * Uses boost program options to read file
 * Outputs to _sol.txt file
 */
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
  boost::filesystem::path json_file( vm["env"].as<string>() );
  ifstream input_json_file;
  if(boost::filesystem::exists(json_file)){
     input_json_file.open( json_file.string().c_str() );
  }
  else{
      printf("File \"%s\" does not exist to be read!\n", json_file.string().c_str());
  }

  if( input_json_file ){
    //use the json file to construct the environment
    Environment::Ptr env = boost::make_shared<Environment>();
    env->readDescription( input_json_file );

    input_json_file.close();

    //plan on the environment
    Graph::Ptr graph = boost::make_shared<Graph>(env);
    Planner::Ptr plnr = boost::make_shared<Planner>(env, graph);

    vector<GraphState::Ptr> path;
    
    //call planner
    bool plannerResult = plnr->plan(path);
    
    if(plannerResult){
        //output plan to file
        boost::filesystem::path parent_dir = json_file.parent_path();
        boost::filesystem::path solution_filename(json_file.stem().string()+"_sol.txt");
        boost::filesystem::path solution_filepath = parent_dir / solution_filename;

        printf("Writing out solution to: %s\n", solution_filepath.string().c_str());
        ofstream ofs;
        ofs.open( solution_filepath.string().c_str() );
        
        for(size_t i=0; i<path.size(); i++){
            ofs << *(path[i]) << endl;
        }
        ofs.close();
    }
    else{
        //
        cout << "No plan found!" << endl;
    }
  }

  return 0;
}
