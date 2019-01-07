#include "Game/ShaderSystem.h"
using namespace gm;

void ShaderManager::OnInit()
{
    flags = ImGuiInputTextFlags_AllowTabInput;

    GetGameObjectsWithShader();
}

void ShaderManager::GUI()
{
    ImGui::Begin("ShaderManager");
    ImGui::InputTextMultiline("##source", code, IM_ARRAYSIZE(code), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), flags);
    if (ImGui::Button("Apply shader to selected"))
    {
        for (auto weakObject : meshs)
        {
            if (auto shader = weakObject.lock())
            {
                list<shared_ptr<eng::ShapeMesh>> shaderControllers;
                bool isNeeded = false;
                if (shader->isEnabled)
                {
                    isNeeded = true;
                    shaderControllers.push_back(shader);
                }
                if (isNeeded)
                    ApplyShaderTo(shaderControllers);
            }
        }
    };
    ImGui::Columns(2, "ShaderGameObject", true);
    ImGui::Text("name  ");
    ImGui::NextColumn();
    ImGui::Text("shader");
    ImGui::NextColumn();
    ImGui::Separator();

    for (auto weakMesh : meshs)
    {
        if (auto shader = weakMesh.lock())
        {
            ImGui::Text(shader->_owner.lock()->GetName().c_str());
            ImGui::NextColumn();

            if(ImGui::Checkbox(to_string(shader->_owner.lock()->id).c_str(), shader->GetEnabled()))
            {
                shader->isEnabled != shader->isEnabled;
            }
            ImGui::NextColumn();
        }
    }
    ImGui::End();
}

void ShaderManager::ApplyShaderTo(list<shared_ptr<eng::ShapeMesh>> shaderControllers)
{
    shared_ptr<sf::Shader> shader = make_shared<sf::Shader>();
    if (shader->loadFromMemory(code, sf::Shader::Fragment))
    {

        for (auto shaderController : shaderControllers)
        {
            cout << shaderController->_owner.lock()->GetName();
            shaderController->SetShader(shader);
        }
    }
}

void ShaderManager::GetGameObjectsWithShader()
{
    if (auto owner = _owner.lock())
    {
        auto allGameObjects = owner->GetScene().lock()->GetGameObjects();
        meshs.clear();
        for (auto concreteGameObject : allGameObjects)
        {
            auto _concreteGameObject = concreteGameObject.lock();
            auto shaderConroller = _concreteGameObject->GetComponent<eng::ShapeMesh>().lock();
            if (shaderConroller)
            {
                meshs.push_back(shaderConroller);
            }
        }
    }
}

shared_ptr<eng::Component> ShaderManager::Clone()
{
    return make_shared<ShaderManager>(*this);
}