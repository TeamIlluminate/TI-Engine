#include "Utils.h"
#include "Mesh.h"
#pragma once
namespace eng
{
class ShapeMesh : public Mesh
{
public:
  ShapeMesh(shared_ptr<sf::Shape> shape);
  weak_ptr<sf::Drawable> GetDrawable();
  sf::RenderStates GetRenderStates();

private:
  shared_ptr<sf::Shape> currentShape;
};
} // namespace eng