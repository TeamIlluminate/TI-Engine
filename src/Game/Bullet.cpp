#include "Component.h"
#include "Components/PhysBody.h"
#include "Scene.h"

#pragma once

namespace eng
{

class Bullet : public Component
{
  public:
    Bullet(sf::Vector2f direction, shared_ptr<GameObject> whoShoot) : direction(direction), _whoShoot(whoShoot) {}

    void OnInit()
    {
        if (shared_ptr<GameObject> owner = _owner.lock())
        {
            _bodyComponent = owner->GetComponent<PhysBody>();
            _bodyComponent.lock()->AddImpulse(direction * 100.f);
        }
    }

    shared_ptr<Component> Clone()
    {
        return make_shared<Bullet>(*this);
    }

    void BeginContact(weak_ptr<GameObject> hit)
    {
        auto collision = hit.lock();
        auto owner = _owner.lock();

        string s = collision->GetName();

        if (collision && owner)
        {
            if (collision->GetName() == "Enemy")
            {
                owner->GetScene().lock()->Destroy(hit);
            }
            owner->GetScene().lock()->Destroy(owner);
        }
    }

  private:
    weak_ptr<PhysBody> _bodyComponent;
    weak_ptr<GameObject> _whoShoot;
    sf::Vector2f direction;
};
} // namespace eng