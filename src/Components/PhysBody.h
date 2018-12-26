#include "Component.h"
#include <Box2D/Box2D.h>
#pragma once

namespace eng
{

class GameObject;

class PhysBody : public Component
{
  public:
    PhysBody(b2FixtureDef fixture, b2BodyType type);
    b2FixtureDef GetFixture();
    b2Body *body;

    void FixedUpdate();
    void OnInit();
    void BeginContact(GameObject *object);

    GameObject *RayCast(sf::Vector2f to);
    void AddImpulse(sf::Vector2f vector);

  private:
    b2FixtureDef fixture;
    b2BodyType type;
};

} // namespace eng