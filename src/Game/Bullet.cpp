#include "Component.h"
#include "Components/PhysBody.h"
#include "Scene.h"

#pragma once

namespace eng{

class Bullet : public Component
{
  public:
    Bullet(sf::Vector2f direction, GameObject *whoShoot) : direction(direction), whoShoot(whoShoot) {}

    void OnInit()
    {
        bodyComponent = attached->GetComponent<PhysBody>();
    }

    void FixedUpdate()
    {
        sf::Vector2f position = direction * 8.f;

        bodyComponent->TransformPosition(position);

        if (Magnitude(whoShoot->transform.position - attached->transform.position) > 3000)
        {
            attached->GetScene()->Destroy(attached);
        }
    }

    void BeginContact(GameObject *hit)
    {
        if (hit->GetName() == "ENEMY")
        {
            attached->GetScene()->Destroy(hit);
        }

        attached->GetScene()->Destroy(attached);
    }

  private:
    PhysBody *bodyComponent;
    GameObject *whoShoot;
    sf::Vector2f direction;
};
}