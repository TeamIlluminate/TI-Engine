#include "CollisionEventManager.h"
#include "Scene.h"
#include "Components/PhysBody.h"

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

        auto body = gameObject.lock()->GetComponent<PhysBody>();

        if (auto bodyShare = body.lock())
        {
            if (bodyShare->body == first)
            {
                firstObject = gameObject;
            }
            if (bodyShare->body == second)
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
                comp->BeginContact(secondObject);
            }
        }

        components = secondShared->GetComponents();
        for (auto component : components)
        {
            if (auto comp = component.lock()) {            
                comp->BeginContact(firstObject);
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

        auto body = gameObject.lock()->GetComponent<PhysBody>();

        if (auto bodyShare = body.lock())
        {
            if (bodyShare->body == first)
            {
                firstObject = gameObject;
            }
            if (bodyShare->body == second)
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
                comp->EndContact(secondObject);
            }
        }

        components = secondShared->GetComponents();
        for (auto component : components)
        {
            if (auto comp = component.lock()) {            
                comp->EndContact(firstObject);
            }
        }
    }
}