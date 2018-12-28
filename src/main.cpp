#include "GameMaster.h"
#include "Render.h"
#include "Scene.h"
#include "Components/PhysBody.h"
#include "Components/ShapeMesh.h"
#include "Component.h"
#include "GameObject.h"
#include <Box2D/Box2D.h>
#include "Game/PlayerController.h"
#include "Game/EnemyAI.h"
#include "Game/FPSDraw.cpp"

shared_ptr<eng::GameObject> createPlayer(float x, float y) {
    shared_ptr<eng::GameObject> player = make_shared<eng::GameObject>("Player");
    player->transform.position = sf::Vector2f(x, y);

    shared_ptr<sf::CircleShape> shape = make_shared<sf::CircleShape>(5.f);
    shape->setOrigin(5.f, 5.f);
    shape->setFillColor(sf::Color::Blue);
    player->AddComponent(make_shared<eng::ShapeMesh>(shape));
    b2CircleShape * physShape = new b2CircleShape();
    physShape->m_radius = 5.f;
    b2FixtureDef fixture;
    fixture.shape = physShape;
    fixture.density = 1;    
    player->AddComponent(make_shared<eng::PhysBody>(fixture, b2_dynamicBody));
    player->AddComponent(make_shared<eng::PlayerController>()); 

    return player; 
                                                             
}

shared_ptr<eng::GameObject> createEnemy(float x, float y)
{

    shared_ptr<eng::GameObject> enemy = make_shared<eng::GameObject>("Enemy");
    enemy->transform.position = sf::Vector2f(x, y);

    shared_ptr<sf::RectangleShape> shape = make_shared<sf::RectangleShape>(sf::Vector2f(20, 25));
    shape->setFillColor(sf::Color::Red);

    enemy->AddComponent(make_shared<eng::ShapeMesh>(shape));

    b2PolygonShape *rect = new b2PolygonShape();
    rect->SetAsBox(20, 25); 

    b2FixtureDef fixture;
    fixture.density = 5;
    fixture.friction = 1;
    fixture.shape = rect;

    enemy->AddComponent(make_shared<eng::PhysBody>(fixture, b2_dynamicBody));
    enemy->AddComponent(make_shared<eng::EnemyAI>());

    return enemy;

}

shared_ptr<eng::GameObject> createStats()
{
    shared_ptr<eng::GameObject> stats = make_shared<eng::GameObject>("stats");

    shared_ptr<sf::CircleShape> shape = make_shared<sf::CircleShape>(0.f);
    shape->setFillColor(sf::Color::Black);
    stats->AddComponent(make_shared<eng::ShapeMesh>(shape));   

    stats->AddComponent(make_shared<eng::FPSDraw>());
    return stats;
}

int main()
{
    eng::Render *render = new eng::Render(sf::VideoMode::getFullscreenModes()[0]);
    eng::GameMaster::Get().GameStarted(true);

    eng::GameMaster &gameMaster = eng::GameMaster::Get();

    eng::Scene *mainScene = new eng::Scene("MainScene");
    gameMaster.LoadScene(mainScene);

    render->SetScene(mainScene);

    mainScene->AddGameObject(createPlayer(600, 500));

    for(int i = 0; i < 15; ++i)
    {
        mainScene->AddGameObject(createEnemy(1 + i * 100, 100 + i * 10));
    }

    mainScene->AddGameObject(createStats());


    while (eng::GameMaster::Get().IsGameStarted())
    {
    } //Now imitate game\editor cycle

    return 0;
}