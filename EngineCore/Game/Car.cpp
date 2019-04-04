#include "Car.h"
#include "../ResourceManager.h"

void Car::OnInit()
{
    if (auto owner = _owner.lock())
    {
        _mesh = owner->GetComponent<eng::Mesh>();
        _mesh.lock()->GetBody()->SetLinearDamping(deltaSpeed / 2.f);
        _mesh.lock()->GetBody()->SetAngularDamping(1.f);
    }
}

shared_ptr<eng::Component> Car::Clone()
{
    auto clone = make_shared<Car>(*this);
    if (skeyIdle != "")
        clone->soundIdle.setBuffer(*eng::ResourceManager::Get().GetSound(skeyIdle).lock()->sf_soundBuffer.get());
    if (skeyStart != "")
        clone->soundStart.setBuffer(*eng::ResourceManager::Get().GetSound(skeyStart).lock()->sf_soundBuffer.get());
    if (skeyForce != "")
        clone->soundForce.setBuffer(*eng::ResourceManager::Get().GetSound(skeyForce).lock()->sf_soundBuffer.get());
    return make_shared<Car>(*this);
}

void Car::Update()
{
    if (auto mesh = _mesh.lock())
    {
        auto velocity = mesh->GetBody()->GetLinearVelocity();
        if (velocity.Length() < maxSpeed / mesh->physicsCoef)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                if (velocity.Length() == 0 && soundStart.getStatus() != soundStart.Playing)
                    soundStart.play();
                
                mesh->AddImpulse(_owner.lock()->GetForwardVector() * deltaSpeed);

                if (soundForce.getStatus() != soundForce.Playing)
                {
                    soundIdle.stop();
                    soundForce.play();
                }
            }
            else
            {
                if (soundForce.getStatus() == soundForce.Playing)
                {
                    soundForce.stop();
                }

                if (soundIdle.getStatus() != soundIdle.Playing)
                {
                    soundIdle.play();
                }
            }
        }
    }
}

void Car::DrawEditor()
{
    if (ImGui::CollapsingHeader("Car"))
    {
        ImGui::InputFloat("maxSpeed", &maxSpeed);
        ImGui::InputFloat("deltaSpeed", &deltaSpeed);

        static int type = 0;
        ImGui::Text(("Idle: " + skeyIdle).c_str());
        ImGui::Text(("Start: " + skeyStart).c_str());
        ImGui::Text(("Force: " + skeyForce).c_str());
        ImGui::Combo("Sound", &type, "IDLE\0START\0FORCE\0");
        if (ImGui::Button("Choose Sound"))
        {
            OpenFileDialog("Resource", this);
        }
        if (OFD_Status())
        {
            if (auto sound = eng::ResourceManager::Get().LoadSound(*GetOFD_Result()).lock())
            {
                if (type == 0)
                {
                    skeyIdle = sound->key;
                    soundIdle.setBuffer(*sound->sf_soundBuffer.get());
                }
                else if (type == 1)
                {
                    skeyStart = sound->key;
                    soundStart.setBuffer(*sound->sf_soundBuffer.get());
                }
                else if (type == 2)
                {
                    skeyForce = sound->key;
                    soundForce.setBuffer(*sound->sf_soundBuffer.get());
                }
            }
        }
    }
}

json Car::Serialize()
{
    json j;
    j["type"] = "Car";
    j["maxSpeed"] = maxSpeed;
    j["deltaSpeed"] = deltaSpeed;
    j["keyForce"] = skeyForce;
    j["keyIdle"] = skeyIdle;
    j["keyStart"] = skeyStart;
    return j;
}

void Car::Deserialize(json j)
{
    eng::DeserializeVar(maxSpeed, j["maxSpeed"]);
    eng::DeserializeVar(deltaSpeed, j["deltaSpeed"]);
    
    eng::DeserializeVar(skeyForce, j["keyForce"]);   
    eng::DeserializeVar(skeyIdle, j["keyIdle"]);
    eng::DeserializeVar(skeyStart, j["keyStart"]);

    if (skeyIdle != "")
        this->soundIdle.setBuffer(*eng::ResourceManager::Get().GetSound(skeyIdle).lock()->sf_soundBuffer.get());
    if (skeyStart != "")
        this->soundStart.setBuffer(*eng::ResourceManager::Get().GetSound(skeyStart).lock()->sf_soundBuffer.get());
    if (skeyForce != "")
        this->soundForce.setBuffer(*eng::ResourceManager::Get().GetSound(skeyForce).lock()->sf_soundBuffer.get());
}