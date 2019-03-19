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
    if (auto owner = _owner.lock()) {
        owner->GetScene().lock()->RegCamera(shared_from_this());
    }
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

json Camera::Serialize()
{
    json jCamera;
    jCamera["type"] = "Camera";
    jCamera["maxScale"] = maxScale;
    jCamera["minScale"] = minScale;
    jCamera["scale"] = scale;
    return jCamera;
}

void Camera::Deserialize(json obj)
{
    eng::DeserializeVar(maxScale, obj["maxScale"]);
    eng::DeserializeVar(minScale, obj["minScale"]);
    eng::DeserializeVar(scale, obj["scale"]);
    
    view.setSize(scale * 16, scale * 9);
}
