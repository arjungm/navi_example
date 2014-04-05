#include <navi_example/Graph.h>

#include <math.h>

Graph::Graph(Environment::Ptr env) : env_(env) {

}

int Graph::getHeuristicCost( const GraphState& state ){
   int eu_dist = sqrt(pow(env_->getGoal()->x - state.coords.x,2) + pow(env_->getGoal()->y - state.coords.y,2));
   return eu_dist;
}
void Graph::getValidSuccessors( const GraphState& state, vector<GraphState>& successors, vector<int>& costs ){
    for(int dx=-1; dx<1; dx++){
        for(int dy=-1; dx<1; dx++){
            GraphState neighbor;
            neighbor.coords.x = state.coords.x + dx;
            neighbor.coords.y = state.coords.y + dy;
            if(env->isCollisionFree( neighbor.coords )){
                successors.push_back(neighbor);
                costs.push_back(1);
            }
        }
    }
}
bool Graph::isGoalState( const GraphState& state ){
    return false;
}
