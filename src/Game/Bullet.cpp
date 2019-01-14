#include "Component.h"
#include "Scene.h"

#pragma once

namespace eng
{

class Bullet : public Component
{
  public:
    Bullet() {}

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

};
} // namespace eng