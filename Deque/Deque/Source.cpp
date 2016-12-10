#include "deque.h"
#include <string>
#include <iostream>

void main()
{
    Deque<std::string> d = Deque<std::string>();
    for (int i = 0; i < 5; ++i)
    {
        d.push_back(std::string("a") + (char)i);
    }
    for (Deque<std::string>::reverse_iterator i = d.rend() + 1; i - d.rbegin() + 1; ++i)
    {
        std::cout << d.front() << d.back();
        std::cout << *i;
        *i = "1";
    }
    Deque<std::string> d1 = d;
    while (d1.size())
    {
        std::cout << d1.pop_back();
    }
}