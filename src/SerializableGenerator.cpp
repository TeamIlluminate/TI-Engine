#include "Serializable.h" 
#include "Components/Camera.h"
#include "Components/Mesh.h"
#include "Game/PlayerController.h"
#include "Game/EnemyAI.h"
#pragma once
namespace eng
{
class SerializableGenerator
{
public:
SerializableGenerator()
{
Generate();
}
private:
void Generate(){
GameMaster::Get().RegisterComponentConstructor("Camera", []() -> shared_ptr<Component> { return make_shared<Camera>(); });
GameMaster::Get().RegisterComponentConstructor("Mesh", []() -> shared_ptr<Component> { return make_shared<Mesh>(); });
GameMaster::Get().RegisterComponentConstructor("PlayerController", []() -> shared_ptr<Component> { return make_shared<PlayerController>(); });
GameMaster::Get().RegisterComponentConstructor("EAI", []() -> shared_ptr<Component> { return make_shared<EnemyAI>(); });
}
};
}