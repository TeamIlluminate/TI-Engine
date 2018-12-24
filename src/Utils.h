#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <list>
#include <string>

#pragma once
// &|----> <-----|& ({*..*}) |-----> {8...8} <-----|
template<class T>
sf::Vector2<T> Normalize(const sf::Vector2<T> vector) {
    float magnitude = magnitude(vector);
    return sf::Vector2<T>(vector.x / magnitude, vector.y / magnitude);
}

template<class T>
    float Magnitude(const sf::Vector2<T> vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}