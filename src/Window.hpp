#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <vector>

#include "Message.hpp"

class Window
{
public:
    Window();
    ~Window();

private:
    std::vector<Message> messages;

    void hide();
    void setFullScreen();
};

#endif
