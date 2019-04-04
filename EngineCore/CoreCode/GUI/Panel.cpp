#include "Panel.h"
#include "../GameMaster.h"
using namespace eng;

Panel::Panel(sf::Vector2f _size, string _name, sf::Vector2f _position, ImGuiWindowFlags flags) : Grid(_size, _name, _position, flags)
{
    id = GameMaster::Get().GetGID();
}

Panel::Panel(ImGuiWindowFlags flags) : Grid(sf::Vector2f(100, 100), "NewGrid", sf::Vector2f(0, 0), flags)
{
    id = GameMaster::Get().GetGID();
}

Panel::~Panel()
{

}

void Panel::Draw()
{
    ImGui::BeginChild(name.c_str(), size, border, flags);
    for(auto ct : controls)
    {
        ct->Draw();
    }
    ImGui::EndChild();
}

void Panel::SetBorder(bool isBordered)
{
    border = isBordered;
}

bool Panel::IsBordered() const
{
    return border;
}

void Panel::Hide()
{

}

void Panel::Close()
{

}

void Panel::Show()
{

}
