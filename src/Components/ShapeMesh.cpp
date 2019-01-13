#include "ShapeMesh.h"
#include "GameObject.h"

using namespace eng;

ShapeMesh::ShapeMesh(shared_ptr<sf::Shape> shape) : currentShape(shape)
{
}

weak_ptr<sf::Drawable> ShapeMesh::GetDrawable()
{
    if (auto owner = _owner.lock())
    {
        currentShape->setPosition(owner->GetGlobalCoordinates());
    }
    return (weak_ptr<sf::Drawable>)currentShape;
}

shared_ptr<Component> ShapeMesh::Clone()
{
    return make_shared<ShapeMesh>(*this);
}

sf::RenderStates ShapeMesh::GetRenderStates() { return states; }

void ShapeMesh::SetShader(shared_ptr<sf::Shader> shader)
{
    currentShader = shader;
    states.shader = shader.get();
}

void ShapeMesh::DrawEditor()
{
    auto gameObject = _owner.lock();
    if (gameObject)
    {

    ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4) ImColor::HSV(0.7f, 0.9f, 0.5f)); 
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, (ImVec4)ImColor::HSV(0.7f, 1.f, 0.5f)); 
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered    , (ImVec4)ImColor::HSV(0.3f, 1.f, 0.5f)); 
        if (ImGui::CollapsingHeader("ShapeMesh"))
        {
            ImGui::PopStyleColor(3);
            DrawSfShape(currentShape);
        } else {
            ImGui::PopStyleColor(3);
        }
    }
}