#include "Utils.h"
#include "Component.h"
#include "GameObject.h"
#pragma once

namespace eng{

//[Serializable]
class EnemyAI : public Component
{

public:

shared_ptr<Component> Clone();

void FixedUpdate();

void BeginContact(weak_ptr<GameObject> hit);

private:

void DrawEditor();

list<weak_ptr<GameObject> > players;
float speed = 5.f;
float range = 200.f;

json Serialize() 
{
    json jAI;
    jAI["type"] = "EAI";
    jAI["speed"] = speed;
    jAI["range"] = range;
    return jAI;
};

void Deserialize(json obj)
{
    speed = obj["speed"];
    range = obj["range"];
};

};

}