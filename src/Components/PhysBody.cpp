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

void PhysBody::RayCast()
{
    //Здесь сначала нужно будет проинитить лучи и скопировать трансформ, затем прорейкастить и описать логику.
    //Можешь сам заняться, либо можешь заняться спрайтом и обеспечением шейдеров в системе \ etc, а я займусь этим, когда проснусь. (20.45 -> [04\05].00)
    this->fixture.shape->RayCast(nullptr, b2RayCastInput(), b2Transform(), 0);
}