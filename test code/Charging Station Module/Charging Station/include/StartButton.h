#include "IStart.h"
#include "Button.h"

class StartButton : public IStart
{
private:
    Button *_button;

public:
    StartButton();
    ~StartButton();

    bool isStarted();
};