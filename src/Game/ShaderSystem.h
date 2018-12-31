#include "Component.h"
#include "GameObject.h"
#include "Components/ShapeMesh.h"
#include "imgui.h"
#include <SFML/Graphics.hpp>
#pragma once
namespace gm
{
class ShaderManager : public eng::Component 
{
    public:
    void OnInit();
    void GUI();
    private:
    void GetGameObjectsWithShader();
    void ApplyShaderTo(list<shared_ptr<eng::ShapeMesh>> shaders);
    ImGuiInputTextFlags flags;
    char code[1024*16] = 
    "#version 130\n"
    "out vec4 colorOut;\n"
            "void main() { \n"
            "colorOut = vec4(1.0, 0.0, 0.0, 1.0);\n"
            " \n"
            "}";
    list<weak_ptr<eng::ShapeMesh>> meshs;

};

} // namespace eng

