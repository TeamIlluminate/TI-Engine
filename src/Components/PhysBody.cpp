#include "GameMaster.h"
#include "PhysBody.h"
#include "Scene.h"
#include "RayCastHandler.h"
#include <stdlib.h>

using namespace eng;
PhysBody::PhysBody(b2FixtureDef fixture, b2BodyType type) : fixtureDef(fixture), type(type) {}

PhysBody::~PhysBody()
{
    if (auto scene = GameMaster::Get().GetCurrentScene().lock())
    {
        scene->DeleteB2B(body);
    }
}

b2Fixture *PhysBody::GetFixture() { return fixture; }

shared_ptr<Component> PhysBody::Clone()
{
    return make_shared<PhysBody>(*this);
}

void PhysBody::OnInit()
{
    b2BodyDef defBody;
    defBody.type = this->type;
    defBody.position.Set(_owner.lock()->transform.position.x, _owner.lock()->transform.position.y);
    this->body = _owner.lock()->GetScene().lock()->GetWorld().lock()->CreateBody(&defBody);
    fixture = this->body->CreateFixture(&fixtureDef);
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
    if (ImGui::CollapsingHeader("PhysBody"))
    {
        ImGui::Text("Velocity");
        b2Vec2 velocity = this->body->GetLinearVelocity();

        if (ImGui::InputFloat("X ##velocity", &velocity.x) || (ImGui::InputFloat("Y ##velocity", &velocity.y)))
        {
            this->body->SetLinearVelocity(velocity);
        }

        float linearDamping = this->body->GetLinearDamping();
        if (ImGui::InputFloat("Linear Damping ", &linearDamping))
        {
            this->body->SetLinearDamping(linearDamping);
        }

        float mass = this->body->GetMass();
        if (ImGui::InputFloat("Mass ", &mass))
        {
        }

        ImGui::Text("Position");
        b2Vec2 position = this->body->GetPosition();
        if (ImGui::InputFloat("X ##position", &position.x) || (ImGui::InputFloat("Y ##position", &position.y)))
        {
            this->body->SetTransform(position, 0);
        }

        float gravity = this->body->GetGravityScale();
        if (ImGui::InputFloat("Gravity ", &gravity))
        {
            this->body->SetGravityScale(gravity);
        }

        ImGui::Text("Fixture");
        float density = this->fixture->GetDensity();
        if (ImGui::InputFloat("Density ", &density))
        {
            this->fixture->SetDensity(density);
        }

        float friction = this->fixture->GetFriction();
        if (ImGui::InputFloat("Friction ", &friction))
        {
            this->fixture->SetFriction(friction);
        }

        auto shape = this->fixture->GetShape();
        b2Shape::Type type = shape->GetType();
        switch (type)
        {
            case b2Shape::Type::e_circle:
            {
                auto circleShape = dynamic_cast<b2CircleShape *>(shape);
                if (auto window = GameMaster::Get().GetWindow().lock())
                {
                    ImGui::InputFloat("Collider Radius", &circleShape->m_radius);

                    sf::CircleShape collider(circleShape->m_radius);
                    collider.setOrigin(circleShape->m_radius, circleShape->m_radius);
                    collider.setFillColor(sf::Color(0.f, 0.f, 0.f, 0.f));
                    collider.setOutlineColor(sf::Color(0.f, 255.f, 0.f, 125.f));
                    collider.setOutlineThickness(1.f);
                    collider.setPosition(_owner.lock()->transform.position);
                    window->draw(collider);
                }
                break;
            }
            case b2Shape::Type::e_polygon:
            {
                auto polygonShape = dynamic_cast<b2PolygonShape *>(shape);
                if (auto window = GameMaster::Get().GetWindow().lock())
                {
                    int indexCount = polygonShape->GetVertexCount();

                    sf::ConvexShape collider(indexCount);
                    for (int i = 0; i < indexCount; ++i)
                    {
                        auto point = polygonShape->GetVertex(i);
                        auto name = ("Collider Point + [" + to_string(i) + "]");
                        if (ImGui::InputFloat((name + " X").c_str(), &point.x) || ImGui::InputFloat((name + " Y").c_str(), &point.y))
                        {
                        }
                        collider.setPoint(i, sf::Vector2f(point.x, point.y));
                    }
                    collider.setFillColor(sf::Color(0.f, 0.f, 0.f, 0.f));
                    collider.setOutlineColor(sf::Color(0.f, 255.f, 0.f, 125.f));
                    collider.setOutlineThickness(1.f);
                    collider.setPosition(_owner.lock()->transform.position);
                    window->draw(collider);
                }
                break;
            }
        }
    }
}
