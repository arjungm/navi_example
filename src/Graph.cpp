#include <navi_example/Graph.h>

#include <math.h>
#include <boost/make_shared.hpp>

GraphState::GraphState( Cell coordinates ) : coords(coordinates)
{
}

GraphState& GraphState::operator=( const GraphState& other ){
    coords = other.coords;
    return *this;
}

ostream& operator<<(ostream& os, const GraphState& s){
  os << s.coords;
  return os;
}

Graph::Graph(Environment::Ptr env) : env_(env) {

}

int Graph::getHeuristicCost( const GraphState::Ptr& state ){
   int eu_dist = pow(env_->getGoal()->x - state->coords.x,2) + pow(env_->getGoal()->y - state->coords.y,2);
   return sqrt(double(eu_dist))*100;
}

void Graph::getValidSuccessors( const GraphState::Ptr& state, vector<GraphState::Ptr>& successors, vector<int>& costs ){
    for(int dx=-1; dx<2; dx++){
        for(int dy=-1; dy<2; dy++){
            GraphState::Ptr neighbor = boost::make_shared<GraphState>( Cell(state->coords.x+dx, state->coords.y+dy) );
            if(env_->isCollisionFree( neighbor->coords )){
                successors.push_back(neighbor);
                costs.push_back(100);
            }
        }
    }
}
bool Graph::isGoalState( const GraphState::Ptr& state ){
    return (state->coords)==*(env_->getGoal());
}

GraphState::Ptr Graph::getStart(){
    return boost::make_shared<GraphState>( *(env_->getStart()) );
}
