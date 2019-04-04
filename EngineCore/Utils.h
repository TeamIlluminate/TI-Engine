#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Libraries/ImGUI/imgui.h"
#include "Libraries/json.hpp"
#include "Libraries/ImGUI/imgui-SFML.h"
#include "Libraries/ImGUI/imgui_stdlib.h"
#include <math.h>
#include <iostream>
#include <string>
#include <memory>
#include <list>
#include <map>
#pragma once

using namespace std;
using namespace nlohmann;

typedef std::function<void()> voidDelegate;

// &|----> <-----|& ({*..*}) |-----> {8...8} <-----|
//    << * || * >>
//     << |||| >>
//    << (____) >>
//       <</\>>
//         \/
template <class T>
float Magnitude(const sf::Vector2<T> vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

template <class T>
sf::Vector2<T> Normalize(const sf::Vector2<T> vector)
{
    float magnitude = Magnitude(vector);
    return sf::Vector2<T>(vector.x / magnitude, vector.y / magnitude);
}

template <class T>
sf::Vector2<T> Rotate(const sf::Vector2<T> vector, const float angle)
{
    float x = vector.x * cos(-angle * M_PI / 180) - vector.y * sin(-angle * M_PI / 180);
    float y = vector.x * sin(-angle * M_PI / 180) + vector.y * cos(-angle * M_PI / 180);
    return sf::Vector2<T>(x, y);
}

//Return coordinates of mouse relative to current window's view
sf::Vector2f GetMouseCoordinates();
sf::Vector2f GetCenterScreenCoordinates();