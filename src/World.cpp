#include "game.h"
using namespace std;

void World::worldUpdate(float deltaF, PlayerInstruction instruct){
    playerMovement(deltaF, instruct);
    updatePhysics(deltaF);
}
void World::playerMovement(float deltaF, PlayerInstruction instruct){
    if(instruct.isJump){
        player.jump();
    }
    if(!instruct.moveLeft && !instruct.moveRight){//slow down the charcter

        if(player.x_velocity <= 0.05f* player.maxSpeed)
            player.x_velocity=0.0f;
        else
            player.updateXVelocity(player.x_velocity*0.75f *-1.0f);
    }
    else{

        if(instruct.moveLeft){//decrease velocity of player
            player.playerMoveLeft(deltaF);
        }
        if(instruct.moveRight){//increase velocity of player
            
            player.playerMoveRight(deltaF);
        }
    }
    
}
void World::updatePhysics(float deltaF){
    //apply gravity to all
    gravity(player, deltaF);

    //move hitboxes
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