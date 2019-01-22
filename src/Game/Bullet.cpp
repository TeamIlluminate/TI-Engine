#include "Component.h"
#include "Scene.h"
#include "Components/Mesh.h"

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
            if(collision->GetName() != "Player") owner->GetScene().lock()->Destroy(owner);
        }
    }

  private:
    void OnInit()
    {
        _mesh = _owner.lock()->GetComponent<Mesh>();
    }

    void FixedUpdate()
    {
        if (auto owner = _owner.lock()) {
        if (auto mesh = _mesh.lock())
        {
            b2Body *body = mesh->GetBody();
            b2Vec2 velocity = body->GetLinearVelocity();
            if (sqrt(velocity.x * velocity.x + velocity.y * velocity.y) < 5)
            {
                owner->GetScene().lock()->Destroy(owner);
            }
        }
        }   
    }
    weak_ptr<Mesh> _mesh;
};
} // namespace eng