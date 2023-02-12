#ifndef PIXPLANEFMCCDU_SRC_X86_KeypadScanner_H_
#define PIXPLANEFMCCDU_SRC_X86_KeypadScanner_H_

#include <syslog.h>
#include <map>
#include <utility>
//#include "KeypadScanner.h"

class KeypadScanner 
{
private:
    KeypadScanner();
    //KeypadScanner(KeypadScanner const &) {};
    /*KeypadScanner &operator=(KeypadScanner const &)
    {
        abort();
    }*/
    std::map<char, std::pair<int, int>> keyMapping;
protected:
    bool isRunning;
    bool stopRequested;

    unsigned int pressedRow;
    unsigned int pressedCol;
    bool somethingPressed;
    static KeypadScanner * instance;
public:
    static KeypadScanner *getInstance()
    {
        if (!instance)
        {
            syslog(LOG_INFO, "Intantiating KeypadScanner");
            instance = new KeypadScanner();
        }
        return instance;
    }

    void launchThread();
    void mainLoop();

    virtual ~KeypadScanner();
};
#endif