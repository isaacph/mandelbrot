#include "game.h"
void Player::playerMoveLeft(float deltaF){
    updateYVelocity(moveAccel*deltaF*-1);
}

void Player::playerMoveRight(float deltaF){
    updateYVelocity(moveAccel*deltaF);
}