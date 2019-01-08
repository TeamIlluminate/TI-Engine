#include "Camera.h"
#include "imgui.h"
#include "GameObject.h"

using namespace eng;

// Camera::Camera(const Camera & camera) {

//     this->maxScale = camera.maxScale;
//     this->minScale = camera.minScale;
//     this->scale = camera.scale;
//     this->view = camera.view;
//     this->isEnabled = camera.isEnabled;
// }

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

void Camera::DrawEditor() {
    auto gameObject = _owner.lock();
    if (gameObject) {
        int id = gameObject->id;
        if (ImGui::TreeNode("Camera of " + id)) {
            ImGui::InputInt("maxScale#" + id, &maxScale );
            ImGui::InputInt("minScale#" + id, &minScale );
            ImGui::InputInt("scale#" + id, &scale );
            ImGui::Checkbox("Is enabled#" +id, &isEnabled);
            ImGui::TreePop();
        }
    }
}

