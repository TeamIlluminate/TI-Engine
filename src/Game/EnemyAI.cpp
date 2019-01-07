#include "Components/PhysBody.h"
#include "Game/EnemyAI.h"

using namespace eng;

    void EnemyAI::FixedUpdate()
    {
        if(auto owner = _owner.lock())
        {
            players = owner->GetScene().lock()->FindGameObjects("Player");

            for(auto iterator : players)
            {
                if (auto player = iterator.lock()) {
                    if(Magnitude(player->transform.position - owner->transform.position) < 250)
                    {
                        auto body = owner->GetComponent<PhysBody>();
                        sf::Vector2f nPos = Normalize(player->transform.position - owner->transform.position) * 8.f * 0.1f;
                        body.lock()->TransformPosition(nPos);
                    }    
                }
             }
        }
    }


shared_ptr<Component> EnemyAI::Clone() {
    return make_shared<EnemyAI>(*this);
}

    void EnemyAI::BeginContact(weak_ptr<GameObject> hit)
    {
        if (auto foundedObject = hit.lock()) {

            if(foundedObject->GetName() == "Player")
            {
                _owner.lock()->GetScene().lock()->Destroy(hit);
            }
        }
    }
