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

Graph::Graph(Environment::Ptr env) : env_(env), verbose(false) {

}

double Graph::getHeuristicCost( const GraphState::Ptr& state ){
   double eu_dist = pow(env_->getGoal()->x - state->coords.x,2) + pow(env_->getGoal()->y - state->coords.y,2);
   return sqrt(eu_dist);
}

void Graph::getValidSuccessors( const GraphState::Ptr& state, vector<GraphState::Ptr>& successors, vector<double>& costs ){
    for(int dx=-1; dx<2; dx++){
        for(int dy=-1; dy<2; dy++){
            if(dx==0 && dx==dy){
                continue;
            }
            //8 connected grid
            GraphState::Ptr neighbor = boost::make_shared<GraphState>( Cell(state->coords.x+dx, state->coords.y+dy) );
            if(env_->isCollisionFree( neighbor->coords )){
                successors.push_back(neighbor);
                costs.push_back( sqrt(pow(dx,2)+pow(dy,2)) );
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


void Graph::getJumpPointSuccessors(const GraphState::Ptr& state, const GraphState::Ptr& parent, vector<GraphState::Ptr>& successors, vector<double>& costs ){
    //if the parent exists then this is not the first state in the search
    if(parent){
        Direction dir = (state->coords) - (parent->coords);
        getJumpPointSuccessorsHelper(state, dir, successors, costs);
    }
    //this is the first state in the search
    else{
        int dx[] = {1,0,-1};
        int dy[] = {0,1,-1};
        
        //iterate over all possible directions from start to begin the search
        for(size_t i=0; i<3; i++){
            for(size_t j=0; j<3; j++){
                
                if(!(j==0 && i==1)){
                    Direction dir(dx[i], dy[j]);
                    getJumpPointSuccessorsHelper(state, dir, successors, costs);
                }
            }
        }
    }
}

void Graph::getJumpPointSuccessorsHelper( const GraphState::Ptr& state, const Direction& dir, vector<GraphState::Ptr>& successors, vector<double>& costs ){
    //add forced neighbors is available for current state
    if(getForced(state, dir, successors, costs)){
        //this function adds forced neighbors if available
    }
    GraphState::Ptr jump;
    double cost;
    //get the next jump point depending on the current states direction
    if(dir.isDiagonal()){
        //if we are jumping diagonally, start off with a horizontal and vertical
        //pass in the true flag because it is the first jumping test
        if(jumpHorizontallyVertically(state, dir.dot(Direction(1,0)), jump, cost, true)){
            successors.push_back(jump);
            costs.push_back(cost);
        }        
        if(jumpHorizontallyVertically(state, dir.dot(Direction(0,1)), jump, cost, true)){
            successors.push_back(jump);
            costs.push_back(cost);
        }        
        //then jump diagonally from the starting state
        if(jumpDiagonally(state, dir, jump, cost, true)){
            successors.push_back(jump);
            costs.push_back(cost);
        }
    }
    else{
        //if we are jumping horizontally / vertically proceed as normal
        if(jumpHorizontallyVertically(state, dir, jump, cost, true)){
            successors.push_back(jump);
            costs.push_back(cost);
        }
    }
}

bool Graph::jumpHorizontallyVertically( const GraphState::Ptr& state, const Direction& dir, GraphState::Ptr& jump, double& cost, bool start_flag){
    GraphState::Ptr current = boost::make_shared<GraphState>( state->coords );
    //if the state we are at is collision free
    if( env_->isCollisionFree( current->coords ) ){
        if( isGoalState( current ) ){
            jump = current;
            return true;
        }
        else{
            //if the state has a forced neighbor, stop jumping and add state as jump point
            if( hasForced( current, dir) && !start_flag){
                jump = current;
                return true;
            }
            else{
                cost += dir.norm();
                //generate next step
                GraphState::Ptr next = boost::make_shared<GraphState>(current->coords + dir);
                //continue jumping
                return jumpHorizontallyVertically( next, dir, jump, cost);
            }
        }
    }
    else{
        return false;//current is blocked
    }
}

bool Graph::jumpDiagonally( const GraphState::Ptr& state, const Direction& dir, GraphState::Ptr& jump, double& cost, bool start_flag){
    //get diagonal step
    GraphState::Ptr current = boost::make_shared<GraphState>( state->coords+dir );
    //if the digaonal step is collision free
    if(env_->isCollisionFree( current->coords )){
        //if we jumped to the goal, then we can stop
        //and add the goal as a jump point
        if( isGoalState( current ) ){
            jump = current;
            return true;
        }
        //if the place we came from has forced neighbor
        //and if it is not the first diagonal step taken
        if(hasForced(state, dir) && !start_flag){
            jump = state;
            return true;
        }
        //test if you can jump horizontally and vertically after the diagonal step
        //if you can then add the current diagonal step as a jump point
        double dummy_cost;
        bool res_jump_h = jumpHorizontallyVertically( current, dir.dot(Direction(1,0)), jump, dummy_cost, true);
        if(res_jump_h){
            jump = current;
            return true;
        }
        bool res_jump_v = jumpHorizontallyVertically( current, dir.dot(Direction(0,1)), jump, dummy_cost, true);
        if(res_jump_v){
            jump = current;
            return true;
        }
        //otherwise continue jumping digaonally
        cost += dir.norm();
        return jumpDiagonally(current, dir, jump, cost);
    }
    //unable to continue jumping diagonally this way
    else
        return false;
}


bool Graph::hasForced (const GraphState::Ptr& state, const Direction& dir){
    //get the positions to check
    Direction dir_free1 = dir;
    Direction dir_free2 = dir;
    Direction dir_block1 = dir;
    Direction dir_block2 = dir;
    
    bool res1, res2; 
    if( dir.isDiagonal() ){
        dir_free1.rotate(M_PI/2);
        dir_free2.rotate(-M_PI/2);
        dir_block1.rotate(3*M_PI/4);
        dir_block2.rotate(-3*M_PI/4);
    }
    else{
        dir_free1.rotate(M_PI/4);
        dir_free2.rotate(-M_PI/4);
        dir_block1.rotate(M_PI/2);
        dir_block2.rotate(-M_PI/2);
    }
    res1 = env_->isCollisionFree( state->coords + dir_free1 ) && !env_->isCollisionFree( state->coords + dir_block1 );
    res2 = env_->isCollisionFree( state->coords + dir_free2 ) && !env_->isCollisionFree( state->coords + dir_block2 );

    return res1||res2;
}

bool Graph::getForced (const GraphState::Ptr& state, const Direction& dir, vector<GraphState::Ptr>& succs, vector<double>& costs){
    Direction dir_free1 = dir;
    Direction dir_free2 = dir;
    Direction dir_block1 = dir;
    Direction dir_block2 = dir;
    
    bool res1, res2; 
    // get the positiosn to check for being blocked or free based on the current direction
    if( dir.isDiagonal() ){
        dir_free1.rotate(M_PI/2);
        dir_free2.rotate(-M_PI/2);
        dir_block1.rotate(3*M_PI/4);
        dir_block2.rotate(-3*M_PI/4);
    }
    else{
        dir_free1.rotate(M_PI/4);
        dir_free2.rotate(-M_PI/4);
        dir_block1.rotate(M_PI/2);
        dir_block2.rotate(-M_PI/2);
    }
    res1 = env_->isCollisionFree( state->coords + dir_free1 ) && !env_->isCollisionFree( state->coords + dir_block1 );
    res2 = env_->isCollisionFree( state->coords + dir_free2 ) && !env_->isCollisionFree( state->coords + dir_block2 );

    //add the free state if there is a forced neighbor
    GraphState::Ptr succ;
    if(res1){
        succ = boost::make_shared<GraphState>( state->coords + dir_free1 );
        succs.push_back(succ);
        costs.push_back(dir_free1.norm());
    }

    if(res2){
        succ = boost::make_shared<GraphState>( state->coords + dir_free2 );
        succs.push_back(succ);
        costs.push_back(dir_free2.norm());
    }

    return res1||res2;
}
