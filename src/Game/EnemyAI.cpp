#include "Utils.h"
#include "Component.h"
#include "Components/PhysBody.h"
#pragma once

namespace eng
{

class EnemyAI : public Component
{
    std::list<GameObject*> players;

    void FixedUpdate()
    {
        players = attached->GetScene()->FindGameObjects("Player");

        for(auto player : players)
        {
            if(Magnitude(player->transform.position - attached->transform.position) < 250)
            {
                PhysBody* body = attached->GetComponent<PhysBody>();
                    
                sf::Vector2f nPos = Normalize(player->transform.position - attached->transform.position) * 8.f * 0.1f;
                body->TransformPosition(nPos);
            }    
        }
    }

    void BeginContact(GameObject* hit)
    {
        if(hit->GetName() == "Player")
        {
            attached->GetScene()->Destroy(hit);
            GameObject* newEnemy = new GameObject(*attached);
            sf::Vector2f subPos = sf::Vector2f(newEnemy->transform.position.x + 20, newEnemy->transform.position.y); 
            newEnemy->transform.position = subPos;
            attached->GetScene()->AddGameObject(newEnemy);
        }
    }

};

} // namespace eng