#include "Component.h"
#include "Components/PhysBody.h"
#include "Scene.h"

#pragma once

namespace eng
{

class Bullet : public Component
{
  public:
    Bullet() {}

    void OnInit()
    {
        if (shared_ptr<GameObject> owner = _owner.lock())
        {
            _bodyComponent = owner->GetComponent<PhysBody>();
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
};
} // namespace eng