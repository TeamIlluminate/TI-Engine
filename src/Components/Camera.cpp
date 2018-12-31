#include "Camera.h"
#include "imgui.h"
#include "GameObject.h"

using namespace eng;

void Camera::OnInit()
{
    view.setSize(scale * 16, scale * 9);
}

void Camera::Update()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
    {
        if (scale < maxScale)
        {
            scale++;
            view.setSize(scale * 16, scale * 9);
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
    {
        if (scale > minScale)
        {
            scale--;
            view.setSize(scale * 16, scale * 9);
        }
    }
    if (auto _owner = owner.lock())
    {
        view.setCenter(_owner->transform.position);
    }
}

void Camera::DrawEditor() {
    auto gameObject = owner.lock();
    if (gameObject) {
        int id = gameObject->id;
        if (ImGui::TreeNode("Camera's of " + id)) {
            ImGui::InputInt("maxScale#" + id, &maxScale );
            ImGui::InputInt("minScale#" + id, &minScale );
            ImGui::InputInt("scale#" + id, &scale );
            ImGui::Checkbox("Is enabled#" +id, &isEnabled);
            ImGui::TreePop();
        }
    }
}