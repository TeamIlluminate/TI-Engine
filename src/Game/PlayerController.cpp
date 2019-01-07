#include "Game/PlayerController.h"
#include "Components/ShapeMesh.h"
#include "Game/Bullet.cpp"

using namespace eng;

void PlayerController::OnInit()
{
    if (auto owner = _owner.lock())
    {
        _bodyComponent = owner->GetComponent<PhysBody>();
    }
}

shared_ptr<Component> PlayerController::Clone() {
    return make_shared<PlayerController>(*this);
}

void PlayerController::Update()
{
    if (auto bodyComponent = _bodyComponent.lock())
    {
        if (!bodyComponent->body->GetWorld()->IsLocked())
        {

            if (shoot < shootDeley)
                shoot += DeltaTime();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                this->MoveIn(sf::Vector2f(0, -5));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                this->MoveIn(sf::Vector2f(0, 5));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                this->MoveIn(sf::Vector2f(-5, 0));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                this->MoveIn(sf::Vector2f(5, 0));
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                if (!isFiring && shoot >= shootDeley)
                {
                    shoot = 0;
                    isFiring != isFiring;
                    sf::Vector2i currentPosition = sf::Mouse::getPosition();
                    this->ShootIn(sf::Vector2f(currentPosition.x, currentPosition.y));
                }
            }
        }
    }
}

void PlayerController::MoveIn(sf::Vector2f position)
{
    if (auto bodyComponent = _bodyComponent.lock())
    {
        bodyComponent->TransformPosition(position);
    }
}

void PlayerController::ShootIn(sf::Vector2f position)
{
    if (auto owner = this->_owner.lock())
    {
        sf::Vector2f direction = position - owner->transform.position;
        if (Magnitude(direction) > 15.f)
        {
            direction = Normalize(direction);

            shared_ptr<GameObject> bullet = std::make_shared<eng::GameObject>("BULLER");
            bullet->transform.position = owner->transform.position + direction * 15.f;

            auto shape = make_shared<sf::CircleShape>(2.f);
            shape->setOrigin(1.f, 1.f);
            shape->setFillColor(sf::Color::Yellow);
            bullet->AddComponent(make_shared<eng::ShapeMesh>(shape));

            b2CircleShape *b2circle = new b2CircleShape();
            b2circle->m_radius = 2.f;

            b2FixtureDef fixture;
            fixture.shape = b2circle;
            fixture.friction = 1.f;
            fixture.density = 2.f;

            bullet->AddComponent(make_shared<eng::PhysBody>(fixture, b2BodyType::b2_dynamicBody));
            bullet->AddComponent(make_shared<Bullet>(Normalize(direction), owner));
            owner->GetScene().lock()->AddGameObject(bullet);

            isFiring != isFiring;
        }
    }
}