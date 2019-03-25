#include "Grid.h"
#include "GameMaster.h"

using namespace eng;

Grid::Grid(sf::Vector2f _size, string _name, sf::Vector2f _position, ImGuiWindowFlags flags) : Control(_size, _name, _position)
{
    flags = flags;
    id = GameMaster::Get().GetGID();
}

Grid::Grid(ImGuiWindowFlags flags) : Control(sf::Vector2f(100, 100), "NewGrid", sf::Vector2f(0, 0))
{
    flags = flags;
    id = GameMaster::Get().GetGID();
}

Grid::~Grid()
{

}

void Grid::AddControl(shared_ptr<Control> control)
{
    cid++;
    control->SetID(id + to_string(cid));
    controls.push_back(control);
}

void Grid::RemoveControl(shared_ptr<Control> control)
{
    controls.remove(control);
}

void Grid::Close()
{
    
}

void Grid::Hide()
{
    isVisible = false;
}

void Grid::Show()
{
   
}

void Grid::Draw()
{
    ImGui::Begin((name + "###" + id).c_str(), nullptr, size, -1.f, flags);
    {
        for (auto guiElement : controls)
        {
            guiElement->Draw();
        }
    }
    ImGui::End();
}