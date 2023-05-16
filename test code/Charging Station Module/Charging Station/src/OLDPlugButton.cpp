#include "IPlug.h"
#include "Button.h"

#define BUTTON_PIN 4

class PlugButton : public IPlug
{
private:
    Button *_button;
public:
    PlugButton()
    {
        _button = new Button(BUTTON_PIN);
    }
    ~PlugButton() 
    {
        delete _button;
    }

    bool isPlugged()
    {
        return _button->toggle();
    }
};