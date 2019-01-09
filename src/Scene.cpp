#include "Scene.h"
#include "GameObject.h"
#include "GameMaster.h"
#include "Components/Camera.h"
#include <Components/Mesh.h>

using namespace eng;

Scene::Scene(std::string name)
{
    this->name = name;

    world = make_shared<b2World>(b2Vec2(0, 0));
    collisionEventManager = new CollisionEventManager(this);
    world->SetContactListener(collisionEventManager);
}

Scene::Scene(const Scene &scene)
{
    this->name = scene.name + "_c";
    this->idCounter = scene.idCounter;
    this->world = make_shared<b2World>(b2Vec2(0, 0));
    this->collisionEventManager = new CollisionEventManager(this);
    this->world->SetContactListener(this->collisionEventManager);
    auto _sceneObjects = scene.sceneObjects;
    for (auto sceneObject : _sceneObjects)
    {
        this->sceneObjects.push_back(make_shared<GameObject>(*sceneObject.get()));
    }
    auto cameras = scene.cameras;
    this->cameras = cameras;
}

void Scene::Rebind()
{
    auto gameObjects = sceneObjects;
    for (auto gameObject : gameObjects)
    {
        gameObject->SetScene(shared_from_this());

        auto _components = gameObject->GetComponents();
        for (auto _component : _components)
        {
            if (auto component = _component.lock())
            {
                component->SetOwner(gameObject);
                component->OnInit();
            }
        }
    }
}

void Scene::PhysicsLoop()
{
    float32 timeStep = 0.02f;
    for (auto bodyToDelete : toDelete)
    {
        world->DestroyBody(bodyToDelete);
    }
    toDelete.clear();

    world->Step(timeStep, velocityIterations, positionIterations);

    auto gameObjects = sceneObjects;
    for (auto gameObject : gameObjects)
    {
        auto components = gameObject->GetComponents();
        for (auto component : components)
        {
            if (auto cmp = component.lock())
            {
                cmp->FixedUpdate();
            }
        }
    }
}

list<weak_ptr<Mesh>> Scene::Update()
{
    list<weak_ptr<Mesh>> meshs;
    auto gameObjects = sceneObjects;

    for (auto gameObject : gameObjects)
    {

        auto list = Update(gameObject);
        meshs.insert(meshs.end(), list.begin(), list.end());
    }

    return meshs;
}

list<weak_ptr<Mesh>> Scene::Update(shared_ptr<GameObject> gameObject)
{
    if (GameMaster::Get().state & GameMaster::_GAME)
    {
        auto components = gameObject->GetComponents();
        for (auto _component : components)
        {
            if (auto component = _component.lock())
            {
                component->Update();
                component->GUI();
            }
        }
    }

    auto childs = gameObject->GetChilds();
    list<weak_ptr<Mesh>> childMeshs;
    for (auto _child : childs)
    {
        auto list = Update(_child.lock());
        childMeshs.insert(childMeshs.end(), list.begin(), list.end());
    }

    auto _mesh = gameObject->GetComponent<Mesh>();
    childMeshs.push_front(_mesh);
    return childMeshs;
}

////////////////////////////////////////////////////////////////////////////////

shared_ptr<GameObject> Scene::CreateGameObject(string name = "empty")
{
    auto gameObject = make_shared<GameObject>(weak_from_this(), name);
    gameObject->id = idCounter;
    idCounter++;
    this->sceneObjects.push_back(gameObject);
    return gameObject;
}

shared_ptr<GameObject> Scene::CreateGameObject(shared_ptr<GameObject> gameObject)
{
    gameObject->SetScene(shared_from_this());
    gameObject->id = idCounter;
    idCounter++;
    this->sceneObjects.push_back(gameObject);
    return gameObject;
}

void Scene::Destroy(weak_ptr<GameObject> _gameObject)
{
    auto gameObject = _gameObject.lock();
    sceneObjects.remove(gameObject);
}

weak_ptr<GameObject> Scene::FindGameObject(std::string objectName) const
{
    for (auto gameObject : sceneObjects)
    {
        if (gameObject->GetName() == objectName)
            return gameObject;
    }
}

std::list<weak_ptr<GameObject>> Scene::FindGameObjects(std::string objectName) const
{
    std::list<weak_ptr<GameObject>> _foundedObjects;

    for (auto gameObject : sceneObjects)
    {
        if (gameObject->GetName() == objectName)
            _foundedObjects.push_back(gameObject);
    }

    return _foundedObjects;
}

//////////////////////////////////////////////////////////////////////////////////////

weak_ptr<b2World> Scene::GetWorld() const
{
    return this->world;
}

void Scene::DeleteB2B(b2Body *body)
{
    toDelete.push_back(body);
}

std::list<weak_ptr<GameObject>> Scene::GetGameObjects() const
{
    list<weak_ptr<GameObject>> _gameObjects;
    for (auto gameObject : this->sceneObjects)
    {
        _gameObjects.push_back(gameObject);
    }
    return _gameObjects;
}
