#include "GameMaster.h"
#include "Render.h"
#include "Scene.h"
#include "Component.h"
#include "GameObject.h"
#include <Box2D/Box2D.h>
#include "Components/Camera.h"
#include "Game/PlayerController.h"
#include "Game/EnemyAI.h"
#include "Game/FPSDraw.cpp"
#include "Game/ShaderSystem.h"
#include "Editor.hpp"
#include "Components/Mesh.h"

shared_ptr<eng::GameObject> createTest()
{
    auto test = make_shared<eng::GameObject>("TestObject");
    test->transform.position = sf::Vector2f(600, 400);
    test->AddComponent(make_shared<eng::Mesh>());
    return test;
}


shared_ptr<eng::GameObject> createPlayer(float x, float y) {
    shared_ptr<eng::GameObject> player = make_shared<eng::GameObject>("Player");
    player->transform.position = sf::Vector2f(x, y);

    auto mesh = make_shared<eng::Mesh>();
    auto circle = make_shared<sf::CircleShape>(5.f);
    circle->setFillColor(sf::Color::Blue);

    player->AddComponent(mesh);
    mesh->physEnable = true;

    mesh->Configure(circle);
    //я знаю в чем дело. надо вот так
//щас не было овнера. из за этого была ошибка.  
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
    auto mesh = make_shared<eng::Mesh>();

    enemy->AddComponent(mesh);

    mesh->physEnable = true;
    mesh->Configure(shape);
    enemy->AddComponent(make_shared<eng::EnemyAI>());

    
    return enemy;

}

shared_ptr<eng::GameObject> createShaderManager() {
    shared_ptr<eng::GameObject> shaderManager = make_shared<eng::GameObject>("shaderManager");
    //shaderManager->AddComponent(make_shared<gm::ShaderManager>());
    return shaderManager;
}


shared_ptr<eng::GameObject> createStats()
{
    shared_ptr<eng::GameObject> stats = make_shared<eng::GameObject>("stats");
    stats->AddComponent(make_shared<eng::FPSDraw>());
    return stats;
}


int main()
{
    eng::Render *render = new eng::Render();
    render->Init(sf::VideoMode::getFullscreenModes()[0]);

    eng::GameMaster::Get().GameStarted(true);

    eng::GameMaster &gameMaster = eng::GameMaster::Get();

    shared_ptr<eng::Scene> mainScene = make_shared<eng::Scene>("MainScene");
    gameMaster.LoadScene(mainScene);
    mainScene->CreateGameObject(createTest());
    mainScene->CreateGameObject(createPlayer(600, 500));

     for(int i = 0; i < 10; ++i)
     {
         mainScene->CreateGameObject(createEnemy(300 + i * 50, 100));
     }
 
    mainScene->CreateGameObject(createStats());
    

    while (eng::GameMaster::Get().IsGameStarted())
    {
    } //Now imitate game\editor cycle

    return 0;
}