#include "Utils.h"
#include "Component.h"

#pragma once
namespace eng
{

//[Serializable]
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

  bool physEnable = false;

  b2Body *GetBody() {
    return body;
  }
  MeshType GetType();
  void Configure(shared_ptr<sf::Shape> shape);

  shared_ptr<Component> Clone();


//Physics control center
    weak_ptr<GameObject> RayCast(sf::Vector2f to);
    void AddImpulse(sf::Vector2f vector);
    void TransformPosition(sf::Vector2f pos);

  json Serialize();
  void Deserialize(json j);

private:
void FixedUpdate();
  void CreatePhysics();

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
  b2Body *body = nullptr;
  b2Fixture *fixture = nullptr;

  sf::RenderStates renderStates = sf::RenderStates::Default;

  int type = 0;
  shared_ptr<sf::Shape> shape;

  shared_ptr<sf::Sprite> LoadSprite(string file);
  shared_ptr<sf::Texture> texture = make_shared<sf::Texture>();



  void EditorCircle();
  void EditorRect();
  void EditorSprite();
  void EditorPhysics();
};
} // namespace eng