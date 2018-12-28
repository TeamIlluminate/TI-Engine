#include "Component.h"
#include "Components/PhysBody.h"
#include "Scene.h"

#pragma once

namespace eng
{

class Bullet : public Component
{
  public:
    Bullet(sf::Vector2f direction, weak_ptr<GameObject> whoShoot) : direction(direction), whoShoot(whoShoot) {}

    void OnInit()
    {
        if (shared_ptr<GameObject> parent = owner.lock())
        {
            bodyComponent = parent->GetComponent<PhysBody>();
        }
    }

    void FixedUpdate()
    {
        auto physBody = bodyComponent.lock();
        auto shooter = whoShoot.lock();
        auto parent = owner.lock();

        if (physBody && shooter && parent)
        {
            sf::Vector2f position = direction * 8.f;
            physBody->TransformPosition(position);
            if (Magnitude(shooter->transform.position - parent->transform.position) > 3000)
            {
                parent->GetScene()->Destroy(owner);
            }
        }
    }

    void BeginContact(weak_ptr<GameObject> hit)
    {
        auto collision = hit.lock();
        auto parent = owner.lock();

        if (collision && parent)
        {
            if (collision->GetName() == "ENEMY")
            {
                parent->GetScene()->Destroy(hit);
            }
            parent->GetScene()->Destroy(owner);
        }
    }

  private:
    weak_ptr<PhysBody> bodyComponent;
    weak_ptr<GameObject> whoShoot;
    sf::Vector2f direction;
};
} // namespace eng