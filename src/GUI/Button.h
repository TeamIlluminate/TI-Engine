#include "Control.h"

namespace eng
{
    class Button : public Control
    {
        public:
        
        Button(string name, sf::Vector2f size);
        void Draw();

        void SetClickHandler(voidDelegate handler);

        ~Button();

        private:

        voidDelegate clickHandler;
        
    };
}