#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <string>

class Message
{
public:
    Message();
    Message(std::string contents);

    ~Message();

    void show();

private:
    std::string contents;
};

#endif
