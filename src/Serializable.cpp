#include "Serializable.h"
#include "Component.h"
#include "Components/Mesh.h"

using namespace eng;

shared_ptr<Component> Serializable::GetComponent(json component)
{
    auto sss = constructorStorage;
    string name = component["type"];
    return constructorStorage[name]();
}

bool Serializable::VerifyRegister(string name)
{
    if (constructorStorage[name])
        return true;
    else
        return false;
}
void Serializable::RegisterComponentConstructor(string typeName, shared_ptr<Component> (*cc)())
{
    constructorStorage.insert(pair<string, shared_ptr<Component> (*)()>(typeName, *cc));
}
