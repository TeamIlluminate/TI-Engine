#include "Game/PlayerController.h"
#include "Components/ShapeMesh.h"
#include "Game/Bullet.cpp"

using namespace eng;

void PlayerController::OnInit()
{
    bodyComponent = this->attached->GetComponent<PhysBody>();
}
void PlayerController::Update()
{
    if(shoot < shootDeley)
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
        if(!isFiring && shoot >= shootDeley)
        {
            shoot = 0;
            isFiring != isFiring;
            sf::Vector2i currentPosition = sf::Mouse::getPosition();
            this->ShootIn(sf::Vector2f(currentPosition.x, currentPosition.y));
        }
    }
}

void PlayerController::MoveIn(sf::Vector2f position)
{
    bodyComponent->TransformPosition(position);
}

void PlayerController::ShootIn(sf::Vector2f position)
{
    sf::Vector2f direction = position - this->attached->transform.position;
    if (Magnitude(direction) > 15.f)
    {
        direction = Normalize(direction);
        GameObject *bullet = new GameObject();

        bullet->transform.position = this->attached->transform.position + direction * 15.f;

        sf::CircleShape *shape = new sf::CircleShape(2.f);
        shape->setOrigin(2.f, 2.f);
        shape->setFillColor(sf::Color::Yellow);

        bullet->AddComponent(new ShapeMesh(shape));

        b2CircleShape* b2circle = new b2CircleShape();
        b2circle->m_radius = 2.f;

        b2FixtureDef fixture; 
        fixture.shape = b2circle;
        fixture.friction = 1.f;
        fixture.density = 2.f;

        bullet->AddComponent(new eng::PhysBody(fixture, b2BodyType::b2_dynamicBody));
        bullet->AddComponent(new eng::Bullet(Normalize(direction), attached));
        this->attached->GetScene()->AddGameObject(bullet);

        isFiring != isFiring;
    }
}