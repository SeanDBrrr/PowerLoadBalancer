#include "IPlug.h"
#include "Button.h"

class PlugButton : public IPlug
{
private:
    Button *_button;

public:
    PlugButton();
    ~PlugButton();

    bool isPlugged();
};