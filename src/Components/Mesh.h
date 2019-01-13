#include "Utils.h"
#include "Component.h"

#pragma once
namespace eng
{

class Mesh : public Component
{
public:
  friend class Render;

  ~Mesh();

  enum MeshType : int
  {
    Circle = 0,
    Rectangle = 1,
    Sprite = 2
  };

  b2Body *GetBody();
  MeshType GetType();

  shared_ptr<Component> Clone();

private:
  weak_ptr<sf::Drawable> GetDrawable()
  {
    if (auto owner = _owner.lock())
    {
      shape->setPosition(owner->GetGlobalCoordinates());
      return shape;
    }
  };

  sf::RenderStates GetRenderStates()
  {
    return renderStates;
  };

  void OnInit();
  void DrawEditor();
  b2Body *body;
  b2Fixture *fixture;

  sf::RenderStates renderStates = sf::RenderStates::Default;

  int type = 0;
  shared_ptr<sf::Shape> shape = make_shared<sf::CircleShape>();

  shared_ptr<sf::Sprite> LoadSprite(string file);
  shared_ptr<sf::Texture> texture = make_shared<sf::Texture>();

  void EditorCircle();
  void EditorRect();
  void EditorSprite();
};
} // namespace eng