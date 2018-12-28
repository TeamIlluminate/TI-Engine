#include "Utils.h"
#include "Component.h"
#include "GameObject.h"
#pragma once

namespace eng{

class EnemyAI : public Component
{

public:

void FixedUpdate();

void BeginContact(weak_ptr<GameObject> hit);

private:

list<weak_ptr<GameObject> > players;

};

}