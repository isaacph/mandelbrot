#include "game.h"
using namespace std;

void World::playerPhysics(){
    gravity(player);
}
void World::gravity(Player player){
    if( !=air){
        player.moveDown(GRAV_ACCEL*player.airtime);
    }
}