#include "MoveByUserInput.h"
#include "GameObject.h"
#include "Player.h"
#include <iostream>

MoveByUserInput::MoveByUserInput(float move_increment) 
    : _move_increment(move_increment) {
    _increments_to_move_sideways = 0;
}

void MoveByUserInput::moveRight() {
    _increments_to_move_sideways++;
}

void MoveByUserInput::moveLeft() {
    _increments_to_move_sideways--;
}

void MoveByUserInput::update(GameObject* game_obj) {
    float move_x = _move_increment * _increments_to_move_sideways;
    _increments_to_move_sideways = 0;

    game_obj->addToX(move_x);
    if (game_obj->xPos() < 0)
        game_obj->setX(0);
    else if (game_obj->xPos() >= 770)
        game_obj->setX(770);
}
