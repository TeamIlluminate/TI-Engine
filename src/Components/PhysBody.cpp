#include "GameMaster.h"
#include "PhysBody.h"
#include "Scene.h"
#include "RayCastHandler.h"
#include <stdlib.h>

using namespace eng;
PhysBody::PhysBody(b2FixtureDef fixture, b2BodyType type) : fixture(fixture), type(type) {}
b2FixtureDef PhysBody::GetFixture() { return fixture; }

void PhysBody::OnInit()
{

    b2BodyDef defBody;

    defBody.type = this->type;
    defBody.position.Set(attached->transform.position.x, attached->transform.position.y);

    this->body = GameMaster::Get().GetCurrentScene()->GetWorld()->CreateBody(&defBody);

    this->body->CreateFixture(&fixture);

    AddImpulse(sf::Vector2f(10, 1));
}

void PhysBody::FixedUpdate()
{
    b2Vec2 position = body->GetPosition();

    attached->transform.position = sf::Vector2f(position.x, position.y);
}

void PhysBody::BeginContact(GameObject *object)
{
    std::cout << attached->GetName() << " with " << object->GetName() << '\n';
}

GameObject *PhysBody::RayCast(sf::Vector2f to)
{
    RayCastHandler *handler = new RayCastHandler();
    this->body->GetWorld()->RayCast(handler, this->body->GetPosition(), b2Vec2(to.x, to.y));
  //  if (handler->foundedObject) {std::cout << "something";}
    return handler->foundedObject;
}

void PhysBody::AddImpulse(sf::Vector2f impulse)
{
    this->body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(attached->transform.position.x, attached->transform.position.y), true);
}