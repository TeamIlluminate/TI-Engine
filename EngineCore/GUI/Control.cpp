#include "Control.h"

using namespace eng;

Control::Control(sf::Vector2f _size, string _name, sf::Vector2f _position) : size(_size), name(_name), position(_position){};

string Control::GetName() { return name; };
void Control::SetID(string _id) { id = _id; };