#include "Component.h"

namespace eng{

class Camera : public Component
{
    public:

    bool GetStatus() const;
    void SetStatus();

    private:

    bool status = false;
    
};

}