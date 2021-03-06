#include "CollisionEventManager.h"
#include "Scene.h"
#include "Components/Mesh.h"

using namespace eng;

CollisionEventManager::CollisionEventManager(Scene *scene) : scene(scene) {}

void CollisionEventManager::BeginContact(b2Contact *contact)
{

    b2Body *first = contact->GetFixtureA()->GetBody();
    b2Body *second = contact->GetFixtureB()->GetBody();

    weak_ptr<GameObject> firstObject;
    weak_ptr<GameObject> secondObject;

    auto gameObjects = scene->GetGameObjects();

    for (auto gameObject : gameObjects)
    {

        auto body = gameObject.lock()->GetComponent<Mesh>();

        if (auto bodyShare = body.lock())
        {
            if (bodyShare->GetBody() == first)
            {
                firstObject = gameObject;
            }
            if (bodyShare->GetBody() == second)
            {
                secondObject = gameObject;
            }
        }
    }

    auto firstShared = firstObject.lock();
    auto secondShared = secondObject.lock();

    if( firstShared && secondShared)
    {
        auto components = firstShared->GetComponents();
        for (auto component : components)
        {
            if (auto comp = component.lock()) {            
                comp->BeginContact(secondObject.lock());
            }
        }

        components = secondShared->GetComponents();
        for (auto component : components)
        {
            if (auto comp = component.lock()) {            
                comp->BeginContact(firstObject.lock());
            }
        }
    }
}

void CollisionEventManager::EndContact(b2Contact *contact)
{

    b2Body *first = contact->GetFixtureA()->GetBody();
    b2Body *second = contact->GetFixtureB()->GetBody();

    weak_ptr<GameObject> firstObject;
    weak_ptr<GameObject> secondObject;

    auto gameObjects = scene->GetGameObjects();

    for (auto gameObject : gameObjects)
    {

        auto body = gameObject.lock()->GetComponent<Mesh>();

        if (auto bodyShare = body.lock())
        {
            if (bodyShare->GetBody() == first)
            {
                firstObject = gameObject;
            }
            if (bodyShare->GetBody() == second)
            {
                secondObject = gameObject;
            }
        }
    }

    auto firstShared = firstObject.lock();
    auto secondShared = secondObject.lock();

    if( firstShared && secondShared)
    {
        auto components = firstShared->GetComponents();
        for (auto component : components)
        {
            if (auto comp = component.lock()) {            
                comp->EndContact(secondObject.lock());
            }
        }

        components = secondShared->GetComponents();
        for (auto component : components)
        {
            if (auto comp = component.lock()) {            
                comp->EndContact(firstObject.lock());
            }
        }
    }
}