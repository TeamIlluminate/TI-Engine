#include "../Utils.h"
#pragma once

namespace eng
{
class Control
{
  public:
    Control(sf::Vector2f _size, string _name, sf::Vector2f _position);
    virtual void Draw() = 0;

    string GetName();
    void SetID(string _id);

  protected:
    sf::Vector2f size;
    sf::Vector2f position;
    string name;
    string id;

    bool isVisible;
    bool isEnabled;
};

} // namespace eng