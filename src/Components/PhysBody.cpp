#include "GameMaster.h"
#include "PhysBody.h"
#include "Scene.h"
#include <stdlib.h>

using namespace eng;
PhysBody::PhysBody(b2FixtureDef fixture, b2BodyType type) : fixture(fixture), type(type) { }
b2FixtureDef PhysBody::GetFixture() { return fixture; }

void PhysBody::OnInit() {

    b2BodyDef defBody;

    defBody.type = this->type;
    defBody.position.Set(attached->transform.position.x, attached->transform.position.y);
    

    this->body = GameMaster::Get().GetCurrentScene()->GetWorld()->CreateBody(&defBody);
    this->body->ApplyLinearImpulse(b2Vec2(0, std::rand() % 20 + 1), b2Vec2(attached->transform.position.x, attached->transform.position.y), true);
    this->body->CreateFixture(&fixture);
}


void PhysBody::FixedUpdate() {
    b2Vec2 position = body->GetPosition();

    attached->transform.position = sf::Vector2f(position.x, position.y);

    // std::cout << '\n' << attached->GetName() << '\n';
    // std::cout << "TRANSFORM: " << attached->transform.position.x << " || " << attached->transform.position.y << '\n';
    // std::cout << "BODY POS: " << body->GetPosition().x << " || " << body->GetPosition().y << '\n';
    // std::cout << "SUB RADIUS: " << fixture.shape->m_radius << '\n';
}

void PhysBody::BeginContact(GameObject* object) {
    std::cout << attached->GetName() << " with " << object->GetName() << '\n';
}