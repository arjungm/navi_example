#include <navi_example/Planner.h>

#include <boost/make_shared.hpp>
#include <algorithm>

using namespace std;

SearchState::SearchState() : g(0), h(0)
{
}

bool operator==(SearchState::Ptr const& s1, SearchState::Ptr const& s2){
    return s1->graph_state_->coords == s2->graph_state_->coords;
}
size_t hash_value(SearchState::Ptr const& s){
    return 0;
}
bool operator<(SearchState::Ptr const& lhs, SearchState::Ptr const& rhs){
    return (lhs->g + lhs->h)<(rhs->g + rhs->h);
}
GraphState::Ptr SearchState::getGraphState(){
    return graph_state_;
}
void SearchState::setGraphState( const GraphState::Ptr& gstate ){
    graph_state_ = gstate;
}

Planner::Planner(Environment::Ptr env, Graph::Ptr graph): env_(env), graph_(graph)
{
    //initialize the priority queue
    SearchState::Ptr start_state = boost::make_shared<SearchState>();
    start_state->setGraphState( graph_->getStart() );
    
    start_state->h =  graph_->getHeuristicCost( start_state->getGraphState() );
    search_state_space_[start_state] = make_pair(start_state, true); //in the open list

    //initialize the heap
    open_list_.push_back(start_state);
    make_heap(open_list_.begin(), open_list_.end());
}

vector<GraphState::Ptr> Planner::plan(){
    vector<GraphState::Ptr> path;
    
    typedef pair<SearchState::Ptr,bool> StatePair;

    while(!open_list_.empty()){
        //pop off open_list
        SearchState::Ptr current = open_list_.front();
        pop_heap(open_list_.begin(), open_list_.end());
        open_list_.pop_back();

        //check if goal
        if(graph_->isGoalState(current->getGraphState()) ){
            //unwind path
            //terminate search
            cout << "Done!" << endl;
        }
        else{
            //add current to closed
            StatePair state_pair = search_state_space_[current];
            state_pair.second = false;
            
            //generate succs
            vector<int> costs;
            vector<GraphState::Ptr> successors;
            graph_->getValidSuccessors( current->getGraphState(), successors, costs );
            
            //check succs in open and closed list
            for(size_t i=0; i<successors.size(); i++){
                SearchState::Ptr succ = boost::make_shared<SearchState>();
                succ->g = current->g + costs[i];
                succ->graph_state_ = successors[i];

                //check if on open or closed list
                HashTable::iterator state_pair_it = search_state_space_.find(succ);
                if(state_pair_it == search_state_space_.end()){
                    //not in open and closed
                    succ->h = graph_->getHeuristicCost( succ->getGraphState() );
                    succ->parent_ = current;
                    open_list_.push_back(succ);
                    push_heap(open_list_.begin(), open_list_.end());
                }
                else{
                    if(state_pair_it->second.second)//true = open list
                    {
                        if(succ->g < state_pair_it->second.first->g){
                            //update g value
                            state_pair_it->second.first->g = succ->g;
                            //update parent
                            state_pair_it->second.first->parent_ = current;
                        }
                        //resort heap
                        //decrease key operation
                        make_heap(open_list_.begin(), open_list_.end());
                    }
                    else//false = closed list
                    {
                        //do not add
                    }
                }
                
            }
        }
    }
     
    return path;    
}
