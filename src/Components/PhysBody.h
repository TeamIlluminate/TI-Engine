#include "Utils.h"
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
    ~PhysBody();
    shared_ptr<Component> Clone();
    b2FixtureDef GetFixture();

    b2Body* body;

    void FixedUpdate();
    void OnInit();
    weak_ptr<GameObject> RayCast(sf::Vector2f to);
    void AddImpulse(sf::Vector2f vector);
    void TransformPosition(sf::Vector2f pos);

  private:
    b2FixtureDef fixture;
    b2BodyType type;
};

} // namespace eng