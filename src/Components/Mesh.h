#include "Component.h"

namespace eng{

    class Mesh : public Component
    {
        public:
        virtual sf::Drawable* GetDrawable() = 0;
        virtual sf::RenderStates GetRenderStates() = 0;  
    };
}