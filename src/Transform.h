#include "Utils.h"
#pragma once

namespace eng
{
struct Transform
{
    Transform(const sf::Vector2f position = sf::Vector2f(0,0), const float angle = 0) {}

    sf::Vector2f position;
    //in radians
    float angle;
};
} // namespace eng