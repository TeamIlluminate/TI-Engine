#include "Camera.h"
#include "imgui.h"
#include "GameObject.h"

using namespace eng;

shared_ptr<Component> Camera::Clone()
{
    return make_shared<Camera>(*this);
}

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
    if (auto owner = _owner.lock())
    {
        view.setCenter(owner->transform.position);
    }
}

void Camera::DrawEditor()
{
    if (ImGui::CollapsingHeader("Camera"))
    {
        ImGui::InputInt("maxScale", &maxScale);
        ImGui::InputInt("minScale", &minScale);
        ImGui::InputInt("scale", &scale);
        if (scale < minScale)
        {
            scale = minScale;
        }
        else if (scale > maxScale)
        {
            scale = maxScale;
        }
        view.setSize(scale * 16, scale * 9);
        ImGui::Checkbox("Is enabled", &isEnabled);
    }
}
