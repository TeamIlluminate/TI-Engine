#include "Utils.h"
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

};
}