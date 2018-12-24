#include "Utils.h"
#pragma once

//Чувак, я подправил мейк, а так же инклюды подправь у себя(c_cpp_properties.json) и тут надо что-то придумать-> sf::Vector2<T> ибо. Мы можем юзать Vector2f, думаю норм будет.
// P.S. Я поставил тип sf::Vector2f сразу, короче. Билд полный, всё о-кей.
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