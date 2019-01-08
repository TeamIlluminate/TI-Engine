#include "Utils.h"
#include <Box2D/Box2D.h>
#pragma once

namespace eng

{
class Editor {
    public:
    virtual void DrawEditor(){};
    
    template<typename V>
    void DrawVector2(sf::Vector2<V>& vector) {
        ImGui::InputFloat("X coord: ", &vector.x);
        ImGui::InputFloat("Y coord: ", &vector.y);
    };

    void DrawVector2(const b2Vec2 vector) {
        ImGui::LabelText("X coord: ", to_string(vector.x).c_str());
        ImGui::LabelText("Y coord: ", to_string(vector.y).c_str());
    };

};
}