#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "json.hpp"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"
#include <math.h>
#include <iostream>
#include <string>
#include <memory>
#include <list>
#pragma once

using namespace std;
using namespace nlohmann;

// &|----> <-----|& ({*..*}) |-----> {8...8} <-----|
            //    << * || * >>
            //     << |||| >>
            //    << (____) >>
            //       <</\>> 
            //         \/
template<class T>
    float Magnitude(const sf::Vector2<T> vector) {
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

template<class T>
sf::Vector2<T> Normalize(const sf::Vector2<T> vector) {
    float magnitude = Magnitude(vector);
    return sf::Vector2<T>(vector.x / magnitude, vector.y / magnitude);
}

//Return coordinates of mouse relative to current window's view
sf::Vector2f GetMouseCoordinates();
sf::Vector2f GetCenterScreenCoordinates();