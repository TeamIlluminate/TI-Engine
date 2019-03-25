#include "Control.h"
#pragma once

namespace eng
{

class Grid : public Control
{
  public:

    Grid(sf::Vector2f _size, string _name, sf::Vector2f _position, ImGuiWindowFlags flags = 0);
    Grid(ImGuiWindowFlags = 0);

    void Draw();

    void Show();
    void Hide();
    void Close();

    // void ShowDialog(); // Пока открыта эта форма(дочерняя), родительская форма блокируется.

    void AddControl(shared_ptr<Control> control);
    void RemoveControl(shared_ptr<Control> control);
    ~Grid();

  protected:
    list<shared_ptr<Control>> controls;
    ImGuiWindowFlags flags;
    int cid = -1;
};
} // namespace eng