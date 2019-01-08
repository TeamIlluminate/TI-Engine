#include "GameMaster.h"
#include "Render.h"
#include "Scene.h"
#include "Components/PhysBody.h"
#include "Components/ShapeMesh.h"
#include "Component.h"
#include "GameObject.h"
#include <Box2D/Box2D.h>
#include "Components/Camera.h"
#include "Game/PlayerController.h"
#include "Game/EnemyAI.h"
#include "Game/FPSDraw.cpp"
#include "Game/ShaderSystem.h"
#include "Editor.hpp"

shared_ptr<eng::GameObject> createPlayer(float x, float y) {
    shared_ptr<eng::GameObject> player = make_shared<eng::GameObject>("Player");
    player->transform.position = sf::Vector2f(x, y);

    shared_ptr<sf::CircleShape> shape = make_shared<sf::CircleShape>(5.f);
    shape->setOrigin(2.5f, 2.5f);
    shape->setFillColor(sf::Color::Blue);
    player->AddComponent(make_shared<eng::ShapeMesh>(shape));
    b2CircleShape * physShape = new b2CircleShape();
    physShape->m_radius = 5.f;
    b2FixtureDef fixture;
    fixture.shape = physShape;
    fixture.density = 1;    
    player->AddComponent(make_shared<eng::PhysBody>(fixture, b2_dynamicBody));
    player->AddComponent(make_shared<eng::PlayerController>()); 
    player->AddComponent(make_shared<eng::Camera>());
    player->GetComponent<eng::Camera>().lock()->isEnabled = true;
    return player;                                                             
}

shared_ptr<eng::GameObject> createEnemy(float x, float y)
{

    shared_ptr<eng::GameObject> enemy = make_shared<eng::GameObject>("Enemy");
    enemy->transform.position = sf::Vector2f(x, y);

    shared_ptr<sf::RectangleShape> shape = make_shared<sf::RectangleShape>(sf::Vector2f(20, 25));
    shape->setFillColor(sf::Color::Red);
    shape->setOrigin(10, 12.5);
    enemy->AddComponent(make_shared<eng::ShapeMesh>(shape));

    b2PolygonShape *rect = new b2PolygonShape();
    rect->SetAsBox(10.f, 12.5f); 

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

shared_ptr<eng::GameObject> createShaderManager() {
    shared_ptr<eng::GameObject> shaderManager = make_shared<eng::GameObject>("shaderManager");
    shaderManager->AddComponent(make_shared<gm::ShaderManager>());
    return shaderManager;
}

int main()
{
    eng::Render *render = new eng::Render(sf::VideoMode::getFullscreenModes()[0]);
    eng::GameMaster::Get().GameStarted(true);

    eng::GameMaster &gameMaster = eng::GameMaster::Get();

    shared_ptr<eng::Scene> mainScene = make_shared<eng::Scene>("MainScene");
    gameMaster.LoadScene(mainScene);

    mainScene->ForceGameObject(createPlayer(600, 500));

    for(int i = 0; i < 10; ++i)
    {
        mainScene->ForceGameObject(createEnemy(300 + i * 50, 100));
    }

    mainScene->ForceGameObject(createStats());


    while (eng::GameMaster::Get().IsGameStarted())
    {
    } //Now imitate game\editor cycle

    return 0;
}