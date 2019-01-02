#include "Utils.h"
#include <thread>

namespace eng
{

class Scene;
class GameObject;

class Render
{
public:
  Render(sf::VideoMode mode);

  void SetScene(weak_ptr<Scene> _scene);

private:
  shared_ptr<sf::RenderWindow> window;

  shared_ptr<std::thread> wThread;
  weak_ptr<Scene> _currentScene;

  void WindowLoop();
  void Draw(shared_ptr<GameObject> object);

  bool isEditor = false;
};

} // namespace eng