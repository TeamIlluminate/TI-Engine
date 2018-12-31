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
  void SetShader(shared_ptr<sf::Shader> shader);
  bool *GetEnabled() {return &isEnabled;};
  bool isEnabled = false;
private:
  shared_ptr<sf::Shape> currentShape;
  shared_ptr<sf::Shader> currentShader;
  sf::RenderStates states;
};
} // namespace eng