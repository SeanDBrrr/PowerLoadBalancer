#include "IPlug.h"

class button : public IPlug
{
private:
int buttonPin;
bool state;
public:
    button(int ButtonPin, bool State)
    {
        buttonPin = ButtonPin;
        state = State;
    }
    ~button(){}

    bool isPlugged()
    {
        //return readButton();
        return false;
    }
};

bool readButton()
{
    //readTheButton
    bool blahblah = false;
    return blahblah;
}