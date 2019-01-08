#include "GameMaster.h"
#include "PhysBody.h"
#include "Scene.h"
#include "RayCastHandler.h"
#include <stdlib.h>

using namespace eng;
PhysBody::PhysBody(b2FixtureDef fixture, b2BodyType type) : fixture(fixture), type(type) {}

PhysBody::~PhysBody()
{
    if (auto scene = GameMaster::Get().GetCurrentScene().lock())
    {
        scene->AddB2BodyToDelete(body);
    }
}

b2FixtureDef PhysBody::GetFixture() { return fixture; }

shared_ptr<Component> PhysBody::Clone()
{
    //  auto newPhysBody = make_shared<PhysBody>(*this);

    return make_shared<PhysBody>(*this);
}

void PhysBody::OnInit()
{
    b2BodyDef defBody;
    defBody.type = this->type;
    defBody.position.Set(_owner.lock()->transform.position.x, _owner.lock()->transform.position.y);
    this->body = _owner.lock()->GetScene().lock()->GetWorld().lock()->CreateBody(&defBody);
    this->body->CreateFixture(&fixture);
    this->body->SetGravityScale(1);
    this->body->SetLinearDamping(0.1f);
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
    if (!this->body->GetWorld()->IsLocked())
    {
        if (auto owner = _owner.lock())
        {
            newPos = newPos + owner->transform.position;
            b2Vec2 toPos = b2Vec2(newPos.x, newPos.y);
            this->body->SetTransform(toPos, 0);
        }
    }
}

void PhysBody::AddImpulse(sf::Vector2f impulse)
{
    this->body->SetLinearVelocity(b2Vec2(impulse.x, impulse.y));
}

void PhysBody::DrawEditor()
{
    auto gameObject = _owner.lock();
    if (gameObject)
    {
        int id = gameObject->id;
        if (ImGui::CollapsingHeader(("PhysBody##" + to_string(id)).c_str()))
        {
            ImGui::Text("Velocity");
            DrawVector2(this->body->GetLinearVelocity());
            ImGui::Text(("Linear Damping " + to_string(this->body->GetLinearDamping())).c_str());
            ImGui::Text(("Mass " + to_string(this->body->GetMass())).c_str());
        }
    }
}
