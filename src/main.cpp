#include "GameMaster.h"
#include "Render.h"
#include "Scene.h"
#include <Box2D/Box2D.h>
#include "SerializableGenerator.cpp"

int main()
{
    eng::Render *render = new eng::Render();
    render->Init(sf::VideoMode::getFullscreenModes()[0]);

    eng::GameMaster::Get().GameStarted(true);

    eng::GameMaster &gameMaster = eng::GameMaster::Get();

    shared_ptr<eng::Scene> loadScene = make_shared<eng::Scene>("scene4rLoad");
    gameMaster.LoadScene(loadScene);

    delete new eng::SerializableGenerator();
     
    while (eng::GameMaster::Get().IsGameStarted())
    {
    } //Now imitate game\editor cycle

    return 0;
}