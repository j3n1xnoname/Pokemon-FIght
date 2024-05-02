#pragma once
#include <vector>

#include "Attack.hpp"
#include "LTexture.hpp"
#include "Pokemon.hpp"

class Player
{
public:
    Player();
    ~Player();
    Pokemon pokemon;
    bool isAlive() { return alive; }

private:
    bool alive;
};