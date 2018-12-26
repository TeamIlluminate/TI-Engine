#include "Component.h"
#include "PhysBody.h"

namespace eng{

class Camera : public Component
{
    public:

    bool GetStatus() const;
    void SetStatus();
    void OnInit();
    void GUI();
    private:

    sf::Color bgColor;
    float forceX = 0;
    float forceY = 0;
    char windowTitle[255] = "ImGui + SFML = <3";

    bool status = false;
    PhysBody * physBody;

};
}