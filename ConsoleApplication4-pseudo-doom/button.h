#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>

class Button
{
	public:
        Button(int x, int y, int w, int h);
        bool is_mouse_over(int mouse_x, int mouse_y); // returns true if the specified coordinates are inside the button

        //forbids copy constructor to avoid warning in c++11
        Button(const Button& b) : Button(b.x, b.y, b.w, b.h) {};
        Button& operator=(const Button& b) = delete;
    private:
        int x, y, w, h;
};

#endif