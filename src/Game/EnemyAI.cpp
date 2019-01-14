#include "Components/Mesh.h"
#include "Game/EnemyAI.h"

using namespace eng;

void EnemyAI::FixedUpdate()
{
    if (auto owner = _owner.lock())
    {
        players = owner->GetScene().lock()->FindGameObjects("Player");

        for (auto iterator : players)
        {
            if (auto player = iterator.lock())
            {
                if (Magnitude(player->transform.position - owner->transform.position) < range)
                {
                    auto mesh = owner->GetComponent<Mesh>();
                    sf::Vector2f nPos = Normalize(player->transform.position - owner->transform.position) * speed * 0.1f;
                    mesh.lock()->TransformPosition(nPos);
                }
            }
        }
    }
}

shared_ptr<Component> EnemyAI::Clone()
{
    return make_shared<EnemyAI>(*this);
}

void EnemyAI::BeginContact(weak_ptr<GameObject> hit)
{
    if (auto foundedObject = hit.lock())
    {

        if (foundedObject->GetName() == "Player")
        {
            _owner.lock()->GetScene().lock()->Destroy(hit);
        }
    }
}

void EnemyAI::DrawEditor()
{
    if(ImGui::CollapsingHeader("EnemyAI"))
    {
        ImGui::InputFloat("Enemy speed", &speed, 1.f, 2.f, 1);
        ImGui::InputFloat("Enemy detect range", &range, 1.f, 2.f, 1);
    }
}
