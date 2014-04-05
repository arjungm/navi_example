#include <navi_example/Planner.h>

SearchState::SearchState() : g(0), h(0)
{
}

int SearchState::operator<(const SearchState& other) const{
    return (g+h)<(other.g+other.h);
}

bool operator==(SearchState::Ptr const& s1, SearchState::Ptr const& s2){
    return s1->graph_state_->coords == s2->graph_state_->coords;
}
size_t hash_value(SearchState::Ptr const& s){
    return 0;
}

Planner::Planner(Environment::Ptr env, Graph::Ptr graph): env_(env), graph_(graph)
{
    //initialize the priority queue
    SearchState start_state;
    start_state.graph_state_->coords.x = env_->getStart()->x;
    start_state.graph_state_->coords.y = env_->getStart()->y;
    
    start_state.h =  graph_->getHeuristicCost( *(start_state.graph_state_) );
    open_list_.push(start_state);
}

vector<GraphState::Ptr> Planner::plan(GraphState::Ptr start){
    vector<GraphState::Ptr> path;
    
    while(!open_list_.empty()){
        //pop off open_list_
        SearchState current = open_list_.top();
        open_list_.pop();

        //check if goal
        if(graph_->isGoalState( *current.graph_state_ )){
            //unwind path
            //terminate search
            cout << "Done!" << endl;
        }
        else{
            //add current to closed
            
            //generate succs
            vector<int> costs;
            vector<GraphState> successors;
            graph_->getValidSuccessors( *current.graph_state_, successors, costs );
            
            //check succs in open and closed list
            for(size_t i=0; i<successors.size(); i++){
                //not in closed list
                
                SearchState succ;
                succ.g = current.g + costs[i];

                //not in open list
            }
            //add to open list
            //check for inconsistencies
        }
    }
     
    return path;    
}
