#include "IPlug.h"
#include "Button.h"

#define BUTTON_PIN 4

Button button(BUTTON_PIN);

button.begin();

class PlugButton : public IPlug
{
    bool state;// not needed anymore?

public:
    PlugButton(/*bool State*/)
    {
        //state = State;
    }
    ~PlugButton() {}

    bool isPlugged()
    {
        if (button.pressed)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};