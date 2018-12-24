#include "Scene.h"
#include "GameObject.h"
#include "Components/Camera.h"

using namespace eng;

Scene::Scene(std::string name)
{
    this->name = name;
}

std::list<GameObject*> Scene::GetGameObjects() const
{
    return this->sceneObjects;
}

GameObject* Scene::FindGameObject(std::string objectName) const
{
    for(GameObject* gameObject : sceneObjects)
    {
        if(gameObject->GetName() == objectName) return gameObject;
    }

    return nullptr;
}

std::list<GameObject*> Scene::FindGameObjects(std::string objectName) const
{
    std::list<GameObject*> findedObjects;

    for(GameObject* gameObject : sceneObjects)
    {
        if(gameObject->GetName() == objectName) findedObjects.push_back(gameObject);
    }

    return findedObjects;
}

void Scene::RemoveFromScene(GameObject* gameObject)
{
    sceneObjects.remove(gameObject);
}

Camera* Scene::GetActiveCamera() const
{
    
}