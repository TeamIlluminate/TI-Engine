#include "Utils.h"
#include "Mesh.h"
#pragma once
namespace eng
{
class ShapeMesh : public Mesh
{
public:
  ShapeMesh(sf::Shape *shape);
  sf::Drawable *GetDrawable();
  sf::RenderStates GetRenderStates();

private:
  sf::Shape *currentShape;
};
} // namespace eng