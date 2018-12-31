#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <string>
#include <memory>
#include <list>
#include <functional>

#pragma once

using namespace std;

// &|----> <-----|& ({*..*}) |-----> {8...8} <-----|

template<class T>
    float Magnitude(const sf::Vector2<T> vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

template<class T>
sf::Vector2<T> Normalize(const sf::Vector2<T> vector) {
    float magnitude = Magnitude(vector);
    return sf::Vector2<T>(vector.x / magnitude, vector.y / magnitude);
}