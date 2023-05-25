#include "IPlug.h"
#include "Button.h"

class PlugButton : public IPlug
{
private:
    SinglePressButton *_button;

public:
    PlugButton();
    ~PlugButton();

    bool isPlugged();
};