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
    b2Fixture * GetFixture();

    b2Body* body = nullptr;

    void DrawEditor();
    weak_ptr<GameObject> RayCast(sf::Vector2f to);
    void AddImpulse(sf::Vector2f vector);
    void TransformPosition(sf::Vector2f pos);

  private:
    void FixedUpdate();
    void OnInit();
    b2FixtureDef fixtureDef;
    b2Fixture * fixture;
    b2BodyType type;
};

} // namespace eng