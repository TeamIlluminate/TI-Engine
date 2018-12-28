#include "Component.h"
#include "PhysBody.h"

namespace eng
{

class Camera : public Component
{
  public:
    bool GetStatus() const;
    void SetStatus();
    void OnInit();
    void GUI();

  private:
};
} // namespace eng