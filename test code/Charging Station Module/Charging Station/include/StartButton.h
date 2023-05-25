#include "IStart.h"
#include "Button.h"

class StartButton : public IStart
{
private:
    SinglePressButton *_button;

public:
    StartButton();
    ~StartButton();

    bool isStarted();
};