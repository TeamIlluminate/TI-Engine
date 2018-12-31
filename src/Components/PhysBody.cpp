#include "GameMaster.h"
#include "PhysBody.h"
#include "Scene.h"
#include "RayCastHandler.h"
#include <stdlib.h>

using namespace eng;
PhysBody::PhysBody(b2FixtureDef fixture, b2BodyType type) : fixture(fixture), type(type) {}

PhysBody::~PhysBody() {
    if (auto parent = owner.lock()) {
        std::cout << "я хочу убить себя" << '\n';
        parent->GetScene()->AddB2BodyToDelete(body);
    }
}

b2FixtureDef PhysBody::GetFixture() { return fixture; }

void PhysBody::OnInit()
{

    b2BodyDef defBody;

    defBody.type = this->type;
    defBody.position.Set(owner.lock()->transform.position.x, owner.lock()->transform.position.y);

    this->body = GameMaster::Get().GetCurrentScene()->GetWorld()->CreateBody(&defBody);

    this->body->CreateFixture(&fixture);
    this->body->SetGravityScale(0);
    
    AddImpulse(sf::Vector2f(10, 1));
}

void PhysBody::FixedUpdate()
{
    b2Vec2 position = body->GetPosition();

    owner.lock()->transform.position = sf::Vector2f(position.x, position.y);
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
        if (auto parent = owner.lock()) {
            newPos = newPos + parent->transform.position;
            b2Vec2 toPos = b2Vec2(newPos.x, newPos.y);
            this->body->SetTransform(toPos, 0);
        }
    }
}

void PhysBody::AddImpulse(sf::Vector2f impulse)
{
    this->body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(owner.lock()->transform.position.x, owner.lock()->transform.position.y), true);
}