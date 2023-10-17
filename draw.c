/** @file   draw.c
    @author Yunu Cho, Owen Zhou
    @date   10 October 2023
    @brief  Functions related to the LED matrix
*/

#include "draw.h"

/** Displays the ball on the LED matrix.
    @param ball pointer of the ball to be displayed.  */
void display_ball(Ball_t* ball)
{
    tinygl_point_t p1 = {ball->x, ball->y};

    tinygl_draw_point(p1, 1);
}

/** Displays the paddle on the LED matrix.
    @param paddle pointer of the paddle to be displayed.  */
void display_paddle(Paddle_t* paddle)
{
    tinygl_point_t p1 = {paddle->x, paddle->y-1};
    tinygl_point_t p2 = {paddle->x, paddle->y+1};

    tinygl_draw_line(p1, p2, 1);
}

/** Displays a single character on the LED matrix.
    @param c the character to be displayed.  */
void display_character(char c)
{
    char buffer[2];
    buffer[0] = c;
    buffer[1] = '\0';

    tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
    tinygl_text(buffer);
}

/** Displays scrolling text on the LED matrix.
    @param text the text to be displayed.  */
void display_text(char text[])
{
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text(text);
}
