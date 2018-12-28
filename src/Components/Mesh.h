#include "Utils.h"
#include "Component.h"

#pragma once
namespace eng
{

class Mesh : public Component
{
  public:
    virtual weak_ptr<sf::Drawable> GetDrawable() = 0;
    virtual sf::RenderStates GetRenderStates() = 0;
};
} // namespace eng