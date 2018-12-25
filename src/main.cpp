#include "GameMaster.h"
#include "Render.h"
#include "Scene.h"
#include "Game/Objects.cpp"

int main() {

    eng::Render* render = new eng::Render(sf::VideoMode::getFullscreenModes()[0]);
    eng::GameMaster::Get().GameStarted(true);

    eng::GameMaster& gameMaster = eng::GameMaster::Get();

    eng::Scene* mainScene = new eng::Scene("MainScene");
    gameMaster.LoadScene(mainScene);

    std::srand(20);
    mainScene->AddGameObject(new JustCircle("Ebashik", 195, 100, 50, b2_dynamicBody));
    mainScene->AddGameObject(new JustCircle("BB",150, 180, 50, b2_staticBody));

    render->SetScene(mainScene);

    while(eng::GameMaster::Get().IsGameStarted()){}

    return 0;
}