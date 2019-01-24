#include "Game/PlayerController.h"
#include "Components/Mesh.h"
#include "Game/Bullet.cpp"
#include "ResourceManager.h"

using namespace eng;

void PlayerController::OnInit()
{
    if (auto owner = _owner.lock())
    {
        _mesh = owner->GetComponent<Mesh>();
    }
}

shared_ptr<Component> PlayerController::Clone()
{
    return make_shared<PlayerController>(*this);
}

void PlayerController::Update()
{
    if (auto mesh = _mesh.lock())
    {
        if (mesh->GetBody())
        {
            if (!mesh->GetBody()->GetWorld()->IsLocked())
            {
                if (shoot < shootDelay)
                    shoot += DeltaTime();

                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    if (!isFiring && shoot >= shootDelay)
                    {
                        shoot = 0;
                        isFiring != isFiring;
                        sf::Vector2f currentPosition = GetMouseCoordinates();
                        this->ShootIn(sf::Vector2f(currentPosition.x, currentPosition.y));
                    }
                }
            }
        }
    }
}

void PlayerController::FixedUpdate()
{
    if (auto mesh = _mesh.lock())
    {
        if (mesh->GetBody())
        {
            if (!mesh->GetBody()->GetWorld()->IsLocked())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                {
                    this->MoveIn(sf::Vector2f(0, -speed));
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                {
                    this->MoveIn(sf::Vector2f(0, speed));
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                {
                    this->MoveIn(sf::Vector2f(-speed, 0));
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                {
                    this->MoveIn(sf::Vector2f(speed, 0));
                }
            }
        }
    }
}

json PlayerController::Serialize()
{
    json jsonPc;
    jsonPc["type"] = "PlayerController";
    jsonPc["speed"] = speed;
    jsonPc["shootDelay"] = shootDelay;
    jsonPc["bulletForce"] = bulletForce;
    jsonPc["soundKey"] = soundBufferKey;
    return jsonPc;
}

void PlayerController::Deserialize(json jsonPc)
{
    speed = jsonPc["speed"];
    shootDelay = jsonPc["shootDelay"];
    bulletForce = jsonPc["bulletForce"];
    soundBufferKey = jsonPc["soundKey"];
}

void PlayerController::MoveIn(sf::Vector2f position)
{
    if (auto mesh = _mesh.lock())
    {
        mesh->TransformPosition(position * 0.02f);
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

            auto bullet = owner->GetScene().lock()->CreateGameObject("BULLET");
            bullet->transform.position = owner->transform.position + direction * 30.f;

            auto shape = make_shared<sf::CircleShape>(2.f);
            shape->setOrigin(1.f, 1.f);
            shape->setFillColor(sf::Color::Yellow);

            auto mesh = make_shared<eng::Mesh>();

            bullet->AddComponent(mesh);
            mesh->physEnable = true;
            mesh->Configure(shape);

            b2Body *bulletBody = mesh->GetBody();
            bulletBody->SetBullet(true);

            bullet->AddComponent(make_shared<Bullet>());
            mesh->AddImpulse(direction * bulletForce);

            if (soundBufferKey != "")
            {
                pew.stop();
                pew.setBuffer(*ResourceManager::Get().GetSound(soundBufferKey).lock()->sf_soundBuffer.get());
                pew.play();
            }
            isFiring != isFiring;
        }
    }
}

void PlayerController::DrawEditor()
{

    ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)ImColor::HSV(0.4f, 0.9f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, (ImVec4)ImColor::HSV(0.5f, 1.f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, (ImVec4)ImColor::HSV(0.3f, 1.f, 0.5f));
    if (ImGui::CollapsingHeader("PlayerController"))
    {
        ImGui::PopStyleColor(3);
        ImGui::InputFloat("Player speed: ", &speed);
        ImGui::InputFloat("Shoot Delay", &shootDelay);
        ImGui::InputFloat("Bullet Force", &bulletForce);
        ImGui::Text(("Selected sound " + soundBufferKey).c_str());
        
        if (ImGui::Button("Choose Sound"))
        {
            OpenFileDialog("Resource", this);
        }
        if (OFD_Status())
        {
            if (auto sound = ResourceManager::Get().LoadSound(*GetOFD_Result()).lock())
            {
                soundBufferKey = sound->key;
            }
        }
    }
    else
    {
        ImGui::PopStyleColor(3);
    }
}