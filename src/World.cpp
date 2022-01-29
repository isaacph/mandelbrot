#include "game.h"
using namespace std;

void World::worldUpdate(float deltaF, PlayerInstruction instruct){
    if(instruct.isJump){
        player.jump();
    }
    updatePhysics(deltaF);
}
void World::updatePhysics(float deltaF){
    gravity(player, deltaF);
    player.updateHitbox(deltaF);
}
void World::gravity(Player player, float deltaF){
    bool fall;
    //TODO check if object should fall

    if(fall){
        player.airtime+= deltaF;
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