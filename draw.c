#include "draw.h"

void display_ball(Ball_t ball)
{
    tinygl_point_t p1 = {ball.x, ball.y};
    tinygl_draw_point(p1, 1);
}
void display_paddle(Paddle_t paddle)
{
    tinygl_point_t p1 = {paddle.x, paddle.y-1};
    tinygl_point_t p2 = {paddle.x, paddle.y+1};
    tinygl_draw_line(p1, p2, 1);
}

void display_text(char text[])
{
    tinygl_text(text);
}

void display_character(char c)
{
    char buffer[2];

    buffer[0] = c;
    buffer[1] = '\0';
    tinygl_text(buffer);
}
