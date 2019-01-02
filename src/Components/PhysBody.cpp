#include "GameMaster.h"
#include "PhysBody.h"
#include "Scene.h"
#include "RayCastHandler.h"
#include <stdlib.h>

using namespace eng;
PhysBody::PhysBody(b2FixtureDef fixture, b2BodyType type) : fixture(fixture), type(type) {}

PhysBody::~PhysBody() {
    if (auto scene = GameMaster::Get().GetCurrentScene().lock()) {
        scene->AddB2BodyToDelete(body);
    }
}

b2FixtureDef PhysBody::GetFixture() { return fixture; }

void PhysBody::OnInit()
{
    b2BodyDef defBody;

    defBody.type = this->type;
    defBody.position.Set(_owner.lock()->transform.position.x, _owner.lock()->transform.position.y);

    this->body = GameMaster::Get().GetCurrentScene().lock()->GetWorld().lock()->CreateBody(&defBody);

    this->body->CreateFixture(&fixture);
    this->body->SetGravityScale(0);
}

void PhysBody::FixedUpdate()
{
    b2Vec2 position = body->GetPosition();

    _owner.lock()->transform.position = sf::Vector2f(position.x, position.y);
}

weak_ptr<GameObject> PhysBody::RayCast(sf::Vector2f to)
{
    RayCastHandler *handler = new RayCastHandler();
    this->body->GetWorld()->RayCast(handler, this->body->GetPosition(), b2Vec2(to.x, to.y));
  
    return handler->foundedObject;
}

void PhysBody::TransformPosition(sf::Vector2f newPos)
{
    if (!this->body->GetWorld()->IsLocked()) {
        if (auto owner = _owner.lock()) {
            newPos = newPos + owner->transform.position;
            b2Vec2 toPos = b2Vec2(newPos.x, newPos.y);
            this->body->SetTransform(toPos, 0);
        }
    }
}

void PhysBody::AddImpulse(sf::Vector2f impulse)
{
    this->body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(_owner.lock()->transform.position.x, _owner.lock()->transform.position.y), true);
}