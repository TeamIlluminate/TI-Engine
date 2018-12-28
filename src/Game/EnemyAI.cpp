#include "Components/PhysBody.h"
#include "Game/EnemyAI.h"

using namespace eng;

    void EnemyAI::FixedUpdate()
    {
        if(auto parent = owner.lock())
        {
            players = parent->GetScene()->FindGameObjects("Player");

            for(auto iterator : players)
            {
                if (auto player = iterator.lock()) {
                    if(Magnitude(player->transform.position - parent->transform.position) < 250)
                    {
                        auto body = parent->GetComponent<PhysBody>();
                        sf::Vector2f nPos = Normalize(player->transform.position - parent->transform.position) * 8.f * 0.1f;
                        body.lock()->TransformPosition(nPos);
                    }    
                }
             }
        }
    }

    void EnemyAI::BeginContact(weak_ptr<GameObject> hit)
    {
        if (auto foundedObject = hit.lock()) {

            if(foundedObject->GetName() == "Player")
            {
                owner.lock()->GetScene()->Destroy(hit);
            }
        }
    }
