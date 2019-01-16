#include "Serializable.h"
#include "Game/PlayerController.h"
#include "Components/Mesh.h"
#include "Components/Camera.h"

using namespace eng;

shared_ptr<Component> Serializable::GetComponent(json component)
{
    string name = component["type"];
    shared_ptr<Component> returning;
    if (name == "PlayerController")
    {
        returning = make_shared<PlayerController>();
    }
    if (name == "Mesh")
    {
        returning = make_shared<Mesh>();
    }
    if (name == "Camera")
    {
        returning = make_shared<Camera>();
    }

  //  returning->Deserialize(component);
    return returning;
}