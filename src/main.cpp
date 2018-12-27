#include "GameMaster.h"
#include "Render.h"
#include "Scene.h"
#include "Game/Objects.cpp"
#include "Components/PhysBody.h"
#include "Component.h"

int main()
{

    eng::Render *render = new eng::Render(sf::VideoMode::getFullscreenModes()[0]);
    eng::GameMaster::Get().GameStarted(true);

    eng::GameMaster &gameMaster = eng::GameMaster::Get();

    eng::Scene *mainScene = new eng::Scene("MainScene");
    gameMaster.LoadScene(mainScene);

    std::srand(20);

    for (int i = 0; i < 3; i++)
        mainScene->AddGameObject(new JustCircle("Movable #" + std::to_string(i), 100 + 300 * i, 100, 20.f, b2_dynamicBody));

        JustCircle * circle =         new JustCircle("Movable 123", 500, 500, 30.f, b2_dynamicBody);
        
        mainScene->AddGameObject(circle);
        auto mesh = circle->GetComponent<eng::Mesh>();
        circle->RemoveComponent(mesh);
        //circle->~JustCircle();

    render->SetScene(mainScene);

    while (eng::GameMaster::Get().IsGameStarted())
    {
    } //Now imitate game\editor cycle

    return 0;
}