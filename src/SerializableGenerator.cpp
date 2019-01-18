#include "Serializable.h" 
#include "Components/Camera.h"
#include "Components/Mesh.h"
#include "Game/PlayerController.h"
#include "Game/FPSDraw.cpp"
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
}
};
}