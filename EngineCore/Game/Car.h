#include "Utils.h"
#include "Components/Mesh.h"

#pragma once

class Car : public eng::Component {
    private:
    shared_ptr<eng::Component> Clone();
    void OnInit();
    void Update();
    json Serialize();
    void Deserialize(json j);
    void DrawEditor();
    weak_ptr<eng::Mesh> _mesh;
    float maxSpeed = 100;
    float deltaSpeed = 3;

    sf::Sound soundForce;
    sf::Sound soundIdle;
    sf::Sound soundStart;

    string skeyForce;
    string skeyIdle;
    string skeyStart;
};