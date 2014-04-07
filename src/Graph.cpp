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
    if(parent){
        Direction dir = (state->coords) - (parent->coords);
        if(verbose)
            cout << "at " << state->coords << " dir =" << dir;
        getJumpPointSuccessorsHelper(state, dir, successors, costs);
    }
    else{
        int dx[] = {1,0,-1};
        int dy[] = {0,1,-1};

        for(size_t i=0; i<3; i++){
            for(size_t j=0; j<3; j++){
                
                if(!(j==0 && i==1)){
                    Direction dir(dx[i], dy[j]);
                    if(verbose)
                        cout << "at " << state->coords << " dir =" << dir;
                    //cin.get();
                    getJumpPointSuccessorsHelper(state, dir, successors, costs);
                }
            }
        }
    }
}

void Graph::getJumpPointSuccessorsHelper( const GraphState::Ptr& state, const Direction& dir, vector<GraphState::Ptr>& successors, vector<double>& costs ){
    if(hasForced(state, dir, successors, costs)){
        //add to successors and costs
    }
    GraphState::Ptr jump;
    double cost;
    if(dir.isDiagonal()){
        if(verbose)
            cout << endl << "\t";
        if(jumpHV(state, dir.dot(Direction(1,0)), jump, cost, true)){
            successors.push_back(jump);
            costs.push_back(cost);
        }        
        if(verbose)
            cout << "\t";
        if(jumpHV(state, dir.dot(Direction(0,1)), jump, cost, true)){
            successors.push_back(jump);
            costs.push_back(cost);
        }        
        if(jumpD(state, dir, jump, cost)){
            successors.push_back(jump);
            costs.push_back(cost);
        }
    }
    else{
        if(jumpHV(state, dir, jump, cost, true)){
            successors.push_back(jump);
            costs.push_back(cost);
        }
    }
}

bool Graph::jumpHV( const GraphState::Ptr& state, const Direction& dir, GraphState::Ptr& jump, double& cost, bool start_flag){
    GraphState::Ptr current = boost::make_shared<GraphState>( state->coords );
    //cout << "\tat " << current->coords <<  " going " << dir;
    if( env_->isCollisionFree( current->coords ) ){
        if( isGoalState( current ) ){
            if(verbose)
                cout << "is goal!" << endl;
            jump = current;
            return true;
        }
        else{
            vector<GraphState::Ptr> succs;
            vector<double> costs;
            if( hasForced( current, dir, succs, costs ) && !start_flag){
                if(verbose)
                    cout << "has forced at " << current->coords << endl;
                jump = current;
                return true;
            }
            else{
                cost += dir.norm();
                GraphState::Ptr next = boost::make_shared<GraphState>(current->coords + dir);
                return jumpHV( next, dir, jump, cost);
            }
        }
    }
    else{
        if(verbose)
            cout << "collided!" << endl;
        return false;//current is blocked
    }
}

bool Graph::jumpD( const GraphState::Ptr& state, const Direction& dir, GraphState::Ptr& jump, double& cost, bool start_flag){
    vector<GraphState::Ptr> succs;
    vector<double> costs;
    GraphState::Ptr current = boost::make_shared<GraphState>( state->coords+dir );
    if(verbose)
        cout << "\tat " << current->coords << " going " << dir << endl;
    if(env_->isCollisionFree( current->coords )){
        if(hasForced(state, dir, succs, costs) && !start_flag){
            if(verbose)
                cout << "\tforced" << endl;
            jump = state;
            return true;
        }
        double dummy_cost;
        if(verbose)
            cout << "\thoriz ";
        bool res_jump_h = jumpHV( current, dir.dot(Direction(1,0)), jump, dummy_cost, true);
        if(res_jump_h){
            jump = current;
            return true;
        }
        if(verbose)
            cout << "\tvert ";
        bool res_jump_v = jumpHV( current, dir.dot(Direction(0,1)), jump, dummy_cost, true);
        if(res_jump_v){
            jump = current;
            return true;
        }
        cost += dir.norm();
        return jumpD(current, dir, jump, cost);
    }
    else
        return false;
}


bool Graph::hasForced (const GraphState::Ptr& state, const Direction& dir, vector<GraphState::Ptr>& succs, vector<double>& costs){
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
