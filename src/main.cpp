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


int main()
{
    eng::Render *render = new eng::Render();
    render->Init(sf::VideoMode::getFullscreenModes()[0]);

    eng::GameMaster::Get().GameStarted(true);

    eng::GameMaster &gameMaster = eng::GameMaster::Get();

    shared_ptr<eng::Scene> loadScene = make_shared<eng::Scene>();
    gameMaster.LoadScene(loadScene);

    while (eng::GameMaster::Get().IsGameStarted())
    {
    } //Now imitate game\editor cycle

    return 0;
}