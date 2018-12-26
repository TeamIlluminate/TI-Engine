#include "Camera.h"
#include "imgui.h"
#include "GameObject.h"

using namespace eng;

void Camera::OnInit()
{
    physBody = this->attached->GetComponent<PhysBody>();
}

void Camera::GUI()
{
    std::string name = "This is " + attached->GetName() + "!";

    ImGuiWindowFlags flag = 0;
    flag |= ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize;
    flag |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin(name.c_str(), NULL, flag);

    ImGui::SetWindowPos(this->attached->transform.position);
    ImGui::SetNextWindowBgAlpha(0);

    ImGui::LabelText("", name.c_str());

    ImGui::InputFloat("Float X", &forceX, 100.f, 1000.f);
    ImGui::InputFloat("Float Y", &forceY, 0.05f, 0.1f);

    if (ImGui::Button("PUSH ME"))
    {

        sf::Vector2f nextPos = this->attached->transform.position + sf::Vector2f(forceX, forceY);
        if (!physBody->RayCast(nextPos * 50.f))
        {

            this->physBody->body->SetTransform(b2Vec2(nextPos.x, nextPos.y), 0);
        }

        //   physBody->AddImpulse(sf::Vector2f(forceX, forceY));
    }

    ImGui::End(); // end window
}
