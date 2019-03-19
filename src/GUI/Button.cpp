#include "Button.h"
#include "Grid.h"
#include "Panel.h"

using namespace eng;

Button::Button(string name, sf::Vector2f size) : Control(size, name, sf::Vector2f(0, 0))
{

}

Button::~Button()
{

}

void Button::Draw()
{
    if(ImGui::Button((name + "###" + id).c_str(), size))
    {
        clickHandler();
    }
}

void Button::SetClickHandler(voidDelegate handler)
{
    clickHandler = handler;
}