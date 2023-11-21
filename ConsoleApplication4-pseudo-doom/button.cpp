#include <iostream>
#include "button.h"

Button::Button(int x, int y, int w, int h) : x(x), y(y), w(w), h(h)
{

}

bool Button::is_mouse_over(int mouse_x, int mouse_y)
{
	bool is_over = mouse_x < x || mouse_y < y || mouse_x > x + w || mouse_y > y + h ? false : true;
    return is_over;
}