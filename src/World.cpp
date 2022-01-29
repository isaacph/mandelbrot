#include "game.h"
using namespace std;

void World::worldUpdate(const float deltaf){
    updatePhysics();
}
void World::updatePhysics(){
    gravity(player);
}
void World::gravity(Player player, float deltaf){
    bool fall;
    //TODO check if object should fall

    if(fall){
        player.airtime+= deltaf;
        float falling_v=  GRAV_ACCEL* player.airtime;
        if(falling_v > MAX_FALL){
            falling_v = MAX_FALL;
        }
        player.updateYVelocity(falling_v);
    }
    else{
        player.airtime=0.0f;
        player.numJumps=2;
    }
}
