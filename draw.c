#include "draw.h"

void display_ball(Ball_t ball)
{
}
void display_paddle(Paddle_t paddle)
{
}

void display_text(char text[])
{
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text(text);
}