#include "Utils.h"
#pragma once

namespace eng
{
struct Transform
{
    Transform(sf::Vector2f position);
    Transform(sf::Vector2f position, float angle);

    sf::Vector2f position;
    //in radians
    float angle;
};
} // namespace eng